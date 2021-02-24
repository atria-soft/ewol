/** @file
 * Original ReactPhysics3D C++ library by Daniel Chappuis <http://www.reactphysics3d.com/> This code is re-licensed with permission from ReactPhysics3D author.
 * @author Daniel CHAPPUIS
 * @author Edouard DUPIN
 * @copyright 2010-2016, Daniel Chappuis
 * @copyright 2017-now, Edouard DUPIN
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ephysics.collision.shapes;

import org.atriasoft.etk.math.Vector3f;

import org.atriasoft.ephysics.collision.ProxyShape;
import org.atriasoft.ephysics.collision.shapes.TriangleShape.TriangleRaycastSide;

/**
 *  This abstract class represents a concave collision shape associated with a
 * body that is used during the narrow-phase collision detection.
 */
public abstract class ConcaveShape extends CollisionShape {
	
	/**
	 *  It is used to encapsulate a callback method for
	 * a single triangle of a ConcaveMesh.
	 */
	public interface TriangleCallback {
		/// Report a triangle
		public void testTriangle(Vector3f[] _trianglePoints);
	}
	
	boolean isSmoothMeshCollisionEnabled; //!< True if the smooth mesh collision algorithm is enabled
	
	protected float triangleMargin; //!< Margin use for collision detection for each triangle
	
	protected TriangleRaycastSide raycastTestType; //!< Raycast test type for the triangle (front, back, front-back)
	
	/// Return true if the collision shape is convex, false if it is concave
	public boolean isConvex;
	
	public ConcaveShape(final CollisionShapeType _type) {
		super(_type);
		this.isSmoothMeshCollisionEnabled = false;
		this.triangleMargin = 0;
		this.raycastTestType = TriangleRaycastSide.FRONT;
	}
	
	/// Return true if the smooth mesh collision is enabled
	public boolean getIsSmoothMeshCollisionEnabled() {
		return this.isSmoothMeshCollisionEnabled;
	}
	
	/// Return the raycast test type (front, back, front-back)
	public TriangleRaycastSide getRaycastTestType() {
		return this.raycastTestType;
	}
	
	/// Return the triangle margin
	public float getTriangleMargin() {
		return this.triangleMargin;
	}
	
	/**
	 *  Enable/disable the smooth mesh collision algorithm
	 *
	 * Smooth mesh collision is used to avoid collisions against some longernal edges of the triangle mesh.
	 * If it is enabled, collsions with the mesh will be smoother but collisions computation is a bit more expensive.
	 */
	public void setIsSmoothMeshCollisionEnabled(final boolean _isEnabled) {
		this.isSmoothMeshCollisionEnabled = _isEnabled;
	}
	
	/**
	 *  Set the raycast test type (front, back, front-back)
	 * @param testType Raycast test type for the triangle (front, back, front-back)
	 */
	public void setRaycastTestType(final TriangleRaycastSide _testType) {
		this.raycastTestType = _testType;
	}
	
	/// Use a callback method on all triangles of the concave shape inside a given AABB
	public abstract void testAllTriangles(TriangleCallback _callback, AABB _localAABB);
	
	@Override
	public boolean testPointInside(final Vector3f _localPoint, final ProxyShape _proxyShape) {
		return false;
	}
	
};
