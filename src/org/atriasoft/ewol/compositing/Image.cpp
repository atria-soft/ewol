/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/debug.hpp>
#include <ewol/compositing/Image.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::compositing::Image);

 int ewol::compositing::Image::sizeAuto(0);

// VBO table property:
 int ewol::compositing::Image::this.vboIdCoord(0);
 int ewol::compositing::Image::this.vboIdCoordTex(1);
 int ewol::compositing::Image::this.vboIdColor(2);
#define NB_VBO (3)

ewol::compositing::Image::Image( etk::Uri _imageName,
                                boolean _df,
                                int _size) :
  this.filename(_imageName),
  this.requestSize(2,2),
  this.position(0.0, 0.0, 0.0),
  this.clippingPosStart(0.0, 0.0, 0.0),
  this.clippingPosStop(0.0, 0.0, 0.0),
  this.clippingEnable(false),
  this.color(etk::color::white),
  this.angle(0.0),
  this.GLprogram(null),
  this.GLPosition(-1),
  this.GLMatrix(-1),
  this.GLColor(-1),
  this.GLtexture(-1),
  this.GLtexID(-1),
  this.distanceFieldMode(_df),
  this.resource(null),
  this.resourceDF(null) {
	// Create the VBO:
	this.VBO = gale::resource::VirtualBufferObject::create(NB_VBO);
	if (this.VBO == null) {
		Log.error("can not instanciate VBO ...");
		return;
	}
	// TO facilitate some debugs we add a name of the VBO:
	this.VBO.setName("[VBO] of ewol::compositing::Image");
	setSource(_imageName, _size);
	loadProgram();
}

ewol::compositing::Image::~Image() {
	
}

void ewol::compositing::Image::loadProgram() {
	// get the shader resource:
	this.GLPosition = 0;
	this.GLprogram.reset();
	if (this.distanceFieldMode == true) {
		this.GLprogram = gale::resource::Program::create("DATA:///texturedDF.prog?lib=ewol");
	} else {
		this.GLprogram = gale::resource::Program::create("DATA:///textured3D.prog?lib=ewol");
	}
	if (this.GLprogram != null) {
		this.GLPosition = this.GLprogram.getAttribute("EW_coord3d");
		this.GLColor    = this.GLprogram.getAttribute("EW_color");
		this.GLtexture  = this.GLprogram.getAttribute("EW_texture2d");
		this.GLMatrix   = this.GLprogram.getUniform("EW_MatrixTransformation");
		this.GLtexID    = this.GLprogram.getUniform("EW_texID");
	}
}

void ewol::compositing::Image::draw(boolean _disableDepthTest) {
	if (this.VBO.bufferSize(this.vboIdCoord) <= 0) {
		//Log.warning("Nothink to draw...");
		return;
	}
	if (    this.resource == null
	     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.resourceDF == null
	     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.resourceImage == null) {
		// this is a normale case ... the user can choice to have no image ...
		return;
	}
	if (this.GLprogram == null) {
		Log.error("No shader ...");
		return;
	}
	//Log.warning("Display image : " + this.VBO.bufferSize(this.vboIdCoord));
	if (_disableDepthTest == true) {
		gale::openGL::disable(gale::openGL::flag_depthTest);
	} else {
		gale::openGL::enable(gale::openGL::flag_depthTest);
	}
	// set Matrix : translation/positionMatrix
	mat4 tmpMatrix = gale::openGL::getMatrix()*this.matrixApply;
	this.GLprogram.use();
	this.GLprogram.uniformMatrix(this.GLMatrix, tmpMatrix);
	// TextureID
	if (this.resourceImage != null) {
		this.GLprogram.setTexture0(this.GLtexID, this.resourceImage.getRendererId());
	} else if (this.resource != null) {
		if (this.distanceFieldMode == true) {
			Log.error("FONT type error Request distance field and display normal ...");
		}
		this.GLprogram.setTexture0(this.GLtexID, this.resource.getRendererId());
	} else {
		if (this.distanceFieldMode == false) {
			Log.error("FONT type error Request normal and display distance field ...");
		}
		this.GLprogram.setTexture0(this.GLtexID, this.resourceDF.getRendererId());
	}
	// position:
	this.GLprogram.sendAttributePointer(this.GLPosition, this.VBO, this.vboIdCoord);
	// Texture:
	this.GLprogram.sendAttributePointer(this.GLtexture, this.VBO, this.vboIdCoordTex);
	// color:
	this.GLprogram.sendAttributePointer(this.GLColor, this.VBO, this.vboIdColor);
	// Request the draw of the elements:
	gale::openGL::drawArrays(gale::openGL::renderMode::triangle, 0, this.VBO.bufferSize(this.vboIdCoord));
	this.GLprogram.unUse();
}

void ewol::compositing::Image::clear() {
	// call upper class
	ewol::Compositing::clear();
	// reset Buffer :
	this.VBO.clear();
	// reset temporal variables :
	this.position = Vector3f(0.0, 0.0, 0.0);
	this.clippingPosStart = Vector3f(0.0, 0.0, 0.0);
	this.clippingPosStop = Vector3f(0.0, 0.0, 0.0);
	this.clippingEnable = false;
	this.color = etk::color::white;
	this.angle = 0.0;
}

