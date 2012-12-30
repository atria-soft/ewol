/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/eObject/EObjectManager.h>
#include <ewol/renderer/os/eSystem.h>
#include <ewol/ewol.h>

#undef __class__
#define __class__	"EObjectManager"

static bool IsInit = false;

// internal element of the widget manager : 
static etk::Vector<ewol::EObject*>   m_eObjectList;             // all widget allocated ==> all time increment ... never removed ...
static etk::Vector<ewol::EObject*>   m_eObjectAutoRemoveList;   // all widget allocated


void ewol::EObjectManager::Init(void)
{
	EWOL_DEBUG("==> Init EObject-Manager");
	// Can create mlemory leak ... ==> but not predictable comportement otherwise ...
	m_eObjectAutoRemoveList.Clear();
	m_eObjectList.Clear();
	IsInit = true;
}

void ewol::EObjectManager::UnInit(void)
{
	EWOL_DEBUG("==> Un-Init EObject-Manager");
	RemoveAllAutoRemove();
	EWOL_INFO(" Remove missing user widget");
	while(0<m_eObjectList.Size()) {
		if (m_eObjectList[0]!=NULL) {
			EWOL_WARNING("Un-INIT : Remove EObject type=\"" << m_eObjectList[0]->GetObjectType() << "\"");
			delete(m_eObjectList[0]);
			m_eObjectList[0] = NULL;
		} else {
			m_eObjectList.Erase(0);
		}
	}
	
	IsInit = false;
}

void ewol::EObjectManager::Add(ewol::EObject* object)
{
	if (NULL != object) {
		m_eObjectList.PushBack(object);
	} else {
		EWOL_ERROR("try to add an inexistant EObject in manager");
	}
}

void informOneObjectIsRemoved(ewol::EObject* object)
{
	for (int32_t iii=0; iii<m_eObjectList.Size(); iii++) {
		if (m_eObjectList[iii] != NULL) {
			m_eObjectList[iii]->OnObjectRemove(object);
		}
	}
	// call input event manager to remove linked widget ...
	eSystem::OnObjectRemove(object);
}

void ewol::EObjectManager::Rm(ewol::EObject* object)
{
	if (NULL == object) {
		EWOL_ERROR("Try to remove (NULL) EObject");
		return;
	}
	for (int32_t iii=0; iii<m_eObjectList.Size(); iii++) {
		if (m_eObjectList[iii] == object) {
			// Remove Element
			m_eObjectList[iii] = NULL;
			m_eObjectList.Erase(iii);
			informOneObjectIsRemoved(object);
			return;
		}
	}
	EWOL_ERROR("Try to remove EObject that is not referenced ...");
}

void ewol::EObjectManager::AutoRemove(ewol::EObject* object)
{
	if (NULL == object) {
		EWOL_ERROR("Try to Auto-Remove (NULL) EObject");
		return;
	}
	for (int32_t iii=0; iii<m_eObjectList.Size(); iii++) {
		if (m_eObjectList[iii] == object) {
			// Remove Element
			m_eObjectList[iii] = NULL;
			m_eObjectList.Erase(iii);
			EWOL_DEBUG("Auto-Remove EObject : [" << object->GetId() << "] type=\"" << object->GetObjectType() << "\"");
			informOneObjectIsRemoved(object);
			m_eObjectAutoRemoveList.PushBack(object);
			ewol::ForceRedrawAll();
			return;
		}
	}
	EWOL_ERROR("Try to Auto-Remove EObject that is not referenced ...");
}

// clean all EObject that request an autoRemove ...
void ewol::EObjectManager::RemoveAllAutoRemove(void)
{
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
