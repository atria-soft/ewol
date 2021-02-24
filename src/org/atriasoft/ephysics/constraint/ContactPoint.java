package org.atriasoft.ephysics.constraint;

import org.atriasoft.etk.math.Vector3f;

import org.atriasoft.ephysics.body.CollisionBody;

/**
 *  This class represents a collision contact point between two
 * bodies in the physics engine.
 */
public class ContactPoint {
	
	private final CollisionBody body1; //!< First rigid body of the contact
	private final CollisionBody body2; //!< Second rigid body of the contact
	private final Vector3f normal; //!< Normalized normal vector of the contact (from body1 toward body2) in world space
	
	private float penetrationDepth; //!< Penetration depth
	private final Vector3f localPointOnBody1; //!< Contact point on body 1 in local space of body 1
	private final Vector3f localPointOnBody2; //!< Contact point on body 2 in local space of body 2
	private Vector3f worldPointOnBody1; //!< Contact point on body 1 in world space
	private Vector3f worldPointOnBody2; //!< Contact point on body 2 in world space
	private boolean isRestingContact; //!< True if the contact is a resting contact (exists for more than one time step)
	private Vector3f frictionVectors_1; //!< Two orthogonal vectors that span the tangential friction plane
	private Vector3f frictionVectors_2; //!< Two orthogonal vectors that span the tangential friction plane
	private float penetrationImpulse; //!< Cached penetration impulse
	private float frictionImpulse1; //!< Cached first friction impulse
	private float frictionImpulse2; //!< Cached second friction impulse
	private Vector3f rollingResistanceImpulse; //!< Cached rolling resistance impulse
	/// Constructor
	
	public ContactPoint(final ContactPointInfo contactInfo) {
		this.body1 = contactInfo.shape1.getBody();
		this.body2 = contactInfo.shape2.getBody();
		this.normal = contactInfo.normal.clone();
		this.penetrationDepth = contactInfo.penetrationDepth;
		this.localPointOnBody1 = contactInfo.localPoint1.clone();
		this.localPointOnBody2 = contactInfo.localPoint2.clone();
		this.worldPointOnBody1 = contactInfo.shape1.getBody().getTransform().multiplyNew(contactInfo.shape1.getLocalToBodyTransform().multiply(contactInfo.localPoint1));
		this.worldPointOnBody2 = contactInfo.shape2.getBody().getTransform().multiplyNew(contactInfo.shape2.getLocalToBodyTransform().multiply(contactInfo.localPoint2));
		this.isRestingContact = false;
		
		this.frictionVectors_1 = new Vector3f(0, 0, 0);
		this.frictionVectors_2 = new Vector3f(0, 0, 0);
		
		assert (this.penetrationDepth >= 0.0f);
		
	}
	
	/// Return the reference to the body 1
	public CollisionBody getBody1() {
		return this.body1;
	}
	
	/// Return the reference to the body 2
	public CollisionBody getBody2() {
		return this.body2;
	}
	
	/// Return the cached first friction impulse
	public float getFrictionImpulse1() {
		return this.frictionImpulse1;
	}
	
	/// Return the cached second friction impulse
	public float getFrictionImpulse2() {
		return this.frictionImpulse2;
	}
	
	/// Get the second friction vector
	public Vector3f getFrictionvec2() {
		return this.frictionVectors_2;
	}
	
	/// Get the first friction vector
	public Vector3f getFrictionVector1() {
		return this.frictionVectors_1;
	}
	
	/// Return true if the contact is a resting contact
	public boolean getIsRestingContact() {
		return this.isRestingContact;
	}
	
	/// Return the contact local point on body 1
	public Vector3f getLocalPointOnBody1() {
		return this.localPointOnBody1;
	}
	
	/// Return the contact local point on body 2
	public Vector3f getLocalPointOnBody2() {
		return this.localPointOnBody2;
	}
	
	/// Return the normal vector of the contact
	public Vector3f getNormal() {
		return this.normal;
	}
	
	/// Return the penetration depth
	public float getPenetrationDepth() {
		return this.penetrationDepth;
	}
	
	/// Return the cached penetration impulse
	public float getPenetrationImpulse() {
		return this.penetrationImpulse;
	}
	
	/// Return the cached rolling resistance impulse
	public Vector3f getRollingResistanceImpulse() {
		return this.rollingResistanceImpulse;
	}
	
	/// Return the contact world point on body 1
	public Vector3f getWorldPointOnBody1() {
		return this.worldPointOnBody1;
	}
	
	/// Return the contact world point on body 2
	public Vector3f getWorldPointOnBody2() {
		return this.worldPointOnBody2;
	}
	
	/// Set the first cached friction impulse
	public void setFrictionImpulse1(final float impulse) {
		this.frictionImpulse1 = impulse;
	}
	
	/// Set the second cached friction impulse
	public void setFrictionImpulse2(final float impulse) {
		this.frictionImpulse2 = impulse;
	}
	
	/// Set the second friction vector
	public void setFrictionvec2(final Vector3f frictionvec2) {
		this.frictionVectors_2 = frictionvec2;
	}
	
	/// Set the first friction vector
	public void setFrictionVector1(final Vector3f frictionVector1) {
		this.frictionVectors_1 = frictionVector1;
	}
	
	/// Set the this.isRestingContact variable
	public void setIsRestingContact(final boolean isRestingContact) {
		this.isRestingContact = isRestingContact;
	}
	
	/// Set the penetration depth of the contact
	public void setPenetrationDepth(final float penetrationDepth) {
		this.penetrationDepth = penetrationDepth;
	}
	
	/// Set the cached penetration impulse
	public void setPenetrationImpulse(final float impulse) {
		this.penetrationImpulse = impulse;
	}
	
	/// Set the cached rolling resistance impulse
	public void setRollingResistanceImpulse(final Vector3f impulse) {
		this.rollingResistanceImpulse = impulse;
	}
	
	/// Set the contact world point on body 1
	public void setWorldPointOnBody1(final Vector3f worldPoint) {
		this.worldPointOnBody1 = worldPoint;
	}
	
	/// Set the contact world point on body 2
	public void setWorldPointOnBody2(final Vector3f worldPoint) {
		this.worldPointOnBody2 = worldPoint;
	}
	
}
