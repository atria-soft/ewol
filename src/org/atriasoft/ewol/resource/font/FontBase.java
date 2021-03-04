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
				void init( etk::Uri _uri) {
					gale::Resource::init(_uri);
				};
				
				 ~FontBase() { };
				
				 boolean getGlyphProperty(int _fontSize,
				                              ewol::GlyphProperty _property) = 0;
				
				 boolean drawGlyph(egami::Image _imageOut,
				                       int _fontSize,
				                       Vector2i _glyphPosition,
				                       ewol::GlyphProperty _property,
				                       int8_t _posInImage) = 0;
				
				 boolean drawGlyph(egami::ImageMono _imageOut,
				                       int _fontSize,
				                       ewol::GlyphProperty _property,
				                       int _borderSize = 0) = 0;
				
				 Vector2f getSize(int _fontSize,  String _unicodeString) = 0;
				 float getSizeWithHeight(float _fontHeight) = 0;
				
				 int getHeight(int _fontSize) = 0;
				
				 void generateKerning(int _fontSize, List<ewol::GlyphProperty> _listGlyph) { };
				
				 void display() {};
		};
	};
};

