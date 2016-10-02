/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/widget/Widget.hpp>

std::ostream& ewol::event::operator <<(std::ostream& _os, const ewol::event::Entry& _obj) {
	_os << "{type=" << _obj.getType();
	_os << " status=" << _obj.getStatus();
	if (_obj.getType() == gale::key::keyboard::character) {
		_os << " char=" << _obj.getChar();
	}
	_os << "}";
	return _os;
}

std::ostream& ewol::event::operator <<(std::ostream& _os, const ewol::event::EntrySystem& _obj) {
	_os << _obj.m_event;
	return _os;
}
