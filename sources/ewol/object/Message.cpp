/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/object/Object.h>

#undef __class__
#define __class__ "object/Message"

std::ostream& ewol::object::operator <<(std::ostream& _os, const ewol::object::Message& _obj) {
	_os << "{";
	if (nullptr != _obj.getMessage()) {
		_os << "msg=\"" << _obj.getMessage() << "\"";
	} else {
		_os << "msg={nullptr}";
	}
	_os << " data=\"" << _obj.getData() << "\"}";
	return _os;
}

