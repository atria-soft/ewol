/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __MESH_H__
#define __MESH_H__

#include <etk/types.h>
#include <ewol/renderer/resources/Resource.h>
#include <ewol/renderer/resources/Image.h>
#include <ewol/renderer/resources/Shader.h>
#include <ewol/renderer/resources/Program.h>

namespace ewol
{
	class Mesh : public ewol::Resource
	{
		protected:
			ewol::Program* m_GLprogram;
			int32_t        m_GLPosition;
			int32_t        m_GLMatrix;
			int32_t        m_GLColor;
			int32_t        m_GLtexture;
			int32_t        m_GLtexID;
		public:
			etk::Vector<uint32_t> m_indices;
			etk::Vector<vec3>     m_vertices;
			etk::Vector<vec2>     m_uvTextures;
			etk::Vector<vec3>     m_normals;
		protected:
			ewol::TextureFile*         m_texture1;
			etk::Vector<draw::Colorf>  m_coordColor;  //!< internal color of the different point
		public:
			Mesh(etk::UString genName);
			virtual ~Mesh(void);
			virtual const char* GetType(void) { return "ewol::Mesh"; };
			virtual void Draw(mat4& positionMatrix);
			
	};
};


#endif
