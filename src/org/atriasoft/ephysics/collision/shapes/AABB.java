/** @file
 * Original ReactPhysics3D C++ library by Daniel Chappuis <http://www.reactphysics3d.com/> This code is re-licensed with permission from ReactPhysics3D author.
 * @author Daniel CHAPPUIS
 * @author Edouard DUPIN
 * @copyright 2010-2016, Daniel Chappuis
 * @copyright 2017-now, Edouard DUPIN
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ephysics.collision.shapes;

import org.atriasoft.ephysics.mathematics.Ray;
import org.atriasoft.etk.math.Constant;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector3f;

/**
 * This class represents a bounding volume of type "Axis Aligned Bounding Box". It's a box where all the edges are
 * always aligned with the world coordinate system. The AABB is defined by the  this.inimum and  this.aximum world coordinates of
 * the three axis.
 *
 * @author Jason Sorensen <sorensenj@smert.net>
 */
public class AABB {
	
	/**
	 * @brief Create and return an AABB for a triangle
	 * @param[in] _trianglePoints List of 3 point od a triangle
	 * @return An AABB box
	 */
	public static AABB createAABBForTriangle(final Vector3f[] _trianglePoints) {
		final Vector3f minCoords = new Vector3f(_trianglePoints[0].x, _trianglePoints[0].y, _trianglePoints[0].z);
		final Vector3f maxCoords = new Vector3f(_trianglePoints[0].x, _trianglePoints[0].y, _trianglePoints[0].z);
		if (_trianglePoints[1].x < minCoords.x) {
			minCoords.setX(_trianglePoints[1].x);
		}
		if (_trianglePoints[1].y < minCoords.y) {
			minCoords.setY(_trianglePoints[1].y);
		}
		if (_trianglePoints[1].z < minCoords.z) {
			minCoords.setZ(_trianglePoints[1].z);
		}
		if (_trianglePoints[2].x < minCoords.x) {
			minCoords.setX(_trianglePoints[2].x);
		}
		if (_trianglePoints[2].y < minCoords.y) {
			minCoords.setY(_trianglePoints[2].y);
		}
		if (_trianglePoints[2].z < minCoords.z) {
			minCoords.setZ(_trianglePoints[2].z);
		}
		if (_trianglePoints[1].x > maxCoords.x) {
			maxCoords.setX(_trianglePoints[1].x);
		}
		if (_trianglePoints[1].y > maxCoords.y) {
			maxCoords.setY(_trianglePoints[1].y);
		}
		if (_trianglePoints[1].z > maxCoords.z) {
			maxCoords.setZ(_trianglePoints[1].z);
		}
		if (_trianglePoints[2].x > maxCoords.x) {
			maxCoords.setX(_trianglePoints[2].x);
		}
		if (_trianglePoints[2].y > maxCoords.y) {
			maxCoords.setY(_trianglePoints[2].y);
		}
		if (_trianglePoints[2].z > maxCoords.z) {
			maxCoords.setZ(_trianglePoints[2].z);
		}
		return new AABB(minCoords, maxCoords);
	}
	
	// Maximum world coordinates of the AABB on the x,y and z axis
	private final Vector3f maxCoordinates;
	
	// Minimum world coordinates of the AABB on the x,y and z axis
	private final Vector3f minCoordinates;
	
	/**
	 * @brief default contructor
	 */
	public AABB() {
		this.maxCoordinates = new Vector3f();
		this.minCoordinates = new Vector3f();
	}
	
	/**
	 * @brief contructor Whit sizes
	 * @param[in] _minCoordinates Minimum coordinates
	 * @param[in] _maxCoordinates Maximum coordinates
	 */
	public AABB(final Vector3f minCoordinates, final Vector3f maxCoordinates) {
		this.maxCoordinates = maxCoordinates;
		this.minCoordinates = minCoordinates;
	}
	
	/**
	 * @brief Return true if the current AABB contains the AABB given in parameter
	 * @param[in] _aabb AABB box that is contains in the current.
	 * @return true The parameter in contained inside
	 */
	public boolean contains(final AABB _aabb) {
		boolean isInside = true;
		isInside = isInside && this.minCoordinates.x <= _aabb.minCoordinates.x;
		isInside = isInside && this.minCoordinates.y <= _aabb.minCoordinates.y;
		isInside = isInside && this.minCoordinates.z <= _aabb.minCoordinates.z;
		isInside = isInside && this.maxCoordinates.x >= _aabb.maxCoordinates.x;
		isInside = isInside && this.maxCoordinates.y >= _aabb.maxCoordinates.y;
		isInside = isInside && this.maxCoordinates.z >= _aabb.maxCoordinates.z;
		return isInside;
	}
	
