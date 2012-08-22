/**
 *******************************************************************************
 * @file ewol/font/Font.h
 * @brief ewol Font system (header)
 * @author Edouard DUPIN
 * @date 29/10/2011
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#ifndef __EWOL_FONT_H__
#define __EWOL_FONT_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <etk/File.h>
#include <draw/Image.h>
#include <ewol/texture/Texture.h>


namespace ewol
{
	// show : http://www.freetype.org/freetype2/docs/tutorial/step2.html
	typedef struct {
		uniChar_t        unicodeCharVal;
		texCoord_ts      posStart;
		texCoord_ts      posStop;
		Vector2D<float>  bearing;
		Vector2D<float>  size;
		int32_t          advance;
	}freeTypeFontElement_ts;
	
	class Font {
		private:
			etk::UString m_name;
			uint32_t     m_counter;
		public:
			Font(etk::UString fontName) :
				m_counter(1)
			{
				m_name = fontName;
			};
			virtual ~Font(void) {};
			bool HasName(etk::UString& fileName) { return fileName==m_name; };
			etk::UString GetName(void) { return m_name; };
			void Increment(void) { m_counter++; };
			bool Decrement(void) { m_counter--; return (m_counter==0)?true:false; };
			virtual int32_t Draw(draw::Image&         imageOut,
			                     int32_t              fontSize,
			                     Vector2D<float>      textPos,
			                     const etk::UString&  unicodeString,
			                     draw::Color&         textColor) = 0;
			virtual int32_t Draw(draw::Image&     imageOut,
			                     int32_t          fontSize,
			                     Vector2D<float>  textPos,
			                     const uniChar_t  unicodeChar,
			                     draw::Color&     textColor) = 0;
			virtual bool GenerateBitmapFont(int32_t size, int32_t& height, ewol::Texture& texture, etk::Vector<freeTypeFontElement_ts>& listElement) = 0;
			virtual Vector2D<float> GetSize(int32_t fontSize, const etk::UString &  unicodeString) = 0;
			virtual int32_t GetHeight(int32_t fontSize) = 0;
	};
};

#endif

