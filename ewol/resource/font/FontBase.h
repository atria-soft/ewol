/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <ewol/debug.h>
#include <egami/Image.h>
#include <egami/ImageMono.h>
#include <ewol/resource/Texture.h>
#include <gale/resource/Resource.h>
#include <ewol/resource/font/GlyphProperty.h>


namespace ewol {
	namespace resource {
		class FontBase : public gale::Resource {
			public:
				FontBase() {
					addResourceType("ewol::FontFreeType");
				}
				void init(const std::string& _fontName) {
					gale::Resource::init(_fontName);
				};
				
				virtual ~FontBase() { };
				
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
				
				virtual void display() {};
		};
	};
};

