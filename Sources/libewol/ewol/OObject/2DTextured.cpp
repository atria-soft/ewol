/**
 *******************************************************************************
 * @file ewol/OObject/2DTextured.cpp
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

#include <ewol/OObject/2DTextured.h>
#include <ewol/Texture.h>
#include <ewol/importgl.h>

#undef __class__
#define __class__	"ewol::OObject2DTextured"



ewol::OObject2DTextured::OObject2DTextured(etk::File textureName)
{
	EWOL_DEBUG("Create OObject textured : \"" << textureName << "\"");
	m_textureId = ewol::LoadTexture(textureName);
}

ewol::OObject2DTextured::~OObject2DTextured(void)
{
	ewol::UnLoadTexture(m_textureId);
}

void ewol::OObject2DTextured::Draw(void)
{
	if (m_coord.Size()<=0) {
		EWOL_WARNING("Nothink to draw...");
		return;
	}
	
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ewol::GetTextureGLID(m_textureId));
	glEnableClientState( GL_VERTEX_ARRAY );						// Enable Vertex Arrays
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );				// Enable Texture Coord Arrays
	glVertexPointer( 2, oglTypeFloat_t, 0, &m_coord[0] );
	glTexCoordPointer( 2, oglTypeFloat_t, 0, &m_coordTex[0] );
	glDrawArrays( GL_TRIANGLES, 0, m_coord.Size());
	//EWOL_DEBUG("request draw of " << m_coord.Size() << " elements");
	glDisableClientState( GL_VERTEX_ARRAY );					// Disable Vertex Arrays
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );				// Disable Texture Coord Arrays
	glDisable(GL_TEXTURE_2D);
}

void ewol::OObject2DTextured::Rectangle(etkFloat_t x, etkFloat_t y, etkFloat_t w, etkFloat_t h, etkFloat_t texX, etkFloat_t texY, etkFloat_t texSX, etkFloat_t texSY)
{
	//EWOL_DEBUG("Add rectangle : ...");
	coord2D_ts point;
	texCoord_ts tex;

	tex.u = texX;
	tex.v = texY;
	point.x = x;
	point.y = y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);


	tex.u = texSX;
	tex.v = texY;
	point.x = x + w;
	point.y = y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);


	tex.u = texSX;
	tex.v = texSY;
	point.x = x + w;
	point.y = y + h;
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);

	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);

	tex.u = texX;
	tex.v = texSY;
	point.x = x;
	point.y = y + h;
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);

	tex.u = texX;
	tex.v = texY;
	point.x = x;
	point.y = y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
}
