/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/resource/font/FontBase.hpp>
#include <etk/uri/uri.hpp>
#include <egami/egami.hpp>

extern "C" {
	#include <ft2build.h>
}
#include FT_FREETYPE_H

namespace ewol {
	namespace resource {
		// show : http://www.freetype.org/freetype2/docs/tutorial/step2.html
		class FontFreeType : public ewol::resource::FontBase {
			private:
				etk::Vector<FT_Byte> m_FileBuffer;
				int32_t m_FileSize;
				FT_Face m_fftFace;
				bool m_init;
				void display();
			protected:
				FontFreeType();
				void init(const etk::Uri& _uri);
			public:
				DECLARE_RESOURCE_URI_FACTORY(FontFreeType);
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
				
				vec2 getSize(int32_t _fontSize, const etk::String& _unicodeString);
				
				int32_t getHeight(int32_t _fontSize);
				float getSizeWithHeight(float _fontHeight);
				
				void generateKerning(int32_t _fontSize, etk::Vector<ewol::GlyphProperty>& _listGlyph);
		};
		void freeTypeInit();
		void freeTypeUnInit();
	};
};

