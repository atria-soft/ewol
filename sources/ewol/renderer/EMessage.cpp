/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/renderer/EObject.h>

#undef __class__
#define __class__	"EMessage"

etk::CCout& ewol::operator <<(etk::CCout& _os, const ewol::EMessage& _obj)
{
	_os << "{";
	if (NULL != _obj.GetMessage()) {
		_os << "msg=\"" << _obj.GetMessage() << "\"";
	} else {
		_os << "msg=\"NULL\"";
	}
	_os << " data=\"" << _obj.GetData() << "\"}";
	return _os;
}

