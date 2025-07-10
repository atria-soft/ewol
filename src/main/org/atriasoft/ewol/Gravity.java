/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol;

import org.atriasoft.etk.math.Vector2f;

/**
 * Gravity of the widget property
 */
public record Gravity(
		GravityHorizontal x,
		GravityVertical y,
		GravityDepth z) {
	
	public static final Gravity BOTTOM = new Gravity(GravityHorizontal.CENTER, GravityVertical.BOTTOM, GravityDepth.CENTER); // !< gravity is in center
	public static final Gravity BOTTOM_LEFT = new Gravity(GravityHorizontal.LEFT, GravityVertical.BOTTOM, GravityDepth.CENTER); // !< gravity is in top
	public static final Gravity BOTTOM_RIGHT = new Gravity(GravityHorizontal.RIGHT, GravityVertical.BOTTOM, GravityDepth.CENTER); // !< gravity is in bottom
	public static final Gravity CENTER = new Gravity(GravityHorizontal.CENTER, GravityVertical.CENTER, GravityDepth.CENTER); // !< gravity is in right
	public static final Gravity LEFT = new Gravity(GravityHorizontal.LEFT, GravityVertical.CENTER, GravityDepth.CENTER); // !< gravity is in left
	public static final Gravity RIGHT = new Gravity(GravityHorizontal.RIGHT, GravityVertical.CENTER, GravityDepth.CENTER); // !< gravity is in top-right
	public static final Gravity TOP = new Gravity(GravityHorizontal.CENTER, GravityVertical.TOP, GravityDepth.CENTER); // !< gravity is in top-left
	public static final Gravity TOP_LEFT = new Gravity(GravityHorizontal.LEFT, GravityVertical.TOP, GravityDepth.CENTER); // !< gravity is in bottom-right
	public static final Gravity TOP_RIGHT = new Gravity(GravityHorizontal.RIGHT, GravityVertical.TOP, GravityDepth.CENTER); // !< gravity is in bottom-left
	
	public Vector2f gravityGenerateDelta(final Vector2f deltas) {
		float outX = 0;
		float outY = 0;
		float outZ = 0;
		if (deltas.x() > 0.0001f) {
			if (this.x == GravityHorizontal.LEFT) {
				// nothing to do
			} else if (this.x == GravityHorizontal.RIGHT) {
				outX = (int) (deltas.x());
			} else {
				outX = (int) (deltas.x() * 0.5f);
			}
		}
		if (deltas.y() > 0.0001f) {
			if (this.y == GravityVertical.BOTTOM) {
				// nothing to do
			} else if (this.y == GravityVertical.TOP) {
				outY = (int) (deltas.y());
			} else {
				outY = (int) (deltas.y() * 0.5f);
			}
		}
		return new Vector2f(outX, outY);
	}
	
	public static Gravity valueOf(String value) {
		GravityHorizontal x = GravityHorizontal.CENTER;
		GravityVertical y = GravityVertical.CENTER;
		GravityDepth z = GravityDepth.CENTER;
		if (value.contains("LEFT")) {
			x = GravityHorizontal.LEFT;
		} else if (value.contains("RIGHT")) {
			x = GravityHorizontal.RIGHT;
		}
		if (value.contains("TOP")) {
			y = GravityVertical.TOP;
		} else if (value.contains("BOTTOM") || value.contains("BUTTOM")) {
			y = GravityVertical.BOTTOM;
		}
		if (value.contains("FRONT")) {
			z = GravityDepth.FRONT;
		} else if (value.contains("BACK")) {
			z = GravityDepth.BACK;
		}
		return new Gravity(x, y, z);
	}
	
	@Override
	public String toString() {
		if (this.x == GravityHorizontal.CENTER && this.y == GravityVertical.CENTER && this.z == GravityDepth.CENTER) {
			return GravityHorizontal.CENTER.toString();
		}
		StringBuilder data = new StringBuilder();
		if (this.x != GravityHorizontal.CENTER) {
			data.append(this.x.toString());
		}
		if (this.y != GravityVertical.CENTER) {
			if (data.length() != 0) {
				data.append("_");
			}
			data.append(this.y.toString());
		}
		if (this.z != GravityDepth.CENTER) {
			if (data.length() != 0) {
				data.append("_");
			}
			data.append(this.z.toString());
		}
		return data.toString();
	}
	
}
