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
				virtual ~ConfigFont();
			private:
				etk::Uri m_folder;
			public:
				/**
				 * @brief Specify the default font folder for the Ewol search system (only needed when embended font)
				 * @param[in] _folder basic folder of the font (ex: DATA:fonts)
				 */
				void setFolder(const etk::Uri& _folder) {
					m_folder = _folder;
				};
				/**
				 * @brief get the default font folder.
				 * @return The default font folder.
				 */
				const etk::Uri& getFolder() {
					return m_folder;
				};
			private:
				etk::String m_name;
				int32_t m_size;
			public:
				/**
				 * @brief set the defaut font for all the widgets and basics display.
				 * @param[in] _fontName The font name requested (not case sensitive) ex "Arial" or multiple separate by ';' ex : "Arial;Helvetica".
				 * @param[in] _size The default size of the font default=10.
				 */
				void set(const etk::String& _fontName, int32_t _size);
				/**
				 * @brief get the current default font name
				 * @raturn a reference on the font name string
				 */
				const etk::String& getName() {
					return m_name;
				};
				/**
				 * @brief Set the current default font name
				 * @param[in] _fontName The font name requested (not case sensitive) ex "Arial" or multiple separate by ';' ex : "Arial;Helvetica".
				 */
				void setName(const etk::String& _fontName);
				/**
				 * @brief get the default font size.
				 * @return the font size.
				 */
				int32_t getSize() {
					return m_size;
				};
				/**
				 * @brief Set the default font size.
				 * @param[in] _size new font size.
				 */
				void setSize(int32_t _size);
			private:
				bool m_useExternal;
			public:
				/**
				 * @brief set use of internal/external Font
				 * @param[in] _val true to enable search of internal data.
				 */
				void setUseExternal(bool _val) {
					m_useExternal=_val;
				};
				/**
				 * @brief get the use of internal/external Font
				 * @return true to enable search of internal data.
				 */
				bool getUseExternal() {
					return m_useExternal;
				};
		};
	};
};


