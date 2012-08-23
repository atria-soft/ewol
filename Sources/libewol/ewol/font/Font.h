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
	/*
	                             |            |          |            |            
	                             |            |          |            |            
	                             |            |          |            |            
	                      Y      |            |          |            |            
	                      ^      |------------|          |------------|            
	                      |                                                        
	     m_advance.y:/->  |                                                        
	                 |    |                                                        
	                 |    |                                                        
	m_sizeTex.x/->   |    |         |------------|          |------------|         
	           |     |    |         |            |          |            |         
	           |     |    |         |            |          |            |         
	           |     |    |         |            |          |            |         
	           |     |    |         |            |          |            |         
	           |     |    |         |     A      |          |     G      |         
	           |     |    |         |            |          |            |         
	           |     |    |         |            |          |            |         
	           |     |    |         |            |          |            |         
	           |     |    |         |            |          |            |         
	           \->   |    |         |------------|          |------------|         
	        /-->     |    |                                                        
	        \-->     \->  |                                                        
	m_bearing.y           |                                                        
	                      |____*________________________*____________>>   X        
	                                                                               
	                                                                               
	                           <------------------------> : m_advance.x            
	                                                                               
	                                <------------> : m_sizeTexture.x               
	                                                                               
	                           <---> : m_bearing.x                                 
	                       
	*/
	typedef struct {
		uniChar_t         m_UVal;        // Unicode value
		int32_t           m_glyphIndex;  // Glyph index in the system
		Vector2D<int32_t> m_sizeTexture; // size of the element to display
		Vector2D<int32_t> m_bearing;     // offset to display the data (can be negatif id the texture sise is bigger than the théoric places in the string)
		Vector2D<int32_t> m_advance;     // space use in the display for this specific char
	} GlyphProperty;
	
	class Font {
		private:
			etk::UString m_name;
			uint32_t     m_counter;
		public:
			Font(etk::UString fontFolder, etk::UString fontName) :
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
			virtual bool GetGlyphProperty(int32_t              fontSize,
			                              ewol::GlyphProperty& property) = 0;
			virtual bool DrawGlyph(draw::Image&         imageOut,
			                       int32_t              fontSize,
			                       Vector2D<int32_t>    glyphPosition,
			                       ewol::GlyphProperty& property) = 0;
			virtual Vector2D<float> GetSize(int32_t fontSize, const etk::UString &  unicodeString) = 0;
			virtual int32_t GetHeight(int32_t fontSize) = 0;
	};
};

#endif

