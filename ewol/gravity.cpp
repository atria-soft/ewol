/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/debug.hpp>
#include <ewol/gravity.hpp>

std::string ewol::gravityToString(const enum ewol::gravity _obj) {
	switch(_obj) {
		case ewol::gravity_center:
			return "center";
		case ewol::gravity_topLeft:
			return "top-left";
		case ewol::gravity_top:
			return "top";
		case ewol::gravity_topRight:
			return "top-right";
		case ewol::gravity_right:
			return "right";
		case ewol::gravity_buttomRight:
			return "buttom-right";
		case ewol::gravity_buttom:
			return "buttom";
		case ewol::gravity_buttomLeft:
			return "buttom-left";
		case ewol::gravity_left:
			return "left";
	}
	return "unknow";
}

enum ewol::gravity ewol::stringToGravity(const std::string& _obj) {
	if (_obj == "center") {
		return ewol::gravity_center;
	} else if (_obj == "top-left") {
		return ewol::gravity_topLeft;
	} else if (_obj == "top") {
		return ewol::gravity_top;
	} else if (_obj == "top-right") {
		return ewol::gravity_topRight;
	} else if (_obj == "right") {
		return ewol::gravity_right;
	} else if (_obj == "buttom-right") {
		return ewol::gravity_buttomRight;
	} else if (_obj == "buttom") {
		return ewol::gravity_buttom;
	} else if (_obj == "buttom-left") {
		return ewol::gravity_buttomLeft;
	} else if (_obj == "left") {
		return ewol::gravity_left;
	}
	return ewol::gravity_center;
}
vec2 ewol::gravityGenerateDelta(const enum ewol::gravity _gravity, const vec2& _deltas) {
	vec2 out(0.0f,0.0f);
	if (_deltas.x() > 0.0001f) {
		if ((uint32_t(_gravity) & uint32_t(ewol::gravity_left)) != 0) {
			// nothing to do
		} else if ((uint32_t(_gravity) & uint32_t(ewol::gravity_right)) != 0) {
			out = vec2(int32_t(_deltas.x()), 0.0f);
		} else {
			out = vec2(int32_t(_deltas.x()*0.5f), 0.0f);
		}
	}
	if (_deltas.y() > 0.0001f) {
		if ((uint32_t(_gravity) & uint32_t(ewol::gravity_buttom)) != 0) {
			// nothing to do
		} else if ((uint32_t(_gravity) & uint32_t(ewol::gravity_top)) != 0) {
			out += vec2(0.0f, int32_t(_deltas.y()));
		} else {
			out += vec2(0.0f, int32_t(_deltas.y()*0.5f));
		}
	}
	return out;
}

std::ostream& ewol::operator <<(std::ostream& _os, const enum ewol::gravity _obj) {
	_os << ewol::gravityToString(_obj);
	return _os;
}

