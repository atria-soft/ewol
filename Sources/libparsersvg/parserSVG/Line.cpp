/**
 *******************************************************************************
 * @file parserSVG/Line.cpp
 * @brief basic line parsing (Sources)
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
#include <parserSVG/Line.h>

svg::Line::Line(paintState_ts parentPaintState) : svg::Base(parentPaintState)
{
	m_startPos.x = 0.0;
	m_startPos.y = 0.0;
	m_stopPos.x = 0.0;
	m_stopPos.y = 0.0;
}

svg::Line::~Line(void)
{
	
}

bool svg::Line::Parse(TiXmlNode * node)
{
	ParseTransform(node);
	ParsePaintAttr(node);
	
	const char * content = node->ToElement()->Attribute("x1");
	if (NULL != content) {
		m_startPos.x = ParseLength(content);
	}
	content = node->ToElement()->Attribute("y1");
	if (NULL != content) {
		m_startPos.y = ParseLength(content);
	}
	content = node->ToElement()->Attribute("x2");
	if (NULL != content) {
		m_stopPos.x = ParseLength(content);
	}
	content = node->ToElement()->Attribute("y2");
	if (NULL != content) {
		m_stopPos.y = ParseLength(content);
	}
}

void svg::Line::Display(int32_t spacing)
{
	SVG_DEBUG(SpacingDist(spacing) << "Line " << m_startPos << " to " << m_stopPos);
}


