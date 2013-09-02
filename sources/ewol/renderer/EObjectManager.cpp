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
#define __class__	"EObjectManager"

ewol::EObjectManager::EObjectManager(void)
{
	EWOL_DEBUG("==> Init EObject-Manager");
	// Can create mlemory leak ... ==> but not predictable comportement otherwise ...
	m_eObjectAutoRemoveList.Clear();
	m_eObjectList.Clear();
}

ewol::EObjectManager::~EObjectManager(void)
{
	bool hasError = false;
	if (m_eObjectAutoRemoveList.Size()!=0) {
		EWOL_ERROR("Must not have anymore eObject to auto-remove !!!");
		hasError = true;
	}
	if (m_eObjectList.Size()!=0) {
		EWOL_ERROR("Must not have anymore eObject !!!");
		hasError = true;
	}
	if (true==hasError) {
		EWOL_ERROR("Check if the function UnInit has been called !!!");
	}
}

void ewol::EObjectManager::UnInit(void)
{
	EWOL_DEBUG("==> Un-Init EObject-Manager");
	RemoveAllAutoRemove();
	EWOL_INFO(" Remove missing user widget");
	int32_t iii=0;
	while(iii<m_eObjectList.Size()) {
		if (m_eObjectList[iii]!=NULL) {
			if (m_eObjectList[iii]->GetStatic() == true) {
				iii++;
			} else {
				EWOL_WARNING("Un-INIT : Remove EObject type=\"" << m_eObjectList[iii]->GetObjectType() << "\"");
				delete(m_eObjectList[iii]);
				m_eObjectList[iii] = NULL;
			}
		} else {
			m_eObjectList.Erase(iii);
		}
	}
}

void ewol::EObjectManager::Add(ewol::EObject* _object)
{
	if (NULL != _object) {
		m_eObjectList.PushBack(_object);
	} else {
		EWOL_ERROR("try to add an inexistant EObject in manager");
	}
}

int32_t ewol::EObjectManager::GetNumberObject(void)
{
	return m_eObjectList.Size() + m_eObjectAutoRemoveList.Size();
}

void ewol::EObjectManager::informOneObjectIsRemoved(ewol::EObject* _object)
{
	for (int32_t iii=0; iii<m_eObjectList.Size(); iii++) {
		if (m_eObjectList[iii] != NULL) {
			m_eObjectList[iii]->OnObjectRemove(_object);
		}
	}
	for (int32_t iii=0; iii<m_eObjectAutoRemoveList.Size(); iii++) {
		if(    m_eObjectAutoRemoveList[iii] != NULL
		    && m_eObjectAutoRemoveList[iii] != _object) {
			m_eObjectAutoRemoveList[iii]->OnObjectRemove(_object);
		}
	}
	// call input event manager to remove linked widget ...
	ewol::GetContext().OnObjectRemove(_object);
}

void ewol::EObjectManager::Rm(ewol::EObject* _object)
{
	if (NULL == _object) {
		EWOL_ERROR("Try to remove (NULL) EObject");
		return;
	}
	for (int32_t iii=0; iii<m_eObjectList.Size(); iii++) {
		if (m_eObjectList[iii] == _object) {
			// Remove Element
			m_eObjectList[iii] = NULL;
			m_eObjectList.Erase(iii);
			informOneObjectIsRemoved(_object);
			return;
		}
	}
	// check if the object has not been auto removed ... or remove in defered time ...
	for (int32_t iii=0; iii<m_eObjectAutoRemoveList.Size(); iii++) {
		if(    m_eObjectAutoRemoveList[iii] != NULL
		    && m_eObjectAutoRemoveList[iii] == _object) {
			return;
		}
	}
	// in this case, we have an error ...
	EWOL_ERROR("Try to remove EObject that is not referenced ...");
}

void ewol::EObjectManager::AutoRemove(ewol::EObject* _object)
{
	if (NULL == _object) {
		EWOL_ERROR("Try to Auto-Remove (NULL) EObject");
		return;
	}
	for (int32_t iii=0; iii<m_eObjectList.Size(); iii++) {
		if (m_eObjectList[iii] == _object) {
			// Remove Element
			m_eObjectList[iii] = NULL;
			m_eObjectList.Erase(iii);
			EWOL_DEBUG("Auto-Remove EObject : [" << _object->GetId() << "] type=\"" << _object->GetObjectType() << "\"");
			informOneObjectIsRemoved(_object);
			m_eObjectAutoRemoveList.PushBack(_object);
			ewol::GetContext().ForceRedrawAll();
			return;
		}
	}
	EWOL_ERROR("Try to Auto-Remove EObject that is not referenced ...");
}

// clean all EObject that request an autoRemove ...
void ewol::EObjectManager::RemoveAllAutoRemove(void)
{
	//EWOL_DEBUG("Auto-Remove EObject section : " << m_eObjectAutoRemoveList.Size() << " elemeents");
	while(0<m_eObjectAutoRemoveList.Size()) {
		if (m_eObjectAutoRemoveList[0]!=NULL) {
			EWOL_DEBUG("Real Auto-Remove EObject type=\"" << m_eObjectAutoRemoveList[0]->GetObjectType() << "\"");
			delete(m_eObjectAutoRemoveList[0]);
			m_eObjectAutoRemoveList[0] = NULL;
		} else {
			m_eObjectAutoRemoveList.Erase(0);
		}
	}
	m_eObjectAutoRemoveList.Clear();
}

ewol::EObject* ewol::EObjectManager::Get(const etk::UString& _name)
{
	if (_name=="") {
		return NULL;
	}
	for (int32_t iii=0; iii<m_eObjectList.Size(); iii++) {
		if (m_eObjectList[iii] != NULL) {
			if (m_eObjectList[iii]->GetName() == _name) {
				return m_eObjectList[iii];
			}
		}
	}
	return NULL;
}

