/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/config.h>
#include <ewol/renderer/resources/FontFreeType.h>

#undef __class__
#define __class__	"ewol::config"

static etk::UString l_fontConfigFolder = "DATA::fonts";
static etk::UString l_fontConfigName = "Arial";
static int32_t      l_fontConfigSize = 10;

void ewol::config::Init(void)
{
	// reset font properties
	l_fontConfigFolder = "DATA::fonts";
	l_fontConfigName = "Arial";
	l_fontConfigSize = 10;
	ewol::FreeTypeInit();
}

void ewol::config::UnInit(void)
{
	// UnInit FreeTypes
	ewol::FreeTypeUnInit();
}

void ewol::config::FontFolder(const etk::UString& _folder)
{
	l_fontConfigFolder = _folder;
}

void ewol::config::FontSetDefault(const etk::UString& _fontName, int32_t _size)
{
	l_fontConfigName = _fontName;
	l_fontConfigSize = _size;
}

const etk::UString& ewol::config::FontGetDefaultName(void)
{
	return l_fontConfigName;
}

int32_t ewol::config::FontGetDefaultSize(void)
{
	return l_fontConfigSize;
}



