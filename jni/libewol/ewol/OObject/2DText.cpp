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

#undef __class__
#define __class__	"ewol::OObject2DText"

ewol::OObject2DText::OObject2DText(etk::String FontName, int32_t size, color_ts textColorFg)
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
	m_textColorFg.red = 0.0;
	m_textColorFg.green = 0.0;
	m_textColorFg.blue = 0.0;
	m_textColorFg.alpha = 1.0;
	m_FontId = GetDefaultFontId();
}

ewol::OObject2DText::~OObject2DText(void)
{
	
}

void ewol::OObject2DText::Draw(void)
{
	if (m_coord.Size()<=0) {
		// TODO : a remètre ...
		//EWOL_WARNING("Nothink to draw...");
		return;
	}
	glColor4f(m_textColorFg.red, m_textColorFg.green, m_textColorFg.blue, m_textColorFg.alpha);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_FontTextureId);
	glEnableClientState( GL_VERTEX_ARRAY );						// Enable Vertex Arrays
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );				// Enable Texture Coord Arrays
	glVertexPointer( 2, oglTypeFloat_t, 0, &m_coord[0] );
	glTexCoordPointer( 2, oglTypeFloat_t, 0, &m_coordTex[0] );
	#if !defined(__PLATFORM__Android)
		glDrawArrays( GL_QUADS, 0, m_coord.Size());
	#else
		// NOTE : Android does not support the Quads elements ...
		glDrawArrays( GL_TRIANGLES, 0, m_coord.Size());
	#endif
	//EWOL_DEBUG("request draw of " << m_coord.Size() << " elements");
	glDisableClientState( GL_VERTEX_ARRAY );					// Disable Vertex Arrays
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );				// Disable Texture Coord Arrays
	glDisable(GL_TEXTURE_2D);
}

void ewol::OObject2DText::Text(etkFloat_t x, etkFloat_t y, const char* utf8String)
{
	m_FontTextureId = 0;
	m_coord.Clear();
	m_coordTex.Clear();
	if (m_FontId == -1) {
		EWOL_ERROR("Font Id is not corectly defined");
	}
	coord2D_ts drawPosition;
	drawPosition.x = x;
	drawPosition.y = y;
	ewol::DrawText(m_FontId, drawPosition, utf8String, m_FontTextureId, m_coord, m_coordTex);
}

void ewol::OObject2DText::UpdateOrigin(etkFloat_t x, etkFloat_t y)
{
	for (int32_t iii=0; iii<m_coord.Size(); iii++) {
		m_coord[iii].x += x;
		m_coord[iii].y += y;
	}
}




