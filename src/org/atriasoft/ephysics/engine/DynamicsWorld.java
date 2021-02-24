package org.atriasoft.ephysics.engine;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.atriasoft.ephysics.Configuration;
import org.atriasoft.ephysics.body.BodyType;
import org.atriasoft.ephysics.body.CollisionBody;
import org.atriasoft.ephysics.body.RigidBody;
import org.atriasoft.ephysics.collision.ContactManifold;
import org.atriasoft.ephysics.collision.ContactManifoldListElement;
import org.atriasoft.ephysics.collision.ContactManifoldSet;
import org.atriasoft.ephysics.collision.ProxyShape;
import org.atriasoft.ephysics.collision.broadphase.DTree;
import org.atriasoft.ephysics.constraint.BallAndSocketJoint;
import org.atriasoft.ephysics.constraint.BallAndSocketJointInfo;
import org.atriasoft.ephysics.constraint.ContactsPositionCorrectionTechnique;
import org.atriasoft.ephysics.constraint.FixedJoint;
import org.atriasoft.ephysics.constraint.FixedJointInfo;
import org.atriasoft.ephysics.constraint.HingeJoint;
import org.atriasoft.ephysics.constraint.HingeJointInfo;
import org.atriasoft.ephysics.constraint.Joint;
import org.atriasoft.ephysics.constraint.JointInfo;
import org.atriasoft.ephysics.constraint.JointListElement;
import org.atriasoft.ephysics.constraint.JointsPositionCorrectionTechnique;
import org.atriasoft.ephysics.constraint.SliderJoint;
import org.atriasoft.ephysics.constraint.SliderJointInfo;
import org.atriasoft.ephysics.internal.Log;
import org.atriasoft.ephysics.mathematics.Set;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Quaternion;
import org.atriasoft.etk.math.Transform3D;
import org.atriasoft.etk.math.Vector3f;

/**
 *  This class represents a dynamics world. This class inherits from
 * the CollisionWorld class. In a dynamics world, bodies can collide
 * and their movements are simulated using the laws of physics.
 */
public class DynamicsWorld extends CollisionWorld {
	private static int kkk = 0;
	protected ContactSolver contactSolver; //!< Contact solver
	protected ConstraintSolver raintSolver; //!< Constraint solver
	protected int nbVelocitySolverIterations; //!< Number of iterations for the velocity solver of the Sequential Impulses technique
	protected int nbPositionSolverIterations; //!< Number of iterations for the position solver of the Sequential Impulses technique
	protected boolean isSleepingEnabled; //!< True if the spleeping technique for inactive bodies is enabled
	protected List<RigidBody> rigidBodies = new ArrayList<>(); //!< All the rigid bodies of the physics world
	protected List<Joint> joints = new ArrayList<>(); //!< All the joints of the world
	protected Vector3f gravity = new Vector3f(); //!< Gravity vector of the world
	public float timeStep; //!< Current frame time step (in seconds)
	protected boolean isGravityEnabled; //!< True if the gravity force is on
	protected Vector3f[] rainedLinearVelocities; //!< Array of rained linear velocities (state of the linear velocities after solving the constraints)
	protected Vector3f[] rainedAngularVelocities; //!< Array of rained angular velocities (state of the angular velocities after solving the constraints)
	protected Vector3f[] splitLinearVelocities; //!< Split linear velocities for the position contact solver (split impulse)
	protected Vector3f[] splitAngularVelocities; //!< Split angular velocities for the position contact solver (split impulse)
	protected Vector3f[] rainedPositions; //!< Array of rained rigid bodies position (for position error correction)
	protected Quaternion[] rainedOrientations; //!< Array of rained rigid bodies orientation (for position error correction)
	protected Map<RigidBody, Integer> mapBodyToConstrainedVelocityIndex = new HashMap<RigidBody, Integer>(); //!< Map body to their index in the rained velocities array
	protected List<Island> islands = new ArrayList<>(); //!< Array with all the islands of awaken bodies
	protected int numberBodiesCapacity; //!< Current allocated capacity for the bodies
	
	protected float sleepLinearVelocity; //!< Sleep linear velocity threshold
	protected float sleepAngularVelocity; //!< Sleep angular velocity threshold
	protected float timeBeforeSleep; //!< Time (in seconds) before a body is put to sleep if its velocity becomes smaller than the sleep velocity.
	
	/**
	 *  Constructor
	 * @param gravity Gravity vector in the world (in meters per second squared)
	 */
	public DynamicsWorld(final Vector3f _gravity) {
		super();
		this.contactSolver = new ContactSolver(this.mapBodyToConstrainedVelocityIndex);
		this.raintSolver = new ConstraintSolver(this.mapBodyToConstrainedVelocityIndex);
		this.nbVelocitySolverIterations = Configuration.DEFAULT_VELOCITY_SOLVER_NB_ITERATIONS;
		this.nbPositionSolverIterations = Configuration.DEFAULT_POSITION_SOLVER_NB_ITERATIONS;
		this.isSleepingEnabled = Configuration.SPLEEPING_ENABLED;
		this.gravity = _gravity;
		this.isGravityEnabled = true;
		this.numberBodiesCapacity = 0;
		this.sleepLinearVelocity = Configuration.DEFAULT_SLEEP_LINEAR_VELOCITY;
		this.sleepAngularVelocity = Configuration.DEFAULT_SLEEP_ANGULAR_VELOCITY;
		this.timeBeforeSleep = Configuration.DEFAULT_TIME_BEFORE_SLEEP;
	}
	
	/**
	 *  Add the joint to the list of joints of the two bodies involved in the joint
	 * @param[in,out] _joint Joint to add at the body.
	 */
	protected void addJointToBody(final Joint _joint) {
		if (_joint == null) {
			//Log.warning("Request add null joint");
			return;
		}
		// Add the joint at the beginning of the linked list of joints of the first body
		_joint.getBody1().jointsList = new JointListElement(_joint, _joint.getBody1().jointsList);
		// Add the joint at the beginning of the linked list of joints of the second body
		_joint.getBody2().jointsList = new JointListElement(_joint, _joint.getBody2().jointsList);
	}
	
