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
 *  This class represents a cone collision shape centered at the
 * origin and alligned with the Y axis. The cone is defined
 * by its height and by the radius of its base. The center of the
 * cone is at the half of the height. The "transform" of the
 * corresponding rigid body gives an orientation and a position
 * to the cone. This collision shape uses an extra margin distance around
 * it for collision detection purpose. The default margin is 4cm (if your
 * units are meters, which is recommended). In case, you want to simulate small
 * objects (smaller than the margin distance), you might want to reduce the margin
 * by specifying your own margin distance using the "margin" parameter in the
 * ructor of the cone shape. Otherwise, it is recommended to use the
 * default margin distance by not using the "margin" parameter in the ructor.
 */
public class ConeShape extends ConvexShape {
	protected float radius; //!< Radius of the base
	protected float halfHeight; //!< Half height of the cone
	
	protected float sinTheta; //!< sine of the semi angle at the apex point
	
	/**
	 *  Constructor
	 * @param _radius Radius of the cone (in meters)
	 * @param _height Height of the cone (in meters)
	 * @param _margin Collision margin (in meters) around the collision shape
	 */
	public ConeShape(final float _radius, final float _height) {
		this(_radius, _height, Defaults.OBJECT_MARGIN);
	}
	
	public ConeShape(final float _radius, final float _height, final float _margin) {
		super(CollisionShapeType.CONE, _margin);
		this.radius = _radius;
		this.halfHeight = _height * 0.5f;
		// Compute the sine of the semi-angle at the apex point
		this.sinTheta = this.radius / (FMath.sqrt(this.radius * this.radius + _height * _height));
	}
	
	@Override
	public void computeLocalInertiaTensor(final Matrix3f _tensor, final float _mass) {
		final float rSquare = this.radius * this.radius;
		final float diagXZ = 0.15f * _mass * (rSquare + this.halfHeight);
		_tensor.set(diagXZ, 0.0f, 0.0f, 0.0f, 0.3f * _mass * rSquare, 0.0f, 0.0f, 0.0f, diagXZ);
	}
	
	/**
	 *  Return the height
	 * @return Height of the cone (in meters)
	 */
	public float getHeight() {
		return 2.0f * this.halfHeight;
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
		final Vector3f v = _direction;
		final float sinThetaTimesLengthV = this.sinTheta * v.length();
		Vector3f supportPoint;
		if (v.y > sinThetaTimesLengthV) {
			supportPoint = new Vector3f(0.0f, this.halfHeight, 0.0f);
		} else {
			final float projectedLength = FMath.sqrt(v.x * v.x + v.z * v.z);
			if (projectedLength > Constant.FLOAT_EPSILON) {
				final float d = this.radius / projectedLength;
				supportPoint = new Vector3f(v.x * d, -this.halfHeight, v.z * d);
			} else {
				supportPoint = new Vector3f(0.0f, -this.halfHeight, 0.0f);
			}
		}
		return supportPoint;
	}
	
	/**
	 *  Return the radius
	 * @return Radius of the cone (in meters)
	 */
	public float getRadius() {
		return this.radius;
	}
	
