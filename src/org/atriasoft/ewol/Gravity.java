/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol;

import org.atriasoft.etk.math.Vector2f;

/**
 * @brief Gravity of the widget property
 */
public enum Gravity {
	center, //!< gravity is in center
	top, //!< gravity is in top
	buttom, //!< gravity is in buttom
	right, //!< gravity is in right
	left, //!< gravity is in left
	topRight, //!< gravity is in top-right
	topLeft, //!< gravity is in top-left
	buttomRight, //!< gravity is in buttom-right
	buttomLeft; //!< gravity is in buttom-left
	
	Vector2f gravityGenerateDelta(final Gravity _gravity, final Vector2f _deltas) {
		final Vector2f out = new Vector2f(0.0f, 0.0f);
		if (_deltas.x > 0.0001f) {
			if (_gravity == left || _gravity == buttomLeft || _gravity == topLeft) {
				// nothing to do
			} else if (_gravity == right || _gravity == buttomRight || _gravity == topRight) {
				out.x = (int) (_deltas.x);
			} else {
				out.x = (int) (_deltas.x * 0.5f);
			}
		}
		if (_deltas.y > 0.0001f) {
			if (_gravity == buttom || _gravity == buttomLeft || _gravity == buttomRight) {
				// nothing to do
			} else if (_gravity == top || _gravity == topRight || _gravity == topLeft) {
				out.y = (int) (_deltas.y);
			} else {
				out.y = (int) (_deltas.y * 0.5f);
			}
		}
		return out;
	}
	
}
