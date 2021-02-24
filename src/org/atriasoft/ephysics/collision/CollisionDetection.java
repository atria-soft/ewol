package org.atriasoft.ephysics.collision;

import java.util.Iterator;
import java.util.Map;
import java.util.TreeMap;

import org.atriasoft.ephysics.RaycastCallback;
import org.atriasoft.ephysics.RaycastTest;
import org.atriasoft.ephysics.body.BodyType;
import org.atriasoft.ephysics.body.CollisionBody;
import org.atriasoft.ephysics.collision.broadphase.BroadPhaseAlgorithm;
import org.atriasoft.ephysics.collision.broadphase.DTree;
import org.atriasoft.ephysics.collision.broadphase.PairDTree;
import org.atriasoft.ephysics.collision.narrowphase.CollisionDispatch;
import org.atriasoft.ephysics.collision.narrowphase.DefaultCollisionDispatch;
import org.atriasoft.ephysics.collision.narrowphase.NarrowPhaseAlgorithm;
import org.atriasoft.ephysics.collision.narrowphase.NarrowPhaseCallback;
import org.atriasoft.ephysics.collision.narrowphase.GJK.GJKAlgorithm;
import org.atriasoft.ephysics.collision.shapes.AABB;
import org.atriasoft.ephysics.collision.shapes.CollisionShape;
import org.atriasoft.ephysics.collision.shapes.CollisionShapeType;
import org.atriasoft.ephysics.constraint.ContactPoint;
import org.atriasoft.ephysics.constraint.ContactPointInfo;
import org.atriasoft.ephysics.engine.CollisionCallback;
import org.atriasoft.ephysics.engine.CollisionWorld;
import org.atriasoft.ephysics.engine.EventListener;
import org.atriasoft.ephysics.engine.OverlappingPair;
import org.atriasoft.ephysics.internal.Log;
import org.atriasoft.ephysics.mathematics.PairInt;
import org.atriasoft.ephysics.mathematics.Ray;
import org.atriasoft.ephysics.mathematics.Set;
import org.atriasoft.ephysics.mathematics.SetMultiple;
import org.atriasoft.etk.math.Vector3f;

/*
 * @brief It computes the collision detection algorithms. We first
 * perform a broad-phase algorithm to know which pairs of bodies can
 * collide and then we run a narrow-phase algorithm to compute the
 * collision contacts between bodies.
 */
public class CollisionDetection implements NarrowPhaseCallback {
	/// Reference on the physics world
	private final CollisionWorld world;
	/// Broad-phase algorithm
	private final BroadPhaseAlgorithm broadPhaseAlgorithm;
	/// Collision Detection Dispatch configuration
	private CollisionDispatch collisionDispatch;
	private final DefaultCollisionDispatch defaultCollisionDispatch = new DefaultCollisionDispatch();
	/// Collision detection matrix (algorithms to use)
	private final NarrowPhaseAlgorithm[][] collisionMatrix = new NarrowPhaseAlgorithm[CollisionShapeType.NB_COLLISION_SHAPE_TYPES][CollisionShapeType.NB_COLLISION_SHAPE_TYPES];
	public Map<PairDTree, OverlappingPair> overlappingPairs = new TreeMap<>(Set.getPairDTreeCoparator()); //!< Broad-phase overlapping pairs
	private final Map<PairDTree, OverlappingPair> contactOverlappingPair = new TreeMap<>(Set.getPairDTreeCoparator()); //!< Overlapping pairs in contact (during the current Narrow-phase collision detection)
	// TODO : Delete this
	private final GJKAlgorithm narrowPhaseGJKAlgorithm; //!< Narrow-phase GJK algorithm
	private final SetMultiple<PairInt> noCollisionPairs = new SetMultiple<PairInt>(Set.getPairIntCoparator()); //!< Set of pair of bodies that cannot collide between each other
	private boolean isCollisionShapesAdded; //!< True if some collision shapes have been added previously
	
