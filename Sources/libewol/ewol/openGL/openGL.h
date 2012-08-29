/**
 *******************************************************************************
 * @file ewol/openGL/openGL.h
 * @brief ewol openGl abstarction (header)
 * @author Edouard DUPIN
 * @date 19/09/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#ifndef __OPEN_GL_H__
#define __OPEN_GL_H__

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__TARGET_OS__Linux)
	#ifdef __VIDEO__OPENGL_ES_2
		// TO ENABLE THE SHADER api ...
		#define GL_GLEXT_PROTOTYPES
	#endif
	#include <GL/gl.h>
#elif defined(__TARGET_OS__Android)
	#ifdef __VIDEO__OPENGL_ES_2
		#include <GLES2/gl2.h>
		#include <GLES2/gl2ext.h>
	#else
		
	#endif
#elif defined(__TARGET_OS__Windows)
	#ifdef __VIDEO__OPENGL_ES_2
		// TO ENABLE THE SHADER api ...
		#define GL_GLEXT_PROTOTYPES
	#endif
	#include <GL/gl.h>
#elif defined(__TARGET_OS__MacOs)
	
#elif defined(__TARGET_OS__IOs)
	
#else
	#error you need to specify a __TAGET_OS__ ...
#endif

void glOrthoEwol(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearVal, GLfloat farVal);

namespace ewol {
	class OglMatrix{
		public :
			float m_Matrix[4*4];
			OglMatrix(float left, float right, float bottom, float top, float nearVal, float farVal);
			~OglMatrix();
			void Generate(float left, float right, float bottom, float top, float nearVal, float farVal);
			void Translate(float x=0.0, float y=0.0, float z=0.0);
			void Scale(float x=1.0, float y=1.0, float z=1.0);
			void rotate(float x, float y, float z, float angle=0.0);
	};
};

#ifdef __cplusplus
}
#endif


#endif
