package org.atriasoft.ephysics.engine;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.atriasoft.ephysics.Configuration;
import org.atriasoft.ephysics.body.BodyType;
import org.atriasoft.ephysics.body.RigidBody;
import org.atriasoft.ephysics.collision.ContactManifold;
import org.atriasoft.ephysics.constraint.ContactPoint;
import org.atriasoft.etk.math.Constant;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Matrix3f;
import org.atriasoft.etk.math.Vector3f;

/**
 *  This class represents the contact solver that is used to solve rigid bodies contacts.
 * The raint solver is based on the "Sequential Impulse" technique described by
 * Erin Catto in his GDC slides (http://code.google.com/p/box2d/downloads/list).
 *
 * A raint between two bodies is represented by a function C(x) which is equal to zero
 * when the raint is satisfied. The condition C(x)=0 describes a valid position and the
 * condition dC(x)/dt=0 describes a valid velocity. We have dC(x)/dt = Jv + b = 0 where J is
 * the Jacobian matrix of the raint, v is a vector that contains the velocity of both
 * bodies and b is the raint bias. We are looking for a force F_c that will act on the
 * bodies to keep the raint satisfied. Note that from the  work principle, we have
 * F_c = J^t * lambda where J^t is the transpose of the Jacobian matrix and lambda is a
 * Lagrange multiplier. Therefore, finding the force F_c is equivalent to finding the Lagrange
 * multiplier lambda.
 *
 * An impulse P = F * dt where F is a force and dt is the timestep. We can apply impulses a
 * body to change its velocity. The idea of the Sequential Impulse technique is to apply
 * impulses to bodies of each raints in order to keep the raint satisfied.
 *
 * --- Step 1 ---
 *
 * First, we integrate the applied force F_a acting of each rigid body (like gravity, ...) and
 * we obtain some new velocities v2' that tends to violate the raints.
 *
 * v2' = v1 + dt * M^-1 * F_a
 *
 * where M is a matrix that contains mass and inertia tensor information.
 *
 * --- Step 2 ---
 *
 * During the second step, we iterate over all the raints for a certain number of
 * iterations and for each raint we compute the impulse to apply to the bodies needed
 * so that the new velocity of the bodies satisfy Jv + b = 0. From the Newton law, we know that
 * M * deltaV = P_c where M is the mass of the body, deltaV is the difference of velocity and
 * P_c is the raint impulse to apply to the body. Therefore, we have
 * v2 = v2' + M^-1 * P_c. For each raint, we can compute the Lagrange multiplier lambda
 * using : lambda = -this.c (Jv2' + b) where this.c = 1 / (J * M^-1 * J^t). Now that we have the
 * Lagrange multiplier lambda, we can compute the impulse P_c = J^t * lambda * dt to apply to
 * the bodies to satisfy the raint.
 *
 * --- Step 3 ---
 *
 * In the third step, we integrate the new position x2 of the bodies using the new velocities
 * v2 computed in the second step with : x2 = x1 + dt * v2.
 *
 * Note that in the following code (as it is also explained in the slides from Erin Catto),
 * the value lambda is not only the lagrange multiplier but is the multiplication of the
 * Lagrange multiplier with the timestep dt. Therefore, in the following code, when we use
 * lambda, we mean (lambda * dt).
 *
 * We are using the accumulated impulse technique that is also described in the slides from
 * Erin Catto.
 *
 * We are also using warm starting. The idea is to warm start the solver at the beginning of
 * each step by applying the last impulstes for the raints that we already existing at the
 * previous step. This allows the iterative solver to converge faster towards the solution.
 *
 * For contact raints, we are also using split impulses so that the position correction
 * that uses Baumgarte stabilization does not change the momentum of the bodies.
 *
 * There are two ways to apply the friction raints. Either the friction raints are
 * applied at each contact point or they are applied only at the center of the contact manifold
 * between two bodies. If we solve the friction raints at each contact point, we need
 * two raints (two tangential friction directions) and if we solve the friction
 * raints at the center of the contact manifold, we need two raints for tangential
 * friction but also another twist friction raint to prevent spin of the body around the
 * contact manifold center.
 */
public class ContactSolver {
	
	private static float BETA = 0.2f; //!< Beta value for the penetration depth position correction without split impulses
	private static float BETA_SPLIT_IMPULSE = 0.2f; //!< Beta value for the penetration depth position correction with split impulses
	private static float SLOP = 0.01f; //!< Slop distance (allowed penetration distance between bodies)
	private Vector3f[] splitLinearVelocities; //!< Split linear velocities for the position contact solver (split impulse)
	private Vector3f[] splitAngularVelocities; //!< Split angular velocities for the position contact solver (split impulse)
	private float timeStep; //!< Current time step
	private List<ContactManifoldSolver> contactConstraints = new ArrayList<>(); //!< Contact raints
	private Vector3f[] linearVelocities; //!< Array of linear velocities
	private Vector3f[] angularVelocities; //!< Array of angular velocities
	private final Map<RigidBody, Integer> mapBodyToConstrainedVelocityIndex; //!< Reference to the map of rigid body to their index in the rained velocities array
	private final boolean isWarmStartingActive; //!< True if the warm starting of the solver is active
	private boolean isSplitImpulseActive; //!< True if the split impulse position correction is active
	private boolean isSolveFrictionAtContactManifoldCenterActive; //!< True if we solve 3 friction raints at the contact manifold center only instead of 2 friction raints at each contact point
	
	/**
	 *  Constructor
	 * @param _mapBodyToVelocityIndex
	 */
	public ContactSolver(final Map<RigidBody, Integer> _mapBodyToVelocityIndex) {
		this.splitLinearVelocities = null;
		this.splitAngularVelocities = null;
		this.linearVelocities = null;
		this.angularVelocities = null;
		this.mapBodyToConstrainedVelocityIndex = _mapBodyToVelocityIndex;
		this.isWarmStartingActive = false; // TODO default true
		this.isSplitImpulseActive = true; // TODO default true
		this.isSolveFrictionAtContactManifoldCenterActive = true; // TODO default true
	}
	
	/**
	 *  Apply an impulse to the two bodies of a raint
	 * @param _impulse Impulse to apply
	 * @param _manifold Constraint to apply the impulse
	 */
	private void applyImpulse(final Impulse _impulse, final ContactManifoldSolver _manifold) {
		//Log.info("    --        _manifold.massInverseBody1=" + _manifold.massInverseBody1);
		//Log.info("    --        _manifold.inverseInertiaTensorBody1=" + _manifold.inverseInertiaTensorBody1);
		//Log.info("    --        _manifold.massInverseBody2=" + _manifold.massInverseBody2);
		//Log.info("    --        _manifold.inverseInertiaTensorBody2=" + _manifold.inverseInertiaTensorBody2);
		//Log.info("    --        _impulse.angularImpulseBody2=" + _impulse.angularImpulseBody2);
		// Update the velocities of the body 1 by applying the impulse P
		this.linearVelocities[_manifold.indexBody1].add(_impulse.linearImpulseBody1.multiplyNew(_manifold.massInverseBody1));
		//Log.info("    --        this.angularVelocities[_manifold.indexBody1]=" + this.angularVelocities[_manifold.indexBody1]);
		this.angularVelocities[_manifold.indexBody1].add(_manifold.inverseInertiaTensorBody1.multiplyNew(_impulse.angularImpulseBody1));
		//Log.info("    --        this.angularVelocities[_manifold.indexBody1]=" + this.angularVelocities[_manifold.indexBody1]);
		// Update the velocities of the body 1 by applying the impulse P
		this.linearVelocities[_manifold.indexBody2].add(_impulse.linearImpulseBody2.multiplyNew(_manifold.massInverseBody2));
		//Log.info("    --        this.angularVelocities[_manifold.indexBody2]=" + this.angularVelocities[_manifold.indexBody2]);
		this.angularVelocities[_manifold.indexBody2].add(_manifold.inverseInertiaTensorBody2.multiplyNew(_impulse.angularImpulseBody2));
		//Log.info("    --        this.angularVelocities[_manifold.indexBody2]=" + this.angularVelocities[_manifold.indexBody2]);
	}
	
	/**
	 *  Apply an impulse to the two bodies of a raint
	 * @param _impulse Impulse to apply
	 * @param _manifold Constraint to apply the impulse
	 */
	private void applySplitImpulse(final Impulse _impulse, final ContactManifoldSolver _manifold) {
		// Update the velocities of the body 1 by applying the impulse P
		this.splitLinearVelocities[_manifold.indexBody1].add(_impulse.linearImpulseBody1.multiplyNew(_manifold.massInverseBody1));
		this.splitAngularVelocities[_manifold.indexBody1].add(_manifold.inverseInertiaTensorBody1.multiplyNew(_impulse.angularImpulseBody1));
		// Update the velocities of the body 1 by applying the impulse P
		this.splitLinearVelocities[_manifold.indexBody2].add(_impulse.linearImpulseBody2.multiplyNew(_manifold.massInverseBody2));
		this.splitAngularVelocities[_manifold.indexBody2].add(_manifold.inverseInertiaTensorBody2.multiplyNew(_impulse.angularImpulseBody2));
	}
	
	/**
	 *  Clean up the raint solver
	 */
	public void cleanup() {
		this.contactConstraints.clear();
	}
	
