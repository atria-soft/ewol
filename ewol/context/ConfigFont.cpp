/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/context/ConfigFont.hpp>
#include <ewol/resource/FontFreeType.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::context::ConfigFont);

ewol::context::ConfigFont::ConfigFont() :
  m_folder("DATA:///fonts?lib=ewol"),
  m_name("Arial;Helvetica"),
  m_size(10),
  m_useExternal(false) {
	#ifdef __TARGET_OS__Android
		m_name = "Roboto;DroidSans";
	#endif
	ewol::resource::freeTypeInit();
}

ewol::context::ConfigFont::~ConfigFont() {
	// UnInit FreeTypes
	ewol::resource::freeTypeUnInit();
}

void ewol::context::ConfigFont::set(const etk::String& _fontName, int32_t _size) {
	m_name = _fontName;
	m_size = _size;
	EWOL_DEBUG("Set default Font : '" << m_name << "' size=" << m_size);
}

void ewol::context::ConfigFont::setSize(int32_t _size) {
	m_size = _size;
	EWOL_DEBUG("Set default Font : '" << m_name << "' size=" << m_size << " (change size only)");
}

void ewol::context::ConfigFont::setName(const etk::String& _fontName) {
	m_name = _fontName;
	EWOL_DEBUG("Set default Font : '" << m_name << "' size=" << m_size << " (change name only)");
}

