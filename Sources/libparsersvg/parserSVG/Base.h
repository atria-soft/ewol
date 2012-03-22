/**
 *******************************************************************************
 * @file parserSVG/Base.h
 * @brief basic Element parsing (Header)
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

#ifndef __SVG_BASE_H__
#define __SVG_BASE_H__

#include <etk/Types.h>
#include <etk/VectorType.h>

#include <tinyXML/tinyxml.h>

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

namespace svg
{
	#define MATRIX_SIZE       (6)
	class PaintState {
		public :
			PaintState(void) {};
			~PaintState(void) {};
			color8_ts  fill;
			color8_ts  stroke;
			etkFloat_t strokeWidth;
			coord2D_ts viewPort;
			etkFloat_t matrix[MATRIX_SIZE];
		bool  operator!= (const svg::PaintState& paintExt) const
		{
			if(    fill.red   != paintExt.fill.red
			    || fill.green != paintExt.fill.green
			    || fill.blue  != paintExt.fill.blue
			    || fill.alpha != paintExt.fill.alpha) {
				return true;
			}
			if(    stroke.red   != paintExt.stroke.red
			    || stroke.green != paintExt.stroke.green
			    || stroke.blue  != paintExt.stroke.blue
			    || stroke.alpha != paintExt.stroke.alpha) {
				return true;
			}
			if (strokeWidth != paintExt.strokeWidth) {
				return true;
			}
			if(    viewPort.x != paintExt.viewPort.x
			    || viewPort.y != paintExt.viewPort.y) {
				return true;
			}
			if(    matrix[0] != paintExt.matrix[0]
			    || matrix[1] != paintExt.matrix[1]
			    || matrix[2] != paintExt.matrix[2]
			    || matrix[3] != paintExt.matrix[3]
			    || matrix[4] != paintExt.matrix[4]
			    || matrix[5] != paintExt.matrix[5]) {
				return true;
			}
			return false;
		}
	};
	
	class Base
	{
		protected:
			PaintState m_paint;
			const char * SpacingDist(int32_t spacing);
		public:
			Base(void) {};
			Base(PaintState parentPaintState);
			~Base(void) { };
			virtual bool Parse(TiXmlNode * node);
			//specific drawing for AAG librairy ...
			virtual void AggDraw(agg::path_storage& path, etk::VectorType<agg::rgba8> &colors, etk::VectorType<uint32_t> &pathIdx, PaintState &curentPaintProp) { };
			virtual void AggCheckChange(agg::path_storage& path, etk::VectorType<agg::rgba8> &colors, etk::VectorType<uint32_t> &pathIdx, PaintState &curentPaintProp);
			
			virtual void Display(int32_t spacing) { };
			void ParseTransform(TiXmlNode *node);
			void ParsePosition(const TiXmlNode *node, coord2D_ts &pos, coord2D_ts &size);
			etkFloat_t ParseLength(const char *dataInput);
			void ParsePaintAttr(const TiXmlNode *node);
			color8_ts ParseColor(const char *inputData);
	};
};

#endif