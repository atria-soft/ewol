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
#include <ewol/font/FontFreeType.h>

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
static ewol::Resource* LocalKeep(etk::UString& filename)
{
	EWOL_VERBOSE("KEEP (DEFAULT) : file : \"" << filename << "\"");
	for (int32_t iii=0; iii<l_resourceList.Size(); iii++) {
		if (l_resourceList[iii] != NULL) {
			if(l_resourceList[iii]->HasName(filename)) {
				l_resourceList[iii]->Increment();
				return l_resourceList[iii];
			}
		}
	}
	// we did not find it ...
	return NULL;
}

// internal generic keeper ...
static void LocalAdd(ewol::Resource* object)
{
	EWOL_VERBOSE("Add ... find empty slot");
	for (int32_t iii=0; iii<l_resourceList.Size(); iii++) {
		if (l_resourceList[iii] == NULL) {
			l_resourceList[iii] = object;
			return;
		}
	}
	l_resourceList.PushBack(object);
}

// return the type of the resource ...
bool ewol::resource::Keep(etk::UString& filename, ewol::TexturedFont*& object)
{
	EWOL_VERBOSE("KEEP : TexturedFont : file : \"" << filename << "\"");
	object = static_cast<ewol::TexturedFont*>(LocalKeep(filename));
	if (NULL != object) {
		return true;
	}
	// need to crate a new one ...
	object = new ewol::TexturedFont(filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << filename);
		return false;
	}
	LocalAdd(object);
	return true;
}


bool ewol::resource::Keep(etk::UString& filename, ewol::Font*& object)
{
	EWOL_VERBOSE("KEEP : Font : file : \"" << filename << "\"");
	object = static_cast<ewol::Font*>(LocalKeep(filename));
	if (NULL != object) {
		return true;
	}
	// need to crate a new one ...
	object = new ewol::FontFreeType(filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << filename);
		return false;
	}
	LocalAdd(object);
	return true;
}


bool ewol::resource::Keep(etk::UString& filename, ewol::Program*& object)
{
	EWOL_VERBOSE("KEEP : Program : file : \"" << filename << "\"");
	object = static_cast<ewol::Program*>(LocalKeep(filename));
	if (NULL != object) {
		return true;
	}
	// need to crate a new one ...
	object = new ewol::Program(filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << filename);
		return false;
	}
	LocalAdd(object);
	return true;
}


bool ewol::resource::Keep(etk::UString& filename, ewol::Shader*& object)
{
	EWOL_VERBOSE("KEEP : Shader : file : \"" << filename << "\"");
	object = static_cast<ewol::Shader*>(LocalKeep(filename));
	if (NULL != object) {
		return true;
	}
	// need to crate a new one ...
	object = new ewol::Shader(filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << filename);
		return false;
	}
	LocalAdd(object);
	return true;
}


void ewol::resource::Release(ewol::Resource*& object)
{
	if (NULL == object) {
		EWOL_ERROR("Try to remove a resource that have null pointer ...");
		return;
	}
	EWOL_VERBOSE("RELEASE (default) : file : \"" << object->GetName() << "\"");
	for (int32_t iii=l_resourceList.Size()-1; iii>=0; iii--) {
		if (l_resourceList[iii] != NULL) {
			if(l_resourceList[iii] == object) {
				if (true == l_resourceList[iii]->Decrement()) {
					// delete element
					delete(l_resourceList[iii]);
					// remove element from the list :
					l_resourceList[iii] = NULL;
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


void ewol::resource::Release(ewol::TexturedFont*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	Release(object2);
	object = NULL;
}
void ewol::resource::Release(ewol::Font*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	Release(object2);
	object = NULL;
}
void ewol::resource::Release(ewol::Program*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	Release(object2);
	object = NULL;
}
void ewol::resource::Release(ewol::Shader*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	Release(object2);
	object = NULL;
}