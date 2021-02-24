package org.atriasoft.ephysics.constraint;

import org.atriasoft.etk.math.Vector3f;

import org.atriasoft.ephysics.collision.ProxyShape;
import org.atriasoft.ephysics.collision.shapes.CollisionShape;

/**
 *  This structure contains informations about a collision contact
 * computed during the narrow-phase collision detection. Those
 * informations are used to compute the contact set for a contact
 * between two bodies.
 */
public class ContactPointInfo {
	public ProxyShape shape1; //!< First proxy shape of the contact
	public ProxyShape shape2; //!< Second proxy shape of the contact
	public CollisionShape collisionShape1; //!< First collision shape
	public CollisionShape collisionShape2; //!< Second collision shape
	public Vector3f normal; //!< Normalized normal vector of the collision contact in world space
	public float penetrationDepth; //!< Penetration depth of the contact
	public Vector3f localPoint1; //!< Contact point of body 1 in local space of body 1
	public Vector3f localPoint2; //!< Contact point of body 2 in local space of body 2
	
	public ContactPointInfo() {
		this.shape1 = null;
		this.shape2 = null;
		this.collisionShape1 = null;
		this.collisionShape2 = null;
	}
	
	public ContactPointInfo(final ContactPointInfo obj) {
		this.shape1 = obj.shape1;
		this.shape2 = obj.shape2;
		this.collisionShape1 = obj.collisionShape1;
		this.collisionShape2 = obj.collisionShape2;
		this.normal = obj.normal;
		this.penetrationDepth = obj.penetrationDepth;
		this.localPoint1 = obj.localPoint1;
		this.localPoint2 = obj.localPoint2;
	}
	
	public ContactPointInfo(final ProxyShape _proxyShape1, final ProxyShape _proxyShape2, final CollisionShape _collShape1, final CollisionShape _collShape2, final Vector3f _normal,
			final float _penetrationDepth, final Vector3f _localPoint1, final Vector3f _localPoint2) {
		this.shape1 = _proxyShape1;
		this.shape2 = _proxyShape2;
		this.collisionShape1 = _collShape1;
		this.collisionShape2 = _collShape2;
		this.normal = _normal;
		this.penetrationDepth = _penetrationDepth;
		this.localPoint1 = _localPoint1;
		this.localPoint2 = _localPoint2;
		
	}
};