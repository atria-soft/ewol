/**
 *******************************************************************************
 * @file ewolOObject.cpp
 * @brief ewol OpenGl Object system (Sources)
 * @author Edouard DUPIN
 * @date 24/10/2011
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

#include <etkTypes.h>
#include <etkString.h>
#include <ewolOObject.h>
#include <ewolTexture.h>
#include <GL/gl.h>


#undef __class__
#define __class__	"ewol::OObject2DColored"



void ewol::OObject2DColored::Draw(void)
{
	if (m_coord.Size()<=0) {
		return;
	}
	// Enable Pointers
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );

	
	// Set the vertex pointer to our vertex data
	glVertexPointer(2, GL_FLOAT, 0, &m_coord[0] );
	glColorPointer(4, GL_FLOAT, 0, &m_coordColor[0] );
	// Render : draw all of the triangles at once
	glDrawArrays( GL_TRIANGLES, 0, m_coord.Size());
	//glDrawElements( GL_TRIANGLES, 0, m_coord.Size());
	//EWOL_DEBUG("Draw ..." << m_coord.Size()/3 << " triangle(s)");

	// Disable Pointers
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );

}


void ewol::OObject2DColored::Rectangle(float x, float y, float w, float h, float red, float green, float blue, float alpha)
{
	//EWOL_DEBUG("Add rectangle : ...");
	coord2D_ts point;
	color_ts color;

	color.red   = red;
	color.green = green;
	color.blue  = blue;
	color.alpha = alpha;

	point.x = x;
	point.y = y + h;
	m_coord.PushBack(point);
	m_coordColor.PushBack(color);

	point.x = x;
	point.y = y;
	m_coord.PushBack(point);
	m_coordColor.PushBack(color);

	point.x = x + w;
	point.y = y;
	m_coord.PushBack(point);
	m_coordColor.PushBack(color);

	m_coord.PushBack(point);
	m_coordColor.PushBack(color);

	point.x = x + w;
	point.y = y + h;
	m_coord.PushBack(point);
	m_coordColor.PushBack(color);

	point.x = x;
	point.y = y + h;
	m_coord.PushBack(point);
	m_coordColor.PushBack(color);
	
}




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
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glEnableClientState( GL_VERTEX_ARRAY );						// Enable Vertex Arrays
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );				// Enable Texture Coord Arrays
	glVertexPointer( 2, GL_FLOAT, 0, &m_coord[0] );
	glTexCoordPointer( 2, GL_FLOAT, 0, &m_coordTex[0] );
	glDrawArrays( GL_TRIANGLES, 0, m_coord.Size());
	//EWOL_DEBUG("request draw of " << m_coord.Size() << " elements");
	glDisableClientState( GL_VERTEX_ARRAY );					// Disable Vertex Arrays
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );				// Disable Texture Coord Arrays
	glDisable(GL_TEXTURE_2D);
}

void ewol::OObject2DTextured::Rectangle(float x, float y, float w, float h, float texX, float texY, float texSX, float texSY)
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