	/// Constructor
	public CollisionDetection(final CollisionWorld _world) {
		this.world = _world;
		this.broadPhaseAlgorithm = new BroadPhaseAlgorithm(this);
		this.isCollisionShapesAdded = false;
		this.narrowPhaseGJKAlgorithm = new GJKAlgorithm(this);
		// Set the default collision dispatch configuration
		setCollisionDispatch(this.defaultCollisionDispatch);
		// Fill-in the collision detection matrix with algorithms
		fillInCollisionMatrix();
	}
	
	/// Add all the contact manifold of colliding pairs to their bodies
	private void addAllContactManifoldsToBodies() {
		// For each overlapping pairs in contact during the narrow-phase
		for (final OverlappingPair it : this.contactOverlappingPair.values()) {
			// Add all the contact manifolds of the pair int32_to the list of contact manifolds
			// of the two bodies involved in the contact
			addContactManifoldToBody(it);
		}
	}
	
	/// Add a contact manifold to the linked list of contact manifolds of the two bodies
	/// involed in the corresponding contact.
	private void addContactManifoldToBody(final OverlappingPair _pair) {
		assert (_pair != null);
		final CollisionBody body1 = _pair.getShape1().getBody();
		final CollisionBody body2 = _pair.getShape2().getBody();
		final ContactManifoldSet manifoldSet = _pair.getContactManifoldSet();
		// For each contact manifold in the set of manifolds in the pair
		for (int i = 0; i < manifoldSet.getNbContactManifolds(); i++) {
			final ContactManifold contactManifold = manifoldSet.getContactManifold(i);
			assert (contactManifold.getNbContactPoints() > 0);
			// Add the contact manifold at the beginning of the linked
			// list of contact manifolds of the first body
			body1.contactManifoldsList = new ContactManifoldListElement(contactManifold, body1.contactManifoldsList);
			
			// Add the contact manifold at the beginning of the linked
			// list of the contact manifolds of the second body
			body2.contactManifoldsList = new ContactManifoldListElement(contactManifold, body2.contactManifoldsList);
			
		}
	}
	
	/// Add a pair of bodies that cannot collide with each other
	public void addNoCollisionPair(final CollisionBody _body1, final CollisionBody _body2) {
		this.noCollisionPairs.add(OverlappingPair.computeBodiesIndexPair(_body1, _body2));
	}
	
	/// Add a proxy collision shape to the collision detection
	public void addProxyCollisionShape(final ProxyShape _proxyShape, final AABB _aabb) {
		// Add the body to the broad-phase
		this.broadPhaseAlgorithm.addProxyCollisionShape(_proxyShape, _aabb);
		this.isCollisionShapesAdded = true;
	}
	
	// Ask for a collision shape to be tested again during broad-phase.
	/// We simply put the shape in the list of collision shape that have moved in the
	/// previous frame so that it is tested for collision again in the broad-phase.
	public void askForBroadPhaseCollisionCheck(final ProxyShape _shape) {
		this.broadPhaseAlgorithm.addMovedCollisionShape(_shape.broadPhaseID);
	}
	