	/**
	 *  Compute the islands of awake bodies.
	 * An island is an isolated group of rigid bodies that have raints (joints or contacts)
	 * between each other. This method computes the islands at each time step as follows: For each
	 * awake rigid body, we run a Depth First Search (DFS) through the raint graph of that body
	 * (graph where nodes are the bodies and where the edges are the raints between the bodies) to
	 * find all the bodies that are connected with it (the bodies that share joints or contacts with
	 * it). Then, we create an island with this group of connected bodies.
	 */
	protected void computeIslands() {
		final int nbBodies = this.rigidBodies.size();
		// Clear all the islands
		this.islands.clear();
		int nbContactManifolds = 0;
		// Reset all the isAlreadyInIsland variables of bodies, joints and contact manifolds
		for (final RigidBody it : this.rigidBodies) {
			final int nbBodyManifolds = it.resetIsAlreadyInIslandAndCountManifolds();
			nbContactManifolds += nbBodyManifolds;
		}
		for (final Joint it : this.joints) {
			it.isAlreadyInIsland = false;
		}
		// Create a stack (using an array) for the rigid bodies to visit during the Depth First Search
		final List<RigidBody> stackBodiesToVisit = new ArrayList<>(nbBodies);
		for (int iii = 0; iii < nbBodies; iii++) {
			stackBodiesToVisit.add(null);
		}
		
		// For each rigid body of the world
		for (final RigidBody body : this.rigidBodies) {
			// If the body has already been added to an island, we go to the next body
			if (body.isAlreadyInIsland) {
				continue;
			}
			// If the body is static, we go to the next body
			if (body.getType() == BodyType.STATIC) {
				continue;
			}
			// If the body is sleeping or inactive, we go to the next body
			if (body.isSleeping() || !body.isActive()) {
				continue;
			}
			// Reset the stack of bodies to visit
			int stackIndex = 0;
			stackBodiesToVisit.set(stackIndex, body);
			stackIndex++;
			body.isAlreadyInIsland = true;
			// Create the new island
			this.islands.add(new Island(nbBodies, nbContactManifolds, this.joints.size()));
			// While there are still some bodies to visit in the stack
			while (stackIndex > 0) {
				// Get the next body to visit from the stack
				stackIndex--;
				final RigidBody bodyToVisit = stackBodiesToVisit.get(stackIndex);
				assert (bodyToVisit.isActive());
				// Awake the body if it is slepping
				bodyToVisit.setIsSleeping(false);
				// Add the body into the island
				this.islands.get(this.islands.size() - 1).addBody(bodyToVisit);
				// If the current body is static, we do not want to perform the DFS
				// search across that body
				if (bodyToVisit.getType() == BodyType.STATIC) {
					continue;
				}
				// For each contact manifold in which the current body is involded
				ContactManifoldListElement contactElement;
				for (contactElement = bodyToVisit.contactManifoldsList; contactElement != null; contactElement = contactElement.next) {
					final ContactManifold contactManifold = contactElement.contactManifold;
					assert (contactManifold.getNbContactPoints() > 0);
					// Check if the current contact manifold has already been added into an island
					if (contactManifold.isAlreadyInIsland()) {
						continue;
					}
					// Add the contact manifold into the island
					this.islands.get(this.islands.size() - 1).addContactManifold(contactManifold);
					contactManifold.isAlreadyInIsland = true;
					// Get the other body of the contact manifold
					final RigidBody body1 = (RigidBody) contactManifold.getBody1();
					final RigidBody body2 = (RigidBody) contactManifold.getBody2();
					final RigidBody otherBody = (body1.getID() == bodyToVisit.getID()) ? body2 : body1;
					// Check if the other body has already been added to the island
					if (otherBody.isAlreadyInIsland) {
						continue;
					}
					// Insert the other body into the stack of bodies to visit
					stackBodiesToVisit.set(stackIndex, otherBody);
					stackIndex++;
					otherBody.isAlreadyInIsland = true;
				}
				// For each joint in which the current body is involved
				JointListElement jointElement;
				for (jointElement = bodyToVisit.jointsList; jointElement != null; jointElement = jointElement.next) {
					final Joint joint = jointElement.joint;
					// Check if the current joint has already been added into an island
					if (joint.isAlreadyInIsland()) {
						continue;
					}
					// Add the joint into the island
					this.islands.get(this.islands.size() - 1).addJoint(joint);
					joint.isAlreadyInIsland = true;
					// Get the other body of the contact manifold
					final RigidBody body1 = joint.getBody1();
					final RigidBody body2 = joint.getBody2();
					final RigidBody otherBody = (body1.getID() == bodyToVisit.getID()) ? body2 : body1;
					// Check if the other body has already been added to the island
					if (otherBody.isAlreadyInIsland) {
						continue;
					}
					// Insert the other body into the stack of bodies to visit
					stackBodiesToVisit.set(stackIndex, otherBody);
					stackIndex++;
					otherBody.isAlreadyInIsland = true;
				}
			}
			this.islands.get(this.islands.size() - 1).resetStaticBobyNotInIsland();
		}
	}
	
