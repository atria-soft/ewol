/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/Color.hpp>

#include <ewol/debug.hpp>
#include <ewol/compositing/Compositing.hpp>
#include <ewol/compositing/Drawing.hpp>
#include <ewol/resource/DistanceFieldFont.hpp>
#include <ewol/compositing/TextBase.hpp>
#include <exml/exml.hpp>
#include <etk/String.hpp>

namespace ewol {
	namespace compositing {
		class TextDF : public ewol::compositing::TextBase {
			protected:
				ememory::Ptr<ewol::resource::DistanceFieldFont> this.fontDF; //!< Font resources
			protected:
				int this.GLglyphLevel; //!< openGL Id on the glyph level display
			public:
				/**
				 * @brief generic ructor
				 * @param[in] _fontName Name of the font that might be loaded
				 * @param[in] _fontSize size of the font that might be loaded
				 */
				TextDF( String _fontName="", int _fontSize=-1);
				/**
				 * @brief generic destructor
				 */
				 ~TextDF();
			public:
				/**
				 * @brief Calculate size to be at the best size for a render in this special size.
				 * @note special for Distance field mode.
				 * @param[in] _size request dimention.
				 */
				void updateSizeToRender( Vector2f _size);
			public:
				 void drawD(boolean _disableDepthTest);
				 void drawMT( mat4 _transformationMatrix, boolean _enableDepthTest);
			protected:
				float this.size;
			public:
				 float getHeight();
				 float getSize() {
					return this.size;
				}
				 void setSize(float _size) {
					this.size = _size;
				}
				 ewol::GlyphProperty * getGlyphPointer(Character _charcode);
				
			public:
				 void loadProgram( String _shaderName);
				 void setFontSize(int _fontSize);
				 void setFontName( String _fontName);
				 void setFont(String _fontName, int _fontSize);
				 void setFontMode(enum ewol::font::mode _mode);
				 void printChar( Character _charcode);
				 Vector3f calculateSizeChar( Character _charcode);
		};
	}
}


