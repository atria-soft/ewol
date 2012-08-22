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
#include <ewol/openGl.h>
#include <ewol/texture/Texture.h>
#include <ewol/font/FontManager.h>

#undef __class__
#define __class__	"ewol::OObject2DTextColored"

void ewol::OObject2DTextColored::SetFontProperty(etk::UString fontName, int32_t fontSize)
{
	// remove old one
	if (NULL != m_font) {
		ewol::font::TexturedRelease(m_font);
		m_font = NULL;
	}
	// link to new One
	m_font = ewol::font::TexturedKeep(fontName, fontSize);
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
		fontName = m_font->GetFontName();
	}
	SetFontProperty(fontName, fontSize);
}

ewol::OObject2DTextColored::OObject2DTextColored(etk::UString fontName, int32_t size) :
	m_font(NULL)
{
	m_color = draw::color::black;
	SetFontProperty(fontName, size);
}


// open with default font ...
ewol::OObject2DTextColored::OObject2DTextColored(void) :
	m_font(NULL)
{
	m_color = draw::color::black;
	SetFontProperty(ewol::font::GetDefaultFont(), ewol::font::GetDefaultSize());
}


ewol::OObject2DTextColored::~OObject2DTextColored(void)
{
	if (NULL != m_font) {
		ewol::font::TexturedRelease(m_font);
		m_font = NULL;
	}
}

void ewol::OObject2DTextColored::Draw(void)
{
	if (m_coord.Size()<=0 || NULL == m_font) {
		// TODO : a remètre ...
		//EWOL_WARNING("Nothink to draw...");
		return;
	}
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_font->GetTex().GetId());
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
	if (true==m_hasClipping) {
		size = m_font->Draw(textPos, unicodeString, m_coord, m_coordTex);
	} else {
		size = m_font->Draw(textPos, unicodeString, m_coord, m_coordTex);
	}
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
	if (true==m_hasClipping) {
		size = m_font->Draw(textPos, unicodeChar, m_coord, m_coordTex);
	} else {
		size = m_font->Draw(textPos, unicodeChar, m_coord, m_coordTex);
	}
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

