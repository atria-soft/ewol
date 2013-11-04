/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_CONFIG_FONT_H__
#define __EWOL_CONFIG_FONT_H__

#include <etk/types.h>
#include <etk/UString.h>

namespace ewol
{
	class ConfigFont
	{
		public:
			/**
			 * Constructor / destructor
			 */
			ConfigFont(void);
			~ConfigFont(void);
		private:
			etk::UString m_folder;
		public:
			/**
			 * @brief Specify the default font folder for the Ewol search system (only needed when embended font)
			 * @param[in] _folder basic folder of the font (ex: DATA:fonts)
			 */
			void setFolder(const etk::UString& _folder) {
				m_folder = _folder;
			};
			/**
			 * @brief get the default font folder.
			 * @return The default font folder.
			 */
			const etk::UString& getFolder(void) {
				return m_folder;
			};
		private:
			etk::UString m_name;
			int32_t m_size;
		public:
			/**
			 * @brief set the defaut font for all the widgets and basics display.
			 * @param[in] _fontName The font name requested (not case sensitive) ex "Arial" or multiple separate by ';' ex : "Arial;Helvetica".
			 * @param[in] _size The default size of the font default=10.
			 */
			void set(const etk::UString& _fontName, int32_t _size);
			/**
			 * @brief get the current default font name
			 * @raturn a reference on the font name string
			 */
			const etk::UString& getName(void) {
				return m_name;
			};
			/**
			 * @brief get the default font size.
			 * @return the font size.
			 */
			int32_t getSize(void) {
				return m_size;
			};
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
			bool getUseExternal(void) {
				return m_useExternal;
			};
	};
};

#endif