	/**
	 * @brief Return true if a point is inside the AABB
	 * @param[in] _point Point to check.
	 * @return true The point in contained inside
	 */
	public boolean contains(final Vector3f _point) {
		return _point.x >= this.minCoordinates.x - Constant.FLOAT_EPSILON && _point.x <= this.maxCoordinates.x + Constant.FLOAT_EPSILON && _point.y >= this.minCoordinates.y - Constant.FLOAT_EPSILON
				&& _point.y <= this.maxCoordinates.y + Constant.FLOAT_EPSILON && _point.z >= this.minCoordinates.z - Constant.FLOAT_EPSILON
				&& _point.z <= this.maxCoordinates.z + Constant.FLOAT_EPSILON;
	}
	
	/**
	 * @brief Get the center point of the AABB box
	 * @return The 3D position of the center
	 */
	public Vector3f getCenter() {
		return new Vector3f(this.minCoordinates).add(this.maxCoordinates).multiply(0.5f);
	}
	
	/**
	 * @brief Get the size of the AABB in the three dimension x, y and z
	 * @return the AABB 3D size
	 */
	public Vector3f getExtent() {
		return this.maxCoordinates.lessNew(this.minCoordinates);
	}
	
	/**
	 * @brief Return the maximum coordinates of the AABB
	 * @return The 3d maximum coordonates
	 */
	public Vector3f getMax() {
		return this.maxCoordinates;
	}
	
	/**
	 * @brief Get the minimum coordinates of the AABB
	 * @return The 3d minimum coordonates
	 */
	public Vector3f getMin() {
		return this.minCoordinates;
	}
	
	/**
	 * @brief Get the volume of the AABB
	 * @return The 3D volume.
	 */
	public float getVolume() {
		final Vector3f diff = this.maxCoordinates.lessNew(this.minCoordinates);
		return (diff.x * diff.y * diff.z);
	}
	
	/**
	 * @brief Inflate each side of the AABB by a given size
	 * @param[in] _dx Inflate X size
	 * @param[in] _dy Inflate Y size
	 * @param[in] _dz Inflate Z size
	 */
	public void inflate(final float _dx, final float _dy, final float _dz) {
		this.maxCoordinates.add(new Vector3f(_dx, _dy, _dz));
		this.minCoordinates.less(new Vector3f(_dx, _dy, _dz));
	}
	
	/**
	 * @brief Replace the current AABB with a new AABB that is the union of two AABBs in parameters
	 * @param[in] _aabb1 first AABB box to merge with _aabb2.
	 * @param[in] _aabb2 second AABB box to merge with _aabb1.
	 */
	public void mergeTwoAABBs(final AABB _aabb1, final AABB _aabb2) {
		this.minCoordinates.setX(FMath.min(_aabb1.minCoordinates.x, _aabb2.minCoordinates.x));
		this.minCoordinates.setY(FMath.min(_aabb1.minCoordinates.y, _aabb2.minCoordinates.y));
		this.minCoordinates.setZ(FMath.min(_aabb1.minCoordinates.z, _aabb2.minCoordinates.z));
		this.maxCoordinates.setX(FMath.max(_aabb1.maxCoordinates.x, _aabb2.maxCoordinates.x));
		this.maxCoordinates.setY(FMath.max(_aabb1.maxCoordinates.y, _aabb2.maxCoordinates.y));
		this.maxCoordinates.setZ(FMath.max(_aabb1.maxCoordinates.z, _aabb2.maxCoordinates.z));
	}
	
	/**
	 * @brief Merge the AABB in parameter with the current one
	 * @param[in] _aabb Other AABB box to merge.
	 */
	public void mergeWithAABB(final AABB _aabb) {
		this.minCoordinates.setX(FMath.min(this.minCoordinates.x, _aabb.minCoordinates.x));
		this.minCoordinates.setY(FMath.min(this.minCoordinates.y, _aabb.minCoordinates.y));
		this.minCoordinates.setZ(FMath.min(this.minCoordinates.z, _aabb.minCoordinates.z));
		this.maxCoordinates.setX(FMath.max(this.maxCoordinates.x, _aabb.maxCoordinates.x));
		this.maxCoordinates.setY(FMath.max(this.maxCoordinates.y, _aabb.maxCoordinates.y));
		this.maxCoordinates.setZ(FMath.max(this.maxCoordinates.z, _aabb.maxCoordinates.z));
	}
	
	/**
	 * @brief Set the maximum coordinates of the AABB
	 * @param[in] _max The 3d maximum coordonates
	 */
	public void setMax(final Vector3f max) {
		this.maxCoordinates.set(max);
	}
	
