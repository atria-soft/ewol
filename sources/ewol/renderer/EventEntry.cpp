/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Widget.h>

#undef __class__
#define __class__	"EventEntry"

etk::CCout& ewol::operator <<(etk::CCout& _os, const ewol::EventEntry& _obj)
{
	_os << "{type=" << _obj.GetType();
	_os << " status=" << _obj.GetStatus();
	if (_obj.GetType() == ewol::keyEvent::keyboardChar) {
		_os << " char=" << _obj.GetChar();
	}
	_os << "}";
	return _os;
}

etk::CCout& operator <<(etk::CCout& _os, const ewol::EventEntrySystem& _obj)
{
	_os << _obj.m_event;
	return _os;
}
