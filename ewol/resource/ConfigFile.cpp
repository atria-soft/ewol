/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etk/os/FSNode.hpp>
#include <ewol/debug.hpp>
#include <ewol/resource/ConfigFile.hpp>
#include <gale/resource/Manager.hpp>
#include <ejson/ejson.hpp>
#include <ejson/Number.hpp>
#include <ejson/String.hpp>

ewol::resource::ConfigFile::ConfigFile() :
  gale::Resource(),
  // set map unorderred
  m_list(0, false) {
	addResourceType("ewol::ConfigFile");
}

void ewol::resource::ConfigFile::init(const etk::String& _filename) {
	ethread::RecursiveLock lock(m_mutex);
	gale::Resource::init(_filename);
	EWOL_DEBUG("SFP : load \"" << _filename << "\"");
	reload();
}


ewol::resource::ConfigFile::~ConfigFile() {
	m_list.clear();
}

void ewol::resource::ConfigFile::reload() {
	ethread::RecursiveLock lock(m_mutex);
	// reset all parameters
	for (int32_t iii=0; iii<m_list.size(); iii++){
		if (m_list.getValue(iii).exist() == true) {
			m_list.getValue(iii) = ejson::empty();
		}
	}
	m_doc.load(m_name);
	
	for (auto elementName : m_list.getKeys()) {
		if (m_doc[elementName].exist() == true) {
			m_list[elementName] = m_doc[elementName];
		}
	}
}


int32_t ewol::resource::ConfigFile::request(const etk::String& _paramName) {
	ethread::RecursiveLock lock(m_mutex);
	// check if the parameters existed :
	if (m_list.exist(_paramName) == false) {
		m_list.add(_paramName, ejson::empty());
	}
	if (m_doc[_paramName].exist() == true) {
		m_list[_paramName] = m_doc[_paramName];
	}
	return m_list.getId(_paramName);
}


double ewol::resource::ConfigFile::getNumber(int32_t _id) {
	ethread::RecursiveLock lock(m_mutex);
	if (    _id < 0
	     || m_list.getValue(_id).exist() == false) {
		return 0.0;
	}
	return m_list.getValue(_id).toNumber().get();
}

etk::String ewol::resource::ConfigFile::getString(int32_t _id) {
	ethread::RecursiveLock lock(m_mutex);
	if (    _id < 0
	     || m_list.getValue(_id).exist() == false) {
		return "";
	}
	return m_list.getValue(_id).toString().get();
}

bool ewol::resource::ConfigFile::getBoolean(int32_t _id) {
	ethread::RecursiveLock lock(m_mutex);
	if (    _id < 0
	     || m_list.getValue(_id).exist() == false) {
		return false;
	}
	return m_list.getValue(_id).toBoolean().get();
}
