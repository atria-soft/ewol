/**
 *******************************************************************************
 * @file ewol/oObject/2DTextColored.cpp
 * @brief ewol OpenGl Object system (Sources)
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

#include <ewol/oObject/2DTextColored.h>
#include <ewol/openGL/openGL.h>
#include <ewol/texture/Texture.h>
#include <ewol/font/FontManager.h>
#include <ewol/ResourceManager.h>

#undef __class__
#define __class__	"ewol::OObject2DTextColored"

void ewol::OObject2DTextColored::SetFontProperty(etk::UString fontName, int32_t fontSize)
{
	// remove old one
	if (NULL != m_font) {
		ewol::resource::Release(m_font);
		m_font = NULL;
	}
	etk::UString tmpName = fontName;
	tmpName += ":";
	tmpName += fontSize;
	// link to new One
	if (false == ewol::resource::Keep(tmpName, m_font)) {
		EWOL_ERROR("Can not get font resource");
	}
}

void ewol::OObject2DTextColored::SetFont(etk::UString fontName)
{
	// get old size
	int32_t fontSize = ewol::font::GetDefaultSize();
	if (NULL != m_font) {
		fontSize = m_font->GetFontSize();
	}
	SetFontProperty(fontName, fontSize);
}

void ewol::OObject2DTextColored::SetSize(int32_t fontSize)
{
	// get old size
	etk::UString fontName = ewol::font::GetDefaultFont();
	if (NULL != m_font) {
		fontName = m_font->GetName();
	}
	SetFontProperty(fontName, fontSize);
}

ewol::OObject2DTextColored::OObject2DTextColored(etk::UString fontName, int32_t size) :
	m_font(NULL)
{
	m_color = draw::color::black;
	SetFontProperty(fontName, size);
	#ifdef __VIDEO__OPENGL_ES_2
		etk::UString tmpString("textured.prog");
		// get the shader resource :
		m_GLPosition = 0;
		if (true == ewol::resource::Keep(tmpString, m_GLprogram) ) {
			m_GLPosition = m_GLprogram->GetAttribute("EW_coord2d");
			m_GLColor    = m_GLprogram->GetAttribute("EW_color");
			m_GLtexture  = m_GLprogram->GetAttribute("EW_texture2d");
			m_GLMatrix   = m_GLprogram->GetUniform("EW_MatrixTransformation");
			m_GLtexID    = m_GLprogram->GetUniform("EW_texID");
		}
	#endif
}


// open with default font ...
ewol::OObject2DTextColored::OObject2DTextColored(void) :
	m_font(NULL)
{
	m_color = draw::color::black;
	SetFontProperty(ewol::font::GetDefaultFont(), ewol::font::GetDefaultSize());
	etk::UString tmpString("textured.prog");
	#ifdef __VIDEO__OPENGL_ES_2
		// get the shader resource :
		m_GLPosition = 0;
		if (true == ewol::resource::Keep(tmpString, m_GLprogram) ) {
			m_GLPosition = m_GLprogram->GetAttribute("EW_coord2d");
			m_GLColor    = m_GLprogram->GetAttribute("EW_color");
			m_GLtexture  = m_GLprogram->GetAttribute("EW_texture2d");
			m_GLMatrix   = m_GLprogram->GetUniform("EW_MatrixTransformation");
			m_GLtexID    = m_GLprogram->GetUniform("EW_texID");
		}
	#endif
}


ewol::OObject2DTextColored::~OObject2DTextColored(void)
{
	if (NULL != m_font) {
		ewol::resource::Release(m_font);
		m_font = NULL;
	}
	#ifdef __VIDEO__OPENGL_ES_2
		ewol::resource::Release(m_GLprogram);
	#endif
}

void ewol::OObject2DTextColored::Draw(void)
{
	if (m_coord.Size()<=0 || NULL == m_font) {
		// TODO : a remètre ...
		//EWOL_WARNING("Nothink to draw...");
		return;
	}
	if (m_font == NULL) {
		EWOL_WARNING("no font...");
		return;
	}
	#ifdef __VIDEO__OPENGL_ES_2
		if (m_GLprogram==NULL) {
			EWOL_ERROR("No shader ...");
			return;
		}
		m_GLprogram->Use();
		// set Matrix : translation/positionMatrix
		etk::Matrix tmpMatrix = ewol::openGL::GetMatrix();
		m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
		// TextureID
		m_GLprogram->SetTexture0(m_GLtexID, m_font->GetId());
		// position :
		m_GLprogram->SendAttribute(m_GLPosition, 2/*x,y*/, &m_coord[0]);
		// Texture :
		m_GLprogram->SendAttribute(m_GLtexture, 2/*u,v*/, &m_coordTex[0]);
		// color :
		m_GLprogram->SendAttribute(m_GLColor, 4/*r,g,b,a*/, &m_coordColor[0]);
		// Request the draw od the elements : 
		glDrawArrays(GL_TRIANGLES, 0, m_coord.Size());
		m_GLprogram->UnUse();
	#else
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_font->GetId());
		glEnableClientState( GL_VERTEX_ARRAY );                     // Enable Vertex Arrays
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );              // Enable Texture Coord Arrays
		glEnableClientState( GL_COLOR_ARRAY );                      // Enable Color Arrays
		glVertexPointer(   2, GL_FLOAT, 0, &m_coord[0] );
		glTexCoordPointer( 2, GL_FLOAT, 0, &m_coordTex[0] );
		glColorPointer(    4, GL_UNSIGNED_BYTE, 0, &m_coordColor[0] );
		glDrawArrays( GL_TRIANGLES, 0, m_coord.Size());
		//EWOL_DEBUG("request draw of " << m_coord.Size() << " elements");
		glDisableClientState( GL_COLOR_ARRAY );                     // Disable Color Arrays
		glDisableClientState( GL_VERTEX_ARRAY );                    // Disable Vertex Arrays
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );             // Disable Texture Coord Arrays
		glDisable(GL_TEXTURE_2D);
	#endif
}

