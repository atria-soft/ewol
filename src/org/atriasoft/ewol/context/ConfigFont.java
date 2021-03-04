/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ewol/debug.hpp>
#include <etk/uri/uri.hpp>

namespace ewol {
	namespace context {
		class ConfigFont {
			public:
				/**
				 * Constructor / destructor
				 */
				ConfigFont();
				 ~ConfigFont();
			private:
				etk::Uri this.folder;
			public:
				/**
				 * @brief Specify the default font folder for the Ewol search system (only needed when embended font)
				 * @param[in] _folder basic folder of the font (ex: DATA:fonts)
				 */
				void setFolder( etk::Uri _folder) {
					this.folder = _folder;
				};
				/**
				 * @brief get the default font folder.
				 * @return The default font folder.
				 */
				 etk::Uri getFolder() {
					return this.folder;
				};
			private:
				String this.name;
				int this.size;
			public:
				/**
				 * @brief set the defaut font for all the widgets and basics display.
				 * @param[in] _fontName The font name requested (not case sensitive) ex "Arial" or multiple separate by ';' ex : "Arial;Helvetica".
				 * @param[in] _size The default size of the font default=10.
				 */
				void set( String _fontName, int _size);
				/**
				 * @brief get the current default font name
				 * @raturn a reference on the font name string
				 */
				 String getName() {
					return this.name;
				};
				/**
				 * @brief Set the current default font name
				 * @param[in] _fontName The font name requested (not case sensitive) ex "Arial" or multiple separate by ';' ex : "Arial;Helvetica".
				 */
				void setName( String _fontName);
				/**
				 * @brief get the default font size.
				 * @return the font size.
				 */
				int getSize() {
					return this.size;
				};
				/**
				 * @brief Set the default font size.
				 * @param[in] _size new font size.
				 */
				void setSize(int _size);
			private:
				boolean this.useExternal;
			public:
				/**
				 * @brief set use of internal/external Font
				 * @param[in] _val true to enable search of internal data.
				 */
				void setUseExternal(boolean _val) {
					this.useExternal=_val;
				};
				/**
				 * @brief get the use of internal/external Font
				 * @return true to enable search of internal data.
				 */
				boolean getUseExternal() {
					return this.useExternal;
				};
		};
	};
};


