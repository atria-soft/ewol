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

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::resource::Colored3DObject);

ewol::resource::Colored3DObject::Colored3DObject() :
  this.GLprogram(null) {
	addResourceType("ewol::Colored3DObject");
}

void ewol::resource::Colored3DObject::init() {
	gale::Resource::init();
	// get the shader resource :
	this.GLPosition = 0;
	this.GLprogram = gale::resource::Program::create("DATA:///simple3D.prog?lib=ewol");
	if (this.GLprogram != null) {
		this.GLPosition = this.GLprogram.getAttribute("EW_coord3d");
		this.GLColor    = this.GLprogram.getUniform("EW_color");
		this.GLMatrix   = this.GLprogram.getUniform("EW_MatrixTransformation");
	}
}

ewol::resource::Colored3DObject::~Colored3DObject() {
	
}


void ewol::resource::Colored3DObject::draw( List<Vector3f> _vertices,
                                            etk::Color<float> _color,
                                           boolean _updateDepthBuffer,
                                           boolean _depthtest) {
	if (_vertices.size() <= 0) {
		return;
	}
	if (this.GLprogram == null) {
		Log.error("No shader ...");
		return;
	}
	if (true == _depthtest) {
		gale::openGL::enable(gale::openGL::flag_depthTest);
		if (false == _updateDepthBuffer) {
			glDepthMask(GL_FALSE);
		}
	}
	//Log.debug("    display " + this.coord.size() + " elements" );
	this.GLprogram.use();
	// set Matrix: translation/positionMatrix
	mat4 projMatrix = gale::openGL::getMatrix();
	mat4 camMatrix = gale::openGL::getCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix;
	this.GLprogram.uniformMatrix(this.GLMatrix, tmpMatrix);
	// position :
	this.GLprogram.sendAttribute(this.GLPosition, 3/*x,y,z,unused*/, _vertices[0], 4*sizeof(float));
	// color :
	this.GLprogram.uniform4fv(this.GLColor, 1/*r,g,b,a*/, (float*)_color);
	// Request the draw od the elements: 
	gale::openGL::drawArrays(gale::openGL::renderMode::triangle, 0, _vertices.size());
	this.GLprogram.unUse();
	// Request the draw od the elements: 
	//glDrawArrays(GL_LINES, 0, vertices.size());
	//this.GLprogram.UnUse();
	if (true == _depthtest) {
		if (false == _updateDepthBuffer) {
			glDepthMask(GL_TRUE);
		}
		gale::openGL::disable(gale::openGL::flag_depthTest);
	}
}

void ewol::resource::Colored3DObject::draw( List<Vector3f> _vertices,
                                            etk::Color<float> _color,
                                           mat4 _transformationMatrix,
                                           boolean _updateDepthBuffer,
                                           boolean _depthtest) {
	if (_vertices.size() <= 0) {
		return;
	}
	if (this.GLprogram == null) {
		Log.error("No shader ...");
		return;
	}
	if (true == _depthtest) {
		gale::openGL::enable(gale::openGL::flag_depthTest);
		if (false == _updateDepthBuffer) {
			glDepthMask(GL_FALSE);
		}
	}
	//Log.debug("    display " + this.coord.size() + " elements" );
	this.GLprogram.use();
	// set Matrix: translation/positionMatrix
	mat4 projMatrix = gale::openGL::getMatrix();
	mat4 camMatrix = gale::openGL::getCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix * _transformationMatrix;
	this.GLprogram.uniformMatrix(this.GLMatrix, tmpMatrix);
	// position :
	this.GLprogram.sendAttribute(this.GLPosition, 3/*x,y,z*/, _vertices[0], 4*sizeof(float));
	// color :
	this.GLprogram.uniform4fv(this.GLColor, 1/*r,g,b,a*/, (float*)_color);
	// Request the draw od the elements: 
	gale::openGL::drawArrays(gale::openGL::renderMode::triangle, 0, _vertices.size());
	this.GLprogram.unUse();
	if (true == _depthtest) {
		if (false == _updateDepthBuffer) {
			glDepthMask(GL_TRUE);
		}
		gale::openGL::disable(gale::openGL::flag_depthTest);
	}
}

