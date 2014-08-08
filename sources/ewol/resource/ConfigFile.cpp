/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <etk/os/FSNode.h>
#include <ewol/debug.h>
#include <ewol/resource/ConfigFile.h>
#include <ewol/resource/Manager.h>
#include <ejson/ejson.h>
#include <ejson/Number.h>
#include <ejson/String.h>
#include <stdexcept>

#undef __class__
#define __class__	"resource::ConfigFile"


ewol::resource::ConfigFile::ConfigFile() :
  ewol::Resource() {
	addObjectType("ewol::ConfigFile");
}

void ewol::resource::ConfigFile::init(const std::string& _filename) {
	ewol::Resource::init(_filename);
	EWOL_DEBUG("SFP : load \"" << _filename << "\"");
	reload();
}


ewol::resource::ConfigFile::~ConfigFile() {
	// remove all element
	for (int32_t iii=0; iii<m_list.size(); iii++){
		if (nullptr != m_list[iii]) {
			delete(m_list[iii]);
			m_list[iii] = nullptr;
		}
	}
	m_list.clear();
}

void ewol::resource::ConfigFile::reload() {
	// reset all parameters
	for (int32_t iii=0; iii<m_list.size(); iii++){
		if (nullptr != m_list[iii]) {
			m_list[iii] = nullptr;
		}
	}
	m_doc.load(m_name);
	
	for (auto elementName : m_list.getKeys()) {
		if (m_doc.exist(elementName) == true) {
			m_list[elementName] = m_doc[elementName];
		}
	}
}


int32_t ewol::resource::ConfigFile::request(const std::string& _paramName) {
	// check if the parameters existed :
	if (m_list.exist(_paramName) == false) {
		m_list.add(_paramName, nullptr);
	}
	if (m_doc.exist(_paramName) == true) {
		m_list[_paramName] = m_doc[_paramName];
	}
	return m_list.getId(_paramName);
}


double ewol::resource::ConfigFile::getNumber(int32_t _id) {
	if (    _id < 0
	     || m_list[_id] == nullptr) {
		return 0.0;
	}
	ejson::Number* tmp = m_list[_id]->toNumber();
	if (tmp == nullptr) {
		return 0.0;
	}
	return tmp->get();
}

const std::string& ewol::resource::ConfigFile::getString(int32_t _id) {
	static const std::string& errorString("");
	if (    _id < 0
	     || m_list[_id] == nullptr) {
		return errorString;
	}
	ejson::String* tmp = m_list[_id]->toString();
	if (tmp == nullptr) {
		return errorString;
	}
	return tmp->get();
}

bool ewol::resource::ConfigFile::getBoolean(int32_t _id) {
	if (    _id < 0
	     || m_list[_id] == nullptr) {
		return false;
	}
	ejson::Boolean* tmp = m_list[_id]->toBoolean();
	if (tmp == nullptr) {
		return false;
	}
	return tmp->get();
}
