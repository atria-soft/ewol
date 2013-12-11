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
#include <etk/Hash.h>
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

namespace ewol {
	namespace resource {
		class Face {
			public:
				int32_t m_vertex[3];
				int32_t m_uv[3];
				int32_t m_normal[3];
			public:
				Face(void) {};
				Face(int32_t v1, int32_t t1,
				     int32_t v2, int32_t t2,
				     int32_t v3, int32_t t3) {
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
				     int32_t v3, int32_t t3, int32_t n3) {
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
		class FaceIndexing {
			public:
				std::vector<Face> m_faces;
				std::vector<uint32_t> m_index;
		};
		class Mesh : public ewol::resource::Resource {
			public:
				enum normalMode {
					normalModeNone,
					normalModeFace,
					normalModeVertex,
				};
			protected:
				enum normalMode m_normalMode; // select the normal mode of display
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
				std::vector<vec3> m_listVertex; //!< List of all vertex in the element
				std::vector<vec2> m_listUV; //!< List of all UV point in the mesh (for the specify texture)
				std::vector<vec3> m_listFacesNormal; //!< List of all Face normal, when calculated
				std::vector<vec3> m_listVertexNormal; //!< List of all Face normal, when calculated
				etk::Hash<FaceIndexing> m_listFaces; //!< List of all Face for the mesh
				etk::Hash<ewol::Material*> m_materials;
				std::vector<ewol::PhysicsShape*> m_physics; //!< collision shape module ... (independent of bullet lib)
			protected:
				ewol::VirtualBufferObject* m_verticesVBO;
			protected:
				Mesh(const std::string& _fileName, const std::string& _shaderName="DATA:textured3D2.prog");
				virtual ~Mesh(void);
			public:
				virtual const char* getType(void) { return "ewol::Mesh"; };
				virtual void draw(mat4& _positionMatrix, bool _enableDepthTest=true, bool _enableDepthUpdate=true);
				void generateVBO(void);
			private:
				void calculateNormaleFace(void);
				void calculateNormaleEdge(void);
			public :
				void createViewBox(const std::string& _materialName,float _size=1.0);
			private:
				bool loadOBJ(const std::string& _fileName);
				bool loadEMF(const std::string& _fileName);
			public:
				void addMaterial(const std::string& _name, ewol::Material* _data);
			public:
				/**
				 * @brief set the check of normal position befor sending it to the openGl card
				 * @param[in] _status New state.
				 */
				void setCheckNormal(bool _status) { m_checkNormal=_status; };
				/**
				 * @brief get the check value of normal position befor sending it to the openGl card
				 * @return get the chcking stus of normal or not
				 */
				bool getCheckNormal(void) { return m_checkNormal; };
				const std::vector<ewol::PhysicsShape*>& getPhysicalProperties(void) const { return m_physics; };
			private:
				void* m_pointerShape; //!< all mesh have a basic shape (bullet or other) the void pointer mermit to not depent on the bullet lib
			public:
				/**
				 * @brief set the shape pointer (no type  == > user might know it ...)
				 * @param[in] _shape The new shape (this remove the previous one)
				 */
				void setShape(void* _shape);
				/**
				 * @brief get the pointer on the shame (no type)
				 * @return Pointer on shape.
				 */
				void* getShape(void) { return m_pointerShape; };
			private:
				void (*m_functionFreeShape)(void* _pointer);
			public:
				void setFreeShapeFunction(void (*_functionFreeShape)(void* _pointer)) { m_functionFreeShape = _functionFreeShape; };
			public:
				/**
				 * @brief keep the resource pointer.
				 * @note Never free this pointer by your own...
				 * @param[in] _filename Name of the ewol mesh file.
				 * @return pointer on the resource or NULL if an error occured.
				 */
				static ewol::Mesh* keep(const std::string& _meshname);
				/**
				 * @brief release the keeped resources
				 * @param[in,out] reference on the object pointer
				 */
				static void release(ewol::Mesh*& _object);
		};
	};
};


#endif
