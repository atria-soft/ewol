package org.atriasoft.ephysics.collision.shapes;

import org.atriasoft.ephysics.collision.ProxyShape;
import org.atriasoft.etk.math.Constant;
import org.atriasoft.etk.math.Vector3f;

public abstract class ConvexShape extends CollisionShape {
	protected float margin; //!< Margin used for the GJK collision detection algorithm
	/// Constructor
	
	public ConvexShape(final CollisionShapeType _type, final float _margin) {
		super(_type);
		this.margin = _margin;
	}
	
	// Return a local support point in a given direction with the object margin
	public Vector3f getLocalSupportPointWithMargin(final Vector3f _direction, final CacheData _cachedCollisionData) {
		////Log.error(" ->  getLocalSupportPointWithMargin(" + _direction);
		// Get the support point without margin
		final Vector3f supportPoint = getLocalSupportPointWithoutMargin(_direction, _cachedCollisionData);
		////Log.error(" ->  supportPoint = " + supportPoint);
		////Log.error(" ->  margin = " + FMath.floatToString(this.margin));
		if (this.margin != 0.0f) {
			// Add the margin to the support point
			Vector3f unitVec = new Vector3f(0.0f, -1.0f, 0.0f);
			////Log.error(" ->      _direction.length2()=" + FMath.floatToString(_direction.length2()));
			////Log.error(" ->      Constant.FLOAT_EPSILON=" + FMath.floatToString(Constant.FLOAT_EPSILON));
			if (_direction.length2() > Constant.FLOAT_EPSILON * Constant.FLOAT_EPSILON) {
				unitVec = _direction.safeNormalizeNew();
				////Log.error(" ->          unitVec= " + unitVec);
			}
			supportPoint.add(unitVec.multiplyNew(this.margin));
		}
		////Log.error(" ->      ==> supportPoint = " + supportPoint);
		return supportPoint;
	}
	
	/// Return a local support point in a given direction without the object margin
	public abstract Vector3f getLocalSupportPointWithoutMargin(Vector3f _direction, CacheData _cachedCollisionData);
	
	/**
	 * @brief Get the current object margin
	 * @return The margin (in meters) around the collision shape
	 */
	public float getMargin() {
		return this.margin;
	}
	
	@Override
	public boolean isConvex() {
		return true;
	}
	
	@Override
	public abstract boolean testPointInside(Vector3f _worldPoint, ProxyShape _proxyShape);
}
