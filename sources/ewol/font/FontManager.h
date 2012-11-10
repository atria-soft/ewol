/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_FONT_MANAGER_H__
#define __EWOL_FONT_MANAGER_H__

#include <ewol/font/Font.h>
#include <ewol/font/TexturedFont.h>
#include <ewol/font/FontFreeType.h>

namespace ewol
{
	namespace font {
		void Init(void);
		void UnInit(void);
		void SetDefaultFont(etk::UString fontName);
		etk::UString& GetDefaultFont(void);
		void SetDefaultSize(int32_t newSize);
		int32_t GetDefaultSize(void);
	};
};

#endif

