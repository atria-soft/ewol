/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/resources/ResourceManager.h>
#include <ewol/resources/FontFreeType.h>
#include <ewol/ewol.h>
#include <ewol/renderer/openGL.h>
#include <ewol/renderer/eContext.h>



ewol::ResourceManager::ResourceManager(void) :
	m_contextHasBeenRemoved(true)
{
	// nothing to do ...
}

ewol::ResourceManager::~ResourceManager(void)
{
	bool hasError = false;
	if (m_resourceListToUpdate.Size()!=0) {
		EWOL_ERROR("Must not have anymore resources to update !!!");
		hasError = true;
	}
	if (m_resourceList.Size()!=0) {
		EWOL_ERROR("Must not have anymore resources !!!");
		hasError = true;
	}
	if (true==hasError) {
		EWOL_ERROR("Check if the function UnInit has been called !!!");
	}
}
void ewol::ResourceManager::UnInit(void)
{
	Display();
	m_resourceListToUpdate.Clear();
	// remove all resources ...
	for (int32_t iii=m_resourceList.Size()-1; iii>=0; iii--) {
		if (m_resourceList[iii] != NULL) {
			EWOL_WARNING("Find a resource that is not removed : [" << m_resourceList[iii]->GetUID() << "]"
			             << "=\"" << m_resourceList[iii]->GetName() << "\" "
			             << m_resourceList[iii]->GetCounter() << " elements");
			delete(m_resourceList[iii]);
			m_resourceList[iii] = NULL;
		}
	}
	m_resourceList.Clear();
}


void ewol::ResourceManager::Display(void)
{
	EWOL_INFO("Resources loaded : ");
	// remove all resources ...
	for (int32_t iii=m_resourceList.Size()-1; iii>=0; iii--) {
		if (m_resourceList[iii] != NULL) {
			EWOL_INFO("    [" << m_resourceList[iii]->GetUID() << "]"
			          << m_resourceList[iii]->GetType()
			          << "=\"" << m_resourceList[iii]->GetName() << "\" "
			          << m_resourceList[iii]->GetCounter() << " elements");
		}
	}
	EWOL_INFO("Resources ---");
}

void ewol::ResourceManager::ReLoadResources(void)
{
	EWOL_INFO("-------------  Resources re-loaded  -------------");
	// remove all resources ...
	if (m_resourceList.Size() != 0) {
		for (int32_t jjj=0; jjj<MAX_RESOURCE_LEVEL; jjj++) {
			EWOL_INFO("    Reload level : " << jjj << "/" << (MAX_RESOURCE_LEVEL-1));
			for (int32_t iii=m_resourceList.Size()-1; iii>=0; iii--) {
				if(m_resourceList[iii] != NULL) {
					if (jjj==m_resourceList[iii]->GetResourceLevel()) {
						m_resourceList[iii]->Reload();
						EWOL_INFO("        [" << m_resourceList[iii]->GetUID() << "]="<< m_resourceList[iii]->GetType());
					}
				}
			}
		}
	}
	// TODO : UNderstand why it is set here ...
	//ewol::RequestUpdateSize();
	EWOL_INFO("-------------  Resources  -------------");
}

void ewol::ResourceManager::Update(ewol::Resource* _object)
{
	// chek if not added before
	for (int32_t iii=0; iii<m_resourceListToUpdate.Size(); iii++) {
		if (m_resourceListToUpdate[iii] != NULL) {
			if (m_resourceListToUpdate[iii] == _object) {
				// just prevent some double add ...
				return;
			}
		}
	}
	// add it ...
	m_resourceListToUpdate.PushBack(_object);
}

