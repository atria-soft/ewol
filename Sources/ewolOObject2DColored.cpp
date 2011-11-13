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
	glVertexPointer(2, oglTypeFloat_t, 0, &m_coord[0] );
	glColorPointer(4, oglTypeFloat_t, 0, &m_coordColor[0] );
	// Render : draw all of the triangles at once
	glDrawArrays( GL_TRIANGLES, 0, m_coord.Size());
	//glDrawElements( GL_TRIANGLES, 0, m_coord.Size());
	//EWOL_DEBUG("Draw ..." << m_coord.Size()/3 << " triangle(s)");

	// Disable Pointers
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );

}


void ewol::OObject2DColored::UpdateOrigin(etkFloat_t x, etkFloat_t y)
{
	for (int32_t iii=0; iii<m_coord.Size(); iii++) {
		m_coord[iii].x += x;
		m_coord[iii].y += y;
	}
}

void generatePolyGone(etk::VectorType<coord2D_ts> & input, etk::VectorType<coord2D_ts> & output )
{
	if (input.Size()<3) {
		return;
	}
	coord2D_ts basePoint = input[0];
	for (int32_t iii=1; iii<input.Size()-1; iii++) {
		output.PushBack(basePoint);
		output.PushBack(input[iii]);
		output.PushBack(input[iii+1]);
	}
	//EWOL_DEBUG("generate Plygone : " << input.Size() << " ==> " << output.Size() );
}

