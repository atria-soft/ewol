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
	m_triElement = 0;
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


void ewol::OObject2DColored::GenerateTriangle(void)
{
	m_triElement = 0;
	
	m_coord.PushBack(m_triangle[0]);
	m_coordColor.PushBack(m_color[0]);
	m_coord.PushBack(m_triangle[1]);
	m_coordColor.PushBack(m_color[1]);
	m_coord.PushBack(m_triangle[2]);
	m_coordColor.PushBack(m_color[2]);
}


void ewol::OObject2DColored::SetColor(color_ts color)
{
	if (m_triElement < 1) {
		m_color[0] = color;
	}
	if (m_triElement < 2) {
		m_color[1] = color;
	}
	if (m_triElement < 3) {
		m_color[2] = color;
	}
}


void ewol::OObject2DColored::SetColor(float red, float green, float blue, float alpha)
{
	if (m_triElement < 1) {
		m_color[0].red = red;
		m_color[0].green = green;
		m_color[0].blue = blue;
		m_color[0].alpha = alpha;
	}
	if (m_triElement < 2) {
		m_color[1].red = red;
		m_color[1].green = green;
		m_color[1].blue = blue;
		m_color[1].alpha = alpha;
	}
	if (m_triElement < 3) {
		m_color[2].red = red;
		m_color[2].green = green;
		m_color[2].blue = blue;
		m_color[2].alpha = alpha;
	}
	
}

void ewol::OObject2DColored::SetPoint(coord2D_ts point)
{
	m_triangle[m_triElement] = point;
	m_triElement++;
	if (m_triElement>=3) {
		GenerateTriangle();
	}
}

void ewol::OObject2DColored::SetPoint(float x, float y)
{
	m_triangle[m_triElement].x = x;
	m_triangle[m_triElement].y = y;
	m_triElement++;
	if (m_triElement>=3) {
		GenerateTriangle();
	}
}


void ewol::OObject2DColored::ResetCount(void)
{
	m_triElement = 0;
	m_color[1] = m_color[0];
	m_color[2] = m_color[0];
}

void ewol::OObject2DColored::Line(float sx, float sy, float ex, float ey, float thickness)
{
	ResetCount();
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

	SetPoint(sx - offsetx, sy - offsety);
	SetPoint(sx + offsetx, sy + offsety);
	SetPoint(ex + offsetx, ey + offsety);
	
	SetPoint(ex + offsetx, ey + offsety);
	SetPoint(ex - offsetx, ey - offsety);
	SetPoint(sx - offsetx, sy - offsety);
}


void ewol::OObject2DColored::Rectangle(float x, float y, float w, float h)
{
	ResetCount();

	SetPoint(x    , y + h);
	SetPoint(x    , y);
	SetPoint(x + w, y);

	SetPoint(x + w, y);
	SetPoint(x + w, y + h);
	SetPoint(x    , y + h);
}


void ewol::OObject2DColored::Circle(float x, float y, float radius, float thickness)
{
	ResetCount();
	if (radius<0) {
		radius *= -1;
	}
	if (radius < thickness/2) {
		Disc(x, y, thickness/2 + radius);
	}
	int32_t nbOcurence = radius*2;
	if (nbOcurence < 10)
	{
		nbOcurence = 10;
	}
	for (int32_t iii=0; iii<nbOcurence; iii++) {
		
		double angleOne = 2*M_PI* iii     / nbOcurence ;
		double offsetExty = sin(angleOne) * (radius+thickness/2);
		double offsetExtx = cos(angleOne) * (radius+thickness/2);
		double offsetInty = sin(angleOne) * (radius-thickness/2);
		double offsetIntx = cos(angleOne) * (radius-thickness/2);
		
		double angleTwo = 2*M_PI* (iii+1) / nbOcurence ;
		double offsetExt2y = sin(angleTwo) * (radius+thickness/2);
		double offsetExt2x = cos(angleTwo) * (radius+thickness/2);
		double offsetInt2y = sin(angleTwo) * (radius-thickness/2);
		double offsetInt2x = cos(angleTwo) * (radius-thickness/2);
		
		SetPoint(x + offsetIntx,  y + offsetInty);
		SetPoint(x + offsetExtx,  y + offsetExty);
		SetPoint(x + offsetExt2x, y + offsetExt2y);
		
		SetPoint(x + offsetExt2x, y + offsetExt2y);
		SetPoint(x + offsetInt2x, y + offsetInt2y);
		SetPoint(x + offsetIntx,  y + offsetInty);
	}
}

void ewol::OObject2DColored::Disc(float x, float y, float radius)
{
	ResetCount();
	coord2D_ts point;
	if (radius<0) {
		radius *= -1;
	}
	int32_t nbOcurence = radius*2;
	if (nbOcurence < 10)
	{
		nbOcurence = 10;
	}

	for (int32_t iii=0; iii<nbOcurence; iii++) {
		SetPoint(x, y);
		
		double angleOne = 2*M_PI* iii / nbOcurence ;
		double offsety = sin(angleOne) * radius;
		double offsetx = cos(angleOne) * radius;
		
		SetPoint(x + offsetx, y + offsety);
		
		double angleTwo = 2*M_PI* (iii+1) / nbOcurence ;
		offsety = sin(angleTwo) * radius;
		offsetx = cos(angleTwo) * radius;
		
		SetPoint(x + offsetx, y + offsety);
	}
}


