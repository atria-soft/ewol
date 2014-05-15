/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_COMPOSITING_TEXT_H__
#define __EWOL_COMPOSITING_TEXT_H__

#include <etk/Color.h>

#include <ewol/debug.h>
#include <ewol/compositing/Compositing.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/resource/TexturedFont.h>
#include <ewol/compositing/TextBase.h>
#include <exml/exml.h>
#include <string>

#include <etk/Color.h>

namespace ewol {
	namespace compositing {
		class Text : public ewol::compositing::TextBase {
			protected:
				ewol::resource::TexturedFont* m_font; //!< Font resources
			public:
				/**
				 * @brief generic constructor
				 * @param[in] _fontName Name of the font that might be loaded
				 * @param[in] _fontSize size of the font that might be loaded
				 */
				Text(const std::string& _fontName="", int32_t _fontSize=-1);
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
				virtual void setFontName(const std::string& _fontName);
				virtual void setFont(std::string _fontName, int32_t _fontSize);
				virtual void setFontMode(enum ewol::font::mode _mode);
				virtual void printChar(const char32_t& _charcode);
				virtual vec3 calculateSizeChar(const char32_t& _charcode);
		};
	};
};


#endif