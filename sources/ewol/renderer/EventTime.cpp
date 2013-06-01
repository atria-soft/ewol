/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Widget.h>

#undef __class__
#define __class__	"EventTime"

etk::CCout& ewol::operator <<(etk::CCout& _os, const ewol::EventTime& _obj)
{
	_os << "{time=" << _obj.GetTime();
	_os << " uptime=" << _obj.GetApplUpTime();
	_os << " delta=" << _obj.GetDelta();
	_os << " deltaCall=" << _obj.GetDeltaCall();
	_os << "}";
	return _os;
}
