/**
 *******************************************************************************
 * @file ewol/OObject/2DColored.cpp
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

#include <ewol/OObject/2DColored.h>
#include <ewol/importgl.h>
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
	m_coord.clear();
	m_coordColor.clear();
}


void ewol::OObject2DColored::Draw(void)
{
	if (m_coord.size()<=0) {
		return;
	}
	glPushMatrix();
	// Enable Pointers
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	
	glScalef(m_scaling.x, m_scaling.y, 1.0);
	
	// Set the vertex pointer to our vertex data
	glVertexPointer(2, GL_FLOAT, 0, &m_coord[0] );
	//glColorPointer(4, oglTypeFloat_t, 0, &m_coordColor[0] );
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, &m_coordColor[0] );
	// Render : draw all of the triangles at once
	glDrawArrays( GL_TRIANGLES, 0, m_coord.size());
	//glDrawElements( GL_TRIANGLES, 0, m_coord.Size());
	//EWOL_DEBUG("Draw ..." << m_coord.Size()/3 << " triangle(s)");

	// Disable Pointers
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
	glPopMatrix();

}

void ewol::OObject2DColored::Clear(void)
{
	m_coord.clear();
	m_coordColor.clear();
}


void generatePolyGone(std::vector<Vector2D<float> > & input, std::vector<Vector2D<float> > & output )
{
	if (input.size()<3) {
		return;
	}
	// TODO : Regenerate a linear poligone generation
	for (int32_t iii=1; iii<input.size()-1; iii++) {
		output.push_back(input[0]);
		output.push_back(input[iii]);
		output.push_back(input[iii+1]);
	}
	//EWOL_DEBUG("generate Plygone : " << input.Size() << " ==> " << output.Size() );
}

void SutherlandHodgman(std::vector<Vector2D<float> > & input, std::vector<Vector2D<float> > & output, float sx, float sy, float ex, float ey)
{
	// with Sutherland-Hodgman-Algorithm
	if (input.size() <=0) {
		return;
	}
	//int32_t sizeInit=input.Size();
	// last element :
	Vector2D<float> destPoint;
	Vector2D<float> lastElement = input[input.size()-1];
	bool inside = true;
	if (lastElement.x < sx) {
		inside = false;
	}
	//EWOL_DEBUG("generate an crop : ");
	for(int32_t iii=0; iii<input.size(); iii++) {
		if(input[iii].x < sx) {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> OUT ");
				//new point intersection ...
				//y=aaax+bbb
				float aaa = (lastElement.y-input[iii].y) / (lastElement.x-input[iii].x);
				float bbb = lastElement.y - (aaa*lastElement.x);
				destPoint.y = aaa*sx + bbb;
				destPoint.x = sx;
				output.push_back(destPoint);
			} else {
				//EWOL_DEBUG("element OUT ==> OUT ");
			}
			inside = false;
		} else {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> IN ");
				output.push_back(input[iii]);
			} else {
				//EWOL_DEBUG("element OUT ==> IN ");
				//new point intersection ...
				//y=aaax+bbb
				float aaa = (lastElement.y-input[iii].y) / (lastElement.x-input[iii].x);
				float bbb = lastElement.y - (aaa*lastElement.x);
				destPoint.y = aaa*sx + bbb;
				destPoint.x = sx;
				output.push_back(destPoint);
				output.push_back(input[iii]);
			}
			inside = true;
		}
		// update the last point position :
		lastElement.x = input[iii].x;
		lastElement.y = input[iii].y;
	}
	
	//EWOL_DEBUG("generate an crop on element : " << sizeInit << " ==> " << output.Size() << "intermediate (1)");
	input = output;
	output.clear();
	lastElement = input[input.size()-1];
	inside = true;
	if (lastElement.y < sy) {
		inside = false;
	}
	for(int32_t iii=0; iii<input.size(); iii++) {
		if(input[iii].y < sy) {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> OUT ");
				//new point intersection ...
				//x=aaay+bbb
				float aaa = (lastElement.x-input[iii].x) / (lastElement.y-input[iii].y);
				float bbb = lastElement.x - (aaa*lastElement.y);
				destPoint.y = sy;
				destPoint.x = sy*aaa + bbb;
				output.push_back(destPoint);
			} else {
				//EWOL_DEBUG("element OUT ==> OUT ");
			}
			inside = false;
		} else {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> IN ");
				output.push_back(input[iii]);
			} else {
				//EWOL_DEBUG("element OUT ==> IN ");
				//new point intersection ...
				//y=aaax+bbb
				float aaa = (lastElement.x-input[iii].x) / (lastElement.y-input[iii].y);
				float bbb = lastElement.x - (aaa*lastElement.y);
				destPoint.y = sy;
				destPoint.x = sy*aaa + bbb;
				output.push_back(destPoint);
				output.push_back(input[iii]);
			}
			inside = true;
		}
		// update the last point position :
		lastElement.x = input[iii].x;
		lastElement.y = input[iii].y;
	}
	
	input = output;
	output.clear();
	lastElement = input[input.size()-1];
	inside = true;
	if (lastElement.x > ex) {
		inside = false;
	}
	//EWOL_DEBUG("generate an crop : ");
	for(int32_t iii=0; iii<input.size(); iii++) {
		if(input[iii].x > ex) {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> OUT ");
				//new point intersection ...
				//y=aaax+bbb
				float aaa = (lastElement.y-input[iii].y) / (lastElement.x-input[iii].x);
				float bbb = lastElement.y - (aaa*lastElement.x);
				destPoint.y = aaa*ex + bbb;
				destPoint.x = ex;
				output.push_back(destPoint);
			} else {
				//EWOL_DEBUG("element OUT ==> OUT ");
			}
			inside = false;
		} else {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> IN ");
				output.push_back(input[iii]);
			} else {
				//EWOL_DEBUG("element OUT ==> IN ");
				//new point intersection ...
				//y=aaax+bbb
				float aaa = (lastElement.y-input[iii].y) / (lastElement.x-input[iii].x);
				float bbb = lastElement.y - (aaa*lastElement.x);
				destPoint.y = aaa*ex + bbb;
				destPoint.x = ex;
				output.push_back(destPoint);
				output.push_back(input[iii]);
			}
			inside = true;
		}
		// update the last point position :
		lastElement.x = input[iii].x;
		lastElement.y = input[iii].y;
	}
	
	input = output;
	output.clear();
	lastElement = input[input.size()-1];
	inside = true;
	if (lastElement.y > ey) {
		inside = false;
	}
	for(int32_t iii=0; iii<input.size(); iii++) {
		if(input[iii].y > ey) {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> OUT ");
				//new point intersection ...
				//x=aaay+bbb
				float aaa = (lastElement.x-input[iii].x) / (lastElement.y-input[iii].y);
				float bbb = lastElement.x - (aaa*lastElement.y);
				destPoint.y = ey;
				destPoint.x = ey*aaa + bbb;
				output.push_back(destPoint);
			} else {
				//EWOL_DEBUG("element OUT ==> OUT ");
			}
			inside = false;
		} else {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> IN ");
				output.push_back(input[iii]);
			} else {
				//EWOL_DEBUG("element OUT ==> IN ");
				//new point intersection ...
				//y=aaax+bbb
				float aaa = (lastElement.x-input[iii].x) / (lastElement.y-input[iii].y);
				float bbb = lastElement.x - (aaa*lastElement.y);
				destPoint.y = ey;
				destPoint.x = ey*aaa + bbb;
				output.push_back(destPoint);
				output.push_back(input[iii]);
			}
			inside = true;
		}
		// update the last point position :
		lastElement.x = input[iii].x;
		lastElement.y = input[iii].y;
	}
	
	
	//EWOL_DEBUG("generate an crop on element : " << sizeInit << " ==> " << output.Size() );
}


void ewol::OObject2DColored::GenerateTriangle(void)
{
	m_triElement = 0;
	
	m_coord.push_back(m_triangle[0]);
	m_coordColor.push_back(m_color[0]);
	m_coord.push_back(m_triangle[1]);
	m_coordColor.push_back(m_color[1]);
	m_coord.push_back(m_triangle[2]);
	m_coordColor.push_back(m_color[2]);
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
		m_color[0].red = red*255;
		m_color[0].green = green*255;
		m_color[0].blue = blue*255;
		m_color[0].alpha = alpha*255;
	}
	if (m_triElement < 2) {
		m_color[1].red = red*255;
		m_color[1].green = green*255;
		m_color[1].blue = blue*255;
		m_color[1].alpha = alpha*255;
	}
	if (m_triElement < 3) {
		m_color[2].red = red*255;
		m_color[2].green = green*255;
		m_color[2].blue = blue*255;
		m_color[2].alpha = alpha*255;
	}
	
}

void ewol::OObject2DColored::SetPoint(Vector2D<float> point)
{
	// TODO : Clean this :
	Vector2D<float> tmpElement;
	tmpElement.x = point.x;
	tmpElement.y = point.y;
	m_triangle[m_triElement] = tmpElement;
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
	float teta = 0;
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
	float offsety = sin(teta-M_PI/2) * (thickness/2);
	float offsetx = cos(teta-M_PI/2) * (thickness/2);

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
	
	/* Bitmap position
	 *      xA     xB
	 *   yC *------*
	 *      |      |
	 *      |      |
	 *   yD *------*
	 */
	float dxA = x;
	float dxB = x + w;
	float dyC = y;
	float dyD = y + h;
	
	if (true == m_hasClipping) {
		if (dxA < m_clipping.x) {
			dxA = m_clipping.x;
		}
		if (dxB > m_clipping.x + m_clipping.w) {
			dxB = m_clipping.x + m_clipping.w;
		}
		if (dyC < m_clipping.y) {
			dyC = m_clipping.y;
		}
		if (dyD > m_clipping.y + m_clipping.h) {
			dyD = m_clipping.y + m_clipping.h;
		}
	}
	if(    dyC >= dyD
	    || dxA >= dxB) {
		return;
	}
	SetPoint(dxA, dyD);
	SetPoint(dxA, dyC);
	SetPoint(dxB, dyC);

	SetPoint(dxB, dyC);
	SetPoint(dxB, dyD);
	SetPoint(dxA, dyD);
}

