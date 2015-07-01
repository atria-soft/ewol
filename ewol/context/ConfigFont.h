/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_CONFIG_FONT_H__
#define __EWOL_CONFIG_FONT_H__

#include <ewol/debug.h>

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
				std::string m_folder;
			public:
				/**
				 * @brief Specify the default font folder for the Ewol search system (only needed when embended font)
				 * @param[in] _folder basic folder of the font (ex: DATA:fonts)
				 */
				void setFolder(const std::string& _folder) {
					m_folder = _folder;
				};
				/**
				 * @brief get the default font folder.
				 * @return The default font folder.
				 */
				const std::string& getFolder() {
					return m_folder;
				};
			private:
				std::string m_name;
				int32_t m_size;
			public:
				/**
				 * @brief set the defaut font for all the widgets and basics display.
				 * @param[in] _fontName The font name requested (not case sensitive) ex "Arial" or multiple separate by ';' ex : "Arial;Helvetica".
				 * @param[in] _size The default size of the font default=10.
				 */
				void set(const std::string& _fontName, int32_t _size);
				/**
				 * @brief get the current default font name
				 * @raturn a reference on the font name string
				 */
				const std::string& getName() {
					return m_name;
				};
				/**
				 * @brief Set the current default font name
				 * @param[in] _fontName The font name requested (not case sensitive) ex "Arial" or multiple separate by ';' ex : "Arial;Helvetica".
				 */
				void setName(const std::string& _fontName);
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

#endif

