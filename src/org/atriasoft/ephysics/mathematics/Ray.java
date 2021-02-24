package org.atriasoft.ephysics.mathematics;

import org.atriasoft.etk.math.Vector3f;

public class Ray {
	public Vector3f point1; //!<First point of the ray (origin)
	public Vector3f point2; //!< Second point of the ray
	public float maxFraction; //!< Maximum fraction value
	
	public Ray(final Ray obj) {
		this.point1 = new Vector3f(obj.point1);
		this.point2 = new Vector3f(obj.point2);
		this.maxFraction = obj.maxFraction;
	}
	
	/// Constructor with arguments
	public Ray(final Vector3f _p1, final Vector3f _p2) {
		this.point1 = _p1;
		this.point2 = _p2;
		this.maxFraction = 1.0f;
	}
	
	public Ray(final Vector3f _p1, final Vector3f _p2, final float _maxFrac) {
		this.point1 = _p1;
		this.point2 = _p2;
		this.maxFraction = _maxFrac;
	}
	
	@Override
	protected Object clone() throws CloneNotSupportedException {
		return new Ray(this);
	}
	
	@Override
	public boolean equals(final Object obj) {
		if (obj == null) {
			return false;
		}
		if (getClass() != obj.getClass()) {
			return false;
		}
		final Ray other = (Ray) obj;
		
		return this.point1.equals(other.point1) && this.point2.equals(other.point2) && this.maxFraction == other.maxFraction;
	}
	
	@Override
	public String toString() {
		return super.toString();
	}
	
}
