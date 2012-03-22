/**
 *******************************************************************************
 * @file parserSVG/Ellipse.cpp
 * @brief basic ellipse parsing (Sources)
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
#include <parserSVG/Ellipse.h>

svg::Ellipse::Ellipse(PaintState parentPaintState) : svg::Base(parentPaintState)
{
	
}

svg::Ellipse::~Ellipse(void)
{
	
}

bool svg::Ellipse::Parse(TiXmlNode * node)
{
	ParseTransform(node);
	ParsePaintAttr(node);
	m_c.x = 0.0;
	m_c.y = 0.0;
	m_r.x = 0.0;
	m_r.y = 0.0;
	
	const char * content = node->ToElement()->Attribute("cx");
	if (NULL != content) {
		m_c.x = ParseLength(content);
	}
	content = node->ToElement()->Attribute("cy");
	if (NULL != content) {
		m_c.y = ParseLength(content);
	}
	content = node->ToElement()->Attribute("rx");
	if (NULL != content) {
		m_r.x = ParseLength(content);
	} else {
		SVG_ERROR("(l "<<node->Row()<<") Ellipse \"rx\" is not present");
		return false;
	}
	content = node->ToElement()->Attribute("ry");
	if (NULL != content) {
		m_r.y = ParseLength(content);
	} else {
		SVG_ERROR("(l "<<node->Row()<<") Ellipse \"ry\" is not present");
		return false;
	}
	
	return true;
}

void svg::Ellipse::Display(int32_t spacing)
{
	SVG_DEBUG(SpacingDist(spacing) << "Ellipse c=" << m_c << " r=" << m_r);
}