	/// Allow the broadphase to notify the collision detection about an overlapping pair.
	/// This method is called by the broad-phase collision detection algorithm
	public void broadPhaseNotifyOverlappingPair(final ProxyShape _shape1, final ProxyShape _shape2) {
		assert (_shape1.broadPhaseID != _shape2.broadPhaseID);
		// If the two proxy collision shapes are from the same body, skip it
		if (_shape1.getBody().getID() == _shape2.getBody().getID()) {
			return;
		}
		//Log.info(" check collision is allowed: " + _shape1.getBody().getID() + "  " + _shape2.getBody().getID());
		// Check if the collision filtering allows collision between the two shapes
		if ((_shape1.getCollideWithMaskBits() & _shape2.getCollisionCategoryBits()) == 0 || (_shape1.getCollisionCategoryBits() & _shape2.getCollideWithMaskBits()) == 0) {
			Log.info("    ==> not permited ...");
			return;
		}
		// Compute the overlapping pair ID
		final PairDTree pairID = OverlappingPair.computeID(_shape1, _shape2);
		// Check if the overlapping pair already exists
		if (this.overlappingPairs.get(pairID) != null) {
			return;
		}
		// Compute the maximum number of contact manifolds for this pair
		final int nbMaxManifolds = CollisionShape.computeNbMaxContactManifolds(_shape1.getCollisionShape().getType(), _shape2.getCollisionShape().getType());
		// Create the overlapping pair and add it int32_to the set of overlapping pairs
		final OverlappingPair newPair = new OverlappingPair(_shape1, _shape2, nbMaxManifolds);
		assert (newPair != null);
		this.overlappingPairs.putIfAbsent(pairID, newPair);
		// Wake up the two bodies
		_shape1.getBody().setIsSleeping(false);
		_shape2.getBody().setIsSleeping(false);
	}
	
	/// Delete all the contact points in the currently overlapping pairs
	private void clearContactPoints() {
		// For each overlapping pair
		this.overlappingPairs.forEach((key, value) -> value.clearContactPoints());
	}
	
	/// Compute the broad-phase collision detection
	private void computeBroadPhase() {
		// If new collision shapes have been added to bodies
		if (this.isCollisionShapesAdded) {
			// Ask the broad-phase to recompute the overlapping pairs of collision
			// shapes. This call can only add new overlapping pairs in the collision
			// detection.
			this.broadPhaseAlgorithm.computeOverlappingPairs();
		}
	}
	
	/// Compute the collision detection
	public void computeCollisionDetection() {
		//Log.info("computeBroadPhase();");
		// Compute the broad-phase collision detection
		computeBroadPhase();
		//Log.info("computeNarrowPhase();");
		// Compute the narrow-phase collision detection
		computeNarrowPhase();
	}
	
