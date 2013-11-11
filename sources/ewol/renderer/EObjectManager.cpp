/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/renderer/EObjectManager.h>
#include <ewol/renderer/eContext.h>
#include <ewol/ewol.h>

#undef __class__
#define __class__ "EObjectManager"

ewol::EObjectManager::EObjectManager(void) {
	EWOL_DEBUG(" == > init EObject-Manager");
	// Can create mlemory leak ...  == > but not predictable comportement otherwise ...
	m_eObjectAutoRemoveList.clear();
	m_eObjectList.clear();
}

ewol::EObjectManager::~EObjectManager(void) {
	bool hasError = false;
	if (m_eObjectAutoRemoveList.size()!=0) {
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

void ewol::EObjectManager::unInit(void) {
	EWOL_DEBUG(" == > Un-Init EObject-Manager");
	removeAllAutoRemove();
	EWOL_INFO(" remove missing user widget");
	int32_t iii=0;
	while(iii<m_eObjectList.size()) {
		if (m_eObjectList[iii]!=NULL) {
			if (m_eObjectList[iii]->getStatic() == true) {
				iii++;
			} else {
				EWOL_WARNING("Un-INIT : remove EObject type=\"" << m_eObjectList[iii]->getObjectType() << "\"");
				delete(m_eObjectList[iii]);
				m_eObjectList[iii] = NULL;
			}
		} else {
			m_eObjectList.erase(iii);
		}
	}
}

void ewol::EObjectManager::add(ewol::EObject* _object) {
	if (NULL != _object) {
		m_eObjectList.push_back(_object);
	} else {
		EWOL_ERROR("try to add an inexistant EObject in manager");
	}
}

int32_t ewol::EObjectManager::getNumberObject(void) {
	return m_eObjectList.size() + m_eObjectAutoRemoveList.size();
}

void ewol::EObjectManager::informOneObjectIsRemoved(ewol::EObject* _object) {
	for (int32_t iii=0; iii<m_eObjectList.size(); iii++) {
		if (m_eObjectList[iii] != NULL) {
			m_eObjectList[iii]->onObjectRemove(_object);
		}
	}
	for (int32_t iii=0; iii<m_eObjectAutoRemoveList.size(); iii++) {
		if(    m_eObjectAutoRemoveList[iii] != NULL
		    && m_eObjectAutoRemoveList[iii] != _object) {
			m_eObjectAutoRemoveList[iii]->onObjectRemove(_object);
		}
	}
	// call input event manager to remove linked widget ...
	ewol::getContext().onObjectRemove(_object);
}

void ewol::EObjectManager::rm(ewol::EObject* _object) {
	if (NULL == _object) {
		EWOL_ERROR("Try to remove (NULL) EObject");
		return;
	}
	for (int32_t iii=0; iii<m_eObjectList.size(); iii++) {
		if (m_eObjectList[iii] == _object) {
			// remove Element
			m_eObjectList[iii] = NULL;
			m_eObjectList.erase(iii);
			informOneObjectIsRemoved(_object);
			return;
		}
	}
	// check if the object has not been auto removed ... or remove in defered time ...
	for (int32_t iii=0; iii<m_eObjectAutoRemoveList.size(); iii++) {
		if(    m_eObjectAutoRemoveList[iii] != NULL
		    && m_eObjectAutoRemoveList[iii] == _object) {
			return;
		}
	}
	// in this case, we have an error ...
	EWOL_ERROR("Try to remove EObject that is not referenced ...");
}

void ewol::EObjectManager::autoRemove(ewol::EObject* _object) {
	if (NULL == _object) {
		EWOL_ERROR("Try to Auto-Remove (NULL) EObject");
		return;
	}
	for (int32_t iii=0; iii<m_eObjectList.size(); iii++) {
		if (m_eObjectList[iii] == _object) {
			// remove Element
			m_eObjectList[iii] = NULL;
			m_eObjectList.erase(iii);
			EWOL_DEBUG("Auto-Remove EObject : [" << _object->getId() << "] type=\"" << _object->getObjectType() << "\"");
			informOneObjectIsRemoved(_object);
			m_eObjectAutoRemoveList.push_back(_object);
			ewol::getContext().forceRedrawAll();
			return;
		}
	}
	EWOL_ERROR("Try to Auto-Remove EObject that is not referenced ...");
}

// clean all EObject that request an autoRemove ...
void ewol::EObjectManager::removeAllAutoRemove(void) {
	//EWOL_DEBUG("Auto-Remove EObject section : " << m_eObjectAutoRemoveList.size() << " elemeents");
	while(0<m_eObjectAutoRemoveList.size()) {
		if (m_eObjectAutoRemoveList[0]!=NULL) {
			EWOL_DEBUG("Real Auto-Remove EObject type=\"" << m_eObjectAutoRemoveList[0]->getObjectType() << "\"");
			delete(m_eObjectAutoRemoveList[0]);
			m_eObjectAutoRemoveList[0] = NULL;
		} else {
			m_eObjectAutoRemoveList.erase(0);
		}
	}
	m_eObjectAutoRemoveList.clear();
}

ewol::EObject* ewol::EObjectManager::get(const etk::UString& _name) {
	if (_name == "") {
		return NULL;
	}
	for (int32_t iii=0; iii<m_eObjectList.size(); iii++) {
		if (m_eObjectList[iii] != NULL) {
			if (m_eObjectList[iii]->getName() == _name) {
				return m_eObjectList[iii];
			}
		}
	}
	return NULL;
}

