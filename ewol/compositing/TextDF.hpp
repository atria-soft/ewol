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
#include <string>

namespace ewol {
	namespace compositing {
		class TextDF : public ewol::compositing::TextBase {
			protected:
				ememory::SharedPtr<ewol::resource::DistanceFieldFont> m_fontDF; //!< Font resources
			protected:
				int32_t m_GLglyphLevel; //!< openGL Id on the glyph level display
			public:
				/**
				 * @brief generic constructor
				 * @param[in] _fontName Name of the font that might be loaded
				 * @param[in] _fontSize size of the font that might be loaded
				 */
				TextDF(const std::string& _fontName="", int32_t _fontSize=-1);
				/**
				 * @brief generic destructor
				 */
				virtual ~TextDF();
			public:
				/**
				 * @brief Calculate size to be at the best size for a render in this special size.
				 * @note special for Distance field mode.
				 * @param[in] _size request dimention.
				 */
				void updateSizeToRender(const vec2& _size);
			public:
				virtual void drawD(bool _disableDepthTest);
				virtual void drawMT(const mat4& _transformationMatrix, bool _enableDepthTest);
			protected:
				float m_size;
			public:
				virtual float getHeight();
				virtual float getSize() {
					return m_size;
				}
				virtual void setSize(float _size) {
					m_size = _size;
				}
				virtual ewol::GlyphProperty * getGlyphPointer(char32_t _charcode);
				
			public:
				virtual void loadProgram(const std::string& _shaderName);
				virtual void setFontSize(int32_t _fontSize);
				virtual void setFontName(const std::string& _fontName);
				virtual void setFont(std::string _fontName, int32_t _fontSize);
				virtual void setFontMode(enum ewol::font::mode _mode);
				virtual void printChar(const char32_t& _charcode);
				virtual vec3 calculateSizeChar(const char32_t& _charcode);
		};
	}
}


