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
#include <unistd.h>

#undef __class__
#define __class__ "ewol::object::Manager"

ewol::object::Manager::Manager(ewol::Context& _context) :
  m_context(_context) {
	EWOL_DEBUG(" == > init Object-Manager");
}

ewol::object::Manager::~Manager() {
	bool hasError = false;
	if (m_eObjectListActive.size()!=0) {
		EWOL_ERROR("Must not have anymore eObject to auto-remove !!!");
		hasError = true;
	}
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
	for (auto &it : m_eObjectList) {
		EWOL_ERROR("List object : " << it->getName() << " : " << it->getTypeDescription() << " nb ref=" << it->getRefCount());
	}
}

void ewol::object::Manager::unInit() {
	EWOL_DEBUG(" == > Un-Init Object-Manager");
	for (auto &it : m_eObjectList) {
		if (it != nullptr) {
			//it->removeObject();
		}
	}
	removeAllRemovedObject();
	EWOL_INFO(" remove missing user object");
	size_t iii=0;
	if (m_eObjectListActive.size() != 0) {
		EWOL_ERROR("Have " << m_eObjectListActive.size() << " active Object");
	}
	m_eObjectListActive.clear();
	m_eObjectList.clear();
	removeAllRemovedObject();
}

void ewol::object::Manager::add(const ewol::object::Shared<ewol::Object>& _object) {
	if (_object == nullptr) {
		EWOL_ERROR("try to add an inexistant Object in manager");
	}
	m_eObjectList.push_back(_object);
	m_eObjectListActive.push_back(_object);
}

int32_t ewol::object::Manager::getNumberObject() {
	return m_eObjectList.size();
}

void ewol::object::Manager::informOneObjectIsRemoved(const ewol::object::Shared<ewol::Object>& _object) {
	for (auto &it : m_eObjectList) {
		if (    it != nullptr
		     && it != _object) {
			EWOL_VERBOSE("[" << _object->getId() << "] Inform remove Element : " << it->getId() << " type : " << it->getTypeDescription());
			it->onObjectRemove(_object);
		}
	}
	// inform context that 	n object is removed ...
	m_context.onObjectRemove(_object);
	EWOL_VERBOSE("m_removeEventList.size() = " << m_removeEventList.size());
	for (auto &it : m_removeEventList) {
		EWOL_VERBOSE("[" << _object->getId() << "] Inform Event Remove Object List : ...");
		it->onObjectRemove(_object);
	}
}

void ewol::object::Manager::respown(const ewol::object::Shared<ewol::Object>& _object){
	if (_object == nullptr) {
		EWOL_ERROR("Try to respown nullptr Object");
		return;
	}
	for (auto it : m_eObjectListActive) {
		if (it == _object) {
			EWOL_ERROR("try to respawn an existing active Object : [" << _object->getId() << "] type='" << _object->getObjectType() << "'");
			return;
		}
	}
	m_eObjectListActive.push_back(_object);
}

void ewol::object::Manager::remove(const ewol::object::Shared<ewol::Object>& _object) {
	if (_object == nullptr) {
		EWOL_ERROR("Try to Auto-Remove (nullptr) Object");
		return;
	}
	for (int64_t iii = (int64_t)m_eObjectListActive.size()-1; iii>=0; --iii) {
		if (m_eObjectListActive[iii] == _object) {
			// remove Element
			EWOL_DEBUG("Auto-Remove Object : [" << _object->getId() << "] type='" << _object->getObjectType() << "'");
			informOneObjectIsRemoved(_object);
			ewol::getContext().forceRedrawAll();
			EWOL_DEBUG("Auto-Remove Object ... done (have " << _object->getRefCount() << " references)");
			m_eObjectListActive.erase(m_eObjectListActive.begin()+iii);
			return;
		}
	}
	EWOL_ERROR("Try to Auto-Remove Object that is not referenced ...");
}

// clean all Object that request an autoRemove ...
void ewol::object::Manager::removeAllRemovedObject() {
	EWOL_VERBOSE("Clean Object List (if needed) : " << m_eObjectListActive.size() << "/" << m_eObjectList.size() << " elemeents");
	bool haveRemoveElement = true;
	while (haveRemoveElement == true) {
		haveRemoveElement = false;
		for (int64_t iii = (int64_t)m_eObjectList.size()-1; iii>=0; --iii) {
			if (m_eObjectList[iii] == nullptr) {
				continue;
			}
			if (m_eObjectList[iii]->getRefCount() > 1) {
				continue;
			}
			EWOL_DEBUG("remove definitly : [" << m_eObjectList[iii]->getId() << "] type='" << m_eObjectList[iii]->getObjectType() << "'");
			m_eObjectList.erase(m_eObjectList.begin() + iii);
			haveRemoveElement = true;
			break;
		}
	}
}

ewol::object::Shared<ewol::Object> ewol::object::Manager::get(const std::string& _name) {
	if (_name == "") {
		return nullptr;
	}
	for (size_t iii=0; iii<m_eObjectList.size(); iii++) {
		if (m_eObjectList[iii] != nullptr) {
			if (m_eObjectList[iii]->getName() == _name) {
				return m_eObjectList[iii];
			}
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