	/**
	 *  Compute the first friction raint impulse
	 * @param _deltaLambda Ratio to apply at the calculation.
	 * @param _contactPoint Contact point property
	 * @return Impulse of the friction result
	 */
	private Impulse computeFriction1Impulse(final float _deltaLambda, final ContactPointSolver _contactPoint) {
		//Log.error("=== r1CrossT1=" + _contactPoint.r1CrossT1);
		//Log.error("=== r2CrossT1=" + _contactPoint.r2CrossT1);
		return new Impulse(_contactPoint.frictionVector1.multiplyNew(-1).multiply(_deltaLambda), _contactPoint.r1CrossT1.multiplyNew(-1).multiply(_deltaLambda),
				_contactPoint.frictionVector1.multiplyNew(_deltaLambda), _contactPoint.r2CrossT1.multiplyNew(_deltaLambda));
	}
	
	/**
	 *  Compute the second friction raint impulse
	 * @param _deltaLambda Ratio to apply at the calculation.
	 * @param _contactPoint Contact point property
	 * @return Impulse of the friction result
	 */
	private Impulse computeFriction2Impulse(final float _deltaLambda, final ContactPointSolver _contactPoint) {
		//Log.error("=== r1CrossT2=" + _contactPoint.r1CrossT2);
		//Log.error("=== r2CrossT2=" + _contactPoint.r2CrossT2);
		return new Impulse(_contactPoint.frictionvec2.multiplyNew(-1).multiply(_deltaLambda), _contactPoint.r1CrossT2.multiplyNew(-1).multiplyNew(_deltaLambda),
				_contactPoint.frictionvec2.multiplyNew(_deltaLambda), _contactPoint.r2CrossT2.multiplyNew(_deltaLambda));
	}
	
	/**
	 *  Compute the two unit orthogonal vectors "t1" and "t2" that span the tangential friction
	 *        plane for a contact manifold. The two vectors have to be such that : t1 x t2 = contactNormal.
	 * @param _deltaVelocity Velocity ratio (with the delta time step)
	 * @param[in,out] _contactPoint Contact point property
	 */
	private void computeFrictionVectors(final Vector3f _deltaVelocity, final ContactManifoldSolver _contactManifold) {
		assert (_contactManifold.normal.length() > 0.0f);
		// Compute the velocity difference vector in the tangential plane
		final Vector3f normalVelocity = _contactManifold.normal.multiplyNew(_deltaVelocity.dot(_contactManifold.normal));
		final Vector3f tangentVelocity = _deltaVelocity.lessNew(normalVelocity);
		// If the velocty difference in the tangential plane is not zero
		final float lengthTangenVelocity = tangentVelocity.length();
		if (lengthTangenVelocity > Constant.FLOAT_EPSILON) {
			// Compute the first friction vector in the direction of the tangent
			// velocity difference
			_contactManifold.frictionVector1 = tangentVelocity.divideNew(lengthTangenVelocity);
			//Log.error("===1==>>>>>> _contactManifold.frictionVector1=" + _contactManifold.frictionVector1);
		} else {
			// Get any orthogonal vector to the normal as the first friction vector
			_contactManifold.frictionVector1 = _contactManifold.normal.getOrthoVector();
			//Log.error("===2==>>>>>> _contactManifold.frictionVector1=" + _contactManifold.frictionVector1);
		}
		
		// The second friction vector is computed by the cross product of the firs
		// friction vector and the contact normal
		_contactManifold.frictionvec2 = _contactManifold.normal.cross(_contactManifold.frictionVector1).safeNormalizeNew();
	}
	
	/**
	 *  Compute the two unit orthogonal vectors "t1" and "t2" that span the tangential friction
	 *        plane for a contact point. The two vectors have to be such that : t1 x t2 = contactNormal.
	 * @param _deltaVelocity Velocity ratio (with the delta time step)
	 * @param[in,out] _contactPoint Contact point property
	 */
	private void computeFrictionVectors(final Vector3f _deltaVelocity, final ContactPointSolver _contactPoint) {
		assert (_contactPoint.normal.length() > 0.0f);
		//Log.error("===3==>>>>>> _contactPoint.normal=" + _contactPoint.normal);
		//Log.error("===3==>>>>>> _deltaVelocity=" + _deltaVelocity);
		// Compute the velocity difference vector in the tangential plane
		final Vector3f normalVelocity = _contactPoint.normal.multiplyNew(_deltaVelocity.dot(_contactPoint.normal));
		//Log.error("===3==>>>>>> normalVelocity=" + normalVelocity);
		final Vector3f tangentVelocity = _deltaVelocity.lessNew(normalVelocity);
		//Log.error("===3==>>>>>> tangentVelocity=" + tangentVelocity);
		// If the velocty difference in the tangential plane is not zero
		final float lengthTangenVelocity = tangentVelocity.length();
		//Log.error("===3==>>>>>> lengthTangenVelocity=" + FMath.floatToString(lengthTangenVelocity));
		if (lengthTangenVelocity > Constant.FLOAT_EPSILON) {
			// Compute the first friction vector in the direction of the tangent
			// velocity difference
			_contactPoint.frictionVector1 = tangentVelocity.divideNew(lengthTangenVelocity);
			//Log.error("===3==>>>>>> _contactPoint.frictionVector1=" + _contactPoint.frictionVector1);
		} else {
			// Get any orthogonal vector to the normal as the first friction vector
			_contactPoint.frictionVector1 = _contactPoint.normal.getOrthoVector();
			//Log.error("===4==>>>>>> _contactPoint.frictionVector1=" + _contactPoint.frictionVector1);
		}
		// The second friction vector is computed by the cross product of the firs
		// friction vector and the contact normal
		_contactPoint.frictionvec2 = _contactPoint.normal.cross(_contactPoint.frictionVector1).safeNormalizeNew();
	}
	
	/**
	 *  Compute the mixed friction coefficient from the friction coefficient of each body
	 * @param _body1 First body to compute
	 * @param _body2 Second body to compute
	 * @return Mixed friction coefficient
	 */
	private float computeMixedFrictionCoefficient(final RigidBody _body1, final RigidBody _body2) {
		// Use the geometric mean to compute the mixed friction coefficient
		return FMath.sqrt(_body1.getMaterial().getFrictionCoefficient() * _body2.getMaterial().getFrictionCoefficient());
	}
	
	/**
	 *  Compute the collision restitution factor from the restitution factor of each body
	 * @param _body1 First body to compute
	 * @param _body2 Second body to compute
	 * @return Collision restitution factor
	 */
	private float computeMixedRestitutionFactor(final RigidBody _body1, final RigidBody _body2) {
		final float restitution1 = _body1.getMaterial().getBounciness();
		final float restitution2 = _body2.getMaterial().getBounciness();
		//Log.error("######################### restitution1=" + FMath.floatToString(restitution1));
		//Log.error("######################### restitution2=" + FMath.floatToString(restitution2));
		// Return the largest restitution factor
		return (restitution1 > restitution2) ? restitution1 : restitution2;
	}
	
	/**
	 *  Compute the mixed rolling resistance factor between two bodies
	 * @param _body1 First body to compute
	 * @param _body2 Second body to compute
	 * @return Mixed rolling resistance
	 */
	private float computeMixedRollingResistance(final RigidBody _body1, final RigidBody _body2) {
		return 0.5f * (_body1.getMaterial().getRollingResistance() + _body2.getMaterial().getRollingResistance());
	}
	
	/**
	 *  Compute a penetration raint impulse
	 * @param _deltaLambda Ratio to apply at the calculation.
	 * @param[in,out] _contactPoint Contact point property
	 * @return Impulse of the penetration result
	 */
	private Impulse computePenetrationImpulse(final float _deltaLambda, final ContactPointSolver _contactPoint) {
		//Log.error("=== r1CrossN=" + _contactPoint.r1CrossN);
		//Log.error("=== r2CrossN=" + _contactPoint.r2CrossN);
		return new Impulse(_contactPoint.normal.multiplyNew(-1).multiply(_deltaLambda), _contactPoint.r1CrossN.multiplyNew(-1).multiply(_deltaLambda), _contactPoint.normal.multiplyNew(_deltaLambda),
				_contactPoint.r2CrossN.multiplyNew(_deltaLambda));
	}
	
