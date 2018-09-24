/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <egami/Image.hpp>
#include <egami/ImageMono.hpp>
#include <ewol/resource/Texture.hpp>
#include <gale/resource/Resource.hpp>
#include <ewol/resource/font/GlyphProperty.hpp>


namespace ewol {
	namespace resource {
		class FontBase : public gale::Resource {
			public:
				FontBase() {
					addResourceType("ewol::FontFreeType");
				}
				void init(const etk::Uri& _uri) {
					gale::Resource::init(_uri);
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
				
				virtual vec2 getSize(int32_t _fontSize, const etk::String& _unicodeString) = 0;
				virtual float getSizeWithHeight(float _fontHeight) = 0;
				
				virtual int32_t getHeight(int32_t _fontSize) = 0;
				
				virtual void generateKerning(int32_t _fontSize, etk::Vector<ewol::GlyphProperty>& _listGlyph) { };
				
				virtual void display() {};
		};
	};
};

