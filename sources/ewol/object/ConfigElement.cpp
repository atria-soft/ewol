/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/object/ConfigElement.h>

#undef __class__
#define __class__ "object::ConfigElement"

std::ostream& ewol::operator <<(std::ostream& _os, const ewol::object::ConfigElement& _obj) {
	_os << "{";
	if (nullptr != _obj.getConfig()) {
		_os << "config=\"" << _obj.getConfig() << "\"";
	}
	if (nullptr != _obj.getType()) {
		_os << " type=\"" << _obj.getType() << "\"";
	}
	if (nullptr != _obj.getControl()) {
		_os << " ctrl=\"" << _obj.getControl() << "\"";
	}
	if (nullptr != _obj.getDescription()) {
		_os << " desc=\"" << _obj.getDescription() << "\"";
	}
	if (nullptr != _obj.getDefault()) {
		_os << " default=\"" << _obj.getDefault() << "\"";
	}
	_os << "}";
	return _os;
}

