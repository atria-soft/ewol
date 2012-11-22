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

ewol::Image::Image(etk::UString imageName) :
	m_position(0.0, 0.0, 0.0),
	m_clippingPosStart(0.0, 0.0, 0.0),
	m_clippingPosStop(0.0, 0.0, 0.0),
	m_clippingEnable(false),
	m_color(draw::color::black),
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
		EWOL_WARNING("no resources ...");
		return;
	}
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	// set Matrix : translation/positionMatrix
	etk::Matrix4 tmpMatrix = ewol::openGL::GetMatrix()*m_matrixApply;
	m_GLprogram->Use(); 
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

void ewol::Image::Clear(void)
{
	// call upper class
	ewol::Compositing::Clear();
	// Reset Buffer :
	m_coord.Clear();
	m_coordTex.Clear();
	m_coordColor.Clear();
	// Reset temporal variables :
	m_position = etk::Vector3D<float>(0.0, 0.0, 0.0);
	m_clippingPosStart = etk::Vector3D<float>(0.0, 0.0, 0.0);
	m_clippingPosStop = etk::Vector3D<float>(0.0, 0.0, 0.0);
	m_clippingEnable = false;
	m_color = draw::color::black;
	m_axes = etk::Vector3D<float>(0.0, 0.0, 0.0);
	m_angle = 0.0;
}


etk::Vector3D<float> ewol::Image::GetPos(void)
{
	return m_position;
}


void ewol::Image::SetPos(etk::Vector3D<float> pos)
{
	m_position = pos;
}


void ewol::Image::SetRelPos(etk::Vector3D<float> pos)
{
	m_position += pos;
}


void ewol::Image::SetColor(draw::Color color)
{
	m_color = color;
}


void ewol::Image::SetClippingWidth(etk::Vector3D<float> pos, etk::Vector3D<float> width)
{
	SetClipping(pos, pos+width);
}

void ewol::Image::SetClipping(etk::Vector3D<float> pos, etk::Vector3D<float> posEnd)
{
	// note the internal system all time request to have a bounding all time in the same order
	if (pos.x <= posEnd.x) {
		m_clippingPosStart.x = pos.x;
		m_clippingPosStop.x = posEnd.x;
	} else {
		m_clippingPosStart.x = posEnd.x;
		m_clippingPosStop.x = pos.x;
	}
	if (pos.y <= posEnd.y) {
		m_clippingPosStart.y = pos.y;
		m_clippingPosStop.y = posEnd.y;
	} else {
		m_clippingPosStart.y = posEnd.y;
		m_clippingPosStop.y = pos.y;
	}
	if (pos.z <= posEnd.z) {
		m_clippingPosStart.z = pos.z;
		m_clippingPosStop.z = posEnd.z;
	} else {
		m_clippingPosStart.z = posEnd.z;
		m_clippingPosStop.z = pos.z;
	}
	m_clippingEnable = true;
}


void ewol::Image::SetClippingMode(bool newMode)
{
	m_clippingEnable = newMode;
}


void ewol::Image::SetAngle(etk::Vector3D<float> axes, float angle)
{
	m_axes = axes;
	m_angle = angle;
	if(    m_axes.x == 0
	    && m_axes.y == 0
	    && m_axes.z == 0) {
		m_angle = 0;
	}
}

void ewol::Image::Print(etk::Vector2D<int32_t> size)
{
	etk::Vector3D<float> point;
	etk::Vector2D<float> tex;
	point.z = 0;

	tex.x = 0;
	tex.y = 1;
	point.x = m_position.x;
	point.y = m_position.y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);


	tex.x = 1;
	tex.y = 1;
	point.x = m_position.x + size.x;
	point.y = m_position.y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);


	tex.x = 1;
	tex.y = 0;
	point.x = m_position.x + size.x;
	point.y = m_position.y + size.y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);

	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);

	tex.x = 0;
	tex.y = 0;
	point.x = m_position.x;
	point.y = m_position.y + size.y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);

	tex.x = 0;
	tex.y = 1;
	point.x = m_position.x;
	point.y = m_position.y;
	m_coord.PushBack(point);
	m_coordTex.PushBack(tex);
	m_coordColor.PushBack(m_color);
}

void ewol::Image::PrintPart(etk::Vector2D<int32_t> size,
               etk::Vector2D<float> sourcePosStart,
               etk::Vector2D<float> sourcePosStop)
{
	
}

void ewol::Image::SetSource(etk::UString newFile)
{
	Clear();
	// remove old one
	if (NULL != m_resource) {
		ewol::resource::Release(m_resource);
		m_resource = NULL;
	}
	etk::Vector2D<int32_t> size(-1,-1);
	// link to new One
	if (false == ewol::resource::Keep(newFile, m_resource, size)) {
		EWOL_ERROR("Can not get Image resource");
	}
}
