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


#include <agg-2.4/agg_basics.h>
#include <agg-2.4/agg_rendering_buffer.h>
#include <agg-2.4/agg_rasterizer_scanline_aa.h>
#include <agg-2.4/agg_scanline_p.h>
#include <agg-2.4/agg_renderer_scanline.h>
#include <agg-2.4/agg_path_storage.h>
#include <agg-2.4/agg_conv_transform.h>
#include <agg-2.4/agg_bounding_rect.h>
#include <agg-2.4/agg_color_rgba.h>
#include <agg-2.4/agg_pixfmt_rgba.h>

void svg::Polygon::AggDraw(svg::Renderer& myRenderer, svg::PaintState &curentPaintProp)
{
	curentPaintProp = m_paint;
	agg::path_storage             path;
	etk::VectorType<agg::rgba8>   colorsList;
	etk::VectorType<uint32_t>     pathListId;
	
	// New color. Every new color creates new path in the path object.
	colorsList.PushBack(agg::rgba8(m_paint.fill.red, m_paint.fill.green, m_paint.fill.blue, m_paint.fill.alpha));
	uint32_t tmpPathNew = path.start_new_path();
	pathListId.PushBack(tmpPathNew);
	
	path.move_to(m_listPoint[0].x, m_listPoint[0].y);
	for( int32_t iii=1; iii< m_listPoint.Size(); iii++) {
		path.line_to(m_listPoint[iii].x, m_listPoint[iii].y);
	}
	path.close_polygon();
	
	
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
}

