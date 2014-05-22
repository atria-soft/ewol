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

ewol::object::Manager::Manager() {
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
}

void ewol::object::Manager::unInit() {
	EWOL_DEBUG(" == > Un-Init Object-Manager");
	removeAllRemovedObject();
	EWOL_INFO(" remove missing user object");
	size_t iii=0;
	if (m_eObjectListActive.size() != 0) {
		EWOL_ERROR("Have " << m_eObjectListActive.size() << " active Object");
	}
	while(iii < m_eObjectListActive.size()) {
		if (m_eObjectListActive[iii] != nullptr) {
			m_eObjectListActive.erase(m_eObjectListActive.begin()+iii);
		}
	}
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
			it->onObjectRemove(_object);
		}
	}
	// inform the context ...
	ewol::getContext().onObjectRemove(_object);
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
			m_eObjectListActive.erase(m_eObjectListActive.begin()+iii);
			EWOL_DEBUG("Auto-Remove Object : [" << _object->getId() << "] type='" << _object->getObjectType() << "'");
			if (_object->getStatusResource() == false) {
				informOneObjectIsRemoved(_object);
			}
			ewol::getContext().forceRedrawAll();
			EWOL_DEBUG("Auto-Remove Object ... done");
			return;
		}
	}
	EWOL_ERROR("Try to Auto-Remove Object that is not referenced ...");
}

// clean all Object that request an autoRemove ...
void ewol::object::Manager::removeAllRemovedObject() {
	//EWOL_DEBUG("Auto-Remove Object section : " << m_eObjectAutoRemoveList.size() << " elemeents");
	for (int64_t iii = (int64_t)m_eObjectList.size()-1; iii>=0; --iii) {
		if (m_eObjectList[iii] == nullptr) {
			continue;
		}
		if (m_eObjectList[iii]->getRefCount() >= 1) {
			continue;
		}
		EWOL_DEBUG("remove definitly : [" << m_eObjectList[iii]->getId() << "] type='" << m_eObjectList[iii]->getObjectType() << "'");
		m_eObjectList.erase(m_eObjectList.begin() + iii);
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

