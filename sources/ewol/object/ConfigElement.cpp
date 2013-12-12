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

etk::CCout& ewol::operator <<(etk::CCout& _os, const ewol::object::ConfigElement& _obj) {
	_os << "{";
	if (NULL != _obj.getConfig()) {
		_os << "config=\"" << _obj.getConfig() << "\"";
	}
	if (NULL != _obj.getType()) {
		_os << " type=\"" << _obj.getType() << "\"";
	}
	if (NULL != _obj.getControl()) {
		_os << " ctrl=\"" << _obj.getControl() << "\"";
	}
	if (NULL != _obj.getDescription()) {
		_os << " desc=\"" << _obj.getDescription() << "\"";
	}
	if (NULL != _obj.getDefault()) {
		_os << " default=\"" << _obj.getDefault() << "\"";
	}
	_os << "}";
	return _os;
}

