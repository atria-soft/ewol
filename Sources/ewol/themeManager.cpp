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
#define __class__	"ewol::theme::"

static ewol::theme::Theme defaultTheme;
static ewol::theme::Theme userTheme;

void ewol::theme::Init(void)
{
	
}


void ewol::theme::UnInit(void)
{
	
}


void ewol::theme::LoadDefault(etk::File filename)
{
	defaultTheme.Load(filename);
}


void ewol::theme::Load(etk::File filename)
{
	userTheme.Load(filename);
}

// if 0 ==> error not fined ...
// if <0 ==> default theme
// if >0 ==> user theme
int32_t ewol::theme::GetObjectId(etk::String name)
{
	int32_t val = userTheme.GetObjectId(name);
	if (-1 == val) {
		val = defaultTheme.GetObjectId(name);
		if (-1 == val) {
			return 0;
		} else {
			return val*(-1);
		}
	}
	return val+1;
}

// ???? GetObjectType(int32_t id);


void ewol::theme::Generate(int32_t id, int32_t frameId, OObject2DTextured & newObject, etkFloat_t posX, etkFloat_t posY, etkFloat_t sizeX, etkFloat_t sizeY)
{
	if (id<=0) {
		id *= -1;
		defaultTheme.Generate(id, frameId, newObject, posX, posY, sizeX, sizeY);
	} else {
		userTheme.Generate(id+1, frameId, newObject, posX, posY, sizeX, sizeY);
	}
}


int32_t ewol::theme::GetNbFrame(int32_t id)
{
	if (id==0) {
		return 0;
	} else if (id<=0) {
		id *= -1;
		return defaultTheme.GetNbFrame(id);
	} else {
		return userTheme.GetNbFrame(id+1);
	}
}


int32_t ewol::theme::GetFrameId(int32_t id, etk::String & frameName)
{
	if (id==0) {
		return 0;
	} else if (id<=0) {
		id *= -1;
		return defaultTheme.GetFrameId(id, frameName);
	} else {
		return userTheme.GetFrameId(id+1, frameName);
	}
}