	/**
	 *  Create a joint between two bodies in the world and return a pointer to the new joint
	 * @param _jointInfo The information that is necessary to create the joint
	 * @return A pointer to the joint that has been created in the world
	 */
	public Joint createJoint(final JointInfo _jointInfo) {
		Joint newJoint = null;
		// Allocate memory to create the new joint
		switch (_jointInfo.type) {
			// Ball-and-Socket joint
			case BALLSOCKETJOINT:
				newJoint = new BallAndSocketJoint((BallAndSocketJointInfo) _jointInfo);
				break;
			// Slider joint
			case SLIDERJOINT:
				newJoint = new SliderJoint((SliderJointInfo) _jointInfo);
				break;
			// Hinge joint
			case HINGEJOINT:
				newJoint = new HingeJoint((HingeJointInfo) _jointInfo);
				break;
			// Fixed joint
			case FIXEDJOINT:
				newJoint = new FixedJoint((FixedJointInfo) _jointInfo);
				break;
			default:
				assert (false);
				return null;
		}
		// If the collision between the two bodies of the raint is disabled
		if (!_jointInfo.isCollisionEnabled) {
			// Add the pair of bodies in the set of body pairs that cannot collide with each other
			this.collisionDetection.addNoCollisionPair(_jointInfo.body1, _jointInfo.body2);
		}
		// Add the joint into the world
		this.joints.add(newJoint);
		// Add the joint into the joint list of the bodies involved in the joint
		addJointToBody(newJoint);
		// Return the pointer to the created joint
		return newJoint;
	}
	
	/**
	 *  Create a rigid body into the physics world
	 * @param _transform Transform3Dation from body local-space to world-space
	 * @return A pointer to the body that has been created in the world
	 */
	public RigidBody createRigidBody(final Transform3D _transform) {
		// Compute the body ID
		final int bodyID = computeNextAvailableBodyID();
		// Largest index cannot be used (it is used for invalid index)
		assert (bodyID < Integer.MAX_VALUE);
		// Create the rigid body
		final RigidBody rigidBody = new RigidBody(_transform, this, bodyID);
		assert (rigidBody != null);
		// Add the rigid body to the physics world
		this.bodies.add(rigidBody);
		this.rigidBodies.add(rigidBody);
		// Return the pointer to the rigid body
		return rigidBody;
	}
	
	/**
	 * Destroy a joint
	 * @param[in,out] _joint Pointer to the joint you want to destroy
	 */
	public void destroyJoint(Joint _joint) {
		if (_joint == null) {
			//Log.warning("Request destroy null joint");
			return;
		}
		// If the collision between the two bodies of the raint was disabled
		if (!_joint.isCollisionEnabled()) {
			// Remove the pair of bodies from the set of body pairs that cannot collide with each other
			this.collisionDetection.removeNoCollisionPair(_joint.getBody1(), _joint.getBody2());
		}
		// Wake up the two bodies of the joint
		_joint.getBody1().setIsSleeping(false);
		_joint.getBody2().setIsSleeping(false);
		// Remove the joint from the world
		this.joints.remove(_joint);
		// Remove the joint from the joint list of the bodies involved in the joint
		_joint.getBody1().removeJointFromjointsList(_joint);
		_joint.getBody2().removeJointFromjointsList(_joint);
		_joint = null;
	}
	
	/**
	 *  Destroy a rigid body and all the joints which it beints
	 * @param[in,out] _rigidBody Pointer to the body you want to destroy
	 */
	public void destroyRigidBody(RigidBody _rigidBody) {
		// Remove all the collision shapes of the body
		_rigidBody.removeAllCollisionShapes();
		// Add the body ID to the list of free IDs
		this.freeBodiesIDs.add(_rigidBody.getID());
		// Destroy all the joints in which the rigid body to be destroyed is involved
		for (JointListElement element = _rigidBody.jointsList; element != null; element = element.next) {
			destroyJoint(element.joint);
		}
		// Reset the contact manifold list of the body
		_rigidBody.resetContactManifoldsList();
		// Remove the rigid body from the list of rigid bodies
		this.bodies.remove(_rigidBody);
		this.rigidBodies.remove(_rigidBody);
		_rigidBody = null;
	}
	
	/**
	 *  Enable/Disable the sleeping technique.
	 * The sleeping technique is used to put bodies that are not moving into sleep
	 * to speed up the simulation.
	 * @param _isSleepingEnabled True if you want to enable the sleeping technique and false otherwise
	 */
	public void enableSleeping(final boolean _isSleepingEnabled) {
		this.isSleepingEnabled = _isSleepingEnabled;
		if (!this.isSleepingEnabled) {
			// For each body of the world
			for (final RigidBody it : this.rigidBodies) {
				// Wake up the rigid body
				it.setIsSleeping(false);
			}
		}
	}
	
	/**
	 *  Get list of all contacts.
	 * @return The list of all contacts of the world
	 */
	public List<ContactManifold> getContactsList() {
		final List<ContactManifold> contactManifolds = new ArrayList<>();
		// For each currently overlapping pair of bodies
		for (final OverlappingPair pair : this.collisionDetection.overlappingPairs.values()) {
			// For each contact manifold of the pair
			final ContactManifoldSet manifoldSet = pair.getContactManifoldSet();
			for (int i = 0; i < manifoldSet.getNbContactManifolds(); i++) {
				final ContactManifold manifold = manifoldSet.getContactManifold(i);
				// Get the contact manifold
				contactManifolds.add(manifold);
			}
		}
		// Return all the contact manifold
		return contactManifolds;
	}
	
	/**
	 *  Get the gravity vector of the world
	 * @return The current gravity vector (in meter per seconds squared)
	 */
	public Vector3f getGravity() {
		return this.gravity;
	}
	
	public List<Island> getIslands() {
		return this.islands;
	}
	
	/**
	 *  Get the number of iterations for the position raint solver
	 */
	public int getNbIterationsPositionSolver() {
		return this.nbPositionSolverIterations;
	}
	
	/**
	 *  Get the number of iterations for the velocity raint solver
	 * @return Number if iteration.
	 */
	public int getNbIterationsVelocitySolver() {
		return this.nbVelocitySolverIterations;
	}
	
	/**
	 *  Get the number of all joints
	 * @return Number of joints in the world
	 */
	public int getNbJoints() {
		return this.joints.size();
	}
	
	/**
	 *  Get the number of rigid bodies in the world
	 * @return Number of rigid bodies in the world
	 */
	public int getNbRigidBodies() {
		return this.rigidBodies.size();
	}
	