void ewol::compositing::Image::setClipping( Vector3f _pos, Vector3f _posEnd) {
	// note the internal system all time request to have a bounding all time in the same order
	if (_pos.x() <= _posEnd.x()) {
		this.clippingPosStart.setX(_pos.x());
		this.clippingPosStop.setX(_posEnd.x());
	} else {
		this.clippingPosStart.setX(_posEnd.x());
		this.clippingPosStop.setX(_pos.x());
	}
	if (_pos.y() <= _posEnd.y()) {
		this.clippingPosStart.setY(_pos.y());
		this.clippingPosStop.setY(_posEnd.y());
	} else {
		this.clippingPosStart.setY(_posEnd.y());
		this.clippingPosStop.setY(_pos.y());
	}
	if (_pos.z() <= _posEnd.z()) {
		this.clippingPosStart.setZ(_pos.z());
		this.clippingPosStop.setZ(_posEnd.z());
	} else {
		this.clippingPosStart.setZ(_posEnd.z());
		this.clippingPosStop.setZ(_pos.z());
	}
	this.clippingEnable = true;
}

void ewol::compositing::Image::setAngle(float _angle) {
	this.angle = _angle;
}

void ewol::compositing::Image::print( Vector2f _size) {
	printPart(_size, Vector2f(0,0), Vector2f(1.0,1.0));
}

void ewol::compositing::Image::printPart( Vector2f _size,
                                         Vector2f _sourcePosStart,
                                         Vector2f _sourcePosStop) {
	if (this.resource == null) {
		return;
	}
	Vector2f openGLSize = Vector2f(this.resource.getOpenGlSize().x(), this.resource.getOpenGlSize().y());
	Vector2f usefullSize = this.resource.getUsableSize();
	Vector2f ratio = usefullSize/openGLSize;
	_sourcePosStart *= ratio;
	_sourcePosStop *= ratio;
	Log.verbose("     openGLSize=" + openGLSize + " usableSize=" + usefullSize + " start=" + _sourcePosStart + " stop=" + _sourcePosStop);
	
	//Log.error("Debug image " + this.filename + "  ==> " + this.position + " " + _size + " " + _sourcePosStart + " " << _sourcePosStop);
	if (this.angle == 0.0f) {
		Vector3f point = this.position;
		Vector2f tex(_sourcePosStart.x(),_sourcePosStop.y());
		this.VBO.pushOnBuffer(this.vboIdCoord, point);
		this.VBO.pushOnBuffer(this.vboIdCoordTex, tex);
		this.VBO.pushOnBuffer(this.vboIdColor, this.color);
		
		tex.setValue(_sourcePosStop.x(),_sourcePosStop.y());
		point.setX(this.position.x() + _size.x());
		point.setY(this.position.y());
		this.VBO.pushOnBuffer(this.vboIdCoord, point);
		this.VBO.pushOnBuffer(this.vboIdCoordTex, tex);
		this.VBO.pushOnBuffer(this.vboIdColor, this.color);
		
		tex.setValue(_sourcePosStop.x(),_sourcePosStart.y());
		point.setX(this.position.x() + _size.x());
		point.setY(this.position.y() + _size.y());
		this.VBO.pushOnBuffer(this.vboIdCoord, point);
		this.VBO.pushOnBuffer(this.vboIdCoordTex, tex);
		this.VBO.pushOnBuffer(this.vboIdColor, this.color);
		
		this.VBO.pushOnBuffer(this.vboIdCoord, point);
		this.VBO.pushOnBuffer(this.vboIdCoordTex, tex);
		this.VBO.pushOnBuffer(this.vboIdColor, this.color);
		
		tex.setValue(_sourcePosStart.x(),_sourcePosStart.y());
		point.setX(this.position.x());
		point.setY(this.position.y() + _size.y());
		this.VBO.pushOnBuffer(this.vboIdCoord, point);
		this.VBO.pushOnBuffer(this.vboIdCoordTex, tex);
		this.VBO.pushOnBuffer(this.vboIdColor, this.color);
		
		tex.setValue(_sourcePosStart.x(),_sourcePosStop.y());
		point.setX(this.position.x());
		point.setY(this.position.y());
		this.VBO.pushOnBuffer(this.vboIdCoord, point);
		this.VBO.pushOnBuffer(this.vboIdCoordTex, tex);
		this.VBO.pushOnBuffer(this.vboIdColor, this.color);
		this.VBO.flush();
		return;
	}
	Vector3f center = this.position + Vector3f(_size.x(),_size.y(),0)/2.0f;
	Vector3f limitedSize(_size.x()*0.5f, _size.y()*0.5f, 0.0f);
	
	Vector3f point(0,0,0);
	Vector2f tex(_sourcePosStart.x(),_sourcePosStop.y());
	
	point.setValue(-limitedSize.x(), -limitedSize.y(), 0);
	point = point.rotate(Vector3f(0,0,1), this.angle) + center;
	this.VBO.pushOnBuffer(this.vboIdCoord, point);
	this.VBO.pushOnBuffer(this.vboIdCoordTex, tex);
	this.VBO.pushOnBuffer(this.vboIdColor, this.color);
	
	tex.setValue(_sourcePosStop.x(),_sourcePosStop.y());
	point.setValue(limitedSize.x(), -limitedSize.y(), 0);
	point = point.rotate(Vector3f(0,0,1), this.angle) + center;
	this.VBO.pushOnBuffer(this.vboIdCoord, point);
	this.VBO.pushOnBuffer(this.vboIdCoordTex, tex);
	this.VBO.pushOnBuffer(this.vboIdColor, this.color);
	
	tex.setValue(_sourcePosStop.x(),_sourcePosStart.y());
	point.setValue(limitedSize.x(), limitedSize.y(), 0);
	point = point.rotate(Vector3f(0,0,1), this.angle) + center;
	this.VBO.pushOnBuffer(this.vboIdCoord, point);
	this.VBO.pushOnBuffer(this.vboIdCoordTex, tex);
	this.VBO.pushOnBuffer(this.vboIdColor, this.color);
	
	this.VBO.pushOnBuffer(this.vboIdCoord, point);
	this.VBO.pushOnBuffer(this.vboIdCoordTex, tex);
	this.VBO.pushOnBuffer(this.vboIdColor, this.color);
	
	tex.setValue(_sourcePosStart.x(),_sourcePosStart.y());
	point.setValue(-limitedSize.x(), limitedSize.y(), 0);
	point = point.rotate(Vector3f(0,0,1), this.angle) + center;
	this.VBO.pushOnBuffer(this.vboIdCoord, point);
	this.VBO.pushOnBuffer(this.vboIdCoordTex, tex);
	this.VBO.pushOnBuffer(this.vboIdColor, this.color);
	
	tex.setValue(_sourcePosStart.x(),_sourcePosStop.y());
	point.setValue(-limitedSize.x(), -limitedSize.y(), 0);
	point = point.rotate(Vector3f(0,0,1), this.angle) + center;
	this.VBO.pushOnBuffer(this.vboIdCoord, point);
	this.VBO.pushOnBuffer(this.vboIdCoordTex, tex);
	this.VBO.pushOnBuffer(this.vboIdColor, this.color);
	
	this.VBO.flush();
}

