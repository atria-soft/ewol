/**
 *******************************************************************************
 * @file parserSVG/Circle.cpp
 * @brief basic circle parsing (Sources)
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
#include <parserSVG/Circle.h>

svg::Circle::Circle(PaintState parentPaintState) : svg::Base(parentPaintState)
{
	
}

svg::Circle::~Circle(void)
{
	
}

bool svg::Circle::Parse(TiXmlNode * node)
{
	m_radius = 0.0;
	m_position.x = 0.0;
	m_position.y = 0.0;
	ParseTransform(node);
	ParsePaintAttr(node);
	
	const char * content = node->ToElement()->Attribute("cx");
	if (NULL != content) {
		m_position.x = ParseLength(content);
	}
	content = node->ToElement()->Attribute("cy");
	if (NULL != content) {
		m_position.y = ParseLength(content);
	}
	content = node->ToElement()->Attribute("r");
	if (NULL != content) {
		m_radius = ParseLength(content);
	} else {
		SVG_ERROR("(l "<<node->Row()<<") Circle \"r\" is not present");
		return false;
	}

	if (0 > m_radius) {
		m_radius = 0;
		SVG_ERROR("(l "<<node->Row()<<") Circle \"r\" is negative");
		return false;
	}
	return true;
}

void svg::Circle::Display(int32_t spacing)
{
	SVG_DEBUG(SpacingDist(spacing) << "Circle " << m_position << " radius=" << m_radius);
}



