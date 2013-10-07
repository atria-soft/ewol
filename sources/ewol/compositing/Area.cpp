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
#define __class__	"ewol::Area"

ewol::Area::Area(const ivec2& _size) :
	m_position(0.0, 0.0, 0.0),
	m_color(etk::color::white),
	m_GLprogram(NULL),
	m_GLPosition(-1),
	m_GLMatrix(-1),
	m_GLColor(-1),
	m_GLtexture(-1),
	m_GLtexID(-1),
	m_resource(NULL)
{
	m_resource = ewol::Texture::keep();
	m_resource->setImageSize(_size);
	m_resource->flush();
	loadProgram();
}

ewol::Area::~Area(void)
{
	ewol::Texture::release(m_resource);
	ewol::Program::release(m_GLprogram);
}

void ewol::Area::loadProgram(void)
{
	// get the shader resource :
	m_GLPosition = 0;
	m_GLprogram = ewol::Program::keep("DATA:textured3D.prog");
	if (NULL != m_GLprogram) {
		m_GLPosition = m_GLprogram->getAttribute("EW_coord3d");
		m_GLColor    = m_GLprogram->getAttribute("EW_color");
		m_GLtexture  = m_GLprogram->getAttribute("EW_texture2d");
		m_GLMatrix   = m_GLprogram->getUniform("EW_MatrixTransformation");
		m_GLtexID    = m_GLprogram->getUniform("EW_texID");
	}
}

void ewol::Area::draw(bool _disableDepthTest)
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

void ewol::Area::clear(void)
{
	// call upper class
	ewol::Compositing::clear();
	// reset Buffer :
	m_coord.clear();
	m_coordTex.clear();
	m_coordColor.clear();
	// reset temporal variables :
	m_position = vec3(0.0, 0.0, 0.0);
}

void ewol::Area::print(const ivec2& _size)
{
	vec3 point(0,0,0);
	vec2 tex(0,1);

	point.setX(m_position.x());
	point.setY(m_position.y());
	m_coord.pushBack(point);
	m_coordTex.pushBack(tex);
	m_coordColor.pushBack(m_color);


	tex.setValue(1,1);
	point.setX(m_position.x() + _size.x());
	point.setY(m_position.y());
	m_coord.pushBack(point);
	m_coordTex.pushBack(tex);
	m_coordColor.pushBack(m_color);


	tex.setValue(1,0);
	point.setX(m_position.x() + _size.x());
	point.setY(m_position.y() + _size.y());
	m_coord.pushBack(point);
	m_coordTex.pushBack(tex);
	m_coordColor.pushBack(m_color);

	m_coord.pushBack(point);
	m_coordTex.pushBack(tex);
	m_coordColor.pushBack(m_color);

	tex.setValue(0,0);
	point.setX(m_position.x());
	point.setY(m_position.y() + _size.y());
	m_coord.pushBack(point);
	m_coordTex.pushBack(tex);
	m_coordColor.pushBack(m_color);

	tex.setValue(0,1);
	point.setX(m_position.x());
	point.setY(m_position.y());
	m_coord.pushBack(point);
	m_coordTex.pushBack(tex);
	m_coordColor.pushBack(m_color);
}


