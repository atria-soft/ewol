/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/resource/Colored3DObject.h>
#include <ewol/resource/Manager.h>

#undef __class__
#define __class__	"resource::Colored3DObject"


ewol::resource::Colored3DObject::Colored3DObject() :
  m_GLprogram(nullptr) {
	addObjectType("ewol::Colored3DObject");
	// get the shader resource :
	m_GLPosition = 0;
	m_GLprogram = ewol::resource::Program::keep("DATA:simple3D.prog");
	if (nullptr != m_GLprogram ) {
		m_GLPosition = m_GLprogram->getAttribute("EW_coord3d");
		m_GLColor    = m_GLprogram->getUniform("EW_color");
		m_GLMatrix   = m_GLprogram->getUniform("EW_MatrixTransformation");
	}
}

ewol::resource::Colored3DObject::~Colored3DObject() {
	
}


void ewol::resource::Colored3DObject::draw(std::vector<vec3>& _vertices,
                                           const etk::Color<float>& _color,
                                           bool _updateDepthBuffer,
                                           bool _depthtest) {
	if (_vertices.size() <= 0) {
		return;
	}
	if (m_GLprogram == nullptr) {
		EWOL_ERROR("No shader ...");
		return;
	}
	if (true == _depthtest) {
		ewol::openGL::enable(ewol::openGL::FLAG_DEPTH_TEST);
		if (false == _updateDepthBuffer) {
			glDepthMask(GL_FALSE);
		}
	}
	//EWOL_DEBUG("    display " << m_coord.size() << " elements" );
	m_GLprogram->use();
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = ewol::openGL::getMatrix();
	mat4 camMatrix = ewol::openGL::getCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix;
	m_GLprogram->uniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// position :
	m_GLprogram->sendAttribute(m_GLPosition, 3/*x,y,z,unused*/, &_vertices[0], 4*sizeof(float));
	// color :
	m_GLprogram->uniform4fv(m_GLColor, 1/*r,g,b,a*/, (float*)&_color);
	// Request the draw od the elements : 
	ewol::openGL::drawArrays(GL_TRIANGLES, 0, _vertices.size());
	m_GLprogram->unUse();
	// Request the draw od the elements : 
	//glDrawArrays(GL_LINES, 0, vertices.size());
	//m_GLprogram->UnUse();
	if (true == _depthtest) {
		if (false == _updateDepthBuffer) {
			glDepthMask(GL_TRUE);
		}
		ewol::openGL::disable(ewol::openGL::FLAG_DEPTH_TEST);
	}
}

void ewol::resource::Colored3DObject::draw(std::vector<vec3>& _vertices,
                                           const etk::Color<float>& _color,
                                           mat4& _transformationMatrix,
                                           bool _updateDepthBuffer,
                                           bool _depthtest) {
	if (_vertices.size() <= 0) {
		return;
	}
	if (m_GLprogram == nullptr) {
		EWOL_ERROR("No shader ...");
		return;
	}
	if (true == _depthtest) {
		ewol::openGL::enable(ewol::openGL::FLAG_DEPTH_TEST);
		if (false == _updateDepthBuffer) {
			glDepthMask(GL_FALSE);
		}
	}
	//EWOL_DEBUG("    display " << m_coord.size() << " elements" );
	m_GLprogram->use();
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = ewol::openGL::getMatrix();
	mat4 camMatrix = ewol::openGL::getCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix * _transformationMatrix;
	m_GLprogram->uniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// position :
	m_GLprogram->sendAttribute(m_GLPosition, 3/*x,y,z*/, &_vertices[0], 4*sizeof(float));
	// color :
	m_GLprogram->uniform4fv(m_GLColor, 1/*r,g,b,a*/, (float*)&_color);
	// Request the draw od the elements : 
	ewol::openGL::drawArrays(GL_TRIANGLES, 0, _vertices.size());
	m_GLprogram->unUse();
	if (true == _depthtest) {
		if (false == _updateDepthBuffer) {
			glDepthMask(GL_TRUE);
		}
		ewol::openGL::disable(ewol::openGL::FLAG_DEPTH_TEST);
	}
}

void ewol::resource::Colored3DObject::drawLine(std::vector<vec3>& _vertices,
                                               const etk::Color<float>& _color,
                                               mat4& _transformationMatrix,
                                               bool _updateDepthBuffer,
                                               bool _depthtest) {
	if (_vertices.size() <= 0) {
		return;
	}
	if (m_GLprogram == nullptr) {
		EWOL_ERROR("No shader ...");
		return;
	}
	if (true == _depthtest) {
		ewol::openGL::enable(ewol::openGL::FLAG_DEPTH_TEST);
		if (false == _updateDepthBuffer) {
			glDepthMask(GL_FALSE);
		}
	}
	//EWOL_DEBUG("    display " << m_coord.size() << " elements" );
	m_GLprogram->use();
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = ewol::openGL::getMatrix();
	mat4 camMatrix = ewol::openGL::getCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix * _transformationMatrix;
	m_GLprogram->uniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// position :
	m_GLprogram->sendAttribute(m_GLPosition, 3/*x,y,z*/, &_vertices[0], 4*sizeof(float));
	// color :
	m_GLprogram->uniform4fv(m_GLColor, 1/*r,g,b,a*/, (float*)&_color);
	// Request the draw od the elements : 
	ewol::openGL::drawArrays(GL_LINES, 0, _vertices.size());
	m_GLprogram->unUse();
	if (true == _depthtest) {
		if (false == _updateDepthBuffer) {
			glDepthMask(GL_TRUE);
		}
		ewol::openGL::disable(ewol::openGL::FLAG_DEPTH_TEST);
	}
}

ewol::object::Shared<ewol::resource::Colored3DObject> ewol::resource::Colored3DObject::keep() {
	EWOL_VERBOSE("KEEP : direct Colored3DObject");
	// need to crate a new one ...
	ewol::object::Shared<ewol::resource::Colored3DObject> object = ewol::object::makeShared(new ewol::resource::Colored3DObject());
	if (object == nullptr) {
		EWOL_ERROR("allocation error of a resource : ???Colored3DObject??? ");
		return nullptr;
	}
	getManager().localAdd(object);
	return object;
}

