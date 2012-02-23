/**
 *******************************************************************************
 * @file ewol/OObject/2DColored.h
 * @brief ewol OpenGl Object system (header)
 * @author Edouard DUPIN
 * @date 09/11/2011
 * @par Project
 * ewol
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

#ifndef __EWOL_O_OBJECT_2D_COLORED_H__
#define __EWOL_O_OBJECT_2D_COLORED_H__

#include <ewol/OObject.h>

namespace ewol {
	class OObject2DColored :public ewol::OObject
	{
		public:
			OObject2DColored(void);
			virtual ~OObject2DColored(void);
		public:
			virtual void Draw(void);
		protected:
			etk::VectorType<coord2D_ts>   m_coord;       //!< internal coord of the object
			etk::VectorType<color_ts>     m_coordColor;  //!< internal color of the different point
			//etk::VectorType<linkCoord_ts> m_linkCoord;   //!< internal link between point to generate triangle
			int32_t    m_triElement;
			color_ts   m_color[3];
			coord2D_ts m_triangle[3];
			void GenerateTriangle(void);
			void ResetCount(void);
		public:
			void Clear(void);
			void SetColor(etkFloat_t red, etkFloat_t green, etkFloat_t blue, etkFloat_t alpha = 1.0);
			void SetColor(color_ts color);
			void SetPoint(coord2D_ts point);
			void SetPoint(etkFloat_t x, etkFloat_t y);
			void Line(etkFloat_t sx, etkFloat_t sy, etkFloat_t ex, etkFloat_t ey, etkFloat_t thickness);
			void Rectangle(etkFloat_t x, etkFloat_t y, etkFloat_t w, etkFloat_t h);
			void RectangleBorder(etkFloat_t x, etkFloat_t y, etkFloat_t w, etkFloat_t h, etkFloat_t thickness);
			void Circle(etkFloat_t x, etkFloat_t y, etkFloat_t radius, etkFloat_t thickness);
			void CirclePart(etkFloat_t x, etkFloat_t y, etkFloat_t radius, etkFloat_t thickness, etkFloat_t angleStart, etkFloat_t angleStop);
			void Disc(etkFloat_t x, etkFloat_t y, etkFloat_t radius);
			void DiscPart(etkFloat_t x, etkFloat_t y, etkFloat_t radius, etkFloat_t angleStart, etkFloat_t angleStop);
	};
};

#endif

