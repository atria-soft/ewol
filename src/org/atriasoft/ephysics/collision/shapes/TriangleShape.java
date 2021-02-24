package org.atriasoft.ephysics.collision.shapes;

import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Matrix3f;
import org.atriasoft.etk.math.Transform3D;
import org.atriasoft.etk.math.Vector3f;

import org.atriasoft.ephysics.RaycastInfo;
import org.atriasoft.ephysics.collision.ProxyShape;
import org.atriasoft.ephysics.configuration.Defaults;
import org.atriasoft.ephysics.mathematics.Ray;

/**
 * This class represents a triangle collision shape that is centered
 * at the origin and defined three points.
 */
public class TriangleShape extends ConvexShape {
	/**
	 *  Raycast test side for the triangle
	 */
	public enum TriangleRaycastSide {
		FRONT, //!< Raycast against front triangle
		BACK, //!< Raycast against back triangle
		FRONT_AND_BACK //!< Raycast against front and back triangle
	};
	
	protected Vector3f[] points = new Vector3f[3]; //!< Three points of the triangle
	protected TriangleRaycastSide raycastTestType; //!< Raycast test type for the triangle (front, back, front-back)
	
	public TriangleShape(final Vector3f _point1, final Vector3f _point2, final Vector3f _point3) {
		super(CollisionShapeType.TRIANGLE, Defaults.OBJECT_MARGIN);
		this.points[0] = _point1;
		this.points[1] = _point2;
		this.points[2] = _point3;
		this.raycastTestType = TriangleRaycastSide.FRONT;
	}
	
	/**
		 *  Constructor
		 * @param _point1 First point of the triangle
		 * @param _point2 Second point of the triangle
		 * @param _point3 Third point of the triangle
		 * @param _margin The collision margin (in meters) around the collision shape
		 */
	public TriangleShape(final Vector3f _point1, final Vector3f _point2, final Vector3f _point3, final float _margin) {
		super(CollisionShapeType.TRIANGLE, _margin);
		this.points[0] = _point1;
		this.points[1] = _point2;
		this.points[2] = _point3;
		this.raycastTestType = TriangleRaycastSide.FRONT;
	}
	
	@Override
	public void computeAABB(final AABB aabb, final Transform3D _transform) {
		final Vector3f worldPoint1 = _transform.multiplyNew(this.points[0]);
		final Vector3f worldPoint2 = _transform.multiplyNew(this.points[1]);
		final Vector3f worldPoint3 = _transform.multiplyNew(this.points[2]);
		final Vector3f xAxis = new Vector3f(worldPoint1.x, worldPoint2.x, worldPoint3.x);
		final Vector3f yAxis = new Vector3f(worldPoint1.y, worldPoint2.y, worldPoint3.y);
		final Vector3f zAxis = new Vector3f(worldPoint1.z, worldPoint2.z, worldPoint3.z);
		aabb.setMin(new Vector3f(xAxis.getMin(), yAxis.getMin(), zAxis.getMin()));
		aabb.setMax(new Vector3f(xAxis.getMax(), yAxis.getMax(), zAxis.getMax()));
	}
	
	@Override
	public void computeLocalInertiaTensor(final Matrix3f _tensor, final float _mass) {
		_tensor.setZero();
	}
	
	@Override
	public void getLocalBounds(final Vector3f min, final Vector3f max) {
		final Vector3f xAxis = new Vector3f(this.points[0].x, this.points[1].x, this.points[2].x);
		final Vector3f yAxis = new Vector3f(this.points[0].y, this.points[1].y, this.points[2].y);
		final Vector3f zAxis = new Vector3f(this.points[0].z, this.points[1].z, this.points[2].z);
		min.setValue(xAxis.getMin() - this.margin, yAxis.getMin() - this.margin, zAxis.getMin() - this.margin);
		max.setValue(xAxis.getMax() + this.margin, yAxis.getMax() + this.margin, zAxis.getMax() + this.margin);
	}
	
	@Override
	public Vector3f getLocalSupportPointWithoutMargin(final Vector3f _direction, final CacheData _cachedCollisionData) {
		final Vector3f dotProducts = new Vector3f(_direction.dot(this.points[0]), _direction.dot(this.points[1]), _direction.dot(this.points[2]));
		return this.points[dotProducts.getMaxAxis()];
	}
	
