/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
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


ewol::resource::ConfigFile::ConfigFile(const std::string& _filename) :
  ewol::Resource(_filename) {
	addObjectType("ewol::ConfigFile");
	EWOL_DEBUG("SFP : load \"" << _filename << "\"");
	reload();
}


ewol::resource::ConfigFile::~ConfigFile(void) {
	// remove all element
	for (size_t iii=0; iii<m_list.size(); iii++){
		if (NULL != m_list[iii]) {
			delete(m_list[iii]);
			m_list[iii] = NULL;
		}
	}
	m_list.clear();
}

void ewol::resource::ConfigFile::reload(void) {
	// reset all parameters
	for (size_t iii=0; iii<m_list.size(); iii++){
		if (NULL != m_list[iii]) {
			m_list[iii] = NULL;
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
		m_list.add(_paramName, NULL);
	}
	if (m_doc.exist(_paramName) == true) {
		m_list[_paramName] = m_doc[_paramName];
	}
	return m_list.getId(_paramName);
}


double ewol::resource::ConfigFile::getNumber(int32_t _id) {
	if (    _id < 0
	     || m_list[_id] == NULL) {
		return 0.0;
	}
	ejson::Number* tmp = m_list[_id]->toNumber();
	if (tmp == NULL) {
		return 0.0;
	}
	return tmp->get();
}

const std::string& ewol::resource::ConfigFile::getString(int32_t _id) {
	static const std::string& errorString("");
	if (    _id < 0
	     || m_list[_id] == NULL) {
		return errorString;
	}
	ejson::String* tmp = m_list[_id]->toString();
	if (tmp == NULL) {
		return errorString;
	}
	return tmp->get();
}

bool ewol::resource::ConfigFile::getBoolean(int32_t _id) {
	if (    _id < 0
	     || m_list[_id] == NULL) {
		return false;
	}
	ejson::Boolean* tmp = m_list[_id]->toBoolean();
	if (tmp == NULL) {
		return false;
	}
	return tmp->get();
}

ewol::resource::ConfigFile* ewol::resource::ConfigFile::keep(const std::string& _filename) {
	EWOL_INFO("KEEP : SimpleConfig : file : \"" << _filename << "\"");
	ewol::resource::ConfigFile* object = NULL;
	ewol::Resource* object2 = getManager().localKeep(_filename);
	if (NULL != object2) {
		object = dynamic_cast<ewol::resource::ConfigFile*>(object2);
		if (NULL == object) {
			EWOL_CRITICAL("Request resource file : '" << _filename << "' With the wrong type (dynamic cast error)");
			return NULL;
		}
	}
	if (NULL != object) {
		return object;
	}
	// this element create a new one every time ....
	object = new ewol::resource::ConfigFile(_filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : '" << _filename << "'");
		return NULL;
	}
	getManager().localAdd(object);
	return object;
}

void ewol::resource::ConfigFile::release(ewol::resource::ConfigFile*& _object) {
	if (NULL == _object) {
		return;
	}
	ewol::Resource* object2 = static_cast<ewol::Resource*>(_object);
	getManager().release(object2);
	_object = NULL;
}