	/**
	 *  Get an iterator to the beginning of the bodies of the physics world
	 * @return Starting iterator of the set of rigid bodies
	 */
	public List<RigidBody> getRigidBodies() {
		return this.rigidBodies;
	}
	
	/**
	 *  Return the current sleep angular velocity
	 * @return The sleep angular velocity (in radian per second)
	 */
	public float getSleepAngularVelocity() {
		return this.sleepAngularVelocity;
	}
	
	/**
	 *  Get the sleep linear velocity
	 * @return The current sleep linear velocity (in meters per second)
	 */
	public float getSleepLinearVelocity() {
		return this.sleepLinearVelocity;
	}
	
	/**
	 *  Get the time a body is required to stay still before sleeping
	 * @return Time a body is required to stay still before sleeping (in seconds)
	 */
	public float getTimeBeforeSleep() {
		return this.timeBeforeSleep;
	}
	
	/**
	 *  Initialize the bodies velocities arrays for the next simulation step.
	 */
	protected void initVelocityArrays() {
		// Allocate memory for the bodies velocity arrays
		final int nbBodies = this.rigidBodies.size();
		// this.numberBodiesCapacity = 0; // TODO: Remove this, this is a test of portage
		if (this.numberBodiesCapacity < nbBodies) {
			this.numberBodiesCapacity = nbBodies;
			this.splitLinearVelocities = new Vector3f[this.numberBodiesCapacity];
			this.splitAngularVelocities = new Vector3f[this.numberBodiesCapacity];
			this.rainedLinearVelocities = new Vector3f[this.numberBodiesCapacity];
			this.rainedAngularVelocities = new Vector3f[this.numberBodiesCapacity];
			this.rainedPositions = new Vector3f[this.numberBodiesCapacity];
			this.rainedOrientations = new Quaternion[this.numberBodiesCapacity];
			for (int iii = 0; iii < this.numberBodiesCapacity; iii++) {
				this.splitLinearVelocities[iii] = Vector3f.zero();
				this.splitAngularVelocities[iii] = Vector3f.zero();
				this.rainedLinearVelocities[iii] = Vector3f.zero();
				this.rainedAngularVelocities[iii] = Vector3f.zero();
				this.rainedPositions[iii] = Vector3f.zero();
				this.rainedOrientations[iii] = Quaternion.identity();
			}
		} else {
			// Reset the velocities arrays
			for (int iii = 0; iii < this.numberBodiesCapacity; iii++) {
				this.splitLinearVelocities[iii].setZero();
				this.splitAngularVelocities[iii].setZero();
				this.rainedLinearVelocities[iii].setZero();
				this.rainedAngularVelocities[iii].setZero();
				this.rainedPositions[iii].setZero();
				this.rainedOrientations[iii].setIdentity();
			}
		}
		// Initialize the map of body indexes in the velocity arrays
		this.mapBodyToConstrainedVelocityIndex.clear();
		int indexBody = 0;
		for (final RigidBody it : this.rigidBodies) {
			// Add the body into the map
			this.mapBodyToConstrainedVelocityIndex.put(it, indexBody);
			indexBody++;
		}
	}
	
	/**
	 *  Integrate position and orientation of the rigid bodies.
	 * The positions and orientations of the bodies are integrated using
	 * the sympletic Euler time stepping scheme.
	 */
	protected void integrateRigidBodiesPositions() {
		//Log.error("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++integrateRigidBodiesPositions");
		// For each island of the world
		for (int i = 0; i < this.islands.size(); i++) {
			final List<RigidBody> bodies = this.islands.get(i).getBodies();
			// For each body of the island
			for (int b = 0; b < bodies.size(); b++) {
				//Log.error("  [" + b + "/" + bodies.size() + "]");
				// Get the rained velocity
				final int indexArray = this.mapBodyToConstrainedVelocityIndex.get(bodies.get(b));
				final Vector3f newLinVelocity = this.rainedLinearVelocities[indexArray].clone();
				final Vector3f newAngVelocity = this.rainedAngularVelocities[indexArray].clone();
				//Log.error("      newAngVelocity = " + newAngVelocity);
				// Add the split impulse velocity from Contact Solver (only used
				// to update the position)
				if (this.contactSolver.isSplitImpulseActive()) {
					newLinVelocity.add(this.splitLinearVelocities[indexArray]);
					newAngVelocity.add(this.splitAngularVelocities[indexArray]);
				}
				// Get current position and orientation of the body
				final Vector3f currentPosition = bodies.get(b).centerOfMassWorld;
				//Log.error("      bodies.get(b).getTransform() = " + bodies.get(b).getTransform());
				final Quaternion currentOrientation = bodies.get(b).getTransform().getOrientation();
				// Update the new rained position and orientation of the body
				this.rainedPositions[indexArray] = newLinVelocity.multiplyNew(this.timeStep).add(currentPosition);
				//Log.error("      currentOrientation = " + currentOrientation);
				//Log.error("      newAngVelocity = " + newAngVelocity);
				//Log.error("      this.timeStep = " + FMath.floatToString(this.timeStep));
				this.rainedOrientations[indexArray] = currentOrientation.addNew(new Quaternion(0, newAngVelocity).multiplyNew(currentOrientation).multiply(0.5f * this.timeStep));
				//Log.error("      this.rainedPositions[indexArray] = " + this.rainedPositions[indexArray]);
				//Log.error("      this.rainedOrientations[indexArray] = " + this.rainedOrientations[indexArray]);
			}
		}
		for (int iii = 1; iii < this.rainedPositions.length; iii++) {
			Log.error(kkk + "         this.rainedPositions[" + iii + "] = " + this.rainedPositions[iii]);
			Log.error(kkk + "      this.rainedOrientations[" + iii + "] = " + this.rainedOrientations[iii]);
		}
		//Log.error("------------------------------------------------------------------------------------------------");
		//Log.error("------------------------------------------------------------------------------------------------");
		//Log.error("------------------------------------------------------------------------------------------------");
		//Log.error("------------------------------------------------------------------------------------------------");
		if (kkk++ == 98) {
			//System.exit(-1);
		}
	}
	
