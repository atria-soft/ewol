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
	// Can create mlemory leak ...  == > but not predictable comportement otherwise ...
	m_eObjectAutoRemoveList.clear();
	m_eObjectList.clear();
}

ewol::object::Manager::~Manager() {
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

void ewol::object::Manager::unInit() {
	EWOL_DEBUG(" == > Un-Init Object-Manager");
	removeAllAutoRemove();
	EWOL_INFO(" remove missing user widget");
	size_t iii=0;
	while(iii < m_eObjectList.size()) {
		if (m_eObjectList[iii] != NULL) {
			if (    m_eObjectList[iii]->getStatic() == true
			     || m_eObjectList[iii]->getStatusResource() == true) {
				iii++;
			} else {
				EWOL_WARNING("Un-INIT : remove Object type=\"" << m_eObjectList[iii]->getObjectType() << "\"");
				m_eObjectList[iii].reset();
				m_eObjectList[iii] = NULL;
			}
		} else {
			m_eObjectList.erase(m_eObjectList.begin()+iii);
		}
	}
	removeAllAutoRemove();
	EWOL_INFO(" remove resources user widgets");
	while(iii < m_eObjectList.size()) {
		if (m_eObjectList[iii] != NULL) {
			if (m_eObjectList[iii]->getStatic() == true) {
				iii++;
			} else {
				EWOL_WARNING("Un-INIT : remove Object type=\"" << m_eObjectList[iii]->getObjectType() << "\"");
				m_eObjectList[iii].reset();
			}
		} else {
			m_eObjectList.erase(m_eObjectList.begin()+iii);
		}
	}
	removeAllAutoRemove();
	EWOL_INFO(" remove static user widgets");
	while(iii < m_eObjectList.size()) {
		if (m_eObjectList[iii] != NULL) {
			EWOL_WARNING("Un-INIT : remove Object type=\"" << m_eObjectList[iii]->getObjectType() << "\"");
			m_eObjectList[iii].reset();
		} else {
			m_eObjectList.erase(m_eObjectList.begin()+iii);
		}
	}
}

void ewol::object::Manager::add(ewol::object::Shared<ewol::Object> _object) {
	if (_object == NULL) {
		EWOL_ERROR("try to add an inexistant Object in manager");
	}
	// ! < it might benerate a shared object !!!
	m_eObjectList.push_back(_object);
}

int32_t ewol::object::Manager::getNumberObject() {
	return m_eObjectList.size() + m_eObjectAutoRemoveList.size();
}

void ewol::object::Manager::informOneObjectIsRemoved(ewol::object::Shared<ewol::Object> _object) {
	size_t mbElement = m_eObjectList.size();
	for (int64_t iii=0; iii<(int64_t)m_eObjectList.size(); ++iii) {
		if (    m_eObjectList[iii] != NULL
		     && m_eObjectList[iii] != _object) {
			//EWOL_DEBUG("inform " << iii+1 << "/" << m_eObjectList.size());
			//EWOL_DEBUG("    id=" << m_eObjectList[iii]->getId() << " named '" << m_eObjectList[iii]->getName() << "' type=" << m_eObjectList[iii]->getObjectType());
			m_eObjectList[iii]->onObjectRemove(_object);
			if (mbElement != m_eObjectList.size()) {
				iii = -1;
				mbElement = m_eObjectList.size();
			}
		}
	}
	//EWOL_DEBUG("inform active done");
	mbElement = m_eObjectAutoRemoveList.size();
	for (size_t iii=0; iii<m_eObjectAutoRemoveList.size(); iii++) {
		if(    m_eObjectAutoRemoveList[iii] != NULL
		    && m_eObjectAutoRemoveList[iii] != _object) {
			//EWOL_DEBUG("inform2 " << iii+1 << "/" << m_eObjectAutoRemoveList.size());
			m_eObjectAutoRemoveList[iii]->onObjectRemove(_object);
			if (mbElement != m_eObjectAutoRemoveList.size()) {
				iii = -1;
				mbElement = m_eObjectAutoRemoveList.size();
			}
		}
	}
	//EWOL_DEBUG("inform in-active done");
	// call input event manager to remove linked widget ...
	ewol::getContext().onObjectRemove(_object);
}

void ewol::object::Manager::rm(ewol::object::Shared<ewol::Object> _object) {
	if (_object == NULL) {
		EWOL_ERROR("Try to remove (NULL) Object");
		return;
	}
	for (size_t iii=0; iii<m_eObjectList.size(); iii++) {
		if (m_eObjectList[iii] == _object) {
			// remove Element
			m_eObjectList[iii] = NULL;
			m_eObjectList.erase(m_eObjectList.begin()+iii);
			informOneObjectIsRemoved(_object);
			return;
		}
	}
	// check if the object has not been auto removed ... or remove in defered time ...
	for (size_t iii=0; iii<m_eObjectAutoRemoveList.size(); iii++) {
		if(    m_eObjectAutoRemoveList[iii] != NULL
		    && m_eObjectAutoRemoveList[iii] == _object) {
			return;
		}
	}
	// in this case, we have an error ...
	EWOL_ERROR("Try to remove Object that is not referenced ...");
}

void ewol::object::Manager::autoRemove(ewol::object::Shared<ewol::Object> _object) {
	if (NULL == _object) {
		EWOL_ERROR("Try to Auto-Remove (NULL) Object");
		return;
	}
	for (size_t iii=0; iii<m_eObjectList.size(); iii++) {
		if (m_eObjectList[iii] == _object) {
			// remove Element
			m_eObjectList[iii] = NULL;
			m_eObjectList.erase(m_eObjectList.begin()+iii);
			EWOL_DEBUG("Auto-Remove Object : [" << _object->getId() << "] type='" << _object->getObjectType() << "'");
			if (_object->getStatusResource() == false) {
				informOneObjectIsRemoved(_object);
			}
			m_eObjectAutoRemoveList.push_back(_object);
			ewol::getContext().forceRedrawAll();
			EWOL_DEBUG("Auto-Remove Object ... done");
			return;
		}
	}
	EWOL_ERROR("Try to Auto-Remove Object that is not referenced ...");
}

// clean all Object that request an autoRemove ...
void ewol::object::Manager::removeAllAutoRemove() {
	//EWOL_DEBUG("Auto-Remove Object section : " << m_eObjectAutoRemoveList.size() << " elemeents");
	while(0<m_eObjectAutoRemoveList.size()) {
		if (m_eObjectAutoRemoveList[0] != NULL) {
			EWOL_DEBUG("Real Auto-Remove Object [" << m_eObjectAutoRemoveList[0]->getId() << "]type='" << m_eObjectAutoRemoveList[0]->getObjectType() << "'");
		}
		m_eObjectAutoRemoveList.erase(m_eObjectAutoRemoveList.begin());
	}
	m_eObjectAutoRemoveList.clear();
}

ewol::object::Shared<ewol::Object> ewol::object::Manager::get(const std::string& _name) {
	if (_name == "") {
		return NULL;
	}
	for (size_t iii=0; iii<m_eObjectList.size(); iii++) {
		if (m_eObjectList[iii] != NULL) {
			if (m_eObjectList[iii]->getName() == _name) {
				return m_eObjectList[iii].get();
			}
		}
	}
	return NULL;
}

