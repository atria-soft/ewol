/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/compositing/Image.h>

#undef __class__
#define __class__	"ewol::Image"

ewol::Image::Image(const etk::UString& _imageName) :
	m_position(0.0, 0.0, 0.0),
	m_clippingPosStart(0.0, 0.0, 0.0),
	m_clippingPosStop(0.0, 0.0, 0.0),
	m_clippingEnable(false),
	m_color(etk::color::white),
	m_angle(0.0),
	m_GLprogram(NULL),
	m_GLPosition(-1),
	m_GLMatrix(-1),
	m_GLColor(-1),
	m_GLtexture(-1),
	m_GLtexID(-1),
	m_resource(NULL)
{
	setSource(_imageName);
	loadProgram();
}

ewol::Image::~Image(void)
{
	ewol::TextureFile::release(m_resource);
	ewol::Program::release(m_GLprogram);
}

void ewol::Image::loadProgram(void)
{
	// get the shader resource :
	m_GLPosition = 0;
	m_GLprogram = ewol::Program::keep("DATA:textured3D.prog");
	if (NULL!=m_GLprogram) {
		m_GLPosition = m_GLprogram->getAttribute("EW_coord3d");
		m_GLColor    = m_GLprogram->getAttribute("EW_color");
		m_GLtexture  = m_GLprogram->getAttribute("EW_texture2d");
		m_GLMatrix   = m_GLprogram->getUniform("EW_MatrixTransformation");
		m_GLtexID    = m_GLprogram->getUniform("EW_texID");
	}
}

