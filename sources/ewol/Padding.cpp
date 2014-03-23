/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD 3 clauses (see license file)
 */

#include <ewol/Padding.h>

etk::CCout& ewol::operator <<(etk::CCout& _os, const ewol::Padding& _obj) {
	_os << "{";
	_os << _obj.xLeft();
	_os << ",";
	_os << _obj.yTop();
	_os << ",";
	_os << _obj.xRight();
	_os << ",";
	_os << _obj.yButtom();
	_os << "}";
	return _os;
}

