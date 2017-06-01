/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#ifndef __TARGET_OS__Web

#include <ewol/debug.hpp>
#include <ewol/resource/Colored3DObject.hpp>
#include <gale/resource/Manager.hpp>
#include <gale/renderer/openGL/openGL-include.hpp>
#include <esignal/Signal.hpp>

ewol::resource::Colored3DObject::Colored3DObject() :
  m_GLprogram(nullptr) {
	addResourceType("ewol::Colored3DObject");
}

void ewol::resource::Colored3DObject::init() {
	gale::Resource::init();
	// get the shader resource :
	m_GLPosition = 0;
	m_GLprogram = gale::resource::Program::create("{ewol}DATA:simple3D.prog");
	if (m_GLprogram != nullptr) {
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
		gale::openGL::enable(gale::openGL::flag_depthTest);
		if (false == _updateDepthBuffer) {
			glDepthMask(GL_FALSE);
		}
	}
	//EWOL_DEBUG("    display " << m_coord.size() << " elements" );
	m_GLprogram->use();
	// set Matrix: translation/positionMatrix
	mat4 projMatrix = gale::openGL::getMatrix();
	mat4 camMatrix = gale::openGL::getCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix;
	m_GLprogram->uniformMatrix(m_GLMatrix, tmpMatrix);
	// position :
	m_GLprogram->sendAttribute(m_GLPosition, 3/*x,y,z,unused*/, &_vertices[0], 4*sizeof(float));
	// color :
	m_GLprogram->uniform4fv(m_GLColor, 1/*r,g,b,a*/, (float*)&_color);
	// Request the draw od the elements: 
	gale::openGL::drawArrays(gale::openGL::renderMode::triangle, 0, _vertices.size());
	m_GLprogram->unUse();
	// Request the draw od the elements: 
	//glDrawArrays(GL_LINES, 0, vertices.size());
	//m_GLprogram->UnUse();
	if (true == _depthtest) {
		if (false == _updateDepthBuffer) {
			glDepthMask(GL_TRUE);
		}
		gale::openGL::disable(gale::openGL::flag_depthTest);
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
		gale::openGL::enable(gale::openGL::flag_depthTest);
		if (false == _updateDepthBuffer) {
			glDepthMask(GL_FALSE);
		}
	}
	//EWOL_DEBUG("    display " << m_coord.size() << " elements" );
	m_GLprogram->use();
	// set Matrix: translation/positionMatrix
	mat4 projMatrix = gale::openGL::getMatrix();
	mat4 camMatrix = gale::openGL::getCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix * _transformationMatrix;
	m_GLprogram->uniformMatrix(m_GLMatrix, tmpMatrix);
	// position :
	m_GLprogram->sendAttribute(m_GLPosition, 3/*x,y,z*/, &_vertices[0], 4*sizeof(float));
	// color :
	m_GLprogram->uniform4fv(m_GLColor, 1/*r,g,b,a*/, (float*)&_color);
	// Request the draw od the elements: 
	gale::openGL::drawArrays(gale::openGL::renderMode::triangle, 0, _vertices.size());
	m_GLprogram->unUse();
	if (true == _depthtest) {
		if (false == _updateDepthBuffer) {
			glDepthMask(GL_TRUE);
		}
		gale::openGL::disable(gale::openGL::flag_depthTest);
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
		gale::openGL::enable(gale::openGL::flag_depthTest);
		if (false == _updateDepthBuffer) {
			glDepthMask(GL_FALSE);
		}
	}
	//EWOL_DEBUG("    display " << m_coord.size() << " elements" );
	m_GLprogram->use();
	// set Matrix: translation/positionMatrix
	mat4 projMatrix = gale::openGL::getMatrix();
	mat4 camMatrix = gale::openGL::getCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix * _transformationMatrix;
	m_GLprogram->uniformMatrix(m_GLMatrix, tmpMatrix);
	// position :
	m_GLprogram->sendAttribute(m_GLPosition, 3/*x,y,z*/, &_vertices[0], 4*sizeof(float));
	// color :
	m_GLprogram->uniform4fv(m_GLColor, 1/*r,g,b,a*/, (float*)&_color);
	// Request the draw od the elements: 
	gale::openGL::drawArrays(gale::openGL::renderMode::line, 0, _vertices.size());
	m_GLprogram->unUse();
	if (true == _depthtest) {
		if (false == _updateDepthBuffer) {
			glDepthMask(GL_TRUE);
		}
		gale::openGL::disable(gale::openGL::flag_depthTest);
	}
}


