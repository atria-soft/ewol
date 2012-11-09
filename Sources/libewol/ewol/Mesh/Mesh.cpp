/**
 *******************************************************************************
 * @file ewol/Mesh/Mesh.cpp
 * @brief ewol Mesh system (sources)
 * @author Edouard DUPIN
 * @date 30/10/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */


#include <ewol/Debug.h>
#include <ewol/Mesh/Mesh.h>
#include <ewol/ResourceManager.h>


ewol::Mesh::Mesh(etk::UString genName) :
	ewol::Resource(genName),
	m_texture1(NULL)
{
	etk::UString tmpString("textured3D.prog");
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
	//EWOL_DEBUG("    Display " << m_coord.Size() << " elements" );
	m_GLprogram->Use();
	// set Matrix : translation/positionMatrix
	etk::Matrix4 tmpMatrix = ewol::openGL::GetMatrix();
	tmpMatrix =   etk::matrix::Scale(100,100,100)
	            * etk::matrix::rotate(1,0,0,rotx)
	            * etk::matrix::rotate(0,1,0,roty)/*
	            * etk::matrix::Translate(0.01,0,0)
	            * etk::matrix::rotate(0,0,1,rotz)*/
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
}

