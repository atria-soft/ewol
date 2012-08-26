/**
 *******************************************************************************
 * @file ewol/oObject/2DTextured.h
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

#ifndef __EWOL_O_OBJECT_2D_TEXTURED_H__
#define __EWOL_O_OBJECT_2D_TEXTURED_H__

#include <ewol/oObject/OObject.h>
#include <ewol/texture/Texture.h>

namespace ewol {
	class OObject2DTextured :public ewol::OObject
	{
		public:
			OObject2DTextured(etk::UString textureName, float sizeX=-1, float sizeY=-1);
			virtual ~OObject2DTextured(void);
		public:
			virtual void Draw(void);
			void Clear(void);
			void Rectangle(float x, float y, float w, float h, float texX=0.0, float texY=0.0, float texSX=1.0, float texSY=1.0, draw::Color tmpColor=draw::color::white);
			void Rectangle(float x, float y, float w, float h, draw::Color tmpColor);
		protected:
			ewol::Texture*                  m_resource;    //!< texture resources
			etk::Vector<Vector2D<float> >   m_coord;       //!< internal coord of the object
			etk::Vector<texCoord_ts>        m_coordTex;    //!< internal texture coordinate for every point
			etk::Vector<draw::Color>        m_coordColor;  //!< internal color of the different point
	};
};

#endif
