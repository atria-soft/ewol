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
#include <etk/Hach.h>
#include <ewol/resources/Resource.h>
#include <ewol/resources/Image.h>
#include <ewol/resources/Shader.h>
#include <ewol/resources/Program.h>
#include <ewol/resources/VirtualBufferObject.h>
#include <ewol/Light.h>
#include <ewol/Material.h>
#include <ewol/physicsShape/PhysicsShape.h>
// 3 "float" elements
#define MESH_VBO_VERTICES  (0)
// 2 "float" elements
#define MESH_VBO_TEXTURE   (1)
// 3 "float" elements
#define MESH_VBO_VERTICES_NORMAL    (2)
// Face normal position :
#define MESH_VBO_FACE_NORMAL       (3)
// 4 "float" elements
#define MESH_VBO_COLOR     (4)

namespace ewol
{
	class Face
	{
		public:
			int32_t m_vertex[3];
			int32_t m_uv[3];
			int32_t m_normal[3];
		public:
			Face(void) {};
			Face(int32_t v1, int32_t t1,
			     int32_t v2, int32_t t2,
			     int32_t v3, int32_t t3)
			{
				m_vertex[0] = v1;
				m_vertex[1] = v2;
				m_vertex[2] = v3;
				m_uv[0] = t1;
				m_uv[1] = t2;
				m_uv[2] = t3;
				m_normal[0] = -1;
				m_normal[1] = -1;
				m_normal[2] = -1;
			};
			Face(int32_t v1, int32_t t1, int32_t n1,
			     int32_t v2, int32_t t2, int32_t n2,
			     int32_t v3, int32_t t3, int32_t n3)
			{
				m_vertex[0] = v1;
				m_vertex[1] = v2;
				m_vertex[2] = v3;
				m_uv[0] = t1;
				m_uv[1] = t2;
				m_uv[2] = t3;
				m_normal[0] = n1;
				m_normal[1] = n2;
				m_normal[2] = n3;
			};
	};
	class FaceIndexing
	{
		public:
			etk::Vector<Face> m_faces;
			etk::Vector<uint32_t> m_index;
	};
	class Mesh : public ewol::Resource
	{
		public:
			typedef enum {
				normalModeNone,
				normalModeFace,
				normalModeVertex,
			} normalMode_te;
		private:
			normalMode_te m_normalMode; // select the normal mode of display
			bool m_checkNormal; //!< when enable, this check the normal of the mesh before sending it at the 3d card
		protected:
			ewol::Program* m_GLprogram;
			int32_t        m_GLPosition;
			int32_t        m_GLMatrix;
			int32_t        m_GLMatrixPosition;
			int32_t        m_GLNormal;
			int32_t        m_GLtexture;
			int32_t        m_bufferOfset;
			int32_t        m_numberOfElments;
			MaterialGlId   m_GLMaterial;
			ewol::Light    m_light;
		protected:
			etk::Vector<vec3> m_listVertex; //!< List of all vertex in the element
			etk::Vector<vec2> m_listUV; //!< List of all UV point in the mesh (for the specify texture)
			etk::Vector<vec3> m_listFacesNormal; //!< List of all Face normal, when calculated
			etk::Vector<vec3> m_listVertexNormal; //!< List of all Face normal, when calculated
			etk::Hash<FaceIndexing> m_listFaces; //!< List of all Face for the mesh
			etk::Hash<ewol::Material*> m_materials;
			etk::Vector<ewol::PhysicsShape*> m_physics; //!< collision shape module ... (independent of bullet lib)
		protected:
			ewol::VirtualBufferObject* m_verticesVBO;
		public:
			Mesh(const etk::UString& _fileName, const etk::UString& _shaderName="DATA:textured3D2.prog");
			virtual ~Mesh(void);
			virtual const char* GetType(void) { return "ewol::Mesh"; };
			virtual void Draw(mat4& positionMatrix);
			void GenerateVBO(void);
		private:
			void CalculateNormaleFace(void);
			void CalculateNormaleEdge(void);
		public :
			void CreateViewBox(const etk::UString& _materialName,float _size=1.0);
		private:
			bool LoadOBJ(const etk::UString& _fileName);
			bool LoadEMF(const etk::UString& _fileName);
		public:
			void AddMaterial(const etk::UString& _name, ewol::Material* _data);
		public:
			
			const etk::Vector<ewol::PhysicsShape*>& GetPhysicalProperties(void) const { return m_physics; };
		private:
			void* m_pointerShape; //!< all mesh have a basic shape (bullet or other) the void pointer mermit to not depent on the bullet lib
		public:
			/**
			 * @brief Set the shape pointer (no type ==> user might know it ...)
			 * @param[in] _shape The new shape (this remove the previous one)
			 */
			void SetShape(void* _shape);
			/**
			 * @brief Get the pointer on the shame (no type)
			 * @return Pointer on shape.
			 */
			void* GetShape(void) { return m_pointerShape; };
		private:
			void (*m_functionFreeShape)(void* _pointer);
		public:
			void SetFreeShapeFunction(void (*_functionFreeShape)(void* _pointer)) { m_functionFreeShape = _functionFreeShape; };
	};
};


#endif
