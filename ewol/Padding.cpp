/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/Padding.hpp>

ewol::Padding::Padding() {
	// nothing to do...
}

ewol::Padding::Padding(float _xl, float _yt, float _xr, float _yb) {
	setValue(_xl, _yt, _xr, _yb);
}

void ewol::Padding::setValue(float _xl, float _yt, float _xr, float _yb) {
	m_value[0] = _xl;
	m_value[1] = _yt;
	m_value[2] = _xr;
	m_value[3] = _yb;
}

float ewol::Padding::x() const {
	return m_value[0] + m_value[2];
}

float ewol::Padding::y() const {
	return m_value[1] + m_value[3];
}

float ewol::Padding::xLeft() const {
	return m_value[0];
}

void ewol::Padding::setXLeft(float _val) {
	m_value[0] = _val;
}

float ewol::Padding::xRight() const {
	return m_value[2];
}

void ewol::Padding::setXRight(float _val) {
	m_value[2] = _val;
}

float ewol::Padding::yTop() const {
	return m_value[1];
}

void ewol::Padding::setYTop(float _val) {
	m_value[1] = _val;
}

float ewol::Padding::yButtom() const {
	return m_value[3];
}

void ewol::Padding::setYButtom(float _val) {
	m_value[3] = _val;
}

ewol::Padding& ewol::Padding::operator+=(const Padding& _v) {
	m_value[0] += _v.m_value[0];
	m_value[1] += _v.m_value[1];
	m_value[2] += _v.m_value[2];
	m_value[3] += _v.m_value[3];
	return *this;
}

ewol::Padding ewol::Padding::operator+(const Padding& _v) {
	return Padding(m_value[0] + _v.m_value[0],
	               m_value[1] + _v.m_value[1],
	               m_value[2] + _v.m_value[2],
	               m_value[3] + _v.m_value[3]);
}

std::ostream& ewol::operator <<(std::ostream& _os, const ewol::Padding& _obj) {
	_os << "{";
	_os << _obj.xLeft();
	_os << ",";
	_os << _obj.yTop();
	_os << ",";
	_os << _obj.xRight();
	_os << ",";
	_os << _obj.yButtom();
	_os << "}";
	return _os;
}

