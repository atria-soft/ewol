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



ewol::OObject2DTextured::OObject2DTextured(etk::UString textureName)
{
	EWOL_VERBOSE("Create OObject textured : \"" << textureName << "\"");
	m_textureId = ewol::texture::Load(textureName);
}
ewol::OObject2DTextured::OObject2DTextured(etk::UString textureName, float sizeX, float sizeY)
{
	EWOL_VERBOSE("Create OObject textured : \"" << textureName << "\"");
	m_textureId = ewol::texture::Load(textureName, sizeX);
}


ewol::OObject2DTextured::~OObject2DTextured(void)
{
	if (-1 != m_textureId) {
		ewol::texture::UnLoad(m_textureId);
	}
}

void ewol::OObject2DTextured::Draw(void)
{
	if (m_coord.Size()<=0) {
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
	glEnableClientState( GL_VERTEX_ARRAY );                     // Enable Vertex Arrays
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );              // Enable Texture Coord Arrays
	glEnableClientState( GL_COLOR_ARRAY );                      // Enable Color Arrays
	glVertexPointer( 2, GL_FLOAT, 0, &m_coord[0] );
	glTexCoordPointer( 2, GL_FLOAT, 0, &m_coordTex[0] );
	glColorPointer(    4, GL_UNSIGNED_BYTE, 0, &m_coordColor[0] );
	glDrawArrays( GL_TRIANGLES, 0, m_coord.Size());
	//EWOL_DEBUG("request draw of " << m_coord.Size() << " elements");
	glDisableClientState( GL_COLOR_ARRAY );                     // Disable Color Arrays
	glDisableClientState( GL_VERTEX_ARRAY );                    // Disable Vertex Arrays
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );             // Disable Texture Coord Arrays
	glDisable(GL_TEXTURE_2D);
}

void ewol::OObject2DTextured::Clear(void)
{
	m_coord.Clear();
	m_coordTex.Clear();
	m_coordColor.Clear();
}

void ewol::OObject2DTextured::Rectangle(float x, float y, float w, float h, etk::Color tmpColor)
{
	Rectangle(x, y, w, h, 0.0, 0.0, 1.0, 1.0, tmpColor);
}

void ewol::OObject2DTextured::Rectangle(float x, float y, float w, float h, float texX, float texY, float texSX, float texSY, etk::Color tmpColor)
{
	//EWOL_DEBUG("Add rectangle : ...");
	Vector2D<float> point;
	texCoord_ts tex;

	tex.u = texX;
	tex.v = texSY;
	point.x = x;
	point.y = y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(tmpColor);


	tex.u = texSX;
	tex.v = texSY;
	point.x = x + w;
	point.y = y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(tmpColor);


	tex.u = texSX;
	tex.v = texY;
	point.x = x + w;
	point.y = y + h;
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(tmpColor);

	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(tmpColor);

	tex.u = texX;
	tex.v = texY;
	point.x = x;
	point.y = y + h;
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(tmpColor);

	tex.u = texX;
	tex.v = texSY;
	point.x = x;
	point.y = y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(tmpColor);
}
