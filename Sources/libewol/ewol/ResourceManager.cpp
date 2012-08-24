/**
 *******************************************************************************
 * @file ewol/ResourcesManager.h
 * @brief ewol resources manager template (header)
 * @author Edouard DUPIN
 * @date 24/08/2012
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


#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/ResourceManager.h>

static etk::Vector<ewol::Resource*> l_resourceList;


void ewol::resource::Init(void)
{
	// nothing to do in theory then, we clean the buffers :
	// NOTE : If we do domething here, then the system does not work corectly
	if (l_resourceList.Size() != 0) {
		EWOL_CRITICAL("Start with a resource manager Not empty, number of resources loaded : " << l_resourceList.Size());
	}
	l_resourceList.Clear();
}

void ewol::resource::UnInit(void)
{
	// remove textured font ...
	for (int32_t iii=0; iii<l_resourceList.Size(); iii++) {
		if (l_resourceList[iii] != NULL) {
			delete(l_resourceList[iii]);
			l_resourceList[iii] = NULL;
		}
	}
	l_resourceList.Clear();
}

// internal generic keeper ...
static ewol::Resource* Keep(etk::UString name)
{
	for (int32_t iii=l_resourceList.Size()-1; iii>=0; iii--) {
		if (l_resourceList[iii] != NULL) {
			if(l_resourceList[iii]->HasName(name)) {
				l_resourceList[iii]->Increment();
				return l_resourceList[iii];
			}
		}
	}
	// we did not find it ...
	return NULL;
}


// return the type of the resource ...
bool ewol::resource::Keep(etk::UString& filename, ewol::TexturedFont*& object, int32_t size)
{
	object = NULL;
	for (int32_t iii=l_resourceList.Size()-1; iii>=0; iii--) {
		if (l_resourceList[iii] != NULL) {
			if(l_resourceList[iii]->HasName(name)) {
				ewol::TexturedFont* tmpObject = static_cast<ewol::TexturedFont*>(l_resourceList[iii]);
				if (NULL!=tmpObject) {
					if (tmpObject->getFontSize() == size) {
						l_resourceList[iii]->Increment();
						object = static_cast<ewol::TexturedFont*>l_resourceList[iii]
						return false;
					}
				}
				// good name but not good Size
			}
		}
	}
	// need to crate a new one ...
	object = new ewol::TexturedFont(filename, size);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << filename);
		return false;
	}
	l_resourceList.PushBack(object);
	return true;
}


bool ewol::resource::Keep(etk::UString& filename, ewol::Font*& object)
{
	object = static_cast<ewol::Font*>(Keep(filename));
	if (NULL != object) {
		return true;
	}
	// need to crate a new one ...
	object = new ewol::Font(filename, size);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << filename);
		return false;
	}
	l_resourceList.PushBack(object);
	return true;
}


bool ewol::resource::Keep(etk::UString& filename, ewol::Program*& object)
{
	object = static_cast<ewol::Program*>(Keep(filename));
	if (NULL != object) {
		return true;
	}
	// need to crate a new one ...
	object = new ewol::Program(filename, size);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << filename);
		return false;
	}
	l_resourceList.PushBack(object);
	return true;
}


bool ewol::resource::Keep(etk::UString& filename, ewol::Shader*& object)
{
	object = static_cast<ewol::Shader*>(Keep(filename));
	if (NULL != object) {
		return true;
	}
	// need to crate a new one ...
	object = new ewol::Shader(filename, size);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << filename);
		return false;
	}
	l_resourceList.PushBack(object);
	return true;
}


void ewol::resource::Release(ewol::Resource*& object)
{
	if (NUUL == object) {
		EWOL_ERROR("Try to remove a resource that have null pointer ...");
		return;
	}
	for (int32_t iii=l_resourceList.Size()-1; iii>=0; iii--) {
		if (l_resourceList[iii] != NULL) {
			if(l_resourceList[iii] == object) {
				if (true == l_resourceList[iii]->Decrement()) {
					// delete element
					delete(l_resourceList[iii]);
					// remove element from the list :
					l_resourceList.Erase(iii);
				}
				// insidiously remove the pointer for the caller ...
				object = NULL;
				return;
			}
		}
	}
	EWOL_ERROR("Can not find the resources in the list : " << (int64_t)object);
	// insidiously remove the pointer for the caller ...
	object = NULL;
}

