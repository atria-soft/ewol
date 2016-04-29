/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/DrawProperty.h>
#include <ewol/debug.h>

std::ostream& ewol::operator <<(std::ostream& _os, const ewol::DrawProperty& _obj) {
	_os << "{ windowsSize=" << _obj.m_windowsSize << " start=" << _obj.m_origin << " stop=" << (_obj.m_origin+_obj.m_size) << "}";
	return _os;
}

void ewol::DrawProperty::limit(const vec2& _origin, const vec2& _size) {
	m_size += m_origin;
	m_origin.setMax(_origin);
	m_size.setMin(_origin+_size);
	m_size -= m_origin;
}