	/**
	 *  Initialize the contact raints before solving the system
	 */
	private void initializeContactConstraints() {
		// For each contact raint
		for (int ccc = 0; ccc < this.contactConstraints.size(); ccc++) {
			//Log.warning("}}}}   ccc=" + ccc);
			final ContactManifoldSolver manifold = this.contactConstraints.get(ccc);
			// Get the inertia tensors of both bodies
			final Matrix3f I1 = manifold.inverseInertiaTensorBody1;
			//Log.warning("}}}}       I1=" + I1);
			final Matrix3f I2 = manifold.inverseInertiaTensorBody2;
			//Log.warning("}}}}       I2=" + I2);
			// If we solve the friction raints at the center of the contact manifold
			if (this.isSolveFrictionAtContactManifoldCenterActive) {
				manifold.normal = new Vector3f(0.0f, 0.0f, 0.0f);
				//Log.error("]]]    manifold.normal=" + manifold.normal);
			}
			//Log.warning("}}}}       manifold.normal=" + manifold.normal);
			// Get the velocities of the bodies
			final Vector3f v1 = this.linearVelocities[manifold.indexBody1];
			final Vector3f w1 = this.angularVelocities[manifold.indexBody1];
			final Vector3f v2 = this.linearVelocities[manifold.indexBody2];
			final Vector3f w2 = this.angularVelocities[manifold.indexBody2];
			//Log.warning("}}}}       v1=" + v1);
			//Log.warning("}}}}       w1=" + w1);
			//Log.warning("}}}}       v2=" + v2);
			//Log.warning("}}}}       w2=" + w2);
			// For each contact point raint
			for (int iii = 0; iii < manifold.nbContacts; iii++) {
				//Log.warning("}}}}       iii=" + iii);
				final ContactPointSolver contactPoint = manifold.contacts[iii];
				//Log.warning("}}}}           contactPoint.r1=" + contactPoint.r1);
				//Log.warning("}}}}           contactPoint.r2=" + contactPoint.r2);
				//Log.warning("}}}}           contactPoint.normal=" + contactPoint.normal);
				final ContactPoint externalContact = contactPoint.externalContact;
				// Compute the velocity difference
				final Vector3f deltaV = v2.addNew(w2.cross(contactPoint.r2)).less(v1).less(w1.cross(contactPoint.r1));
				//Log.warning("}}}}           deltaV=" + deltaV);
				contactPoint.r1CrossN = contactPoint.r1.cross(contactPoint.normal);
				contactPoint.r2CrossN = contactPoint.r2.cross(contactPoint.normal);
				//Log.warning("}}}}           contactPoint.r1CrossN=" + contactPoint.r1CrossN);
				//Log.warning("}}}}           contactPoint.r2CrossN=" + contactPoint.r2CrossN);
				// Compute the inverse mass matrix K for the penetration raint
				final float massPenetration = manifold.massInverseBody1 + manifold.massInverseBody2 + ((I1.multiplyNew(contactPoint.r1CrossN)).cross(contactPoint.r1)).dot(contactPoint.normal)
						+ ((I2.multiplyNew(contactPoint.r2CrossN)).cross(contactPoint.r2)).dot(contactPoint.normal);
				//Log.warning("}}}}           massPenetration=" + FMath.floatToString(massPenetration));
				if (massPenetration > 0.0f) {
					contactPoint.inversePenetrationMass = 1.0f / massPenetration;
				}
				//Log.warning("}}}}           contactPoint.inversePenetrationMass=" + FMath.floatToString(contactPoint.inversePenetrationMass));
				// If we do not solve the friction raints at the center of the contact manifold
				//Log.warning("}}}}           this.isSolveFrictionAtContactManifoldCenterActive=" + this.isSolveFrictionAtContactManifoldCenterActive);
				if (!this.isSolveFrictionAtContactManifoldCenterActive) {
					// Compute the friction vectors
					computeFrictionVectors(deltaV, contactPoint);
					//Log.warning("}}}}           contactPoint.frictionVector1=" + contactPoint.frictionVector1);
					contactPoint.r1CrossT1 = contactPoint.r1.cross(contactPoint.frictionVector1);
					contactPoint.r1CrossT2 = contactPoint.r1.cross(contactPoint.frictionvec2);
					contactPoint.r2CrossT1 = contactPoint.r2.cross(contactPoint.frictionVector1);
					contactPoint.r2CrossT2 = contactPoint.r2.cross(contactPoint.frictionvec2);
					//Log.warning("}}}}           contactPoint.r1CrossT1=" + contactPoint.r1CrossT1);
					//Log.warning("}}}}           contactPoint.r1CrossT2=" + contactPoint.r1CrossT2);
					//Log.warning("}}}}           contactPoint.r2CrossT1=" + contactPoint.r2CrossT1);
					//Log.warning("}}}}           contactPoint.r2CrossT2=" + contactPoint.r2CrossT2);
					// Compute the inverse mass matrix K for the friction
					// raints at each contact point
					final float friction1Mass = manifold.massInverseBody1 + manifold.massInverseBody2
							+ ((I1.multiplyNew(contactPoint.r1CrossT1)).cross(contactPoint.r1)).dot(contactPoint.frictionVector1)
							+ ((I2.multiplyNew(contactPoint.r2CrossT1)).cross(contactPoint.r2)).dot(contactPoint.frictionVector1);
					final float friction2Mass = manifold.massInverseBody1 + manifold.massInverseBody2 + ((I1.multiplyNew(contactPoint.r1CrossT2)).cross(contactPoint.r1)).dot(contactPoint.frictionvec2)
							+ ((I2.multiplyNew(contactPoint.r2CrossT2)).cross(contactPoint.r2)).dot(contactPoint.frictionvec2);
					if (friction1Mass > 0.0f) {
						contactPoint.inverseFriction1Mass = 1.0f / friction1Mass;
					}
					if (friction2Mass > 0.0f) {
						contactPoint.inverseFriction2Mass = 1.0f / friction2Mass;
					}
				}
				// Compute the restitution velocity bias "b". We compute this here instead
				// of inside the solve() method because we need to use the velocity difference
				// at the beginning of the contact. Note that if it is a resting contact (normal
				// velocity bellow a given threshold), we do not add a restitution velocity bias
				contactPoint.restitutionBias = 0.0f;
				//Log.warning("}}}}+++++++++++++++++++++           contactPoint.restitutionBias=" + contactPoint.restitutionBias);
				final float deltaVDotN = deltaV.dot(contactPoint.normal);
				//Log.warning("}}}}+++++++++++++++++++++           deltaV=" + deltaV);
				//Log.warning("}}}}+++++++++++++++++++++           contactPoint.normal=" + contactPoint.normal);
				//Log.warning("}}}}+++++++++++++++++++++           deltaVDotN=" + FMath.floatToString(deltaVDotN));
				//Log.warning("}}}}+++++++++++++++++++++           Configuration.RESTITUTION_VELOCITY_THRESHOLD=" + FMath.floatToString(Configuration.RESTITUTION_VELOCITY_THRESHOLD));
				//Log.warning("}}}}+++++++++++++++++++++           manifold.restitutionFactor=" + FMath.floatToString(manifold.restitutionFactor));
				if (deltaVDotN < -Configuration.RESTITUTION_VELOCITY_THRESHOLD) {
					contactPoint.restitutionBias = manifold.restitutionFactor * deltaVDotN;
					//Log.warning("}}}}+++++++++++++++++++++           contactPoint.restitutionBias=" + FMath.floatToString(contactPoint.restitutionBias));
				}
				// If the warm starting of the contact solver is active
				if (this.isWarmStartingActive) {
					// Get the cached accumulated impulses from the previous step
					contactPoint.penetrationImpulse = externalContact.getPenetrationImpulse();
					contactPoint.friction1Impulse = externalContact.getFrictionImpulse1();
					contactPoint.friction2Impulse = externalContact.getFrictionImpulse2();
					contactPoint.rollingResistanceImpulse = externalContact.getRollingResistanceImpulse();
				}
				// Initialize the split impulses to zero
				contactPoint.penetrationSplitImpulse = 0.0f;
				// If we solve the friction raints at the center of the contact manifold
				if (this.isSolveFrictionAtContactManifoldCenterActive) {
					//Log.error("]]]    contactPoint.normal=" + contactPoint.normal);
					manifold.normal.add(contactPoint.normal);
					//Log.error("]]]    manifold.normal=" + manifold.normal);
				}
			}
			// Compute the inverse K matrix for the rolling resistance raint
			manifold.inverseRollingResistance.setZero();
			if (manifold.rollingResistanceFactor > 0 && (manifold.isBody1DynamicType || manifold.isBody2DynamicType)) {
				manifold.inverseRollingResistance = manifold.inverseInertiaTensorBody1.addNew(manifold.inverseInertiaTensorBody2);
				manifold.inverseRollingResistance = manifold.inverseRollingResistance.inverseNew();
			}
			// If we solve the friction raints at the center of the contact manifold
			if (this.isSolveFrictionAtContactManifoldCenterActive) {
				//Log.error("]]]    manifold.normal=" + manifold.normal);
				manifold.normal.normalize();
				//Log.error("]]]    manifold.normal=" + manifold.normal);
				final Vector3f deltaVFrictionPoint = v2.addNew(w2.cross(manifold.r2Friction)).less(v1).less(w1.cross(manifold.r1Friction));
				//Log.error("]]]    deltaVFrictionPoint=" + deltaVFrictionPoint);
				// Compute the friction vectors
				computeFrictionVectors(deltaVFrictionPoint, manifold);
				// Compute the inverse mass matrix K for the friction raints at the center of
				// the contact manifold
				manifold.r1CrossT1 = manifold.r1Friction.cross(manifold.frictionVector1);
				manifold.r1CrossT2 = manifold.r1Friction.cross(manifold.frictionvec2);
				manifold.r2CrossT1 = manifold.r2Friction.cross(manifold.frictionVector1);
				manifold.r2CrossT2 = manifold.r2Friction.cross(manifold.frictionvec2);
				//Log.warning("}}}}           manifold.r1CrossT1=" + manifold.r1CrossT1);
				//Log.warning("}}}}           manifold.r1CrossT2=" + manifold.r1CrossT2);
				//Log.warning("}}}}           manifold.r2CrossT1=" + manifold.r2CrossT1);
				//Log.warning("}}}}           manifold.r2CrossT2=" + manifold.r2CrossT2);
				final float friction1Mass = manifold.massInverseBody1 + manifold.massInverseBody2 + ((I1.multiplyNew(manifold.r1CrossT1)).cross(manifold.r1Friction)).dot(manifold.frictionVector1)
						+ ((I2.multiplyNew(manifold.r2CrossT1)).cross(manifold.r2Friction)).dot(manifold.frictionVector1);
				//Log.error("]]]    friction1Mass=" + FMath.floatToString(friction1Mass));
				final float friction2Mass = manifold.massInverseBody1 + manifold.massInverseBody2 + ((I1.multiplyNew(manifold.r1CrossT2)).cross(manifold.r1Friction)).dot(manifold.frictionvec2)
						+ ((I2.multiplyNew(manifold.r2CrossT2)).cross(manifold.r2Friction)).dot(manifold.frictionvec2);
				//Log.error("]]]    friction2Mass=" + FMath.floatToString(friction2Mass));
				final float frictionTwistMass = manifold.normal.dot(manifold.inverseInertiaTensorBody1.multiplyNew(manifold.normal))
						+ manifold.normal.dot(manifold.inverseInertiaTensorBody2.multiplyNew(manifold.normal));
				//Log.error("]]]    frictionTwistMass=" + FMath.floatToString(frictionTwistMass));
				if (friction1Mass > 0.0f) {
					manifold.inverseFriction1Mass = 1.0f / friction1Mass;
					//Log.error("]]]    manifold.inverseFriction1Mass=" + FMath.floatToString(manifold.inverseFriction1Mass));
				}
				if (friction2Mass > 0.0f) {
					manifold.inverseFriction2Mass = 1.0f / friction2Mass;
					//Log.error("]]]    manifold.inverseFriction2Mass=" + FMath.floatToString(manifold.inverseFriction2Mass));
				}
				if (frictionTwistMass > 0.0f) {
					manifold.inverseTwistFrictionMass = 1.0f / frictionTwistMass;
					//Log.error("]]]    manifold.inverseTwistFrictionMass=" + FMath.floatToString(manifold.inverseTwistFrictionMass));
				}
			}
		}
	}
	
