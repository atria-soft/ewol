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
#include <ewol/ResourceManager.h>
#include <ewol/openGL/openGL.h>
#include <math.h>

#undef __class__
#define __class__	"Sprite"


ewol::Sprite::Sprite(etk::UString spriteName, float sizeX, float sizeY) :
	ewol::OObject3DTextured(spriteName, sizeX, sizeY)
{
	m_name = spriteName;
}


ewol::Sprite::~Sprite(void)
{
	
}

void ewol::Sprite::Element(etk::Vector2D<float> pos, float size, float angle)
{
	draw::Color tmpColor(0xFFFFFFFF);
	etk::Vector3D<float> pos2;
	pos2.x = pos.x;
	pos2.y = pos.y;
	pos2.z = 0.0;
	Element(pos2, size, angle, tmpColor);
}

void ewol::Sprite::Element(etk::Vector3D<float> pos, float size, float angle)
{
	draw::Color tmpColor(0xFFFFFFFF);
	Element(pos, size, angle, tmpColor);
}

void ewol::Sprite::Element(etk::Vector2D<float> pos, float size, float angle, draw::Color tmpColor)
{
	etk::Vector3D<float> pos2;
	pos2.x = pos.x;
	pos2.y = pos.y;
	pos2.z = 0.0;
	Element(pos2, size, angle, tmpColor);
}


void ewol::Sprite::Element(etk::Vector3D<float> pos, float size, float angle, draw::Color tmpColor)
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
	
	draw::Colorf localColor = tmpColor;
	etk::Vector3D<float> point = pos;
	float yyySin = sin(angle) * size;
	float xxxCos = cos(angle) * size;
	
	point.x = xxxCos + pos.x;
	point.y = yyySin + pos.y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(texB);
	m_coordColor.PushBack(localColor);
	
	point.x = yyySin + pos.x;
	point.y = -xxxCos + pos.y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(texC);
	m_coordColor.PushBack(localColor);
	
	point.x = -xxxCos + pos.x;
	point.y = -yyySin + pos.y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(texD);
	m_coordColor.PushBack(localColor);
	
	m_coord.PushBack(point);
	m_coordTex.PushBack(texD);
	m_coordColor.PushBack(localColor);
	
	point.x = -yyySin + pos.x;
	point.y = xxxCos + pos.y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(texA);
	m_coordColor.PushBack(localColor);
	
	point.x = xxxCos + pos.x;
	point.y = yyySin + pos.y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(texB);
	m_coordColor.PushBack(localColor);
}