void ewol::OObject2DTextColored::Clear(void)
{
	m_coord.Clear();
	m_coordTex.Clear();
	m_coordColor.Clear();
}

int32_t ewol::OObject2DTextColored::Text(Vector2D<float> textPos, const etk::UString& unicodeString)
{
	if (m_font == NULL) {
		EWOL_ERROR("Font Id is not corectly defined");
		return 0;
	}
	int32_t nbElementInTheArray = m_coord.Size();
	int32_t size = 0;
	size = m_font->Draw(textPos, unicodeString, m_coord, m_coordTex, m_hasClipping, m_clipping);
	// set the color ...
	for (int32_t iii=nbElementInTheArray; iii<m_coord.Size(); iii++) {
		m_coordColor.PushBack(m_color);
	}
	return size;
}

int32_t ewol::OObject2DTextColored::Text(Vector2D<float> textPos, const uniChar_t unicodeChar)
{
	if (m_font == NULL) {
		EWOL_ERROR("Font Id is not corectly defined");
		return 0;
	}
	int32_t nbElementInTheArray = m_coord.Size();
	int32_t size = 0;
	size = m_font->Draw(textPos, unicodeChar, m_coord, m_coordTex, m_hasClipping, m_clipping);
	for (int32_t iii=nbElementInTheArray; iii<m_coord.Size(); iii++) {
		m_coordColor.PushBack(m_color);
	}
	return size;
}


void ewol::OObject2DTextColored::SetColor(draw::Color color)
{
	m_color = color;
}


void ewol::OObject2DTextColored::SetColor(float red, float green, float blue, float alpha)
{
	m_color = draw::Color(red, green, blue, alpha);
}

Vector2D<float> ewol::OObject2DTextColored::GetSize(const uniChar_t unicodeChar)
{
	if (m_font == NULL) {
		EWOL_ERROR("Font Id is not corectly defined");
		return Vector2D<float>(0,0);
	}
	return m_font->GetSize(unicodeChar);
}

Vector2D<float> ewol::OObject2DTextColored::GetSize(const etk::UString& unicodeString)
{
	if (m_font == NULL) {
		EWOL_ERROR("Font Id is not corectly defined");
		return Vector2D<float>(0,0);
	}
	return m_font->GetSize(unicodeString);
}