	/**
	 *  Initialize the raint solver for a given island
	 * @param _dt Delta step time
	 * @param _island Island list property
	 */
	public void initializeForIsland(final float _dt, final Island _island) {
		assert (_island != null);
		assert (_island.getNbBodies() > 0);
		assert (_island.getNbContactManifolds() > 0);
		assert (this.splitLinearVelocities != null);
		assert (this.splitAngularVelocities != null);
		// Set the current time step
		// TODO: optimize this... this is so ugly ...
		this.timeStep = _dt;
		{
			this.contactConstraints = new ArrayList<>(_island.getNbContactManifolds());
		}
		// this.contactConstraints.resize(_island.getNbContactManifolds());
		// For each contact manifold of the island
		final List<ContactManifold> contactManifolds = _island.getContactManifold();
		for (int iii = 0; iii < _island.getNbContactManifolds(); ++iii) {
			final ContactManifold externalManifold = contactManifolds.get(iii);
			final ContactManifoldSolver internalManifold = new ContactManifoldSolver(); // TODO Note no real need to reallocate a new one ==> maybe reuse (done in C++ not in java ...)
			this.contactConstraints.add(internalManifold);
			
			assert (externalManifold.getNbContactPoints() > 0);
			// Get the two bodies of the contact
			final RigidBody body1 = (RigidBody) (externalManifold.getContactPoint(0).getBody1());
			final RigidBody body2 = (RigidBody) (externalManifold.getContactPoint(0).getBody2());
			assert (body1 != null);
			assert (body2 != null);
			// Get the position of the two bodies
			final Vector3f x1 = body1.centerOfMassWorld;
			final Vector3f x2 = body2.centerOfMassWorld;
			// Initialize the internal contact manifold structure using the external
			// contact manifold
			internalManifold.indexBody1 = this.mapBodyToConstrainedVelocityIndex.get(body1);
			internalManifold.indexBody2 = this.mapBodyToConstrainedVelocityIndex.get(body2);
			internalManifold.inverseInertiaTensorBody1 = body1.getInertiaTensorInverseWorld();
			internalManifold.inverseInertiaTensorBody2 = body2.getInertiaTensorInverseWorld();
			internalManifold.massInverseBody1 = body1.massInverse;
			internalManifold.massInverseBody2 = body2.massInverse;
			internalManifold.nbContacts = externalManifold.getNbContactPoints();
			internalManifold.restitutionFactor = computeMixedRestitutionFactor(body1, body2);
			internalManifold.frictionCoefficient = computeMixedFrictionCoefficient(body1, body2);
			internalManifold.rollingResistanceFactor = computeMixedRollingResistance(body1, body2);
			internalManifold.externalContactManifold = externalManifold;
			internalManifold.isBody1DynamicType = body1.getType() == BodyType.DYNAMIC;
			internalManifold.isBody2DynamicType = body2.getType() == BodyType.DYNAMIC;
			// If we solve the friction raints at the center of the contact manifold
			if (this.isSolveFrictionAtContactManifoldCenterActive) {
				internalManifold.frictionPointBody1 = new Vector3f(0.0f, 0.0f, 0.0f);
				internalManifold.frictionPointBody2 = new Vector3f(0.0f, 0.0f, 0.0f);
				//Log.error("]]]    internalManifold.frictionPointBody1=" + internalManifold.frictionPointBody1);
				//Log.error("]]]    internalManifold.frictionPointBody2=" + internalManifold.frictionPointBody2);
			}
			// For each  contact point of the contact manifold
			for (int ccc = 0; ccc < externalManifold.getNbContactPoints(); ++ccc) {
				final ContactPointSolver contactPoint = internalManifold.contacts[ccc];
				// Get a contact point
				final ContactPoint externalContact = externalManifold.getContactPoint(ccc);
				// Get the contact point on the two bodies
				final Vector3f p1 = externalContact.getWorldPointOnBody1();
				final Vector3f p2 = externalContact.getWorldPointOnBody2();
				contactPoint.externalContact = externalContact;
				contactPoint.normal = externalContact.getNormal();
				contactPoint.r1 = p1.lessNew(x1);
				contactPoint.r2 = p2.lessNew(x2);
				contactPoint.penetrationDepth = externalContact.getPenetrationDepth();
				contactPoint.isRestingContact = externalContact.getIsRestingContact();
				externalContact.setIsRestingContact(true);
				contactPoint.oldFrictionVector1 = externalContact.getFrictionVector1().clone();
				contactPoint.oldFrictionvec2 = externalContact.getFrictionvec2().clone();
				contactPoint.penetrationImpulse = 0.0f;
				contactPoint.friction1Impulse = 0.0f;
				contactPoint.friction2Impulse = 0.0f;
				contactPoint.rollingResistanceImpulse = new Vector3f(0.0f, 0.0f, 0.0f);
				// If we solve the friction raints at the center of the contact manifold
				if (this.isSolveFrictionAtContactManifoldCenterActive) {
					internalManifold.frictionPointBody1.add(p1);
					internalManifold.frictionPointBody2.add(p2);
					//Log.error("]]]    internalManifold.frictionPointBody1=" + internalManifold.frictionPointBody1);
					//Log.error("]]]    internalManifold.frictionPointBody2=" + internalManifold.frictionPointBody2);
				}
			}
			// If we solve the friction raints at the center of the contact manifold
			if (this.isSolveFrictionAtContactManifoldCenterActive) {
				internalManifold.frictionPointBody1.divide(internalManifold.nbContacts);
				internalManifold.frictionPointBody2.divide(internalManifold.nbContacts);
				//Log.error("]]]    internalManifold.frictionPointBody1=" + internalManifold.frictionPointBody1);
				//Log.error("]]]    internalManifold.frictionPointBody2=" + internalManifold.frictionPointBody2);
				internalManifold.r1Friction = internalManifold.frictionPointBody1.lessNew(x1);
				internalManifold.r2Friction = internalManifold.frictionPointBody2.lessNew(x2);
				//Log.error("]]]    internalManifold.r1Friction=" + internalManifold.r1Friction);
				//Log.error("]]]    internalManifold.r2Friction=" + internalManifold.r2Friction);
				internalManifold.oldFrictionVector1 = externalManifold.getFrictionVector1().clone();
				internalManifold.oldFrictionvec2 = externalManifold.getFrictionvec2().clone();
				//Log.error("]]]    internalManifold.oldFrictionVector1=" + internalManifold.oldFrictionVector1);
				//Log.error("]]]    internalManifold.oldFrictionvec2=" + internalManifold.oldFrictionvec2);
				// If warm starting is active
				if (this.isWarmStartingActive) {
					// Initialize the accumulated impulses with the previous step accumulated impulses
					internalManifold.friction1Impulse = externalManifold.getFrictionImpulse1();
					internalManifold.friction2Impulse = externalManifold.getFrictionImpulse2();
					internalManifold.frictionTwistImpulse = externalManifold.getFrictionTwistImpulse();
					//Log.error("]]]    internalManifold.friction1Impulse=" + FMath.floatToString(internalManifold.friction1Impulse));
					//Log.error("]]]    internalManifold.friction2Impulse=" + FMath.floatToString(internalManifold.friction2Impulse));
					//Log.error("]]]    internalManifold.frictionTwistImpulse=" + FMath.floatToString(internalManifold.frictionTwistImpulse));
				} else {
					// Initialize the accumulated impulses to zero
					internalManifold.friction1Impulse = 0.0f;
					internalManifold.friction2Impulse = 0.0f;
					internalManifold.frictionTwistImpulse = 0.0f;
					internalManifold.rollingResistanceImpulse = new Vector3f(0, 0, 0);
				}
			}
		}
		// Fill-in all the matrices needed to solve the LCP problem
		initializeContactConstraints();
	}
	
