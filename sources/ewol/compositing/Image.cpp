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
#define __class__	"ewol::compositing::Image"

ewol::compositing::Image::Image(const std::string& _imageName, bool _df) :
  m_filename(_imageName),
  m_requestSize(2,2),
  m_position(0.0, 0.0, 0.0),
  m_clippingPosStart(0.0, 0.0, 0.0),
  m_clippingPosStop(0.0, 0.0, 0.0),
  m_clippingEnable(false),
  m_color(etk::color::white),
  m_angle(0.0),
  m_GLprogram(nullptr),
  m_GLPosition(-1),
  m_GLMatrix(-1),
  m_GLColor(-1),
  m_GLtexture(-1),
  m_GLtexID(-1),
  m_distanceFieldMode(_df),
  m_resource(nullptr),
  m_resourceDF(nullptr) {
	setSource(_imageName);
	loadProgram();
}

ewol::compositing::Image::~Image() {
	ewol::resource::TextureFile::release(m_resource);
	ewol::resource::ImageDF::release(m_resourceDF);
	ewol::resource::Program::release(m_GLprogram);
}

void ewol::compositing::Image::loadProgram() {
	// get the shader resource :
	m_GLPosition = 0;
	ewol::resource::Program::release(m_GLprogram);
	if (m_distanceFieldMode == true) {
		m_GLprogram = ewol::resource::Program::keep("DATA:texturedDF.prog");
	} else {
		m_GLprogram = ewol::resource::Program::keep("DATA:textured3D.prog");
	}
	if (m_GLprogram != nullptr) {
		m_GLPosition = m_GLprogram->getAttribute("EW_coord3d");
		m_GLColor    = m_GLprogram->getAttribute("EW_color");
		m_GLtexture  = m_GLprogram->getAttribute("EW_texture2d");
		m_GLMatrix   = m_GLprogram->getUniform("EW_MatrixTransformation");
		m_GLtexID    = m_GLprogram->getUniform("EW_texID");
	}
}

