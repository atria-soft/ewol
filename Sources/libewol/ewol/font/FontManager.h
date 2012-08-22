/**
 *******************************************************************************
 * @file ewol/font/FontManager.h
 * @brief ewol Font manager system (header)
 * @author Edouard DUPIN
 * @date 22/08/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
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
		void SetFontFolder(etk::UString folderName);
		void SetDefaultFont(etk::UString fontName);
		etk::UString& GetDefaultFont(void);
		void SetDefaultSize(int32_t newSize);
		int32_t GetDefaultSize(void);
		
		ewol::Font* Keep(etk::UString fontName);
		void        Release(ewol::Font*& object);
		
		ewol::TexturedFont* TexturedKeep(etk::UString fontName, int32_t size);
		void                TexturedRelease(ewol::TexturedFont*& object);
	};
};

#endif