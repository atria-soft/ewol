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
const int32_t ewol::compositing::Area::m_vboIdCoord(0);
const int32_t ewol::compositing::Area::m_vboIdCoordText(1);
const int32_t ewol::compositing::Area::m_vboIdColor(2);
#define NB_VBO (3)

ewol::compositing::Area::Area(const ivec2& _size) :
  m_position(0.0, 0.0, 0.0),
  m_color(etk::color::white),
  m_GLprogram(null),
  m_GLPosition(-1),
  m_GLMatrix(-1),
  m_GLColor(-1),
  m_GLtexture(-1),
  m_GLtexID(-1),
  m_resource(null) {
	m_resource = ewol::resource::Texture::create();
	m_resource->setImageSize(_size);
	m_resource->flush();
	// Create the VBO:
	m_VBO = gale::resource::VirtualBufferObject::create(NB_VBO);
	if (m_VBO == null) {
		EWOL_ERROR("can not instanciate VBO ...");
		return;
	}
	// TO facilitate some debugs we add a name of the VBO:
	m_VBO->setName("[VBO] of ewol::compositing::Area");
	loadProgram();
}

ewol::compositing::Area::~Area() {
	
}

void ewol::compositing::Area::loadProgram() {
	// get the shader resource :
	m_GLPosition = 0;
	m_GLprogram = gale::resource::Program::create(etk::String("DATA:///textured3D.prog?lib=ewol"));
	if (m_GLprogram != null) {
		m_GLPosition = m_GLprogram->getAttribute("EW_coord3d");
		m_GLColor    = m_GLprogram->getAttribute("EW_color");
		m_GLtexture  = m_GLprogram->getAttribute("EW_texture2d");
		m_GLMatrix   = m_GLprogram->getUniform("EW_MatrixTransformation");
		m_GLtexID    = m_GLprogram->getUniform("EW_texID");
	}
}

void ewol::compositing::Area::draw(bool _disableDepthTest) {
	if (m_VBO->bufferSize(m_vboIdCoord) <= 0) {
		//EWOL_WARNING("Nothink to draw...");
		return;
	}
	if (m_resource == null) {
		// this is a normale case ... the user can choice to have no image ...
		return;
	}
	if (m_GLprogram == null) {
		EWOL_ERROR("No shader ...");
		return;
	}
	// set Matrix : translation/positionMatrix
	mat4 tmpMatrix = gale::openGL::getMatrix()*m_matrixApply;
	m_GLprogram->use(); 
	m_GLprogram->uniformMatrix(m_GLMatrix, tmpMatrix);
	// TextureID
	m_GLprogram->setTexture0(m_GLtexID, m_resource->getRendererId());
	// position:
	m_GLprogram->sendAttributePointer(m_GLPosition, m_VBO, m_vboIdCoord);
	// Texture:
	m_GLprogram->sendAttributePointer(m_GLtexture, m_VBO, m_vboIdColor);
	// color:
	m_GLprogram->sendAttributePointer(m_GLColor, m_VBO, m_vboIdCoordText);
	// Request the draw od the elements : 
	gale::openGL::drawArrays(gale::openGL::renderMode::triangle, 0, m_VBO->bufferSize(m_vboIdCoord));
	m_GLprogram->unUse();
}

void ewol::compositing::Area::clear() {
	// call upper class
	ewol::Compositing::clear();
	// reset all VBOs:
	m_VBO->clear();
	// reset temporal variables :
	m_position = vec3(0.0, 0.0, 0.0);
}

void ewol::compositing::Area::print(const ivec2& _size) {
	vec3 point(0,0,0);
	vec2 tex(0,1);
	point.setX(m_position.x());
	point.setY(m_position.y());
	m_VBO->pushOnBuffer(m_vboIdCoord, point);
	m_VBO->pushOnBuffer(m_vboIdColor, m_color);
	m_VBO->pushOnBuffer(m_vboIdCoordText, tex);
	
	tex.setValue(1,1);
	point.setX(m_position.x() + _size.x());
	point.setY(m_position.y());
	m_VBO->pushOnBuffer(m_vboIdCoord, point);
	m_VBO->pushOnBuffer(m_vboIdColor, m_color);
	m_VBO->pushOnBuffer(m_vboIdCoordText, tex);
	
	tex.setValue(1,0);
	point.setX(m_position.x() + _size.x());
	point.setY(m_position.y() + _size.y());
	m_VBO->pushOnBuffer(m_vboIdCoord, point);
	m_VBO->pushOnBuffer(m_vboIdColor, m_color);
	m_VBO->pushOnBuffer(m_vboIdCoordText, tex);
	
	m_VBO->pushOnBuffer(m_vboIdCoord, point);
	m_VBO->pushOnBuffer(m_vboIdColor, m_color);
	m_VBO->pushOnBuffer(m_vboIdCoordText, tex);
	
	tex.setValue(0,0);
	point.setX(m_position.x());
	point.setY(m_position.y() + _size.y());
	m_VBO->pushOnBuffer(m_vboIdCoord, point);
	m_VBO->pushOnBuffer(m_vboIdColor, m_color);
	m_VBO->pushOnBuffer(m_vboIdCoordText, tex);
	
	tex.setValue(0,1);
	point.setX(m_position.x());
	point.setY(m_position.y());
	m_VBO->pushOnBuffer(m_vboIdCoord, point);
	m_VBO->pushOnBuffer(m_vboIdColor, m_color);
	m_VBO->pushOnBuffer(m_vboIdCoordText, tex);
	
	m_VBO->flush();
}