void ewol::compositing::Image::draw(bool _disableDepthTest) {
	if (m_coord.size() <= 0) {
		//EWOL_WARNING("Nothink to draw...");
		return;
	}
	if (    m_resource == nullptr
	     && m_resourceDF == nullptr) {
		// this is a normale case ... the user can choice to have no image ...
		return;
	}
	if (m_GLprogram == nullptr) {
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
	if (m_resource != nullptr) {
		if (m_distanceFieldMode == true) {
			EWOL_ERROR("FONT type error Request distance field and display normal ...");
		}
		m_GLprogram->setTexture0(m_GLtexID, m_resource->getId());
	} else {
		if (m_distanceFieldMode == false) {
			EWOL_ERROR("FONT type error Request normal and display distance field ...");
		}
		m_GLprogram->setTexture0(m_GLtexID, m_resourceDF->getId());
	}
	// position :
	m_GLprogram->sendAttribute(m_GLPosition, m_coord);
	// Texture :
	m_GLprogram->sendAttribute(m_GLtexture, m_coordTex);
	// color :
	m_GLprogram->sendAttribute(m_GLColor, m_coordColor);
	// Request the draw od the elements : 
	ewol::openGL::drawArrays(GL_TRIANGLES, 0, m_coord.size());
	m_GLprogram->unUse();
}

void ewol::compositing::Image::clear() {
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

void ewol::compositing::Image::setClipping(const vec3& _pos, vec3 _posEnd) {
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

void ewol::compositing::Image::setAngle(float _angle) {
	m_angle = _angle;
}

void ewol::compositing::Image::print(const vec2& _size) {
	printPart(_size, vec2(0,0), vec2(1.0,1.0));
}

void ewol::compositing::Image::printPart(const vec2& _size,
                                         const vec2& _sourcePosStart,
                                         const vec2& _sourcePosStop) {
	if (m_angle == 0.0f) {
		vec3 point = m_position;
		vec2 tex(_sourcePosStart.x(),_sourcePosStop.y());
		
		m_coord.push_back(point);
		m_coordTex.push_back(tex);
		m_coordColor.push_back(m_color);
		
		
		tex.setValue(_sourcePosStop.x(),_sourcePosStop.y());
		point.setX(m_position.x() + _size.x());
		point.setY(m_position.y());
		m_coord.push_back(point);
		m_coordTex.push_back(tex);
		m_coordColor.push_back(m_color);
		
		
		tex.setValue(_sourcePosStop.x(),_sourcePosStart.y());
		point.setX(m_position.x() + _size.x());
		point.setY(m_position.y() + _size.y());
		m_coord.push_back(point);
		m_coordTex.push_back(tex);
		m_coordColor.push_back(m_color);
		
		m_coord.push_back(point);
		m_coordTex.push_back(tex);
		m_coordColor.push_back(m_color);
		
		tex.setValue(_sourcePosStart.x(),_sourcePosStart.y());
		point.setX(m_position.x());
		point.setY(m_position.y() + _size.y());
		m_coord.push_back(point);
		m_coordTex.push_back(tex);
		m_coordColor.push_back(m_color);
		
		tex.setValue(_sourcePosStart.x(),_sourcePosStop.y());
		point.setX(m_position.x());
		point.setY(m_position.y());
		m_coord.push_back(point);
		m_coordTex.push_back(tex);
		m_coordColor.push_back(m_color);
		return;
	}
	vec3 center = m_position + vec3(_size.x(),_size.y(),0)/2.0f;
	vec3 limitedSize(_size.x()*0.5f, _size.y()*0.5f, 0.0f);
	
	vec3 point(0,0,0);
	vec2 tex(_sourcePosStart.x(),_sourcePosStop.y());
	
	point.setValue(-limitedSize.x(), -limitedSize.y(), 0);
	point = point.rotate(vec3(0,0,1), m_angle) + center;
	m_coord.push_back(point);
	m_coordTex.push_back(tex);
	m_coordColor.push_back(m_color);
	
	
	tex.setValue(_sourcePosStop.x(),_sourcePosStop.y());
	point.setValue(limitedSize.x(), -limitedSize.y(), 0);
	point = point.rotate(vec3(0,0,1), m_angle) + center;
	m_coord.push_back(point);
	m_coordTex.push_back(tex);
	m_coordColor.push_back(m_color);
	
	
	tex.setValue(_sourcePosStop.x(),_sourcePosStart.y());
	point.setValue(limitedSize.x(), limitedSize.y(), 0);
	point = point.rotate(vec3(0,0,1), m_angle) + center;
	m_coord.push_back(point);
	m_coordTex.push_back(tex);
	m_coordColor.push_back(m_color);
	
	m_coord.push_back(point);
	m_coordTex.push_back(tex);
	m_coordColor.push_back(m_color);
	
	tex.setValue(_sourcePosStart.x(),_sourcePosStart.y());
	point.setValue(-limitedSize.x(), limitedSize.y(), 0);
	point = point.rotate(vec3(0,0,1), m_angle) + center;
	m_coord.push_back(point);
	m_coordTex.push_back(tex);
	m_coordColor.push_back(m_color);
	
	tex.setValue(_sourcePosStart.x(),_sourcePosStop.y());
	point.setValue(-limitedSize.x(), -limitedSize.y(), 0);
	point = point.rotate(vec3(0,0,1), m_angle) + center;
	m_coord.push_back(point);
	m_coordTex.push_back(tex);
	m_coordColor.push_back(m_color);
}

void ewol::compositing::Image::setSource(const std::string& _newFile, const vec2& _size) {
	clear();
	ewol::resource::TextureFile* resource = m_resource;
	ewol::resource::ImageDF* resourceDF = m_resourceDF;
	m_filename = _newFile;
	m_requestSize = _size;
	m_resource = nullptr;
	m_resourceDF = nullptr;
	ivec2 tmpSize(_size.x(),_size.y());
	// note that no image can be loaded...
	if (_newFile != "") {
		// link to new one
		if (m_distanceFieldMode == false) {
			m_resource = ewol::resource::TextureFile::keep(m_filename, tmpSize);
			if (nullptr == m_resource) {
				EWOL_ERROR("Can not get Image resource");
			}
		} else {
			m_resourceDF = ewol::resource::ImageDF::keep(m_filename, tmpSize);
			if (nullptr == m_resourceDF) {
				EWOL_ERROR("Can not get Image resource DF");
			}
		}
	}
	if (    m_resource == nullptr
	     && m_resourceDF == nullptr) {
		if (resource != nullptr) {
			EWOL_WARNING("Retrive previous resource");
			m_resource = resource;
		}
		if (resourceDF != nullptr) {
			EWOL_WARNING("Retrive previous resource (DF)");
			m_resourceDF = resourceDF;
		}
	} else {
		ewol::resource::TextureFile::release(resource);
		ewol::resource::ImageDF::release(resourceDF);
	}
}

bool ewol::compositing::Image::hasSources() {
	return (m_resource != nullptr || m_resourceDF != NULL);
}


vec2 ewol::compositing::Image::getRealSize() {
	if (    m_resource == nullptr
	     && m_resourceDF == nullptr) {
		return vec2(0,0);
	}
	if (m_resource != nullptr) {
		return m_resource->getRealSize();
	}
	if (m_resourceDF != nullptr) {
		return m_resourceDF->getRealSize();
	}
	return vec2(0,0);
}



void ewol::compositing::Image::setDistanceFieldMode(bool _mode) {
	if (m_distanceFieldMode == _mode) {
		return;
	}
	m_distanceFieldMode = _mode;
	// Force reload input
	setSource(m_filename, m_requestSize);
	loadProgram();
}
