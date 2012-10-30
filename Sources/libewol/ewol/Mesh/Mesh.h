/**
 *******************************************************************************
 * @file ewol/Mesh/Mesh.h
 * @brief ewol Mesh system (header)
 * @author Edouard DUPIN
 * @date 30/10/2012
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

#ifndef __MESH_H__
#define __MESH_H__

#include <etk/Types.h>
#include <ewol/Resource.h>
#include <ewol/texture/TextureFile.h>
#include <ewol/openGL/Shader.h>
#include <ewol/openGL/Program.h>

namespace ewol
{
	class Mesh : public ewol::Resource
	{
		protected:
			#ifdef __VIDEO__OPENGL_ES_2
				ewol::Program* m_GLprogram;
				int32_t        m_GLPosition;
				int32_t        m_GLMatrix;
				int32_t        m_GLColor;
				int32_t        m_GLtexture;
				int32_t        m_GLtexID;
			#endif
			etk::Vector<uint32_t> m_indices;
			etk::Vector< etk::Vector3D<float> > m_vertices;
			etk::Vector< etk::Vector2D<float> > m_uvTextures;
			etk::Vector< etk::Vector3D<float> > m_normals;
			ewol::TextureFile*                  m_texture1;
			#ifdef __VIDEO__OPENGL_ES_2
				etk::Vector<draw::Colorf>        m_coordColor;  //!< internal color of the different point
			#else
				etk::Vector<draw::Color>         m_coordColor;  //!< internal color of the different point
			#endif
		public:
			Mesh(etk::UString genName);
			virtual ~Mesh(void);
			virtual const char* GetType(void) { return "ewol::Mesh"; };
			virtual void Draw(void);
			
	};
};


#endif
