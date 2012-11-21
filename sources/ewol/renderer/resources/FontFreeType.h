/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
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
	// show : http://www.freetype.org/freetype2/docs/tutorial/step2.html
	class FontFreeType : public ewol::Font
	{
		private:
			FT_Byte *    m_FileBuffer;
			int32_t      m_FileSize;
			FT_Face      m_fftFace;
			bool         m_init;
			void Display(void);
		public:
			FontFreeType(etk::UString fontName);
			~FontFreeType(void);
			
			bool GetGlyphProperty(int32_t              fontSize,
			                      ewol::GlyphProperty& property);
			
			bool DrawGlyph(draw::Image&                imageOut,
			               int32_t                     fontSize,
			               etk::Vector2D<int32_t>      glyphPosition,
			               ewol::GlyphProperty&        property,
			               int8_t posInImage);
			
			etk::Vector2D<float> GetSize(int32_t fontSize, const etk::UString & unicodeString);
			
			int32_t GetHeight(int32_t fontSize);
			
			void GenerateKerning(int32_t fontSize, etk::Vector<ewol::GlyphProperty>& listGlyph);
	};
	void FreeTypeInit(void);
	void FreeTypeUnInit(void);
	
};

#endif