void ewol::resource::Colored3DObject::drawCubeLine(const vec3& _min,
                                                   const vec3& _max,
                                                   const etk::Color<float>& _color,
                                                   mat4& _transformationMatrix,
                                                   bool _updateDepthBuffer,
                                                   bool _depthtest) {
	std::vector<vec3> vertices;
	vertices.push_back(vec3(_min.x(), _min.y(),_min.z()));
	vertices.push_back(vec3(_max.x(), _min.y(),_min.z()));
	
	vertices.push_back(vec3(_max.x(), _min.y(),_min.z()));
	vertices.push_back(vec3(_max.x(), _min.y(),_max.z()));
	
	vertices.push_back(vec3(_max.x(), _min.y(),_max.z()));
	vertices.push_back(vec3(_min.x(), _min.y(),_max.z()));
	
	vertices.push_back(vec3(_min.x(), _min.y(),_max.z()));
	vertices.push_back(vec3(_min.x(), _min.y(),_min.z()));
	
	
	vertices.push_back(vec3(_min.x(), _max.y(),_min.z()));
	vertices.push_back(vec3(_max.x(), _max.y(),_min.z()));
	
	vertices.push_back(vec3(_max.x(), _max.y(),_min.z()));
	vertices.push_back(vec3(_max.x(), _max.y(),_max.z()));
	
	vertices.push_back(vec3(_max.x(), _max.y(),_max.z()));
	vertices.push_back(vec3(_min.x(), _max.y(),_max.z()));
	
	vertices.push_back(vec3(_min.x(), _max.y(),_max.z()));
	vertices.push_back(vec3(_min.x(), _max.y(),_min.z()));
	
	
	vertices.push_back(vec3(_min.x(), _min.y(),_min.z()));
	vertices.push_back(vec3(_min.x(), _max.y(),_min.z()));
	
	vertices.push_back(vec3(_max.x(), _min.y(),_min.z()));
	vertices.push_back(vec3(_max.x(), _max.y(),_min.z()));
	
	vertices.push_back(vec3(_max.x(), _min.y(),_max.z()));
	vertices.push_back(vec3(_max.x(), _max.y(),_max.z()));
	
	vertices.push_back(vec3(_min.x(), _min.y(),_max.z()));
	vertices.push_back(vec3(_min.x(), _max.y(),_max.z()));
	
	drawLine(vertices, _color, _transformationMatrix, _updateDepthBuffer, _depthtest);
}

void ewol::resource::Colored3DObject::drawSquare(const vec3& _size,
                                                 mat4& _transformationMatrix,
                                                 const etk::Color<float>& _tmpColor) {
	std::vector<vec3> tmpVertices;
	static int indices[36] = { 0,1,2,	3,2,1,	4,0,6,
	                           6,0,2,	5,1,4,	4,1,0,
	                           7,3,1,	7,1,5,	5,4,7,
	                           7,4,6,	7,2,3,	7,6,2};
	vec3 vertices[8]={ vec3(_size[0],_size[1],_size[2]),
	                   vec3(-_size[0],_size[1],_size[2]),
	                   vec3(_size[0],-_size[1],_size[2]),
	                   vec3(-_size[0],-_size[1],_size[2]),
	                   vec3(_size[0],_size[1],-_size[2]),
	                   vec3(-_size[0],_size[1],-_size[2]),
	                   vec3(_size[0],-_size[1],-_size[2]),
	                   vec3(-_size[0],-_size[1],-_size[2])};
	tmpVertices.clear();
	for (int32_t iii=0 ; iii<36 ; iii+=3) {
		// normal calculation :
		//btVector3 normal = (vertices[indices[iii+2]]-vertices[indices[iii]]).cross(vertices[indices[iii+1]]-vertices[indices[iii]]);
		//normal.normalize ();
		tmpVertices.push_back(vertices[indices[iii]]);
		tmpVertices.push_back(vertices[indices[iii+1]]);
		tmpVertices.push_back(vertices[indices[iii+2]]);
	}
	draw(tmpVertices, _tmpColor, _transformationMatrix);
}

