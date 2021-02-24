/** @file
 * Original ReactPhysics3D C++ library by Daniel Chappuis <http://www.reactphysics3d.com/> This code is re-licensed with permission from ReactPhysics3D author.
 * @author Daniel CHAPPUIS
 * @author Edouard DUPIN
 * @copyright 2010-2016, Daniel Chappuis
 * @copyright 2017-now, Edouard DUPIN
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ephysics.collision.shapes;

import org.atriasoft.ephysics.RaycastInfo;
import org.atriasoft.ephysics.collision.ProxyShape;
import org.atriasoft.ephysics.mathematics.Ray;
import org.atriasoft.etk.math.Constant;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Matrix3f;
import org.atriasoft.etk.math.Vector3f;

/**
 * This class represents a capsule collision shape that is defined around the Y axis. A capsule shape can be seen as the
 * convex hull of two spheres. The capsule shape is defined by its radius (radius of the two spheres of the capsule) and
 * its height (distance between the centers of the two spheres). This collision shape does not have an explicit object
 * margin distance. The margin is implicitly the radius and height of the shape. Therefore, no need to specify an object
 * margin for a capsule shape.
 *
 * @author Jason Sorensen <sorensenj@smert.net>
 */
public class CapsuleShape extends ConvexShape {
	// Half height of the capsule (height = distance between the centers of the two spheres)
	private float halfHeight;
	
	// Copy-constructor
	public CapsuleShape(final CapsuleShape shape) {
		super(CollisionShapeType.CAPSULE, shape.margin);
		this.halfHeight = shape.halfHeight;
	}
	
	// Constructor
	public CapsuleShape(final float radius, final float height) {
		// TODO: Should radius really be the margin for a capsule? Seems like a bug.
		super(CollisionShapeType.CAPSULE, radius);
		this.halfHeight = height * 0.5f;
	}
	
	@Override
	public CapsuleShape clone() {
		return new CapsuleShape(this);
	}
	
	@Override
	public void computeLocalInertiaTensor(final Matrix3f _tensor, final float _mass) {
		// The inertia tensor formula for a capsule can be found in : Game Engine Gems, Volume 1
		final float height = this.halfHeight + this.halfHeight;
		final float radiusSquare = this.margin * this.margin;
		final float heightSquare = height * height;
		final float radiusSquareDouble = radiusSquare + radiusSquare;
		final float factor1 = 2.0f * this.margin / (4.0f * this.margin + 3.0f * height);
		final float factor2 = 3.0f * height / (4.0f * this.margin + 3.0f * height);
		final float sum1 = 0.4f * radiusSquareDouble;
		final float sum2 = 0.75f * height * this.margin + 0.5f * heightSquare;
		final float sum3 = 0.25f * radiusSquare + 1.0f / 12.0f * heightSquare;
		final float IxxAndzz = factor1 * _mass * (sum1 + sum2) + factor2 * _mass * sum3;
		final float Iyy = factor1 * _mass * sum1 + factor2 * _mass * 0.25f * radiusSquareDouble;
		_tensor.set(IxxAndzz, 0.0f, 0.0f, 0.0f, Iyy, 0.0f, 0.0f, 0.0f, IxxAndzz);
	}
	
	// Return the height of the capsule
	public float getHeight() {
		return this.halfHeight + this.halfHeight;
	}
	
	@Override
	public void getLocalBounds(final Vector3f min, final Vector3f max) {
		// Maximum bounds
		max.setX(this.margin);
		max.setY(this.halfHeight + this.margin);
		max.setZ(this.margin);
		// Minimum bounds
		min.setX(-max.x);
		min.setY(-max.y);
		min.setZ(-max.z);
	}
	
	@Override
	public Vector3f getLocalSupportPointWithoutMargin(final Vector3f _direction, final CacheData _cachedCollisionData) {
		// Support point top sphere
		final float dotProductTop = this.halfHeight * _direction.y;
		// Support point bottom sphere
		final float dotProductBottom = -this.halfHeight * _direction.y;
		// Return the point with the maximum dot product
		if (dotProductTop > dotProductBottom) {
			return new Vector3f(0, this.halfHeight, 0);
		}
		return new Vector3f(0, -this.halfHeight, 0);
	}
	
	// Get the radius of the capsule
	public float getRadius() {
		return this.margin;
	}
	
