/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/renderer/EObject.h>

#undef __class__
#define __class__	"EConfig"

etk::CCout& ewol::operator <<(etk::CCout& _os, const ewol::EConfig& _obj)
{
	_os << "{";
	_os << "config=\"" << _obj.GetConfig() << "\"";
	_os << " data=\"" << _obj.GetData() << "\"}";
	return _os;
}

etk::CCout& ewol::operator <<(etk::CCout& _os, const ewol::EConfigElement& _obj)
{
	_os << "{";
	if (NULL != _obj.GetConfig()) {
		_os << "config=\"" << _obj.GetConfig() << "\"";
	}
	if (NULL != _obj.GetType()) {
		_os << " type=\"" << _obj.GetType() << "\"";
	}
	if (NULL != _obj.GetControl()) {
		_os << " ctrl=\"" << _obj.GetControl() << "\"";
	}
	if (NULL != _obj.GetDescription()) {
		_os << " desc=\"" << _obj.GetDescription() << "\"";
	}
	if (NULL != _obj.GetDefault()) {
		_os << " default=\"" << _obj.GetDefault() << "\"";
	}
	_os << "}";
	return _os;
}