	/// Return the raycast test type (front, back, front-back)
	public TriangleRaycastSide getRaycastTestType() {
		return this.raycastTestType;
	}
	
	/**
	 *  Return the coordinates of a given vertex of the triangle
	 * @param _index Index (0 to 2) of a vertex of the triangle
	 */
	public Vector3f getVertex(final int _index) {
		assert (_index >= 0 && _index < 3);
		return this.points[_index];
	}
	
	@Override
	public boolean raycast(final Ray _ray, final RaycastInfo _raycastInfo, final ProxyShape _proxyShape) {
		final Vector3f pq = _ray.point2.lessNew(_ray.point1);
		final Vector3f pa = this.points[0].lessNew(_ray.point1);
		final Vector3f pb = this.points[1].lessNew(_ray.point1);
		final Vector3f pc = this.points[2].lessNew(_ray.point1);
		// Test if the line PQ is inside the eges BC, CA and AB. We use the triple
		// product for this test.
		final Vector3f m = pq.cross(pc);
		float u = pb.dot(m);
		if (this.raycastTestType == TriangleRaycastSide.FRONT) {
			if (u < 0.0f) {
				return false;
			}
		} else if (this.raycastTestType == TriangleRaycastSide.BACK) {
			if (u > 0.0f) {
				return false;
			}
		}
		float v = -pa.dot(m);
		if (this.raycastTestType == TriangleRaycastSide.FRONT) {
			if (v < 0.0f) {
				return false;
			}
		} else if (this.raycastTestType == TriangleRaycastSide.BACK) {
			if (v > 0.0f) {
				return false;
			}
		} else if (this.raycastTestType == TriangleRaycastSide.FRONT_AND_BACK) {
			if (!FMath.sameSign(u, v)) {
				return false;
			}
		}
		float w = pa.dot(pq.cross(pb));
		if (this.raycastTestType == TriangleRaycastSide.FRONT) {
			if (w < 0.0f) {
				return false;
			}
		} else if (this.raycastTestType == TriangleRaycastSide.BACK) {
			if (w > 0.0f) {
				return false;
			}
		} else if (this.raycastTestType == TriangleRaycastSide.FRONT_AND_BACK) {
			if (!FMath.sameSign(u, w)) {
				return false;
			}
		}
		// If the line PQ is in the triangle plane (case where u=v=w=0)
		if (FMath.approxEqual(u, 0.0f) && FMath.approxEqual(v, 0) && FMath.approxEqual(w, 0)) {
			return false;
		}
		// Compute the barycentric coordinates (u, v, w) to determine the
		// longersection point R, R = u * a + v * b + w * c
		final float denom = 1.0f / (u + v + w);
		u *= denom;
		v *= denom;
		w *= denom;
		// Compute the local hit point using the barycentric coordinates
		final Vector3f localHitPoint = this.points[0].multiplyNew(u).add(this.points[1].multiplyNew(v)).add(this.points[2].multiplyNew(w));
		final float hitFraction = localHitPoint.lessNew(_ray.point1).length() / pq.length();
		if (hitFraction < 0.0f || hitFraction > _ray.maxFraction) {
			return false;
		}
		final Vector3f localHitNormal = this.points[1].lessNew(this.points[0]).cross(this.points[2].lessNew(this.points[0]));
		if (localHitNormal.dot(pq) > 0.0f) {
			localHitNormal.less(localHitNormal);
		}
		_raycastInfo.body = _proxyShape.getBody();
		_raycastInfo.proxyShape = _proxyShape;
		_raycastInfo.worldPoint = localHitPoint;
		_raycastInfo.hitFraction = hitFraction;
		_raycastInfo.worldNormal = localHitNormal;
		return true;
	}
	
	@Override
	public void setLocalScaling(final Vector3f _scaling) {
		this.points[0].divide(this.scaling).multiply(_scaling);
		this.points[1].divide(this.scaling).multiply(_scaling);
		this.points[2].divide(this.scaling).multiply(_scaling);
		super.setLocalScaling(_scaling);
	}
	
	/**
	 *  Set the raycast test type (front, back, front-back)
	 * @param _testType Raycast test type for the triangle (front, back, front-back)
	 */
	public void setRaycastTestType(final TriangleRaycastSide _testType) {
		this.raycastTestType = _testType;
	}
	
	@Override
	public boolean testPointInside(final Vector3f _localPoint, final ProxyShape _proxyShape) {
		return false;
	}
	
};