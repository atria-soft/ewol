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
//		m_GLColor    = m_GLprogram->GetAttribute("EW_color");
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
	m_GLprogram->SendAttributePointer(m_GLPosition, 3/*x,y,z*/, m_verticesVBO, 0);
	// Texture :
	m_GLprogram->SendAttributePointer(m_GLtexture, 2/*u,v*/, m_verticesVBO, 1);
	// color :
	//m_GLprogram->SendAttributePointer(m_GLColor, 4/*r,g,b,a*/, m_verticesVBO, 2);
	// Request the draw od the elements : 
	glDrawArrays(GL_TRIANGLES, 0, m_numberOfElments);
	m_GLprogram->UnUse();
	glDisable(GL_DEPTH_TEST);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void ewol::Mesh::GenerateVBO(void)
{
	m_numberOfElments = 0;
	// TODO : Set a better display system, this one is the worst I known ...
	for (int32_t iii=0; iii<m_listFaces.Size() ; iii++) {
		m_numberOfElments += m_listFaces[iii].m_nbElement;
		// 2 possibilities : triangle or quad :
		int32_t indice = 0;
		vec3 tmpPos = m_listVertex[m_listFaces[iii].m_vertex[indice]];
		vec2 tmpUV = m_listUV[m_listFaces[iii].m_uv[indice]];
		m_verticesVBO->GetRefBuffer(0).PushBack(tmpPos.x());
		m_verticesVBO->GetRefBuffer(0).PushBack(tmpPos.y());
		m_verticesVBO->GetRefBuffer(0).PushBack(tmpPos.z());
		m_verticesVBO->GetRefBuffer(1).PushBack(tmpUV.x());
		m_verticesVBO->GetRefBuffer(1).PushBack(1.0f-tmpUV.y());
		
		indice = 1;
		tmpPos = m_listVertex[m_listFaces[iii].m_vertex[indice]];
		tmpUV = m_listUV[m_listFaces[iii].m_uv[indice]];
		m_verticesVBO->GetRefBuffer(0).PushBack(tmpPos.x());
		m_verticesVBO->GetRefBuffer(0).PushBack(tmpPos.y());
		m_verticesVBO->GetRefBuffer(0).PushBack(tmpPos.z());
		m_verticesVBO->GetRefBuffer(1).PushBack(tmpUV.x());
		m_verticesVBO->GetRefBuffer(1).PushBack(1.0f-tmpUV.y());
		
		indice = 2;
		tmpPos = m_listVertex[m_listFaces[iii].m_vertex[indice]];
		tmpUV = m_listUV[m_listFaces[iii].m_uv[indice]];
		m_verticesVBO->GetRefBuffer(0).PushBack(tmpPos.x());
		m_verticesVBO->GetRefBuffer(0).PushBack(tmpPos.y());
		m_verticesVBO->GetRefBuffer(0).PushBack(tmpPos.z());
		m_verticesVBO->GetRefBuffer(1).PushBack(tmpUV.x());
		m_verticesVBO->GetRefBuffer(1).PushBack(1.0f-tmpUV.y());
		
		if (m_listFaces[iii].m_nbElement==4) {
			indice = 0;
			tmpPos = m_listVertex[m_listFaces[iii].m_vertex[indice]];
			tmpUV = m_listUV[m_listFaces[iii].m_uv[indice]];
			m_verticesVBO->GetRefBuffer(0).PushBack(tmpPos.x());
			m_verticesVBO->GetRefBuffer(0).PushBack(tmpPos.y());
			m_verticesVBO->GetRefBuffer(0).PushBack(tmpPos.z());
			m_verticesVBO->GetRefBuffer(1).PushBack(tmpUV.x());
			m_verticesVBO->GetRefBuffer(1).PushBack(1.0f-tmpUV.y());
			
			indice = 2;
			tmpPos = m_listVertex[m_listFaces[iii].m_vertex[indice]];
			tmpUV = m_listUV[m_listFaces[iii].m_uv[indice]];
			m_verticesVBO->GetRefBuffer(0).PushBack(tmpPos.x());
			m_verticesVBO->GetRefBuffer(0).PushBack(tmpPos.y());
			m_verticesVBO->GetRefBuffer(0).PushBack(tmpPos.z());
			m_verticesVBO->GetRefBuffer(1).PushBack(tmpUV.x());
			m_verticesVBO->GetRefBuffer(1).PushBack(1.0f-tmpUV.y());
			
			indice = 3;
			tmpPos = m_listVertex[m_listFaces[iii].m_vertex[indice]];
			tmpUV = m_listUV[m_listFaces[iii].m_uv[indice]];
			m_verticesVBO->GetRefBuffer(0).PushBack(tmpPos.x());
			m_verticesVBO->GetRefBuffer(0).PushBack(tmpPos.y());
			m_verticesVBO->GetRefBuffer(0).PushBack(tmpPos.z());
			m_verticesVBO->GetRefBuffer(1).PushBack(tmpUV.x());
			m_verticesVBO->GetRefBuffer(1).PushBack(1.0f-tmpUV.y());
		}
	}
	// update all the VBO elements ...
	m_verticesVBO->Flush();
}