void ewol::OObject2DColored::RectangleBorder(float x, float y, float w, float h, float thickness)
{
	// TODO : This did not manage the thickness of the line ...
	Line(x,   y,   x+w, y,   thickness);
	Line(x+w, y,   x+w, y+h, thickness);
	Line(x+w, y+h, x,   y+h, thickness);
	Line(x,   y+h, x,   y,   thickness);
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
	int32_t nbOcurence = radius;
	if (nbOcurence < 10)
	{
		nbOcurence = 10;
	}
	for (int32_t iii=0; iii<nbOcurence; iii++) {
		
		float angleOne = 2*M_PI* iii     / nbOcurence ;
		float offsetExty = sin(angleOne) * (radius+thickness/2);
		float offsetExtx = cos(angleOne) * (radius+thickness/2);
		float offsetInty = sin(angleOne) * (radius-thickness/2);
		float offsetIntx = cos(angleOne) * (radius-thickness/2);
		
		float angleTwo = 2*M_PI* (iii+1) / nbOcurence ;
		float offsetExt2y = sin(angleTwo) * (radius+thickness/2);
		float offsetExt2x = cos(angleTwo) * (radius+thickness/2);
		float offsetInt2y = sin(angleTwo) * (radius-thickness/2);
		float offsetInt2x = cos(angleTwo) * (radius-thickness/2);
		
		SetPoint(x + offsetIntx,  y + offsetInty);
		SetPoint(x + offsetExtx,  y + offsetExty);
		SetPoint(x + offsetExt2x, y + offsetExt2y);
		
		SetPoint(x + offsetExt2x, y + offsetExt2y);
		SetPoint(x + offsetInt2x, y + offsetInt2y);
		SetPoint(x + offsetIntx,  y + offsetInty);
	}
}