void ewol::Image::draw(bool _disableDepthTest)
{
	if (m_coord.size() <= 0) {
		//EWOL_WARNING("Nothink to draw...");
		return;
	}
	if (m_resource == NULL) {
		// this is a normale case ... the user can choice to have no image ...
		return;
	}
	if (m_GLprogram == NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	if (_disableDepthTest == true) {
		ewol::openGL::disable(ewol::openGL::FLAG_DEPTH_TEST);
	} else {
		ewol::openGL::enable(ewol::openGL::FLAG_DEPTH_TEST);
	}
	// set Matrix : translation/positionMatrix
	mat4 tmpMatrix = ewol::openGL::getMatrix()*m_matrixApply;
	m_GLprogram->use(); 
	m_GLprogram->uniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// TextureID
	m_GLprogram->setTexture0(m_GLtexID, m_resource->getId());
	// position :
	m_GLprogram->sendAttribute(m_GLPosition, 3/*x,y,z,unused*/, &m_coord[0], 4*sizeof(btScalar));
	// Texture :
	m_GLprogram->sendAttribute(m_GLtexture, 2/*u,v*/, &m_coordTex[0]);
	// color :
	m_GLprogram->sendAttribute(m_GLColor, 4/*r,g,b,a*/, &m_coordColor[0]);
	// Request the draw od the elements : 
	ewol::openGL::drawArrays(GL_TRIANGLES, 0, m_coord.size());
	m_GLprogram->unUse();
}

void ewol::Image::clear(void)
{
	// call upper class
	ewol::Compositing::clear();
	// reset Buffer :
	m_coord.clear();
	m_coordTex.clear();
	m_coordColor.clear();
	// reset temporal variables :
	m_position = vec3(0.0, 0.0, 0.0);
	m_clippingPosStart = vec3(0.0, 0.0, 0.0);
	m_clippingPosStop = vec3(0.0, 0.0, 0.0);
	m_clippingEnable = false;
	m_color = etk::color::white;
	m_angle = 0.0;
}

void ewol::Image::setClipping(const vec3& _pos, vec3 _posEnd)
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

void ewol::Image::setAngle(float _angle)
{
	m_angle = _angle;
}

void ewol::Image::print(const vec2& _size)
{
	printPart(_size, vec2(0,0), vec2(1,1));
}

void ewol::Image::printPart(const vec2& _size,
                            const vec2& _sourcePosStart,
                            const vec2& _sourcePosStop)
{
	if (m_angle == 0.0f) {
		vec3 point = m_position;
		vec2 tex(_sourcePosStart.x(),_sourcePosStop.y());
		
		m_coord.pushBack(point);
		m_coordTex.pushBack(tex);
		m_coordColor.pushBack(m_color);
		
		
		tex.setValue(_sourcePosStop.x(),_sourcePosStop.y());
		point.setX(m_position.x() + _size.x());
		point.setY(m_position.y());
		m_coord.pushBack(point);
		m_coordTex.pushBack(tex);
		m_coordColor.pushBack(m_color);
		
		
		tex.setValue(_sourcePosStop.x(),_sourcePosStart.y());
		point.setX(m_position.x() + _size.x());
		point.setY(m_position.y() + _size.y());
		m_coord.pushBack(point);
		m_coordTex.pushBack(tex);
		m_coordColor.pushBack(m_color);
		
		m_coord.pushBack(point);
		m_coordTex.pushBack(tex);
		m_coordColor.pushBack(m_color);
		
		tex.setValue(_sourcePosStart.x(),_sourcePosStart.y());
		point.setX(m_position.x());
		point.setY(m_position.y() + _size.y());
		m_coord.pushBack(point);
		m_coordTex.pushBack(tex);
		m_coordColor.pushBack(m_color);
		
		tex.setValue(_sourcePosStart.x(),_sourcePosStop.y());
		point.setX(m_position.x());
		point.setY(m_position.y());
		m_coord.pushBack(point);
		m_coordTex.pushBack(tex);
		m_coordColor.pushBack(m_color);
		return;
	}
	vec3 center = m_position + vec3(_size.x(),_size.y(),0)/2.0f;
	vec3 limitedSize(_size.x()*0.5f, _size.y()*0.5f, 0.0f);
	
	vec3 point(0,0,0);
	vec2 tex(_sourcePosStart.x(),_sourcePosStop.y());
	
	point.setValue(-limitedSize.x(), -limitedSize.y(), 0);
	point = point.rotate(vec3(0,0,1), m_angle) + center;
	m_coord.pushBack(point);
	m_coordTex.pushBack(tex);
	m_coordColor.pushBack(m_color);
	
	
	tex.setValue(_sourcePosStop.x(),_sourcePosStop.y());
	point.setValue(limitedSize.x(), -limitedSize.y(), 0);
	point = point.rotate(vec3(0,0,1), m_angle) + center;
	m_coord.pushBack(point);
	m_coordTex.pushBack(tex);
	m_coordColor.pushBack(m_color);
	
	
	tex.setValue(_sourcePosStop.x(),_sourcePosStart.y());
	point.setValue(limitedSize.x(), limitedSize.y(), 0);
	point = point.rotate(vec3(0,0,1), m_angle) + center;
	m_coord.pushBack(point);
	m_coordTex.pushBack(tex);
	m_coordColor.pushBack(m_color);
	
	m_coord.pushBack(point);
	m_coordTex.pushBack(tex);
	m_coordColor.pushBack(m_color);
	
	tex.setValue(_sourcePosStart.x(),_sourcePosStart.y());
	point.setValue(-limitedSize.x(), limitedSize.y(), 0);
	point = point.rotate(vec3(0,0,1), m_angle) + center;
	m_coord.pushBack(point);
	m_coordTex.pushBack(tex);
	m_coordColor.pushBack(m_color);
	
	tex.setValue(_sourcePosStart.x(),_sourcePosStop.y());
	point.setValue(-limitedSize.x(), -limitedSize.y(), 0);
	point = point.rotate(vec3(0,0,1), m_angle) + center;
	m_coord.pushBack(point);
	m_coordTex.pushBack(tex);
	m_coordColor.pushBack(m_color);
}

void ewol::Image::setSource(const etk::UString& _newFile, const vec2& _size)
{
	clear();
	// remove old one
	ewol::TextureFile::release(m_resource);
	ivec2 tmpSize(_size.x(),_size.y());
	// note that no image can be loaded...
	if (_newFile != "") {
		// link to new one
		m_resource = ewol::TextureFile::keep(_newFile, tmpSize);
		if (NULL == m_resource) {
			EWOL_ERROR("Can not get Image resource");
		}
	}
}

bool ewol::Image::hasSources(void)
{
	return m_resource!=NULL;
}


vec2 ewol::Image::getRealSize(void)
{
	if (NULL == m_resource) {
		return vec2(0,0);
	}
	return m_resource->getRealSize();
}



