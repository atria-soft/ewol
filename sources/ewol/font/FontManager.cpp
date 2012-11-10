/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
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


