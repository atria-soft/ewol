/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/compositing/Image.h>
#include <ewol/config.h>

#undef __class__
#define __class__	"ewol::Image"

ewol::Image::Image(const etk::UString& _imageName) :
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
	SetSource(_imageName);
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

void ewol::Image::SetClipping(const vec3& _pos, vec3 _posEnd)
{
	// note the internal system all time request to have a bounding all time in the same order
	if (_pos.x() <= _posEnd.x()) {
		m_clippingPosStart.setX(_pos.x());
		m_clippingPosStop.setX(_posEnd.x());
	} else {
		m_clippingPosStart.setX(_posEnd.x());
		m_clippingPosStop.setX(_pos.x());
	}
	if (_pos.y() <= _posEnd.y()) {
		m_clippingPosStart.setY(_pos.y());
		m_clippingPosStop.setY(_posEnd.y());
	} else {
		m_clippingPosStart.setY(_posEnd.y());
		m_clippingPosStop.setY(_pos.y());
	}
	if (_pos.z() <= _posEnd.z()) {
		m_clippingPosStart.setZ(_pos.z());
		m_clippingPosStop.setZ(_posEnd.z());
	} else {
		m_clippingPosStart.setZ(_posEnd.z());
		m_clippingPosStop.setZ(_pos.z());
	}
	m_clippingEnable = true;
}

void ewol::Image::SetAngle(const vec3& _axes, float _angle)
{
	m_axes = _axes;
	m_angle = _angle;
	if(    m_axes.x() == 0
	    && m_axes.y() == 0
	    && m_axes.z() == 0) {
		m_angle = 0;
	}
}

void ewol::Image::Print(const vec2& _size)
{
	#if 1
		PrintPart(_size, vec2(0,0), vec2(1,1));
	#else
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
	#endif
}

void ewol::Image::PrintPart(const vec2& _size,
                            const vec2& _sourcePosStart,
                            const vec2& _sourcePosStop)
{
	vec3 point(0,0,0);
	vec2 tex(_sourcePosStart.x(),_sourcePosStop.y());

	point.setX(m_position.x());
	point.setY(m_position.y());
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);


	tex.setValue(_sourcePosStop.x(),_sourcePosStop.y());
	point.setX(m_position.x() + _size.x());
	point.setY(m_position.y());
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);


	tex.setValue(_sourcePosStop.x(),_sourcePosStart.y());
	point.setX(m_position.x() + _size.x());
	point.setY(m_position.y() + _size.y());
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);

	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);

	tex.setValue(_sourcePosStart.x(),_sourcePosStart.y());
	point.setX(m_position.x());
	point.setY(m_position.y() + _size.y());
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);

	tex.setValue(_sourcePosStart.x(),_sourcePosStop.y());
	point.setX(m_position.x());
	point.setY(m_position.y());
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);
}

void ewol::Image::SetSource(const etk::UString& _newFile, const vec2& _size)
{
	Clear();
	// remove old one
	if (NULL != m_resource) {
		ewol::resource::Release(m_resource);
		m_resource = NULL;
	}
	ivec2 tmpSize(_size.x(),_size.y());
	// note that no image can be loaded...
	if (_newFile != "") {
		// link to new One
		if (false == ewol::resource::Keep(_newFile, m_resource, tmpSize)) {
			EWOL_ERROR("Can not get Image resource");
		}
	}
}

bool ewol::Image::HasSources(void)
{
	return m_resource!=NULL;
}


vec2 ewol::Image::GetRealSize(void)
{
	if (NULL==m_resource) {
		return vec2(0,0);
	}
	return m_resource->GetRealSize();
}