	@Override
	public boolean raycast(final Ray _ray, final RaycastInfo _raycastInfo, final ProxyShape _proxyShape) {
		final Vector3f n = _ray.point2.lessNew(_ray.point1);
		final float epsilon = 0.01f;
		final Vector3f p = new Vector3f(0.0f, -this.halfHeight, 0.0f);
		final Vector3f q = new Vector3f(0.0f, this.halfHeight, 0.0f);
		final Vector3f d = q.lessNew(p);
		final Vector3f m = _ray.point1.lessNew(p);
		float t;
		final float mDotD = m.dot(d);
		final float nDotD = n.dot(d);
		final float dDotD = d.dot(d);
		// Test if the segment is outside the cylinder
		final float vec1DotD = _ray.point1.lessNew(new Vector3f(0.0f, -this.halfHeight - this.margin, 0.0f)).dot(d);
		if (vec1DotD < 0.0f && vec1DotD + nDotD < 0.0f) {
			return false;
		}
		final float ddotDExtraCaps = 2.0f * this.margin * d.y;
		if (vec1DotD > dDotD + ddotDExtraCaps && vec1DotD + nDotD > dDotD + ddotDExtraCaps) {
			return false;
		}
		final float nDotN = n.dot(n);
		final float mDotN = m.dot(n);
		final float a = dDotD * nDotN - nDotD * nDotD;
		final float k = m.dot(m) - this.margin * this.margin;
		final float c = dDotD * k - mDotD * mDotD;
		// If the ray is parallel to the capsule axis
		if (FMath.abs(a) < epsilon) {
			// If the origin is outside the surface of the capusle's cylinder, we return no hit
			if (c > 0.0f) {
				return false;
			}
			// Here we know that the segment longersect an endcap of the capsule
			// If the ray longersects with the "p" endcap of the capsule
			if (mDotD < 0.0f) {
				// Check longersection between the ray and the "p" sphere endcap of the capsule
				final Vector3f hitLocalPoint = new Vector3f();
				final Float hitFraction = 0.0f;
				if (raycastWithSphereEndCap(_ray.point1, _ray.point2, p, _ray.maxFraction, hitLocalPoint, hitFraction)) {
					_raycastInfo.body = _proxyShape.getBody();
					_raycastInfo.proxyShape = _proxyShape;
					_raycastInfo.hitFraction = hitFraction;
					_raycastInfo.worldPoint = hitLocalPoint;
					final Vector3f normalDirection = hitLocalPoint.lessNew(p);
					_raycastInfo.worldNormal = normalDirection;
					return true;
				}
				return false;
			} else if (mDotD > dDotD) { // If the ray longersects with the "q" endcap of the cylinder
				// Check longersection between the ray and the "q" sphere endcap of the capsule
				final Vector3f hitLocalPoint = new Vector3f();
				final Float hitFraction = 0.0f;
				if (raycastWithSphereEndCap(_ray.point1, _ray.point2, q, _ray.maxFraction, hitLocalPoint, hitFraction)) {
					_raycastInfo.body = _proxyShape.getBody();
					_raycastInfo.proxyShape = _proxyShape;
					_raycastInfo.hitFraction = hitFraction;
					_raycastInfo.worldPoint = hitLocalPoint;
					final Vector3f normalDirection = hitLocalPoint.lessNew(q);
					_raycastInfo.worldNormal = normalDirection;
					return true;
				}
				return false;
			} else {
				// If the origin is inside the cylinder, we return no hit
				return false;
			}
		}
		final float b = dDotD * mDotN - nDotD * mDotD;
		final float discriminant = b * b - a * c;
		// If the discriminant is negative, no real roots and therfore, no hit
		if (discriminant < 0.0f) {
			return false;
		}
		// Compute the smallest root (first longersection along the ray)
		final float t0 = t = (-b - FMath.sqrt(discriminant)) / a;
		// If the longersection is outside the finite cylinder of the capsule on "p" endcap side
		final float value = mDotD + t * nDotD;
		if (value < 0.0f) {
			// Check longersection between the ray and the "p" sphere endcap of the capsule
			final Vector3f hitLocalPoint = new Vector3f();
			final Float hitFraction = 0.0f;
			if (raycastWithSphereEndCap(_ray.point1, _ray.point2, p, _ray.maxFraction, hitLocalPoint, hitFraction)) {
				_raycastInfo.body = _proxyShape.getBody();
				_raycastInfo.proxyShape = _proxyShape;
				_raycastInfo.hitFraction = hitFraction;
				_raycastInfo.worldPoint = hitLocalPoint;
				final Vector3f normalDirection = hitLocalPoint.lessNew(p);
				_raycastInfo.worldNormal = normalDirection;
				return true;
			}
			return false;
		} else if (value > dDotD) { // If the longersection is outside the finite cylinder on the "q" side
			// Check longersection between the ray and the "q" sphere endcap of the capsule
			final Vector3f hitLocalPoint = new Vector3f();
			final Float hitFraction = 0.0f;
			if (raycastWithSphereEndCap(_ray.point1, _ray.point2, q, _ray.maxFraction, hitLocalPoint, hitFraction)) {
				_raycastInfo.body = _proxyShape.getBody();
				_raycastInfo.proxyShape = _proxyShape;
				_raycastInfo.hitFraction = hitFraction;
				_raycastInfo.worldPoint = hitLocalPoint;
				final Vector3f normalDirection = hitLocalPoint.lessNew(q);
				_raycastInfo.worldNormal = normalDirection;
				return true;
			}
			return false;
		}
		t = t0;
		// If the longersection is behind the origin of the ray or beyond the maximum
		// raycasting distance, we return no hit
		if (t < 0.0f || t > _ray.maxFraction) {
			return false;
		}
		// Compute the hit information
		final Vector3f localHitPoint = n.multiplyNew(t).add(_ray.point1);
		_raycastInfo.body = _proxyShape.getBody();
		_raycastInfo.proxyShape = _proxyShape;
		_raycastInfo.hitFraction = t;
		_raycastInfo.worldPoint = localHitPoint;
		final Vector3f v = localHitPoint.lessNew(p);
		final Vector3f w = d.multiplyNew(v.dot(d) / d.length2());
		final Vector3f normalDirection = localHitPoint.lessNew(p.addNew(w)).safeNormalize();
		_raycastInfo.worldNormal = normalDirection;
		return true;
	}
	
