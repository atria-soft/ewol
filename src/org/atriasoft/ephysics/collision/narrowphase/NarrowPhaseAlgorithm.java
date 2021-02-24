package org.atriasoft.ephysics.collision.narrowphase;

import org.atriasoft.ephysics.collision.CollisionDetection;
import org.atriasoft.ephysics.collision.CollisionShapeInfo;
import org.atriasoft.ephysics.engine.OverlappingPair;

/**
 * @breif It is the base class for a  narrow-phase collision
 * detection algorithm. The goal of the narrow phase algorithm is to
 * compute information about the contact between two proxy shapes.
 */
public abstract class NarrowPhaseAlgorithm {
	
	protected CollisionDetection collisionDetection; //!< Pointer to the collision detection object
	protected OverlappingPair currentOverlappingPair; //!< Overlapping pair of the bodies currently tested for collision
	
	/// Constructor
	public NarrowPhaseAlgorithm(final CollisionDetection collisionDetection) {
		this.currentOverlappingPair = null;
		this.collisionDetection = null;
		this.collisionDetection = collisionDetection;
	}
	
	/// Set the current overlapping pair of bodies
	public void setCurrentOverlappingPair(final OverlappingPair overlappingPair) {
		this.currentOverlappingPair = overlappingPair;
	}
	
	/// Compute a contact info if the two bounding volume collide
	public abstract void testCollision(final CollisionShapeInfo _shape1Info, CollisionShapeInfo _shape2Info, NarrowPhaseCallback _narrowPhaseCallback);
	
}