	/**
	 *  Get the split impulses position correction technique is used for contacts
	 * @return true the split status is Enable
	 * @return true the split status is Disable
	 */
	public boolean isSplitImpulseActive() {
		return this.isSplitImpulseActive;
	}
	
	/**
	 *  Set the rained velocities arrays
	 * @param _rainedLinearVelocities Constrained Linear velocities Table pointer (not free)
	 * @param _rainedAngularVelocities Constrained angular velocities Table pointer (not free)
	 */
	public void setConstrainedVelocitiesArrays(final Vector3f[] _rainedLinearVelocities, final Vector3f[] _rainedAngularVelocities) {
		assert (_rainedLinearVelocities != null);
		assert (_rainedAngularVelocities != null);
		this.linearVelocities = _rainedLinearVelocities;
		this.angularVelocities = _rainedAngularVelocities;
	}
	
	/**
	 * Activate or deactivate the solving of friction raints at the center of
	 * the contact manifold instead of solving them at each contact point
	 * @param _isActive Enable or not the center inertie
	 */
	public void setIsSolveFrictionAtContactManifoldCenterActive(final boolean _isActive) {
		this.isSolveFrictionAtContactManifoldCenterActive = _isActive;
	}
	
	/**
	 *  Activate or Deactivate the split impulses for contacts
	 * @param _isActive status to set.
	 */
	public void setIsSplitImpulseActive(final boolean _isActive) {
		this.isSplitImpulseActive = _isActive;
	}
	
	/**
	 *  Set the split velocities arrays
	 * @param _splitLinearVelocities Split linear velocities Table pointer (not free)
	 * @param _splitAngularVelocities Split angular velocities Table pointer (not free)
	 */
	public void setSplitVelocitiesArrays(final Vector3f[] _splitLinearVelocities, final Vector3f[] _splitAngularVelocities) {
		assert (_splitLinearVelocities != null);
		assert (_splitAngularVelocities != null);
		this.splitLinearVelocities = _splitLinearVelocities;
		this.splitAngularVelocities = _splitAngularVelocities;
	}
	
