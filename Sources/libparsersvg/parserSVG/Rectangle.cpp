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
#include <agg-2.4/agg_rounded_rect.h>
#include <agg-2.4/agg_conv_stroke.h>

svg::Rectangle::Rectangle(PaintState parentPaintState) : svg::Base(parentPaintState)
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
	SVG_DEBUG(SpacingDist(spacing) << "Rectangle : pos=" << m_position << " size=" << m_size << " corner=" << m_roundedCorner);
}



void svg::Rectangle::AggDraw(agg::path_storage& path, etk::VectorType<agg::rgba8> &colors, etk::VectorType<uint32_t> &pathIdx, PaintState &curentPaintProp)
{
	if (m_size.x<=0 && m_size.y<=0) {
		// nothing to draw ...
		return;
	}
	AggCheckChange(path, colors, pathIdx, curentPaintProp);
	#if 1
	path.move_to(m_position.x, m_position.y);
	//path.move_to(0, 0);
	path.line_rel(m_size.x, 0);
	path.line_rel(0, m_size.y);
	path.line_rel(-m_size.x, 0);
	path.line_rel(0, -m_size.y);
	path.close_polygon();
	#else
	
	// Creating a rounded rectangle
	agg::rounded_rect r(m_position.x, m_position.y, m_size.x, m_size.y, m_roundedCorner.x);
	r.normalize_radius();
	
	// Drawing as an outline
	agg::conv_stroke<agg::rounded_rect> p(r);
	p.width(1.0);
	path.add_path(p);
	#endif
}


