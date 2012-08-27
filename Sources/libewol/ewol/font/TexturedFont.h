/**
 *******************************************************************************
 * @file ewol/font/TexturedFont.h
 * @brief ewol Textured Font system (header)
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



#ifndef __EWOL_TEXTURED_FONT_H__
#define __EWOL_TEXTURED_FONT_H__

#include <ewol/font/Font.h>
#include <ewol/texture/Texture.h>
#include <ewol/Resource.h>

namespace ewol
{
	class TexturedFont : public ewol::Resource {
		
		typedef struct {
			GlyphProperty    property;
			texCoord_ts      posStart;
			texCoord_ts      posStop;
		}freeTypeFontElement_ts;
		private:
			int32_t                             m_size;
			int32_t                             m_height;
			ewol::Font*                         m_font;
			ewol::Texture                       m_texture;
			etk::Vector<freeTypeFontElement_ts> m_listElement;
			// for the texture generation :
			Vector2D<int32_t>                   m_lastGlyphPos;
			int32_t                             m_lastRawHeigh;
		public:
			TexturedFont(etk::UString fontName);
			~TexturedFont(void);
			virtual bool HasName(etk::UString& fileName);
			const char* GetType(void) { return "ewol::TexturedFont"; };
			int32_t getFontSize(void) { return m_size; };
			int32_t Draw(Vector2D<float>                 textPos,
			             const etk::UString&             unicodeString,
			             etk::Vector<Vector2D<float> > & coord,
			             etk::Vector<texCoord_ts> &      coordTex);
			int32_t Draw(Vector2D<float>                 textPos,
			             const uniChar_t                 unicodeChar,
			             etk::Vector<Vector2D<float> > & coord,
			             etk::Vector<texCoord_ts> &      coordTex);
			Vector2D<float> GetSize(const etk::UString & unicodeString);
			Vector2D<float> GetSize(const uniChar_t      unicodeChar);
			// TODO : Remove this element, it is stupid ...
			int32_t GetHeight(void) { return m_height; };
			int32_t GetFontSize(void) { return m_size; };
			ewol::Texture& GetTex(void) { return m_texture; };
	};
	
	
};

#endif

