/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_COMPOSITING_TEXT_DF_H__
#define __EWOL_COMPOSITING_TEXT_DF_H__

#include <etk/Color.h>

#include <ewol/debug.h>
#include <ewol/compositing/Compositing.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/resource/DistanceFieldFont.h>
#include <ewol/compositing/TextBase.h>
#include <exml/exml.h>
#include <string>

namespace ewol {
	namespace compositing {
		class TextDF : public ewol::compositing::TextBase {
			protected:
				ewol::resource::DistanceFieldFont* m_fontDF; //!< Font resources
				std::vector<float> m_glyphLevel; //!< Level of display of the glyph (notmal : 0.50, bold : 0.40, super bold : 0.30 ...)
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
				virtual ~TextDF(void);
			public:
				/**
				 * @brief Calculate size to be at the best size for a render in this special size.
				 * @note special for Distance field mode.
				 * @param[in] _size request dimention.
				 */
				void updateSizeToRender(const vec2& _size);
			public:
				virtual void clear(void);
				virtual void drawD(bool _disableDepthTest);
				virtual void drawMT(const mat4& _transformationMatrix, bool _enableDepthTest);
			protected:
				float m_size;
			public:
				virtual float getHeight(void);
				virtual float getSize(void) {
					return m_size;
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
	};
};

#endif

