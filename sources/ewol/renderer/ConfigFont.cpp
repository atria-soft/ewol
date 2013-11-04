/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/renderer/ConfigFont.h>
#include <ewol/resources/FontFreeType.h>

#undef __class__
#define __class__ "ConfigFont"

ewol::ConfigFont::ConfigFont(void) :
  m_folder("DATA:fonts"),
  m_name("Arial;Helvetica"),
  m_size(10),
  m_useExternal(false) {
	ewol::freeTypeInit();
}

ewol::ConfigFont::~ConfigFont(void) {
	// UnInit FreeTypes
	ewol::freeTypeUnInit();
}

void ewol::ConfigFont::set(const etk::UString& _fontName, int32_t _size) {
	m_name = _fontName;
	m_size = _size;
	EWOL_INFO("Set default Font : '" << _fontName << "' size=" << _size);
}


