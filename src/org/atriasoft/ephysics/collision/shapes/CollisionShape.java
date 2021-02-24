/** @file
 * Original ReactPhysics3D C++ library by Daniel Chappuis <http://www.reactphysics3d.com/> This code is re-licensed with permission from ReactPhysics3D author.
 * @author Daniel CHAPPUIS
 * @author Edouard DUPIN
 * @copyright 2010-2016, Daniel Chappuis
 * @copyright 2017-now, Edouard DUPIN
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ephysics.collision.shapes;

import org.atriasoft.etk.math.Matrix3f;
import org.atriasoft.etk.math.Transform3D;
import org.atriasoft.etk.math.Vector3f;

import org.atriasoft.ephysics.RaycastInfo;
import org.atriasoft.ephysics.collision.ProxyShape;
import org.atriasoft.ephysics.configuration.Defaults;
import org.atriasoft.ephysics.mathematics.Ray;

/**
 * This abstract class represents the collision shape associated with a
 * body that is used during the narrow-phase collision detection.
 */
public abstract class CollisionShape {
	/**
	 * @brief Get the maximum number of contact
	 * @return The maximum number of contact manifolds in an overlapping pair given two shape types
	 */
	public static int computeNbMaxContactManifolds(final CollisionShapeType _shapeType1, final CollisionShapeType _shapeType2) {
		// If both shapes are convex
		if (isConvex(_shapeType1) && isConvex(_shapeType2)) {
			return Defaults.NB_MAX_CONTACT_MANIFOLDS_CONVEX_SHAPE;
		}
		// If there is at least one concave shape
		return Defaults.NB_MAX_CONTACT_MANIFOLDS_CONCAVE_SHAPE;
	}
	
	/**
	 * @brief Check if the shape is convex
	 * @param[in] _shapeType shape type
	 * @return true If the collision shape is convex
	 * @return false If it is concave
	 */
	public static boolean isConvex(final CollisionShapeType _shapeType) {
		return _shapeType != CollisionShapeType.CONCAVE_MESH && _shapeType != CollisionShapeType.HEIGHTFIELD;
	}
	
	protected CollisionShapeType type; //!< Type of the collision shape
	protected Vector3f scaling; //!< Scaling vector of the collision shape
	/// Constructor
	
	public CollisionShape(final CollisionShapeType type) {
		this.type = type;
		this.scaling = new Vector3f(1.0f, 1.0f, 1.0f);
	}
	
	/**
	 * @brief Update the AABB of a body using its collision shape
	 * @param[out] _aabb The axis-aligned bounding box (AABB) of the collision shape computed in world-space coordinates
	 * @param[in] _transform Transform3D used to compute the AABB of the collision shape
	 */
	public void computeAABB(final AABB _aabb, final Transform3D _transform) {
		// Get the local bounds in x,y and z direction
		final Vector3f minBounds = new Vector3f(0, 0, 0);
		final Vector3f maxBounds = new Vector3f(0, 0, 0);
		getLocalBounds(minBounds, maxBounds);
		// Rotate the local bounds according to the orientation of the body
		final Matrix3f worldAxis = _transform.getOrientation().getMatrix().getAbsolute();
		final Vector3f worldMinBounds = new Vector3f(worldAxis.getColumn(0).dot(minBounds), worldAxis.getColumn(1).dot(minBounds), worldAxis.getColumn(2).dot(minBounds));
		final Vector3f worldMaxBounds = new Vector3f(worldAxis.getColumn(0).dot(maxBounds), worldAxis.getColumn(1).dot(maxBounds), worldAxis.getColumn(2).dot(maxBounds));
		// Compute the minimum and maximum coordinates of the rotated extents
		final Vector3f minCoordinates = _transform.getPosition().addNew(worldMinBounds);
		final Vector3f maxCoordinates = _transform.getPosition().addNew(worldMaxBounds);
		// Update the AABB with the new minimum and maximum coordinates
		_aabb.setMin(minCoordinates);
		_aabb.setMax(maxCoordinates);
	}
	
	/**
	 * @brief Compute the local inertia tensor of the sphere
	 * @param[out] _tensor The 3x3 inertia tensor matrix of the shape in local-space coordinates
	 * @param[in] _mass Mass to use to compute the inertia tensor of the collision shape
	 */
	public abstract void computeLocalInertiaTensor(Matrix3f _tensor, float _mass);
	
	/**
	 * @brief Get the local bounds of the shape in x, y and z directions.
	 * This method is used to compute the AABB of the box
	 * @param _min The minimum bounds of the shape in local-space coordinates
	 * @param _max The maximum bounds of the shape in local-space coordinates
	 */
	public abstract void getLocalBounds(Vector3f _min, Vector3f _max);
	
	/// Return the scaling vector of the collision shape
	public Vector3f getScaling() {
		return this.scaling;
	}
	
	/**
	 * @brief Get the type of the collision shapes
	 * @return The type of the collision shape (box, sphere, cylinder, ...)
	 */
	public CollisionShapeType getType() {
		return this.type;
	}
	
	/**
	 * @brief Check if the shape is convex
	 * @return true If the collision shape is convex
	 * @return false If it is concave
	 */
	public abstract boolean isConvex();
	
	/// Raycast method with feedback information
	public abstract boolean raycast(Ray ray, RaycastInfo raycastInfo, ProxyShape proxyShape);
	
	/**
	 * @brief Set the scaling vector of the collision shape
	 */
	public void setLocalScaling(final Vector3f _scaling) {
		this.scaling = _scaling;
	}
	
	/// Return true if a point is inside the collision shape
	public abstract boolean testPointInside(Vector3f worldPoint, ProxyShape proxyShape);
};