	/**
	 *  Integrate the velocities of rigid bodies.
	 * This method only set the temporary velocities but does not update
	 * the actual velocitiy of the bodies. The velocities updated in this method
	 * might violate the raints and will be corrected in the raint and
	 * contact solver.
	 */
	protected void integrateRigidBodiesVelocities() {
		// Initialize the bodies velocity arrays
		initVelocityArrays();
		//Log.info("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
		// For each island of the world
		for (int iii = 0; iii < this.islands.size(); iii++) {
			//Log.info("Manage island : " + iii + "/" + this.islands.size());
			final List<RigidBody> bodies = this.islands.get(iii).getBodies();
			// For each body of the island
			for (int bbb = 0; bbb < bodies.size(); bbb++) {
				//Log.info("     body : " + bbb + "/" + bodies.size());
				// Insert the body into the map of rained velocities
				final RigidBody tmpval = bodies.get(bbb);
				int indexBody = this.mapBodyToConstrainedVelocityIndex.get(tmpval);
				//Log.info("         indexBody=" + indexBody);
				
				assert (this.splitLinearVelocities[indexBody].isZero());
				assert (this.splitAngularVelocities[indexBody].isZero());
				// Integrate the external force to get the new velocity of the body
				this.rainedLinearVelocities[indexBody] = bodies.get(bbb).getLinearVelocity().addNew(bodies.get(bbb).externalForce.multiplyNew(this.timeStep * bodies.get(bbb).massInverse));
				//Log.info("         this.rainedLinearVelocities[indexBody]=" + this.rainedLinearVelocities[indexBody]);
				this.rainedAngularVelocities[indexBody] = bodies.get(bbb).getAngularVelocity()
						.addNew(bodies.get(bbb).getInertiaTensorInverseWorld().multiplyNew(bodies.get(bbb).externalTorque.multiplyNew(this.timeStep)));
				//Log.info("         this.rainedAngularVelocities[indexBody]=" + this.rainedAngularVelocities[indexBody]);
				// If the gravity has to be applied to this rigid body
				if (bodies.get(bbb).isGravityEnabled() && this.isGravityEnabled) {
					// Integrate the gravity force
					this.rainedLinearVelocities[indexBody].add(this.gravity.multiplyNew(this.timeStep * bodies.get(bbb).massInverse * bodies.get(bbb).getMass()));
				}
				//Log.info("   G     this.rainedLinearVelocities[indexBody]=" + this.rainedLinearVelocities[indexBody]);
				// Apply the velocity damping
				// Damping force : F_c = -c' * v (c=damping factor)
				// Equation	  : m * dv/dt = -c' * v
				//				 => dv/dt = -c * v (with c=c'/m)
				//				 => dv/dt + c * v = 0
				// Solution	  : v(t) = v0 * e^(-c * t)
				//				 => v(t + dt) = v0 * e^(-c(t + dt))
				//							  = v0 * e^(-ct) * e^(-c * dt)
				//							  = v(t) * e^(-c * dt)
				//				 => v2 = v1 * e^(-c * dt)
				// Using Taylor Serie for e^(-x) : e^x ~ 1 + x + x^2/2! + ...
				//							  => e^(-x) ~ 1 - x
				//				 => v2 = v1 * (1 - c * dt)
				final float linDampingFactor = bodies.get(bbb).getLinearDamping();
				//Log.info("         linDampingFactor=" + FMath.floatToString(linDampingFactor));
				final float angDampingFactor = bodies.get(bbb).getAngularDamping();
				//Log.info("         angDampingFactor=" + FMath.floatToString(angDampingFactor));
				final float linearDamping = FMath.pow(1.0f - linDampingFactor, this.timeStep);
				//Log.info("         linearDamping=" + FMath.floatToString(linearDamping));
				final float angularDamping = FMath.pow(1.0f - angDampingFactor, this.timeStep);
				//Log.info("         angularDamping=" + FMath.floatToString(angularDamping));
				this.rainedLinearVelocities[indexBody].multiply(linearDamping);
				//Log.info("         this.rainedLinearVelocities[indexBody]=" + this.rainedLinearVelocities[indexBody]);
				this.rainedAngularVelocities[indexBody].multiply(angularDamping);
				//Log.info("         this.rainedAngularVelocities[indexBody]=" + this.rainedAngularVelocities[indexBody]);
				indexBody++;
			}
		}
	}
	
	/**
	 *  Get if the gravity is enaled
	 * @return True if the gravity is enabled in the world
	 */
	public boolean isGravityEnabled() {
		return this.isGravityEnabled;
	}
	
	/**
	 *  Get if the sleeping technique is enabled
	 * @return True if the sleeping technique is enabled and false otherwise
	 */
	public boolean isSleepingEnabled() {
		return this.isSleepingEnabled;
	}
	
	/**
	 *  Reset the external force and torque applied to the bodies
	 */
	protected void resetBodiesForceAndTorque() {
		// For each body of the world
		this.rigidBodies.forEach((obj) -> {
			obj.externalForce.setZero();
			obj.externalTorque.setZero();
		});
	}
	
	/**
	 *  Set the position correction technique used for contacts
	 * @param _technique Technique used for the position correction (Baumgarte or Split Impulses)
	 */
	public void setContactsPositionCorrectionTechnique(final ContactsPositionCorrectionTechnique _technique) {
		if (_technique == ContactsPositionCorrectionTechnique.BAUMGARTE_CONTACTS) {
			this.contactSolver.setIsSplitImpulseActive(false);
		} else {
			this.contactSolver.setIsSplitImpulseActive(true);
		}
	}
	
