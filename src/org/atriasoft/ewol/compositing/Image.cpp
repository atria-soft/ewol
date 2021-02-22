/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/debug.hpp>
#include <ewol/compositing/Image.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::compositing::Image);

const int32_t ewol::compositing::Image::sizeAuto(0);

// VBO table property:
const int32_t ewol::compositing::Image::m_vboIdCoord(0);
const int32_t ewol::compositing::Image::m_vboIdCoordTex(1);
const int32_t ewol::compositing::Image::m_vboIdColor(2);
#define NB_VBO (3)

ewol::compositing::Image::Image(const etk::Uri& _imageName,
                                bool _df,
                                int32_t _size) :
  m_filename(_imageName),
  m_requestSize(2,2),
  m_position(0.0, 0.0, 0.0),
  m_clippingPosStart(0.0, 0.0, 0.0),
  m_clippingPosStop(0.0, 0.0, 0.0),
  m_clippingEnable(false),
  m_color(etk::color::white),
  m_angle(0.0),
  m_GLprogram(null),
  m_GLPosition(-1),
  m_GLMatrix(-1),
  m_GLColor(-1),
  m_GLtexture(-1),
  m_GLtexID(-1),
  m_distanceFieldMode(_df),
  m_resource(null),
  m_resourceDF(null) {
	// Create the VBO:
	m_VBO = gale::resource::VirtualBufferObject::create(NB_VBO);
	if (m_VBO == null) {
		Log.error("can not instanciate VBO ...");
		return;
	}
	// TO facilitate some debugs we add a name of the VBO:
	m_VBO->setName("[VBO] of ewol::compositing::Image");
	setSource(_imageName, _size);
	loadProgram();
}

ewol::compositing::Image::~Image() {
	
}

void ewol::compositing::Image::loadProgram() {
	// get the shader resource:
	m_GLPosition = 0;
	m_GLprogram.reset();
	if (m_distanceFieldMode == true) {
		m_GLprogram = gale::resource::Program::create("DATA:///texturedDF.prog?lib=ewol");
	} else {
		m_GLprogram = gale::resource::Program::create("DATA:///textured3D.prog?lib=ewol");
	}
	if (m_GLprogram != null) {
		m_GLPosition = m_GLprogram->getAttribute("EW_coord3d");
		m_GLColor    = m_GLprogram->getAttribute("EW_color");
		m_GLtexture  = m_GLprogram->getAttribute("EW_texture2d");
		m_GLMatrix   = m_GLprogram->getUniform("EW_MatrixTransformation");
		m_GLtexID    = m_GLprogram->getUniform("EW_texID");
	}
}

void ewol::compositing::Image::draw(bool _disableDepthTest) {
	if (m_VBO->bufferSize(m_vboIdCoord) <= 0) {
		//EWOL_WARNING("Nothink to draw...");
		return;
	}
	if (    m_resource == null
	     && m_resourceDF == null
	     && m_resourceImage == null) {
		// this is a normale case ... the user can choice to have no image ...
		return;
	}
	if (m_GLprogram == null) {
		Log.error("No shader ...");
		return;
	}
	//EWOL_WARNING("Display image : " << m_VBO->bufferSize(m_vboIdCoord));
	if (_disableDepthTest == true) {
		gale::openGL::disable(gale::openGL::flag_depthTest);
	} else {
		gale::openGL::enable(gale::openGL::flag_depthTest);
	}
	// set Matrix : translation/positionMatrix
	mat4 tmpMatrix = gale::openGL::getMatrix()*m_matrixApply;
	m_GLprogram->use();
	m_GLprogram->uniformMatrix(m_GLMatrix, tmpMatrix);
	// TextureID
	if (m_resourceImage != null) {
		m_GLprogram->setTexture0(m_GLtexID, m_resourceImage->getRendererId());
	} else if (m_resource != null) {
		if (m_distanceFieldMode == true) {
			Log.error("FONT type error Request distance field and display normal ...");
		}
		m_GLprogram->setTexture0(m_GLtexID, m_resource->getRendererId());
	} else {
		if (m_distanceFieldMode == false) {
			Log.error("FONT type error Request normal and display distance field ...");
		}
		m_GLprogram->setTexture0(m_GLtexID, m_resourceDF->getRendererId());
	}
	// position:
	m_GLprogram->sendAttributePointer(m_GLPosition, m_VBO, m_vboIdCoord);
	// Texture:
	m_GLprogram->sendAttributePointer(m_GLtexture, m_VBO, m_vboIdCoordTex);
	// color:
	m_GLprogram->sendAttributePointer(m_GLColor, m_VBO, m_vboIdColor);
	// Request the draw of the elements:
	gale::openGL::drawArrays(gale::openGL::renderMode::triangle, 0, m_VBO->bufferSize(m_vboIdCoord));
	m_GLprogram->unUse();
}

