package org.atriasoft.ephysics.engine;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.ephysics.RaycastCallback;
import org.atriasoft.ephysics.body.CollisionBody;
import org.atriasoft.ephysics.collision.CollisionDetection;
import org.atriasoft.ephysics.collision.ProxyShape;
import org.atriasoft.ephysics.collision.broadphase.DTree;
import org.atriasoft.ephysics.collision.narrowphase.CollisionDispatch;
import org.atriasoft.ephysics.collision.shapes.AABB;
import org.atriasoft.ephysics.mathematics.Ray;
import org.atriasoft.ephysics.mathematics.Set;
import org.atriasoft.etk.math.Transform3D;

/**
 *  This class represent a world where it is possible to move bodies
 * by hand and to test collision between each other. In this kind of
 * world, the bodies movement is not computed using the laws of physics.
 */
public class CollisionWorld {
	
	public CollisionDetection collisionDetection; //!< Reference to the collision detection
	protected Set<CollisionBody> bodies = new Set<CollisionBody>(Set.getCollisionBodyCoparator()); //!< All the bodies (rigid and soft) of the world
	protected int currentBodyID; //!< Current body ID
	protected List<Integer> freeBodiesIDs = new ArrayList<>(); //!< List of free ID for rigid bodies
	public EventListener eventListener; //!< Pointer to an event listener object
	/// Return the next available body ID
	
	/// Constructor
	public CollisionWorld() {
		this.collisionDetection = new CollisionDetection(this);
		this.currentBodyID = 0;
		this.eventListener = null;
	}
	
	int computeNextAvailableBodyID() {
		// Compute the body ID
		int bodyID;
		if (!this.freeBodiesIDs.isEmpty()) {
			bodyID = this.freeBodiesIDs.get(0);
			this.freeBodiesIDs.remove(bodyID);
		} else {
			bodyID = this.currentBodyID;
			this.currentBodyID++;
		}
		return bodyID;
	}
	
	/*
	public Set<CollisionBody>::Iterator getBodiesEndIterator() {
			return this.bodies.end();
		}
	*/
	/**
	*  Create a collision body and add it to the world
	* @param transform Transform3Dation mapping the local-space of the body to world-space
	* @return A pointer to the body that has been created in the world
	*/
	public CollisionBody createCollisionBody(final Transform3D transform) {
		// Get the next available body ID
		final int bodyID = computeNextAvailableBodyID();
		// Largest index cannot be used (it is used for invalid index)
		assert (bodyID < Integer.MAX_VALUE);
		// Create the collision body
		final CollisionBody collisionBody = new CollisionBody(transform, this, bodyID);
		assert (collisionBody != null);
		// Add the collision body to the world
		this.bodies.add(collisionBody);
		// Return the pointer to the rigid body
		return collisionBody;
	}
	
	/**
	 *  Destroy a collision body
	 * @param collisionBody Pointer to the body to destroy
	 */
	public void destroyCollisionBody(CollisionBody collisionBody) {
		// Remove all the collision shapes of the body
		collisionBody.removeAllCollisionShapes();
		// Add the body ID to the list of free IDs
		this.freeBodiesIDs.add(collisionBody.getID());
		// Remove the collision body from the list of bodies
		this.bodies.erase(collisionBody);
		collisionBody = null;
	}
	
	/**
	*  Get an iterator to the beginning of the bodies of the physics world
	* @return An starting iterator to the set of bodies of the world
	*/
	/*
	public Set<CollisionBody>::Iterator getBodiesBeginIterator() {
			return this.bodies.begin();
		}
	*/
	/**
	*  Get an iterator to the end of the bodies of the physics world
	* @return An ending iterator to the set of bodies of the world
	*/
	
	public CollisionDetection getCollisionDetection() {
		return this.collisionDetection;
	}
	
	/**
	 *  Ray cast method
	 * @param _ray Ray to use for raycasting
	 * @param _raycastCallback Pointer to the class with the callback method
	 * @param _raycastWithCategoryMaskBits Bits mask corresponding to the category of bodies to be raycasted
	 */
	public void raycast(final Ray _ray, final RaycastCallback _raycastCallback) {
		raycast(_ray, _raycastCallback, 0xFFFF);
	}
	
	public void raycast(final Ray _ray, final RaycastCallback _raycastCallback, final int _raycastWithCategoryMaskBits) {
		this.collisionDetection.raycast(_raycastCallback, _ray, _raycastWithCategoryMaskBits);
	}
	
	/// Reset all the contact manifolds linked list of each body
	void resetContactManifoldListsOfBodies() {
		// For each rigid body of the world
		for (final CollisionBody it : this.bodies) {
			// Reset the contact manifold list of the body
			it.resetContactManifoldsList();
		}
	}
	
	/**
	 *  Set the collision dispatch configuration
	 * This can be used to replace default collision detection algorithms by your
	 * custom algorithm for instance.
	 * @param _CollisionDispatch Pointer to a collision dispatch object describing
	 * which collision detection algorithm to use for two given collision shapes
	 */
	public void setCollisionDispatch(final CollisionDispatch _collisionDispatch) {
		this.collisionDetection.setCollisionDispatch(_collisionDispatch);
	}
	
