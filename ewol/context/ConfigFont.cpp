/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/context/ConfigFont.hpp>
#include <ewol/resource/FontFreeType.hpp>

ewol::context::ConfigFont::ConfigFont() :
  m_folder("{ewol}DATA:fonts"),
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

void ewol::context::ConfigFont::set(const std::string& _fontName, int32_t _size) {
	m_name = _fontName;
	m_size = _size;
	EWOL_INFO("Set default Font : '" << m_name << "' size=" << m_size);
}

void ewol::context::ConfigFont::setSize(int32_t _size) {
	m_size = _size;
	EWOL_INFO("Set default Font : '" << m_name << "' size=" << m_size << " (change size only)");
}

void ewol::context::ConfigFont::setName(const std::string& _fontName) {
	m_name = _fontName;
	EWOL_INFO("Set default Font : '" << m_name << "' size=" << m_size << " (change name only)");
}

