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
	
	const char * content = node->ToElement()->Attribute("rx");
	if (NULL != content) {
		m_roundedCorner.x = ParseLength(content);
	}
	content = node->ToElement()->Attribute("ry");
	if (NULL != content) {
		m_roundedCorner.y = ParseLength(content);
	}
	return true;
}

void svg::Rectangle::Display(int32_t spacing)
{
	SVG_DEBUG(SpacingDist(spacing) << "Rectangle : pos=" << m_position << " size=" << m_size << " corner=" << m_roundedCorner);
}


void svg::Rectangle::AggDraw(svg::Renderer& myRenderer, svg::PaintState &curentPaintProp)
{
	myRenderer.m_renderArea->color(agg::rgba8(m_paint.fill.red, m_paint.fill.green, m_paint.fill.blue, m_paint.fill.alpha));
	// Creating a rounded rectangle
	agg::rounded_rect rect_r(m_position.x, m_position.y, m_size.x, m_size.y, m_roundedCorner.x);
	rect_r.radius(m_roundedCorner.x, m_roundedCorner.y);
	rect_r.normalize_radius();
	
	myRenderer.m_rasterizer.add_path(rect_r);
	agg::render_scanlines(myRenderer.m_rasterizer, myRenderer.m_scanLine, *myRenderer.m_renderArea);

	if (m_paint.strokeWidth > 0) {
		myRenderer.m_renderArea->color(agg::rgba8(m_paint.stroke.red, m_paint.stroke.green, m_paint.stroke.blue, m_paint.stroke.alpha));
		// Drawing as an outline
		agg::conv_stroke<agg::rounded_rect> rect_p(rect_r);
		rect_p.width(m_paint.strokeWidth);
		myRenderer.m_rasterizer.add_path(rect_p);
		agg::render_scanlines(myRenderer.m_rasterizer, myRenderer.m_scanLine, *myRenderer.m_renderArea);
	}
	
	/*
	agg::trans_affine  mtx;
	//mtx *= agg::trans_affine_translation(-g_base_dx, -g_base_dy);
	//mtx *= agg::trans_affine_scaling(g_scale*coefmult, g_scale*coefmult);
	//mtx *= agg::trans_affine_rotation(g_angle);// + agg::pi);
	//mtx *= agg::trans_affine_skewing(g_skew_x/1000.0, g_skew_y/1000.0);
	//mtx *= agg::trans_affine_translation(width*0.3, height/2);
	mtx *= agg::trans_affine_translation(myRenderer.m_size.x*0.5, myRenderer.m_size.x/2);
	
	// This code renders the lion:
	agg::conv_transform<agg::path_storage, agg::trans_affine> trans(path, mtx);
	
	agg::render_all_paths( myRenderer.m_rasterizer, myRenderer.m_scanLine, *myRenderer.m_renderArea, trans, &colorsList[0], &pathListId[0], pathListId.Size());
	*/

}

