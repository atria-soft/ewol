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



ewol::Mesh::Mesh(etk::UString genName, etk::UString shaderName) :
	ewol::Resource(genName),
	m_enableFaceNormal(true),
	m_enableVertexNormal(true),
	m_numberOfElments(0),
	m_texture0(NULL)
{
	// get the shader resource :
	m_GLPosition = 0;
	
	// set the element material properties : 
	m_material.SetAmbientFactor(vec4(0.100000,0.100000,0.100000, 1.0));
	m_material.SetDiffuseFactor(vec4(0.640000, 0.640000, 0.640000, 1.0));
	m_material.SetSpecularFactor(vec4(0.500000, 0.500000, 0.500000, 1.0));
	m_material.SetShininess(0.96078431);
	
	m_light.SetDirection(vec3(0,cos(M_PI/4),sin(M_PI/4)));
	m_light.SetHalfPlane(vec3(1,0,0));
	m_light.SetAmbientColor(vec4(1,1,1,1));
	m_light.SetDiffuseColor(vec4(1.0,1.0,1.0,1));
	m_light.SetSpecularColor(vec4(0.0,0.0,0.0,1));
	
	if (true == ewol::resource::Keep(shaderName, m_GLprogram) ) {
		m_GLPosition = m_GLprogram->GetAttribute("EW_coord3d");
		m_GLtexture  = m_GLprogram->GetAttribute("EW_texture2d");
		m_GLNormal   = m_GLprogram->GetAttribute("EW_normal");
		m_GLNormalFace = m_GLprogram->GetAttribute("EW_faceNormal");
		m_GLMatrix         = m_GLprogram->GetUniform("EW_MatrixTransformation");
		m_GLMatrixPosition = m_GLprogram->GetUniform("EW_MatrixPosition");
		m_GLtexID0   = m_GLprogram->GetUniform("EW_texID");
		// Link material and Lights
		m_material.Link(m_GLprogram, "EW_material");
		m_light.Link(m_GLprogram, "EW_directionalLight");
	}
	// this is the properties of the buffer requested : "r"/"w" + "-" + buffer type "f"=flaot "i"=integer
	ewol::resource::Keep("w-fff", m_verticesVBO);
}

ewol::Mesh::~Mesh(void)
{
	// remove dynamics dependencies :
	if(NULL!=m_texture0) {
		ewol::resource::Release(m_texture0);
	}
	ewol::resource::Release(m_GLprogram);
	ewol::resource::Release(m_verticesVBO);
	m_numberOfElments=0;
}


void ewol::Mesh::Draw(mat4& positionMatrix)
{
	if (m_numberOfElments<=0) {
		return;
	}
	if (NULL == m_texture0) {
		EWOL_WARNING("Texture does not exist ...");
		return;
	}
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	ewol::openGL::Enable(GL_DEPTH_TEST);
	//EWOL_DEBUG("    Display " << m_coord.Size() << " elements" );
	m_GLprogram->Use();
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = ewol::openGL::GetMatrix();
	mat4 camMatrix = ewol::openGL::GetCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix;
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	m_GLprogram->UniformMatrix4fv(m_GLMatrixPosition, 1, positionMatrix.m_mat);
	// TextureID
	m_GLprogram->SetTexture0(m_GLtexID0, m_texture0->GetId());
	// position :
	m_GLprogram->SendAttributePointer(m_GLPosition, 3/*x,y,z*/, m_verticesVBO, MESH_VBO_VERTICES);
	// Texture :
	m_GLprogram->SendAttributePointer(m_GLtexture, 2/*u,v*/, m_verticesVBO, MESH_VBO_TEXTURE);
	// position :
	m_GLprogram->SendAttributePointer(m_GLNormal, 3/*x,y,z*/, m_verticesVBO, MESH_VBO_VERTICES_NORMAL);
	// position :
	m_GLprogram->SendAttributePointer(m_GLNormalFace, 3/*x,y,z*/, m_verticesVBO, MESH_VBO_FACE_NORMAL);
	// draw materials :
	m_material.Draw(m_GLprogram);
	m_light.Draw(m_GLprogram);
	
	// Request the draw od the elements : 
	ewol::openGL::DrawArrays(GL_TRIANGLES, 0, m_numberOfElments);
	m_GLprogram->UnUse();
	ewol::openGL::Disable(GL_DEPTH_TEST);
	//glBindBuffer(GL_ARRAY_BUFFER,0);
}

// normal calculation of the normal face is really easy :
void ewol::Mesh::CalculateNormaleFace(void)
{
	m_listFacesNormal.Clear();
	// TODO : Preallocation of the vertex :
	
	if(    true==m_enableFaceNormal
	    || true==m_enableVertexNormal) {
		for(int32_t iii=0 ; iii<m_listFaces.Size() ; iii++) {
			// for all case, We use only the 3 vertex for quad element, in theory 3D modeler export element in triangle if it is not a real plane.
			vec3 normal = btCross(m_listVertex[m_listFaces[iii].m_vertex[0]]-m_listVertex[m_listFaces[iii].m_vertex[1]],
			                      m_listVertex[m_listFaces[iii].m_vertex[1]]-m_listVertex[m_listFaces[iii].m_vertex[2]]);
			m_listFacesNormal.PushBack(normal.normalized());
		}
	}
}