	/**
	 *  Set an event listener object to receive events callbacks.
	 * @note If you use null as an argument, the events callbacks will be disabled.
	 * @param _eventListener Pointer to the event listener object that will receive
	 * event callbacks during the simulation
	 */
	public void setEventListener(final EventListener _eventListener) {
		this.eventListener = _eventListener;
	}
	
	/**
	 *  Set the gravity vector of the world
	 * @param _gravity The gravity vector (in meter per seconds squared)
	 */
	public void setGravity(final Vector3f _gravity) {
		this.gravity = _gravity;
	}
	
	/**
	 *  Enable/Disable the gravity
	 * @param _isGravityEnabled True if you want to enable the gravity in the world
	 * and false otherwise
	 */
	public void setIsGratityEnabled(final boolean _isGravityEnabled) {
		this.isGravityEnabled = _isGravityEnabled;
	}
	
	/**
	 *  Activate or deactivate the solving of friction raints at the center of the contact
	 * manifold instead of solving them at each contact point
	 * @param _isActive True if you want the friction to be solved at the center of
	 * the contact manifold and false otherwise
	 */
	public void setIsSolveFrictionAtContactManifoldCenterActive(final boolean _isActive) {
		this.contactSolver.setIsSolveFrictionAtContactManifoldCenterActive(_isActive);
	}
	
	/**
	 *  Set the position correction technique used for joints
	 * @param _technique Technique used for the joins position correction (Baumgarte or Non Linear Gauss Seidel)
	 */
	public void setJointsPositionCorrectionTechnique(final JointsPositionCorrectionTechnique _technique) {
		if (_technique == JointsPositionCorrectionTechnique.BAUMGARTE_JOINTS) {
			this.raintSolver.setIsNonLinearGaussSeidelPositionCorrectionActive(false);
		} else {
			this.raintSolver.setIsNonLinearGaussSeidelPositionCorrectionActive(true);
		}
	}
	
	/**
	 *  Set the number of iterations for the position raint solver
	 * @param _nbIterations Number of iterations for the position solver
	 */
	public void setNbIterationsPositionSolver(final int _nbIterations) {
		this.nbPositionSolverIterations = _nbIterations;
	}
	
	/**
	*  Set the number of iterations for the velocity raint solver
	* @param _nbIterations Number of iterations for the velocity solver
	*/
	public void setNbIterationsVelocitySolver(final int _nbIterations) {
		this.nbVelocitySolverIterations = _nbIterations;
	}
	
	/**
	 *  Set the sleep angular velocity.
	 * When the velocity of a body becomes smaller than the sleep linear/angular
	 * velocity for a given amount of time, the body starts sleeping and does not need
	 * to be simulated anymore.
	 * @param _sleepAngularVelocity The sleep angular velocity (in radian per second)
	 */
	public void setSleepAngularVelocity(final float _sleepAngularVelocity) {
		if (_sleepAngularVelocity < 0.0f) {
			//Log.error("Can not set _sleepAngularVelocity=" + _sleepAngularVelocity + " < 0 ");
			return;
		}
		this.sleepAngularVelocity = _sleepAngularVelocity;
	}
	
	/**
	 *  Set the sleep linear velocity
	 * @param _sleepLinearVelocity The new sleep linear velocity (in meters per second)
	 */
	public void setSleepLinearVelocity(final float _sleepLinearVelocity) {
		if (_sleepLinearVelocity < 0.0f) {
			//Log.error("Can not set _sleepLinearVelocity=" + _sleepLinearVelocity + " < 0 ");
			return;
		}
		this.sleepLinearVelocity = _sleepLinearVelocity;
	}
	
	/**
	 *  Set the time a body is required to stay still before sleeping
	 * @param timeBeforeSleep Time a body is required to stay still before sleeping (in seconds)
	 */
	public void setTimeBeforeSleep(final float timeBeforeSleep) {
		if (timeBeforeSleep < 0.0f) {
			//Log.error("Can not set timeBeforeSleep=" + timeBeforeSleep + " < 0 ");
			return;
		}
		this.timeBeforeSleep = timeBeforeSleep;
	}
	
	/**
	 *  Solve the contacts and raints
	 */
	protected void solveContactsAndConstraints() {
		// Set the velocities arrays
		this.contactSolver.setSplitVelocitiesArrays(this.splitLinearVelocities, this.splitAngularVelocities);
		this.contactSolver.setConstrainedVelocitiesArrays(this.rainedLinearVelocities, this.rainedAngularVelocities);
		this.raintSolver.setConstrainedVelocitiesArrays(this.rainedLinearVelocities, this.rainedAngularVelocities);
		this.raintSolver.setConstrainedPositionsArrays(this.rainedPositions, this.rainedOrientations);
		
		//Log.info(")))))))))))))))");
		for (int iii = 0; iii < this.rainedAngularVelocities.length; iii++) {
			//Log.info("    " + iii + " : " + this.rainedAngularVelocities[iii]);
		}
		// ---------- Solve velocity raints for joints and contacts ---------- //
		final int idIsland = 0;
		// For each island of the world
		for (final Island island : this.islands) {
			// Check if there are contacts and raints to solve
			final boolean isConstraintsToSolve = island.getNbJoints() > 0;
			final boolean isContactsToSolve = island.getNbContactManifolds() > 0;
			//Log.info("solveContactsAndConstraints : " + idIsland + " " + isConstraintsToSolve + " " + isContactsToSolve);
			if (!isConstraintsToSolve && !isContactsToSolve) {
				continue;
			}
			// If there are contacts in the current island
			if (isContactsToSolve) {
				// Initialize the solver
				this.contactSolver.initializeForIsland(this.timeStep, island);
				// Warm start the contact solver
				this.contactSolver.warmStart();
			}
			// If there are raints
			if (isConstraintsToSolve) {
				// Initialize the raint solver
				this.raintSolver.initializeForIsland(this.timeStep, island);
			}
			// For each iteration of the velocity solver
			for (int i = 0; i < this.nbVelocitySolverIterations; i++) {
				// Solve the raints
				if (isConstraintsToSolve) {
					this.raintSolver.solveVelocityConstraints(island);
				}
				// Solve the contacts
				if (isContactsToSolve) {
					this.contactSolver.solve();
				}
			}
			// Cache the lambda values in order to use them in the next
			// step and cleanup the contact solver
			if (isContactsToSolve) {
				this.contactSolver.storeImpulses();
				this.contactSolver.cleanup();
			}
		}
		//Log.info("((((((((((((((");
		for (int iii = 0; iii < this.rainedAngularVelocities.length; iii++) {
			//Log.info("    " + iii + " : " + this.rainedAngularVelocities[iii]);
		}
	}
	
