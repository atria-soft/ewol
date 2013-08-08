/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/renderer/resources/Mesh.h>
#include <ewol/renderer/ResourceManager.h>
#include <etk/os/FSNode.h>

#undef __class__
#define __class__	"Mesh"

typedef enum {
	VERTEX_OLD,
	VERTEX_CENTER_FACE,
	VERTEX_CERTER_EDGE
} vertex_te;

class VertexNode {
	private:
		vertex_te            m_type;
		vec3                 m_pos;
		etk::Vector<int32_t> m_link;
	public:
		VertexNode(vertex_te type, const vec3& pos) :
			m_type(type),
			m_pos(pos)
		{
			
		};
		void AddLink(int32_t id)
		{
			for(int32_t iii=0; iii<m_link.Size(); iii++) {
				if (m_link[iii] == id) {
					return;
				}
			}
			m_link.PushBack(id);
		};
		const vec3& GetPos(void)
		{
			return m_pos;
		};
		void SetPos(const vec3& pos)
		{
			m_pos = pos;
		};
		const vertex_te GetType(void)
		{
			return m_type;
		};
		etk::Vector<int32_t>& GetLink(void)
		{
			return m_link;
		};
	
};



ewol::Mesh::Mesh(const etk::UString& _fileName, const etk::UString& _shaderName) :
	ewol::Resource(_fileName),
	m_normalMode(normalModeNone)
{
	EWOL_DEBUG("Load a new mesh : '" << _fileName << "'");
	// get the shader resource :
	m_GLPosition = 0;
	
	m_light.SetDirection(vec3(0,cos(M_PI/4),sin(M_PI/4)));
	m_light.SetHalfPlane(vec3(1,0,0));
	m_light.SetAmbientColor(vec4(1,1,1,1));
	m_light.SetDiffuseColor(vec4(1.0,1.0,1.0,1));
	m_light.SetSpecularColor(vec4(0.0,0.0,0.0,1));
	
	if (true == ewol::resource::Keep(_shaderName, m_GLprogram) ) {
		m_GLPosition = m_GLprogram->GetAttribute("EW_coord3d");
		m_GLtexture = m_GLprogram->GetAttribute("EW_texture2d");
		m_GLNormal = m_GLprogram->GetAttribute("EW_normal");
		m_GLNormalFace = m_GLprogram->GetAttribute("EW_faceNormal");
		m_GLMatrix = m_GLprogram->GetUniform("EW_MatrixTransformation");
		m_GLMatrixPosition = m_GLprogram->GetUniform("EW_MatrixPosition");
		// Link material and Lights
		m_GLMaterial.Link(m_GLprogram, "EW_material");
		m_light.Link(m_GLprogram, "EW_directionalLight");
	}
	// this is the properties of the buffer requested : "r"/"w" + "-" + buffer type "f"=flaot "i"=integer
	ewol::resource::Keep("w-fff", m_verticesVBO);
	
	// load the curent file :
	etk::UString tmpName = _fileName.ToLower();
	// select the corect Loader :
	if (true == tmpName.EndWith(".obj") ) {
		if (false == LoadOBJ(_fileName)) {
			EWOL_ERROR("Error To load OBJ file " << tmpName );
			return;
		}
	} else if (true == tmpName.EndWith(".emf") ) {
		if (false == LoadEMF(_fileName)) {
			EWOL_ERROR("Error To load EMF file " << tmpName );
			return;
		}
		//EWOL_CRITICAL("Load a new mesh : '" << _fileName << "' (end)");
	} else {
		// nothing to do ==> reqiest an enmpty mesh ==> user manage it ...
	}
}

ewol::Mesh::~Mesh(void)
{
	// remove dynamics dependencies :
	ewol::resource::Release(m_GLprogram);
	ewol::resource::Release(m_verticesVBO);
}


void ewol::Mesh::Draw(mat4& positionMatrix)
{
	//EWOL_DEBUG("Request Draw : " << m_listFaces.GetValue(0).m_index.Size() << " elements");
	/*
	if (m_listIndexFaces.Size()<=0) {
		return;
	}
	*/
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	ewol::openGL::Enable(ewol::openGL::FLAG_DEPTH_TEST);
	//EWOL_DEBUG("    Display " << m_coord.Size() << " elements" );
	m_GLprogram->Use();
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = ewol::openGL::GetMatrix();
	mat4 camMatrix = ewol::openGL::GetCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix;
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	m_GLprogram->UniformMatrix4fv(m_GLMatrixPosition, 1, positionMatrix.m_mat);
	// position :
	m_GLprogram->SendAttributePointer(m_GLPosition, 3/*x,y,z*/, m_verticesVBO, MESH_VBO_VERTICES);
	// Texture :
	m_GLprogram->SendAttributePointer(m_GLtexture, 2/*u,v*/, m_verticesVBO, MESH_VBO_TEXTURE);
	// position :
	m_GLprogram->SendAttributePointer(m_GLNormal, 3/*x,y,z*/, m_verticesVBO, MESH_VBO_VERTICES_NORMAL);
	// draw lights :
	m_light.Draw(m_GLprogram);
	
	int32_t nbElementDraw = 0;
	for (esize_t kkk=0; kkk<m_listFaces.Size(); kkk++) {
		if (false == m_materials.Exist(m_listFaces.GetKey(kkk))) {
			EWOL_WARNING("missing materials : '" << m_listFaces.GetKey(kkk) << "'");
			continue;
		}
		m_materials[m_listFaces.GetKey(kkk)]->Draw(m_GLprogram, m_GLMaterial);
		ewol::openGL::DrawElements(GL_TRIANGLES, m_listFaces.GetValue(kkk).m_index);
		nbElementDraw += m_listFaces.GetValue(kkk).m_index.Size();
	}
	EWOL_DEBUG("Request Draw : " << m_listFaces.Size() << ":" << nbElementDraw << " elements [" << m_name << "]");
	m_GLprogram->UnUse();
	ewol::openGL::Disable(ewol::openGL::FLAG_DEPTH_TEST);
	// TODO : UNDERSTAND why ... it is needed
	glBindBuffer(GL_ARRAY_BUFFER,0);
}
void ewol::Mesh::Draw2(mat4& positionMatrix)
{
	//
	/*
	if (m_listIndexFaces.Size()<=0) {
		return;
	}
	*/
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	ewol::openGL::Enable(ewol::openGL::FLAG_DEPTH_TEST);
	//EWOL_DEBUG("    Display " << m_coord.Size() << " elements" );
	m_GLprogram->Use();
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = ewol::openGL::GetMatrix();
	mat4 tmpMatrix = projMatrix;
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	m_GLprogram->UniformMatrix4fv(m_GLMatrixPosition, 1, positionMatrix.m_mat);
	// position :
	m_GLprogram->SendAttributePointer(m_GLPosition, 3/*x,y,z*/, m_verticesVBO, MESH_VBO_VERTICES);
	// Texture :
	m_GLprogram->SendAttributePointer(m_GLtexture, 2/*u,v*/, m_verticesVBO, MESH_VBO_TEXTURE);
	// position :
	m_GLprogram->SendAttributePointer(m_GLNormal, 3/*x,y,z*/, m_verticesVBO, MESH_VBO_VERTICES_NORMAL);
	m_light.Draw(m_GLprogram);
	
	// draw materials :
	int32_t nbElementDraw = 0;
	for (esize_t kkk=0; kkk<m_listFaces.Size(); kkk++) {
		if (false == m_materials.Exist(m_listFaces.GetKey(kkk))) {
			EWOL_WARNING("missing materials : '" << m_listFaces.GetKey(kkk) << "'");
			continue;
		}
		m_materials[m_listFaces.GetKey(kkk)]->Draw(m_GLprogram, m_GLMaterial);
		ewol::openGL::DrawElements(GL_TRIANGLES, m_listFaces.GetValue(kkk).m_index);
		nbElementDraw += m_listFaces.GetValue(kkk).m_index.Size();
	}
	EWOL_DEBUG("Request Draw : " << m_listFaces.Size() << ":" << nbElementDraw << " elements [" << m_name << "]");
	m_GLprogram->UnUse();
	ewol::openGL::Disable(ewol::openGL::FLAG_DEPTH_TEST);
	// TODO : UNDERSTAND why ... it is needed
	glBindBuffer(GL_ARRAY_BUFFER,0);
}


