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
				List<FT_Byte> this.FileBuffer;
				int this.FileSize;
				FT_Face this.fftFace;
				boolean this.init;
				void display();
			protected:
				FontFreeType();
				void init( etk::Uri _uri);
			public:
				DECLARE_RESOURCE_URI_FACTORY(FontFreeType);
				 ~FontFreeType();
			public:
				
				boolean getGlyphProperty(int _fontSize,
				                      ewol::GlyphProperty _property);
				
				boolean drawGlyph(egami::Image _imageOut,
				               int _fontSize,
				               Vector2i _glyphPosition,
				               ewol::GlyphProperty _property,
				               int8_t _posInImage);
				
				boolean drawGlyph(egami::ImageMono _imageOut,
				               int _fontSize,
				               ewol::GlyphProperty _property,
				               int _borderSize = 0);
				
				Vector2f getSize(int _fontSize,  String _unicodeString);
				
				int getHeight(int _fontSize);
				float getSizeWithHeight(float _fontHeight);
				
				void generateKerning(int _fontSize, List<ewol::GlyphProperty> _listGlyph);
		};
		void freeTypeInit();
		void freeTypeUnInit();
	};
};

