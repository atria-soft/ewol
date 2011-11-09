/**
 *******************************************************************************
 * @file ewolOObject2DText.cpp
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

#include <ewolOObject2DText.h>
#include <GL/gl.h>

#undef __class__
#define __class__	"ewol::OObject2DText"

ewol::OObject2DText::OObject2DText(float x, float y, etk::String FontName, int32_t size, fontMode_te mode, color_ts textColorFg, const char* utf8String)
{
	Text(x, y, FontName, size, mode, textColorFg, utf8String);
}

ewol::OObject2DText::~OObject2DText(void)
{
	
}

void ewol::OObject2DText::Draw(void)
{
	if (m_coord.Size()<=0) {
		EWOL_WARNING("Nothink to draw...");
		return;
	}
	
	glColor4f(m_textColorFg.red, m_textColorFg.green, m_textColorFg.blue, m_textColorFg.alpha);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_FontTextureId);
	glEnableClientState( GL_VERTEX_ARRAY );						// Enable Vertex Arrays
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );				// Enable Texture Coord Arrays
	glVertexPointer( 2, GL_FLOAT, 0, &m_coord[0] );
	glTexCoordPointer( 2, GL_FLOAT, 0, &m_coordTex[0] );
	glDrawArrays( GL_QUADS, 0, m_coord.Size());
	//EWOL_DEBUG("request draw of " << m_coord.Size() << " elements");
	glDisableClientState( GL_VERTEX_ARRAY );					// Disable Vertex Arrays
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );				// Disable Texture Coord Arrays
	glDisable(GL_TEXTURE_2D);
}

void ewol::OObject2DText::Text(float x, float y, etk::String FontName, int32_t size, fontMode_te mode, color_ts textColorFg, const char* utf8String)
{
	m_FontTextureId = 0;
	m_coord.Clear();
	m_coordTex.Clear();
	// get font Name : 
	if (FontName == "") {
		m_FontId = GetDefaultFontId();
	} else {
		EWOL_TODO("pas encore fait...");
		//m_FontId = GetFontIdWithName(FontName);
		return;
	}
	if (m_FontId == -1) {
		EWOL_ERROR("Can not find the font with the name : " << FontName);
	}
	EWOL_DEBUG("Font name : " << FontName << " id=" << m_FontId);
	m_textColorFg = textColorFg;
	coord2D_ts drawPosition;
	drawPosition.x = x;
	drawPosition.y = y;
	ewol::DrawText(m_FontId, drawPosition, utf8String, m_FontTextureId, m_coord, m_coordTex);
}

void ewol::OObject2DText::UpdateOrigin(float x, float y)
{
	for (int32_t iii=0; iii<m_coord.Size(); iii++) {
		m_coord[iii].x += x;
		m_coord[iii].y += y;
	}
}




