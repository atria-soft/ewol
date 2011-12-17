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

static ewol::theme::Theme localTheme;

void ewol::theme::Init(void)
{
	EWOL_INFO("Init theme system");
}


void ewol::theme::UnInit(void)
{
	EWOL_INFO("Un-Init theme system");
}


void ewol::theme::LoadDefault(etk::File filename)
{
	EWOL_INFO("Load default Theme : " << filename);
	localTheme.Load(filename, true);
}


void ewol::theme::Load(etk::File filename)
{
	EWOL_INFO("Add personal theme : " << filename);
	localTheme.Load(filename, false);
}


int32_t ewol::theme::GetObjectId(etk::String name)
{
	return localTheme.GetObjectId(name);
}

// ???? GetObjectType(int32_t id);


void ewol::theme::Generate(int32_t id, int32_t frameId, ewol::OObject2DColored & newObject, etkFloat_t posX, etkFloat_t posY, etkFloat_t sizeX, etkFloat_t sizeY)
{
	if (id<0) {
		return;
	}
	localTheme.Generate(id, frameId, newObject, posX, posY, sizeX, sizeY);
}


int32_t ewol::theme::GetNbFrame(int32_t id)
{
	if (id<0) {
		return 0;
	}
	return localTheme.GetNbFrame(id);
}


int32_t ewol::theme::GetFrameId(int32_t id, etk::String & frameName)
{
	if (id<0) {
		return 0;
	}
	return localTheme.GetFrameId(id, frameName);
}

