/**
 *******************************************************************************
 * @file ewol/oObject/3DTextured.h
 * @brief ewol OpenGl Object system (header)
 * @author Edouard DUPIN
 * @date 29/08/2012
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

#ifndef __EWOL_O_OBJECT_3D_TEXTURED_H__
#define __EWOL_O_OBJECT_3D_TEXTURED_H__

#include <ewol/oObject/OObject.h>
#include <ewol/ResourceManager.h>

namespace ewol {
	class OObject3DTextured :public ewol::OObject
	{
		public:
			OObject3DTextured(etk::UString textureName, float sizeX=-1, float sizeY=-1);
			virtual ~OObject3DTextured(void);
		public:
			virtual void Draw(void);
			void Clear(void);
			void Rectangle(float x, float y, float w, float h, float texX=0.0, float texY=0.0, float texSX=1.0, float texSY=1.0, draw::Color tmpColor=draw::color::white);
			void Rectangle(float x, float y, float w, float h, draw::Color tmpColor);
		protected:
			#ifdef __VIDEO__OPENGL_ES_2
				ewol::Program* m_GLprogram;
				GLint          m_GLPosition;
				GLint          m_GLMatrix;
				GLint          m_GLColor;
				GLint          m_GLtexture;
				GLint          m_GLtexID;
			#endif
			ewol::TextureFile*              m_resource;    //!< texture resources
			etk::Vector<Vector3D<float> >   m_coord;       //!< internal coord of the object
			etk::Vector<texCoord_ts>        m_coordTex;    //!< internal texture coordinate for every point
			#ifdef __VIDEO__OPENGL_ES_2
				etk::Vector<draw::Colorf>   m_coordColor;  //!< internal color of the different point
			#else
				etk::Vector<draw::Color>    m_coordColor;  //!< internal color of the different point
			#endif
	};
};

#endif

