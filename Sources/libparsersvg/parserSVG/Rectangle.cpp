/**
 *******************************************************************************
 * @file parserSVG/Rectangle.cpp
 * @brief basic rectangle parsing (Sources)
 * @author Edouard DUPIN
 * @date 20/03/2012
 * @par Project
 * parserSVG
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

#include <parserSVG/Debug.h>
#include <parserSVG/Rectangle.h>

svg::Rectangle::Rectangle(paintState_ts parentPaintState) : svg::Base(parentPaintState)
{
	m_position.x = 0.0;
	m_position.y = 0.0;
	m_size.x = 0.0;
	m_size.y = 0.0;
	m_roundedCorner.x = 0.0;
	m_roundedCorner.y = 0.0;
}

svg::Rectangle::~Rectangle(void)
{
	
}

bool svg::Rectangle::Parse(TiXmlNode * node)
{
	m_position.x = 0.0;
	m_position.y = 0.0;
	m_size.x = 0.0;
	m_size.y = 0.0;
	m_roundedCorner.x = 0.0;
	m_roundedCorner.y = 0.0;
	
	ParseTransform(node);
	ParsePaintAttr(node);
	
	ParsePosition(node, m_position, m_size);
	
	return true;
}

void svg::Rectangle::Display(int32_t spacing)
{
	SVG_DEBUG(SpacingDist(spacing) << "Rectangle");
}