// Specific to load or update the data in the openGl context ==> system use only
void ewol::ResourceManager::UpdateContext(void)
{
	if (true == m_contextHasBeenRemoved) {
		// need to update all ...
		m_contextHasBeenRemoved = false;
		if (m_resourceList.Size() != 0) {
			for (int32_t jjj=0; jjj<MAX_RESOURCE_LEVEL; jjj++) {
				EWOL_INFO("    UpdateContext level (D) : " << jjj << "/" << (MAX_RESOURCE_LEVEL-1));
				for (int32_t iii=0; iii<m_resourceList.Size(); iii++) {
					if(m_resourceList[iii] != NULL) {
						if (jjj==m_resourceList[iii]->GetResourceLevel()) {
							//EWOL_DEBUG("Update context of " << iii << " named : " << l_resourceList[iii]->GetName());
							m_resourceList[iii]->UpdateContext();
						}
					}
				}
			}
		}
	}else {
		if (m_resourceListToUpdate.Size() != 0) {
			for (int32_t jjj=0; jjj<MAX_RESOURCE_LEVEL; jjj++) {
				EWOL_INFO("    UpdateContext level (U) : " << jjj << "/" << (MAX_RESOURCE_LEVEL-1));
				for (int32_t iii=0; iii<m_resourceListToUpdate.Size(); iii++) {
					if(m_resourceListToUpdate[iii] != NULL) {
						if (jjj==m_resourceListToUpdate[iii]->GetResourceLevel()) {
							m_resourceListToUpdate[iii]->UpdateContext();
						}
					}
				}
			}
		}
	}
	// Clean the update list
	m_resourceListToUpdate.Clear();
}

// in this case, it is really too late ...
void ewol::ResourceManager::ContextHasBeenDestroyed(void)
{
	for (int32_t iii=0; iii<m_resourceList.Size(); iii++) {
		if (m_resourceList[iii] != NULL) {
			m_resourceList[iii]->RemoveContextToLate();
		}
	}
	// no context preent ...
	m_contextHasBeenRemoved = true;
}





// internal generic keeper ...
ewol::Resource* ewol::ResourceManager::LocalKeep(const etk::UString& _filename)
{
	EWOL_VERBOSE("KEEP (DEFAULT) : file : \"" << _filename << "\"");
	for (int32_t iii=0; iii<m_resourceList.Size(); iii++) {
		if (m_resourceList[iii] != NULL) {
			if(m_resourceList[iii]->HasName(_filename)) {
				m_resourceList[iii]->Increment();
				return m_resourceList[iii];
			}
		}
	}
	// we did not find it ...
	return NULL;
}

// internal generic keeper ...
void ewol::ResourceManager::LocalAdd(ewol::Resource* _object)
{
	//Add ... find empty slot
	for (int32_t iii=0; iii<m_resourceList.Size(); iii++) {
		if (m_resourceList[iii] == NULL) {
			m_resourceList[iii] = _object;
			return;
		}
	}
	// add at the end if no slot is free
	m_resourceList.PushBack(_object);
}

void ewol::ResourceManager::Release(ewol::Resource*& _object)
{
	if (NULL == _object) {
		EWOL_ERROR("Try to remove a resource that have null pointer ...");
		return;
	}
	for (int32_t iii=0; iii<m_resourceListToUpdate.Size(); iii++) {
		if (m_resourceListToUpdate[iii] == _object) {
			m_resourceListToUpdate[iii] = NULL;
			//l_resourceListToUpdate.Erase(iii);
		}
	}
	EWOL_VERBOSE("RELEASE (default) : file : \"" << _object->GetName() << "\"");
	for (int32_t iii=m_resourceList.Size()-1; iii>=0; iii--) {
		if (m_resourceList[iii] != NULL) {
			if(m_resourceList[iii] == _object) {
				if (true == m_resourceList[iii]->Decrement()) {
					// delete element
					delete(m_resourceList[iii]);
					// remove element from the list :
					m_resourceList[iii] = NULL;
				}
				// insidiously remove the pointer for the caller ...
				_object = NULL;
				return;
			}
		}
	}
	EWOL_ERROR("Can not find the resources in the list : " << (int64_t)_object);
	// insidiously remove the pointer for the caller ...
	_object = NULL;
}
