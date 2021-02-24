package org.atriasoft.ephysics.collision;

import org.atriasoft.ephysics.RaycastInfo;
import org.atriasoft.ephysics.body.CollisionBody;
import org.atriasoft.ephysics.collision.broadphase.DTree;
import org.atriasoft.ephysics.collision.shapes.CollisionShape;
import org.atriasoft.ephysics.mathematics.Ray;
import org.atriasoft.etk.math.Transform3D;
import org.atriasoft.etk.math.Vector3f;

public class ProxyShape {
	protected CollisionBody body; //!< Pointer to the parent body
	protected CollisionShape collisionShape; //!< Internal collision shape
	public Transform3D localToBodyTransform; //!< Local-space to parent body-space transform (does not change over time)
	protected float mass; //!< Mass (in kilogramms) of the corresponding collision shape
	public ProxyShape next = null; //!< Pointer to the next proxy shape of the body (linked list)
	public DTree broadPhaseID = null; //!< Broad-phase ID (node ID in the dynamic AABB tree)
	
	protected Object cachedCollisionData = null; //!< Cached collision data
	
	protected Object userData = null; //!< Pointer to user data
	
	/**
	 * @brief Bits used to define the collision category of this shape.
	 * You can set a single bit to one to define a category value for this
	 * shape. This value is one (0x0001) by default. This variable can be used
	 * together with the this.collideWithMaskBits variable so that given
	 * categories of shapes collide with each other and do not collide with
	 * other categories.
	 */
	protected int collisionCategoryBits = 0x0001;
	/**
	 * @brief Bits mask used to state which collision categories this shape can
	 * collide with. This value is 0xFFFF by default. It means that this
	 * proxy shape will collide with every collision categories by default.
	 */
	protected int collideWithMaskBits = 0xFFFF;
	
	/**
	 * @param body Pointer to the parent body
	 * @param shape Pointer to the collision shape
	 * @param transform Transform3Dation from collision shape local-space to body local-space
	 * @param mass Mass of the collision shape (in kilograms)
	 */
	public ProxyShape(final CollisionBody body, final CollisionShape shape, final Transform3D transform, final float mass) {
		this.body = body;
		this.collisionShape = shape;
		this.localToBodyTransform = transform;
		this.mass = mass;
	}
	
	/**
	 * @return Pointer to the parent body
	 */
	public CollisionBody getBody() {
		return this.body;
	}
	
	public DTree getBroadPhaseID() {
		return this.broadPhaseID;
	}
	
	/// Return the pointer to the cached collision data
	public Object getCachedCollisionData() {
		return this.cachedCollisionData;
	}
	
	/// Return the collision bits mask
	public int getCollideWithMaskBits() {
		return this.collideWithMaskBits;
	}
	
	/// Return the collision category bits
	public int getCollisionCategoryBits() {
		return this.collisionCategoryBits;
	}
	
	/// Return the collision shape
	public CollisionShape getCollisionShape() {
		return this.collisionShape;
	}
	
	/// Return the local scaling vector of the collision shape
	public Vector3f getLocalScaling() {
		return this.collisionShape.getScaling();
	}
	
	/**
	 * Return the local to parent body transform
	 * @return The transformation that transforms the local-space of the collision shape
	 *		 to the local-space of the parent body
	 */
	public Transform3D getLocalToBodyTransform() {
		return this.localToBodyTransform;
	}
	
	/// Return the local to world transform
	public Transform3D getLocalToWorldTransform() {
		return this.body.getTransform().multiplyNew(this.localToBodyTransform);
	}
	
	/// Return the mass of the collision shape
	public float getMass() {
		return this.mass;
	}
	
	/// Return the next proxy shape in the linked list of proxy shapes
	public ProxyShape getNext() {
		return this.next;
	}
	
	/// Return a pointer to the user data attached to this body
	public Object getUserData() {
		return this.userData;
	}
	
	/**
	 * @param ray Ray to use for the raycasting
	 * @param[out] raycastInfo Result of the raycasting that is valid only if the
	 *			 methods returned true
	 * @return True if the ray hit the collision shape
	 */
	public boolean raycast(final Ray ray, final RaycastInfo raycastInfo) {
		
		// If the corresponding body is not active, it cannot be hit by rays
		if (!this.body.isActive()) {
			return false;
		}
		
		// Convert the ray longo the local-space of the collision shape
		final Transform3D localToWorldTransform = getLocalToWorldTransform();
		final Transform3D worldToLocalTransform = localToWorldTransform.inverseNew();
		
		final Ray rayLocal = new Ray(worldToLocalTransform.multiplyNew(ray.point1), worldToLocalTransform.multiplyNew(ray.point2), ray.maxFraction);
		
		final boolean isHit = this.collisionShape.raycast(rayLocal, raycastInfo, this);
		if (isHit == true) {
			// Convert the raycast info longo world-space
			raycastInfo.worldPoint = localToWorldTransform.multiplyNew(raycastInfo.worldPoint);
			raycastInfo.worldNormal = localToWorldTransform.getOrientation().multiply(raycastInfo.worldNormal);
			raycastInfo.worldNormal.normalize();
		}
		return isHit;
	}
	
	public void setBroadPhaseID(final DTree broadPhase) {
		this.broadPhaseID = broadPhase;
	}
	
	/// Set the collision bits mask
	public void setCollideWithMaskBits(final int collideWithMaskBits) {
		this.collideWithMaskBits = collideWithMaskBits;
	}
	
	/// Set the collision category bits
	public void setCollisionCategoryBits(final int collisionCategoryBits) {
		this.collisionCategoryBits = collisionCategoryBits;
	}
	
	/**
	 * Set the local scaling vector of the collision shape
	 * @param scaling The new local scaling vector
	 */
	public void setLocalScaling(final Vector3f scaling) {
		
		// Set the local scaling of the collision shape
		this.collisionShape.setLocalScaling(scaling);
		
		this.body.setIsSleeping(false);
		
		// Notify the body that the proxy shape has to be updated in the broad-phase
		this.body.updateProxyShapeInBroadPhase(this, true);
	}
	
	/// Set the local to parent body transform
	public void setLocalToBodyTransform(final Transform3D transform) {
		
		this.localToBodyTransform = transform;
		
		this.body.setIsSleeping(false);
		
		// Notify the body that the proxy shape has to be updated in the broad-phase
		this.body.updateProxyShapeInBroadPhase(this, true);
	}
	
	/// Attach user data to this body
	public void setUserData(final Object userData) {
		this.userData = userData;
	}
	
	/**
	 * @param worldPoint Point to test in world-space coordinates
	 * @return True if the point is inside the collision shape
	 */
	public boolean testPointInside(final Vector3f worldPoint) {
		final Transform3D localToWorld = this.body.getTransform().multiplyNew(this.localToBodyTransform);
		final Vector3f localPoint = localToWorld.inverseNew().multiply(worldPoint);
		return this.collisionShape.testPointInside(localPoint, this);
	}
	
}
