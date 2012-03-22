/**
 *******************************************************************************
 * @file parserSVG/Polyline.cpp
 * @brief basic Poliline parsing (Sources)
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
#include <parserSVG/Polyline.h>

svg::Polyline::Polyline(PaintState parentPaintState) : svg::Base(parentPaintState)
{
	
}

svg::Polyline::~Polyline(void)
{
	
}

bool svg::Polyline::Parse(TiXmlNode * node)
{
	ParseTransform(node);
	ParsePaintAttr(node);
	
	const char *sss = node->ToElement()->Attribute("points");
	if (NULL == sss) {
		SVG_ERROR("(l "<<node->Row()<<") polyline: missing points attribute");
		return false;
	}
	SVG_VERBOSE("Parse polyline : \"" << sss << "\"");
	while ('\0' != sss[0]) {
		coord2D_ts pos;
		int32_t n;
		if (sscanf(sss, "%f,%f %n", &pos.x, &pos.y, &n) == 2) {
			m_listPoint.PushBack(pos);
			sss += n;
		} else {
			break;
		}
	}
	return true;
}

void svg::Polyline::Display(int32_t spacing)
{
	SVG_DEBUG(SpacingDist(spacing) << "Polyline nbPoint=" << m_listPoint.Size());
}