	/**
	 *  Test if the AABBs of two bodies overlap
	 * @param _body1 Pointer to the first body to test
	 * @param _body2 Pointer to the second body to test
	 * @return True if the AABBs of the two bodies overlap and false otherwise
	 */
	public boolean testAABBOverlap(final CollisionBody _body1, final CollisionBody _body2) {
		// If one of the body is not active, we return no overlap
		if (!_body1.isActive() || !_body2.isActive()) {
			return false;
		}
		// Compute the AABBs of both bodies
		final AABB body1AABB = _body1.getAABB();
		final AABB body2AABB = _body2.getAABB();
		// Return true if the two AABBs overlap
		return body1AABB.testCollision(body2AABB);
	}
	
	/**
	 *  Test if the AABBs of two proxy shapes overlap
	 * @param _shape1 Pointer to the first proxy shape to test
	 * @param _shape2 Pointer to the second proxy shape to test
	 */
	public boolean testAABBOverlap(final ProxyShape _shape1, final ProxyShape _shape2) {
		return this.collisionDetection.testAABBOverlap(_shape1, _shape2);
	}
	
	/**
	 *  Test and report collisions between two bodies
	 * @param _body1 Pointer to the first body to test
	 * @param _body2 Pointer to the second body to test
	 * @param _callback Pointer to the object with the callback method
	 */
	public void testCollision(final CollisionBody _body1, final CollisionBody _body2, final CollisionCallback _callback) {
		// Reset all the contact manifolds lists of each body
		resetContactManifoldListsOfBodies();
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
		this.collisionDetection.testCollisionBetweenShapes(_callback, shapes1, shapes2);
	}
	
	/**
	 *  Test and report collisions between a body and all the others bodies of the world.
	 * @param _body Pointer to the first body to test
	 * @param _callback Pointer to the object with the callback method
	 */
	public void testCollision(final CollisionBody _body, final CollisionCallback _callback) {
		// Reset all the contact manifolds lists of each body
		resetContactManifoldListsOfBodies();
		// Create the sets of shapes
		final Set<DTree> shapes1 = new Set<DTree>(Set.getDTreeCoparator());
		// For each shape of the body
		for (ProxyShape shape = _body.getProxyShapesList(); shape != null; shape = shape.getNext()) {
			shapes1.add(shape.broadPhaseID);
		}
		final Set<DTree> emptySet = new Set<DTree>(Set.getDTreeCoparator());
		// Perform the collision detection and report contacts
		this.collisionDetection.testCollisionBetweenShapes(_callback, shapes1, emptySet);
	}
	
	/**
	 *  Test and report collisions between all shapes of the world
	 * @param _callback Pointer to the object with the callback method
	 */
	public void testCollision(final CollisionCallback _callback) {
		// Reset all the contact manifolds lists of each body
		resetContactManifoldListsOfBodies();
		final Set<DTree> emptySet = new Set<DTree>(Set.getDTreeCoparator());
		// Perform the collision detection and report contacts
		this.collisionDetection.testCollisionBetweenShapes(_callback, emptySet, emptySet);
	}
	
	/**
	 *  Test and report collisions between a given shape and all the others shapes of the world.
	 * @param _shape Pointer to the proxy shape to test
	 * @param _callback Pointer to the object with the callback method
	 */
	public void testCollision(final ProxyShape _shape, final CollisionCallback _callback) {
		// Reset all the contact manifolds lists of each body
		resetContactManifoldListsOfBodies();
		// Create the sets of shapes
		final Set<DTree> shapes = new Set<DTree>(Set.getDTreeCoparator());
		shapes.add(_shape.broadPhaseID);
		final Set<DTree> emptySet = new Set<DTree>(Set.getDTreeCoparator());
		// Perform the collision detection and report contacts
		this.collisionDetection.testCollisionBetweenShapes(_callback, shapes, emptySet);
	}
	
	/**
	 * Test and report collisions between two given shapes
	 * @param _shape1 Pointer to the first proxy shape to test
	 * @param _shape2 Pointer to the second proxy shape to test
	 * @param _callback Pointer to the object with the callback method
	 */
	public void testCollision(final ProxyShape _shape1, final ProxyShape _shape2, final CollisionCallback _callback) {
		// Reset all the contact manifolds lists of each body
		resetContactManifoldListsOfBodies();
		// Create the sets of shapes
		final Set<DTree> shapes1 = new Set<DTree>(Set.getDTreeCoparator());
		shapes1.add(_shape1.broadPhaseID);
		final Set<DTree> shapes2 = new Set<DTree>(Set.getDTreeCoparator());
		shapes2.add(_shape2.broadPhaseID);
		// Perform the collision detection and report contacts
		this.collisionDetection.testCollisionBetweenShapes(_callback, shapes1, shapes2);
	}
}
