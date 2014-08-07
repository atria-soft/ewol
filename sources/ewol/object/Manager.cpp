/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/object/Manager.h>
#include <ewol/context/Context.h>
#include <ewol/ewol.h>
#include <etk/stdTools.h>
#include <unistd.h>

#undef __class__
#define __class__ "ewol::object::Manager"

ewol::object::Manager::Manager(ewol::Context& _context) :
  m_context(_context) {
	EWOL_DEBUG(" == > init Object-Manager");
}

ewol::object::Manager::~Manager() {
	bool hasError = false;
	if (m_eObjectList.size()!=0) {
		EWOL_ERROR("Must not have anymore eObject !!!");
		hasError = true;
	}
	if (true == hasError) {
		EWOL_ERROR("Check if the function UnInit has been called !!!");
	}
	displayListObject();
}

void ewol::object::Manager::displayListObject() {
	EWOL_INFO("List loaded object : ");
	for (auto &it : m_eObjectList) {
		std::shared_ptr<ewol::Object> element = it.lock();
		if (element != nullptr) {
			EWOL_INFO("  name='" << element->getName() << "' type=" << element->getObjectType());
		}
	}
}

void ewol::object::Manager::unInit() {
	EWOL_DEBUG(" == > Un-Init Object-Manager");
	for (auto &it : m_eObjectList) {
		std::shared_ptr<ewol::Object> element = it.lock();
		if (element != nullptr) {
			//it->removeObject();
		}
	}
	if (m_eObjectList.size() != 0) {
		EWOL_ERROR("Have " << m_eObjectList.size() << " active Object");
	}
	m_multiCast.clear();
	m_eObjectList.clear();
}

void ewol::object::Manager::add(const std::shared_ptr<ewol::Object>& _object) {
	if (_object == nullptr) {
		EWOL_ERROR("try to add an inexistant Object in manager");
	}
	m_eObjectList.push_back(_object);
}

int32_t ewol::object::Manager::getNumberObject() {
	return m_eObjectList.size();
}

void ewol::object::Manager::informOneObjectIsRemoved(const std::shared_ptr<ewol::Object>& _object) {
	EWOL_TODO("ewol::object::Manager::informOneObjectIsRemoved()");
	/*
	for (auto &it : m_eObjectList) {
		std::shared_ptr<ewol::Object> element = it.lock();
		if (    element != nullptr
		     && element != _object) {
			EWOL_VERBOSE("[" << _object->getId() << "] onObjectRemove() : " << element->getId() << " type=" << element->getObjectType() << " name='" << element->getName() << "'");
			element->onObjectRemove(_object);
		}
	}
	// inform context that 	n object is removed ...
	m_context.onObjectRemove(_object);
	m_multiCast.onObjectRemove(_object);
	EWOL_VERBOSE("m_removeEventList.size() = " << m_removeEventList.size());
	for (auto &it : m_removeEventList) {
		EWOL_VERBOSE("[" << _object->getId() << "] Inform Event Remove Object List : ...");
		std::shared_ptr<ewol::Object> element = it.lock();
		if (element != nullptr) {
			element->onObjectRemove(_object);
		}
	}
	*/
}

void ewol::object::Manager::remove(const std::shared_ptr<ewol::Object>& _object) {
	EWOL_TODO("ewol::object::Manager::remove()");
	/*
	if (_object == nullptr) {
		EWOL_ERROR("Try to Auto-Remove (nullptr) Object");
		return;
	}
	int32_t count = 0;
	auto it(m_eObjectListActive.begin());
	while (it != m_eObjectListActive.end()) {
		if (*it == _object) {
			m_eObjectListActive.erase(it);
			// remove Element
			EWOL_DEBUG("Auto-Remove Object : [" << _object->getId() << "] type='" << _object->getObjectType() << "' name=" << _object->getName());
			informOneObjectIsRemoved(_object);
			ewol::getContext().forceRedrawAll();
			EWOL_VERBOSE("Auto-Remove Object ... done (have " << _object->getRefCount() << " references)");
			it = m_eObjectListActive.begin();
			count++;
		} else {
			++it;
		}
	}
	if (count == 0) {
		EWOL_ERROR("Try to Auto-Remove Object that is not referenced ...");
	} else if (    count>1
	            || count<0) {
		EWOL_ERROR("Remove more than one object in the system list ==> this is a real problem ...");
	}
	*/
}

// clean all Object that request an autoRemove ...
void ewol::object::Manager::removeAllRemovedObject() {
	size_t nbObject = m_eObjectList.size();
	EWOL_VERBOSE("Clean Object List (if needed) : " << m_eObjectList.size() << " elements");
	auto it(m_eObjectList.begin());
	while (it != m_eObjectList.end()) {
		if (it->expired() == true) {
			m_eObjectList.erase(it);
			it = m_eObjectList.begin();
		} else {
			++it;
		}
	}
	if (m_eObjectList.size() != nbObject) {
		EWOL_VERBOSE(" remove " << nbObject - m_eObjectList.size() << " deprecated objects");
	}
}

std::shared_ptr<ewol::Object> ewol::object::Manager::get(const std::string& _name) {
	if (_name == "") {
		return nullptr;
	}
	for (auto &it : m_eObjectList) {
		std::shared_ptr<ewol::Object> element = it.lock();
		if (    element != nullptr
		     && element->getName() == _name) {
			return element;
		}
	}
	return nullptr;
}


void ewol::object::Manager::add(ewol::object::RemoveEvent* _class) {
	m_removeEventList.push_back(_class);
}

void ewol::object::Manager::rm(ewol::object::RemoveEvent* _class) {
	for (size_t iii=0; iii<m_removeEventList.size(); ++iii) {
		if (m_removeEventList[iii] == _class) {
			m_removeEventList.erase(m_removeEventList.begin() + iii);
			return;
		}
	}
}

std::shared_ptr<ewol::Object> ewol::object::Manager::getObjectNamed(const std::string& _name) {
	return ewol::object::Manager::get(_name);
}

