/**
 *******************************************************************************
 * @file ewol/font/FontFreeType.h
 * @brief ewol Font freeType system (header)
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

#ifndef __EWOL_FONT_FREE_TYPE_H__
#define __EWOL_FONT_FREE_TYPE_H__

#include <ewol/font/Font.h>

extern "C" {
	#include <freetype/ft2build.h>
}
#include FT_FREETYPE_H

namespace ewol
{
	class FontFreeType : public ewol::Font
	{
		private:
			FT_Byte *    m_FileBuffer;
			int32_t      m_FileSize;
			FT_Face      m_fftFace;
			void Display(void);
		public:
			FontFreeType(etk::UString fontName);
			~FontFreeType(void);
			int32_t Draw(draw::Image&         imageOut,
			             int32_t              fontSize,
			             Vector2D<float>      textPos,
			             const etk::UString&  unicodeString,
			             draw::Color&         textColor);
			int32_t Draw(draw::Image&     imageOut,
			             int32_t          fontSize,
			             Vector2D<float>  textPos,
			             const uniChar_t  unicodeChar,
			             draw::Color&     textColor);
			bool GenerateBitmapFont(int32_t size, int32_t& height, ewol::Texture& texture, etk::Vector<freeTypeFontElement_ts>& listElement);
			Vector2D<float> GetSize(int32_t fontSize, const etk::UString & unicodeString);
			int32_t GetHeight(int32_t fontSize);
	};
	void FreeTypeInit(void);
	void FreeTypeUnInit(void);
	
};

#endif

