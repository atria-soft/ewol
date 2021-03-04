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
#include <ewol/resource/TexturedFont.hpp>
#include <ewol/compositing/TextBase.hpp>
#include <exml/exml.hpp>
#include <etk/String.hpp>

#include <etk/Color.hpp>

namespace ewol {
	namespace compositing {
		class Text : public ewol::compositing::TextBase {
			protected:
				ememory::Ptr<ewol::resource::TexturedFont> this.font; //!< Font resources
			public:
				/**
				 * @brief generic ructor
				 * @param[in] _fontName Name of the font that might be loaded
				 * @param[in] _fontSize size of the font that might be loaded
				 */
				Text( String _fontName="", int _fontSize=-1);
				/**
				 * @brief generic destructor
				 */
				 ~Text();
			public:
				 void drawD(boolean _disableDepthTest);
				 void drawMT( mat4 _transformationMatrix, boolean _enableDepthTest);
			protected:
				float this.size;
			public:
				 float getHeight();
				 float getSize();
				 ewol::GlyphProperty * getGlyphPointer(Character _charcode);
				
			public:
				 void setFontSize(int _fontSize);
				 void setFontName( String _fontName);
				 void setFont(String _fontName, int _fontSize);
				 void setFontMode(enum ewol::font::mode _mode);
				 void printChar( Character _charcode);
				 Vector3f calculateSizeChar( Character _charcode);
		};
	}
}

