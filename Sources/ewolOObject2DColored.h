/**
 *******************************************************************************
 * @file ewolOObject2DColored.h
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

#include <ewolOObject.h>

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
			color_ts m_Color;
		public:
			void SetColor(float red, float green, float blue, float alpha = 1.0);
			void SetColor(color_ts color);
			//void SetPoint(coord2D_ts point); //==> for direct adding custum element ...
			void Line(float sx, float sy, float ex, float ey, float thickness);
			void Rectangle(float x, float y, float w, float h);
			//void RectangleAngle(float x, float y, float w, float h, float angle);
			//void RectangleBorder(float x, float y, float w, float h, float thickness);
			//void RectangleBorder(float x, float y, float w, float h, float thickness, float angle);
			void Circle(float x, float y, float radius, float thickness);
			//void CirclePart(float x, float y, float radius, float thickness, float angleStart, float angleStop);
			void Disc(float x, float y, float radius);
			//void DiscPart(float x, float y, float radius, float angleStart, float angleStop);
		public:
			// Ewol internal ... : done to update at the origin of the widget ...
			virtual void UpdateOrigin(float x, float y);
	};
};

#endif

