/**
 *******************************************************************************
 * @file ewol/EObjectManager.cpp
 * @brief basic ewol Object Manager (Sources)
 * @author Edouard DUPIN
 * @date 27/02/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#include <ewol/EObjectManager.h>
#include <ewol/base/eventInputManagement.h>

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
	ewol::eventInput::OnObjectRemove(object);
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
			EWOL_DEBUG("Auto-Remove EObject : [" << object->GetId() << "]");
			informOneObjectIsRemoved(object);
			m_eObjectAutoRemoveList.PushBack(object);
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
			delete(m_eObjectAutoRemoveList[0]);
			m_eObjectAutoRemoveList[0] = NULL;
		} else {
			m_eObjectAutoRemoveList.Erase(0);
		}
	}
	m_eObjectAutoRemoveList.Clear();
}