	@Override
	public boolean raycast(final Ray _ray, final RaycastInfo _raycastInfo, final ProxyShape _proxyShape) {
		final Vector3f r = _ray.point2.lessNew(_ray.point1);
		final float epsilon = 0.00001f;
		final Vector3f V = new Vector3f(0, this.halfHeight, 0);
		final Vector3f centerBase = new Vector3f(0, -this.halfHeight, 0);
		final Vector3f axis = new Vector3f(0, -1.0f, 0);
		final float heightSquare = 4.0f * this.halfHeight * this.halfHeight;
		final float cosThetaSquare = heightSquare / (heightSquare + this.radius * this.radius);
		final float factor = 1.0f - cosThetaSquare;
		final Vector3f delta = _ray.point1.lessNew(V);
		final float c0 = -cosThetaSquare * delta.x * delta.x + factor * delta.y * delta.y - cosThetaSquare * delta.z * delta.z;
		final float c1 = -cosThetaSquare * delta.x * r.x + factor * delta.y * r.y - cosThetaSquare * delta.z * r.z;
		final float c2 = -cosThetaSquare * r.x * r.x + factor * r.y * r.y - cosThetaSquare * r.z * r.z;
		final float tHit[] = { -1.0f, -1.0f, -1.0f };
		final Vector3f[] localHitPoint = new Vector3f[3];
		final Vector3f[] localNormal = new Vector3f[3];
		// If c2 is different from zero
		if (FMath.abs(c2) > Constant.FLOAT_EPSILON) {
			final float gamma = c1 * c1 - c0 * c2;
			// If there is no real roots in the quadratic equation
			if (gamma < 0.0f) {
				return false;
			} else if (gamma > 0.0f) { // The equation has two real roots
				// Compute two longersections
				final float sqrRoot = FMath.sqrt(gamma);
				tHit[0] = (-c1 - sqrRoot) / c2;
				tHit[1] = (-c1 + sqrRoot) / c2;
			} else { // If the equation has a single real root
				// Compute the longersection
				tHit[0] = -c1 / c2;
			}
		} else // If c2 == 0
		if (FMath.abs(c1) > Constant.FLOAT_EPSILON) {
			// If c2 = 0 and c1 != 0
			tHit[0] = -c0 / (2.0f * c1);
		} else {
			// If c2 = c1 = 0
			// If c0 is different from zero, no solution and if c0 = 0, we have a
			// degenerate case, the whole ray is contained in the cone side
			// but we return no hit in this case
			return false;
		}
		// If the origin of the ray is inside the cone, we return no hit
		if (testPointInside(_ray.point1, null)) {
			return false;
		}
		localHitPoint[0] = r.multiplyNew(tHit[0]).add(_ray.point1);
		localHitPoint[1] = r.multiplyNew(tHit[1]).add(_ray.point1);
		// Only keep hit points in one side of the double cone (the cone we are longerested in)
		if (axis.dot(localHitPoint[0].lessNew(V)) < 0.0f) {
			tHit[0] = -1.0f;
		}
		if (axis.dot(localHitPoint[1].lessNew(V)) < 0.0f) {
			tHit[1] = -1.0f;
		}
		// Only keep hit points that are within the correct height of the cone
		if (localHitPoint[0].y < -this.halfHeight) {
			tHit[0] = -1.0f;
		}
		if (localHitPoint[1].y < -this.halfHeight) {
			tHit[1] = -1.0f;
		}
		// If the ray is in direction of the base plane of the cone
		if (r.y > epsilon) {
			// Compute the longersection with the base plane of the cone
			tHit[2] = (-_ray.point1.y - this.halfHeight) / (r.y);
			// Only keep this longersection if it is inside the cone radius
			localHitPoint[2] = r.multiplyNew(tHit[2]).add(_ray.point1);
			if ((localHitPoint[2].lessNew(centerBase)).length2() > this.radius * this.radius) {
				tHit[2] = -1.0f;
			}
			// Compute the normal direction
			localNormal[2] = axis;
		}
		// Find the smallest positive t value
		int hitIndex = -1;
		float t = Float.MAX_VALUE;
		for (int i = 0; i < 3; i++) {
			if (tHit[i] < 0.0f) {
				continue;
			}
			if (tHit[i] < t) {
				hitIndex = i;
				t = tHit[hitIndex];
			}
		}
		if (hitIndex < 0) {
			return false;
		}
		if (t > _ray.maxFraction) {
			return false;
		}
		// Compute the normal direction for hit against side of the cone
		if (hitIndex != 2) {
			final float h = 2.0f * this.halfHeight;
			final float value1 = (localHitPoint[hitIndex].x * localHitPoint[hitIndex].x + localHitPoint[hitIndex].z * localHitPoint[hitIndex].z);
			final float rOverH = this.radius / h;
			final float value2 = 1.0f + rOverH * rOverH;
			final float factor22 = 1.0f / FMath.sqrt(value1 * value2);
			final float x = localHitPoint[hitIndex].x * factor22;
			final float z = localHitPoint[hitIndex].z * factor22;
			localNormal[hitIndex].setX(x);
			localNormal[hitIndex].setY(FMath.sqrt(x * x + z * z) * rOverH);
			localNormal[hitIndex].setZ(z);
		}
		_raycastInfo.body = _proxyShape.getBody();
		_raycastInfo.proxyShape = _proxyShape;
		_raycastInfo.hitFraction = t;
		_raycastInfo.worldPoint = localHitPoint[hitIndex];
		_raycastInfo.worldNormal = localNormal[hitIndex];
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
		final float radiusHeight = this.radius * (-_localPoint.y + this.halfHeight) / (this.halfHeight * 2.0f);
		return (_localPoint.y < this.halfHeight && _localPoint.y > -this.halfHeight) && (_localPoint.x * _localPoint.x + _localPoint.z * _localPoint.z < radiusHeight * radiusHeight);
	}
	
}