void ewol::resource::Colored3DObject::drawLine(List<Vector3f> _vertices,
                                                etk::Color<float> _color,
                                               mat4 _transformationMatrix,
                                               boolean _updateDepthBuffer,
                                               boolean _depthtest) {
	if (_vertices.size() <= 0) {
		return;
	}
	if (this.GLprogram == null) {
		Log.error("No shader ...");
		return;
	}
	if (true == _depthtest) {
		gale::openGL::enable(gale::openGL::flag_depthTest);
		if (false == _updateDepthBuffer) {
			glDepthMask(GL_FALSE);
		}
	}
	//Log.debug("    display " + this.coord.size() + " elements" );
	this.GLprogram.use();
	// set Matrix: translation/positionMatrix
	mat4 projMatrix = gale::openGL::getMatrix();
	mat4 camMatrix = gale::openGL::getCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix * _transformationMatrix;
	this.GLprogram.uniformMatrix(this.GLMatrix, tmpMatrix);
	// position :
	this.GLprogram.sendAttribute(this.GLPosition, 3/*x,y,z*/, _vertices[0], 4*sizeof(float));
	// color :
	this.GLprogram.uniform4fv(this.GLColor, 1/*r,g,b,a*/, (float*)_color);
	// Request the draw od the elements: 
	gale::openGL::drawArrays(gale::openGL::renderMode::line, 0, _vertices.size());
	this.GLprogram.unUse();
	if (true == _depthtest) {
		if (false == _updateDepthBuffer) {
			glDepthMask(GL_TRUE);
		}
		gale::openGL::disable(gale::openGL::flag_depthTest);
	}
}


void ewol::resource::Colored3DObject::drawCubeLine( Vector3f _min,
                                                    Vector3f _max,
                                                    etk::Color<float> _color,
                                                   mat4 _transformationMatrix,
                                                   boolean _updateDepthBuffer,
                                                   boolean _depthtest) {
	List<Vector3f> vertices;
	vertices.pushBack(Vector3f(_min.x(), _min.y(),_min.z()));
	vertices.pushBack(Vector3f(_max.x(), _min.y(),_min.z()));
	
	vertices.pushBack(Vector3f(_max.x(), _min.y(),_min.z()));
	vertices.pushBack(Vector3f(_max.x(), _min.y(),_max.z()));
	
	vertices.pushBack(Vector3f(_max.x(), _min.y(),_max.z()));
	vertices.pushBack(Vector3f(_min.x(), _min.y(),_max.z()));
	
	vertices.pushBack(Vector3f(_min.x(), _min.y(),_max.z()));
	vertices.pushBack(Vector3f(_min.x(), _min.y(),_min.z()));
	
	
	vertices.pushBack(Vector3f(_min.x(), _max.y(),_min.z()));
	vertices.pushBack(Vector3f(_max.x(), _max.y(),_min.z()));
	
	vertices.pushBack(Vector3f(_max.x(), _max.y(),_min.z()));
	vertices.pushBack(Vector3f(_max.x(), _max.y(),_max.z()));
	
	vertices.pushBack(Vector3f(_max.x(), _max.y(),_max.z()));
	vertices.pushBack(Vector3f(_min.x(), _max.y(),_max.z()));
	
	vertices.pushBack(Vector3f(_min.x(), _max.y(),_max.z()));
	vertices.pushBack(Vector3f(_min.x(), _max.y(),_min.z()));
	
	
	vertices.pushBack(Vector3f(_min.x(), _min.y(),_min.z()));
	vertices.pushBack(Vector3f(_min.x(), _max.y(),_min.z()));
	
	vertices.pushBack(Vector3f(_max.x(), _min.y(),_min.z()));
	vertices.pushBack(Vector3f(_max.x(), _max.y(),_min.z()));
	
	vertices.pushBack(Vector3f(_max.x(), _min.y(),_max.z()));
	vertices.pushBack(Vector3f(_max.x(), _max.y(),_max.z()));
	
	vertices.pushBack(Vector3f(_min.x(), _min.y(),_max.z()));
	vertices.pushBack(Vector3f(_min.x(), _max.y(),_max.z()));
	
	drawLine(vertices, _color, _transformationMatrix, _updateDepthBuffer, _depthtest);
}

