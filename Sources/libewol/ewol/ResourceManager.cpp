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
static etk::Vector<ewol::Resource*> l_resourceListToUpdate;
static bool                         l_contextHasBeenRemoved = true;

void ewol::resource::Init(void)
{
	// nothing to do in theory then, we clean the buffers :
	// NOTE : If we do domething here, then the system does not work corectly
	if (l_resourceList.Size() != 0) {
		EWOL_CRITICAL("Start with a resource manager Not empty, number of resources loaded : " << l_resourceList.Size());
	}
	l_resourceListToUpdate.Clear();
	l_resourceList.Clear();
	l_contextHasBeenRemoved = true;
}

void ewol::resource::UnInit(void)
{
	l_resourceListToUpdate.Clear();
	// remove all resources ...
	for (int32_t iii=0; iii<l_resourceList.Size(); iii++) {
		if (l_resourceList[iii] != NULL) {
			EWOL_WARNING("Find a resource that is not removed : [" << l_resourceList[iii]->GetUID() << "]");
			delete(l_resourceList[iii]);
			l_resourceList[iii] = NULL;
		}
	}
	l_resourceList.Clear();
}


void ewol::resource::Update(ewol::Resource* object)
{
	// chek if not added before
	for (int32_t iii=0; iii<l_resourceListToUpdate.Size(); iii++) {
		if (l_resourceListToUpdate[iii] != NULL) {
			if (l_resourceListToUpdate[iii] == object) {
				// just prevent some double add ...
				return;
			}
		}
	}
	// add it ...
	l_resourceListToUpdate.PushBack(object);
}

// Specific to load or update the data in the openGl context ==> system use only
void ewol::resource::UpdateContext(void)
{
	if (true == l_contextHasBeenRemoved) {
		// need to update all ...
		l_contextHasBeenRemoved = false;
		for (int32_t iii=0; iii<l_resourceList.Size(); iii++) {
			if (l_resourceList[iii] != NULL) {
				l_resourceList[iii]->UpdateContext();
			}
		}
	}else {
		for (int32_t iii=0; iii<l_resourceListToUpdate.Size(); iii++) {
			if (l_resourceListToUpdate[iii] != NULL) {
				l_resourceListToUpdate[iii]->UpdateContext();
			}
		}
	}
	// Clean the update list
	l_resourceListToUpdate.Clear();
}

// in this case, it is really too late ...
void ewol::resource::ContextHasBeenDestroyed(void)
{
	for (int32_t iii=0; iii<l_resourceList.Size(); iii++) {
		if (l_resourceList[iii] != NULL) {
			l_resourceList[iii]->RemoveContextToLate();
		}
	}
	// no context preent ...
	l_contextHasBeenRemoved = true;
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
	//Add ... find empty slot
	for (int32_t iii=0; iii<l_resourceList.Size(); iii++) {
		if (l_resourceList[iii] == NULL) {
			l_resourceList[iii] = object;
			return;
		}
	}
	// add at the end if no slot is free
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

bool ewol::resource::Keep(ewol::Texture*& object)
{
	// this element create a new one every time ....
	object = new ewol::Texture("");
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : ??TEX??");
		return false;
	}
	LocalAdd(object);
	return true;
}

bool ewol::resource::Keep(etk::UString& filename, ewol::TextureFile*& object, Vector2D<int32_t> size)
{
	etk::UString TmpFilename = filename;
	TmpFilename += ":";
	TmpFilename += size.x;
	TmpFilename += "x";
	TmpFilename += size.y;
	
	EWOL_VERBOSE("KEEP : TectureFile : file : \"" << TmpFilename << "\"");
	object = static_cast<ewol::TextureFile*>(LocalKeep(TmpFilename));
	if (NULL != object) {
		return true;
	}
	// need to crate a new one ...
	object = new ewol::TextureFile(TmpFilename, filename, size);
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
	for (int32_t iii=0; iii<l_resourceListToUpdate.Size(); iii++) {
		if (l_resourceListToUpdate[iii] == object) {
			l_resourceListToUpdate[iii] = NULL;
			l_resourceListToUpdate.Erase(iii);
		}
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
void ewol::resource::Release(ewol::Texture*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	Release(object2);
	object = NULL;
}
void ewol::resource::Release(ewol::TextureFile*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	Release(object2);
	object = NULL;
}