// normal calculation of the normal face is really easy :
void ewol::Mesh::CalculateNormaleFace(void)
{
	m_listFacesNormal.Clear();
	if (m_normalMode != ewol::Mesh::normalModeFace) {
		etk::Vector<Face>& tmpFaceList = m_listFaces.GetValue(0).m_faces;
		for(int32_t iii=0 ; iii<tmpFaceList.Size() ; iii++) {
			// for all case, We use only the 3 vertex for quad element, in theory 3D modeler export element in triangle if it is not a real plane.
			vec3 normal = btCross(m_listVertex[tmpFaceList[iii].m_vertex[0]]-m_listVertex[tmpFaceList[iii].m_vertex[1]],
			                      m_listVertex[tmpFaceList[iii].m_vertex[1]]-m_listVertex[tmpFaceList[iii].m_vertex[2]]);
			m_listFacesNormal.PushBack(normal.normalized());
		}
		m_normalMode = ewol::Mesh::normalModeFace;
	}
}

void ewol::Mesh::CalculateNormaleEdge(void)
{
	m_listVertexNormal.Clear();
	if (m_normalMode != ewol::Mesh::normalModeVertex) {
		for(int32_t iii=0 ; iii<m_listVertex.Size() ; iii++) {
			etk::Vector<Face>& tmpFaceList = m_listFaces.GetValue(0).m_faces;
			vec3 normal(0,0,0);
			// add the vertex from all the element in the list for face when the element in the face ...
			for(int32_t jjj=0 ; jjj<tmpFaceList.Size() ; jjj++) {
				if(    tmpFaceList[jjj].m_vertex[0] == iii
				    || tmpFaceList[jjj].m_vertex[1] == iii
				    || tmpFaceList[jjj].m_vertex[2] == iii) {
					normal += m_listFacesNormal[jjj];
				}
			}
			if (normal == vec3(0,0,0)) {
				m_listVertexNormal.PushBack(vec3(1,1,1));
			} else {
				m_listVertexNormal.PushBack(normal.normalized());
			}
		}
		m_normalMode = ewol::Mesh::normalModeVertex;
	}
}

// for debugging ...
//#define PRINT_HALF (1)
//#define TRY_MINIMAL_VBO

void ewol::Mesh::GenerateVBO(void)
{
	// calculate the normal of all faces if needed
	if (m_normalMode == ewol::Mesh::normalModeNone) {
		// when no normal detected ==> auto Generate Face normal ....
		CalculateNormaleFace();
	}
	// Generate element in 2 pass : 
	//    - create new index dependeng a vertex is a unique componenet of position, texture, normal
	//    - the index list generation (can be dynamic ... (TODO later)
	for (esize_t kkk=0; kkk<m_listFaces.Size(); kkk++) {
		// clean faces indexes :
		m_listFaces.GetValue(kkk).m_index.Clear();
		#ifdef TRY_MINIMAL_VBO
			int64_t tmpppppp=0;
		#endif
		FaceIndexing& tmpFaceList = m_listFaces.GetValue(kkk);
		for (int32_t iii=0; iii<tmpFaceList.m_faces.Size() ; iii++) {
			int32_t vertexVBOId[3];
			for(int32_t indice=0 ; indice<3; indice++) {
				vec3 position = m_listVertex[tmpFaceList.m_faces[iii].m_vertex[indice]];
				vec3 normal;
				if (m_normalMode == ewol::Mesh::normalModeVertex) {
					normal = m_listVertexNormal[tmpFaceList.m_faces[iii].m_normal[indice]];
				} else {
					normal = m_listFacesNormal[tmpFaceList.m_faces[iii].m_normal[indice]];
				}
				vec2 texturepos(m_listUV[tmpFaceList.m_faces[iii].m_uv[indice]].x(),1.0f-m_listUV[tmpFaceList.m_faces[iii].m_uv[indice]].y());
				// try to find it in the list :
				bool elementFind = false;
				#ifdef TRY_MINIMAL_VBO
				for (int32_t jjj=0; jjj<m_verticesVBO->SizeOnBufferVec3(MESH_VBO_VERTICES); jjj++) {
					if(    m_verticesVBO->GetOnBufferVec3(MESH_VBO_VERTICES,jjj) == position
					    && m_verticesVBO->GetOnBufferVec3(MESH_VBO_VERTICES_NORMAL,jjj) == normal
					    && m_verticesVBO->GetOnBufferVec2(MESH_VBO_TEXTURE,jjj) == texturepos) {
						vertexVBOId[indice] = jjj;
						elementFind = true;
						//EWOL_DEBUG("search indice : " << jjj);
						tmpppppp += jjj;
						// stop searching ...
						break;
					}
				}
				#endif
				if (false == elementFind) {
					m_verticesVBO->PushOnBuffer(MESH_VBO_VERTICES, position);
					m_verticesVBO->PushOnBuffer(MESH_VBO_VERTICES_NORMAL, normal);
					m_verticesVBO->PushOnBuffer(MESH_VBO_TEXTURE, texturepos);
					vertexVBOId[indice] = m_verticesVBO->SizeOnBufferVec3(MESH_VBO_VERTICES)-1;
				}
			}
			for(int32_t indice=0 ; indice<3; indice++) {
				tmpFaceList.m_index.PushBack(vertexVBOId[indice]);
			}
		}
		#ifdef TRY_MINIMAL_VBO
			EWOL_DEBUG("nb cycle ? : " << tmpppppp);
		#endif
	}
	// update all the VBO elements ...
	m_verticesVBO->Flush();
}


