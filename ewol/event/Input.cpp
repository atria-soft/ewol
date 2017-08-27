/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/Widget.hpp>

etk::Stream& ewol::event::operator <<(etk::Stream& _os, const ewol::event::Input& _obj) {
	_os << "{type=" << _obj.getType();
	_os << " status=" << _obj.getStatus();
	_os << " id=" << etk::toString(_obj.getId());
	_os << " pos=" << _obj.getPos();
	_os << "}";
	return _os;
}

etk::Stream& ewol::event::operator <<(etk::Stream& _os, const ewol::event::InputSystem& _obj) {
	_os << _obj.m_event;
	return _os;
}
