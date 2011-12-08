/**
 *******************************************************************************
 * @file ewol/ewol.cpp
 * @brief Main code of ewol (sources)
 * @author Edouard DUPIN
 * @date 17/10/2011
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


#include <ewol/ewol.h>
#include <ewol/Font.h>
#include <ewol/WidgetManager.h>
#include <ewol/themeManager.h>

#include <base/gui.h>

#undef __class__
#define __class__	"ewol"

void ewol::Init(int argc, char *argv[])
{
	EWOL_INFO("v" EWOL_VERSION_TAG_NAME);
	EWOL_INFO("Build Date: " VERSION_BUILD_TIME);
	guiAbstraction::Init(argc, argv);
	ewol::theme::Init();
	ewol::widgetManager::Init();
	ewol::InitFont();
}

void ewol::Run(void)
{
	guiAbstraction::Run();
}

void ewol::UnInit(void)
{
	guiAbstraction::UnInit();
	ewol::UnInitFont();
	ewol::widgetManager::UnInit();
	ewol::theme::UnInit();
}


void ewol::DisplayWindows(ewol::Windows * windows)
{
	// Remove current Focus :
	ewol::widgetManager::FocusSetDefault(NULL);
	ewol::widgetManager::FocusRelease();
	// set display of the windows :
	guiAbstraction::SetDisplayOnWindows(windows);
	// Set the new default Focus :
	ewol::widgetManager::FocusSetDefault(windows);
}


void ewol::Stop(void)
{
	guiAbstraction::Stop();
}


void ewol::ChangeSize(int32_t w, int32_t h)
{
	guiAbstraction::ChangeSize(w, h);
}

void ewol::ChangePos(int32_t x, int32_t y)
{
	guiAbstraction::ChangePos(x, y);
}

void ewol::GetAbsPos(int32_t & x, int32_t & y)
{
	guiAbstraction::GetAbsPos(x, y);
}


void ewol::StartResizeSystem(void)
{
#ifdef __PLATFORM__X11
	guiAbstraction::StartResizeSystem();
#endif
}

void ewol::StartMoveSystem(void)
{
#ifdef __PLATFORM__X11
	guiAbstraction::StartMoveSystem();
#endif
}


bool ewol::IsPressedInput(int32_t inputID)
{
	return guiAbstraction::IsPressedInput(inputID);
}