void ewol::Mesh::CreateCube(float size)
{
	m_normalMode = ewol::Mesh::normalModeNone;
#if 0
	m_listVertex.Clear();
	m_listUV.Clear();
	m_listFaces.Clear();
	// This is the direct generation basis on the .obj system
	/*
			           5                       6  
			            o---------------------o   
			           /.                    /|   
			          / .                   / |   
			         /  .                  /  |   
			        /   .                 /   |   
			       /    .                /    |   
			    4 /     .               /     |   
			     o---------------------o      |   
			     |      .              |7     |   
			     |      .              |      |   
			     |      .              |      |   
			     |      .              |      |   
			     |      o..............|......o   
			     |     . 1             |     / 2  
			     |    .                |    /     
			     |   .                 |   /      
			     |  .                  |  /       
			     | .                   | /        
			     |.                    |/         
			     o---------------------o          
			    0                       3         
	*/
	m_listVertex.PushBack(vec3( size, -size, -size)); // 0
	m_listVertex.PushBack(vec3( size, -size,  size)); // 1
	m_listVertex.PushBack(vec3(-size, -size,  size)); // 2
	m_listVertex.PushBack(vec3(-size, -size, -size)); // 3
	m_listVertex.PushBack(vec3( size,  size, -size)); // 4
	m_listVertex.PushBack(vec3( size,  size,  size)); // 5
	m_listVertex.PushBack(vec3(-size,  size,  size)); // 6
	m_listVertex.PushBack(vec3(-size,  size, -size)); // 7
	
	m_listUV.PushBack(vec2(0.0, 0.0));
	m_listUV.PushBack(vec2(1.0, 0.0));
	m_listUV.PushBack(vec2(1.0, 1.0));
	m_listUV.PushBack(vec2(0.0, 1.0));
	
	
	m_listFaces.PushBack(Face(0,0, 1,1, 2,2, 3,3));
	m_listFaces.PushBack(Face(4,0, 0,1, 3,2, 7,3));
	m_listFaces.PushBack(Face(2,0, 6,1, 7,2, 3,3));
	m_listFaces.PushBack(Face(4,0, 7,1, 6,2, 5,3));
	m_listFaces.PushBack(Face(1,0, 5,1, 6,2, 2,3));
	m_listFaces.PushBack(Face(0,0, 4,1, 5,2, 1,3));
#endif
}

void ewol::Mesh::CreateViewBox(const etk::UString& _materialName,float _size)
{
	m_normalMode = ewol::Mesh::normalModeNone;
	// This is the direct generation basis on the .obj system
	/*
			           5                       6  
			            o---------------------o   
			           /.                    /|   
			          / .                   / |   
			         /  .                  /  |   
			        /   .                 /   |   
			       /    .                /    |   
			    4 /     .               /     |   
			     o---------------------o      |   
			     |      .              |7     |   
			     |      .              |      |   
			     |      .              |      |   
			     |      .              |      |   
			     |      o..............|......o   
			     |     . 1             |     / 2  
			     |    .                |    /     
			     |   .                 |   /      
			     |  .                  |  /       
			     | .                   | /        
			     |.                    |/         
			     o---------------------o          
			    0                       3         
	*/
	m_listVertex.PushBack(vec3( _size, -_size, -_size)); // 0
	m_listVertex.PushBack(vec3( _size, -_size,  _size)); // 1
	m_listVertex.PushBack(vec3(-_size, -_size,  _size)); // 2
	m_listVertex.PushBack(vec3(-_size, -_size, -_size)); // 3
	m_listVertex.PushBack(vec3( _size,  _size, -_size)); // 4
	m_listVertex.PushBack(vec3( _size,  _size,  _size)); // 5
	m_listVertex.PushBack(vec3(-_size,  _size,  _size)); // 6
	m_listVertex.PushBack(vec3(-_size,  _size, -_size)); // 7
	/*
		     o----------o----------o----------o
		     |8         |9         |10        |11
		     |          |          |          |
		     |          |          |          |
		     |    0     |    1     |    2     |
		     |          |          |          |
		     |          |          |          |
		     |          |          |          |
		     |          |          |          |
		     o----------o----------o----------o
		     |4         |5         |6         |7
		     |          |          |          |
		     |          |          |          |
		     |    3     |    4     |    5     |
		     |          |          |          |
		     |          |          |          |
		     |          |          |          |
		     |          |          |          |
		     o----------o----------o----------o
		     0          1          2          3
	*/
	m_listUV.PushBack(vec2(0.0    , 0.0    )); // 0
	m_listUV.PushBack(vec2(1.0/3.0, 0.0    )); // 1
	m_listUV.PushBack(vec2(2.0/3.0, 0.0    )); // 2
	m_listUV.PushBack(vec2(1.0    , 0.0    )); // 3
	m_listUV.PushBack(vec2(0.0    , 0.5    )); // 4
	m_listUV.PushBack(vec2(1.0/3.0, 0.5    )); // 5
	m_listUV.PushBack(vec2(2.0/3.0, 0.5    )); // 6
	m_listUV.PushBack(vec2(1.0    , 0.5    )); // 7
	m_listUV.PushBack(vec2(0.0    , 1.0    )); // 8
	m_listUV.PushBack(vec2(1.0/3.0, 1.0    )); // 9
	m_listUV.PushBack(vec2(2.0/3.0, 1.0    )); // 10
	m_listUV.PushBack(vec2(1.0    , 1.0    )); // 11
	
	if (m_listFaces.Exist(_materialName)==false) {
		FaceIndexing empty;
		m_listFaces.Add(_materialName, empty);
	}
	{
		FaceIndexing& tmpElement = m_listFaces[_materialName];
		tmpElement.m_faces.PushBack(Face(0,1, 1,5,  2,6)); // 4
		tmpElement.m_faces.PushBack(Face(0,1, 2,6,  3,2)); // 4
		tmpElement.m_faces.PushBack(Face(4,4, 0,0,  3,1)); // 3
		tmpElement.m_faces.PushBack(Face(4,4, 3,1,  7,5)); // 3
		tmpElement.m_faces.PushBack(Face(2,6, 6,10, 7,11)); // 2
		tmpElement.m_faces.PushBack(Face(2,6, 7,11, 3,7)); // 2
		tmpElement.m_faces.PushBack(Face(4,2, 7,3,  6,7)); // 5
		tmpElement.m_faces.PushBack(Face(4,2, 6,7,  5,6)); // 5
		tmpElement.m_faces.PushBack(Face(1,5, 5,9,  6,10)); // 1
		tmpElement.m_faces.PushBack(Face(1,5, 6,10, 2,6)); // 1
		tmpElement.m_faces.PushBack(Face(0,4, 4,8,  5,9)); // 0
		tmpElement.m_faces.PushBack(Face(0,4, 5,9,  1,5)); // 0
	}
	CalculateNormaleFace();
}

