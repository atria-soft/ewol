/**
 *******************************************************************************
 * @file ewol/font/FontManager.cpp
 * @brief ewol Font manager system (Sources)
 * @author Edouard DUPIN
 * @date 22/08/2012
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
#include <etk/Vector.h>
#include <ewol/Debug.h>
#include <ewol/font/FontManager.h>
#include <ewol/font/FontFreeType.h>
#include <ewol/font/TexturedFont.h>

static etk::Vector<ewol::Font*>         l_fontList;
static etk::Vector<ewol::TexturedFont*> l_fontTexturedList;
static etk::UString                     l_folderName = "";
static etk::UString                     l_defaultFontName = "";
static int32_t                          l_defaultFontSize = 10;

void ewol::font::Init(void)
{
	// nothing to do in théory then, we clean the buffers :
	// NOTE : If we do domething here, then the system does not work corectly
	l_fontList.Clear();
	l_fontTexturedList.Clear();
	l_folderName = "";
	l_defaultFontName = "";
	l_defaultFontSize = 10;
	// Specific for free Font
	ewol::FreeTypeInit();
}

void ewol::font::UnInit(void)
{
	// remove textured font ...
	for (int32_t iii=0; iii<l_fontTexturedList.Size(); iii++) {
		if (l_fontTexturedList[iii] != NULL) {
			delete(l_fontTexturedList[iii]);
			l_fontTexturedList[iii] = NULL;
		}
	}
	l_fontTexturedList.Clear();
	// remove fonts :
	for (int32_t iii=0; iii<l_fontList.Size(); iii++) {
		if (l_fontList[iii] != NULL) {
			delete(l_fontList[iii]);
			l_fontList[iii] = NULL;
		}
	}
	l_fontList.Clear();
	// Specific for free Font
	ewol::FreeTypeUnInit();
}

void ewol::font::SetFontFolder(etk::UString folderName)
{
	if (l_folderName != "") {
		EWOL_WARNING("Change the FontFolder, old=\"" << l_folderName << "\"");
	}
	EWOL_TODO("Check if folder exist");
	l_folderName = folderName;
	EWOL_INFO("New default font folder name=\"" << l_folderName << "\"");
}


void ewol::font::SetDefaultFont(etk::UString fontName)
{
	l_defaultFontName = fontName;
}

etk::UString& ewol::font::GetDefaultFont(void)
{
	return l_defaultFontName;
}

void ewol::font::SetDefaultSize(int32_t newSize)
{
	l_defaultFontSize = newSize;
}

int32_t ewol::font::GetDefaultSize(void)
{
	return l_defaultFontSize;
}


ewol::Font* ewol::font::Keep(etk::UString fontName)
{
	etk::UString tmpFileName = l_folderName + "/" + fontName;
	for (int32_t iii=l_fontList.Size()-1; iii>=0; iii--) {
		if (l_fontList[iii] != NULL) {
			if(l_fontList[iii]->HasName(tmpFileName)) {
				l_fontList[iii]->Increment();
				return l_fontList[iii];
			}
		}
	}
	ewol::FontFreeType* tmpResources = new ewol::FontFreeType(tmpFileName);
	if (NULL == tmpResources) {
		EWOL_ERROR("allocation error of a resource Font : " << tmpFileName);
		return NULL;
	}
	l_fontList.PushBack(tmpResources);
	return tmpResources;
}

void ewol::font::Release(ewol::Font*& object)
{
	for (int32_t iii=l_fontList.Size()-1; iii>=0; iii--) {
		if (l_fontList[iii] != NULL) {
			if(l_fontList[iii] == object) {
				if (true == l_fontList[iii]->Decrement()) {
					// delete element
					delete(l_fontList[iii]);
					// remove element from the list :
					l_fontList.Erase(iii);
				}
				// insidiously remove the pointer for the caller ...
				object = NULL;
				return;
			}
		}
	}
}


ewol::TexturedFont* ewol::font::TexturedKeep(etk::UString fontName, int32_t size)
{
	etk::UString tmpFileName = l_folderName + "/" + fontName;
	for (int32_t iii=l_fontTexturedList.Size()-1; iii>=0; iii--) {
		if (l_fontTexturedList[iii] != NULL) {
			if(l_fontTexturedList[iii]->HasName(tmpFileName, size)) {
				l_fontTexturedList[iii]->Increment();
				return l_fontTexturedList[iii];
			}
		}
	}
	ewol::TexturedFont* tmpResources = new ewol::TexturedFont(tmpFileName, size);
	if (NULL == tmpResources) {
		EWOL_ERROR("allocation error of a resource textured Font : " << tmpFileName);
		return NULL;
	}
	l_fontTexturedList.PushBack(tmpResources);
	return tmpResources;
}

void ewol::font::TexturedRelease(ewol::TexturedFont*& object)
{
	for (int32_t iii=l_fontTexturedList.Size()-1; iii>=0; iii--) {
		if (l_fontTexturedList[iii] != NULL) {
			if(l_fontTexturedList[iii] == object) {
				if (true == l_fontTexturedList[iii]->Decrement()) {
					// delete element
					delete(l_fontTexturedList[iii]);
					// remove element from the list :
					l_fontTexturedList.Erase(iii);
				}
				// insidiously remove the pointer for the caller ...
				object = NULL;
				return;
			}
		}
	}
}