package org.atriasoft.ephysics.collision.narrowphase;

import org.atriasoft.ephysics.collision.CollisionDetection;
import org.atriasoft.ephysics.collision.narrowphase.GJK.GJKAlgorithm;
import org.atriasoft.ephysics.collision.shapes.CollisionShape;
import org.atriasoft.ephysics.collision.shapes.CollisionShapeType;

/**
 *  This is the default collision dispatch configuration use in ephysics.
 * Collision dispatching decides which collision
 * algorithm to use given two types of proxy collision shapes.
 */
public class DefaultCollisionDispatch extends CollisionDispatch {
	
	//!< Sphere vs Sphere collision algorithm
	protected SphereVsSphereAlgorithm sphereVsSphereAlgorithm;
	//!< Concave vs Convex collision algorithm
	protected ConcaveVsConvexAlgorithm concaveVsConvexAlgorithm;
	//!< GJK Algorithm
	protected GJKAlgorithm GJKAlgorithm;
	
	@Override
	public void init(final CollisionDetection _collisionDetection) {
		// Initialize the collision algorithms
		this.sphereVsSphereAlgorithm = new SphereVsSphereAlgorithm(_collisionDetection);
		this.GJKAlgorithm = new GJKAlgorithm(_collisionDetection);
		this.concaveVsConvexAlgorithm = new ConcaveVsConvexAlgorithm(_collisionDetection);
	}
	
	@Override
	public NarrowPhaseAlgorithm selectAlgorithm(final CollisionShapeType _type1, final CollisionShapeType _type2) {
		// Sphere vs Sphere algorithm
		if (_type1 == CollisionShapeType.SPHERE && _type2 == CollisionShapeType.SPHERE) {
			return this.sphereVsSphereAlgorithm;
		} else if ((!CollisionShape.isConvex(_type1) && CollisionShape.isConvex(_type2)) || (!CollisionShape.isConvex(_type2) && CollisionShape.isConvex(_type1))) {
			// Concave vs Convex algorithm
			return this.concaveVsConvexAlgorithm;
		} else if (CollisionShape.isConvex(_type1) && CollisionShape.isConvex(_type2)) {
			// Convex vs Convex algorithm (GJK algorithm)
			return this.GJKAlgorithm;
		} else {
			return null;
		}
	}
	
}
