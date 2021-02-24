package org.atriasoft.ephysics.collision.narrowphase;

import org.atriasoft.ephysics.collision.CollisionDetection;
import org.atriasoft.ephysics.collision.shapes.CollisionShapeType;

/**
 * @biref Abstract base class for dispatching the narrow-phase
 * collision detection algorithm. Collision dispatching decides which collision
 * algorithm to use given two types of proxy collision shapes.
 */
public abstract class CollisionDispatch {
	
	/// Initialize the collision dispatch configuration
	public void init(final CollisionDetection _collisionDetection) {
		// Nothing to do ...
	}
	
	/// Select and return the narrow-phase collision detection algorithm to
	/// use between two types of collision shapes.
	public abstract NarrowPhaseAlgorithm selectAlgorithm(CollisionShapeType _shape1Type, CollisionShapeType _shape2Type);
}
