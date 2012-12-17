/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __GAME_MESH_OBJECT_H__
#define __GAME_MESH_OBJECT_H__

#include <etk/types.h>
#include <etk/Vector.h>
#include <etk/math/Vector3D.h>
#include <etk/math/Vector2D.h>

namespace game
{
	class MeshObject
	{
		public:
			etk::Vector<uint32_t> m_indices;
			etk::Vector<vec3>     m_vertices;
			etk::Vector<vec2>     m_uvTextures;
			etk::Vector<vec3>     m_normals;
		public:
			MeshObject(void) {};
			virtual ~MeshObject(void) {};
	};
};


#endif