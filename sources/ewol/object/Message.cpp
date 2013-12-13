/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/object/Object.h>

#undef __class__
#define __class__ "object/Message"

etk::CCout& ewol::object::operator <<(etk::CCout& _os, const ewol::object::Message& _obj) {
	_os << "{";
	if (NULL != _obj.getMessage()) {
		_os << "msg=\"" << _obj.getMessage() << "\"";
	} else {
		_os << "msg={NULL}";
	}
	_os << " data=\"" << _obj.getData() << "\"}";
	return _os;
}