void ewol::Mesh::Subdivide(int32_t numberOfTime, bool smooth)
{
#if 0
	for(int32_t iii=0; iii<numberOfTime ; iii++) {
		InternalSubdivide(smooth);
	}
#endif
}

int32_t CreateOrGetNewPointId(vertex_te type, const vec3& point, etk::Vector<VertexNode*>& list)
{
#if 0
	for (int32_t iii=0; iii<list.Size(); iii++) {
		if (list[iii]->GetPos() == point) {
			return iii;
		}
	}
	VertexNode* tmpElement = new VertexNode(type, point);
	if (NULL==tmpElement) {
		EWOL_CRITICAL ("allocation error");
	}
	list.PushBack(tmpElement);
	return list.Size()-1;
#else
	return 0;
#endif
}

int32_t CreateOrGetNewTexId(const vec2& point, etk::Vector<vec2>& list)
{
#if 0
	for (int32_t iii=0; iii<list.Size(); iii++) {
		if (list[iii] == point) {
			return iii;
		}
	}
	list.PushBack(point);
	return list.Size()-1;
#else
	return 0;
#endif
}

void ewol::Mesh::InternalSubdivide(bool smooth)
{
#if 0
	//Copy the mesh for modify this one and not his parrent (that one is needed for smoothing)
	etk::Vector<VertexNode*> listVertex;
	for(int32_t iii=0; iii<m_listVertex.Size(); iii++) {
		VertexNode* tmpElement = new VertexNode(VERTEX_OLD, m_listVertex[iii]);
		listVertex.PushBack(tmpElement);
	}
	etk::Vector<vec2> listUV(m_listUV);
	etk::Vector<Face> listFaces; // no face here ...
	etk::Vector<int32_t> listElementHalfPoint(16);// preallocate at 16..
	etk::Vector<int32_t> listElementHalfUV(16);// preallocate at 16..
	
	for (int32_t iii=0; iii<m_listFaces.Size() ; iii++) {
		vec3 centerPoint;
		vec2 centerTex;
		if (3==m_listFaces[iii].m_nbElement) {
			// create the center point:
			centerPoint = (   m_listVertex[m_listFaces[iii].m_vertex[0]]
			                + m_listVertex[m_listFaces[iii].m_vertex[1]]
			                + m_listVertex[m_listFaces[iii].m_vertex[2]] ) / vec3(3,3,3);
			// create the center Texture coord:
			centerTex = (   listUV[m_listFaces[iii].m_uv[0]]
			              + listUV[m_listFaces[iii].m_uv[1]]
			              + listUV[m_listFaces[iii].m_uv[2]] ) / vec2(3,3);
			/*
			                                                         
			                o                          o             
			               / \                        / \            
			              /   \                      /   \           
			             /     \                    /     \          
			            /       \                  /       \         
			           /         \       ==>      o..     ..o        
			          /           \              /   ''o''   \       
			         /             \            /      |      \      
			        /               \          /       |       \     
			       /                 \        /        |        \    
			      o-------------------o      o---------o---------o   
			                                                         
			*/
		} else {
			// create the center point:
			centerPoint = (   m_listVertex[m_listFaces[iii].m_vertex[0]]
			                + m_listVertex[m_listFaces[iii].m_vertex[1]]
			                + m_listVertex[m_listFaces[iii].m_vertex[2]]
			                + m_listVertex[m_listFaces[iii].m_vertex[3]] ) / vec3(4,4,4);
			// create the center Texture coord:
			centerTex = (   listUV[m_listFaces[iii].m_uv[0]]
			              + listUV[m_listFaces[iii].m_uv[1]]
			              + listUV[m_listFaces[iii].m_uv[2]]
			              + listUV[m_listFaces[iii].m_uv[3]] ) / vec2(4,4);
			/*
			                                                              
			     o---------------------o         o----------o----------o  
			     |                     |         |          |          |  
			     |                     |         |          |          |  
			     |                     |         |          |          |  
			     |                     |         |          |          |  
			     |                     |         |          |          |  
			     |                     |   ==>   o----------o----------o  
			     |                     |         |          |          |  
			     |                     |         |          |          |  
			     |                     |         |          |          |  
			     |                     |         |          |          |  
			     |                     |         |          |          |  
			     o---------------------o         o----------o----------o  
			                                                              
			*/
		}
		
		int32_t newCenterVertexID = CreateOrGetNewPointId(VERTEX_CENTER_FACE, centerPoint, listVertex);
		int32_t newCenterTexID = CreateOrGetNewTexId(centerTex, listUV);
		
		listElementHalfPoint.Clear();
		listElementHalfUV.Clear();
		// generate list f the forder elements
		for (int32_t jjj=0; jjj<m_listFaces[iii].m_nbElement ; jjj++) {
			// for the last element finding at the good position...
			int32_t cyclicID = (jjj+1) % m_listFaces[iii].m_nbElement;
			
			listElementHalfPoint.PushBack(m_listFaces[iii].m_vertex[jjj]);
			listElementHalfUV.PushBack(m_listFaces[iii].m_uv[jjj]);
			// calculate and add middle point : 
			vec3 middlePoint = (   m_listVertex[m_listFaces[iii].m_vertex[jjj]]
			                     + m_listVertex[m_listFaces[iii].m_vertex[cyclicID]] ) / 2.0f;
			int32_t newMiddleVertexID = CreateOrGetNewPointId(VERTEX_CERTER_EDGE, middlePoint, listVertex);
			if (true==smooth) {
				// add center : at the middle point
				listVertex[newMiddleVertexID]->AddLink(newCenterVertexID);
				// add center of edge end face at the old element point :
				listVertex[m_listFaces[iii].m_vertex[jjj]]->AddLink(newCenterVertexID);
				listVertex[m_listFaces[iii].m_vertex[jjj]]->AddLink(newMiddleVertexID);
				listVertex[m_listFaces[iii].m_vertex[cyclicID]]->AddLink(newCenterVertexID);
				listVertex[m_listFaces[iii].m_vertex[cyclicID]]->AddLink(newMiddleVertexID);
			}
			// list of all middle point to recontitute the faces
			listElementHalfPoint.PushBack(newMiddleVertexID);
			// create the center Texture coord:
			vec2 middleTex = (   listUV[m_listFaces[iii].m_uv[jjj]]
			                   + listUV[m_listFaces[iii].m_uv[cyclicID]]) / 2.0f;
			int32_t newMiddleTexID = CreateOrGetNewTexId(middleTex, listUV);
			listElementHalfUV.PushBack(newMiddleTexID);
		}
		// generate faces:
		//EWOL_DEBUG(" ==> Generatedd faces");
		for (int32_t jjj=0; jjj<listElementHalfPoint.Size() ; jjj+=2) {
			int32_t cyclicID = (jjj-1 + listElementHalfPoint.Size()) % listElementHalfPoint.Size();
			listFaces.PushBack(Face(listElementHalfPoint[jjj], listElementHalfUV[jjj],
			                        listElementHalfPoint[jjj+1], listElementHalfUV[jjj+1],
			                        newCenterVertexID, newCenterTexID,
			                        listElementHalfPoint[cyclicID], listElementHalfUV[cyclicID]) );
		}
	}
	if (true==smooth) {
		//EWOL_DEBUG(" ==> Update middle edge points position");
		// reposition the Middle point of the edge
		for(int32_t iii=0; iii<listVertex.Size(); iii++) {
			if(NULL == listVertex[iii]) {
				continue;
			}
			if (VERTEX_CERTER_EDGE == listVertex[iii]->GetType()) {
				// generate barycenter of all link point if > 1...
				etk::Vector<int32_t>& link = listVertex[iii]->GetLink();
				if (1<link.Size()) {
					int32_t nbDivide = 1;
					vec3 posBase = listVertex[iii]->GetPos();
					for(int32_t jjj=0; jjj<link.Size(); jjj++) {
						if (link[jjj] >= listVertex.Size()) {
							EWOL_ERROR("Id is out of bounds ... : " << iii << " link=" << link[jjj] << " / " << listVertex.Size());
							continue;
						}
						if(NULL == listVertex[link[jjj]]) {
							continue;
						}
						if (VERTEX_CENTER_FACE != listVertex[link[jjj]]->GetType()) {
							EWOL_ERROR("Center face error type ..." << (int32_t)listVertex[link[jjj]]->GetType());
							continue;
						}
						nbDivide++;
						posBase += listVertex[link[jjj]]->GetPos();
					}
					posBase /= nbDivide;
					// update the position :
					listVertex[iii]->SetPos(posBase);
				}
			}
		}
		//EWOL_DEBUG(" ==> Update old points position");
		/*
		   Formule de calcule : 
		      - calculate F the barycenter of k Face center nearest
		      - Calculate R the barycenter of k Edge point nearest
		      - Move P with equation :     F + 2R + (k-3)P 
		                                  -----------------
		                                           k
		*/
		// reposition the old pont of the system
		for(int32_t iii=0; iii<listVertex.Size(); iii++) {
			if(    NULL != listVertex[iii]
			    && VERTEX_OLD == listVertex[iii]->GetType()) {
				vec3 val_P = listVertex[iii]->GetPos();
				vec3 val_F(0,0,0);
				vec3 val_R(0,0,0);
				int32_t countFace = 0;
				int32_t countEdge = 0;
				// generate barycenter of all link point if > 1...
				etk::Vector<int32_t>& link = listVertex[iii]->GetLink();
				for(int32_t jjj=0; jjj<link.Size(); jjj++) {
					if (link[jjj] >= listVertex.Size()) {
						EWOL_ERROR("Id is out of bounds ... : " << iii << " link=" << link[jjj] << " / " << listVertex.Size());
						continue;
					}
					if(NULL == listVertex[link[jjj]]) {
						continue;
					}
					if (VERTEX_OLD == listVertex[link[jjj]]->GetType()) {
						EWOL_ERROR("Center face error type ... old???");
						continue;
					}
					if (VERTEX_CENTER_FACE == listVertex[link[jjj]]->GetType()) {
						countFace++;
						val_F += listVertex[link[jjj]]->GetPos();
					} else {
						countEdge++;
						val_R += listVertex[link[jjj]]->GetPos();
					}
				}
				val_F /= countFace;
				val_R /= countEdge;
				if (countFace != countEdge) {
					EWOL_WARNING("Case not coded, result not predictible ...");
				} else {
					vec3 newPos = (val_F + 2*val_R + (countFace-3)*val_P)/countFace;
					/*
					if (newPos != val_P) {
						EWOL_DEBUG("update position : " << newPos << " <== " << val_P << " count=" << countFace);
					}
					*/
					// update the position :
					listVertex[iii]->SetPos(newPos);
				}
			}
		}
	}
	//EWOL_DEBUG(" ==> Back to the normal list of element:");
	// copy all the element in the internal structure : 
	m_listUV = listUV;
	m_listFaces = listFaces;
	m_listVertex.Clear();
	for(int32_t iii=0; iii<listVertex.Size(); iii++) {
		if (NULL != listVertex[iii]) {
			m_listVertex.PushBack(listVertex[iii]->GetPos());
			delete(listVertex[iii]);
			listVertex[iii] = NULL;
		}
	}
	listVertex.Clear();
#endif
}


