/**
 *******************************************************************************
 * @file ewol/oObject/Sprite.cpp
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

#include <ewol/oObject/Sprite.h>
#include <ewol/texture/Texture.h>
#include <ewol/texture/TextureManager.h>
#include <ewol/openGl.h>
#include <math.h>

#undef __class__
#define __class__	"Sprite"


ewol::Sprite::Sprite(etk::UString spriteName, float sizeX, float sizeY)
{
	m_name = spriteName;
	EWOL_VERBOSE("Create Sprite : \"" << m_name << "\"");
	m_resource = ewol::textureManager::ImageKeep(m_name, Vector2D<int32_t>(sizeX,sizeY));
}


ewol::Sprite::~Sprite(void)
{
	if (NULL != m_resource) {
		ewol::textureManager::ImageRelease(m_resource);
	}
}

void ewol::Sprite::Draw(void)
{
	if (m_coord.Size()<=0) {
		//EWOL_WARNING("Nothink to draw...");
		return;
	}
	if (NULL == m_resource) {
		EWOL_WARNING("Texture does not exist ...");
		return;
	}
	glEnable(GL_TEXTURE_2D);
	//EWOL_WARNING("Draw with texture : " << m_textureId << " ==> ogl=" << ewol::texture::GetGLID(m_textureId));
	glBindTexture(GL_TEXTURE_2D, m_resource->GetId());
	glEnableClientState( GL_VERTEX_ARRAY );                     // Enable Vertex Arrays
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );              // Enable Texture Coord Arrays
	glEnableClientState( GL_COLOR_ARRAY );                      // Enable Color Arrays
	glVertexPointer(   3, GL_FLOAT,         0, &m_coord[0] );
	glTexCoordPointer( 2, GL_FLOAT,         0, &m_coordTex[0] );
	glColorPointer(    4, GL_UNSIGNED_BYTE, 0, &m_coordColor[0] );
	glDrawArrays( GL_TRIANGLES, 0, m_coord.Size());
	//EWOL_DEBUG("request draw of " << m_coord.Size() << " elements");
	glDisableClientState( GL_COLOR_ARRAY );                     // Disable Color Arrays
	glDisableClientState( GL_VERTEX_ARRAY );                    // Disable Vertex Arrays
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );             // Disable Texture Coord Arrays
	glDisable(GL_TEXTURE_2D);
}

void ewol::Sprite::Clear(void)
{
	m_coord.Clear();
	m_coordTex.Clear();
	m_coordColor.Clear();
}

void ewol::Sprite::Element(Vector2D<float> pos, float size, float angle)
{
	draw::Color tmpColor(0xFFFFFFFF);
	Vector3D<float> pos2;
	pos2.x = pos.x;
	pos2.y = pos.y;
	pos2.z = 0.0;
	Element(pos2, size, angle, tmpColor);
}

void ewol::Sprite::Element(Vector3D<float> pos, float size, float angle)
{
	draw::Color tmpColor(0xFFFFFFFF);
	Element(pos, size, angle, tmpColor);
}

void ewol::Sprite::Element(Vector2D<float> pos, float size, float angle, draw::Color tmpColor)
{
	Vector3D<float> pos2;
	pos2.x = pos.x;
	pos2.y = pos.y;
	pos2.z = 0.0;
	Element(pos2, size, angle, tmpColor);
}


void ewol::Sprite::Element(Vector3D<float> pos, float size, float angle, draw::Color tmpColor)
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
	
	Vector3D<float> point = pos;
	float yyySin = sin(angle) * size;
	float xxxCos = cos(angle) * size;
	
	point.x = xxxCos + pos.x;
	point.y = yyySin + pos.y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(texB);
	m_coordColor.PushBack(tmpColor);
	
	point.x = yyySin + pos.x;
	point.y = -xxxCos + pos.y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(texC);
	m_coordColor.PushBack(tmpColor);
	
	point.x = -xxxCos + pos.x;
	point.y = -yyySin + pos.y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(texD);
	m_coordColor.PushBack(tmpColor);
	
	m_coord.PushBack(point);
	m_coordTex.PushBack(texD);
	m_coordColor.PushBack(tmpColor);
	
	point.x = -yyySin + pos.x;
	point.y = xxxCos + pos.y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(texA);
	m_coordColor.PushBack(tmpColor);
	
	point.x = xxxCos + pos.x;
	point.y = yyySin + pos.y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(texB);
	m_coordColor.PushBack(tmpColor);
}

