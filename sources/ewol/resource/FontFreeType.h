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
				void display(void);
			protected:
				FontFreeType(const std::string& _fontName);
				~FontFreeType(void);
			public:
				
				bool getGlyphProperty(int32_t _fontSize,
				                      ewol::GlyphProperty& _property);
				
				bool drawGlyph(egami::Image& _imageOut,
				               int32_t _fontSize,
				               ivec2 _glyphPosition,
				               ewol::GlyphProperty& _property,
				               int8_t _posInImage);
				
				vec2 getSize(int32_t _fontSize, const std::string& _unicodeString);
				
				int32_t getHeight(int32_t _fontSize);
				
				void generateKerning(int32_t _fontSize, std::vector<ewol::GlyphProperty>& _listGlyph);
			public:
				/**
				 * @brief keep the resource pointer.
				 * @note Never free this pointer by your own...
				 * @param[in] _filename Name of the base font.
				 * @return pointer on the resource or NULL if an error occured.
				 */
				static ewol::resource::FontBase* keep(const std::string& _filename);
				/**
				 * @brief release the keeped resources
				 * @param[in,out] reference on the object pointer
				 */
				static void release(ewol::resource::FontBase*& _object);
		};
		void freeTypeInit(void);
		void freeTypeUnInit(void);
	};
};

#endif