void ewol::Mesh::DisplaceElement(const ewol::DisplacementTable& displacement)
{
#if 0
	CalculateNormaleFace();
	CalculateNormaleEdge();
	// displacement is done from the center of the element:
	for (int32_t iii=0; iii<m_listVertex.Size(); iii++) {
		vec3 position = m_listVertex[iii].normalized();
		vec3 positionBase = vec3(position.x(), position.y(), 0).normalized();
		vec3 positionBase2 = vec3(0, position.y(), position.z()).normalized();
		
		float modifx = 0.5f;
		float modify = 0.5f;
		if (position.x()!=0.0 || position.y()!=0.0) {
			modifx = (acos(positionBase.x())/M_PI)/2.0f;
			if (positionBase.y()>=0) {
				modifx = 0.5f - modifx;
			} else {
				modifx = 0.5f + modifx;
			}
		}
		if (position.y()!=0.0 || position.z()!=0.0) {
			modify = (acos(positionBase2.z())/M_PI)/2.0f;
			if (positionBase2.y()>=0) {
				modify = 0.5f - modify;
			} else {
				modify = 0.5f + modify;
			}
		}
		
		float move = displacement.GetInterpolate(modifx,modify);
		//EWOL_DEBUG("Get interpolate : from=" << position << " ==> (" << modifx << "," << modify << ") ==> " << move);
		
		vec3 translate = m_listVertexNormal[iii] * (move*4.0);
		m_listVertex[iii] += translate;
	}
#endif
}


