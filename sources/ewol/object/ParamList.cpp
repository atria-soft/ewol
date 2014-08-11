/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/object/ParamList.h>

void ewol::object::ParamList::add(int64_t _value, const std::string& _name, const std::string& _description) {
	auto it = m_list.find(_name);
	if (it != m_list.end()) {
		it->second = _value;
		return;
	}
	m_list.insert(std::make_pair(_name, _value));
}

std::string ewol::object::ParamList::getElement(int64_t _intValue) {
	for (auto &it : m_list) {
		if (it.second == _intValue) {
			return it.first;
		}
	}
	return "???";
}

std::string ewol::object::ParamList::getString() {
	return getElement(m_value);
}

std::string ewol::object::ParamList::getDefault() {
	return getElement(m_default);
}

std::string ewol::object::ParamList::getInfo() {
	return "List default=" + getElement(m_default);
}

void ewol::object::ParamList::setString(const std::string& _newVal) {
	auto it = m_list.find(_newVal);
	if (it != m_list.end()) {
		m_value = it->second;
		return;
	}
	EWOL_WARNING("paramList value='" << _newVal << "' is not un the list ... ==> no change");
	for (auto &it : m_list) {
		EWOL_DEBUG("        element : " << it.first);
	}
}


void ewol::object::ParamList::set(int64_t _newVal) {
	for (auto &it : m_list) {
		if (it.second == _newVal) {
			m_value = it.second;
			return;
		}
	}
	EWOL_WARNING("paramList value=" << _newVal << " is not un the list ... ==> no change");
}


std::ostream& ewol::object::operator <<(std::ostream& _os, const ewol::object::ParamList& _obj) {
	_os << _obj.get();
	return _os;
}

