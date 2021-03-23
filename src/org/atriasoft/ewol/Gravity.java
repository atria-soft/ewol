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
	center, // !< gravity is in center
	top, // !< gravity is in top
	buttom, // !< gravity is in buttom
	right, // !< gravity is in right
	left, // !< gravity is in left
	topRight, // !< gravity is in top-right
	topLeft, // !< gravity is in top-left
	buttomRight, // !< gravity is in buttom-right
	buttomLeft; // !< gravity is in buttom-left

	public static Vector2f gravityGenerateDelta(final Gravity _gravity, final Vector2f _deltas) {
		float outX = 0;
		float outY = 0;
		if (_deltas.x() > 0.0001f) {
			if (_gravity == left || _gravity == buttomLeft || _gravity == topLeft) {
				// nothing to do
			} else if (_gravity == right || _gravity == buttomRight || _gravity == topRight) {
				outX = (int) (_deltas.x());
			} else {
				outX = (int) (_deltas.x() * 0.5f);
			}
		}
		if (_deltas.y() > 0.0001f) {
			if (_gravity == buttom || _gravity == buttomLeft || _gravity == buttomRight) {
				// nothing to do
			} else if (_gravity == top || _gravity == topRight || _gravity == topLeft) {
				outY = (int) (_deltas.y());
			} else {
				outY = (int) (_deltas.y() * 0.5f);
			}
		}
		return new Vector2f(outX, outY);
	}

}
