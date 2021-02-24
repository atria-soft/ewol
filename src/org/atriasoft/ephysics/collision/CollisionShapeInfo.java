package org.atriasoft.ephysics.collision;

import org.atriasoft.etk.math.Transform3D;

import org.atriasoft.ephysics.collision.shapes.CollisionShape;
import org.atriasoft.ephysics.engine.OverlappingPair;

/**
 *  It regroups different things about a collision shape. This is
 * used to pass information about a collision shape to a collision algorithm.
 */
public class CollisionShapeInfo {
	
	public final OverlappingPair overlappingPair; //!< Broadphase overlapping pair
	public final ProxyShape proxyShape; //!< Proxy shape
	public final CollisionShape collisionShape; //!< Pointer to the collision shape
	public final Transform3D shapeToWorldTransform; //!< Transform3D that maps from collision shape local-space to world-space
	public final Object cachedCollisionData; //!< Cached collision data of the proxy shape
	/// Constructor
	
	public CollisionShapeInfo(final ProxyShape _proxyCollisionShape, final CollisionShape _shape, final Transform3D _shapeLocalToWorldTransform, final OverlappingPair _pair,
			final Object _cachedData) {
		this.overlappingPair = _pair;
		this.proxyShape = _proxyCollisionShape;
		this.collisionShape = _shape;
		this.shapeToWorldTransform = _shapeLocalToWorldTransform;
		this.cachedCollisionData = _cachedData;
		
	}
}