	/**
	 *  Solve the position error correction of the raints
	 */
	protected void solvePositionCorrection() {
		// Do not continue if there is no raints
		if (this.joints.isEmpty()) {
			return;
		}
		// For each island of the world
		for (int islandIndex = 0; islandIndex < this.islands.size(); islandIndex++) {
			// ---------- Solve the position error correction for the raints ---------- //
			// For each iteration of the position (error correction) solver
			for (int i = 0; i < this.nbPositionSolverIterations; i++) {
				// Solve the position raints
				this.raintSolver.solvePositionConstraints(this.islands.get(islandIndex));
			}
		}
	}
	
	@Override
	public void testCollision(final CollisionBody _body1, final CollisionBody _body2, final CollisionCallback _callback) {
		// Create the sets of shapes
		final Set<DTree> shapes1 = new Set<DTree>(Set.getDTreeCoparator());
		for (ProxyShape shape = _body1.getProxyShapesList(); shape != null; shape = shape.getNext()) {
			shapes1.add(shape.broadPhaseID);
		}
		final Set<DTree> shapes2 = new Set<DTree>(Set.getDTreeCoparator());
		for (ProxyShape shape = _body2.getProxyShapesList(); shape != null; shape = shape.getNext()) {
			shapes2.add(shape.broadPhaseID);
		}
		// Perform the collision detection and report contacts
		this.collisionDetection.reportCollisionBetweenShapes(_callback, shapes1, shapes2);
	}
	
	@Override
	public void testCollision(final CollisionBody _body, final CollisionCallback _callback) {
		// Create the sets of shapes
		final Set<DTree> shapes1 = new Set<DTree>(Set.getDTreeCoparator());
		// For each shape of the body
		for (ProxyShape shape = _body.getProxyShapesList(); shape != null; shape = shape.getNext()) {
			shapes1.add(shape.broadPhaseID);
		}
		final Set<DTree> emptySet = new Set<DTree>(Set.getDTreeCoparator());
		// Perform the collision detection and report contacts
		this.collisionDetection.reportCollisionBetweenShapes(_callback, shapes1, emptySet);
	}
	
	/// Test and report collisions between all shapes of the world
	@Override
	public void testCollision(final CollisionCallback _callback) {
		final Set<DTree> emptySet = new Set<DTree>(Set.getDTreeCoparator());
		// Perform the collision detection and report contacts
		this.collisionDetection.reportCollisionBetweenShapes(_callback, emptySet, emptySet);
	}
	
	@Override
	public void testCollision(final ProxyShape _shape, final CollisionCallback _callback) {
		// Create the sets of shapes
		final Set<DTree> shapes = new Set<DTree>(Set.getDTreeCoparator());
		shapes.add(_shape.broadPhaseID);
		final Set<DTree> emptySet = new Set<DTree>(Set.getDTreeCoparator());
		// Perform the collision detection and report contacts
		this.collisionDetection.reportCollisionBetweenShapes(_callback, shapes, emptySet);
	}
	
	@Override
	public void testCollision(final ProxyShape _shape1, final ProxyShape _shape2, final CollisionCallback _callback) {
		// Create the sets of shapes
		final Set<DTree> shapes1 = new Set<DTree>(Set.getDTreeCoparator());
		shapes1.add(_shape1.broadPhaseID);
		final Set<DTree> shapes2 = new Set<DTree>(Set.getDTreeCoparator());
		shapes2.add(_shape2.broadPhaseID);
		// Perform the collision detection and report contacts
		this.collisionDetection.reportCollisionBetweenShapes(_callback, shapes1, shapes2);
	}
	
