/**
 *******************************************************************************
 * @file ewol/oObject/2DTextColored.h
 * @brief ewol OpenGl Object system (header)
 * @author Edouard DUPIN
 * @date 16/01/2012
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

#ifndef __EWOL_O_OBJECT_2D_TEXT_SHADER_H__
#define __EWOL_O_OBJECT_2D_TEXT_SHADER_H__

#ifdef __VIDEO__OPENGL_ES_2

#include <ewol/oObject/OObject.h>
#include <ewol/font/DistantFieldFont.h>
#include <ewol/ResourceManager.h>

namespace ewol {
	class OObject2DTextShader :public ewol::OObject
	{
		public:
			OObject2DTextShader(etk::UString FontName, int32_t size);
			OObject2DTextShader(void);
			virtual ~OObject2DTextShader(void);
		public:
			virtual void Draw(void);
			void SetColor(float red, float green, float blue, float alpha = 1.0);
			void SetColor(draw::Color color);
			// set a specific text
			void    Clear(void);
			int32_t Text(etk::Vector2D<float>  textPos, const etk::UString& unicodeString);
			int32_t Text(etk::Vector2D<float>  textPos, const uniChar_t     unicodeChar);
		protected:
			ewol::Program* m_GLprogram;
			int32_t        m_GLPosition;
			int32_t        m_GLMatrix;
			int32_t        m_GLColor;
			int32_t        m_GLtexture;
			int32_t        m_GLtexID;
			int32_t        m_GLSoftEdgeMin;
			int32_t        m_GLSoftEdgeMax;
			int32_t        m_GLSoftEdge;
			ewol::DistantFieldFont*         m_font;          //!< ewol font system
			draw::Color                     m_color;         //!< tmp text color ...
			etk::Vector<etk::Vector2D<float> >   m_coord;         //!< internal coord of the object
			etk::Vector<texCoord_ts>        m_coordTex;      //!< internal texture coordinate for every point
			etk::Vector<draw::Colorf>       m_coordColor;    //!< internal color of the different point
			bool           m_bold;
			bool           m_italic;
		public:
			void SetBold(bool newVal) { m_bold=newVal; };
			void SetItalic(bool newVal) { m_italic=newVal; };
			void SetFont(etk::UString fontName);
			void SetSize(int32_t fontSize);
			void SetFontProperty(etk::UString fontName, int32_t fontSize);
			int32_t GetHeight(void) { return (NULL!=m_font)?m_font->GetHeight():10; };
			etk::Vector2D<float> GetSize(const uniChar_t unicodeChar);
			etk::Vector2D<float> GetSize(const etk::UString& unicodeString);
	};
};

#endif

#endif
