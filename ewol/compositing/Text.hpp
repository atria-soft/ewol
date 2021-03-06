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
				ememory::SharedPtr<ewol::resource::TexturedFont> m_font; //!< Font resources
			public:
				/**
				 * @brief generic constructor
				 * @param[in] _fontName Name of the font that might be loaded
				 * @param[in] _fontSize size of the font that might be loaded
				 */
				Text(const etk::String& _fontName="", int32_t _fontSize=-1);
				/**
				 * @brief generic destructor
				 */
				virtual ~Text();
			public:
				virtual void drawD(bool _disableDepthTest);
				virtual void drawMT(const mat4& _transformationMatrix, bool _enableDepthTest);
			protected:
				float m_size;
			public:
				virtual float getHeight();
				virtual float getSize();
				virtual ewol::GlyphProperty * getGlyphPointer(char32_t _charcode);
				
			public:
				virtual void setFontSize(int32_t _fontSize);
				virtual void setFontName(const etk::String& _fontName);
				virtual void setFont(etk::String _fontName, int32_t _fontSize);
				virtual void setFontMode(enum ewol::font::mode _mode);
				virtual void printChar(const char32_t& _charcode);
				virtual vec3 calculateSizeChar(const char32_t& _charcode);
		};
	}
}

