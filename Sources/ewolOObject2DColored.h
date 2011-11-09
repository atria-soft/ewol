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
			OObject2DColored(void) {};
			virtual ~OObject2DColored(void) {};
		public:
			virtual void Draw(void);
		protected:
			etk::VectorType<coord2D_ts>   m_coord;       //!< internal coord of the object
			etk::VectorType<color_ts>     m_coordColor;  //!< internal color of the different point
			//etk::VectorType<linkCoord_ts> m_linkCoord;   //!< internal link between point to generate triangle
		public:
			void Rectangle(float x, float y, float w, float h, float red, float green, float blue, float alpha);
		public:
			virtual void UpdateOrigin(float x, float y);
	};
};

#endif

