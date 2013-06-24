/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/compositing/Area.h>
#include <ewol/config.h>

#undef __class__
#define __class__	"ewol::Area"

ewol::Area::Area(const ivec2& _size) :
	m_position(0.0, 0.0, 0.0),
	m_color(draw::color::white),
	m_GLprogram(NULL),
	m_GLPosition(-1),
	m_GLMatrix(-1),
	m_GLColor(-1),
	m_GLtexture(-1),
	m_GLtexID(-1),
	m_resource(NULL)
{
	ewol::resource::Keep(m_resource);
	m_resource->SetImageSize(_size);
	m_resource->Flush();
	LoadProgram();
}

ewol::Area::~Area(void)
{
	if (NULL != m_resource) {
		ewol::resource::Release(m_resource);
		m_resource = NULL;
	}
	ewol::resource::Release(m_GLprogram);
}

void ewol::Area::LoadProgram(void)
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

void ewol::Area::Draw(void)
{
	if (m_coord.Size()<=0) {
		//EWOL_WARNING("Nothink to draw...");
		return;
	}
	if (m_resource == NULL) {
		// this is a normale case ... the user can choice to have no image ...
		return;
	}
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	// set Matrix : translation/positionMatrix
	mat4 tmpMatrix = ewol::openGL::GetMatrix()*m_matrixApply;
	m_GLprogram->Use(); 
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// TextureID
	m_GLprogram->SetTexture0(m_GLtexID, m_resource->GetId());
	// position :
	m_GLprogram->SendAttribute(m_GLPosition, 3/*x,y,z,unused*/, &m_coord[0], 4*sizeof(btScalar));
	// Texture :
	m_GLprogram->SendAttribute(m_GLtexture, 2/*u,v*/, &m_coordTex[0]);
	// color :
	m_GLprogram->SendAttribute(m_GLColor, 4/*r,g,b,a*/, &m_coordColor[0]);
	// Request the draw od the elements : 
	ewol::openGL::DrawArrays(GL_TRIANGLES, 0, m_coord.Size());
	m_GLprogram->UnUse();
}

void ewol::Area::Clear(void)
{
	// call upper class
	ewol::Compositing::Clear();
	// Reset Buffer :
	m_coord.Clear();
	m_coordTex.Clear();
	m_coordColor.Clear();
	// Reset temporal variables :
	m_position = vec3(0.0, 0.0, 0.0);
}

void ewol::Area::Print(const ivec2& _size)
{
	vec3 point(0,0,0);
	vec2 tex(0,1);

	point.setX(m_position.x());
	point.setY(m_position.y());
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);


	tex.setValue(1,1);
	point.setX(m_position.x() + _size.x());
	point.setY(m_position.y());
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);


	tex.setValue(1,0);
	point.setX(m_position.x() + _size.x());
	point.setY(m_position.y() + _size.y());
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);

	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);

	tex.setValue(0,0);
	point.setX(m_position.x());
	point.setY(m_position.y() + _size.y());
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);

	tex.setValue(0,1);
	point.setX(m_position.x());
	point.setY(m_position.y());
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);
}


