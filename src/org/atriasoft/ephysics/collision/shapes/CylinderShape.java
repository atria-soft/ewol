/*
 * ReactPhysics3D physics library, http://code.google.com/p/reactphysics3d/
 * Copyright (c) 2010-2013 Daniel Chappuis
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim
 *    that you wrote the original software. If you use this software in a
 *    product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * This file has been modified during the port to Java and differ from the source versions.
 */
package org.atriasoft.ephysics.collision.shapes;

import org.atriasoft.ephysics.RaycastInfo;
import org.atriasoft.ephysics.collision.ProxyShape;
import org.atriasoft.ephysics.configuration.Defaults;
import org.atriasoft.ephysics.mathematics.Ray;
import org.atriasoft.etk.math.Constant;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Matrix3f;
import org.atriasoft.etk.math.Vector3f;

/**
 * This class represents a cylinder collision shape around the Y axis and centered at the origin. The cylinder is
 * defined by its height and the radius of its base. The "transform" of the corresponding rigid body gives an
 * orientation and a position to the cylinder. This collision shape uses an extra margin distance around it for
 * collision detection purpose. The default margin is 4cm (if your units are meters, which is recommended). In case, you
 * want to simulate small objects (smaller than the margin distance), you might want to reduce the margin by specifying
 * your own margin distance using the "margin" parameter in the constructor of the cylinder shape. Otherwise, it is
 * recommended to use the default margin distance by not using the "margin" parameter in the constructor.
 *
 * @author Jason Sorensen <sorensenj@smert.net>
 */
/**
 *  It represents a cylinder collision shape around the Y axis
 * and centered at the origin. The cylinder is defined by its height
 * and the radius of its base. The "transform" of the corresponding
 * rigid body gives an orientation and a position to the cylinder.
 * This collision shape uses an extra margin distance around it for collision
 * detection purpose. The default margin is 4cm (if your units are meters,
 * which is recommended). In case, you want to simulate small objects
 * (smaller than the margin distance), you might want to reduce the margin by
 * specifying your own margin distance using the "margin" parameter in the
 * ructor of the cylinder shape. Otherwise, it is recommended to use the
 * default margin distance by not using the "margin" parameter in the ructor.
 */
public class CylinderShape extends ConvexShape {
	protected float radius; //!< Radius of the base
	protected float halfHeight; //!< Half height of the cylinder
	
	/**
	 * Contructor
	 * @param radius Radius of the cylinder (in meters)
	 * @param height Height of the cylinder (in meters)
	 * @param margin Collision margin (in meters) around the collision shape
	 */
	public CylinderShape(final float _radius, final float _height) {
		this(_radius, _height, Defaults.OBJECT_MARGIN);
	}
	
	public CylinderShape(final float _radius, final float _height, final float _margin) {
		super(CollisionShapeType.CYLINDER, _margin);
		this.radius = _radius;
		this.halfHeight = _height / 2.0f;
	}
	
	@Override
	public void computeLocalInertiaTensor(final Matrix3f _tensor, final float _mass) {
		final float height = 2.0f * this.halfHeight;
		final float diag = (1.0f / 12.0f) * _mass * (3.0f * this.radius * this.radius + height * height);
		_tensor.set(diag, 0.0f, 0.0f, 0.0f, 0.5f * _mass * this.radius * this.radius, 0.0f, 0.0f, 0.0f, diag);
	}
	
	/**
	 * Get the Shape height
	 * @return Height of the cylinder (in meters)
	 */
	public float getHeight() {
		return this.halfHeight + this.halfHeight;
	}
	
	@Override
	public void getLocalBounds(final Vector3f min, final Vector3f max) {
		// Maximum bounds
		max.setX(this.radius + this.margin);
		max.setY(this.halfHeight + this.margin);
		max.setZ(this.radius + this.margin);
		// Minimum bounds
		min.setX(-max.x);
		min.setY(-max.y);
		min.setZ(-max.z);
	}
	
	@Override
	public Vector3f getLocalSupportPointWithoutMargin(final Vector3f _direction, final CacheData _cachedCollisionData) {
		final Vector3f supportPoint = new Vector3f(0.0f, 0.0f, 0.0f);
		final float uDotv = _direction.y;
		final Vector3f w = new Vector3f(_direction.x, 0.0f, _direction.z);
		final float lengthW = FMath.sqrt(_direction.x * _direction.x + _direction.z * _direction.z);
		if (lengthW > Constant.FLOAT_EPSILON) {
			if (uDotv < 0.0f) {
				supportPoint.setY(-this.halfHeight);
			} else {
				supportPoint.setY(this.halfHeight);
			}
			supportPoint.add(w.multiply(this.radius / lengthW));
		} else if (uDotv < 0.0f) {
			supportPoint.setY(-this.halfHeight);
		} else {
			supportPoint.setY(this.halfHeight);
		}
		return supportPoint;
	}
	
