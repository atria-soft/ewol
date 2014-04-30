/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/object/Config.h>
#include <etk/types.h>


#undef __class__
#define __class__ "object::Config"

std::ostream& ewol::operator <<(std::ostream& _os, const ewol::object::Config& _obj) {
	_os << "{";
	_os << "config=\"" << _obj.getConfig() << "\"";
	_os << " data=\"" << _obj.getData() << "\"}";
	return _os;
}