void ewol::resource::Colored3DObject::drawSquare( Vector3f _size,
                                                 mat4 _transformationMatrix,
                                                  etk::Color<float> _tmpColor) {
	List<Vector3f> tmpVertices;
	static int indices[36] = { 0,1,2,	3,2,1,	4,0,6,
	                           6,0,2,	5,1,4,	4,1,0,
	                           7,3,1,	7,1,5,	5,4,7,
	                           7,4,6,	7,2,3,	7,6,2};
	Vector3f vertices[8]={ Vector3f(_size[0],_size[1],_size[2]),
	                   Vector3f(-_size[0],_size[1],_size[2]),
	                   Vector3f(_size[0],-_size[1],_size[2]),
	                   Vector3f(-_size[0],-_size[1],_size[2]),
	                   Vector3f(_size[0],_size[1],-_size[2]),
	                   Vector3f(-_size[0],_size[1],-_size[2]),
	                   Vector3f(_size[0],-_size[1],-_size[2]),
	                   Vector3f(-_size[0],-_size[1],-_size[2])};
	tmpVertices.clear();
	for (int iii=0 ; iii<36 ; iii+=3) {
		// normal calculation :
		//btVector3 normal = (vertices[indices[iii+2]]-vertices[indices[iii]]).cross(vertices[indices[iii+1]]-vertices[indices[iii]]);
		//normal.normalize ();
		tmpVertices.pushBack(vertices[indices[iii]]);
		tmpVertices.pushBack(vertices[indices[iii+1]]);
		tmpVertices.pushBack(vertices[indices[iii+2]]);
	}
	draw(tmpVertices, _tmpColor, _transformationMatrix);
}