	/**
	 * Get the Shape radius
	 * @return Radius of the cylinder (in meters)
	 */
	public float getRadius() {
		return this.radius;
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
		if (mDotD < 0.0f && mDotD + nDotD < 0.0f) {
			return false;
		}
		
		if (mDotD > dDotD && mDotD + nDotD > dDotD) {
			return false;
		}
		final float nDotN = n.dot(n);
		final float mDotN = m.dot(n);
		final float a = dDotD * nDotN - nDotD * nDotD;
		final float k = m.dot(m) - this.radius * this.radius;
		final float c = dDotD * k - mDotD * mDotD;
		// If the ray is parallel to the cylinder axis
		if (FMath.abs(a) < epsilon) {
			// If the origin is outside the surface of the cylinder, we return no hit
			if (c > 0.0f) {
				return false;
			}
			// Here we know that the segment longersect an endcap of the cylinder
			// If the ray longersects with the "p" endcap of the cylinder
			if (mDotD < 0.0f) {
				t = -mDotN / nDotN;
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
				final Vector3f normalDirection = new Vector3f(0.0f, -1.0f, 0.0f);
				_raycastInfo.worldNormal = normalDirection;
				return true;
			}
			// If the ray longersects with the "q" endcap of the cylinder
			if (mDotD > dDotD) {
				t = (nDotD - mDotN) / nDotN;
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
				_raycastInfo.worldNormal = new Vector3f(0, 1.0f, 0);
				return true;
			}
			// If the origin is inside the cylinder, we return no hit
			return false;
		}
		final float b = dDotD * mDotN - nDotD * mDotD;
		final float discriminant = b * b - a * c;
		// If the discriminant is negative, no real roots and therfore, no hit
		if (discriminant < 0.0f) {
			return false;
		}
		// Compute the smallest root (first longersection along the ray)
		final float t0 = t = (-b - FMath.sqrt(discriminant)) / a;
		// If the longersection is outside the cylinder on "p" endcap side
		final float value = mDotD + t * nDotD;
		if (value < 0.0f) {
			// If the ray is pointing away from the "p" endcap, we return no hit
			if (nDotD <= 0.0f) {
				return false;
			}
			// Compute the longersection against the "p" endcap (longersection agains whole plane)
			t = -mDotD / nDotD;
			// Keep the longersection if the it is inside the cylinder radius
			if (k + t * (2.0f * mDotN + t) > 0.0f) {
				return false;
			}
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
			_raycastInfo.worldNormal = new Vector3f(0, -1.0f, 0);
			return true;
		}
		// If the longersection is outside the cylinder on the "q" side
		if (value > dDotD) {
			// If the ray is pointing away from the "q" endcap, we return no hit
			if (nDotD >= 0.0f) {
				return false;
			}
			// Compute the longersection against the "q" endcap (longersection against whole plane)
			t = (dDotD - mDotD) / nDotD;
			// Keep the longersection if it is inside the cylinder radius
			if (k + dDotD - 2.0f * mDotD + t * (2.0f * (mDotN - nDotD) + t) > 0.0f) {
				return false;
			}
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
			_raycastInfo.worldNormal = new Vector3f(0, 1.0f, 0);
			return true;
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
		final Vector3f normalDirection = localHitPoint.lessNew(p.addNew(w));
		_raycastInfo.worldNormal = normalDirection;
		return true;
	}
	
	@Override
	public void setLocalScaling(final Vector3f _scaling) {
		this.halfHeight = (this.halfHeight / this.scaling.y) * _scaling.y;
		this.radius = (this.radius / this.scaling.x) * _scaling.x;
		super.setLocalScaling(_scaling);
	}
	
	@Override
	public boolean testPointInside(final Vector3f _localPoint, final ProxyShape _proxyShape) {
		return ((_localPoint.x * _localPoint.x + _localPoint.z * _localPoint.z) < this.radius * this.radius && _localPoint.y < this.halfHeight && _localPoint.y > -this.halfHeight);
	}
}