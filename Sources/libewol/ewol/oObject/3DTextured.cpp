/**
 *******************************************************************************
 * @file ewol/oObject/2DTextured.cpp
 * @brief ewol OpenGl Object system (Sources)
 * @author Edouard DUPIN
 * @date 09/11/2011
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

#include <ewol/oObject/3DTextured.h>
#include <ewol/ResourceManager.h>
#include <ewol/openGL/openGL.h>

#undef __class__
#define __class__	"ewol::OObject3DTextured"


ewol::OObject3DTextured::OObject3DTextured(etk::UString textureName, float sizeX, float sizeY)
{
	EWOL_VERBOSE("Create OObject textured : \"" << textureName << "\"");
	if (false == ewol::resource::Keep(textureName, m_resource, etk::Vector2D<int32_t>(sizeX,sizeY)) ) {
		EWOL_CRITICAL("can not get a resource Texture");
	}
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


ewol::OObject3DTextured::~OObject3DTextured(void)
{
	if (NULL != m_resource) {
		ewol::resource::Release(m_resource);
	}
	ewol::resource::Release(m_GLprogram);
}

void ewol::OObject3DTextured::Draw(void)
{
	if (m_coord.Size()<=0) {
		return;
	}
	if (NULL == m_resource) {
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
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// TextureID
	m_GLprogram->SetTexture0(m_GLtexID, m_resource->GetId());
	// position :
	m_GLprogram->SendAttribute(m_GLPosition, 3/*x,y,z*/, &m_coord[0]);
	// Texture :
	m_GLprogram->SendAttribute(m_GLtexture, 2/*u,v*/, &m_coordTex[0]);
	// color :
	m_GLprogram->SendAttribute(m_GLColor, 4/*r,g,b,a*/, &m_coordColor[0]);
	// Request the draw od the elements : 
	glDrawArrays(GL_TRIANGLES, 0, m_coord.Size());
	m_GLprogram->UnUse();
}

void ewol::OObject3DTextured::Clear(void)
{
	m_coord.Clear();
	m_coordTex.Clear();
	m_coordColor.Clear();
}

void ewol::OObject3DTextured::Rectangle(float x, float y, float w, float h, draw::Color tmpColor)
{
	Rectangle(x, y, w, h, 0.0, 0.0, 1.0, 1.0, tmpColor);
}

void ewol::OObject3DTextured::Rectangle(float x, float y, float w, float h, float texX, float texY, float texSX, float texSY, draw::Color tmpColor)
{
	/*
	x += 3;
	y += 3;
	w -= 6;
	h -= 6;
	*/
	//EWOL_DEBUG("Add rectangle : ...");
	etk::Vector3D<float> point;
	texCoord_ts tex;
	point.z = 0;

	tex.u = texX;
	tex.v = texSY;
	point.x = x;
	point.y = y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(tmpColor);


	tex.u = texSX;
	tex.v = texSY;
	point.x = x + w;
	point.y = y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(tmpColor);


	tex.u = texSX;
	tex.v = texY;
	point.x = x + w;
	point.y = y + h;
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(tmpColor);

	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(tmpColor);

	tex.u = texX;
	tex.v = texY;
	point.x = x;
	point.y = y + h;
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(tmpColor);

	tex.u = texX;
	tex.v = texSY;
	point.x = x;
	point.y = y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(tmpColor);
}
