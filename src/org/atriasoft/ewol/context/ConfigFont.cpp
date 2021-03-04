/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/context/ConfigFont.hpp>
#include <ewol/resource/FontFreeType.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ConfigFont);

ConfigFont::ConfigFont() :
  this.folder("DATA:///fonts?lib=ewol"),
  this.name("Arial;Helvetica"),
  this.size(10),
  this.useExternal(false) {
	#ifdef __TARGET_OS__Android
		this.name = "Roboto;DroidSans";
	#endif
	ewol::resource::freeTypeInit();
}

ConfigFont::~ConfigFont() {
	// UnInit FreeTypes
	ewol::resource::freeTypeUnInit();
}

void ConfigFont::set( String _fontName, int _size) {
	this.name = _fontName;
	this.size = _size;
	Log.debug("Set default Font : '" + this.name + "' size=" + this.size);
}

void ConfigFont::setSize(int _size) {
	this.size = _size;
	Log.debug("Set default Font : '" + this.name + "' size=" + this.size + " (change size only)");
}

void ConfigFont::setName( String _fontName) {
	this.name = _fontName;
	Log.debug("Set default Font : '" + this.name + "' size=" + this.size + " (change name only)");
}

