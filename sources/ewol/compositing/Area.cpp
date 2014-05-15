/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/compositing/Area.h>

#undef __class__
#define __class__	"ewol::compositing::Area"

ewol::compositing::Area::Area(const ivec2& _size) :
  m_position(0.0, 0.0, 0.0),
  m_color(etk::color::white),
  m_GLprogram(NULL),
  m_GLPosition(-1),
  m_GLMatrix(-1),
  m_GLColor(-1),
  m_GLtexture(-1),
  m_GLtexID(-1),
  m_resource(NULL) {
	m_resource = ewol::resource::Texture::keep();
	m_resource->setImageSize(_size);
	m_resource->flush();
	loadProgram();
}

ewol::compositing::Area::~Area() {
	ewol::resource::Texture::release(m_resource);
	ewol::resource::Program::release(m_GLprogram);
}

void ewol::compositing::Area::loadProgram() {
	// get the shader resource :
	m_GLPosition = 0;
	m_GLprogram = ewol::resource::Program::keep("DATA:textured3D.prog");
	if (NULL != m_GLprogram) {
		m_GLPosition = m_GLprogram->getAttribute("EW_coord3d");
		m_GLColor    = m_GLprogram->getAttribute("EW_color");
		m_GLtexture  = m_GLprogram->getAttribute("EW_texture2d");
		m_GLMatrix   = m_GLprogram->getUniform("EW_MatrixTransformation");
		m_GLtexID    = m_GLprogram->getUniform("EW_texID");
	}
}

void ewol::compositing::Area::draw(bool _disableDepthTest) {
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
	// set Matrix : translation/positionMatrix
	mat4 tmpMatrix = ewol::openGL::getMatrix()*m_matrixApply;
	m_GLprogram->use(); 
	m_GLprogram->uniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// TextureID
	m_GLprogram->setTexture0(m_GLtexID, m_resource->getId());
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

void ewol::compositing::Area::clear() {
	// call upper class
	ewol::Compositing::clear();
	// reset Buffer :
	m_coord.clear();
	m_coordTex.clear();
	m_coordColor.clear();
	// reset temporal variables :
	m_position = vec3(0.0, 0.0, 0.0);
}

void ewol::compositing::Area::print(const ivec2& _size) {
	vec3 point(0,0,0);
	vec2 tex(0,1);
	point.setX(m_position.x());
	point.setY(m_position.y());
	m_coord.push_back(point);
	m_coordTex.push_back(tex);
	m_coordColor.push_back(m_color);
	
	tex.setValue(1,1);
	point.setX(m_position.x() + _size.x());
	point.setY(m_position.y());
	m_coord.push_back(point);
	m_coordTex.push_back(tex);
	m_coordColor.push_back(m_color);
	
	tex.setValue(1,0);
	point.setX(m_position.x() + _size.x());
	point.setY(m_position.y() + _size.y());
	m_coord.push_back(point);
	m_coordTex.push_back(tex);
	m_coordColor.push_back(m_color);
	
	m_coord.push_back(point);
	m_coordTex.push_back(tex);
	m_coordColor.push_back(m_color);
	
	tex.setValue(0,0);
	point.setX(m_position.x());
	point.setY(m_position.y() + _size.y());
	m_coord.push_back(point);
	m_coordTex.push_back(tex);
	m_coordColor.push_back(m_color);
	
	tex.setValue(0,1);
	point.setX(m_position.x());
	point.setY(m_position.y());
	m_coord.push_back(point);
	m_coordTex.push_back(tex);
	m_coordColor.push_back(m_color);
}


