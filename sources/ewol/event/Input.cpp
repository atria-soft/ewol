/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Widget.h>

#undef __class__
#define __class__ "event::Input"

etk::CCout& ewol::event::operator <<(etk::CCout& _os, const ewol::event::Input& _obj) {
	_os << "{type=" << _obj.getType();
	_os << " status=" << _obj.getStatus();
	_os << " id=" << _obj.getId();
	_os << " pos=" << _obj.getPos();
	_os << "}";
	return _os;
}

etk::CCout& ewol::event::operator <<(etk::CCout& _os, const ewol::event::InputSystem& _obj) {
	_os << _obj.m_event;
	return _os;
}
