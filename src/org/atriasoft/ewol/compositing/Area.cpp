/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/debug.hpp>
#include <ewol/compositing/Area.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::compositing::Area);

// VBO table property:
 int ewol::compositing::Area::this.vboIdCoord(0);
 int ewol::compositing::Area::this.vboIdCoordText(1);
 int ewol::compositing::Area::this.vboIdColor(2);
#define NB_VBO (3)

ewol::compositing::Area::Area( Vector2i _size) :
  this.position(0.0, 0.0, 0.0),
  this.color(etk::color::white),
  this.GLprogram(null),
  this.GLPosition(-1),
  this.GLMatrix(-1),
  this.GLColor(-1),
  this.GLtexture(-1),
  this.GLtexID(-1),
  this.resource(null) {
	this.resource = ewol::resource::Texture::create();
	this.resource.setImageSize(_size);
	this.resource.flush();
	// Create the VBO:
	this.VBO = gale::resource::VirtualBufferObject::create(NB_VBO);
	if (this.VBO == null) {
		Log.error("can not instanciate VBO ...");
		return;
	}
	// TO facilitate some debugs we add a name of the VBO:
	this.VBO.setName("[VBO] of ewol::compositing::Area");
	loadProgram();
}

ewol::compositing::Area::~Area() {
	
}

void ewol::compositing::Area::loadProgram() {
	// get the shader resource :
	this.GLPosition = 0;
	this.GLprogram = gale::resource::Program::create(String("DATA:///textured3D.prog?lib=ewol"));
	if (this.GLprogram != null) {
		this.GLPosition = this.GLprogram.getAttribute("EW_coord3d");
		this.GLColor    = this.GLprogram.getAttribute("EW_color");
		this.GLtexture  = this.GLprogram.getAttribute("EW_texture2d");
		this.GLMatrix   = this.GLprogram.getUniform("EW_MatrixTransformation");
		this.GLtexID    = this.GLprogram.getUniform("EW_texID");
	}
}

void ewol::compositing::Area::draw(boolean _disableDepthTest) {
	if (this.VBO.bufferSize(this.vboIdCoord) <= 0) {
		//Log.warning("Nothink to draw...");
		return;
	}
	if (this.resource == null) {
		// this is a normale case ... the user can choice to have no image ...
		return;
	}
	if (this.GLprogram == null) {
		Log.error("No shader ...");
		return;
	}
	// set Matrix : translation/positionMatrix
	mat4 tmpMatrix = gale::openGL::getMatrix()*this.matrixApply;
	this.GLprogram.use(); 
	this.GLprogram.uniformMatrix(this.GLMatrix, tmpMatrix);
	// TextureID
	this.GLprogram.setTexture0(this.GLtexID, this.resource.getRendererId());
	// position:
	this.GLprogram.sendAttributePointer(this.GLPosition, this.VBO, this.vboIdCoord);
	// Texture:
	this.GLprogram.sendAttributePointer(this.GLtexture, this.VBO, this.vboIdColor);
	// color:
	this.GLprogram.sendAttributePointer(this.GLColor, this.VBO, this.vboIdCoordText);
	// Request the draw od the elements : 
	gale::openGL::drawArrays(gale::openGL::renderMode::triangle, 0, this.VBO.bufferSize(this.vboIdCoord));
	this.GLprogram.unUse();
}

void ewol::compositing::Area::clear() {
	// call upper class
	ewol::Compositing::clear();
	// reset all VBOs:
	this.VBO.clear();
	// reset temporal variables :
	this.position = Vector3f(0.0, 0.0, 0.0);
}

void ewol::compositing::Area::print( Vector2i _size) {
	Vector3f point(0,0,0);
	Vector2f tex(0,1);
	point.setX(this.position.x());
	point.setY(this.position.y());
	this.VBO.pushOnBuffer(this.vboIdCoord, point);
	this.VBO.pushOnBuffer(this.vboIdColor, this.color);
	this.VBO.pushOnBuffer(this.vboIdCoordText, tex);
	
	tex.setValue(1,1);
	point.setX(this.position.x() + _size.x());
	point.setY(this.position.y());
	this.VBO.pushOnBuffer(this.vboIdCoord, point);
	this.VBO.pushOnBuffer(this.vboIdColor, this.color);
	this.VBO.pushOnBuffer(this.vboIdCoordText, tex);
	
	tex.setValue(1,0);
	point.setX(this.position.x() + _size.x());
	point.setY(this.position.y() + _size.y());
	this.VBO.pushOnBuffer(this.vboIdCoord, point);
	this.VBO.pushOnBuffer(this.vboIdColor, this.color);
	this.VBO.pushOnBuffer(this.vboIdCoordText, tex);
	
	this.VBO.pushOnBuffer(this.vboIdCoord, point);
	this.VBO.pushOnBuffer(this.vboIdColor, this.color);
	this.VBO.pushOnBuffer(this.vboIdCoordText, tex);
	
	tex.setValue(0,0);
	point.setX(this.position.x());
	point.setY(this.position.y() + _size.y());
	this.VBO.pushOnBuffer(this.vboIdCoord, point);
	this.VBO.pushOnBuffer(this.vboIdColor, this.color);
	this.VBO.pushOnBuffer(this.vboIdCoordText, tex);
	
	tex.setValue(0,1);
	point.setX(this.position.x());
	point.setY(this.position.y());
	this.VBO.pushOnBuffer(this.vboIdCoord, point);
	this.VBO.pushOnBuffer(this.vboIdColor, this.color);
	this.VBO.pushOnBuffer(this.vboIdCoordText, tex);
	
	this.VBO.flush();
}