void ewol::OObject2DColored::CirclePart(float x, float y, float radius, float thickness, float angleStart, float angleStop)
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
	float AStart = angleStart * (M_PI)/180;
	//float AStop  = angleStop  * (M_PI)/180;
	float angleLinear = (angleStop-angleStart)* (M_PI)/180;
	
	int32_t nbOcurence = radius;
	if (nbOcurence < 10)
	{
		nbOcurence = 10;
	}
	for (int32_t iii=0; iii<nbOcurence; iii++) {
		
		float angleOne =  AStart + (angleLinear* iii     / nbOcurence) ;
		float offsetExty = sin(angleOne) * (radius+thickness/2);
		float offsetExtx = cos(angleOne) * (radius+thickness/2);
		float offsetInty = sin(angleOne) * (radius-thickness/2);
		float offsetIntx = cos(angleOne) * (radius-thickness/2);
		
		float angleTwo =  AStart + (angleLinear*  (iii+1) / nbOcurence );
		float offsetExt2y = sin(angleTwo) * (radius+thickness/2);
		float offsetExt2x = cos(angleTwo) * (radius+thickness/2);
		float offsetInt2y = sin(angleTwo) * (radius-thickness/2);
		float offsetInt2x = cos(angleTwo) * (radius-thickness/2);
		
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
	if (radius<0) {
		radius *= -1;
	}
	int32_t nbOcurence = radius*0.50;
	if (nbOcurence < 15)
	{
		nbOcurence = 15;
	}

	// TODO : Generate a poligone instead if this ...
	for (int32_t iii=0; iii<nbOcurence; iii++) {
		SetPoint(x, y);
		
		float angleOne = 2*M_PI* iii / nbOcurence ;
		float offsety = sin(angleOne) * radius;
		float offsetx = cos(angleOne) * radius;
		
		SetPoint(x + offsetx, y + offsety);
		
		float angleTwo = 2*M_PI* (iii+1) / nbOcurence ;
		offsety = sin(angleTwo) * radius;
		offsetx = cos(angleTwo) * radius;
		
		SetPoint(x + offsetx, y + offsety);
	}
}

