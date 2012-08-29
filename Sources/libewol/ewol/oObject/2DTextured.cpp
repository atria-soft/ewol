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

#include <ewol/oObject/2DTextured.h>
#include <ewol/ResourceManager.h>
#include <ewol/openGL/openGL.h>

#undef __class__
#define __class__	"ewol::OObject2DTextured"


ewol::OObject2DTextured::OObject2DTextured(etk::UString textureName, float sizeX, float sizeY)
{
	EWOL_VERBOSE("Create OObject textured : \"" << textureName << "\"");
	if (false == ewol::resource::Keep(textureName, m_resource, Vector2D<int32_t>(sizeX,sizeY)) ) {
		EWOL_CRITICAL("can not get a resource Texture");
	}
	#ifdef __VIDEO__OPENGL_ES_2
		etk::UString tmpString("textured.prog");
		// get the shader resource :
		m_GLPosition = 0;
		if (true == ewol::resource::Keep(tmpString, m_GLprogram) ) {
			m_GLPosition = m_GLprogram->GetAttribute("EW_coord2d");
			m_GLColor    = m_GLprogram->GetAttribute("EW_color");
			m_GLtexture  = m_GLprogram->GetAttribute("EW_texture2d");
			m_GLMatrix   = m_GLprogram->GetUniform("EW_MatrixTransformation");
			m_GLtexID    = m_GLprogram->GetUniform("EW_texID");
		}
	#endif
}


ewol::OObject2DTextured::~OObject2DTextured(void)
{
	if (NULL != m_resource) {
		ewol::resource::Release(m_resource);
	}
	#ifdef __VIDEO__OPENGL_ES_2
		ewol::resource::Release(m_GLprogram);
	#endif
}

void ewol::OObject2DTextured::Draw(void)
{
	if (m_coord.Size()<=0) {
		return;
	}
	if (NULL == m_resource) {
		EWOL_WARNING("Texture does not exist ...");
		return;
	}
	#ifdef __VIDEO__OPENGL_ES_2
		if (m_GLprogram==NULL) {
			EWOL_ERROR("No shader ...");
			return;
		}
		glColor4f(1.0, 1.0, 1.0, 1.0);
		m_GLprogram->Use();
		// set Matrix : translation/positionMatrix
		etk::Matrix tmpMatrix = ewol::openGL::GetMatrix();
		m_GLprogram->SendUniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
		// TextureID
		m_GLprogram->SetTexture0(m_GLtexID, m_resource->GetId());
		// position :
		m_GLprogram->SendAttribute(m_GLPosition, 2/*x,y*/, &m_coord[0]);
		// Texture :
		m_GLprogram->SendAttribute(m_GLtexture, 2/*u,v*/, &m_coordTex[0]);
		// color :
		m_GLprogram->SendAttribute(m_GLColor, 4/*r,g,b,a*/, &m_coordColor[0]);
		// Request the draw od the elements : 
		glDrawArrays(GL_TRIANGLES, 0, m_coord.Size());
		m_GLprogram->UnUse();
	#else
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glEnable(GL_TEXTURE_2D);
		//EWOL_WARNING("Draw with texture : " << m_textureId << " ==> ogl=" << ewol::texture::GetGLID(m_textureId));
		glBindTexture(GL_TEXTURE_2D, m_resource->GetId() );
		glEnableClientState( GL_VERTEX_ARRAY );                     // Enable Vertex Arrays
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );              // Enable Texture Coord Arrays
		glEnableClientState( GL_COLOR_ARRAY );                      // Enable Color Arrays
		glVertexPointer( 2, GL_FLOAT, 0, &m_coord[0] );
		glTexCoordPointer( 2, GL_FLOAT, 0, &m_coordTex[0] );
		glColorPointer(    4, GL_UNSIGNED_BYTE, 0, &m_coordColor[0] );
		glDrawArrays( GL_TRIANGLES, 0, m_coord.Size());
		//EWOL_DEBUG("request draw of " << m_coord.Size() << " elements");
		glDisableClientState( GL_COLOR_ARRAY );                     // Disable Color Arrays
		glDisableClientState( GL_VERTEX_ARRAY );                    // Disable Vertex Arrays
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );             // Disable Texture Coord Arrays
		glDisable(GL_TEXTURE_2D);
	#endif
}

void ewol::OObject2DTextured::Clear(void)
{
	m_coord.Clear();
	m_coordTex.Clear();
	m_coordColor.Clear();
}

void ewol::OObject2DTextured::Rectangle(float x, float y, float w, float h, draw::Color tmpColor)
{
	Rectangle(x, y, w, h, 0.0, 0.0, 1.0, 1.0, tmpColor);
}

void ewol::OObject2DTextured::Rectangle(float x, float y, float w, float h, float texX, float texY, float texSX, float texSY, draw::Color tmpColor)
{
	/*
	x += 3;
	y += 3;
	w -= 6;
	h -= 6;
	*/
	//EWOL_DEBUG("Add rectangle : ...");
	Vector2D<float> point;
	texCoord_ts tex;

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