	/// Compute the narrow-phase collision detection
	private void computeNarrowPhase() {
		// Clear the set of overlapping pairs in narrow-phase contact
		this.contactOverlappingPair.clear();
		{
			//Log.info("list elements:");
			final Iterator<Map.Entry<PairDTree, OverlappingPair>> ittt = this.overlappingPairs.entrySet().iterator();
			while (ittt.hasNext()) {
				final Map.Entry<PairDTree, OverlappingPair> entry = ittt.next();
				//Log.info("    " + entry.getKey() + "  " + entry.getValue());
				
			}
		}
		// For each possible collision pair of bodies
		final Iterator<Map.Entry<PairDTree, OverlappingPair>> it = this.overlappingPairs.entrySet().iterator();
		
		while (it.hasNext()) {
			final Map.Entry<PairDTree, OverlappingPair> entry = it.next();
			final OverlappingPair pair = entry.getValue();
			final ProxyShape shape1 = pair.getShape1();
			final ProxyShape shape2 = pair.getShape2();
			assert (shape1.broadPhaseID != shape2.broadPhaseID);
			// Check if the collision filtering allows collision between the two shapes and
			// that the two shapes are still overlapping. Otherwise, we destroy the
			// overlapping pair
			if (((shape1.getCollideWithMaskBits() & shape2.getCollisionCategoryBits()) == 0 || (shape1.getCollisionCategoryBits() & shape2.getCollideWithMaskBits()) == 0)
					|| !this.broadPhaseAlgorithm.testOverlappingShapes(shape1, shape2)) {
				// TODO : Remove all the contact manifold of the overlapping pair from the contact manifolds list of the two bodies involved
				// Destroy the overlapping pair
				it.remove();
				continue;
			}
			final CollisionBody body1 = shape1.getBody();
			final CollisionBody body2 = shape2.getBody();
			// Update the contact cache of the overlapping pair
			pair.update();
			// Check that at least one body is awake and not static
			final boolean isBody1Active = !body1.isSleeping() && body1.getType() != BodyType.STATIC;
			final boolean isBody2Active = !body2.isSleeping() && body2.getType() != BodyType.STATIC;
			if (!isBody1Active && !isBody2Active) {
				continue;
			}
			// Check if the bodies are in the set of bodies that cannot collide between each other
			final PairInt bodiesIndex = OverlappingPair.computeBodiesIndexPair(body1, body2);
			if (this.noCollisionPairs.count(bodiesIndex) > 0) {
				continue;
			}
			// Select the narrow phase algorithm to use according to the two collision shapes
			final CollisionShapeType shape1Type = shape1.getCollisionShape().getType();
			final CollisionShapeType shape2Type = shape2.getCollisionShape().getType();
			final NarrowPhaseAlgorithm narrowPhaseAlgorithm = this.collisionMatrix[shape1Type.value][shape2Type.value];
			// If there is no collision algorithm between those two kinds of shapes
			if (narrowPhaseAlgorithm == null) {
				continue;
			}
			// Notify the narrow-phase algorithm about the overlapping pair we are going to test
			narrowPhaseAlgorithm.setCurrentOverlappingPair(pair);
			// Create the CollisionShapeInfo objects
			final CollisionShapeInfo shape1Info = new CollisionShapeInfo(shape1, shape1.getCollisionShape(), shape1.getLocalToWorldTransform(), pair, shape1.getCachedCollisionData());
			
			final CollisionShapeInfo shape2Info = new CollisionShapeInfo(shape2, shape2.getCollisionShape(), shape2.getLocalToWorldTransform(), pair, shape2.getCachedCollisionData());
			
			// Use the narrow-phase collision detection algorithm to check
			// if there really is a collision. If a collision occurs, the
			// notifyContact() callback method will be called.
			narrowPhaseAlgorithm.testCollision(shape1Info, shape2Info, this);
		}
		
		// Add all the contact manifolds (between colliding bodies) to the bodies
		addAllContactManifoldsToBodies();
		
	}
	
