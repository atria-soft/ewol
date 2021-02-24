package org.atriasoft.ephysics;

import org.atriasoft.etk.math.Vector3f;

import org.atriasoft.ephysics.body.CollisionBody;
import org.atriasoft.ephysics.collision.ProxyShape;

public class RaycastInfo {
	public Vector3f worldPoint = new Vector3f(); //!< Hit point in world-space coordinates
	public Vector3f worldNormal = new Vector3f(); //!< Surface normal at hit point in world-space coordinates
	public float hitFraction = 0.0f; //!< Fraction distance of the hit point between point1 and point2 of the ray. The hit point "p" is such that p = point1 + hitFraction * (point2 - point1)
	public long meshSubpart = -1; //!< Mesh subpart index that has been hit (only used for triangles mesh and -1 otherwise)
	public long triangleIndex = -1; //!< Hit triangle index (only used for triangles mesh and -1 otherwise)
	public CollisionBody body; //!< Pointer to the hit collision body;
	public ProxyShape proxyShape; //!< Pointer to the hit proxy collision shape
}
