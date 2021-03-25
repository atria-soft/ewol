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
public enum Gravity {
	buttom, // !< gravity is in center
	buttomLeft, // !< gravity is in top
	buttomRight, // !< gravity is in buttom
	center, // !< gravity is in right
	left, // !< gravity is in left
	right, // !< gravity is in top-right
	top, // !< gravity is in top-left
	topLeft, // !< gravity is in buttom-right
	topRight; // !< gravity is in buttom-left
	
	public static Vector2f gravityGenerateDelta(final Gravity gravity, final Vector2f deltas) {
		float outX = 0;
		float outY = 0;
		if (deltas.x() > 0.0001f) {
			if (gravity == left || gravity == buttomLeft || gravity == topLeft) {
				// nothing to do
			} else if (gravity == right || gravity == buttomRight || gravity == topRight) {
				outX = (int) (deltas.x());
			} else {
				outX = (int) (deltas.x() * 0.5f);
			}
		}
		if (deltas.y() > 0.0001f) {
			if (gravity == buttom || gravity == buttomLeft || gravity == buttomRight) {
				// nothing to do
			} else if (gravity == top || gravity == topRight || gravity == topLeft) {
				outY = (int) (deltas.y());
			} else {
				outY = (int) (deltas.y() * 0.5f);
			}
		}
		return new Vector2f(outX, outY);
	}
	
}