void ewol::OObject2DColored::DiscPart(float x, float y, float radius, float angleStart, float angleStop)
{
	ResetCount();
	if (radius<0) {
		radius *= -1;
	}
	angleStart -= 90;
	angleStop  -= 90;
	float AStart = angleStart * (M_PI)/180;
	//float AStop  = angleStop  * (M_PI)/180;
	float angleLinear = (angleStop-angleStart)* (M_PI)/180;
	//EWOL_DEBUG("Write a part of disk " << angleStart << " -> " << angleStop << "  ocurence=" << (angleLinear*180/(M_PI)) );
	
	int32_t nbOcurence = radius*0.50;
	if (nbOcurence < 15)
	{
		nbOcurence = 15;
	}
	
	// TODO : Generate a poligone instead if this ...
	for (int32_t iii=0; iii<nbOcurence; iii++) {
		SetPoint(x, y);
		
		float angleOne = AStart + (angleLinear* iii / nbOcurence) ;
		float offsety = sin(angleOne) * radius;
		float offsetx = cos(angleOne) * radius;
		
		SetPoint(x + offsetx, y + offsety);
		
		float angleTwo = AStart + (angleLinear* (iii+1) / nbOcurence) ;
		offsety = sin(angleTwo) * radius;
		offsetx = cos(angleTwo) * radius;
		
		SetPoint(x + offsetx, y + offsety);
	}
}
