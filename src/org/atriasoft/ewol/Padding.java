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
	private float yTop;
	private float xRight;
	private float yBottom; // !< this represent the 4 padding value Left top right buttom (like css)

	public Padding() {
		setValue();
	}

	public Padding(final float _xLeft) {
		setValue(_xLeft);
	}

	public Padding(final float _xLeft, final float _yt) {
		setValue(_xLeft, _yt);
	}

	public Padding(final float _xLeft, final float _yt, final float _xr) {
		setValue(_xLeft, _yt, _xr);
	}

	public Padding(final float _xLeft, final float _yt, final float _xr, final float _yb) {
		setValue(_xLeft, _yt, _xr, _yb);
	}

	/**
	 * @brief Add a vector to this one
	 * @param _v The vector to add to this one
	 */
	public Padding add(final Padding _v) {
		this.xLeft += _v.xLeft;
		this.yTop += _v.yTop;
		this.xRight += _v.xRight;
		this.yBottom += _v.yBottom;
		return this;
	}

	// ! @previous
	public Padding addNew(final Padding _v) {
		return new Padding(this.xLeft + _v.xLeft, this.yTop + _v.yTop, this.xRight + _v.xRight,
				this.yBottom + _v.yBottom);
	}

	public void setValue() {
		this.xLeft = 0;
		this.yTop = 0;
		this.xRight = 0;
		this.yBottom = 0;
	}

	public void setValue(final float _xLeft) {
		this.xLeft = _xLeft;
		this.yTop = 0;
		this.xRight = 0;
		this.yBottom = 0;
	}

	public void setValue(final float _xLeft, final float _yt) {
		this.xLeft = _xLeft;
		this.yTop = _yt;
		this.xRight = 0;
		this.yBottom = 0;
	}

	public void setValue(final float _xLeft, final float _yt, final float _xr) {
		this.xLeft = _xLeft;
		this.yTop = _yt;
		this.xRight = _xr;
		this.yBottom = 0;
	}

	public void setValue(final float _xLeft, final float _yt, final float _xr, final float _yb) {
		this.xLeft = _xLeft;
		this.yTop = _yt;
		this.xRight = _xr;
		this.yBottom = _yb;
	}

	public void setXLeft(final float _val) {
		this.xLeft = _val;
	}

	public void setXRight(final float _val) {
		this.xRight = _val;
	}

	public void setYButtom(final float _val) {
		this.yBottom = _val;
	}

	public void setYTop(final float _val) {
		this.yTop = _val;
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
