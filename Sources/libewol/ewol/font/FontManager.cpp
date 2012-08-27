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

static etk::UString                     l_defaultFontName = "";
static int32_t                          l_defaultFontSize = 10;

void ewol::font::Init(void)
{
	l_defaultFontName = "";
	l_defaultFontSize = 10;
	// Specific for free Font
	ewol::FreeTypeInit();
}

void ewol::font::UnInit(void)
{
	// Specific for free Font
	ewol::FreeTypeUnInit();
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