	/**
	 * @brief Raycasting method between a ray one of the two spheres end cap of the capsule
	 */
	protected boolean raycastWithSphereEndCap(final Vector3f _point1, final Vector3f _point2, final Vector3f _sphereCenter, final float _maxFraction, Vector3f _hitLocalPoint, Float _hitFraction) {
		final Vector3f m = _point1.lessNew(_sphereCenter);
		final float c = m.dot(m) - this.margin * this.margin;
		// If the origin of the ray is inside the sphere, we return no longersection
		if (c < 0.0f) {
			return false;
		}
		final Vector3f rayDirection = _point2.lessNew(_point1);
		final float b = m.dot(rayDirection);
		// If the origin of the ray is outside the sphere and the ray
		// is pointing away from the sphere, there is no longersection
		if (b > 0.0f) {
			return false;
		}
		final float raySquareLength = rayDirection.length2();
		// Compute the discriminant of the quadratic equation
		final float discriminant = b * b - raySquareLength * c;
		// If the discriminant is negative or the ray length is very small, there is no longersection
		if (discriminant < 0.0f || raySquareLength < Constant.FLOAT_EPSILON) {
			return false;
		}
		// Compute the solution "t" closest to the origin
		float t = -b - FMath.sqrt(discriminant);
		assert (t >= 0.0f);
		// If the hit point is withing the segment ray fraction
		if (t < _maxFraction * raySquareLength) {
			// Compute the longersection information
			t /= raySquareLength;
			_hitFraction = t;
			_hitLocalPoint = rayDirection.multiplyNew(t).add(_point1);
			return true;
		}
		return false;
	}
	
	@Override
	public void setLocalScaling(final Vector3f _scaling) {
		this.halfHeight = (this.halfHeight / this.scaling.y) * _scaling.y;
		this.margin = (this.margin / this.scaling.x) * _scaling.x;
		super.setLocalScaling(_scaling);
	}
	
	@Override
	public boolean testPointInside(final Vector3f _localPoint, final ProxyShape _proxyShape) {
		final float diffYCenterSphere1 = _localPoint.y - this.halfHeight;
		final float diffYCenterSphere2 = _localPoint.y + this.halfHeight;
		final float xSquare = _localPoint.x * _localPoint.x;
		final float zSquare = _localPoint.z * _localPoint.z;
		final float squareRadius = this.margin * this.margin;
		// Return true if the point is inside the cylinder or one of the two spheres of the capsule
		return ((xSquare + zSquare) < squareRadius && _localPoint.y < this.halfHeight && _localPoint.y > -this.halfHeight)
				|| (xSquare + zSquare + diffYCenterSphere1 * diffYCenterSphere1) < squareRadius || (xSquare + zSquare + diffYCenterSphere2 * diffYCenterSphere2) < squareRadius;
	}
	
}
