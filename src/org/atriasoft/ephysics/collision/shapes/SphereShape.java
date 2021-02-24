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
import org.atriasoft.ephysics.mathematics.Ray;
import org.atriasoft.etk.math.Constant;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Matrix3f;
import org.atriasoft.etk.math.Transform3D;
import org.atriasoft.etk.math.Vector3f;

/**
 * Represents a sphere collision shape that is centered
 * at the origin and defined by its radius. This collision shape does not
 * have an explicit object margin distance. The margin is implicitly the
 * radius of the sphere. Therefore, no need to specify an object margin
 * for a sphere shape.
 */
public class SphereShape extends ConvexShape {
	/**
		 *  Constructor
		 * @param radius Radius of the sphere (in meters)
		 */
	public SphereShape(final float _radius) {
		super(CollisionShapeType.SPHERE, _radius);
	}
	
	@Override
	public void computeAABB(final AABB _aabb, final Transform3D _transform) {
		
		// Get the local extents in x,y and z direction
		final Vector3f extents = new Vector3f(this.margin, this.margin, this.margin);
		// Update the AABB with the new minimum and maximum coordinates
		_aabb.setMin(_transform.getPosition().lessNew(extents));
		_aabb.setMax(_transform.getPosition().addNew(extents));
	}
	
	@Override
	public void computeLocalInertiaTensor(final Matrix3f _tensor, final float _mass) {
		final float diag = 0.4f * _mass * this.margin * this.margin;
		_tensor.set(diag, 0.0f, 0.0f, 0.0f, diag, 0.0f, 0.0f, 0.0f, diag);
	}
	
	@Override
	public void getLocalBounds(final Vector3f min, final Vector3f max) {
		// Maximum bounds
		max.setX(this.margin);
		max.setY(this.margin);
		max.setZ(this.margin);
		// Minimum bounds
		min.setX(-max.x);
		min.setY(-max.y);
		min.setZ(-max.z);
	}
	
	@Override
	public Vector3f getLocalSupportPointWithoutMargin(final Vector3f _direction, final CacheData _cachedCollisionData) {
		return new Vector3f(0.0f, 0.0f, 0.0f);
	}
	
	/**
	 *  Get the radius of the sphere
	 * @return Radius of the sphere (in meters)
	 */
	public float getRadius() {
		return this.margin;
	}
	
	@Override
	public boolean raycast(final Ray _ray, final RaycastInfo _raycastInfo, final ProxyShape _proxyShape) {
		final Vector3f m = _ray.point1;
		final float c = m.dot(m) - this.margin * this.margin;
		// If the origin of the ray is inside the sphere, we return no longersection
		if (c < 0.0f) {
			return false;
		}
		final Vector3f rayDirection = _ray.point2.lessNew(_ray.point1);
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
		if (t < _ray.maxFraction * raySquareLength) {
			// Compute the longersection information
			t /= raySquareLength;
			_raycastInfo.body = _proxyShape.getBody();
			_raycastInfo.proxyShape = _proxyShape;
			_raycastInfo.hitFraction = t;
			_raycastInfo.worldPoint = rayDirection.multiplyNew(t).add(_ray.point1);
			_raycastInfo.worldNormal = _raycastInfo.worldPoint;
			return true;
		}
		return false;
	}
	
	@Override
	public void setLocalScaling(final Vector3f _scaling) {
		this.margin = (this.margin / this.scaling.x) * _scaling.x;
		super.setLocalScaling(_scaling);
	}
	
	@Override
	public boolean testPointInside(final Vector3f _localPoint, final ProxyShape _proxyShape) {
		return (_localPoint.length2() < this.margin * this.margin);
	}
};