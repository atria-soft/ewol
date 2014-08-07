/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/resource/Manager.h>
#include <ewol/resource/FontFreeType.h>
#include <ewol/ewol.h>
#include <ewol/openGL/openGL.h>
#include <ewol/context/Context.h>



ewol::resource::Manager::Manager() :
  m_contextHasBeenRemoved(true) {
	// nothing to do ...
}

ewol::resource::Manager::~Manager() {
	bool hasError = false;
	if (m_resourceListToUpdate.size()!=0) {
		EWOL_ERROR("Must not have anymore resources to update !!!");
		hasError = true;
	}
	// TODO : Remove unneeded elements
	if (m_resourceList.size()!=0) {
		EWOL_ERROR("Must not have anymore resources !!!");
		hasError = true;
	}
	if (true == hasError) {
		EWOL_ERROR("Check if the function UnInit has been called !!!");
	}
}

void ewol::resource::Manager::unInit() {
	display();
	m_resourceListToUpdate.clear();
	// remove all resources ...
	auto it(m_resourceList.begin());
	while(it != m_resourceList.end()) {
		std::shared_ptr<ewol::Resource> tmpRessource = (*it).lock();
		if (tmpRessource != nullptr) {
			EWOL_WARNING("Find a resource that is not removed : [" << tmpRessource->getId() << "]"
			             << "=\"" << tmpRessource->getName() << "\" "
			             << tmpRessource.use_count() << " elements");
		}
		m_resourceList.erase(it);
		it = m_resourceList.begin();
	}
	m_resourceList.clear();
}

void ewol::resource::Manager::display() {
	EWOL_INFO("Resources loaded : ");
	// remove all resources ...
	for (auto &it : m_resourceList) {
		std::shared_ptr<ewol::Resource> tmpRessource = it.lock();
		if (tmpRessource != nullptr) {
			EWOL_INFO("    [" << tmpRessource->getId() << "]"
			          << tmpRessource->getObjectType()
			          << "=\"" << tmpRessource->getName() << "\" "
			          << tmpRessource.use_count() << " elements");
		}
	}
	EWOL_INFO("Resources ---");
}

void ewol::resource::Manager::reLoadResources() {
	EWOL_INFO("-------------  Resources re-loaded  -------------");
	// remove all resources ...
	if (m_resourceList.size() != 0) {
		for (size_t jjj=0; jjj<MAX_RESOURCE_LEVEL; jjj++) {
			EWOL_INFO("    Reload level : " << jjj << "/" << (MAX_RESOURCE_LEVEL-1));
			for (auto &it : m_resourceList) {
				std::shared_ptr<ewol::Resource> tmpRessource = it.lock();
				if(tmpRessource != nullptr) {
					if (jjj == tmpRessource->getResourceLevel()) {
						tmpRessource->reload();
						EWOL_INFO("        [" << tmpRessource->getId() << "]="<< tmpRessource->getObjectType());
					}
				}
			}
		}
	}
	// TODO : UNderstand why it is set here ...
	//ewol::requestUpdateSize();
	EWOL_INFO("-------------  Resources  -------------");
}

void ewol::resource::Manager::update(const std::shared_ptr<ewol::Resource>& _object) {
	// chek if not added before
	for (auto &it : m_resourceListToUpdate) {
		if (    it != nullptr
		     && it == _object) {
			// just prevent some double add ...
			return;
		}
	}
	// add it ...
	m_resourceListToUpdate.push_back(_object);
}

// Specific to load or update the data in the openGl context  == > system use only
void ewol::resource::Manager::updateContext() {
	if (m_contextHasBeenRemoved == true) {
		// need to update all ...
		m_contextHasBeenRemoved = false;
		if (m_resourceList.size() != 0) {
			for (size_t jjj=0; jjj<MAX_RESOURCE_LEVEL; jjj++) {
				EWOL_INFO("    updateContext level (D) : " << jjj << "/" << (MAX_RESOURCE_LEVEL-1));
				for (auto &it : m_resourceList) {
					std::shared_ptr<ewol::Resource> tmpRessource = it.lock();
					if(    tmpRessource != nullptr
					    && jjj == tmpRessource->getResourceLevel()) {
						//EWOL_DEBUG("Update context named : " << l_resourceList[iii]->getName());
						tmpRessource->updateContext();
					}
				}
			}
		}
	} else {
		if (m_resourceListToUpdate.size() != 0) {
			for (size_t jjj=0; jjj<MAX_RESOURCE_LEVEL; jjj++) {
				EWOL_INFO("    updateContext level (U) : " << jjj << "/" << (MAX_RESOURCE_LEVEL-1));
				for (auto &it : m_resourceListToUpdate) {
					if (    it != nullptr
					     && jjj == it->getResourceLevel()) {
						it->updateContext();
					}
				}
			}
		}
	}
	// Clean the update list
	m_resourceListToUpdate.clear();
}

// in this case, it is really too late ...
void ewol::resource::Manager::contextHasBeenDestroyed() {
	for (auto &it : m_resourceList) {
		std::shared_ptr<ewol::Resource> tmpRessource = it.lock();
		if (tmpRessource != nullptr) {
			tmpRessource->removeContextToLate();
		}
	}
	// no context preent ...
	m_contextHasBeenRemoved = true;
}

// internal generic keeper ...
std::shared_ptr<ewol::Resource> ewol::resource::Manager::localKeep(const std::string& _filename) {
	EWOL_VERBOSE("KEEP (DEFAULT) : file : \"" << _filename << "\"");
	for (auto &it : m_resourceList) {
		std::shared_ptr<ewol::Resource> tmpRessource = it.lock();
		if (    tmpRessource != nullptr
		     && tmpRessource->getName() == _filename) {
			return tmpRessource;
		}
	}
	// we did not find it ...
	return nullptr;
}

// internal generic keeper ...
void ewol::resource::Manager::localAdd(const std::shared_ptr<ewol::Resource>& _object) {
	//Add ... find empty slot
	for (auto &it : m_resourceList) {
		std::shared_ptr<ewol::Resource> tmpRessource = it.lock();
		if (tmpRessource == nullptr) {
			it = _object;
			return;
		}
	}
	// add at the end if no slot is free
	m_resourceList.push_back(_object);
}

// in case of error ...
bool ewol::resource::Manager::checkResourceToRemove() {
	//EWOL_INFO("remove object in Manager");
	updateContext();
	for (auto it(m_resourceList.begin()); it!=m_resourceList.end(); ++it) {
		if ((*it).lock() == nullptr) {
			m_resourceList.erase(it);
			it = m_resourceList.begin();
			continue;
		}
		m_resourceList.erase(it);
		it = m_resourceList.begin();
		return true;
	}
	return false;
}
