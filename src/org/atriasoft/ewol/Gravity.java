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
	BUTTOM, // !< gravity is in center
	BUTTOM_LEFT, // !< gravity is in top
	BUTTOM_RIGHT, // !< gravity is in buttom
	CENTER, // !< gravity is in right
	LEFT, // !< gravity is in left
	RIGHT, // !< gravity is in top-right
	TOP, // !< gravity is in top-left
	TOP_LEFT, // !< gravity is in buttom-right
	TOP_RIGHT; // !< gravity is in buttom-left
	
	public static Vector2f gravityGenerateDelta(final Gravity gravity, final Vector2f deltas) {
		float outX = 0;
		float outY = 0;
		if (deltas.x() > 0.0001f) {
			if (gravity == LEFT || gravity == BUTTOM_LEFT || gravity == TOP_LEFT) {
				// nothing to do
			} else if (gravity == RIGHT || gravity == BUTTOM_RIGHT || gravity == TOP_RIGHT) {
				outX = (int) (deltas.x());
			} else {
				outX = (int) (deltas.x() * 0.5f);
			}
		}
		if (deltas.y() > 0.0001f) {
			if (gravity == BUTTOM || gravity == BUTTOM_LEFT || gravity == BUTTOM_RIGHT) {
				// nothing to do
			} else if (gravity == TOP || gravity == TOP_RIGHT || gravity == TOP_LEFT) {
				outY = (int) (deltas.y());
			} else {
				outY = (int) (deltas.y() * 0.5f);
			}
		}
		return new Vector2f(outX, outY);
	}
	
}

