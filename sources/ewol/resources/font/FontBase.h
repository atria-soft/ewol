/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_FONT_H__
#define __EWOL_FONT_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <egami/Image.h>
#include <ewol/resources/Texture.h>
#include <ewol/resources/Resource.h>
#include <ewol/resources/font/GlyphProperty.h>


namespace ewol
{
	class FontBase : public ewol::Resource
	{
		protected:
		public:
			FontBase(const etk::UString& _fontName) : ewol::Resource(_fontName) {};
			
			virtual ~FontBase(void) { };
			
			const char* getType(void) { return "ewol::Font"; };
			
			virtual bool getGlyphProperty(int32_t _fontSize,
			                              ewol::GlyphProperty& _property) = 0;
			
			virtual bool drawGlyph(egami::Image& _imageOut,
			                       int32_t _fontSize,
			                       ivec2 _glyphPosition,
			                       ewol::GlyphProperty& _property,
			                       int8_t _posInImage) = 0;
			
			virtual vec2 getSize(int32_t _fontSize, const etk::UString& _unicodeString) = 0;
			
			virtual int32_t getHeight(int32_t _fontSize) = 0;
			
			virtual void generateKerning(int32_t _fontSize, etk::Vector<ewol::GlyphProperty>& _listGlyph) { };
			
			virtual void display(void) {};
	};
};

#endif

