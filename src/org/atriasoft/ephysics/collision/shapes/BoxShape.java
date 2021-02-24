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
import org.atriasoft.ephysics.configuration.Defaults;
import org.atriasoft.ephysics.mathematics.Ray;
import org.atriasoft.etk.math.Constant;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Matrix3f;
import org.atriasoft.etk.math.Vector3f;

/**
 * This class represents a 3D box shape. Those axis are unit length. The three extents are half-widths of the box along
 * the three axis x, y, z local axis. The "transform" of the corresponding rigid body will give an orientation and a
 * position to the box. This collision shape uses an extra margin distance around it for collision detection purpose.
 * The default margin is 4cm (if your units are meters, which is recommended). In case, you want to simulate small
 * objects (smaller than the margin distance), you might want to reduce the margin by specifying your own margin
 * distance using the "margin" parameter in the constructor of the box shape. Otherwise, it is recommended to use the
 * default margin distance by not using the "margin" parameter in the constructor.
 *
 * @author Jason Sorensen <sorensenj@smert.net>
 */
public class BoxShape extends ConvexShape {
	
	// Extent sizes of the box in the x, y and z direction
	private final Vector3f extent;
	
	// Copy-constructor
	public BoxShape(final BoxShape shape) {
		super(CollisionShapeType.BOX, shape.margin);
		this.extent = shape.extent.clone();
	}
	
	// Constructor
	public BoxShape(final Vector3f extent) {
		this(extent, Defaults.OBJECT_MARGIN);
	}
	
	public BoxShape(final Vector3f extent, final float margin) {
		super(CollisionShapeType.BOX, margin);
		assert (extent.getX() > 0.0f && extent.getX() > margin);
		assert (extent.getY() > 0.0f && extent.getY() > margin);
		assert (extent.getZ() > 0.0f && extent.getZ() > margin);
		this.extent = extent.lessNew(margin);
	}
	
	@Override
	public BoxShape clone() {
		return new BoxShape(this);
	}
	
	@Override
	public void computeLocalInertiaTensor(final Matrix3f tensor, final float _mass) {
		final float factor = (1.0f / 3.0f) * _mass;
		final Vector3f realExtent = this.extent.addNew(new Vector3f(this.margin, this.margin, this.margin));
		final float xSquare = realExtent.x * realExtent.x;
		final float ySquare = realExtent.y * realExtent.y;
		final float zSquare = realExtent.z * realExtent.z;
		tensor.set(factor * (ySquare + zSquare), 0.0f, 0.0f, 0.0f, factor * (xSquare + zSquare), 0.0f, 0.0f, 0.0f, factor * (xSquare + ySquare));
	}
	
	public Vector3f getExtent() {
		return (new Vector3f(this.margin, this.margin, this.margin)).add(this.extent);
	}
	
	@Override
	public void getLocalBounds(final Vector3f _min, final Vector3f _max) {
		// Maximum bounds
		_max.set(this.extent.x + this.margin, this.extent.y + this.margin, this.extent.z + this.margin);
		// Minimum bounds
		_min.set(-_max.x, -_max.y, -_max.z);
	}
	
	/*
	protected size_t getSizeInBytes() {
		return sizeof(BoxShape);
	}
	 */
	@Override
	public Vector3f getLocalSupportPointWithoutMargin(final Vector3f _direction, final CacheData _cachedCollisionData) {
		//Log.error("getLocalSupportPointWithoutMargin(" + _direction);
		//Log.error("    extends = " + this.extent);
		return new Vector3f(_direction.x < 0.0 ? -this.extent.x : this.extent.x, _direction.y < 0.0 ? -this.extent.y : this.extent.y, _direction.z < 0.0 ? -this.extent.z : this.extent.z);
	}
	
	@Override
	public boolean raycast(final Ray _ray, final RaycastInfo _raycastInfo, final ProxyShape _proxyShape) {
		final Vector3f rayDirection = _ray.point2.less(_ray.point1);
		float tMin = Float.MIN_VALUE;
		float tMax = Float.MAX_VALUE;
		Vector3f normalDirection = new Vector3f(0, 0, 0);
		Vector3f currentNormal = new Vector3f(0, 0, 0);
		// For each of the three slabs
		for (int iii = 0; iii < 3; ++iii) {
			// If ray is parallel to the slab
			if (FMath.abs(rayDirection.get(iii)) < Constant.FLOAT_EPSILON) {
				// If the ray's origin is not inside the slab, there is no hit
				if (_ray.point1.get(iii) > this.extent.get(iii) || _ray.point1.get(iii) < -this.extent.get(iii)) {
					return false;
				}
			} else {
				// Compute the intersection of the ray with the near and far plane of the slab
				final float oneOverD = 1.0f / rayDirection.get(iii);
				float t1 = (-this.extent.get(iii) - _ray.point1.get(iii)) * oneOverD;
				float t2 = (this.extent.get(iii) - _ray.point1.get(iii)) * oneOverD;
				currentNormal.x = (iii == 0) ? -this.extent.get(iii) : 0.0f;
				currentNormal.y = (iii == 1) ? -this.extent.get(iii) : 0.0f;
				currentNormal.z = (iii == 2) ? -this.extent.get(iii) : 0.0f;
				// Swap t1 and t2 if need so that t1 is intersection with near plane and
				// t2 with far plane
				if (t1 > t2) {
					final float ttt = t2;
					t2 = t1;
					t1 = ttt;
					currentNormal = currentNormal.multiply(-1);
				}
				// Compute the intersection of the of slab intersection interval with previous slabs
				if (t1 > tMin) {
					tMin = t1;
					normalDirection = currentNormal;
				}
				tMax = FMath.min(tMax, t2);
				// If tMin is larger than the maximum raycasting fraction, we return no hit
				if (tMin > _ray.maxFraction) {
					return false;
				}
				// If the slabs intersection is empty, there is no hit
				if (tMin > tMax) {
					return false;
				}
			}
		}
		// If tMin is negative, we return no hit
		if (tMin < 0.0f || tMin > _ray.maxFraction) {
			return false;
		}
		if (normalDirection.isZero()) {
			return false;
		}
		// The ray longersects the three slabs, we compute the hit point
		final Vector3f localHitPoint = rayDirection.multiplyNew(tMin).add(_ray.point1);
		_raycastInfo.body = _proxyShape.getBody();
		_raycastInfo.proxyShape = _proxyShape;
		_raycastInfo.hitFraction = tMin;
		_raycastInfo.worldPoint = localHitPoint;
		_raycastInfo.worldNormal = normalDirection;
		return true;
	}
	
	@Override
	public void setLocalScaling(final Vector3f _scaling) {
		this.extent.divide(this.scaling).multiply(_scaling);
		super.setLocalScaling(_scaling);
	}
	
	@Override
	public boolean testPointInside(final Vector3f _localPoint, final ProxyShape _proxyShape) {
		return (_localPoint.x < this.extent.x && _localPoint.x > -this.extent.x && _localPoint.y < this.extent.y && _localPoint.y > -this.extent.y && _localPoint.z < this.extent.z
				&& _localPoint.z > -this.extent.z);
	}
	
}
