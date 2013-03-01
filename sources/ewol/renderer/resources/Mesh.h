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
#include <ewol/renderer/resources/VirtualBufferObject.h>

// note using modify Half-Edge system to store data (modify is for storing UV mapping too
// help on : http://www.flipcode.com/archives/The_Half-Edge_Data_Structure.shtml

namespace ewol
{
	class Face
	{
		public:
			int32_t m_nbElement;
			int32_t m_vertex[4];
			int32_t m_uv[4];
	};
	class Mesh : public ewol::Resource
	{
		// 3 "float" elements
		#define MESH_VBO_VERTICES  (0)
		// 2 "float" elements
		#define MESH_VBO_TEXTURE   (1)
		// 4 "float" elements
		#define MESH_VBO_COLOR     (2)
		// 3 "float" elements
		#define MESH_VBO_NORMAL    (3)
		// TODO : Use indice system ...
		protected:
			ewol::Program* m_GLprogram;
			int32_t        m_GLPosition;
			int32_t        m_GLMatrix;
			int32_t        m_GLColor;
			int32_t        m_GLtexture;
			int32_t        m_GLtexID;
			int32_t        m_bufferOfset;
			int32_t        m_numberOfElments;
		protected:
			etk::Vector<vec3> m_listVertex;
			etk::Vector<vec2> m_listUV;
			etk::Vector<Face> m_listFaces;
			
		public: // For display storage : (not really usable for mathématical division and other ...
			etk::Vector<uint32_t>      m_indices;
			etk::Vector<vec3>          m_vertices;
			etk::Vector<vec2>          m_uvTextures;
			etk::Vector<vec3>          m_normals;
			etk::Vector<draw::Colorf>  m_coordColor;  //!< internal color of the different point
		protected:
			ewol::VirtualBufferObject*  m_verticesVBO;
			ewol::TextureFile*          m_texture1;
		public:
			Mesh(etk::UString genName);
			virtual ~Mesh(void);
			virtual const char* GetType(void) { return "ewol::Mesh"; };
			virtual void Draw(mat4& positionMatrix);
			void GenerateVBO(void);
			
	};
};


#endif
