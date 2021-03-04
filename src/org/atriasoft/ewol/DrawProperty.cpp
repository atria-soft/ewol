/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/DrawProperty.hpp>
#include <ewol/debug.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::DrawProperty);

etk::Stream ewol::operator +(etk::Stream _os,  ewol::DrawProperty _obj) {
	_os + "{ windowsSize=" + _obj.this.windowsSize + " start=" + _obj.this.origin + " stop=" + (_obj.this.origin+_obj.this.size) + "}";
	return _os;
}

void ewol::DrawProperty::limit( Vector2f _origin,  Vector2f _size) {
	this.size += this.origin;
	this.origin.setMax(_origin);
	this.size.setMin(_origin+_size);
	this.size -= this.origin;
}