void ewol::resource::Colored3DObject::drawSphere(float _radius,
                                                 int _lats,
                                                 int _longs,
                                                 mat4 _transformationMatrix,
                                                  etk::Color<float> _tmpColor) {
	List<Vector3f> tmpVertices;
	for(int iii=0; iii<=_lats; ++iii) {
		float lat0 = M_PI * (-0.5f + float(iii - 1) / _lats);
		float z0  = _radius*sin(lat0);
		float zr0 = _radius*cos(lat0);
		
		float lat1 = M_PI * (-0.5f + float(iii) / _lats);
		float z1 = _radius*sin(lat1);
		float zr1 = _radius*cos(lat1);
		
		for(int jjj=0; jjj<_longs; ++jjj) {
			float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
			float x = cos(lng);
			float y = sin(lng);
			Vector3f v1 = Vector3f(x * zr1, y * zr1, z1);
			Vector3f v4 = Vector3f(x * zr0, y * zr0, z0);
			
			lng = 2 * M_PI * float(jjj) / _longs;
			x = cos(lng);
			y = sin(lng);
			Vector3f v2 = Vector3f(x * zr1, y * zr1, z1);
			Vector3f v3 = Vector3f(x * zr0, y * zr0, z0);
			
			tmpVertices.pushBack(v1);
			tmpVertices.pushBack(v2);
			tmpVertices.pushBack(v3);
			
			tmpVertices.pushBack(v1);
			tmpVertices.pushBack(v3);
			tmpVertices.pushBack(v4);
		}
	}
	draw(tmpVertices, _tmpColor, _transformationMatrix);
}
void ewol::resource::Colored3DObject::drawCylinder(float _radius,
                                                   float _size,
                                                   int _lats,
                                                   int _longs,
                                                   mat4 _transformationMatrix,
                                                    etk::Color<float> _tmpColor) {
	List<Vector3f> tmpVertices;
	// center to border (TOP)
	
	// center to border (TOP)
	for(int jjj=0; jjj<_longs; ++jjj) {
		float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
		
		float z = _size*0.5f;
		Vector3f v1 = Vector3f(0.0f, 0.0f, z);
		
		float x = cos(lng)*_radius;
		float y = sin(lng)*_radius;
		Vector3f v2 = Vector3f(x, y, z);
		
		lng = 2.0f * M_PI * float(jjj) / _longs;
		x = cos(lng)*_radius;
		y = sin(lng)*_radius;
		Vector3f v3 = Vector3f(x, y, z);
		tmpVertices.pushBack(v1);
		tmpVertices.pushBack(v3);
		tmpVertices.pushBack(v2);
	}
	// Cylinder
	for(int jjj=0; jjj<_longs; ++jjj) {
		float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
		
		float z = _size*0.5f;
		
		float x = cos(lng)*_radius;
		float y = sin(lng)*_radius;
		Vector3f v2  = Vector3f(x, y, z);
		Vector3f v2b = Vector3f(x, y, -z);
		
		lng = 2.0f * M_PI * float(jjj) / _longs;
		x = cos(lng)*_radius;
		y = sin(lng)*_radius;
		Vector3f v3  = Vector3f(x, y, z);
		Vector3f v3b = Vector3f(x, y, -z);
		
		tmpVertices.pushBack(v2);
		tmpVertices.pushBack(v3);
		tmpVertices.pushBack(v3b);
		
		tmpVertices.pushBack(v2);
		tmpVertices.pushBack(v3b);
		tmpVertices.pushBack(v2b);
	}
	// center to border (BUTTOM)
	for(int jjj=0; jjj<_longs; ++jjj) {
		float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
		
		float z = _size*-0.5f;
		Vector3f v1 = Vector3f(0.0f, 0.0f, z);
		
		float x = cos(lng)*_radius;
		float y = sin(lng)*_radius;
		Vector3f v2 = Vector3f(x, y, z);
		
		lng = 2.0f * M_PI * float(jjj) / _longs;
		x = cos(lng)*_radius;
		y = sin(lng)*_radius;
		Vector3f v3 = Vector3f(x, y, z);
		tmpVertices.pushBack(v1);
		tmpVertices.pushBack(v2);
		tmpVertices.pushBack(v3);
	}
	draw(tmpVertices, _tmpColor, _transformationMatrix);
}
void ewol::resource::Colored3DObject::drawCapsule(float _radius,
                                                  float _size,
                                                  int _lats,
                                                  int _longs,
                                                  mat4 _transformationMatrix,
                                                   etk::Color<float> _tmpColor) {
	List<Vector3f> tmpVertices;
	_lats = int(_lats / 2)*2;
	
	// center to border (TOP)
	float offset = _size*0.5f;
	for(int iii=_lats/2+1; iii<=_lats; ++iii) {
		float lat0 = M_PI * (-0.5f + float(iii - 1) / _lats);
		float z0  = _radius*sin(lat0);
		float zr0 = _radius*cos(lat0);
		
		float lat1 = M_PI * (-0.5f + float(iii) / _lats);
		float z1 = _radius*sin(lat1);
		float zr1 = _radius*cos(lat1);
		
		for(int jjj=0; jjj<_longs; ++jjj) {
			float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
			float x = cos(lng);
			float y = sin(lng);
			Vector3f v1 = Vector3f(x * zr1, y * zr1, z1+offset);
			Vector3f v4 = Vector3f(x * zr0, y * zr0, z0+offset);
			
			lng = 2 * M_PI * float(jjj) / _longs;
			x = cos(lng);
			y = sin(lng);
			Vector3f v2 = Vector3f(x * zr1, y * zr1, z1+offset);
			Vector3f v3 = Vector3f(x * zr0, y * zr0, z0+offset);
			tmpVertices.pushBack(v1);
			tmpVertices.pushBack(v2);
			tmpVertices.pushBack(v3);
			
			tmpVertices.pushBack(v1);
			tmpVertices.pushBack(v3);
			tmpVertices.pushBack(v4);
		}
	}
	// Cylinder
	for(int jjj=0; jjj<_longs; ++jjj) {
		float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
		
		float z = _size*0.5f;
		
		float x = cos(lng)*_radius;
		float y = sin(lng)*_radius;
		Vector3f v2  = Vector3f(x, y, z);
		Vector3f v2b = Vector3f(x, y, -z);
		
		lng = 2.0f * M_PI * float(jjj) / _longs;
		x = cos(lng)*_radius;
		y = sin(lng)*_radius;
		Vector3f v3  = Vector3f(x, y, z);
		Vector3f v3b = Vector3f(x, y, -z);
		
		tmpVertices.pushBack(v2);
		tmpVertices.pushBack(v3);
		tmpVertices.pushBack(v3b);
		
		tmpVertices.pushBack(v2);
		tmpVertices.pushBack(v3b);
		tmpVertices.pushBack(v2b);
	}
	// center to border (BUTTOM)
	offset = -_size*0.5f;
	for(int iii=0; iii<=_lats/2; ++iii) {
		float lat0 = M_PI * (-0.5f + float(iii - 1) / _lats);
		float z0  = _radius*sin(lat0);
		float zr0 = _radius*cos(lat0);
		
		float lat1 = M_PI * (-0.5f + float(iii) / _lats);
		float z1 = _radius*sin(lat1);
		float zr1 = _radius*cos(lat1);
		
		for(int jjj=0; jjj<_longs; ++jjj) {
			float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
			float x = cos(lng);
			float y = sin(lng);
			Vector3f v1 = Vector3f(x * zr1, y * zr1, z1+offset);
			Vector3f v4 = Vector3f(x * zr0, y * zr0, z0+offset);
			
			lng = 2 * M_PI * float(jjj) / _longs;
			x = cos(lng);
			y = sin(lng);
			Vector3f v2 = Vector3f(x * zr1, y * zr1, z1+offset);
			Vector3f v3 = Vector3f(x * zr0, y * zr0, z0+offset);
			tmpVertices.pushBack(v1);
			tmpVertices.pushBack(v2);
			tmpVertices.pushBack(v3);
			
			tmpVertices.pushBack(v1);
			tmpVertices.pushBack(v3);
			tmpVertices.pushBack(v4);
		}
	}
	draw(tmpVertices, _tmpColor, _transformationMatrix);
}