bool ewol::Mesh::LoadOBJ(const etk::UString& _fileName)
{
	m_normalMode = ewol::Mesh::normalModeNone;
#if 0
	etk::FSNode fileName(_fileName);
	// Get the fileSize ...
	int32_t size = fileName.FileSize();
	if (size == 0 ) {
		EWOL_ERROR("No data in the file named=\"" << fileName << "\"");
		return false;
	}
	if(false == fileName.FileOpenRead() ) {
		EWOL_ERROR("Can not find the file name=\"" << fileName << "\"");
		return false;
	}
	char inputDataLine[2048];
	
	int32_t lineID = 0;
	while (NULL != fileName.FileGets(inputDataLine, 2048) )
	{
		lineID++;
		if (inputDataLine[0]=='v') {
			if (inputDataLine[1]=='n') {
				// Vertice normal   : vn 0.000000 0.000000 -1.000000
				// we did not use normal ==> we recalculated it if needed (some .obj does not export normal, then it is simple like this ...
				// TODO : Use the normal provided ... => can be smooth or not ... (cf check "s 1")
			} else if (inputDataLine[1]=='t') {
				// Texture position : vt 0.748573 0.750412
				vec2 vertex(0,0);
				sscanf(&inputDataLine[3], "%f %f", &vertex.m_floats[0], &vertex.m_floats[1]);
				m_listUV.PushBack(vertex);
			} else {
				// Vertice position : v 1.000000 -1.000000 -1.000000
				vec3 vertex(0,0,0);
				sscanf(&inputDataLine[2], "%f %f %f", &vertex.m_floats[0], &vertex.m_floats[1], &vertex.m_floats[2] );
				m_listVertex.PushBack(vertex);
			}
		} else if (inputDataLine[0]=='f') {
			// face : f 5/1/1 1/2/1 4/3/1*
			uint32_t vertexIndex[4], uvIndex[4], normalIndex[4];
			bool quadMode = true;
			int32_t matches = sscanf(&inputDataLine[2], "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
			                         &vertexIndex[0], &uvIndex[0], &normalIndex[0],
			                         &vertexIndex[1], &uvIndex[1], &normalIndex[1],
			                         &vertexIndex[2], &uvIndex[2], &normalIndex[2],
			                         &vertexIndex[3], &uvIndex[3], &normalIndex[3] );
			if (12 != matches){
				// no normal mode :
				matches = sscanf(&inputDataLine[2], "%d/%d %d/%d %d/%d %d/%d\n",
				                 &vertexIndex[0], &uvIndex[0],
				                 &vertexIndex[1], &uvIndex[1],
				                 &vertexIndex[2], &uvIndex[2],
				                 &vertexIndex[3], &uvIndex[3] );
				if (8 != matches){
					quadMode = false;
					matches = sscanf(&inputDataLine[2], "%d/%d/%d %d/%d/%d %d/%d/%d\n",
					                 &vertexIndex[0], &uvIndex[0], &normalIndex[0],
					                 &vertexIndex[1], &uvIndex[1], &normalIndex[1],
					                 &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
					if (9 != matches){
						// no normal mode :
						matches = sscanf(&inputDataLine[2], "%d/%d %d/%d %d/%d\n",
						                 &vertexIndex[0], &uvIndex[0],
						                 &vertexIndex[1], &uvIndex[1],
						                 &vertexIndex[2], &uvIndex[2] );
						if (6 != matches){
							EWOL_ERROR("Parsing error in the .obj files : " << fileName << " (l=" << lineID << ") in 'f' section : \"" << &inputDataLine[2] << "\" expected : %d/%d(/%d) %d/%d(/%d) %d/%d(/%d) (%d/%d(/%d)) () for option");
							continue;
						}
					}
				}
			}
			if (true==quadMode) {
				m_listFaces.PushBack(Face(vertexIndex[0]-1, uvIndex[0]-1,
				                          vertexIndex[1]-1, uvIndex[1]-1,
				                          vertexIndex[2]-1, uvIndex[2]-1,
				                          vertexIndex[3]-1, uvIndex[3]-1));
			} else {
				m_listFaces.PushBack(Face(vertexIndex[0]-1, uvIndex[0]-1,
				                          vertexIndex[1]-1, uvIndex[1]-1,
				                          vertexIndex[2]-1, uvIndex[2]-1));
			}
			/*
			EWOL_DEBUG(" plop : " << tmpFace.m_nbElement << " ? " << m_listFaces[m_listFaces.Size()-1].m_nbElement);
			EWOL_DEBUG("      : " << tmpFace.m_vertex[0] << " ? " << m_listFaces[m_listFaces.Size()-1].m_vertex[0]);
			EWOL_DEBUG("      : " << tmpFace.m_uv[0] << " ? " << m_listFaces[m_listFaces.Size()-1].m_uv[0]);
			*/
		} else if (inputDataLine[0]=='s') {
			// ??? : s off
		} else if (inputDataLine[0]=='#') {
			// comment
			// nothing to do ... just go to the new line ...
		} else if(    inputDataLine[0]=='u'
		           && inputDataLine[1]=='s'
		           && inputDataLine[2]=='e'
		           && inputDataLine[3]=='m'
		           && inputDataLine[4]=='t'
		           && inputDataLine[5]=='l' ) {
			// Use Material : usemtl imageName.xxx
			while(    inputDataLine[strlen(inputDataLine)-1] == '\n'
			       || inputDataLine[strlen(inputDataLine)-1] == '\r'
			       || inputDataLine[strlen(inputDataLine)-1] == ' ') {
				if (1 == strlen(inputDataLine) ){
					break;
				}
				inputDataLine[strlen(inputDataLine)-1] = '\0';
			}
			etk::UString tmpVal(&inputDataLine[7]);
			SetTexture(fileName.GetRelativeFolder() + tmpVal);
		} else if(    inputDataLine[0]=='m'
		           && inputDataLine[1]=='t'
		           && inputDataLine[2]=='l'
		           && inputDataLine[3]=='l'
		           && inputDataLine[4]=='i'
		           && inputDataLine[5]=='b' ) {
			// ???? : mtllib cube.mtl
		}
	}
	fileName.FileClose();
	GenerateVBO();
#endif
	return true;
}


char* LoadNextData(char* _elementLine, int64_t _maxData, etk::FSNode& _file, bool _removeTabs=false, bool _stopColomn=false)
{
	memset(_elementLine, 0, _maxData);
	char * element = _elementLine;
	int64_t outSize = 0;
	/*
	if (m_zipReadingOffset>=m_zipContent->Size()) {
		element[0] = '\0';
		return NULL;
	}
	*/
	char current = _file.FileGet();
	while (current != '\0') {
		if(    _removeTabs==false
		    || element != _elementLine) {
			*element = current;
			element++;
		}
		if(    current == '\n'
		    || current == '\r'
		    || current == '|'
		    || (    current == ':'
		         && _stopColomn==true) )
		{
			*element = '\0';
			//EWOL_DEBUG(" plop : '" << _elementLine << "'" );
			return _elementLine;
		} else if(    element == _elementLine
		           && current != '\t') {
			*element = current;
			element++;
		}
		// check maxData Size ...
		if (outSize>=_maxData-1) {
			*element = '\0';
			return _elementLine;
		}
		current = _file.FileGet();
	}
	if (outSize==0) {
		return NULL;
	} else {
		// send last line
		return _elementLine;
	}
	return NULL;
}

bool ewol::Mesh::LoadEMF(const etk::UString& _fileName)
{
	m_normalMode = ewol::Mesh::normalModeNone;
	etk::FSNode fileName(_fileName);
	// Get the fileSize ...
	int32_t size = fileName.FileSize();
	if (size == 0 ) {
		EWOL_ERROR("No data in the file named=\"" << fileName << "\"");
		return false;
	}
	if(false == fileName.FileOpenRead() ) {
		EWOL_ERROR("Can not find the file name=\"" << fileName << "\"");
		return false;
	}
	char inputDataLine[2048];
	// load the first line :
	fileName.FileGets(inputDataLine, 2048);
	if(0==strncmp(inputDataLine, "EMF(STRING)", 11)) {
		// parse in string mode ...
	} else if (0==strncmp(inputDataLine, "EMF(BINARY)", 11)) {
		EWOL_ERROR(" file binary mode is not supported now : 'EMF(BINARY)'");
		return false;
	} else {
		EWOL_ERROR(" file mode is not supported now : 'EMF(? ? ?)' = '" << inputDataLine << "'");
		return false;
	}
	while (NULL != LoadNextData(inputDataLine, 2048, fileName) ) {
		//EWOL_DEBUG("min line : '" << inputDataLine << "'");
		if(    inputDataLine[0]=='#'
		    || inputDataLine[0]=='\0' ) {
			// comment line ... or empty line
			continue;
		}
		if(    inputDataLine[0]=='\t'
		    || 0 == strncmp(inputDataLine, "    ", 4) ) {
			// Sub section that is not parsed ...
			continue;
		}
		if( 0 == strncmp(inputDataLine, "Mesh :", 6) ) {
			//Find a mesh ==> parse it ...
			while (NULL != LoadNextData(inputDataLine, 2048, fileName) ) {
				if(    inputDataLine[0]=='#'
				    || inputDataLine[0]=='\0' ) {
					// comment line ... or empty line
					continue;
				}
				if( 0 == strncmp(inputDataLine, "\tName : ", 8) ) {
					// find the mesh name.
					etk::UString m_meshName = &inputDataLine[8];
					EWOL_DEBUG("Load sub mesh named : '" << m_meshName << "'");
					continue;
				}
				if( 0 == strncmp(inputDataLine, "\tVertex : ", 10) ) {
					// find the vertex list of elements.
					while (NULL != LoadNextData(inputDataLine, 2048, fileName, true) ) {
						if (inputDataLine[0] == '\0') {
							break;
						}
						vec3 vertex(0,0,0);
						sscanf(inputDataLine, "%f %f %f", &vertex.m_floats[0], &vertex.m_floats[1], &vertex.m_floats[2] );
						m_listVertex.PushBack(vertex);
					}
					EWOL_DEBUG("Load " << m_listVertex.Size() << " vertex");
					continue;
				}
				if( 0 == strncmp(inputDataLine, "\tUV-mapping :", 13) ) {
					// find the UV mapping.
					while (NULL != LoadNextData(inputDataLine, 2048, fileName, true) ) {
						if (inputDataLine[0] == '\0') {
							break;
						}
						vec2 uvMap(0,0);
						sscanf(inputDataLine, "%f %f", &uvMap.m_floats[0], &uvMap.m_floats[1]);
						m_listUV.PushBack(uvMap);
					}
					EWOL_DEBUG("Load " << m_listUV.Size() << " uv mapping point");
					continue;
				}
				if( 0 == strncmp(inputDataLine, "\tNormal(vertex) : ", 18) ) {
					m_normalMode = ewol::Mesh::normalModeVertex;
					// find the vertex Normal list.
					while (NULL != LoadNextData(inputDataLine, 2048, fileName, true) ) {
						if (inputDataLine[0] == '\0') {
							break;
						}
						vec3 normal(0,0,0);
						sscanf(inputDataLine, "%f %f %f", &normal.m_floats[0], &normal.m_floats[1], &normal.m_floats[2] );
						m_listVertexNormal.PushBack(normal);
					}
					EWOL_DEBUG("Load " << m_listVertexNormal.Size() << " vertex normal");
					continue;
				}
				if( 0 == strncmp(inputDataLine, "\tNormal(face) : ", 16) ) {
					m_normalMode = ewol::Mesh::normalModeFace;
					// find the face Normal list.
					while (NULL != LoadNextData(inputDataLine, 2048, fileName, true) ) {
						if (inputDataLine[0] == '\0') {
							break;
						}
						vec3 normal(0,0,0);
						sscanf(inputDataLine, "%f %f %f", &normal.m_floats[0], &normal.m_floats[1], &normal.m_floats[2] );
						m_listFacesNormal.PushBack(normal);
					}
					EWOL_DEBUG("Load " << m_listFacesNormal.Size() << " face normal");
					continue;
				}
				if( 0 == strncmp(inputDataLine, "\tFace : ", 8) ) {
					// find the face list.
					int32_t elementID = -1;
					while (NULL != LoadNextData(inputDataLine, 2048, fileName, true, true) ) {
						if (inputDataLine[0] == '\0') {
							break;
						}
						//EWOL_DEBUG("parse :'" << inputDataLine << "'");
						int32_t len = strlen(inputDataLine);
						if (inputDataLine[len-1] == ':') {
							// find the material :
							inputDataLine[len-1] = '\0';
							etk::UString materialName = inputDataLine;
							FaceIndexing empty;
							m_listFaces.Add(materialName, empty);
							elementID = m_listFaces.GetId(materialName);
						} else {
							if (elementID < 0) {
								continue;
							}
							int32_t matches;
							uint32_t vertexIndex[3], uvIndex[3], normalIndex[3];
							vertexIndex[0] = 0;
							vertexIndex[1] = 0;
							vertexIndex[2] = 0;
							uvIndex[0] = 0;
							uvIndex[1] = 0;
							uvIndex[2] = 0;
							normalIndex[0] = 0;
							normalIndex[1] = 0;
							normalIndex[2] = 0;
							matches = sscanf(inputDataLine, "%d/%d/%d %d/%d/%d %d/%d/%d",
							                 &vertexIndex[0], &uvIndex[0], &normalIndex[0],
							                 &vertexIndex[1], &uvIndex[1], &normalIndex[1],
							                 &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
							m_listFaces.GetValue(elementID).m_faces.PushBack(Face(vertexIndex[0], uvIndex[0], normalIndex[0],
							                                                      vertexIndex[1], uvIndex[1], normalIndex[1],
							                                                      vertexIndex[2], uvIndex[2], normalIndex[2]));
							/*
							EWOL_DEBUG("face :" << vertexIndex[0] << "/" << uvIndex[0] << "/" << normalIndex[0] <<
							           " " << vertexIndex[1] << "/" << uvIndex[1] << "/" << normalIndex[1] <<
							           " " << vertexIndex[2] << "/" << uvIndex[2] << "/" << normalIndex[2]);
							*/
						}
					}
					EWOL_DEBUG("Load " << m_listFaces.Size() << " faces (mode) :");
					for (esize_t iii=0; iii< m_listFaces.Size(); iii++) {
						EWOL_DEBUG("    mat='" << m_listFaces.GetKey(iii) << "' nb faces=" << m_listFaces.GetValue(iii).m_faces.Size());
					}
					break;
				}
			}
			continue;
		}
		if( 0 == strncmp(inputDataLine, "Materials : ", 11) ) {
			//Find Material section ==> parse it ...
			//EWOL_DEBUG("find a Maretials section :");
			// all object might have minimaly one material :
			ewol::Material* material = NULL;
			etk::UString name = "";
			//Find a mesh ==> parse it ...
			while (NULL != LoadNextData(inputDataLine, 2048, fileName) ) {
				//EWOL_DEBUG("mat parse : '" << inputDataLine << "'");
				if(    inputDataLine[0] == '\t'
				    && inputDataLine[1] != '\t') {
					if(    name != ""
					    && material!=NULL) {
						m_materials.Add(name, material);
						name = "";
						material = NULL;
					}
					material = new ewol::Material();
					inputDataLine[strlen(inputDataLine)-1] = '\0';
					name = &inputDataLine[1];
					EWOL_DEBUG("Create materials : '" << name << "'");
				} else {
					if (NULL != material) {
						if(    inputDataLine[2] == 'N'
						    && inputDataLine[3] == 's'
						    && inputDataLine[4] == ' ' ) {
							float tmpVal=0;
							sscanf(&inputDataLine[5], "%f", &tmpVal);
							material->SetShininess(tmpVal);
						} else if(    inputDataLine[2] == 'K'
						           && inputDataLine[3] == 'a'
						           && inputDataLine[4] == ' ' ) {
							float tmpVal1=0;
							float tmpVal2=0;
							float tmpVal3=0;
							sscanf(&inputDataLine[5], "%f %f %f", &tmpVal1, &tmpVal2, &tmpVal3);
							vec4 tmp(tmpVal1, tmpVal2, tmpVal3, 1);
							material->SetAmbientFactor(tmp);
						} else if(    inputDataLine[2] == 'K'
						           && inputDataLine[3] == 'd'
						           && inputDataLine[4] == ' ' ) {
							float tmpVal1=0;
							float tmpVal2=0;
							float tmpVal3=0;
							sscanf(&inputDataLine[5], "%f %f %f", &tmpVal1, &tmpVal2, &tmpVal3);
							vec4 tmp(tmpVal1, tmpVal2, tmpVal3, 1);
							material->SetDiffuseFactor(tmp);
						} else if(    inputDataLine[2] == 'K'
						           && inputDataLine[3] == 's'
						           && inputDataLine[4] == ' ' ) {
							float tmpVal1=0;
							float tmpVal2=0;
							float tmpVal3=0;
							sscanf(&inputDataLine[5], "%f %f %f", &tmpVal1, &tmpVal2, &tmpVal3);
							vec4 tmp(tmpVal1, tmpVal2, tmpVal3, 1);
							material->SetSpecularFactor(tmp);
						} else if(    inputDataLine[2] == 'N'
						           && inputDataLine[3] == 'i'
						           && inputDataLine[4] == ' ' ) {
							float tmpVal=0;
							sscanf(&inputDataLine[5], "%f", &tmpVal);
							// TODO : ...
						} else if(    inputDataLine[2] == 'd'
						           && inputDataLine[3] == ' ' ) {
							float tmpVal=0;
							sscanf(&inputDataLine[4], "%f", &tmpVal);
							// TODO : ...
						} else if(    inputDataLine[2] == 'i'
						           && inputDataLine[3] == 'l'
						           && inputDataLine[4] == 'l'
						           && inputDataLine[5] == 'u'
						           && inputDataLine[6] == 'm'
						           && inputDataLine[7] == ' ' ) {
							// TODO : ...
						} else if(    inputDataLine[2] == 'm'
						           && inputDataLine[3] == 'a'
						           && inputDataLine[4] == 'p'
						           && inputDataLine[5] == '_'
						           && inputDataLine[6] == 'K'
						           && inputDataLine[7] == 'd'
						           && inputDataLine[8] == ' ' ) {
							inputDataLine[strlen(inputDataLine)-1] = '\0';
							material->SetTexture0(fileName.GetRelativeFolder() + &inputDataLine[9]);
						} else {
							EWOL_ERROR("unknow material property ... : '" << inputDataLine << "'");
						}
					}
				}
			}
			if(    name != ""
			    && material!=NULL) {
				m_materials.Add(name, material);
				name = "";
				material = NULL;
			}
			continue;
		}
	}
	fileName.FileClose();
	GenerateVBO();
	return true;
}



void ewol::Mesh::AddMaterial(const etk::UString& _name, ewol::Material* _data)
{
	if (NULL==_data) {
		EWOL_ERROR(" can not add material with null pointer");
		return;
	}
	if (_name=="") {
		EWOL_ERROR(" can not add material with no name");
		return;
	}
	// really add the material :
	m_materials.Add(_name, _data);
}
