/**
 *******************************************************************************
 * @file parserSVG/Renderer.h
 * @brief Basic SVG renderer for the AGG librairy (Header)
 * @author Edouard DUPIN
 * @date 23/03/2012
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

#ifndef __SVG_RENDERER_H__
#define __SVG_RENDERER_H__

#include <etk/UString.h>

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
			etkFloat_t opacity;   //!< opacity on the result drawing ...
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
	
	// basic definition type for the renderer
	typedef agg::renderer_base<agg::pixfmt_rgba32>           rendererBase_t;
	typedef agg::renderer_scanline_aa_solid<rendererBase_t>  rendererSolid_t;
	
	class Renderer {
		private:
			uint8_t *                     m_buffer;
		public:
			Renderer(uint32_t width, uint32_t height);
			~Renderer(void);
			void WritePpm(etk::UString fileName);
			coord2D_ts                    m_size;
			agg::rendering_buffer *       m_renderingBuffer;
			agg::pixfmt_rgba32 *          m_pixFrame;
			rendererBase_t *              m_renderBase;
			rendererSolid_t *             m_renderArea;
			agg::rasterizer_scanline_aa<> m_rasterizer;  //!< AGG renderer system
			agg::scanline_p8              m_scanLine;    //!< 
			agg::trans_affine             m_basicMatrix; //!< specific render of the curent element
	};
};

#endif