	/// Compute the narrow-phase collision detection
	public void computeNarrowPhaseBetweenShapes(final CollisionCallback _callback, final Set<DTree> _shapes1, final Set<DTree> _shapes2) {
		this.contactOverlappingPair.clear();
		// For each possible collision pair of bodies
		final Iterator<Map.Entry<PairDTree, OverlappingPair>> it = this.overlappingPairs.entrySet().iterator();
		while (it.hasNext()) {
			final Map.Entry<PairDTree, OverlappingPair> entry = it.next();
			final OverlappingPair pair = entry.getValue();
			final ProxyShape shape1 = pair.getShape1();
			final ProxyShape shape2 = pair.getShape2();
			assert (shape1.broadPhaseID != shape2.broadPhaseID);
			// If both shapes1 and shapes2 sets are non-empty, we check that
			// shape1 is among on set and shape2 is among the other one
			if (!_shapes1.isEmpty() && !_shapes2.isEmpty() && (_shapes1.count(shape1.broadPhaseID) == 0 || _shapes2.count(shape2.broadPhaseID) == 0)
					&& (_shapes1.count(shape2.broadPhaseID) == 0 || _shapes2.count(shape1.broadPhaseID) == 0)) {
				continue;
			}
			if (!_shapes1.isEmpty() && _shapes2.isEmpty() && _shapes1.count(shape1.broadPhaseID) == 0 && _shapes1.count(shape2.broadPhaseID) == 0) {
				continue;
			}
			if (!_shapes2.isEmpty() && _shapes1.isEmpty() && _shapes2.count(shape1.broadPhaseID) == 0 && _shapes2.count(shape2.broadPhaseID) == 0) {
				continue;
			}
			// Check if the collision filtering allows collision between the two shapes and
			// that the two shapes are still overlapping. Otherwise, we destroy the
			// overlapping pair
			if (((shape1.getCollideWithMaskBits() & shape2.getCollisionCategoryBits()) == 0 || (shape1.getCollisionCategoryBits() & shape2.getCollideWithMaskBits()) == 0)
					|| !this.broadPhaseAlgorithm.testOverlappingShapes(shape1, shape2)) {
				// TODO : Remove all the contact manifold of the overlapping pair from the contact manifolds list of the two bodies involved
				// Destroy the overlapping pair
				it.remove();
				continue;
			}
			final CollisionBody body1 = shape1.getBody();
			final CollisionBody body2 = shape2.getBody();
			// Update the contact cache of the overlapping pair
			pair.update();
			// Check if the two bodies are allowed to collide, otherwise, we do not test for collision
			if (body1.getType() != BodyType.DYNAMIC && body2.getType() != BodyType.DYNAMIC) {
				continue;
			}
			final PairInt bodiesIndex = OverlappingPair.computeBodiesIndexPair(body1, body2);
			if (this.noCollisionPairs.count(bodiesIndex) > 0) {
				continue;
			}
			// Check if the two bodies are sleeping, if so, we do no test collision between them
			if (body1.isSleeping() && body2.isSleeping()) {
				continue;
			}
			// Select the narrow phase algorithm to use according to the two collision shapes
			final CollisionShapeType shape1Type = shape1.getCollisionShape().getType();
			final CollisionShapeType shape2Type = shape2.getCollisionShape().getType();
			final NarrowPhaseAlgorithm narrowPhaseAlgorithm = this.collisionMatrix[shape1Type.value][shape2Type.value];
			// If there is no collision algorithm between those two kinds of shapes
			if (narrowPhaseAlgorithm == null) {
				continue;
			}
			// Notify the narrow-phase algorithm about the overlapping pair we are going to test
			narrowPhaseAlgorithm.setCurrentOverlappingPair(pair);
			// Create the CollisionShapeInfo objects
			final CollisionShapeInfo shape1Info = new CollisionShapeInfo(shape1, shape1.getCollisionShape(), shape1.getLocalToWorldTransform(), pair, shape1.getCachedCollisionData());
			
			final CollisionShapeInfo shape2Info = new CollisionShapeInfo(shape2, shape2.getCollisionShape(), shape2.getLocalToWorldTransform(), pair, shape2.getCachedCollisionData());
			
			final TestCollisionBetweenShapesCallback narrowPhaseCallback = new TestCollisionBetweenShapesCallback(_callback);
			// Use the narrow-phase collision detection algorithm to check
			// if there really is a collision
			narrowPhaseAlgorithm.testCollision(shape1Info, shape2Info, narrowPhaseCallback);
		}
		
		// Add all the contact manifolds (between colliding bodies) to the bodies
		addAllContactManifoldsToBodies();
	}
	
	void createContact(final OverlappingPair _overlappingPair, final ContactPointInfo _contactInfo) {
		// Create a new contact
		final ContactPoint contact = new ContactPoint(_contactInfo);
		// Add the contact to the contact manifold set of the corresponding overlapping pair
		_overlappingPair.addContact(contact);
		// Add the overlapping pair int32_to the set of pairs in contact during narrow-phase
		final PairDTree pairId = OverlappingPair.computeID(_overlappingPair.getShape1(), _overlappingPair.getShape2());
		this.contactOverlappingPair.put(pairId, _overlappingPair);
	}
	
	/// Fill-in the collision detection matrix
	private void fillInCollisionMatrix() {
		// For each possible type of collision shape
		for (int i = 0; i < CollisionShapeType.NB_COLLISION_SHAPE_TYPES; i++) {
			for (int j = 0; j < CollisionShapeType.NB_COLLISION_SHAPE_TYPES; j++) {
				this.collisionMatrix[i][j] = this.collisionDispatch.selectAlgorithm(CollisionShapeType.getType(i), CollisionShapeType.getType(j));
			}
		}
	}
	
