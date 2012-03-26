/**
 *******************************************************************************
 * @file parserSVG/Path.h
 * @brief basic path parsing (Header)
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

#ifndef __SVG_PATH_H__
#define __SVG_PATH_H__

#include <parserSVG/Base.h>

namespace svg
{
	typedef enum {
		PATH_ENUM_STOP,
		PATH_ENUM_GOTO,
		PATH_ENUM_LINETO,
		PATH_ENUM_CURVETO,
		PATH_ENUM_SMOTH_CURVETO,
		PATH_ENUM_BEZIER_CURVETO,
		PATH_ENUM_BEZIER_SMOTH_CURVETO,
		PATH_ENUM_ELLIPTIC,
	} pathEnum_te;
	
	typedef struct {
		pathEnum_te cmd;
		union {
			struct{
				etkFloat_t x;
				etkFloat_t y;
			} position_s;
			struct{
				etkFloat_t x1;
				etkFloat_t y1;
				etkFloat_t x2;
				etkFloat_t y2;
				etkFloat_t x3;
				etkFloat_t y3;
			}curveto_s;
			struct{
				etkFloat_t x1;
				etkFloat_t y1;
				etkFloat_t x2;
				etkFloat_t y2;
			}shorthandSmoothCurveto_s;
			struct{
				etkFloat_t x1;
				etkFloat_t y1;
				etkFloat_t x2;
				etkFloat_t y2;
			}quadraticBezierCurveto_s;
			struct{
				etkFloat_t x;
				etkFloat_t y;
			} shorthandSmoothQuadraticBezierCurveto_s;
			struct{
				etkFloat_t rx;
				etkFloat_t ry;
				etkFloat_t rotation;
				etkFloat_t large_arc;
				etkFloat_t sweep;
				etkFloat_t x;
				etkFloat_t y;
			}ellipticArc_s;
		};
	}pathBasic_ts;
	
	class Path : public svg::Base
	{
		private:
			etk::VectorType<pathBasic_ts> m_listElement;
		public:
			Path(PaintState parentPaintState);
			~Path(void);
			virtual bool Parse(TiXmlNode * node, agg::trans_affine& parentTrans);
			virtual void Display(int32_t spacing);
	};
};

#endif