void ewol::resource::Colored3DObject::drawSphere(float _radius,
                                                 int _lats,
                                                 int _longs,
                                                 mat4& _transformationMatrix,
                                                 const etk::Color<float>& _tmpColor) {
	std::vector<vec3> tmpVertices;
	for(int32_t iii=0; iii<=_lats; ++iii) {
		float lat0 = M_PI * (-0.5f + float(iii - 1) / _lats);
		float z0  = _radius*sin(lat0);
		float zr0 = _radius*cos(lat0);
		
		float lat1 = M_PI * (-0.5f + float(iii) / _lats);
		float z1 = _radius*sin(lat1);
		float zr1 = _radius*cos(lat1);
		
		for(int32_t jjj=0; jjj<_longs; ++jjj) {
			float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
			float x = cos(lng);
			float y = sin(lng);
			vec3 v1 = vec3(x * zr1, y * zr1, z1);
			vec3 v4 = vec3(x * zr0, y * zr0, z0);
			
			lng = 2 * M_PI * float(jjj) / _longs;
			x = cos(lng);
			y = sin(lng);
			vec3 v2 = vec3(x * zr1, y * zr1, z1);
			vec3 v3 = vec3(x * zr0, y * zr0, z0);
			
			tmpVertices.push_back(v1);
			tmpVertices.push_back(v2);
			tmpVertices.push_back(v3);
			
			tmpVertices.push_back(v1);
			tmpVertices.push_back(v3);
			tmpVertices.push_back(v4);
		}
	}
	draw(tmpVertices, _tmpColor, _transformationMatrix);
}
void ewol::resource::Colored3DObject::drawCylinder(float _radius,
                                                   float _size,
                                                   int _lats,
                                                   int _longs,
                                                   mat4& _transformationMatrix,
                                                   const etk::Color<float>& _tmpColor) {
	std::vector<vec3> tmpVertices;
	// center to border (TOP)
	
	// center to border (TOP)
	for(int32_t jjj=0; jjj<_longs; ++jjj) {
		float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
		
		float z = _size*0.5f;
		vec3 v1 = vec3(0.0f, 0.0f, z);
		
		float x = cos(lng)*_radius;
		float y = sin(lng)*_radius;
		vec3 v2 = vec3(x, y, z);
		
		lng = 2.0f * M_PI * float(jjj) / _longs;
		x = cos(lng)*_radius;
		y = sin(lng)*_radius;
		vec3 v3 = vec3(x, y, z);
		tmpVertices.push_back(v1);
		tmpVertices.push_back(v3);
		tmpVertices.push_back(v2);
	}
	// Cylinder
	for(int32_t jjj=0; jjj<_longs; ++jjj) {
		float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
		
		float z = _size*0.5f;
		
		float x = cos(lng)*_radius;
		float y = sin(lng)*_radius;
		vec3 v2  = vec3(x, y, z);
		vec3 v2b = vec3(x, y, -z);
		
		lng = 2.0f * M_PI * float(jjj) / _longs;
		x = cos(lng)*_radius;
		y = sin(lng)*_radius;
		vec3 v3  = vec3(x, y, z);
		vec3 v3b = vec3(x, y, -z);
		
		tmpVertices.push_back(v2);
		tmpVertices.push_back(v3);
		tmpVertices.push_back(v3b);
		
		tmpVertices.push_back(v2);
		tmpVertices.push_back(v3b);
		tmpVertices.push_back(v2b);
	}
	// center to border (BUTTOM)
	for(int32_t jjj=0; jjj<_longs; ++jjj) {
		float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
		
		float z = _size*-0.5f;
		vec3 v1 = vec3(0.0f, 0.0f, z);
		
		float x = cos(lng)*_radius;
		float y = sin(lng)*_radius;
		vec3 v2 = vec3(x, y, z);
		
		lng = 2.0f * M_PI * float(jjj) / _longs;
		x = cos(lng)*_radius;
		y = sin(lng)*_radius;
		vec3 v3 = vec3(x, y, z);
		tmpVertices.push_back(v1);
		tmpVertices.push_back(v2);
		tmpVertices.push_back(v3);
	}
	draw(tmpVertices, _tmpColor, _transformationMatrix);
}
void ewol::resource::Colored3DObject::drawCapsule(float _radius,
                                                  float _size,
                                                  int _lats,
                                                  int _longs,
                                                  mat4& _transformationMatrix,
                                                  const etk::Color<float>& _tmpColor) {
	std::vector<vec3> tmpVertices;
	_lats = int32_t(_lats / 2)*2;
	
	// center to border (TOP)
	float offset = _size*0.5f;
	for(int32_t iii=_lats/2+1; iii<=_lats; ++iii) {
		float lat0 = M_PI * (-0.5f + float(iii - 1) / _lats);
		float z0  = _radius*sin(lat0);
		float zr0 = _radius*cos(lat0);
		
		float lat1 = M_PI * (-0.5f + float(iii) / _lats);
		float z1 = _radius*sin(lat1);
		float zr1 = _radius*cos(lat1);
		
		for(int32_t jjj=0; jjj<_longs; ++jjj) {
			float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
			float x = cos(lng);
			float y = sin(lng);
			vec3 v1 = vec3(x * zr1, y * zr1, z1+offset);
			vec3 v4 = vec3(x * zr0, y * zr0, z0+offset);
			
			lng = 2 * M_PI * float(jjj) / _longs;
			x = cos(lng);
			y = sin(lng);
			vec3 v2 = vec3(x * zr1, y * zr1, z1+offset);
			vec3 v3 = vec3(x * zr0, y * zr0, z0+offset);
			tmpVertices.push_back(v1);
			tmpVertices.push_back(v2);
			tmpVertices.push_back(v3);
			
			tmpVertices.push_back(v1);
			tmpVertices.push_back(v3);
			tmpVertices.push_back(v4);
		}
	}
	// Cylinder
	for(int32_t jjj=0; jjj<_longs; ++jjj) {
		float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
		
		float z = _size*0.5f;
		
		float x = cos(lng)*_radius;
		float y = sin(lng)*_radius;
		vec3 v2  = vec3(x, y, z);
		vec3 v2b = vec3(x, y, -z);
		
		lng = 2.0f * M_PI * float(jjj) / _longs;
		x = cos(lng)*_radius;
		y = sin(lng)*_radius;
		vec3 v3  = vec3(x, y, z);
		vec3 v3b = vec3(x, y, -z);
		
		tmpVertices.push_back(v2);
		tmpVertices.push_back(v3);
		tmpVertices.push_back(v3b);
		
		tmpVertices.push_back(v2);
		tmpVertices.push_back(v3b);
		tmpVertices.push_back(v2b);
	}
	// center to border (BUTTOM)
	offset = -_size*0.5f;
	for(int32_t iii=0; iii<=_lats/2; ++iii) {
		float lat0 = M_PI * (-0.5f + float(iii - 1) / _lats);
		float z0  = _radius*sin(lat0);
		float zr0 = _radius*cos(lat0);
		
		float lat1 = M_PI * (-0.5f + float(iii) / _lats);
		float z1 = _radius*sin(lat1);
		float zr1 = _radius*cos(lat1);
		
		for(int32_t jjj=0; jjj<_longs; ++jjj) {
			float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
			float x = cos(lng);
			float y = sin(lng);
			vec3 v1 = vec3(x * zr1, y * zr1, z1+offset);
			vec3 v4 = vec3(x * zr0, y * zr0, z0+offset);
			
			lng = 2 * M_PI * float(jjj) / _longs;
			x = cos(lng);
			y = sin(lng);
			vec3 v2 = vec3(x * zr1, y * zr1, z1+offset);
			vec3 v3 = vec3(x * zr0, y * zr0, z0+offset);
			tmpVertices.push_back(v1);
			tmpVertices.push_back(v2);
			tmpVertices.push_back(v3);
			
			tmpVertices.push_back(v1);
			tmpVertices.push_back(v3);
			tmpVertices.push_back(v4);
		}
	}
	draw(tmpVertices, _tmpColor, _transformationMatrix);
}

