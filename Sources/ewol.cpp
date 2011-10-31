/**
 *******************************************************************************
 * @file ewol.cpp
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


#include "ewol.h"
#include "ewolFont.h"

#if __PLATFORM__ == X11
	#include "guiX11.h"
#elif __PLATFORM__ == DoubleBuffer
	#include "guiDoubleBuffer.h"
#elif __PLATFORM__ == Android
	#include "guiAndroid.h"
#elif __PLATFORM__ == AndroidTablet
	#include "guiAndroidTablet.h"
#elif __PLATFORM__ == IPhone
	#include "guiIPhone.h"
#elif __PLATFORM__ == IPad
	#include "guiIPad.h"
#else
	#error you need to specify a platform ...
#endif

#undef __class__
#define __class__	"ewol"

void ewol::Init(int argc, char *argv[])
{
	EWOL_INFO("v" EWOL_VERSION_TAG_NAME);
	EWOL_INFO("Build Date: " VERSION_BUILD_TIME);
	guiAbstraction::Init(argc, argv);
}

void ewol::Run(void)
{
	guiAbstraction::Run();
}

void ewol::UnInit(void)
{
	guiAbstraction::UnInit();
	ewol::UnInitFont();
}


void ewol::DisplayWindows(ewol::Windows * windows)
{
	guiAbstraction::SetDisplayOnWindows(windows);
}


void ewol::Stop(void)
{
	guiAbstraction::Stop();
}


