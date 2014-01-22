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
#include <egami/ImageMono.h>
#include <ewol/resource/Texture.h>
#include <ewol/resource/Resource.h>
#include <ewol/resource/font/GlyphProperty.h>


namespace ewol {
	namespace resource {
		class FontBase : public ewol::Resource {
			public:
				FontBase(const std::string& _fontName) : ewol::Resource(_fontName) {};
				
				virtual ~FontBase(void) { };
				
				virtual bool getGlyphProperty(int32_t _fontSize,
				                              ewol::GlyphProperty& _property) = 0;
				
				virtual bool drawGlyph(egami::Image& _imageOut,
				                       int32_t _fontSize,
				                       ivec2 _glyphPosition,
				                       ewol::GlyphProperty& _property,
				                       int8_t _posInImage) = 0;
				
				virtual bool drawGlyph(egami::ImageMono& _imageOut,
				                       int32_t _fontSize,
				                       ewol::GlyphProperty& _property,
				                       int32_t _borderSize = 0) = 0;
				
				virtual vec2 getSize(int32_t _fontSize, const std::string& _unicodeString) = 0;
				virtual float getSizeWithHeight(float _fontHeight) = 0;
				
				virtual int32_t getHeight(int32_t _fontSize) = 0;
				
				virtual void generateKerning(int32_t _fontSize, std::vector<ewol::GlyphProperty>& _listGlyph) { };
				
				virtual void display(void) {};
		};
	};
};

#endif