void ewol::Mesh::CalculateNormaleEdge(void)
{
	m_listVertexNormal.Clear();
	// TODO : Preallocation of the vertex :
	
	if(true==m_enableVertexNormal) {
		for(int32_t iii=0 ; iii<m_listVertex.Size() ; iii++) {
			vec3 normal(0,0,0);
			// add the vertex from all the element in the list for face when the element in the face ...
			for(int32_t jjj=0 ; jjj<m_listFaces.Size() ; jjj++) {
				if(    m_listFaces[jjj].m_vertex[0] == iii
				    || m_listFaces[jjj].m_vertex[1] == iii
				    || m_listFaces[jjj].m_vertex[2] == iii
				    || (    m_listFaces[jjj].m_nbElement == 4
				         && m_listFaces[jjj].m_vertex[3] == iii) ) {
					normal += m_listFacesNormal[jjj];
				}
			}
			if (normal == vec3(0,0,0)) {
				m_listVertexNormal.PushBack(vec3(1,1,1));
			} else {
				m_listVertexNormal.PushBack(normal.normalized());
			}
		}
	}
}

// for debugging ...
//#define PRINT_HALF (1)

void ewol::Mesh::GenerateVBO(void)
{
	m_numberOfElments = 0;
	// calculate the normal of all faces if needed
	CalculateNormaleFace();
	CalculateNormaleEdge();
	// TODO : Set a better display system, this one is the worst I known ...
	for (int32_t iii=0; iii<m_listFaces.Size() ; iii++) {
		#ifdef PRINT_HALF
			m_numberOfElments += 3*3;
		#else
			m_numberOfElments += m_listFaces[iii].m_nbElement*3;
		#endif
		// 2 possibilities : triangle or quad :
		int32_t indice = 0;
		vec2 tmpUV = m_listUV[m_listFaces[iii].m_uv[indice]];
		m_verticesVBO->PushOnBuffer(MESH_VBO_VERTICES,m_listVertex[m_listFaces[iii].m_vertex[indice]]);
		m_verticesVBO->PushOnBuffer(MESH_VBO_TEXTURE, vec2(tmpUV.x(),1.0f-tmpUV.y()));
		if(true==m_enableVertexNormal) {
			m_verticesVBO->PushOnBuffer(MESH_VBO_VERTICES_NORMAL,m_listVertexNormal[m_listFaces[iii].m_vertex[indice]]);
		}
		if(true==m_enableFaceNormal) {
			m_verticesVBO->PushOnBuffer(MESH_VBO_FACE_NORMAL,m_listFacesNormal[iii]);
		}
		
		indice = 1;
		tmpUV = m_listUV[m_listFaces[iii].m_uv[indice]];
		m_verticesVBO->PushOnBuffer(MESH_VBO_VERTICES,m_listVertex[m_listFaces[iii].m_vertex[indice]]);
		m_verticesVBO->PushOnBuffer(MESH_VBO_TEXTURE, vec2(tmpUV.x(),1.0f-tmpUV.y()));
		if(true==m_enableVertexNormal) {
			m_verticesVBO->PushOnBuffer(MESH_VBO_VERTICES_NORMAL,m_listVertexNormal[m_listFaces[iii].m_vertex[indice]]);
		}
		if(true==m_enableFaceNormal) {
			m_verticesVBO->PushOnBuffer(MESH_VBO_FACE_NORMAL,m_listFacesNormal[iii]);
		}
		
		indice = 2;
		tmpUV = m_listUV[m_listFaces[iii].m_uv[indice]];
		m_verticesVBO->PushOnBuffer(MESH_VBO_VERTICES,m_listVertex[m_listFaces[iii].m_vertex[indice]]);
		m_verticesVBO->PushOnBuffer(MESH_VBO_TEXTURE, vec2(tmpUV.x(),1.0f-tmpUV.y()));
		if(true==m_enableVertexNormal) {
			m_verticesVBO->PushOnBuffer(MESH_VBO_VERTICES_NORMAL,m_listVertexNormal[m_listFaces[iii].m_vertex[indice]]);
		}
		if(true==m_enableFaceNormal) {
			m_verticesVBO->PushOnBuffer(MESH_VBO_FACE_NORMAL,m_listFacesNormal[iii]);
		}
		#ifndef PRINT_HALF
			if (m_listFaces[iii].m_nbElement==4) {
				indice = 0;
				tmpUV = m_listUV[m_listFaces[iii].m_uv[indice]];
				m_verticesVBO->PushOnBuffer(MESH_VBO_VERTICES,m_listVertex[m_listFaces[iii].m_vertex[indice]]);
				m_verticesVBO->PushOnBuffer(MESH_VBO_TEXTURE, vec2(tmpUV.x(),1.0f-tmpUV.y()));
				if(true==m_enableVertexNormal) {
					m_verticesVBO->PushOnBuffer(MESH_VBO_VERTICES_NORMAL,m_listVertexNormal[m_listFaces[iii].m_vertex[indice]]);
				}
				if(true==m_enableFaceNormal) {
					m_verticesVBO->PushOnBuffer(MESH_VBO_FACE_NORMAL,m_listFacesNormal[iii]);
				}
				
				indice = 2;
				tmpUV = m_listUV[m_listFaces[iii].m_uv[indice]];
				m_verticesVBO->PushOnBuffer(MESH_VBO_VERTICES,m_listVertex[m_listFaces[iii].m_vertex[indice]]);
				m_verticesVBO->PushOnBuffer(MESH_VBO_TEXTURE, vec2(tmpUV.x(),1.0f-tmpUV.y()));
				if(true==m_enableVertexNormal) {
					m_verticesVBO->PushOnBuffer(MESH_VBO_VERTICES_NORMAL,m_listVertexNormal[m_listFaces[iii].m_vertex[indice]]);
				}
				if(true==m_enableFaceNormal) {
					m_verticesVBO->PushOnBuffer(MESH_VBO_FACE_NORMAL,m_listFacesNormal[iii]);
				}
				
				indice = 3;
				tmpUV = m_listUV[m_listFaces[iii].m_uv[indice]];
				m_verticesVBO->PushOnBuffer(MESH_VBO_VERTICES,m_listVertex[m_listFaces[iii].m_vertex[indice]]);
				m_verticesVBO->PushOnBuffer(MESH_VBO_TEXTURE, vec2(tmpUV.x(),1.0f-tmpUV.y()));
				if(true==m_enableVertexNormal) {
					m_verticesVBO->PushOnBuffer(MESH_VBO_VERTICES_NORMAL,m_listVertexNormal[m_listFaces[iii].m_vertex[indice]]);
				}
				if(true==m_enableFaceNormal) {
					m_verticesVBO->PushOnBuffer(MESH_VBO_FACE_NORMAL,m_listFacesNormal[iii]);
				}
			}
		#endif
	}
	// update all the VBO elements ...
	m_verticesVBO->Flush();
}