void SutherlandHodgman(etk::VectorType<coord2D_ts> & input, etk::VectorType<coord2D_ts> & output, etkFloat_t sx, etkFloat_t sy, etkFloat_t ex, etkFloat_t ey)
{
	// with Sutherland-Hodgman-Algorithm
	if (input.Size() <0) {
		return;
	}
	int32_t sizeInit=input.Size();
	// last element :
	coord2D_ts destPoint;
	coord2D_ts lastElement = input[input.Size()-1];
	bool inside = true;
	if (lastElement.x < sx) {
		inside = false;
	}
	//EWOL_DEBUG("generate an crop : ");
	for(int32_t iii=0; iii<input.Size(); iii++) {
		if(input[iii].x < sx) {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> OUT ");
				//new point intersection ...
				//y=aaax+bbb
				etkFloat_t aaa = (lastElement.y-input[iii].y) / (lastElement.x-input[iii].x);
				etkFloat_t bbb = lastElement.y - (aaa*lastElement.x);
				destPoint.y = aaa*sx + bbb;
				destPoint.x = sx;
				output.PushBack(destPoint);
			} else {
				//EWOL_DEBUG("element OUT ==> OUT ");
			}
			inside = false;
		} else {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> IN ");
				output.PushBack(input[iii]);
			} else {
				//EWOL_DEBUG("element OUT ==> IN ");
				//new point intersection ...
				//y=aaax+bbb
				etkFloat_t aaa = (lastElement.y-input[iii].y) / (lastElement.x-input[iii].x);
				etkFloat_t bbb = lastElement.y - (aaa*lastElement.x);
				destPoint.y = aaa*sx + bbb;
				destPoint.x = sx;
				output.PushBack(destPoint);
				output.PushBack(input[iii]);
			}
			inside = true;
		}
		// update the last point position :
		lastElement.x = input[iii].x;
		lastElement.y = input[iii].y;
	}
	
	//EWOL_DEBUG("generate an crop on element : " << sizeInit << " ==> " << output.Size() << "intermediate (1)");
	input = output;
	output.Clear();
	lastElement = input[input.Size()-1];
	inside = true;
	if (lastElement.y < sy) {
		inside = false;
	}
	for(int32_t iii=0; iii<input.Size(); iii++) {
		if(input[iii].y < sy) {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> OUT ");
				//new point intersection ...
				//x=aaay+bbb
				etkFloat_t aaa = (lastElement.x-input[iii].x) / (lastElement.y-input[iii].y);
				etkFloat_t bbb = lastElement.x - (aaa*lastElement.y);
				destPoint.y = sy;
				destPoint.x = sy*aaa + bbb;
				output.PushBack(destPoint);
			} else {
				//EWOL_DEBUG("element OUT ==> OUT ");
			}
			inside = false;
		} else {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> IN ");
				output.PushBack(input[iii]);
			} else {
				//EWOL_DEBUG("element OUT ==> IN ");
				//new point intersection ...
				//y=aaax+bbb
				etkFloat_t aaa = (lastElement.x-input[iii].x) / (lastElement.y-input[iii].y);
				etkFloat_t bbb = lastElement.x - (aaa*lastElement.y);
				destPoint.y = sy;
				destPoint.x = sy*aaa + bbb;
				output.PushBack(destPoint);
				output.PushBack(input[iii]);
			}
			inside = true;
		}
		// update the last point position :
		lastElement.x = input[iii].x;
		lastElement.y = input[iii].y;
	}
	
	input = output;
	output.Clear();
	lastElement = input[input.Size()-1];
	inside = true;
	if (lastElement.x > ex) {
		inside = false;
	}
	//EWOL_DEBUG("generate an crop : ");
	for(int32_t iii=0; iii<input.Size(); iii++) {
		if(input[iii].x > ex) {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> OUT ");
				//new point intersection ...
				//y=aaax+bbb
				etkFloat_t aaa = (lastElement.y-input[iii].y) / (lastElement.x-input[iii].x);
				etkFloat_t bbb = lastElement.y - (aaa*lastElement.x);
				destPoint.y = aaa*ex + bbb;
				destPoint.x = ex;
				output.PushBack(destPoint);
			} else {
				//EWOL_DEBUG("element OUT ==> OUT ");
			}
			inside = false;
		} else {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> IN ");
				output.PushBack(input[iii]);
			} else {
				//EWOL_DEBUG("element OUT ==> IN ");
				//new point intersection ...
				//y=aaax+bbb
				etkFloat_t aaa = (lastElement.y-input[iii].y) / (lastElement.x-input[iii].x);
				etkFloat_t bbb = lastElement.y - (aaa*lastElement.x);
				destPoint.y = aaa*ex + bbb;
				destPoint.x = ex;
				output.PushBack(destPoint);
				output.PushBack(input[iii]);
			}
			inside = true;
		}
		// update the last point position :
		lastElement.x = input[iii].x;
		lastElement.y = input[iii].y;
	}
	
	input = output;
	output.Clear();
	lastElement = input[input.Size()-1];
	inside = true;
	if (lastElement.y > ey) {
		inside = false;
	}
	for(int32_t iii=0; iii<input.Size(); iii++) {
		if(input[iii].y > ey) {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> OUT ");
				//new point intersection ...
				//x=aaay+bbb
				etkFloat_t aaa = (lastElement.x-input[iii].x) / (lastElement.y-input[iii].y);
				etkFloat_t bbb = lastElement.x - (aaa*lastElement.y);
				destPoint.y = ey;
				destPoint.x = ey*aaa + bbb;
				output.PushBack(destPoint);
			} else {
				//EWOL_DEBUG("element OUT ==> OUT ");
			}
			inside = false;
		} else {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> IN ");
				output.PushBack(input[iii]);
			} else {
				//EWOL_DEBUG("element OUT ==> IN ");
				//new point intersection ...
				//y=aaax+bbb
				etkFloat_t aaa = (lastElement.x-input[iii].x) / (lastElement.y-input[iii].y);
				etkFloat_t bbb = lastElement.x - (aaa*lastElement.y);
				destPoint.y = ey;
				destPoint.x = ey*aaa + bbb;
				output.PushBack(destPoint);
				output.PushBack(input[iii]);
			}
			inside = true;
		}
		// update the last point position :
		lastElement.x = input[iii].x;
		lastElement.y = input[iii].y;
	}
	
	
	//EWOL_DEBUG("generate an crop on element : " << sizeInit << " ==> " << output.Size() );
}