void ewol::resource::Colored3DObject::drawCone(float _radius,
                                               float _size,
                                               int _lats,
                                               int _longs,
                                               mat4 _transformationMatrix,
                                                etk::Color<float> _tmpColor) {
	List<Vector3f> tmpVertices;
	// center to border (TOP)
	for(int jjj=0; jjj<_longs; ++jjj) {
		float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
		Vector3f v1 = Vector3f(0.0f, 0.0f, -_size/2);
		
		float x = cos(lng)*_radius;
		float y = sin(lng)*_radius;
		Vector3f v2 = Vector3f(x, y, _size/2);
		
		lng = 2.0f * M_PI * float(jjj) / _longs;
		x = cos(lng)*_radius;
		y = sin(lng)*_radius;
		Vector3f v3 = Vector3f(x, y, _size/2);
		tmpVertices.pushBack(v1);
		tmpVertices.pushBack(v3);
		tmpVertices.pushBack(v2);
	}
	// center to border (BUTTOM)
	for(int jjj=0; jjj<_longs; ++jjj) {
		float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
		
		Vector3f v1 = Vector3f(0.0f, 0.0f, _size/2);
		
		float x = cos(lng)*_radius;
		float y = sin(lng)*_radius;
		Vector3f v2 = Vector3f(x, y, _size/2);
		
		lng = 2.0f * M_PI * float(jjj) / _longs;
		x = cos(lng)*_radius;
		y = sin(lng)*_radius;
		Vector3f v3 = Vector3f(x, y, _size/2);
		tmpVertices.pushBack(v1);
		tmpVertices.pushBack(v2);
		tmpVertices.pushBack(v3);
	}
	draw(tmpVertices, _tmpColor, _transformationMatrix);
}

void ewol::resource::Colored3DObject::drawTriangles( List<Vector3f> _vertex,
                                                     List<uint> _indice,
                                                    mat4 _transformationMatrix,
                                                     etk::Color<float> _tmpColor,
                                                     Vector3f _offset) {
	List<Vector3f> tmpVertices;
	for (int iii=0; iii<_indice.size()/3; ++iii) {
		tmpVertices.pushBack(_vertex[_indice[iii*3 + 0]]+_offset);
		tmpVertices.pushBack(_vertex[_indice[iii*3 + 1]]+_offset);
		tmpVertices.pushBack(_vertex[_indice[iii*3 + 2]]+_offset);
		//Log.info("  indices " + _indice[iii*3 + 0] + " " + _indice[iii*3 + 1] + " " + _indice[iii*3 + 2]);
		//Log.info(" triangle " + _vertex[_indice[iii*3 + 0]] + " " + _vertex[_indice[iii*3 + 1]] + " " + _vertex[_indice[iii*3 + 2]]);
	}
	//Log.info("display " + tmpVertices.size() + " vertices form " + _indice.size());
	draw(tmpVertices, _tmpColor, _transformationMatrix);
}

namespace etk {
	template<> String toString(ewol::resource::Colored3DObject ) {
		return "!!ewol::resource::Colored3DObject!ERROR!CAN_NOT_BE_CONVERT!!";
	}
}
#include <esignal/details/Signal.hxx>

// declare for signal event
ESIGNAL_DECLARE_SIGNAL(ewol::resource::Colored3DObject);
ESIGNAL_DECLARE_SIGNAL(ememory::Ptr<ewol::resource::Colored3DObject>);

#endif