void ewol::compositing::Image::setSource( etk::Uri _uri,  Vector2f _size) {
	clear();
	if (    this.filename == _uri
	     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.requestSize == _size) {
		// Nothing to do ...
		return;
	}
	ememory::Ptr<ewol::resource::TextureFile> resource = this.resource;
	ememory::Ptr<ewol::resource::ImageDF> resourceDF = this.resourceDF;
	ememory::Ptr<ewol::resource::Texture> resourceTex = this.resourceImage;
	this.filename = _uri;
	this.requestSize = _size;
	this.resource.reset();
	this.resourceDF.reset();
	this.resourceImage.reset();
	Vector2i tmpSize(_size.x(),_size.y());
	// note that no image can be loaded...
	if (_uri.isEmpty() == false) {
		// link to new one
		if (this.distanceFieldMode == false) {
			this.resource = ewol::resource::TextureFile::create(this.filename, tmpSize);
			if (this.resource == null) {
				Log.error("Can not get Image resource");
			}
		} else {
			this.resourceDF = ewol::resource::ImageDF::create(this.filename, tmpSize);
			if (this.resourceDF == null) {
				Log.error("Can not get Image resource DF");
			}
		}
	}
	if (    this.resource == null
	     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.resourceDF == null
	     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.resourceImage == null) {
		if (resource != null) {
			Log.warning("Retrive previous resource");
			this.resource = resource;
		}
		if (resourceDF != null) {
			Log.warning("Retrive previous resource (DF)");
			this.resourceDF = resourceDF;
		}
		if (resourceTex != null) {
			Log.warning("Retrive previous resource (image)");
			this.resourceImage = resourceTex;
		}
	}
}
void ewol::compositing::Image::setSource(egami::Image _image) {
	clear();
	this.filename = "direct image BUFFER";
	this.requestSize = _image.getSize();
	this.resourceImage = ewol::resource::Texture::create();
	this.resourceImage.set(etk::move(_image));
}

boolean ewol::compositing::Image::hasSources() {
	return    this.resource != null
	       || this.resourceDF != null;
}


Vector2f ewol::compositing::Image::getRealSize() {
	if (    this.resource == null
	     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.resourceDF == null
	     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.resourceImage == null) {
		return Vector2f(0,0);
	}
	if (this.resource != null) {
		return this.resource.getRealSize();
	}
	if (this.resourceDF != null) {
		return this.resourceDF.getRealSize();
	}
	if (this.resourceImage != null) {
		return this.resourceImage.getUsableSize();
	}
	return Vector2f(0,0);
}



void ewol::compositing::Image::setDistanceFieldMode(boolean _mode) {
	if (this.distanceFieldMode == _mode) {
		return;
	}
	this.distanceFieldMode = _mode;
	// Force reload input
	setSource(this.filename, this.requestSize);
	loadProgram();
}
