/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <etk/math/Vector2D.h>

namespace ewol {
	/**
	 * @brief Gravity of the widget property
	 * @not_in_doc
	 */
	enum gravity {
		gravity_center      = 0x00, //!< gravity is in center
		gravity_top         = 0x01, //!< gravity is in top
		gravity_buttom      = 0x02, //!< gravity is in buttom
		gravity_right       = 0x04, //!< gravity is in right
		gravity_left        = 0x08, //!< gravity is in left
		gravity_topRight    = gravity_top|gravity_right, //!< gravity is in top-right
		gravity_topLeft     = gravity_top|gravity_left, //!< gravity is in top-left
		gravity_buttomRight = gravity_buttom|gravity_right, //!< gravity is in buttom-right
		gravity_buttomLeft  = gravity_buttom|gravity_left, //!< gravity is in buttom-left
	};
	std::ostream& operator <<(std::ostream& _os, const enum ewol::gravity _obj);
	std::string gravityToString(const enum ewol::gravity _obj);
	enum ewol::gravity stringToGravity(const std::string& _obj);
	vec2 gravityGenerateDelta(const enum ewol::gravity _gravity, const vec2& _deltas);
}
