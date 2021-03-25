/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol;

/**
 * @breif Simple class to abstarct the padding porperty.
 */
public class Padding {
	private float xLeft;
	private float xRight;
	private float yBottom; // !< this represent the 4 padding value Left top right buttom (like css)
	private float yTop;
	
	public Padding() {
		setValue();
	}
	
	public Padding(final float xLeft) {
		setValue(xLeft);
	}
	
	public Padding(final float xLeft, final float yt) {
		setValue(xLeft, yt);
	}
	
	public Padding(final float xLeft, final float yt, final float xr) {
		setValue(xLeft, yt, xr);
	}
	
	public Padding(final float xLeft, final float yt, final float xr, final float yb) {
		setValue(xLeft, yt, xr, yb);
	}
	
	/**
	 * Add a vector to this one
	 * @param v The vector to add to this one
	 */
	public Padding add(final Padding v) {
		this.xLeft += v.xLeft;
		this.yTop += v.yTop;
		this.xRight += v.xRight;
		this.yBottom += v.yBottom;
		return this;
	}
	
	// ! @previous
	public Padding addNew(final Padding v) {
		return new Padding(this.xLeft + v.xLeft, this.yTop + v.yTop, this.xRight + v.xRight, this.yBottom + v.yBottom);
	}
	
	public void setValue() {
		this.xLeft = 0;
		this.yTop = 0;
		this.xRight = 0;
		this.yBottom = 0;
	}
	
	public void setValue(final float xLeft) {
		this.xLeft = xLeft;
		this.yTop = 0;
		this.xRight = 0;
		this.yBottom = 0;
	}
	
	public void setValue(final float xLeft, final float yt) {
		this.xLeft = xLeft;
		this.yTop = yt;
		this.xRight = 0;
		this.yBottom = 0;
	}
	
	public void setValue(final float xLeft, final float yt, final float xr) {
		this.xLeft = xLeft;
		this.yTop = yt;
		this.xRight = xr;
		this.yBottom = 0;
	}
	
	public void setValue(final float xLeft, final float yt, final float xr, final float yb) {
		this.xLeft = xLeft;
		this.yTop = yt;
		this.xRight = xr;
		this.yBottom = yb;
	}
	
	public void setXLeft(final float val) {
		this.xLeft = val;
	}
	
	public void setXRight(final float val) {
		this.xRight = val;
	}
	
	public void setYButtom(final float val) {
		this.yBottom = val;
	}
	
	public void setYTop(final float val) {
		this.yTop = val;
	}
	
	@Override
	public String toString() {
		return "{" + xLeft() + "," + yTop() + "," + xRight() + "," + yButtom() + "}";
	}
	
	public float x() {
		return this.xLeft + this.xRight;
	}
	
	public float xLeft() {
		return this.xLeft;
	}
	
	public float xRight() {
		return this.xRight;
	}
	
	public float y() {
		return this.yTop + this.yBottom;
	}
	
	public float yButtom() {
		return this.yBottom;
	}
	
	public float yTop() {
		return this.yTop;
	}
}
