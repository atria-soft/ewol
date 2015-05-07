/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/context/ConfigFont.h>
#include <ewol/resource/FontFreeType.h>

#undef __class__
#define __class__ "context::ConfigFont"

ewol::context::ConfigFont::ConfigFont() :
  m_folder("DATA:fonts"),
  m_name("Arial;Helvetica"),
  m_size(10),
  m_useExternal(false) {
	ewol::resource::freeTypeInit();
}

ewol::context::ConfigFont::~ConfigFont() {
	// UnInit FreeTypes
	ewol::resource::freeTypeUnInit();
}

void ewol::context::ConfigFont::set(const std::string& _fontName, int32_t _size) {
	m_name = _fontName;
	m_size = _size;
	EWOL_INFO("Set default Font : '" << _fontName << "' size=" << _size);
}


