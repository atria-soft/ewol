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
#include <ewol/resources/font/FontBase.h>

extern "C" {
	#include <freetype/ft2build.h>
}
#include FT_FREETYPE_H

namespace ewol
{
	// show : http://www.freetype.org/freetype2/docs/tutorial/step2.html
	class FontFreeType : public ewol::FontBase
	{
		private:
			FT_Byte* m_FileBuffer;
			int32_t m_FileSize;
			FT_Face m_fftFace;
			bool m_init;
			void Display(void);
		protected:
			FontFreeType(const etk::UString& _fontName);
			~FontFreeType(void);
		public:
			
			bool GetGlyphProperty(int32_t _fontSize,
			                      ewol::GlyphProperty& _property);
			
			bool DrawGlyph(egami::Image& _imageOut,
			               int32_t _fontSize,
			               ivec2 _glyphPosition,
			               ewol::GlyphProperty& _property,
			               int8_t _posInImage);
			
			vec2 GetSize(int32_t _fontSize, const etk::UString& _unicodeString);
			
			int32_t GetHeight(int32_t _fontSize);
			
			void GenerateKerning(int32_t _fontSize, etk::Vector<ewol::GlyphProperty>& _listGlyph);
		public:
			/**
			 * @brief Keep the resource pointer.
			 * @note Never free this pointer by your own...
			 * @param[in] _filename Name of the base font.
			 * @return pointer on the resource or NULL if an error occured.
			 */
			static ewol::FontBase* Keep(const etk::UString& _filename);
			/**
			 * @brief Release the keeped resources
			 * @param[in,out] reference on the object pointer
			 */
			static void Release(ewol::FontBase*& _object);
	};
	void FreeTypeInit(void);
	void FreeTypeUnInit(void);
	
};

#endif

