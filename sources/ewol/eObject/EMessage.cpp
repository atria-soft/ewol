/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/eObject/EObject.h>

#undef __class__
#define __class__	"EMessage"

etk::CCout& ewol::operator <<(etk::CCout &os, const ewol::EMessage &obj)
{
	if (NULL != obj.GetMessage()) {
		os << " msg=\"" << obj.GetMessage() << "\"";
	} else {
		os << " msg=\"NULL\"";
	}
	os << " data=\"" << obj.GetData() << "\"";
	return os;
}

