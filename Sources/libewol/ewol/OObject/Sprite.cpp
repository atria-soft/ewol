/**
 *******************************************************************************
 * @file ewol/OObject/Sprite.cpp
 * @brief ewol OpenGl Object system (Sources)
 * @author Edouard DUPIN
 * @date 04/04/2012
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

#include <ewol/OObject/Sprite.h>
#include <ewol/Texture.h>
#include <ewol/importgl.h>
#include <math.h>

#undef __class__
#define __class__	"Sprite"



ewol::Sprite::Sprite(etk::UString spriteName)
{
	m_name = spriteName;
	EWOL_VERBOSE("Create Sprite : \"" << m_name << "\"");
	m_textureId = ewol::texture::Load(m_name);
}
ewol::Sprite::Sprite(etk::UString spriteName, etkFloat_t sizeX, etkFloat_t sizeY)
{
	m_name = spriteName;
	EWOL_VERBOSE("Create Sprite : \"" << m_name << "\"");
	m_textureId = ewol::texture::Load(m_name, sizeX);
}


ewol::Sprite::~Sprite(void)
{
	if (-1 != m_textureId) {
		ewol::texture::UnLoad(m_textureId);
	}
}

void ewol::Sprite::Draw(void)
{
	if (m_coord.Size()<=0) {
		//EWOL_WARNING("Nothink to draw...");
		return;
	}
	if (m_textureId == -1) {
		EWOL_WARNING("Texture does not exist ...");
		return;
	}
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	//EWOL_WARNING("Draw with texture : " << m_textureId << " ==> ogl=" << ewol::texture::GetGLID(m_textureId));
	glBindTexture(GL_TEXTURE_2D, ewol::texture::GetGLID(m_textureId));
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

void ewol::Sprite::Clear(void)
{
	m_coord.Clear();
	m_coordTex.Clear();
}

void ewol::Sprite::Element(coord2D_ts pos, etkFloat_t size, etkFloat_t angle)
{
	angle -= M_PI/4;
	size *= 0.7;
	texCoord_ts texA, texB, texC, texD;
	texA.u = 0.0;
	texA.v = 0.0;
	texB.u = 0.0;
	texB.v = 1.0;
	texC.u = 1.0;
	texC.v = 1.0;
	texD.u = 1.0;
	texD.v = 0.0;
	
	coord2D_ts point;
	etkFloat_t yyySin = sin(angle) * size;
	etkFloat_t xxxCos = cos(angle) * size;
	
	point.x = xxxCos + pos.x;
	point.y = yyySin + pos.y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(texB);
	
	point.x = yyySin + pos.x;
	point.y = -xxxCos + pos.y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(texC);
	
	point.x = -xxxCos + pos.x;
	point.y = -yyySin + pos.y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(texD);
	
	m_coord.PushBack(point);
	m_coordTex.PushBack(texD);
	
	point.x = -yyySin + pos.x;
	point.y = xxxCos + pos.y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(texA);
	
	point.x = xxxCos + pos.x;
	point.y = yyySin + pos.y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(texB);
}