	/**
	 *  Solve the contacts
	 */
	public void solve() {
		//Log.warning("========================================");
		//Log.warning("== Contact SOLVER ... " + this.contactConstraints.size());
		//Log.warning("========================================");
		// For each contact manifold
		for (int ccc = 0; ccc < this.contactConstraints.size(); ++ccc) {
			//Log.warning("ccc=" + ccc + " / " + this.contactConstraints.size());
			final ContactManifoldSolver contactManifold = this.contactConstraints.get(ccc);
			float sumPenetrationImpulse = 0.0f;
			// Get the rained velocities
			final Vector3f v1 = this.linearVelocities[contactManifold.indexBody1];
			final Vector3f w1 = this.angularVelocities[contactManifold.indexBody1];
			final Vector3f v2 = this.linearVelocities[contactManifold.indexBody2];
			final Vector3f w2 = this.angularVelocities[contactManifold.indexBody2];
			//Log.warning("    v1=" + v1);
			//Log.warning("    w1=" + w1);
			//Log.warning("    v2=" + v2);
			//Log.warning("    w2=" + w2);
			for (int iii = 0; iii < contactManifold.nbContacts; ++iii) {
				//Log.warning("    iii=" + iii);
				final ContactPointSolver contactPoint = contactManifold.contacts[iii];
				// --------- Penetration --------- //
				// Compute J*v
				Vector3f deltaV = v2.addNew(w2.cross(contactPoint.r2)).less(v1).less(w1.cross(contactPoint.r1));
				final float deltaVDotN = deltaV.dot(contactPoint.normal);
				//Log.warning("        contactPoint.R1=" + contactPoint.r1);
				//Log.warning("        contactPoint.R2=" + contactPoint.r2);
				//Log.warning("        contactPoint.r1CrossN=" + contactPoint.r1CrossN);
				//Log.warning("        contactPoint.r2CrossN=" + contactPoint.r2CrossN);
				//Log.warning("        contactPoint.r1CrossT1=" + contactPoint.r1CrossT1);
				//Log.warning("        contactPoint.r2CrossT1=" + contactPoint.r2CrossT1);
				//Log.warning("        contactPoint.r1CrossT2=" + contactPoint.r1CrossT2);
				//Log.warning("        contactPoint.r2CrossT2=" + contactPoint.r2CrossT2);
				//Log.warning("        contactPoint.penetrationDepth=" + FMath.floatToString(contactPoint.penetrationDepth));
				//Log.warning("        contactPoint.normal=" + contactPoint.normal);
				//Log.warning("        deltaV=" + deltaV);
				float Jv = deltaVDotN;
				// Compute the bias "b" of the raint
				final float beta = this.isSplitImpulseActive ? BETA_SPLIT_IMPULSE : BETA;
				//Log.warning("        beta=" + FMath.floatToString(beta));
				//Log.warning("        BETA=" + FMath.floatToString(BETA));
				//Log.warning("        SLOP=" + FMath.floatToString(SLOP));
				//Log.warning("        this.timeStep=" + FMath.floatToString(this.timeStep));
				//Log.warning("        contactPoint.restitutionBias=" + FMath.floatToString(contactPoint.restitutionBias));
				float biasPenetrationDepth = 0.0f;
				if (contactPoint.penetrationDepth > SLOP) {
					//Log.warning("        (beta / this.timeStep)=" + FMath.floatToString((beta / this.timeStep)));
					//Log.warning("        FMath.max(0.0f, contactPoint.penetrationDepth - SLOP)=" + FMath.floatToString(FMath.max(0.0f, contactPoint.penetrationDepth - SLOP)));
					biasPenetrationDepth = -(beta / this.timeStep) * FMath.max(0.0f, contactPoint.penetrationDepth - SLOP);
					//Log.warning("        biasPenetrationDepth=" + FMath.floatToString(biasPenetrationDepth));
				}
				//Log.warning("        contactPoint.restitutionBias=" + FMath.floatToString(contactPoint.restitutionBias));
				//Log.warning("        biasPenetrationDepth=" + FMath.floatToString(biasPenetrationDepth));
				final float b = biasPenetrationDepth + contactPoint.restitutionBias;
				//Log.warning("        b=" + FMath.floatToString(b));
				// Compute the Lagrange multiplier lambda
				float deltaLambda;
				if (this.isSplitImpulseActive) {
					deltaLambda = -(Jv + contactPoint.restitutionBias) * contactPoint.inversePenetrationMass;
				} else {
					deltaLambda = -(Jv + b) * contactPoint.inversePenetrationMass;
				}
				//Log.warning("        deltaLambda=" + FMath.floatToString(deltaLambda));
				float lambdaTemp = contactPoint.penetrationImpulse;
				contactPoint.penetrationImpulse = FMath.max(contactPoint.penetrationImpulse + deltaLambda, 0.0f);
				deltaLambda = contactPoint.penetrationImpulse - lambdaTemp;
				//Log.warning("        contactPoint.penetrationImpulse=" + FMath.floatToString(contactPoint.penetrationImpulse));
				//Log.warning("        deltaLambda=" + FMath.floatToString(deltaLambda));
				// Compute the impulse P=J^T * lambda
				final Impulse impulsePenetration = computePenetrationImpulse(deltaLambda, contactPoint);
				//Log.warning("        impulsePenetration.a1=" + impulsePenetration.angularImpulseBody1);
				//Log.warning("        impulsePenetration.a2=" + impulsePenetration.angularImpulseBody2);
				//Log.warning("        impulsePenetration.i1=" + impulsePenetration.linearImpulseBody1);
				//Log.warning("        impulsePenetration.i2=" + impulsePenetration.linearImpulseBody2);
				// Apply the impulse to the bodies of the raint
				applyImpulse(impulsePenetration, contactManifold);
				sumPenetrationImpulse += contactPoint.penetrationImpulse;
				//Log.warning("        sumpenetrationImpulse=" + FMath.floatToString(sumPenetrationImpulse));
				//Log.warning("        isSplitImpulseActive=" + this.isSplitImpulseActive);
				//Log.warning("        isSolveFrictionAtContactManifoldCenterActive=" + this.isSolveFrictionAtContactManifoldCenterActive);
				// If the split impulse position correction is active
				if (this.isSplitImpulseActive) {
					// Split impulse (position correction)
					final Vector3f v1Split = this.splitLinearVelocities[contactManifold.indexBody1];
					final Vector3f w1Split = this.splitAngularVelocities[contactManifold.indexBody1];
					final Vector3f v2Split = this.splitLinearVelocities[contactManifold.indexBody2];
					final Vector3f w2Split = this.splitAngularVelocities[contactManifold.indexBody2];
					final Vector3f deltaVSplit = v2Split.addNew(w2Split.cross(contactPoint.r2)).less(v1Split).less(w1Split.cross(contactPoint.r1));
					//Log.warning("        deltaVSplit=" + deltaVSplit);
					final float JvSplit = deltaVSplit.dot(contactPoint.normal);
					final float deltaLambdaSplit = -(JvSplit + biasPenetrationDepth) * contactPoint.inversePenetrationMass;
					final float lambdaTempSplit = contactPoint.penetrationSplitImpulse;
					contactPoint.penetrationSplitImpulse = FMath.max(contactPoint.penetrationSplitImpulse + deltaLambdaSplit, 0.0f);
					deltaLambda = contactPoint.penetrationSplitImpulse - lambdaTempSplit;
					// Compute the impulse P=J^T * lambda
					final Impulse splitImpulsePenetration = computePenetrationImpulse(deltaLambdaSplit, contactPoint);
					//Log.warning("            splitImpulsePenetration.a1=" + splitImpulsePenetration.angularImpulseBody1);
					//Log.warning("            splitImpulsePenetration.a2=" + splitImpulsePenetration.angularImpulseBody2);
					//Log.warning("            splitImpulsePenetration.i1=" + splitImpulsePenetration.linearImpulseBody1);
					//Log.warning("            splitImpulsePenetration.i2=" + splitImpulsePenetration.linearImpulseBody2);
					applySplitImpulse(splitImpulsePenetration, contactManifold);
				}
				// If we do not solve the friction raints at the center of the contact manifold
				if (!this.isSolveFrictionAtContactManifoldCenterActive) {
					// --------- Friction 1 --------- //
					// Compute J*v
					deltaV = v2.addNew(w2.cross(contactPoint.r2)).less(v1).less(w1.cross(contactPoint.r1));
					Jv = deltaV.dot(contactPoint.frictionVector1);
					// Compute the Lagrange multiplier lambda
					deltaLambda = -Jv;
					deltaLambda *= contactPoint.inverseFriction1Mass;
					//Log.warning("        deltaLambda=" + FMath.floatToString(deltaLambda));
					float frictionLimit = contactManifold.frictionCoefficient * contactPoint.penetrationImpulse;
					lambdaTemp = contactPoint.friction1Impulse;
					contactPoint.friction1Impulse = FMath.max(-frictionLimit, FMath.min(contactPoint.friction1Impulse + deltaLambda, frictionLimit));
					deltaLambda = contactPoint.friction1Impulse - lambdaTemp;
					//Log.warning("        deltaLambda=" + FMath.floatToString(deltaLambda));
					// Compute the impulse P=J^T * lambda
					final Impulse impulseFriction1 = computeFriction1Impulse(deltaLambda, contactPoint);
					
					//Log.warning("            impulseFriction1.a1=" + impulseFriction1.angularImpulseBody1);
					//Log.warning("            impulseFriction1.a2=" + impulseFriction1.angularImpulseBody2);
					//Log.warning("            impulseFriction1.i1=" + impulseFriction1.linearImpulseBody1);
					//Log.warning("            impulseFriction1.i2=" + impulseFriction1.linearImpulseBody2);
					
					// Apply the impulses to the bodies of the raint
					applyImpulse(impulseFriction1, contactManifold);
					// --------- Friction 2 --------- //
					// Compute J*v
					deltaV = v2.addNew(w2.cross(contactPoint.r2)).less(v1).less(w1.cross(contactPoint.r1));
					Jv = deltaV.dot(contactPoint.frictionvec2);
					// Compute the Lagrange multiplier lambda
					deltaLambda = -Jv;
					deltaLambda *= contactPoint.inverseFriction2Mass;
					//Log.warning("        deltaLambda=" + FMath.floatToString(deltaLambda));
					frictionLimit = contactManifold.frictionCoefficient * contactPoint.penetrationImpulse;
					lambdaTemp = contactPoint.friction2Impulse;
					contactPoint.friction2Impulse = FMath.max(-frictionLimit, FMath.min(contactPoint.friction2Impulse + deltaLambda, frictionLimit));
					deltaLambda = contactPoint.friction2Impulse - lambdaTemp;
					//Log.warning("        deltaLambda=" + FMath.floatToString(deltaLambda));
					// Compute the impulse P=J^T * lambda
					final Impulse impulseFriction2 = computeFriction2Impulse(deltaLambda, contactPoint);
					//Log.warning("            impulseFriction2.a1=" + impulseFriction2.angularImpulseBody1);
					//Log.warning("            impulseFriction2.a2=" + impulseFriction2.angularImpulseBody2);
					//Log.warning("            impulseFriction2.i1=" + impulseFriction2.linearImpulseBody1);
					//Log.warning("            impulseFriction2.i2=" + impulseFriction2.linearImpulseBody2);
					// Apply the impulses to the bodies of the raint
					applyImpulse(impulseFriction2, contactManifold);
					// --------- Rolling resistance raint --------- //
					if (contactManifold.rollingResistanceFactor > 0) {
						// Compute J*v
						final Vector3f JvRolling = w2.lessNew(w1);
						// Compute the Lagrange multiplier lambda
						Vector3f deltaLambdaRolling = contactManifold.inverseRollingResistance.multiplyNew((JvRolling.multiplyNew(-1)));
						final float rollingLimit = contactManifold.rollingResistanceFactor * contactPoint.penetrationImpulse;
						final Vector3f lambdaTempRolling = contactPoint.rollingResistanceImpulse;
						contactPoint.rollingResistanceImpulse = contactPoint.rollingResistanceImpulse.addNew(deltaLambdaRolling).clampNew(rollingLimit);
						deltaLambdaRolling = contactPoint.rollingResistanceImpulse.lessNew(lambdaTempRolling);
						// Compute the impulse P=J^T * lambda
						final Impulse impulseRolling = new Impulse(new Vector3f(0.0f, 0.0f, 0.0f), deltaLambdaRolling.multiplyNew(-1), new Vector3f(0.0f, 0.0f, 0.0f), deltaLambdaRolling);
						//Log.warning("            impulseRolling.a1=" + impulseRolling.angularImpulseBody1);
						//Log.warning("            impulseRolling.a2=" + impulseRolling.angularImpulseBody2);
						//Log.warning("            impulseRolling.i1=" + impulseRolling.linearImpulseBody1);
						//Log.warning("            impulseRolling.i2=" + impulseRolling.linearImpulseBody2);
						// Apply the impulses to the bodies of the raint
						applyImpulse(impulseRolling, contactManifold);
					}
				}
			}
			//Log.info("    w1=" + w1);
			//Log.info("    w2=" + w2);
			// If we solve the friction raints at the center of the contact manifold
			if (this.isSolveFrictionAtContactManifoldCenterActive) {
				//Log.warning("   HHH   isSolveFrictionAtContactManifoldCenterActive");
				//Log.info("            v2=" + v2);
				
				// ------ First friction raint at the center of the contact manifol ------ //
				// Compute J*v
				Vector3f deltaV = v2.addNew(w2.cross(contactManifold.r2Friction)).less(v1).less(w1.cross(contactManifold.r1Friction));
				float Jv = deltaV.dot(contactManifold.frictionVector1);
				//Log.info("            v2=" + v2);
				//Log.warning("                Jv=" + FMath.floatToString(Jv));
				//Log.warning("                contactManifold.frictionVector1=" + contactManifold.frictionVector1);
				//Log.warning("                contactManifold.inverseFriction1Mass=" + FMath.floatToString(contactManifold.inverseFriction1Mass));
				// Compute the Lagrange multiplier lambda
				float deltaLambda = -Jv * contactManifold.inverseFriction1Mass;
				//Log.warning("                contactManifold.frictionCoefficient=" + FMath.floatToString(contactManifold.frictionCoefficient));
				//Log.warning("                sumPenetrationImpulse=" + FMath.floatToString(sumPenetrationImpulse));
				//Log.info("            v2=" + v2);
				float frictionLimit = contactManifold.frictionCoefficient * sumPenetrationImpulse;
				float lambdaTemp = contactManifold.friction1Impulse;
				//Log.warning("                frictionLimit=" + FMath.floatToString(frictionLimit));
				//Log.warning("                lambdaTemp=" + FMath.floatToString(lambdaTemp));
				//Log.info("            v2=" + v2);
				contactManifold.friction1Impulse = FMath.max(-frictionLimit, FMath.min(contactManifold.friction1Impulse + deltaLambda, frictionLimit));
				deltaLambda = contactManifold.friction1Impulse - lambdaTemp;
				//Log.warning("                deltaLambda=" + FMath.floatToString(deltaLambda));
				// Compute the impulse P=J^T * lambda
				Vector3f linearImpulseBody1 = contactManifold.frictionVector1.multiplyNew(-1).multiply(deltaLambda);
				Vector3f angularImpulseBody1 = contactManifold.r1CrossT1.multiplyNew(-1).multiply(deltaLambda);
				Vector3f linearImpulseBody2 = contactManifold.frictionVector1.multiplyNew(deltaLambda);
				Vector3f angularImpulseBody2 = contactManifold.r2CrossT1.multiplyNew(deltaLambda);
				
				//Log.info("            v2=" + v2);
				final Impulse impulseFriction1 = new Impulse(linearImpulseBody1, angularImpulseBody1, linearImpulseBody2, angularImpulseBody2);
				//Log.warning("            impulseFriction1.a1=" + impulseFriction1.angularImpulseBody1);
				//Log.warning("            impulseFriction1.a2=" + impulseFriction1.angularImpulseBody2);
				//Log.warning("            impulseFriction1.i1=" + impulseFriction1.linearImpulseBody1);
				//Log.warning("            impulseFriction1.i2=" + impulseFriction1.linearImpulseBody2);
				// Apply the impulses to the bodies of the raint
				applyImpulse(impulseFriction1, contactManifold);
				//Log.info("            v2=" + v2);
				//Log.info("            w2=" + w2);
				//Log.info("            v1=" + v1);
				//Log.info("            w1=" + w1);
				//Log.info("            contactManifold.r2Friction=" + contactManifold.r2Friction);
				//Log.info("            contactManifold.r1Friction=" + contactManifold.r1Friction);
				// ------ Second friction raint at the center of the contact manifol ----- //
				// Compute J*v
				deltaV = v2.addNew(w2.cross(contactManifold.r2Friction)).less(v1).less(w1.cross(contactManifold.r1Friction));
				//Log.warning(">>>>            deltaV=" + deltaV);
				Jv = deltaV.dot(contactManifold.frictionvec2);
				//Log.warning(">>>>            Jv=" + FMath.floatToString(Jv));
				//Log.warning(">>>>            contactManifold.inverseFriction2Mass=" + FMath.floatToString(contactManifold.inverseFriction2Mass));
				// Compute the Lagrange multiplier lambda
				deltaLambda = -Jv * contactManifold.inverseFriction2Mass;
				//Log.warning(">>>>            deltaLambda=" + FMath.floatToString(deltaLambda));
				frictionLimit = contactManifold.frictionCoefficient * sumPenetrationImpulse;
				lambdaTemp = contactManifold.friction2Impulse;
				//Log.warning(">>>>            lambdaTemp=" + FMath.floatToString(lambdaTemp));
				//Log.warning(">>>>            contactManifold.friction2Impulse=" + FMath.floatToString(contactManifold.friction2Impulse));
				//Log.warning(">>>>**            frictionLimit=" + FMath.floatToString(frictionLimit));
				contactManifold.friction2Impulse = FMath.max(-frictionLimit, FMath.min(contactManifold.friction2Impulse + deltaLambda, frictionLimit));
				//Log.warning(">>>>            contactManifold.friction2Impulse=" + FMath.floatToString(contactManifold.friction2Impulse));
				deltaLambda = contactManifold.friction2Impulse - lambdaTemp;
				//Log.warning(">>>>            deltaLambda=" + FMath.floatToString(deltaLambda));
				// Compute the impulse P=J^T * lambda
				linearImpulseBody1 = contactManifold.frictionvec2.multiplyNew(-deltaLambda);
				angularImpulseBody1 = contactManifold.r1CrossT2.multiplyNew(-deltaLambda);
				linearImpulseBody2 = contactManifold.frictionvec2.multiplyNew(deltaLambda);
				angularImpulseBody2 = contactManifold.r2CrossT2.multiplyNew(deltaLambda);
				
				final Impulse impulseFriction2 = new Impulse(linearImpulseBody1, angularImpulseBody1, linearImpulseBody2, angularImpulseBody2);
				//Log.warning("            impulseFriction2.a1=" + impulseFriction2.angularImpulseBody1);
				//Log.warning("            impulseFriction2.a2=" + impulseFriction2.angularImpulseBody2);
				//Log.warning("            impulseFriction2.i1=" + impulseFriction2.linearImpulseBody1);
				//Log.warning("            impulseFriction2.i2=" + impulseFriction2.linearImpulseBody2);
				// Apply the impulses to the bodies of the raint
				applyImpulse(impulseFriction2, contactManifold);
				//Log.info("            v2=" + v2);
				//Log.info("            w2=" + w2);
				//Log.info("            v1=" + v1);
				//Log.info("            w1=" + w1);
				// ------ Twist friction raint at the center of the contact manifol ------ //
				// Compute J*v
				deltaV = w2.lessNew(w1);
				//Log.warning("            deltaV=" + deltaV);
				//Log.warning("            contactManifold.normal=" + contactManifold.normal);
				Jv = deltaV.dot(contactManifold.normal);
				//Log.warning("            Jv=" + FMath.floatToString(Jv));
				//Log.warning("            contactManifold.inverseTwistFrictionMass=" + contactManifold.inverseTwistFrictionMass);
				deltaLambda = -Jv * (contactManifold.inverseTwistFrictionMass);
				//Log.warning("            deltaLambda=" + FMath.floatToString(deltaLambda));
				frictionLimit = contactManifold.frictionCoefficient * sumPenetrationImpulse;
				//Log.warning("            contactManifold.frictionCoefficient=" + contactManifold.frictionCoefficient);
				//Log.warning("            sumPenetrationImpulse=" + sumPenetrationImpulse);
				//Log.warning("            frictionLimit=" + FMath.floatToString(frictionLimit));
				lambdaTemp = contactManifold.frictionTwistImpulse;
				//Log.warning("            lambdaTemp=" + lambdaTemp);
				contactManifold.frictionTwistImpulse = FMath.max(-frictionLimit, FMath.min(contactManifold.frictionTwistImpulse + deltaLambda, frictionLimit));
				//Log.warning("            contactManifold.frictionTwistImpulse=" + contactManifold.frictionTwistImpulse);
				deltaLambda = contactManifold.frictionTwistImpulse - lambdaTemp;
				//Log.warning("            deltaLambda=" + deltaLambda);
				// Compute the impulse P=J^T * lambda
				linearImpulseBody1 = new Vector3f(0.0f, 0.0f, 0.0f);
				angularImpulseBody1 = contactManifold.normal.multiplyNew(-deltaLambda);
				linearImpulseBody2 = new Vector3f(0.0f, 0.0f, 0.0f);
				angularImpulseBody2 = contactManifold.normal.multiplyNew(deltaLambda);
				
				final Impulse impulseTwistFriction = new Impulse(linearImpulseBody1, angularImpulseBody1, linearImpulseBody2, angularImpulseBody2);
				//Log.warning("            impulseTwistFriction.a1=" + impulseTwistFriction.angularImpulseBody1);
				//Log.warning("            impulseTwistFriction.a2=" + impulseTwistFriction.angularImpulseBody2);
				//Log.warning("            impulseTwistFriction.i1=" + impulseTwistFriction.linearImpulseBody1);
				//Log.warning("            impulseTwistFriction.i2=" + impulseTwistFriction.linearImpulseBody2);
				// Apply the impulses to the bodies of the raint
				applyImpulse(impulseTwistFriction, contactManifold);
				// --------- Rolling resistance raint at the center of the contact manifold --------- //
				if (contactManifold.rollingResistanceFactor > 0) {
					// Compute J*v
					final Vector3f JvRolling = w2.lessNew(w1);
					// Compute the Lagrange multiplier lambda
					Vector3f deltaLambdaRolling = contactManifold.inverseRollingResistance.multiplyNew(JvRolling.multiplyNew(-1));
					final float rollingLimit = contactManifold.rollingResistanceFactor * sumPenetrationImpulse;
					final Vector3f lambdaTempRolling = contactManifold.rollingResistanceImpulse;
					contactManifold.rollingResistanceImpulse = (contactManifold.rollingResistanceImpulse.addNew(deltaLambdaRolling)).clampNew(rollingLimit);
					deltaLambdaRolling = contactManifold.rollingResistanceImpulse.lessNew(lambdaTempRolling);
					// Compute the impulse P=J^T * lambda
					angularImpulseBody1 = deltaLambdaRolling.multiplyNew(-1);
					angularImpulseBody2 = deltaLambdaRolling;
					
					final Impulse impulseRolling = new Impulse(new Vector3f(0.0f, 0.0f, 0.0f), angularImpulseBody1, new Vector3f(0.0f, 0.0f, 0.0f), angularImpulseBody2);
					//Log.warning("            impulseRolling.a1=" + impulseRolling.angularImpulseBody1);
					//Log.warning("            impulseRolling.a2=" + impulseRolling.angularImpulseBody2);
					//Log.warning("            impulseRolling.i1=" + impulseRolling.linearImpulseBody1);
					//Log.warning("            impulseRolling.i2=" + impulseRolling.linearImpulseBody2);
					// Apply the impulses to the bodies of the raint
					applyImpulse(impulseRolling, contactManifold);
				}
			}
		}
		//System.exit(-1);
	}
	
