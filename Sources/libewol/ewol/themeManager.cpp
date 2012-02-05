/**
 *******************************************************************************
 * @file ewol/themeManager.cpp
 * @brief basic ewol theme Manager (Sources)
 * @author Edouard DUPIN
 * @date 23/11/2011
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

#include <ewol/themeManager.h>
#include <ewol/theme/Theme.h>


#undef __class__
#define __class__	"ewol::theme"

static ewol::theme::Theme * localTheme = NULL;

void ewol::theme::Init(void)
{
	EWOL_DEBUG("==> Init Theme-Manager");
	localTheme = new ewol::theme::Theme();
}


void ewol::theme::UnInit(void)
{
	EWOL_DEBUG("==> Un-Init Theme-Manager");
	if (NULL != localTheme) {
		delete(localTheme);
		localTheme = NULL;
	}
}


void ewol::theme::LoadDefault(etk::File filename)
{
	EWOL_INFO("Load default Theme : " << filename);
	if (NULL == localTheme) {
		return;
	}
	localTheme->Load(filename, true);
}


void ewol::theme::Load(etk::File filename)
{
	EWOL_INFO("Add personal theme : " << filename);
	if (NULL == localTheme) {
		return;
	}
	localTheme->Load(filename, false);
}


int32_t ewol::theme::GetObjectId(etk::String name)
{
	if (NULL == localTheme) {
		return 0;
	}
	return localTheme->GetObjectId(name);
}

// ???? GetObjectType(int32_t id);


void ewol::theme::Generate(int32_t id, int32_t frameId, ewol::OObject2DColored & newObject, etkFloat_t posX, etkFloat_t posY, etkFloat_t sizeX, etkFloat_t sizeY)
{
	if (NULL == localTheme) {
		return;
	}
	if (id<0) {
		return;
	}
	localTheme->Generate(id, frameId, newObject, posX, posY, sizeX, sizeY);
}


int32_t ewol::theme::GetNbFrame(int32_t id)
{
	if (NULL == localTheme) {
		return 0;
	}
	if (id<0) {
		return 0;
	}
	return localTheme->GetNbFrame(id);
}


int32_t ewol::theme::GetFrameId(int32_t id, etk::String & frameName)
{
	if (NULL == localTheme) {
		return 0;
	}
	if (id<0) {
		return 0;
	}
	return localTheme->GetFrameId(id, frameName);
}

