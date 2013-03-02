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


// 3 "float" elements
#define MESH_VBO_VERTICES  (0)
// 2 "float" elements
#define MESH_VBO_TEXTURE   (1)
// 4 "float" elements
#define MESH_VBO_COLOR     (2)
// 3 "float" elements
#define MESH_VBO_NORMAL    (3)


ewol::Mesh::Mesh(etk::UString genName) :
	ewol::Resource(genName),
	m_numberOfElments(0),
	m_texture1(NULL)
{
	etk::UString tmpString("DATA:textured3D2.prog");
	// get the shader resource :
	m_GLPosition = 0;
	if (true == ewol::resource::Keep(tmpString, m_GLprogram) ) {
		m_GLPosition = m_GLprogram->GetAttribute("EW_coord3d");
		m_GLtexture  = m_GLprogram->GetAttribute("EW_texture2d");
		m_GLMatrix   = m_GLprogram->GetUniform("EW_MatrixTransformation");
		m_GLtexID    = m_GLprogram->GetUniform("EW_texID");
	}
	ewol::resource::Keep("w", m_verticesVBO);
}

ewol::Mesh::~Mesh(void)
{
	// remove dynamics dependencies :
	if(NULL!=m_texture1) {
		ewol::resource::Release(m_texture1);
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
	if (NULL == m_texture1) {
		EWOL_WARNING("Texture does not exist ...");
		return;
	}
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	glEnable(GL_DEPTH_TEST);
	//EWOL_DEBUG("    Display " << m_coord.Size() << " elements" );
	m_GLprogram->Use();
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = ewol::openGL::GetMatrix();
	mat4 camMatrix = ewol::openGL::GetCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix * positionMatrix;
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// TextureID
	m_GLprogram->SetTexture0(m_GLtexID, m_texture1->GetId());
	// position :
	m_GLprogram->SendAttributePointer(m_GLPosition, 3/*x,y,z*/, m_verticesVBO, MESH_VBO_VERTICES);
	// Texture :
	m_GLprogram->SendAttributePointer(m_GLtexture, 2/*u,v*/, m_verticesVBO, MESH_VBO_TEXTURE);
	// Request the draw od the elements : 
	glDrawArrays(GL_TRIANGLES, 0, m_numberOfElments);
	m_GLprogram->UnUse();
	glDisable(GL_DEPTH_TEST);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

// for debugging ...
//#define PRINT_HALF (1)

void ewol::Mesh::GenerateVBO(void)
{
	m_numberOfElments = 0;
	// TODO : Set a better display system, this one is the worst I known ...
	for (int32_t iii=0; iii<m_listFaces.Size() ; iii++) {
		#ifdef PRINT_HALF
			m_numberOfElments += 3*3;
		#else
			m_numberOfElments += m_listFaces[iii].m_nbElement*3;
		#endif
		// 2 possibilities : triangle or quad :
		int32_t indice = 0;
		vec3 tmpPos = m_listVertex[m_listFaces[iii].m_vertex[indice]];
		vec2 tmpUV = m_listUV[m_listFaces[iii].m_uv[indice]];
		m_verticesVBO->GetRefBuffer(MESH_VBO_VERTICES).PushBack(tmpPos.x());
		m_verticesVBO->GetRefBuffer(MESH_VBO_VERTICES).PushBack(tmpPos.y());
		m_verticesVBO->GetRefBuffer(MESH_VBO_VERTICES).PushBack(tmpPos.z());
		m_verticesVBO->GetRefBuffer(MESH_VBO_TEXTURE).PushBack(tmpUV.x());
		m_verticesVBO->GetRefBuffer(MESH_VBO_TEXTURE).PushBack(1.0f-tmpUV.y());
		
		indice = 1;
		tmpPos = m_listVertex[m_listFaces[iii].m_vertex[indice]];
		tmpUV = m_listUV[m_listFaces[iii].m_uv[indice]];
		m_verticesVBO->GetRefBuffer(MESH_VBO_VERTICES).PushBack(tmpPos.x());
		m_verticesVBO->GetRefBuffer(MESH_VBO_VERTICES).PushBack(tmpPos.y());
		m_verticesVBO->GetRefBuffer(MESH_VBO_VERTICES).PushBack(tmpPos.z());
		m_verticesVBO->GetRefBuffer(MESH_VBO_TEXTURE).PushBack(tmpUV.x());
		m_verticesVBO->GetRefBuffer(MESH_VBO_TEXTURE).PushBack(1.0f-tmpUV.y());
		
		indice = 2;
		tmpPos = m_listVertex[m_listFaces[iii].m_vertex[indice]];
		tmpUV = m_listUV[m_listFaces[iii].m_uv[indice]];
		m_verticesVBO->GetRefBuffer(MESH_VBO_VERTICES).PushBack(tmpPos.x());
		m_verticesVBO->GetRefBuffer(MESH_VBO_VERTICES).PushBack(tmpPos.y());
		m_verticesVBO->GetRefBuffer(MESH_VBO_VERTICES).PushBack(tmpPos.z());
		m_verticesVBO->GetRefBuffer(MESH_VBO_TEXTURE).PushBack(tmpUV.x());
		m_verticesVBO->GetRefBuffer(MESH_VBO_TEXTURE).PushBack(1.0f-tmpUV.y());
		#ifndef PRINT_HALF
			if (m_listFaces[iii].m_nbElement==4) {
				indice = 0;
				tmpPos = m_listVertex[m_listFaces[iii].m_vertex[indice]];
				tmpUV = m_listUV[m_listFaces[iii].m_uv[indice]];
				m_verticesVBO->GetRefBuffer(MESH_VBO_VERTICES).PushBack(tmpPos.x());
				m_verticesVBO->GetRefBuffer(MESH_VBO_VERTICES).PushBack(tmpPos.y());
				m_verticesVBO->GetRefBuffer(MESH_VBO_VERTICES).PushBack(tmpPos.z());
				m_verticesVBO->GetRefBuffer(MESH_VBO_TEXTURE).PushBack(tmpUV.x());
				m_verticesVBO->GetRefBuffer(MESH_VBO_TEXTURE).PushBack(1.0f-tmpUV.y());
				
				indice = 2;
				tmpPos = m_listVertex[m_listFaces[iii].m_vertex[indice]];
				tmpUV = m_listUV[m_listFaces[iii].m_uv[indice]];
				m_verticesVBO->GetRefBuffer(MESH_VBO_VERTICES).PushBack(tmpPos.x());
				m_verticesVBO->GetRefBuffer(MESH_VBO_VERTICES).PushBack(tmpPos.y());
				m_verticesVBO->GetRefBuffer(MESH_VBO_VERTICES).PushBack(tmpPos.z());
				m_verticesVBO->GetRefBuffer(MESH_VBO_TEXTURE).PushBack(tmpUV.x());
				m_verticesVBO->GetRefBuffer(MESH_VBO_TEXTURE).PushBack(1.0f-tmpUV.y());
				
				indice = 3;
				tmpPos = m_listVertex[m_listFaces[iii].m_vertex[indice]];
				tmpUV = m_listUV[m_listFaces[iii].m_uv[indice]];
				m_verticesVBO->GetRefBuffer(MESH_VBO_VERTICES).PushBack(tmpPos.x());
				m_verticesVBO->GetRefBuffer(MESH_VBO_VERTICES).PushBack(tmpPos.y());
				m_verticesVBO->GetRefBuffer(MESH_VBO_VERTICES).PushBack(tmpPos.z());
				m_verticesVBO->GetRefBuffer(MESH_VBO_TEXTURE).PushBack(tmpUV.x());
				m_verticesVBO->GetRefBuffer(MESH_VBO_TEXTURE).PushBack(1.0f-tmpUV.y());
			}
		#endif
	}
	// update all the VBO elements ...
	m_verticesVBO->Flush();
}


void ewol::Mesh::CreateCube(void)
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
	m_listVertex.PushBack(vec3( 1.0, -1.0, -1.0)); // 0
	m_listVertex.PushBack(vec3( 1.0, -1.0,  1.0)); // 1
	m_listVertex.PushBack(vec3(-1.0, -1.0,  1.0)); // 2
	m_listVertex.PushBack(vec3(-1.0, -1.0, -1.0)); // 3
	m_listVertex.PushBack(vec3( 1.0,  1.0, -1.0)); // 4
	m_listVertex.PushBack(vec3( 1.0,  1.0,  1.0)); // 5
	m_listVertex.PushBack(vec3(-1.0,  1.0,  1.0)); // 6
	m_listVertex.PushBack(vec3(-1.0,  1.0, -1.0)); // 7
	
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

void ewol::Mesh::SetTexture(const etk::UString& myTexture)
{
	ivec2 tmpSize(256, 256);
	// prevent overloard error :
	ewol::TextureFile* tmpCopy = m_texture1;
	m_texture1 = NULL;
	if (false == ewol::resource::Keep(myTexture, m_texture1, tmpSize)) {
		EWOL_ERROR("Can not load specific texture : " << myTexture);
		// retreave previous texture:
		m_texture1 = tmpCopy;
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

int32_t CreateOrGetNewPointId(const vec3& point, etk::Vector<vec3>& list)
{
	for (int32_t iii=0; iii<list.Size(); iii++) {
		if (list[iii] == point) {
			return iii;
		}
	}
	list.PushBack(point);
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
	etk::Vector<vec3> listVertex(m_listVertex);
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
		
		int32_t newCenterVertexID = CreateOrGetNewPointId(centerPoint, listVertex);
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
			int32_t newMiddleVertexID = CreateOrGetNewPointId(middlePoint, listVertex);
			listElementHalfPoint.PushBack(newMiddleVertexID);
			// create the center Texture coord:
			vec2 middleTex = (   listUV[m_listFaces[iii].m_uv[jjj]]
			                   + listUV[m_listFaces[iii].m_uv[cyclicID]]) / 2.0f;
			int32_t newMiddleTexID = CreateOrGetNewTexId(middleTex, listUV);
			listElementHalfUV.PushBack(newMiddleTexID);
		}
		// generate faces:
		for (int32_t jjj=0; jjj<listElementHalfPoint.Size() ; jjj+=2) {
			int32_t cyclicID = (jjj-1 + listElementHalfPoint.Size()) % listElementHalfPoint.Size();
			listFaces.PushBack(Face(listElementHalfPoint[jjj], listElementHalfUV[jjj],
			                        listElementHalfPoint[jjj+1], listElementHalfUV[jjj+1],
			                        newCenterVertexID, newCenterTexID,
			                        listElementHalfPoint[cyclicID], listElementHalfUV[cyclicID]) );
		}
	}
	// TODO SMOOTH :
	if (true==smooth) {
		
	}
	// copy all the element in the internal structure : 
	m_listVertex = listVertex;
	m_listUV = listUV;
	m_listFaces = listFaces;
}
