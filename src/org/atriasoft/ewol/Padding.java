/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol;

/**
 * Simple class to abstract the padding property.
 */
public record Padding(
		float left,
		float top,
		float right,
		float bottom, // !< this represent the 4 padding value Left top right buttom (like css)
		float front,
		float back) {
	
	public static final Padding ZERO = new Padding(0, 0, 0, 0, 0, 0);
	
	public Padding() {
		this(0, 0, 0, 0, 0, 0);
	}
	
	public Padding(final float left) {
		this(left, 0, 0, 0, 0, 0);
	}
	
	public Padding(final float left, final float top) {
		this(left, top, 0, 0, 0, 0);
	}
	
	public Padding(final float left, final float top, final float right) {
		this(left, top, right, 0, 0, 0);
	}
	
	public Padding(final float left, final float top, final float right, final float bottom) {
		this(left, top, right, bottom, 0, 0);
	}
	
	public Padding(final double left, final double top, final double right, final double bottom) {
		this((float) left, (float) top, (float) right, (float) bottom, 0, 0);
	}
	
	public Padding(final float left, final float top, final float right, final float bottom, final float front) {
		this(left, top, right, bottom, front, 0);
	}
	
	public Padding(final float left, final float top, final float right, final float bottom, final float front, final float back) {
		this.left = left;
		this.top = top;
		this.right = right;
		this.bottom = bottom;
		this.front = front;
		this.back = back;
	}
	
	/**
	 * Add a vector to this one
	 * @param v The vector to add to this one
	 */
	public Padding add(final Padding v) {
		return new Padding(this.left + v.left, this.top + v.top, this.right + v.right, this.bottom + v.bottom, this.front + v.front, this.back + v.back);
	}
	
	public Padding withLeft(final float left) {
		return new Padding(left, this.top, this.right, this.bottom, this.front, this.back);
	}
	
	public Padding withRight(final float right) {
		return new Padding(this.left, this.top, right, this.bottom, this.front, this.back);
	}
	
	public Padding withBottom(final float bottom) {
		return new Padding(this.left, this.top, this.right, bottom, this.front, this.back);
	}
	
	public Padding withTop(final float top) {
		return new Padding(this.left, top, this.right, this.bottom, this.front, this.back);
	}
	
	public Padding withFront(final float front) {
		return new Padding(this.left, this.top, this.right, this.bottom, front, this.back);
	}
	
	public Padding withBack(final float back) {
		return new Padding(this.left, this.top, this.right, this.bottom, this.front, back);
	}
	
	@Override
	public String toString() {
		return "{" + left() + "," + top() + "," + right() + "," + bottom() + "," + front() + "," + back() + "}";
	}
	
	public float x() {
		return this.left + this.right;
	}
	
	public float y() {
		return this.top + this.bottom;
	}
	
	public float z() {
		return this.front + this.back;
	}
	
}
