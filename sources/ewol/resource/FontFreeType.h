/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_FONT_FREE_TYPE_H__
#define __EWOL_FONT_FREE_TYPE_H__

#include <etk/types.h>
#include <ewol/resource/font/FontBase.h>
#include <egami/egami.h>

extern "C" {
	#include <freetype/ft2build.h>
}
#include FT_FREETYPE_H

namespace ewol {
	namespace resource {
		// show : http://www.freetype.org/freetype2/docs/tutorial/step2.html
		class FontFreeType : public ewol::resource::FontBase {
			private:
				FT_Byte* m_FileBuffer;
				int32_t m_FileSize;
				FT_Face m_fftFace;
				bool m_init;
				void display();
			protected:
				FontFreeType();
				void init(const std::string& _fontName);
			public:
				DECLARE_RESOURCE_NAMED_FACTORY(FontFreeType);
				virtual ~FontFreeType();
			public:
				
				bool getGlyphProperty(int32_t _fontSize,
				                      ewol::GlyphProperty& _property);
				
				bool drawGlyph(egami::Image& _imageOut,
				               int32_t _fontSize,
				               ivec2 _glyphPosition,
				               ewol::GlyphProperty& _property,
				               int8_t _posInImage);
				
				bool drawGlyph(egami::ImageMono& _imageOut,
				               int32_t _fontSize,
				               ewol::GlyphProperty& _property,
				               int32_t _borderSize = 0);
				
				vec2 getSize(int32_t _fontSize, const std::string& _unicodeString);
				
				int32_t getHeight(int32_t _fontSize);
				float getSizeWithHeight(float _fontHeight);
				
				void generateKerning(int32_t _fontSize, std::vector<ewol::GlyphProperty>& _listGlyph);
		};
		void freeTypeInit();
		void freeTypeUnInit();
	};
};

#endif