void ewol::Mesh::CreateCube(float size)
{
	m_listVertex.Clear();
	m_listUV.Clear();
	m_listFaces.Clear();
	m_numberOfElments = 0;
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
	
}

void ewol::Mesh::CreateViewBox(float size)
{
	m_listVertex.Clear();
	m_listUV.Clear();
	m_listFaces.Clear();
	m_numberOfElments = 0;
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
	
	m_listFaces.PushBack(Face(0,1, 1,5,  2,6,  3,2)); // 4
	m_listFaces.PushBack(Face(4,4, 0,0,  3,1,  7,5)); // 3
	m_listFaces.PushBack(Face(2,6, 6,10, 7,11, 3,7)); // 2
	m_listFaces.PushBack(Face(4,2, 7,3,  6,7,  5,6)); // 5
	m_listFaces.PushBack(Face(1,5, 5,9,  6,10, 2,6)); // 1
	m_listFaces.PushBack(Face(0,4, 4,8,  5,9,  1,5)); // 0
	
}

void ewol::Mesh::SetTexture(const etk::UString& myTexture)
{
	ivec2 tmpSize(256, 256);
	// prevent overloard error :
	ewol::TextureFile* tmpCopy = m_texture0;
	m_texture0 = NULL;
	if (false == ewol::resource::Keep(myTexture, m_texture0, tmpSize)) {
		EWOL_ERROR("Can not load specific texture : " << myTexture);
		// retreave previous texture:
		m_texture0 = tmpCopy;
		return;
	}
	if (NULL != tmpCopy) {
		// really release previous texture. In case of same texture loading, then we did not have reload it .. just increase and decrease index...
		ewol::resource::Release(tmpCopy);
	}
}

void ewol::Mesh::Subdivide(int32_t numberOfTime, bool smooth)
{
	for(int32_t iii=0; iii<numberOfTime ; iii++) {
		InternalSubdivide(smooth);
	}
}

int32_t CreateOrGetNewPointId(vertex_te type, const vec3& point, etk::Vector<VertexNode*>& list)
{
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
}

int32_t CreateOrGetNewTexId(const vec2& point, etk::Vector<vec2>& list)
{
	for (int32_t iii=0; iii<list.Size(); iii++) {
		if (list[iii] == point) {
			return iii;
		}
	}
	list.PushBack(point);
	return list.Size()-1;
}

void ewol::Mesh::InternalSubdivide(bool smooth)
{
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
}



void ewol::Mesh::LoadMaterial(const etk::UString& name)
{
	
}

void ewol::Mesh::DisplaceElement(const ewol::DisplacementTable& displacement)
{
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
}