	/**
	 * @brief Set the minimum coordinates of the AABB
	 * @param[in] _min The 3d minimum coordonates
	 */
	public void setMin(final Vector3f min) {
		this.minCoordinates.set(min);
	}
	
	/**
	 * @brief Return true if the current AABB is overlapping with the AABB in argument
	 * Two AABBs overlap if they overlap in the three x, y and z axis at the same time
	 * @param[in] _aabb Other AABB box to check.
	 * @return true Collision detected
	 * @return false Not collide
	 */
	public boolean testCollision(final AABB aabb) {
		if (this == aabb) {
			///Log.info("test : AABB    ==> same object ");
			return true;
		}
		//Log.info("test : " + this + " && " + aabb);
		if (this.maxCoordinates.getX() < aabb.minCoordinates.getX() || aabb.maxCoordinates.getX() < this.minCoordinates.getX()) {
			return false;
		}
		if (this.maxCoordinates.getZ() < aabb.minCoordinates.getZ() || aabb.maxCoordinates.getZ() < this.minCoordinates.getZ()) {
			return false;
		}
		if (this.maxCoordinates.getY() < aabb.minCoordinates.getY() || aabb.maxCoordinates.getY() < this.minCoordinates.getY()) {
			return false;
		}
		//Log.info("detect collision ");
		return true;
	}
	
	/**
	 * @brief check if the AABB of a triangle intersects the AABB
	 * @param[in] _trianglePoints List of 3 point od a triangle
	 * @return true The triangle is contained in the Box
	 */
	public boolean testCollisionTriangleAABB(final Vector3f[] _trianglePoints) {
		if (FMath.min(_trianglePoints[0].x, _trianglePoints[1].x, _trianglePoints[2].x) > this.maxCoordinates.x) {
			return false;
		}
		if (FMath.min(_trianglePoints[0].y, _trianglePoints[1].y, _trianglePoints[2].y) > this.maxCoordinates.y) {
			return false;
		}
		if (FMath.min(_trianglePoints[0].z, _trianglePoints[1].z, _trianglePoints[2].z) > this.maxCoordinates.z) {
			return false;
		}
		if (FMath.max(_trianglePoints[0].x, _trianglePoints[1].x, _trianglePoints[2].x) < this.minCoordinates.x) {
			return false;
		}
		if (FMath.max(_trianglePoints[0].y, _trianglePoints[1].y, _trianglePoints[2].y) < this.minCoordinates.y) {
			return false;
		}
		if (FMath.max(_trianglePoints[0].z, _trianglePoints[1].z, _trianglePoints[2].z) < this.minCoordinates.z) {
			return false;
		}
		return true;
	}
	
	/*
	 * @brief check if the ray intersects the AABB
	 * This method use the line vs AABB raycasting technique described in
	 * Real-time Collision Detection by Christer Ericson.
	 * @param[in] _ray Ray to test
	 * @return true The raytest intersect the AABB box
	 */
	public boolean testRayIntersect(final Ray ray) {
		final Vector3f point2 = ray.point2.lessNew(ray.point1).multiply(ray.maxFraction).add(ray.point1);
		final Vector3f e = this.maxCoordinates.lessNew(this.minCoordinates);
		final Vector3f d = point2.lessNew(ray.point1);
		final Vector3f m = ray.point1.addNew(point2).less(this.minCoordinates).less(this.maxCoordinates);
		// Test if the AABB face normals are separating axis
		float adx = FMath.abs(d.x);
		if (FMath.abs(m.x) > e.x + adx) {
			return false;
		}
		float ady = FMath.abs(d.y);
		if (FMath.abs(m.y) > e.y + ady) {
			return false;
		}
		float adz = FMath.abs(d.z);
		if (FMath.abs(m.z) > e.z + adz) {
			return false;
		}
		// Add in an epsilon term to counteract arithmetic errors when segment is
		// (near) parallel to a coordinate axis (see text for detail)
		final float epsilon = 0.00001f;
		adx += epsilon;
		ady += epsilon;
		adz += epsilon;
		// Test if the cross products between face normals and ray direction are
		// separating axis
		if (FMath.abs(m.y * d.z - m.z * d.y) > e.y * adz + e.z * ady) {
			return false;
		}
		if (FMath.abs(m.z * d.x - m.x * d.z) > e.x * adz + e.z * adx) {
			return false;
		}
		if (FMath.abs(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx) {
			return false;
		}
		// No separating axis has been found
		return true;
	}
	
	@Override
	public String toString() {
		return "AABB [min=" + this.minCoordinates + ", max=" + this.maxCoordinates + "]";
	}
	
}