	/// Return the Narrow-phase collision detection algorithm to use between two types of shapes
	public NarrowPhaseAlgorithm getCollisionAlgorithm(final CollisionShapeType _shape1Type, final CollisionShapeType _shape2Type) {
		return this.collisionMatrix[_shape1Type.value][_shape2Type.value];
	}
	
	public GJKAlgorithm getNarrowPhaseGJKAlgorithm() {
		return this.narrowPhaseGJKAlgorithm;
	}
	
	/// Return a pointer to the world
	public CollisionWorld getWorld() {
		return this.world;
	}
	
	/// Return the world event listener
	public EventListener getWorldEventListener() {
		return this.world.eventListener;
	}
	
	/// Called by a narrow-phase collision algorithm when a new contact has been found
	@Override
	public void notifyContact(final OverlappingPair _overlappingPair, final ContactPointInfo _contactInfo) {
		Log.error("Notify Contact ...     --------------------");
		// If it is the first contact since the pairs are overlapping
		if (_overlappingPair.getNbContactPoints() == 0) {
			// Trigger a callback event
			if (this.world.eventListener != null) {
				this.world.eventListener.beginContact(_contactInfo);
			}
		}
		// Create a new contact
		createContact(_overlappingPair, _contactInfo);
		// Trigger a callback event for the new contact
		if (this.world.eventListener != null) {
			this.world.eventListener.newContact(_contactInfo);
		}
	}
	
	/// Ray casting method
	public void raycast(final RaycastCallback _raycastCallback, final Ray _ray, final int _raycastWithCategoryMaskBits) {
		final RaycastTest rayCastTest = new RaycastTest(_raycastCallback);
		// Ask the broad-phase algorithm to call the testRaycastAgainstShape()
		// callback method for each proxy shape hit by the ray in the broad-phase
		this.broadPhaseAlgorithm.raycast(_ray, rayCastTest, _raycastWithCategoryMaskBits);
	}
	
	/// Remove a pair of bodies that cannot collide with each other
	public void removeNoCollisionPair(final CollisionBody _body1, final CollisionBody _body2) {
		this.noCollisionPairs.erase(this.noCollisionPairs.find(OverlappingPair.computeBodiesIndexPair(_body1, _body2)));
	}
	
	/// Remove a proxy collision shape from the collision detection
	public void removeProxyCollisionShape(final ProxyShape _proxyShape) {
		// Remove all the overlapping pairs involving this proxy shape
		final Iterator<Map.Entry<PairDTree, OverlappingPair>> it = this.overlappingPairs.entrySet().iterator();
		while (it.hasNext()) {
			final Map.Entry<PairDTree, OverlappingPair> entry = it.next();
			final OverlappingPair pair = entry.getValue();
			if (pair.getShape1().broadPhaseID == _proxyShape.broadPhaseID || pair.getShape2().broadPhaseID == _proxyShape.broadPhaseID) {
				// TODO : Remove all the contact manifold of the overlapping pair from the contact manifolds list of the two bodies involved
				// Destroy the overlapping pair
				it.remove();
			}
		}
		// Remove the body from the broad-phase
		this.broadPhaseAlgorithm.removeProxyCollisionShape(_proxyShape);
	}
	