void ewol::compositing::Image::clear() {
	// call upper class
	ewol::Compositing::clear();
	// reset Buffer :
	m_VBO->clear();
	// reset temporal variables :
	m_position = Vector3f(0.0, 0.0, 0.0);
	m_clippingPosStart = Vector3f(0.0, 0.0, 0.0);
	m_clippingPosStop = Vector3f(0.0, 0.0, 0.0);
	m_clippingEnable = false;
	m_color = etk::color::white;
	m_angle = 0.0;
}

void ewol::compositing::Image::setClipping(const Vector3f& _pos, Vector3f _posEnd) {
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

void ewol::compositing::Image::print(const Vector2f& _size) {
	printPart(_size, Vector2f(0,0), Vector2f(1.0,1.0));
}

void ewol::compositing::Image::printPart(const Vector2f& _size,
                                         Vector2f _sourcePosStart,
                                         Vector2f _sourcePosStop) {
	if (m_resource == null) {
		return;
	}
	Vector2f openGLSize = Vector2f(m_resource->getOpenGlSize().x(), m_resource->getOpenGlSize().y());
	Vector2f usefullSize = m_resource->getUsableSize();
	Vector2f ratio = usefullSize/openGLSize;
	_sourcePosStart *= ratio;
	_sourcePosStop *= ratio;
	Log.verbose("     openGLSize=" << openGLSize << " usableSize=" << usefullSize << " start=" << _sourcePosStart << " stop=" << _sourcePosStop);
	
	//Log.error("Debug image " << m_filename << "  ==> " << m_position << " " << _size << " " << _sourcePosStart << " " << _sourcePosStop);
	if (m_angle == 0.0f) {
		Vector3f point = m_position;
		Vector2f tex(_sourcePosStart.x(),_sourcePosStop.y());
		m_VBO->pushOnBuffer(m_vboIdCoord, point);
		m_VBO->pushOnBuffer(m_vboIdCoordTex, tex);
		m_VBO->pushOnBuffer(m_vboIdColor, m_color);
		
		tex.setValue(_sourcePosStop.x(),_sourcePosStop.y());
		point.setX(m_position.x() + _size.x());
		point.setY(m_position.y());
		m_VBO->pushOnBuffer(m_vboIdCoord, point);
		m_VBO->pushOnBuffer(m_vboIdCoordTex, tex);
		m_VBO->pushOnBuffer(m_vboIdColor, m_color);
		
		tex.setValue(_sourcePosStop.x(),_sourcePosStart.y());
		point.setX(m_position.x() + _size.x());
		point.setY(m_position.y() + _size.y());
		m_VBO->pushOnBuffer(m_vboIdCoord, point);
		m_VBO->pushOnBuffer(m_vboIdCoordTex, tex);
		m_VBO->pushOnBuffer(m_vboIdColor, m_color);
		
		m_VBO->pushOnBuffer(m_vboIdCoord, point);
		m_VBO->pushOnBuffer(m_vboIdCoordTex, tex);
		m_VBO->pushOnBuffer(m_vboIdColor, m_color);
		
		tex.setValue(_sourcePosStart.x(),_sourcePosStart.y());
		point.setX(m_position.x());
		point.setY(m_position.y() + _size.y());
		m_VBO->pushOnBuffer(m_vboIdCoord, point);
		m_VBO->pushOnBuffer(m_vboIdCoordTex, tex);
		m_VBO->pushOnBuffer(m_vboIdColor, m_color);
		
		tex.setValue(_sourcePosStart.x(),_sourcePosStop.y());
		point.setX(m_position.x());
		point.setY(m_position.y());
		m_VBO->pushOnBuffer(m_vboIdCoord, point);
		m_VBO->pushOnBuffer(m_vboIdCoordTex, tex);
		m_VBO->pushOnBuffer(m_vboIdColor, m_color);
		m_VBO->flush();
		return;
	}
	Vector3f center = m_position + Vector3f(_size.x(),_size.y(),0)/2.0f;
	Vector3f limitedSize(_size.x()*0.5f, _size.y()*0.5f, 0.0f);
	
	Vector3f point(0,0,0);
	Vector2f tex(_sourcePosStart.x(),_sourcePosStop.y());
	
	point.setValue(-limitedSize.x(), -limitedSize.y(), 0);
	point = point.rotate(Vector3f(0,0,1), m_angle) + center;
	m_VBO->pushOnBuffer(m_vboIdCoord, point);
	m_VBO->pushOnBuffer(m_vboIdCoordTex, tex);
	m_VBO->pushOnBuffer(m_vboIdColor, m_color);
	
	tex.setValue(_sourcePosStop.x(),_sourcePosStop.y());
	point.setValue(limitedSize.x(), -limitedSize.y(), 0);
	point = point.rotate(Vector3f(0,0,1), m_angle) + center;
	m_VBO->pushOnBuffer(m_vboIdCoord, point);
	m_VBO->pushOnBuffer(m_vboIdCoordTex, tex);
	m_VBO->pushOnBuffer(m_vboIdColor, m_color);
	
	tex.setValue(_sourcePosStop.x(),_sourcePosStart.y());
	point.setValue(limitedSize.x(), limitedSize.y(), 0);
	point = point.rotate(Vector3f(0,0,1), m_angle) + center;
	m_VBO->pushOnBuffer(m_vboIdCoord, point);
	m_VBO->pushOnBuffer(m_vboIdCoordTex, tex);
	m_VBO->pushOnBuffer(m_vboIdColor, m_color);
	
	m_VBO->pushOnBuffer(m_vboIdCoord, point);
	m_VBO->pushOnBuffer(m_vboIdCoordTex, tex);
	m_VBO->pushOnBuffer(m_vboIdColor, m_color);
	
	tex.setValue(_sourcePosStart.x(),_sourcePosStart.y());
	point.setValue(-limitedSize.x(), limitedSize.y(), 0);
	point = point.rotate(Vector3f(0,0,1), m_angle) + center;
	m_VBO->pushOnBuffer(m_vboIdCoord, point);
	m_VBO->pushOnBuffer(m_vboIdCoordTex, tex);
	m_VBO->pushOnBuffer(m_vboIdColor, m_color);
	
	tex.setValue(_sourcePosStart.x(),_sourcePosStop.y());
	point.setValue(-limitedSize.x(), -limitedSize.y(), 0);
	point = point.rotate(Vector3f(0,0,1), m_angle) + center;
	m_VBO->pushOnBuffer(m_vboIdCoord, point);
	m_VBO->pushOnBuffer(m_vboIdCoordTex, tex);
	m_VBO->pushOnBuffer(m_vboIdColor, m_color);
	
	m_VBO->flush();
}

void ewol::compositing::Image::setSource(const etk::Uri& _uri, const Vector2f& _size) {
	clear();
	if (    m_filename == _uri
	     && m_requestSize == _size) {
		// Nothing to do ...
		return;
	}
	ememory::SharedPtr<ewol::resource::TextureFile> resource = m_resource;
	ememory::SharedPtr<ewol::resource::ImageDF> resourceDF = m_resourceDF;
	ememory::SharedPtr<ewol::resource::Texture> resourceTex = m_resourceImage;
	m_filename = _uri;
	m_requestSize = _size;
	m_resource.reset();
	m_resourceDF.reset();
	m_resourceImage.reset();
	Vector2i tmpSize(_size.x(),_size.y());
	// note that no image can be loaded...
	if (_uri.isEmpty() == false) {
		// link to new one
		if (m_distanceFieldMode == false) {
			m_resource = ewol::resource::TextureFile::create(m_filename, tmpSize);
			if (m_resource == null) {
				Log.error("Can not get Image resource");
			}
		} else {
			m_resourceDF = ewol::resource::ImageDF::create(m_filename, tmpSize);
			if (m_resourceDF == null) {
				Log.error("Can not get Image resource DF");
			}
		}
	}
	if (    m_resource == null
	     && m_resourceDF == null
	     && m_resourceImage == null) {
		if (resource != null) {
			EWOL_WARNING("Retrive previous resource");
			m_resource = resource;
		}
		if (resourceDF != null) {
			EWOL_WARNING("Retrive previous resource (DF)");
			m_resourceDF = resourceDF;
		}
		if (resourceTex != null) {
			EWOL_WARNING("Retrive previous resource (image)");
			m_resourceImage = resourceTex;
		}
	}
}
void ewol::compositing::Image::setSource(egami::Image _image) {
	clear();
	m_filename = "direct image BUFFER";
	m_requestSize = _image.getSize();
	m_resourceImage = ewol::resource::Texture::create();
	m_resourceImage->set(etk::move(_image));
}

bool ewol::compositing::Image::hasSources() {
	return    m_resource != null
	       || m_resourceDF != null;
}


Vector2f ewol::compositing::Image::getRealSize() {
	if (    m_resource == null
	     && m_resourceDF == null
	     && m_resourceImage == null) {
		return Vector2f(0,0);
	}
	if (m_resource != null) {
		return m_resource->getRealSize();
	}
	if (m_resourceDF != null) {
		return m_resourceDF->getRealSize();
	}
	if (m_resourceImage != null) {
		return m_resourceImage->getUsableSize();
	}
	return Vector2f(0,0);
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