	/**
	 *  Update the physics simulation
	 * @param timeStep The amount of time to step the simulation by (in seconds)
	 */
	public void update(final float timeStep) {
		//Log.error("========> start compute     " + this.rigidBodies.size());
		for (final CollisionBody elem : this.bodies) {
			//Log.info("    " + elem.getID() + " / " + elem.getAABB());
			//Log.info("    " + elem.getID() + " / " + elem.getTransform());
		}
		
		this.timeStep = timeStep;
		// Notify the event listener about the beginning of an internal tick
		if (this.eventListener != null) {
			this.eventListener.beginInternalTick();
		}
		// Reset all the contact manifolds lists of each body
		resetContactManifoldListsOfBodies();
		if (this.rigidBodies.size() == 0) {
			// no rigid body ==> no process to do ...
			return;
		}
		// Compute the collision detection
		this.collisionDetection.computeCollisionDetection();
		// Compute the islands (separate groups of bodies with raints between each others)
		computeIslands();
		for (final CollisionBody elem : this.bodies) {
			//Log.info("111    " + elem.getID() + " / " + elem.getAABB());
			//Log.info("111    " + elem.getID() + " / " + elem.getTransform());
		}
		// Integrate the velocities
		integrateRigidBodiesVelocities();
		for (final CollisionBody elem : this.bodies) {
			//Log.info("222    " + elem.getID() + " / " + elem.getAABB());
			//Log.info("222    " + elem.getID() + " / " + elem.getTransform());
		}
		// Solve the contacts and raints
		solveContactsAndConstraints();
		for (final CollisionBody elem : this.bodies) {
			//Log.info("333    " + elem.getID() + " / " + elem.getAABB());
			//Log.info("333    " + elem.getID() + " / " + elem.getTransform());
		}
		// Integrate the position and orientation of each body
		integrateRigidBodiesPositions();
		for (final CollisionBody elem : this.bodies) {
			//Log.info("444    " + elem.getID() + " / " + elem.getAABB());
			//Log.info("444    " + elem.getID() + " / " + elem.getTransform());
		}
		// Solve the position correction for raints
		solvePositionCorrection();
		for (final CollisionBody elem : this.bodies) {
			//Log.info("555    " + elem.getID() + " / " + elem.getAABB());
			//Log.info("555    " + elem.getID() + " / " + elem.getTransform());
		}
		// Update the state (positions and velocities) of the bodies
		//Log.info(" ==> update state");
		updateBodiesState(); // here to update each aaBB skeleton in the Broadphase ... not systematic but why ?
		//Log.info(" ===> bug a partir d'ici sur le quaternion");
		for (final CollisionBody elem : this.bodies) {
			//Log.info("---    " + elem.getID() + " / " + elem.getAABB());
			//Log.info("---    " + elem.getID() + " / " + elem.getTransform());
		}
		if (this.isSleepingEnabled) {
			updateSleepingBodies();
		}
		for (final CollisionBody elem : this.bodies) {
			//Log.info("666    " + elem.getID() + " / " + elem.getAABB());
			//Log.info("666    " + elem.getID() + " / " + elem.getTransform());
		}
		// Notify the event listener about the end of an internal tick
		if (this.eventListener != null) {
			this.eventListener.endInternalTick();
		}
		for (final CollisionBody elem : this.bodies) {
			//Log.info("777    " + elem.getID() + " / " + elem.getAABB());
			//Log.info("777    " + elem.getID() + " / " + elem.getTransform());
		}
		// Reset the external force and torque applied to the bodies
		resetBodiesForceAndTorque();
		//Log.error("<< ============= end compute ");
	}
	
	/**
	 *  Update the postion/orientation of the bodies
	 */
	protected void updateBodiesState() {
		// For each island of the world
		for (int islandIndex = 0; islandIndex < this.islands.size(); islandIndex++) {
			// For each body of the island
			final List<RigidBody> bodies = this.islands.get(islandIndex).getBodies();
			for (int b = 0; b < this.islands.get(islandIndex).getNbBodies(); b++) {
				final int index = this.mapBodyToConstrainedVelocityIndex.get(bodies.get(b));
				// Update the linear and angular velocity of the body
				bodies.get(b).linearVelocity = this.rainedLinearVelocities[index].clone();
				if (bodies.get(b).isAngularReactionEnable() == true) {
					bodies.get(b).angularVelocity = this.rainedAngularVelocities[index].clone();
				}
				// Update the position of the center of mass of the body
				bodies.get(b).centerOfMassWorld = this.rainedPositions[index].clone();
				// Update the orientation of the body
				//Log.error("<< ============= this.rainedOrientations[index] " + this.rainedOrientations[index]);
				//Log.error("<< ============= this.rainedOrientations[index].safeNormalizeNew() " + this.rainedOrientations[index].safeNormalizeNew());
				if (bodies.get(b).isAngularReactionEnable() == true) {
					bodies.get(b).getTransform().setOrientation(this.rainedOrientations[index].safeNormalizeNew());
				}
				// Update the transform of the body (using the new center of mass and new orientation)
				bodies.get(b).updateTransformWithCenterOfMass();
				// Update the broad-phase state of the body
				//Log.info("     " + b + " ==> updateBroadPhaseState");
				bodies.get(b).updateBroadPhaseState();
			}
		}
	}
	
	/**
	 *  Put bodies to sleep if needed.
	 * For each island, if all the bodies have been almost still for a int enough period of
	 * time, we put all the bodies of the island to sleep.
	 */
	protected void updateSleepingBodies() {
		final float sleepLinearVelocitySquare = this.sleepLinearVelocity * this.sleepLinearVelocity;
		final float sleepAngularVelocitySquare = this.sleepAngularVelocity * this.sleepAngularVelocity;
		// For each island of the world
		for (int i = 0; i < this.islands.size(); i++) {
			float minSleepTime = Float.MAX_VALUE;
			// For each body of the island
			final List<RigidBody> bodies = this.islands.get(i).getBodies();
			for (int b = 0; b < bodies.size(); b++) {
				// Skip static bodies
				if (bodies.get(b).getType() == BodyType.STATIC) {
					continue;
				}
				// If the body is velocity is large enough to stay awake
				Log.error(" check if ready to sleep: linear  = " + bodies.get(b).getLinearVelocity().length2() + " > " + sleepLinearVelocitySquare);
				Log.error(" check if ready to sleep: angular = " + bodies.get(b).getAngularVelocity().length2() + " > " + sleepAngularVelocitySquare);
				if (bodies.get(b).getLinearVelocity().length2() > sleepLinearVelocitySquare || bodies.get(b).getAngularVelocity().length2() > sleepAngularVelocitySquare
						|| !bodies.get(b).isAllowedToSleep()) {
					// Reset the sleep time of the body
					bodies.get(b).sleepTime = 0.0f;
					minSleepTime = 0.0f;
				} else { // If the body velocity is bellow the sleeping velocity threshold
					// Increase the sleep time
					bodies.get(b).sleepTime += this.timeStep;
					if (bodies.get(b).sleepTime < minSleepTime) {
						minSleepTime = bodies.get(b).sleepTime;
					}
				}
			}
			// If the velocity of all the bodies of the island is under the
			// sleeping velocity threshold for a period of time larger than
			// the time required to become a sleeping body
			if (minSleepTime >= this.timeBeforeSleep) {
				// Put all the bodies of the island to sleep
				for (int b = 0; b < this.islands.get(i).getNbBodies(); b++) {
					bodies.get(b).setIsSleeping(true);
				}
			}
		}
	}
	
}