	/// Report collision between two sets of shapes
	public void reportCollisionBetweenShapes(final CollisionCallback _callback, final Set<DTree> _shapes1, final Set<DTree> _shapes2) {
		
		// For each possible collision pair of bodies
		final Iterator<Map.Entry<PairDTree, OverlappingPair>> it = this.overlappingPairs.entrySet().iterator();
		while (it.hasNext()) {
			final Map.Entry<PairDTree, OverlappingPair> entry = it.next();
			final OverlappingPair pair = entry.getValue();
			final ProxyShape shape1 = pair.getShape1();
			final ProxyShape shape2 = pair.getShape2();
			assert (shape1.broadPhaseID != shape2.broadPhaseID);
			// If both shapes1 and shapes2 sets are non-empty, we check that
			// shape1 is among on set and shape2 is among the other one
			if (!_shapes1.isEmpty() && !_shapes2.isEmpty() && (_shapes1.count(shape1.broadPhaseID) == 0 || _shapes2.count(shape2.broadPhaseID) == 0)
					&& (_shapes1.count(shape2.broadPhaseID) == 0 || _shapes2.count(shape1.broadPhaseID) == 0)) {
				continue;
			}
			if (!_shapes1.isEmpty() && _shapes2.isEmpty() && _shapes1.count(shape1.broadPhaseID) == 0 && _shapes1.count(shape2.broadPhaseID) == 0) {
				continue;
			}
			if (!_shapes2.isEmpty() && _shapes1.isEmpty() && _shapes2.count(shape1.broadPhaseID) == 0 && _shapes2.count(shape2.broadPhaseID) == 0) {
				continue;
			}
			// For each contact manifold set of the overlapping pair
			final ContactManifoldSet manifoldSet = pair.getContactManifoldSet();
			for (int j = 0; j < manifoldSet.getNbContactManifolds(); j++) {
				final ContactManifold manifold = manifoldSet.getContactManifold(j);
				// For each contact manifold of the manifold set
				for (int i = 0; i < manifold.getNbContactPoints(); i++) {
					final ContactPoint contactPoint = manifold.getContactPoint(i);
					// Create the contact info object for the contact
					final ContactPointInfo contactInfo = new ContactPointInfo(manifold.getShape1(), manifold.getShape2(), manifold.getShape1().getCollisionShape(),
							manifold.getShape2().getCollisionShape(), contactPoint.getNormal(), contactPoint.getPenetrationDepth(), contactPoint.getLocalPointOnBody1(),
							contactPoint.getLocalPointOnBody2());
					// Notify the collision callback about this new contact
					if (_callback != null) {
						_callback.notifyContact(contactInfo);
					}
				}
				
			}
		}
	}
	
	/// Set the collision dispatch configuration
	public void setCollisionDispatch(final CollisionDispatch _collisionDispatch) {
		this.collisionDispatch = _collisionDispatch;
		this.collisionDispatch.init(this);
		// Fill-in the collision matrix with the new algorithms to use
		fillInCollisionMatrix();
	}
	
	/// Test if the AABBs of two proxy shapes overlap
	public boolean testAABBOverlap(final ProxyShape _shape1, final ProxyShape _shape2) {
		// If one of the shape's body is not active, we return no overlap
		if (!_shape1.getBody().isActive() || !_shape2.getBody().isActive()) {
			return false;
		}
		return this.broadPhaseAlgorithm.testOverlappingShapes(_shape1, _shape2);
	}
	
	/// Compute the collision detection
	public void testCollisionBetweenShapes(final CollisionCallback _callback, final Set<DTree> _shapes1, final Set<DTree> _shapes2) {
		// Compute the broad-phase collision detection
		computeBroadPhase();
		// Delete all the contact points in the currently overlapping pairs
		clearContactPoints();
		// Compute the narrow-phase collision detection among given sets of shapes
		computeNarrowPhaseBetweenShapes(_callback, _shapes1, _shapes2);
	}
	
	/// Update a proxy collision shape (that has moved for instance)
	public void updateProxyCollisionShape(final ProxyShape _shape, final AABB _aabb) {
		updateProxyCollisionShape(_shape, _aabb, new Vector3f(0, 0, 0), false);
	}
	
	public void updateProxyCollisionShape(final ProxyShape _shape, final AABB _aabb, final Vector3f _displacement) {
		updateProxyCollisionShape(_shape, _aabb, _displacement, false);
	}
	
	public void updateProxyCollisionShape(final ProxyShape _shape, final AABB _aabb, final Vector3f _displacement, final boolean _forceReinsert) {
		this.broadPhaseAlgorithm.updateProxyCollisionShape(_shape, _aabb, _displacement);
	}
}
