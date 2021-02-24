package org.atriasoft.ephysics.engine;

import org.atriasoft.ephysics.constraint.ContactPointInfo;

/**
 *  This class can be used to register a callback for collision test queries.
 * You should implement your own class inherited from this one and implement
 * the notifyRaycastHit() method. This method will be called for each ProxyShape
 * that is hit by the ray.
 */
public interface CollisionCallback {
	/**
	 *  This method will be called for contact.
	 * @param _contactPointInfo Contact information property.
	 */
	void notifyContact(ContactPointInfo _contactPointInfo);
}
