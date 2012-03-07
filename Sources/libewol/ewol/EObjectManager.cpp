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
static etk::VectorType<ewol::EObject*>   m_eObjectList;          // all widget allocated ==> all time increment ... never removed ...
static etk::VectorType<ewol::EObject*>   m_eObjectDeletedList;   // all widget allocated


void ewol::EObjectManager::Init(void)
{
	EWOL_DEBUG("==> Init EObject-Manager");
	// Can create mlemory leak ... ==> but not predictable comportement otherwise ...
	// TODO : Check if we can do sotthing better
	m_eObjectDeletedList.Clear();
	m_eObjectList.Clear();
	IsInit = true;
}

void ewol::EObjectManager::UnInit(void)
{
	EWOL_DEBUG("==> Un-Init EObject-Manager");
	// Some call to permit to remove all the needed stack of EObject
	for(int32_t iii=0; iii<128 ; iii++) {
		ewol::EObjectManager::RemoveAllMark();
	}
	EWOL_INFO(" Remove missing user widget");
	while(0<m_eObjectList.Size()) {
		if (m_eObjectList[0]!=NULL) {
			MarkToRemoved(m_eObjectList[0]);
		} else {
			m_eObjectList.Erase(0);
		}
	}
	// local acces ==> this control the mutex Lock
	ewol::EObjectManager::RemoveAllMark();
	
	IsInit = false;
}

void ewol::EObjectManager::Add(ewol::EObject* object)
{
	// TODO : Chek if not existed before ...
	if (NULL != object) {
		m_eObjectList.PushBack(object);
	} else {
		EWOL_ERROR("try to add an inexistant EObject in manager");
	}
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
			m_eObjectList.Erase(iii);
			EWOL_CRITICAL("EObject direct remove is really DANGEROUS due to the multithreading ...");
			return;
		}
	}
	for (int32_t iii=0; iii<m_eObjectDeletedList.Size(); iii++) {
		if (m_eObjectDeletedList[iii] == object) {
			// Remove Element
			m_eObjectDeletedList.Erase(iii);
			return;
		}
	}
	EWOL_ERROR("EObject already removed ...");
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


void ewol::EObjectManager::MarkToRemoved(ewol::EObject* object)
{
	if (object == NULL) {
		EWOL_WARNING("try to remove a NULL Pointer on the EObject manager");
		return;
	}
	int32_t findId = -1;
	// check if the widget is not destroy :
	for(int32_t iii=0; iii<m_eObjectList.Size(); iii++) {
		if (m_eObjectList[iii] == object) {
			findId = iii;
			break;
		}
	}
	if (-1 == findId) {
		EWOL_CRITICAL("Try to mark remove an object already removed (or not registerd [imposible case])");
		return;
	}
	m_eObjectList.Erase(findId);
	m_eObjectDeletedList.PushBack(object);
	// Informe all EObject to remove reference of this one ...
	informOneObjectIsRemoved(object);
}



void ewol::EObjectManager::RemoveAllMark(void)
{
	etk::VectorType<ewol::EObject*>   m_tmpList = m_eObjectDeletedList;
	// direct delete of the current list ...
	for(int32_t iii=0; iii<m_tmpList.Size(); iii++) {
		if (NULL != m_tmpList[iii]) {
			delete(m_tmpList[iii]);
			m_tmpList[iii] = NULL;
		}
	}
}

