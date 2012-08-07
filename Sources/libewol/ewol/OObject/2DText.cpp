/**
 *******************************************************************************
 * @file ewol/OObject/2DText.cpp
 * @brief ewol OpenGl Object system (Sources)
 * @author Edouard DUPIN
 * @date 09/11/2011
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

#include <ewol/OObject/2DText.h>
#include <ewol/importgl.h>
#include <ewol/Texture.h>

#undef __class__
#define __class__	"ewol::OObject2DText"

ewol::OObject2DText::OObject2DText(etk::UString FontName, int32_t size, color_ts textColorFg)
{
	m_textColorFg = textColorFg;
	if (FontName == "") {
		m_FontId = GetDefaultFontId();
	} else {
		EWOL_TODO("pas encore fait...");
		//m_FontId = GetFontIdWithName(FontName);
		m_FontId = -1;
		return;
	}
}
// open with default font ...
ewol::OObject2DText::OObject2DText(void)
{
	m_textColorFg.red = 0;
	m_textColorFg.green = 0;
	m_textColorFg.blue = 0;
	m_textColorFg.alpha = 0xFF;
	m_FontId = GetDefaultFontId();
}

ewol::OObject2DText::~OObject2DText(void)
{
	
}

void ewol::OObject2DText::Draw(void)
{
	if (m_coord.size()<=0) {
		// TODO : a remètre ...
		//EWOL_WARNING("Nothink to draw...");
		return;
	}
	glColor4ub(m_textColorFg.red, m_textColorFg.green, m_textColorFg.blue, m_textColorFg.alpha);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ewol::texture::GetGLID(m_FontTextureId));
	glEnableClientState( GL_VERTEX_ARRAY );						// Enable Vertex Arrays
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );				// Enable Texture Coord Arrays
	glVertexPointer( 2, GL_FLOAT, 0, &m_coord[0] );
	glTexCoordPointer( 2, GL_FLOAT, 0, &m_coordTex[0] );
	glDrawArrays( GL_TRIANGLES, 0, m_coord.size());
	//EWOL_DEBUG("request draw of " << m_coord.Size() << " elements");
	glDisableClientState( GL_VERTEX_ARRAY );					// Disable Vertex Arrays
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );				// Disable Texture Coord Arrays
	glDisable(GL_TEXTURE_2D);
}

void ewol::OObject2DText::Clear(void)
{
	m_coord.clear();
	m_coordTex.clear();
}

int32_t ewol::OObject2DText::Text(Vector2D<float> textPos, clipping_ts drawClipping, const etk::UString& unicodeString)
{
	m_FontTextureId = 0;
	if (m_FontId == -1) {
		EWOL_ERROR("Font Id is not corectly defined");
		return 0;
	}
	return ewol::DrawText(m_FontId, textPos, drawClipping, unicodeString, m_FontTextureId, m_coord, m_coordTex);
}

int32_t ewol::OObject2DText::Text(Vector2D<float> textPos, clipping_ts drawClipping, const uniChar_t unicodeChar)
{
	m_FontTextureId = 0;
	if (m_FontId == -1) {
		EWOL_ERROR("Font Id is not corectly defined");
		return 0;
	}
	return ewol::DrawText(m_FontId, textPos, drawClipping, unicodeChar, m_FontTextureId, m_coord, m_coordTex);
}




