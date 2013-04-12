/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <tinyXML/tinyxml.h>

#include <ewol/debug.h>
#include <ewol/compositing/Image.h>
#include <ewol/config.h>

#undef __class__
#define __class__	"ewol::Image"

ewol::Image::Image(const etk::UString& imageName) :
	m_position(0.0, 0.0, 0.0),
	m_clippingPosStart(0.0, 0.0, 0.0),
	m_clippingPosStop(0.0, 0.0, 0.0),
	m_clippingEnable(false),
	m_color(draw::color::white),
	m_axes(0.0, 0.0, 0.0),
	m_angle(0.0),
	m_GLprogram(NULL),
	m_GLPosition(-1),
	m_GLMatrix(-1),
	m_GLColor(-1),
	m_GLtexture(-1),
	m_GLtexID(-1),
	m_resource(NULL)
{
	SetSource(imageName);
	LoadProgram();
}

ewol::Image::~Image(void)
{
	if (NULL != m_resource) {
		ewol::resource::Release(m_resource);
		m_resource = NULL;
	}
	ewol::resource::Release(m_GLprogram);
}

void ewol::Image::LoadProgram(void)
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

void ewol::Image::Draw(void)
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

void ewol::Image::Clear(void)
{
	// call upper class
	ewol::Compositing::Clear();
	// Reset Buffer :
	m_coord.Clear();
	m_coordTex.Clear();
	m_coordColor.Clear();
	// Reset temporal variables :
	m_position = vec3(0.0, 0.0, 0.0);
	m_clippingPosStart = vec3(0.0, 0.0, 0.0);
	m_clippingPosStop = vec3(0.0, 0.0, 0.0);
	m_clippingEnable = false;
	m_color = draw::color::white;
	m_axes = vec3(0.0, 0.0, 0.0);
	m_angle = 0.0;
}


vec3 ewol::Image::GetPos(void)
{
	return m_position;
}


void ewol::Image::SetPos(const vec3& pos)
{
	m_position = pos;
}


void ewol::Image::SetRelPos(const vec3& pos)
{
	m_position += pos;
}


void ewol::Image::SetColor(const draw::Color& color)
{
	m_color = color;
}


void ewol::Image::SetClippingWidth(const vec3& pos, vec3 width)
{
	SetClipping(pos, pos+width);
}

void ewol::Image::SetClipping(const vec3& pos, vec3 posEnd)
{
	// note the internal system all time request to have a bounding all time in the same order
	if (pos.x() <= posEnd.x()) {
		m_clippingPosStart.setX(pos.x());
		m_clippingPosStop.setX(posEnd.x());
	} else {
		m_clippingPosStart.setX(posEnd.x());
		m_clippingPosStop.setX(pos.x());
	}
	if (pos.y() <= posEnd.y()) {
		m_clippingPosStart.setY(pos.y());
		m_clippingPosStop.setY(posEnd.y());
	} else {
		m_clippingPosStart.setY(posEnd.y());
		m_clippingPosStop.setY(pos.y());
	}
	if (pos.z() <= posEnd.z()) {
		m_clippingPosStart.setZ(pos.z());
		m_clippingPosStop.setZ(posEnd.z());
	} else {
		m_clippingPosStart.setZ(posEnd.z());
		m_clippingPosStop.setZ(pos.z());
	}
	m_clippingEnable = true;
}


void ewol::Image::SetClippingMode(bool newMode)
{
	m_clippingEnable = newMode;
}


void ewol::Image::SetAngle(const vec3& axes, float angle)
{
	m_axes = axes;
	m_angle = angle;
	if(    m_axes.x() == 0
	    && m_axes.y() == 0
	    && m_axes.z() == 0) {
		m_angle = 0;
	}
}

void ewol::Image::Print(const ivec2& size)
{
	Print(vec2(size.x(),size.y()));
}

void ewol::Image::Print(const vec2& size)
{
	vec3 point(0,0,0);
	vec2 tex(0,1);

	point.setX(m_position.x());
	point.setY(m_position.y());
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);


	tex.setValue(1,1);
	point.setX(m_position.x() + size.x());
	point.setY(m_position.y());
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);


	tex.setValue(1,0);
	point.setX(m_position.x() + size.x());
	point.setY(m_position.y() + size.y());
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);

	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);

	tex.setValue(0,0);
	point.setX(m_position.x());
	point.setY(m_position.y() + size.y());
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

void ewol::Image::PrintPart(const ivec2& size,
                            const vec2& sourcePosStart,
                            const vec2& sourcePosStop)
{
	
}

void ewol::Image::SetSource(const etk::UString& newFile, int32_t size)
{
	SetSource(newFile, vec2(size,size));
}

void ewol::Image::SetSource(const etk::UString& newFile, const vec2& size)
{
	Clear();
	// remove old one
	if (NULL != m_resource) {
		ewol::resource::Release(m_resource);
		m_resource = NULL;
	}
	ivec2 tmpSize(size.x(),size.y());
	// note that no image can be loaded...
	if (newFile != "") {
		// link to new One
		if (false == ewol::resource::Keep(newFile, m_resource, tmpSize)) {
			EWOL_ERROR("Can not get Image resource");
		}
	}
}

bool ewol::Image::HasSources(void)
{
	return m_resource!=NULL;
}

