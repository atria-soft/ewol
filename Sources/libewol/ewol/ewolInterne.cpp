/**
 *******************************************************************************
 * @file ewol/ewolInterne.cpp
 * @brief Main code of ewol interne interaction (sources)
 * @author Edouard DUPIN
 * @date 07/01/2012
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


#include <ewol/ewolInterne.h>
#include <ewol/Font.h>
#include <ewol/WidgetManager.h>
#include <ewol/themeManager.h>

#include <ewol/base/gui.h>

#undef __class__
#define __class__	"ewol"

void ewol::Init(int argc, char *argv[])
{
	EWOL_INFO("v" EWOL_VERSION_TAG_NAME);
	EWOL_INFO("Build Date: " VERSION_BUILD_TIME);
	ewol::theme::Init();
	ewol::widgetManager::Init();
	ewol::InitFont();
}

void ewol::UnInit(void)
{
	ewol::UnInitFont();
	ewol::widgetManager::UnInit();
	ewol::theme::UnInit();
}