void ewol::resource::Colored3DObject::drawCone(float _radius,
                                               float _size,
                                               int _lats,
                                               int _longs,
                                               mat4& _transformationMatrix,
                                               const etk::Color<float>& _tmpColor) {
	std::vector<vec3> tmpVertices;
	// center to border (TOP)
	for(int32_t jjj=0; jjj<_longs; ++jjj) {
		float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
		vec3 v1 = vec3(0.0f, 0.0f, -_size/2);
		
		float x = cos(lng)*_radius;
		float y = sin(lng)*_radius;
		vec3 v2 = vec3(x, y, _size/2);
		
		lng = 2.0f * M_PI * float(jjj) / _longs;
		x = cos(lng)*_radius;
		y = sin(lng)*_radius;
		vec3 v3 = vec3(x, y, _size/2);
		tmpVertices.push_back(v1);
		tmpVertices.push_back(v3);
		tmpVertices.push_back(v2);
	}
	// center to border (BUTTOM)
	for(int32_t jjj=0; jjj<_longs; ++jjj) {
		float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
		
		vec3 v1 = vec3(0.0f, 0.0f, _size/2);
		
		float x = cos(lng)*_radius;
		float y = sin(lng)*_radius;
		vec3 v2 = vec3(x, y, _size/2);
		
		lng = 2.0f * M_PI * float(jjj) / _longs;
		x = cos(lng)*_radius;
		y = sin(lng)*_radius;
		vec3 v3 = vec3(x, y, _size/2);
		tmpVertices.push_back(v1);
		tmpVertices.push_back(v2);
		tmpVertices.push_back(v3);
	}
	draw(tmpVertices, _tmpColor, _transformationMatrix);
}

namespace etk {
	template<> std::string to_string(ewol::resource::Colored3DObject const&) {
		return "!!ewol::resource::Colored3DObject!ERROR!CAN_NOT_BE_CONVERT!!";
	}
}
#include <esignal/details/Signal.hxx>

// declare for signal event
ESIGNAL_DECLARE_SIGNAL(ewol::resource::Colored3DObject);
ESIGNAL_DECLARE_SIGNAL(ememory::SharedPtr<ewol::resource::Colored3DObject>);

#endif