	/**
	 *  Store the computed impulses to use them to warm start the solver at the next iteration
	 */
	public void storeImpulses() {
		// For each contact manifold
		for (int ccc = 0; ccc < this.contactConstraints.size(); ++ccc) {
			final ContactManifoldSolver manifold = this.contactConstraints.get(ccc);
			for (int iii = 0; iii < manifold.nbContacts; ++iii) {
				final ContactPointSolver contactPoint = manifold.contacts[iii];
				contactPoint.externalContact.setPenetrationImpulse(contactPoint.penetrationImpulse);
				contactPoint.externalContact.setFrictionImpulse1(contactPoint.friction1Impulse);
				contactPoint.externalContact.setFrictionImpulse2(contactPoint.friction2Impulse);
				contactPoint.externalContact.setRollingResistanceImpulse(contactPoint.rollingResistanceImpulse.clone());
				contactPoint.externalContact.setFrictionVector1(contactPoint.frictionVector1.clone());
				contactPoint.externalContact.setFrictionvec2(contactPoint.frictionvec2.clone());
			}
			manifold.externalContactManifold.setFrictionImpulse1(manifold.friction1Impulse);
			manifold.externalContactManifold.setFrictionImpulse2(manifold.friction2Impulse);
			manifold.externalContactManifold.setFrictionTwistImpulse(manifold.frictionTwistImpulse);
			manifold.externalContactManifold.setRollingResistanceImpulse(manifold.rollingResistanceImpulse.clone());
			manifold.externalContactManifold.setFrictionVector1(manifold.frictionVector1.clone());
			manifold.externalContactManifold.setFrictionvec2(manifold.frictionvec2.clone());
		}
	}
	