void ewol::OObject2DColored::UpdateSize(etkFloat_t sizeX, etkFloat_t sizeY)
{
	// copy the data
	etk::VectorType<coord2D_ts>   coord = m_coord;
	etk::VectorType<color_ts>     color = m_coordColor;
	// Clear the generated display ...
	m_coord.Clear();
	m_coordColor.Clear();
	// Check if the triangle is in the area...
	int32_t nbTriangle = coord.Size()/3;
	for (int32_t iii=0; iii<nbTriangle; iii++) {
		if(    coord[iii*3+0].x >= 0.0 && coord[iii*3+0].x < sizeX
		    && coord[iii*3+0].y >= 0.0 && coord[iii*3+0].y < sizeY
		    && coord[iii*3+1].x >= 0.0 && coord[iii*3+1].x < sizeX
		    && coord[iii*3+1].y >= 0.0 && coord[iii*3+1].y < sizeY
		    && coord[iii*3+2].x >= 0.0 && coord[iii*3+2].x < sizeX
		    && coord[iii*3+2].y >= 0.0 && coord[iii*3+2].y < sizeY)
		{
			// point 1-2-3 inside
			m_coord.PushBack(coord[iii*3+0]);
			m_coordColor.PushBack(color[iii*3+0]);
			m_coord.PushBack(coord[iii*3+1]);
			m_coordColor.PushBack(color[iii*3+1]);
			m_coord.PushBack(coord[iii*3+2]);
			m_coordColor.PushBack(color[iii*3+2]);
		} else {
			etk::VectorType<coord2D_ts>   tmpCoord;
			etk::VectorType<coord2D_ts>   tmpCoordOut;
			tmpCoord.PushBack(coord[iii*3+0]);
			tmpCoord.PushBack(coord[iii*3+1]);
			tmpCoord.PushBack(coord[iii*3+2]);
			
			SutherlandHodgman(tmpCoord, tmpCoordOut, 0, 0, sizeX, sizeY);
			tmpCoord.Clear();
			generatePolyGone(tmpCoordOut, tmpCoord);
			for (int32_t jjj=0; jjj<tmpCoord.Size(); jjj++) {
				m_coord.PushBack(tmpCoord[jjj]);
				m_coordColor.PushBack(color[iii*3+0]);
			}
		}
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


void ewol::OObject2DColored::SetColor(etkFloat_t red, etkFloat_t green, etkFloat_t blue, etkFloat_t alpha)
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

void ewol::OObject2DColored::SetPoint(etkFloat_t x, etkFloat_t y)
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

void ewol::OObject2DColored::Line(etkFloat_t sx, etkFloat_t sy, etkFloat_t ex, etkFloat_t ey, etkFloat_t thickness)
{
	ResetCount();
	if (sx == ex && sy == ey) {
		EWOL_WARNING("Try to draw an line width 0");
		return;
	}
	//teta = tan-1(oposer/adjacent)
	etkFloat_t teta = 0;
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
	etkFloat_t offsety = sin(teta-M_PI/2) * (thickness/2);
	etkFloat_t offsetx = cos(teta-M_PI/2) * (thickness/2);

	SetPoint(sx - offsetx, sy - offsety);
	SetPoint(sx + offsetx, sy + offsety);
	SetPoint(ex + offsetx, ey + offsety);
	
	SetPoint(ex + offsetx, ey + offsety);
	SetPoint(ex - offsetx, ey - offsety);
	SetPoint(sx - offsetx, sy - offsety);
}


void ewol::OObject2DColored::Rectangle(etkFloat_t x, etkFloat_t y, etkFloat_t w, etkFloat_t h)
{
	ResetCount();

	SetPoint(x    , y + h);
	SetPoint(x    , y);
	SetPoint(x + w, y);

	SetPoint(x + w, y);
	SetPoint(x + w, y + h);
	SetPoint(x    , y + h);
}


void ewol::OObject2DColored::Circle(etkFloat_t x, etkFloat_t y, etkFloat_t radius, etkFloat_t thickness)
{
	ResetCount();
	if (radius<0) {
		radius *= -1;
	}
	if (radius < thickness/2) {
		Disc(x, y, thickness/2 + radius);
	}
	int32_t nbOcurence = radius;
	if (nbOcurence < 10)
	{
		nbOcurence = 10;
	}
	for (int32_t iii=0; iii<nbOcurence; iii++) {
		
		etkFloat_t angleOne = 2*M_PI* iii     / nbOcurence ;
		etkFloat_t offsetExty = sin(angleOne) * (radius+thickness/2);
		etkFloat_t offsetExtx = cos(angleOne) * (radius+thickness/2);
		etkFloat_t offsetInty = sin(angleOne) * (radius-thickness/2);
		etkFloat_t offsetIntx = cos(angleOne) * (radius-thickness/2);
		
		etkFloat_t angleTwo = 2*M_PI* (iii+1) / nbOcurence ;
		etkFloat_t offsetExt2y = sin(angleTwo) * (radius+thickness/2);
		etkFloat_t offsetExt2x = cos(angleTwo) * (radius+thickness/2);
		etkFloat_t offsetInt2y = sin(angleTwo) * (radius-thickness/2);
		etkFloat_t offsetInt2x = cos(angleTwo) * (radius-thickness/2);
		
		SetPoint(x + offsetIntx,  y + offsetInty);
		SetPoint(x + offsetExtx,  y + offsetExty);
		SetPoint(x + offsetExt2x, y + offsetExt2y);
		
		SetPoint(x + offsetExt2x, y + offsetExt2y);
		SetPoint(x + offsetInt2x, y + offsetInt2y);
		SetPoint(x + offsetIntx,  y + offsetInty);
	}
}

void ewol::OObject2DColored::CirclePart(etkFloat_t x, etkFloat_t y, etkFloat_t radius, etkFloat_t thickness, etkFloat_t angleStart, etkFloat_t angleStop)
{
	ResetCount();
	if (radius<0) {
		radius *= -1;
	}
	if (radius < thickness/2) {
		Disc(x, y, thickness/2 + radius);
	}
	
	angleStart -= 90;
	angleStop  -= 90;
	etkFloat_t AStart = angleStart * (M_PI)/180;
	etkFloat_t AStop  = angleStop  * (M_PI)/180;
	etkFloat_t angleLinear = (angleStop-angleStart)* (M_PI)/180;
	
	int32_t nbOcurence = radius;
	if (nbOcurence < 10)
	{
		nbOcurence = 10;
	}
	for (int32_t iii=0; iii<nbOcurence; iii++) {
		
		etkFloat_t angleOne =  AStart + (angleLinear* iii     / nbOcurence) ;
		etkFloat_t offsetExty = sin(angleOne) * (radius+thickness/2);
		etkFloat_t offsetExtx = cos(angleOne) * (radius+thickness/2);
		etkFloat_t offsetInty = sin(angleOne) * (radius-thickness/2);
		etkFloat_t offsetIntx = cos(angleOne) * (radius-thickness/2);
		
		etkFloat_t angleTwo =  AStart + (angleLinear*  (iii+1) / nbOcurence );
		etkFloat_t offsetExt2y = sin(angleTwo) * (radius+thickness/2);
		etkFloat_t offsetExt2x = cos(angleTwo) * (radius+thickness/2);
		etkFloat_t offsetInt2y = sin(angleTwo) * (radius-thickness/2);
		etkFloat_t offsetInt2x = cos(angleTwo) * (radius-thickness/2);
		
		SetPoint(x + offsetIntx,  y + offsetInty);
		SetPoint(x + offsetExtx,  y + offsetExty);
		SetPoint(x + offsetExt2x, y + offsetExt2y);
		
		SetPoint(x + offsetExt2x, y + offsetExt2y);
		SetPoint(x + offsetInt2x, y + offsetInt2y);
		SetPoint(x + offsetIntx,  y + offsetInty);
	}
}

void ewol::OObject2DColored::Disc(etkFloat_t x, etkFloat_t y, etkFloat_t radius)
{
	ResetCount();
	if (radius<0) {
		radius *= -1;
	}
	int32_t nbOcurence = radius*0.50;
	if (nbOcurence < 15)
	{
		nbOcurence = 15;
	}

	for (int32_t iii=0; iii<nbOcurence; iii++) {
		SetPoint(x, y);
		
		etkFloat_t angleOne = 2*M_PI* iii / nbOcurence ;
		etkFloat_t offsety = sin(angleOne) * radius;
		etkFloat_t offsetx = cos(angleOne) * radius;
		
		SetPoint(x + offsetx, y + offsety);
		
		etkFloat_t angleTwo = 2*M_PI* (iii+1) / nbOcurence ;
		offsety = sin(angleTwo) * radius;
		offsetx = cos(angleTwo) * radius;
		
		SetPoint(x + offsetx, y + offsety);
	}
}

void ewol::OObject2DColored::DiscPart(etkFloat_t x, etkFloat_t y, etkFloat_t radius, etkFloat_t angleStart, etkFloat_t angleStop)
{
	ResetCount();
	if (radius<0) {
		radius *= -1;
	}
	angleStart -= 90;
	angleStop  -= 90;
	etkFloat_t AStart = angleStart * (M_PI)/180;
	etkFloat_t AStop  = angleStop  * (M_PI)/180;
	etkFloat_t angleLinear = (angleStop-angleStart)* (M_PI)/180;
	//EWOL_DEBUG("Write a part of disk " << angleStart << " -> " << angleStop << "  ocurence=" << (angleLinear*180/(M_PI)) );
	
	int32_t nbOcurence = radius*0.50;
	if (nbOcurence < 15)
	{
		nbOcurence = 15;
	}
	

	for (int32_t iii=0; iii<nbOcurence; iii++) {
		SetPoint(x, y);
		
		etkFloat_t angleOne = AStart + (angleLinear* iii / nbOcurence) ;
		etkFloat_t offsety = sin(angleOne) * radius;
		etkFloat_t offsetx = cos(angleOne) * radius;
		
		SetPoint(x + offsetx, y + offsety);
		
		etkFloat_t angleTwo = AStart + (angleLinear* (iii+1) / nbOcurence) ;
		offsety = sin(angleTwo) * radius;
		offsetx = cos(angleTwo) * radius;
		
		SetPoint(x + offsetx, y + offsety);
	}
}
