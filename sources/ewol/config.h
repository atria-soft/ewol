/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_CONFIG_H__
#define __EWOL_CONFIG_H__

#include <etk/types.h>
#include <etk/UString.h>

namespace ewol
{
	namespace config
	{
		/**
		 * @brief Specify the default font folder for the Ewol search system (only needed when embended font)
		 * @param[in] _folder basic folder of the font (ex: DATA:fonts)
		 */
		void FontFolder(const etk::UString& _folder);
		/**
		 * @brief Set the defaut font for all the widgets and basics display.
		 * @param[in] _fontName The font name requested (not case sensitive) ex "Arial" or multiple separate by ';' ex : "Arial;Helvetica".
		 * @param[in] _size The default size of the font default=10.
		 */
		void FontSetDefault(const etk::UString& _fontName, int32_t _size);
		/**
		 * @brief Get the current default font name
		 * @raturn a reference on the font name string
		 */
		const etk::UString& FontGetDefaultName(void);
		/**
		 * @brief Get the default font size.
		 * @return the font size.
		 */
		int32_t FontGetDefaultSize(void);
		
		
		// Internal section :
		
		/**
		 * Init the configuration
		 */
		void Init(void);
		/**
		 * UnInit the configuration
		 */
		void UnInit(void);
	};
};

#endif

