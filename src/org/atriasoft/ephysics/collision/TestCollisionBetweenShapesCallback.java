package org.atriasoft.ephysics.collision;

import org.atriasoft.ephysics.collision.narrowphase.NarrowPhaseCallback;
import org.atriasoft.ephysics.constraint.ContactPointInfo;
import org.atriasoft.ephysics.engine.CollisionCallback;
import org.atriasoft.ephysics.engine.OverlappingPair;

public class TestCollisionBetweenShapesCallback implements NarrowPhaseCallback {
	
	private final CollisionCallback collisionCallback;
	
	// Constructor
	public TestCollisionBetweenShapesCallback(final CollisionCallback _callback) {
		this.collisionCallback = _callback;
		
	}
	
	// Called by a narrow-phase collision algorithm when a new contact has been found
	public void notifyContact(final OverlappingPair _overlappingPair, final ContactPointInfo _contactInfo) {
		this.collisionCallback.notifyContact(_contactInfo);
	}
}
