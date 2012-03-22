/**
 *******************************************************************************
 * @file parserSVG/Polygon.cpp
 * @brief basic poligon parsing (Sources)
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
#include <parserSVG/Polygon.h>

svg::Polygon::Polygon(PaintState parentPaintState) : svg::Base(parentPaintState)
{
	
}

svg::Polygon::~Polygon(void)
{
	
}

bool svg::Polygon::Parse(TiXmlNode * node)
{
	ParseTransform(node);
	ParsePaintAttr(node);
	
	const char *sss = node->ToElement()->Attribute("points");
	if (NULL == sss) {
		SVG_ERROR("(l "<<node->Row()<<") polygon: missing points attribute");
		return false;
	}
	SVG_VERBOSE("Parse polygon : \"" << sss << "\"");
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

void svg::Polygon::Display(int32_t spacing)
{
	SVG_DEBUG(SpacingDist(spacing) << "Polygon nbPoint=" << m_listPoint.Size());
}


void svg::Polygon::AggDraw(agg::path_storage& path, etk::VectorType<agg::rgba8> &colors, etk::VectorType<uint32_t> &pathIdx, PaintState &curentPaintProp)
{
	if (m_listPoint.Size()<2) {
		// nothing to draw ...
		return;
	}
	AggCheckChange(path, colors, pathIdx, curentPaintProp);
	path.move_to(m_listPoint[0].x, m_listPoint[0].y);
	for( int32_t iii=1; iii< m_listPoint.Size(); iii++) {
		path.line_to(m_listPoint[iii].x, m_listPoint[iii].y);
	}
	path.close_polygon();
}