	/**
	 *  Warm start the solver.
	 * For each raint, we apply the previous impulse (from the previous step)
	 * at the beginning. With this technique, we will converge faster towards the solution of the linear system
	 */
	public void warmStart() {
		// Check that warm starting is active
		if (!this.isWarmStartingActive) {
			return;
		}
		// For each raint
		for (int ccc = 0; ccc < this.contactConstraints.size(); ++ccc) {
			final ContactManifoldSolver contactManifold = this.contactConstraints.get(ccc);
			boolean atLeastOneRestingContactPoint = false;
			for (int iii = 0; iii < contactManifold.nbContacts; ++iii) {
				final ContactPointSolver contactPoint = contactManifold.contacts[iii];
				// If it is not a new contact (this contact was already existing at last time step)
				if (contactPoint.isRestingContact) {
					atLeastOneRestingContactPoint = true;
					// --------- Penetration --------- //
					// Compute the impulse P = J^T * lambda
					final Impulse impulsePenetration = computePenetrationImpulse(contactPoint.penetrationImpulse, contactPoint);
					// Apply the impulse to the bodies of the raint
					applyImpulse(impulsePenetration, contactManifold);
					// If we do not solve the friction raints at the center of the contact manifold
					if (!this.isSolveFrictionAtContactManifoldCenterActive) {
						// Project the old friction impulses (with old friction vectors) into
						// the new friction vectors to get the new friction impulses
						final Vector3f oldFrictionImpulse = contactPoint.oldFrictionVector1.multiplyNew(contactPoint.friction1Impulse)
								.add(contactPoint.oldFrictionvec2.multiplyNew(contactPoint.friction2Impulse));
						contactPoint.friction1Impulse = oldFrictionImpulse.dot(contactPoint.frictionVector1);
						contactPoint.friction2Impulse = oldFrictionImpulse.dot(contactPoint.frictionvec2);
						// --------- Friction 1 --------- //
						// Compute the impulse P = J^T * lambda
						final Impulse impulseFriction1 = computeFriction1Impulse(contactPoint.friction1Impulse, contactPoint);
						// Apply the impulses to the bodies of the raint
						applyImpulse(impulseFriction1, contactManifold);
						// --------- Friction 2 --------- //
						// Compute the impulse P=J^T * lambda
						final Impulse impulseFriction2 = computeFriction2Impulse(contactPoint.friction2Impulse, contactPoint);
						// Apply the impulses to the bodies of the raint
						applyImpulse(impulseFriction2, contactManifold);
						// ------ Rolling resistance------ //
						if (contactManifold.rollingResistanceFactor > 0) {
							// Compute the impulse P = J^T * lambda
							final Impulse impulseRollingResistance = new Impulse(new Vector3f(0.0f, 0.0f, 0.0f), contactPoint.rollingResistanceImpulse.multiplyNew(-1), new Vector3f(0.0f, 0.0f, 0.0f),
									contactPoint.rollingResistanceImpulse);
							// Apply the impulses to the bodies of the raint
							applyImpulse(impulseRollingResistance, contactManifold);
						}
					}
				} else {
					// If it is a new contact point
					// Initialize the accumulated impulses to zero
					contactPoint.penetrationImpulse = 0.0f;
					contactPoint.friction1Impulse = 0.0f;
					contactPoint.friction2Impulse = 0.0f;
					contactPoint.rollingResistanceImpulse = new Vector3f(0.0f, 0.0f, 0.0f);
				}
			}
			// If we solve the friction raints at the center of the contact manifold and there is
			// at least one resting contact point in the contact manifold
			if (this.isSolveFrictionAtContactManifoldCenterActive && atLeastOneRestingContactPoint) {
				
				// Project the old friction impulses (with old friction vectors) into the new friction
				// vectors to get the new friction impulses
				final Vector3f oldFrictionImpulse = contactManifold.oldFrictionVector1.multiplyNew(contactManifold.friction1Impulse)
						.add(contactManifold.oldFrictionvec2.multiplyNew(contactManifold.friction2Impulse));
				
				//Log.error("]]]    oldFrictionImpulse=" + oldFrictionImpulse);
				contactManifold.friction1Impulse = oldFrictionImpulse.dot(contactManifold.frictionVector1);
				contactManifold.friction2Impulse = oldFrictionImpulse.dot(contactManifold.frictionvec2);
				//Log.error("]]]    contactManifold.friction1Impulse=" + contactManifold.friction1Impulse);
				//Log.error("]]]    contactManifold.friction2Impulse=" + contactManifold.friction2Impulse);
				// ------ First friction raint at the center of the contact manifold ------ //
				// Compute the impulse P = J^T * lambda
				Vector3f linearImpulseBody1 = contactManifold.frictionVector1.multiplyNew(-1).multiply(contactManifold.friction1Impulse);
				Vector3f angularImpulseBody1 = contactManifold.r1CrossT1.multiplyNew(-1).multiply(contactManifold.friction1Impulse);
				Vector3f linearImpulseBody2 = contactManifold.frictionVector1.multiplyNew(contactManifold.friction1Impulse);
				Vector3f angularImpulseBody2 = contactManifold.r2CrossT1.multiplyNew(contactManifold.friction1Impulse);
				
				//Log.error("]]]    linearImpulseBody1=" + linearImpulseBody1);
				//Log.error("]]]    angularImpulseBody1=" + angularImpulseBody1);
				//Log.error("]]]    linearImpulseBody2=" + linearImpulseBody2);
				//Log.error("]]]    angularImpulseBody2=" + angularImpulseBody2);
				final Impulse impulseFriction1 = new Impulse(linearImpulseBody1, angularImpulseBody1, linearImpulseBody2, angularImpulseBody2);
				// Apply the impulses to the bodies of the raint
				applyImpulse(impulseFriction1, contactManifold);
				// ------ Second friction raint at the center of the contact manifold ----- //
				// Compute the impulse P = J^T * lambda
				linearImpulseBody1 = contactManifold.frictionvec2.multiplyNew(-1).multiply(contactManifold.friction2Impulse);
				angularImpulseBody1 = contactManifold.r1CrossT2.multiplyNew(-1).multiply(contactManifold.friction2Impulse);
				linearImpulseBody2 = contactManifold.frictionvec2.multiplyNew(contactManifold.friction2Impulse);
				angularImpulseBody2 = contactManifold.r2CrossT2.multiplyNew(contactManifold.friction2Impulse);
				
				//Log.error("]]]    linearImpulseBody1=" + linearImpulseBody1);
				//Log.error("]]]    angularImpulseBody1=" + angularImpulseBody1);
				//Log.error("]]]    linearImpulseBody2=" + linearImpulseBody2);
				//Log.error("]]]    angularImpulseBody2=" + angularImpulseBody2);
				final Impulse impulseFriction2 = new Impulse(linearImpulseBody1, angularImpulseBody1, linearImpulseBody2, angularImpulseBody2);
				// Apply the impulses to the bodies of the raint
				applyImpulse(impulseFriction2, contactManifold);
				// ------ Twist friction raint at the center of the contact manifold ------ //
				// Compute the impulse P = J^T * lambda
				linearImpulseBody1 = new Vector3f(0.0f, 0.0f, 0.0f);
				angularImpulseBody1 = contactManifold.normal.multiplyNew(contactManifold.frictionTwistImpulse);
				linearImpulseBody2 = new Vector3f(0.0f, 0.0f, 0.0f);
				angularImpulseBody2 = contactManifold.normal.multiplyNew(contactManifold.frictionTwistImpulse);
				
				//Log.error("]]]    linearImpulseBody1=" + linearImpulseBody1);
				//Log.error("]]]    angularImpulseBody1=" + angularImpulseBody1);
				//Log.error("]]]    linearImpulseBody2=" + linearImpulseBody2);
				//Log.error("]]]    angularImpulseBody2=" + angularImpulseBody2);
				final Impulse impulseTwistFriction = new Impulse(linearImpulseBody1, angularImpulseBody1, linearImpulseBody2, angularImpulseBody2);
				// Apply the impulses to the bodies of the raint
				applyImpulse(impulseTwistFriction, contactManifold);
				// ------ Rolling resistance at the center of the contact manifold ------ //
				// Compute the impulse P = J^T * lambda
				angularImpulseBody1 = contactManifold.rollingResistanceImpulse.multiplyNew(-1);
				angularImpulseBody2 = contactManifold.rollingResistanceImpulse;
				
				//Log.error("]]]    angularImpulseBody1=" + angularImpulseBody1);
				//Log.error("]]]    angularImpulseBody2=" + angularImpulseBody2);
				final Impulse impulseRollingResistance = new Impulse(new Vector3f(0.0f, 0.0f, 0.0f), angularImpulseBody1, new Vector3f(0.0f, 0.0f, 0.0f), angularImpulseBody2);
				// Apply the impulses to the bodies of the raint
				applyImpulse(impulseRollingResistance, contactManifold);
			} else
			
			{
				// If it is a new contact manifold
				// Initialize the accumulated impulses to zero
				contactManifold.friction1Impulse = 0.0f;
				contactManifold.friction2Impulse = 0.0f;
				contactManifold.frictionTwistImpulse = 0.0f;
				contactManifold.rollingResistanceImpulse = new Vector3f(0.0f, 0.0f, 0.0f);
			}
		}
	}
}
