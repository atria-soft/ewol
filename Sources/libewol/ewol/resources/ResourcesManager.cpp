/**
 *******************************************************************************
 * @file ewol/resources/ResourcesManager.cpp
 * @brief ewol Resources manager system (Sources)
 * @author Edouard DUPIN
 * @date 21/08/2012
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
#include <etk/File.h>
#include <etk/Vector.h>
#include <ewol/resources/ResourcesManager.h>

static etk::Vector<ewol::ResourcesImage *>  l_imageList;

static uint32_t l_uniqueIdResources = 0;

void ewol::resourcesManager::Init(void)
{
	// nothing to do in théory then, we clean the buffers :
	// NOTE : If we do domething here, then the system does not work corectly
	l_imageList.Clear();
	l_uniqueIdResources = 0;
}

void ewol::resourcesManager::UnInit(void)
{
	for (int32_t iii=0; iii<l_imageList.Size(); iii++) {
		if (l_imageList[iii] != NULL) {
			delete(l_imageList[iii]);
			l_imageList[iii] = NULL;
		}
	}
	l_imageList.Clear();
}

// note : Return the UniqueID ...
uint32_t ewol::resourcesManager::Add(ewol::ResourcesImage *object)
{
	if (object==NULL) {
		EWOL_CRITICAL("try to add an Image Resources with NULL pointer ...");
		return 0;
	}
	for (int32_t iii=0; iii<l_imageList.Size(); iii++) {
		if (l_imageList[iii] != NULL) {
			if (l_imageList[iii] == object) {
				EWOL_CRITICAL("try to add a Image resources a second time ...");
				return 0;
			}
		}
	}
	l_uniqueIdResources ++;
	// add it in the list
	l_imageList.PushBack(object);
	// return his ID ...
	return l_uniqueIdResources;
}

void ewol::resourcesManager::Rm(ewol::ResourcesImage *object)
{
	if (object==NULL) {
		EWOL_CRITICAL("try to remove a texture with NULL pointer ...");
		return;
	}
	for (int32_t iii=l_imageList.Size()-1; iii>=0; iii--) {
		if (l_imageList[iii] != NULL) {
			if (l_imageList[iii] == object) {
				// we find the texture :
				l_imageList.Erase(iii);
				return;
			}
		}
	}
	EWOL_CRITICAL("Try to remove an Image resources that is not present in the resources pool");
}


ewol::ResourcesImage* ewol::resourcesManager::ImageKeep(etk::UString fileName, Vector2D<int32_t> size)
{
	for (int32_t iii=l_imageList.Size()-1; iii>=0; iii--) {
		if (l_imageList[iii] != NULL) {
			if(    l_imageList[iii]->HasName(fileName)
			    && l_imageList[iii]->Get().GetSize() == size) {
				l_imageList[iii]->Increment();
				return l_imageList[iii];
			}
		}
	}
	ewol::ResourcesImage* tmpResources = new ewol::ResourcesImage(fileName, size);
	if (NULL == tmpResources) {
		EWOL_ERROR("allocation error of a resource Image : " << fileName);
		return NULL;
	}
	l_imageList.PushBack(tmpResources);
	return tmpResources;
}

void ewol::resourcesManager::ImageRelease(ewol::ResourcesImage* object)
{
	for (int32_t iii=l_imageList.Size()-1; iii>=0; iii--) {
		if (l_imageList[iii] != NULL) {
			if(l_imageList[iii] == object) {
				if (true == l_imageList[iii]->Decrement()) {
					delete(l_imageList[iii]);
					return;
				}
			}
		}
	}
}

