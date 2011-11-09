/**
 *******************************************************************************
 * @file ewolOObject2DColored.cpp
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

#include <ewolOObject2DColored.h>
#include <GL/gl.h>
#include <math.h>


#undef __class__
#define __class__	"ewol::OObject2DColored"


ewol::OObject2DColored::OObject2DColored(void)
{
	SetColor(1.0, 1.0, 1.0, 1.0);
}


ewol::OObject2DColored::~OObject2DColored(void)
{
	m_coord.Clear();
	m_coordColor.Clear();
}


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

void ewol::OObject2DColored::UpdateOrigin(float x, float y)
{
	for (int32_t iii=0; iii<m_coord.Size(); iii++) {
		m_coord[iii].x += x;
		m_coord[iii].y += y;
	}
}



void ewol::OObject2DColored::SetColor( float red, float green, float blue, float alpha)
{
	m_Color.red = red;
	m_Color.green = green;
	m_Color.blue = blue;
	m_Color.alpha = alpha;
}


void ewol::OObject2DColored::Line(float sx, float sy, float ex, float ey, float thickness)
{
	if (sx == ex && sy == ey) {
		EWOL_WARNING("Try to draw an line width 0");
		return;
	}
	//teta = tan-1(oposer/adjacent)
	double teta = 0;
	if (sx <= ex) {
		teta = atan((ey-sy)/(ex-sx));
	} else {
		teta = M_PI + atan((ey-sy)/(ex-sx));
	}
	if (teta < 0) {
		teta += 2*M_PI;
	} else if (teta > 2*M_PI) {
		teta -= 2*M_PI;
	}
	//EWOL_DEBUG("teta = " << (teta*180/(M_PI)) << " deg." );
	double offsety = sin(teta-M_PI/2) * (thickness/2);
	double offsetx = cos(teta-M_PI/2) * (thickness/2);
	// just for debug ...
	/*if (offsetx <= 0.001 && offsetx >= -0.001) {
		offsetx = 0;
	}
	if (offsety <= 0.001 && offsety >= -0.001) {
		offsety = 0;
	}
	EWOL_DEBUG("ofset (" << offsetx << "," << offsety << ")");
	*/
	coord2D_ts point;

	point.x = sx - offsetx;
	point.y = sy - offsety;
	m_coord.PushBack(point);
	m_coordColor.PushBack(m_Color);
	
	point.x = sx + offsetx;
	point.y = sy + offsety;
	m_coord.PushBack(point);
	m_coordColor.PushBack(m_Color);
	
	point.x = ex + offsetx;
	point.y = ey + offsety;
	m_coord.PushBack(point);
	m_coordColor.PushBack(m_Color);

	m_coord.PushBack(point);
	m_coordColor.PushBack(m_Color);
	
	point.x = ex - offsetx;
	point.y = ey - offsety;
	m_coord.PushBack(point);
	m_coordColor.PushBack(m_Color);
	
	point.x = sx - offsetx;
	point.y = sy - offsety;
	m_coord.PushBack(point);
	m_coordColor.PushBack(m_Color);
}


void ewol::OObject2DColored::Rectangle(float x, float y, float w, float h)
{
	coord2D_ts point;

	point.x = x;
	point.y = y + h;
	m_coord.PushBack(point);
	m_coordColor.PushBack(m_Color);
	
	point.x = x;
	point.y = y;
	m_coord.PushBack(point);
	m_coordColor.PushBack(m_Color);
	
	point.x = x + w;
	point.y = y;
	m_coord.PushBack(point);
	m_coordColor.PushBack(m_Color);

	m_coord.PushBack(point);
	m_coordColor.PushBack(m_Color);
	
	point.x = x + w;
	point.y = y + h;
	m_coord.PushBack(point);
	m_coordColor.PushBack(m_Color);
	
	point.x = x;
	point.y = y + h;
	m_coord.PushBack(point);
	m_coordColor.PushBack(m_Color);
}


void ewol::OObject2DColored::Circle(float x, float y, float radius, float thickness)
{
	
}

void ewol::OObject2DColored::Disc(float x, float y, float radius)
{
	coord2D_ts point;
	if (radius<0) {
		radius *= -1;
	}
	int32_t nbOcurence = radius*5;

	for (int32_t iii=0; iii<nbOcurence; iii++) {
		point.x = x;
		point.y = y;
		m_coord.PushBack(point);
		m_coordColor.PushBack(m_Color);
		
		double angleOne = 2*M_PI* iii / nbOcurence ;
		double offsety = sin(angleOne) * radius;
		double offsetx = cos(angleOne) * radius;
		
		point.x = x + offsetx;
		point.y = y + offsety;
		m_coord.PushBack(point);
		m_coordColor.PushBack(m_Color);
		
		double angleTwo = 2*M_PI* (iii+1) / nbOcurence ;
		offsety = sin(angleTwo) * radius;
		offsetx = cos(angleTwo) * radius;
		
		point.x = x + offsetx;
		point.y = y + offsety;
		m_coord.PushBack(point);
		m_coordColor.PushBack(m_Color);
	}
}


