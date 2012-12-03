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
	m_texture1(NULL)
{
	etk::UString tmpString("DATA:textured3D.prog");
	// get the shader resource :
	m_GLPosition = 0;
	if (true == ewol::resource::Keep(tmpString, m_GLprogram) ) {
		m_GLPosition = m_GLprogram->GetAttribute("EW_coord3d");
		m_GLColor    = m_GLprogram->GetAttribute("EW_color");
		m_GLtexture  = m_GLprogram->GetAttribute("EW_texture2d");
		m_GLMatrix   = m_GLprogram->GetUniform("EW_MatrixTransformation");
		m_GLtexID    = m_GLprogram->GetUniform("EW_texID");
	}
}

ewol::Mesh::~Mesh(void)
{
	// remove dynamics dependencies :
	if(NULL!=m_texture1) {
		ewol::resource::Release(m_texture1);
	}
	ewol::resource::Release(m_GLprogram);
}


void ewol::Mesh::Draw(void)
{
	static float rotx = 0;
	static float roty = 0;
	static float rotz = 0;
	rotx += 0.01;
	roty += 0.02;
	rotz += 0.005;
	if (m_vertices.Size()<=0) {
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
	mat4 tmpMatrix = ewol::openGL::GetMatrix();
	tmpMatrix =   mat4::Scale(vec3(100,100,100) )
	            * mat4::Rotate(vec3(1,0,0), rotx)
	            * mat4::Rotate(vec3(0,1,0), roty)/*
	            * mat4::Translate(vec3(0.01,0.0,0.0))
	            * mat4::Rotate(vec3(0,0,1), rotz)*/
	            * tmpMatrix;
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// TextureID
	m_GLprogram->SetTexture0(m_GLtexID, m_texture1->GetId());
	// position :
	m_GLprogram->SendAttribute(m_GLPosition, 3/*x,y,z*/, &m_vertices[0]);
	// Texture :
	m_GLprogram->SendAttribute(m_GLtexture, 2/*u,v*/, &m_uvTextures[0]);
	// color :
	m_GLprogram->SendAttribute(m_GLColor, 4/*r,g,b,a*/, &m_coordColor[0]);
	// Request the draw od the elements : 
	glDrawArrays(GL_TRIANGLES, 0, m_vertices.Size());
	m_GLprogram->UnUse();
	glDisable(GL_DEPTH_TEST);
}

