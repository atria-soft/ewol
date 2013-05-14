/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Widget.h>

#undef __class__
#define __class__	"EventInput"

etk::CCout& ewol::operator <<(etk::CCout& _os, const ewol::EventInput& _obj)
{
	_os << "{type=" << _obj.GetType();
	_os << " status=" << _obj.GetStatus();
	_os << " id=" << _obj.GetId();
	_os << " pos=" << _obj.GetPos();
	_os << "}";
	return _os;
}

etk::CCout& ewol::operator <<(etk::CCout& _os, const ewol::EventInputSystem& _obj)
{
	_os << _obj.m_event;
	return _os;
}
