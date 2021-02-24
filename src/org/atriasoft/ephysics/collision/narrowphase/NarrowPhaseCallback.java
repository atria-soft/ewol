package org.atriasoft.ephysics.collision.narrowphase;

import org.atriasoft.ephysics.constraint.ContactPointInfo;
import org.atriasoft.ephysics.engine.OverlappingPair;

/**
 * It is the base class for a narrow-phase collision
 * callback class.
 */
public interface NarrowPhaseCallback {
	/// Called by a narrow-phase collision algorithm when a new contact has been found
	void notifyContact(OverlappingPair _overlappingPair, ContactPointInfo _contactInfo);
	
};