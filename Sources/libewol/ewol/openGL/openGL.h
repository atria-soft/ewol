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

#include <etk/math/Matrix4.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__TARGET_OS__Linux)
	// TO ENABLE THE SHADER api ...
	#define GL_GLEXT_PROTOTYPES
	#include <GL/gl.h>
	// TODO : Check it it work
	// This is to prevent the use of these element that is not allowed in the OpenGL ES
	#undef glVertexPointer
	#undef glTexCoordPointer
	#undef glColorPointer
	#undef glPopMatrix
	#undef glPushMatrix
	#undef glMatrixMode
	#undef glLoadIdentity
	#undef glTranslatef
#elif defined(__TARGET_OS__Android)
	// Include openGL ES 2
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#elif defined(__TARGET_OS__Windows)
	// TO ENABLE THE SHADER api ...
	//#define GL_GLEXT_PROTOTYPES
	#define GLEW_STATIC
	#include <GL/glew.h>
#elif defined(__TARGET_OS__MacOs)
	
#elif defined(__TARGET_OS__IOs)
	
#else
	#error you need to specify a __TAGET_OS__ ...
#endif

namespace ewol {
	namespace openGL {
		void Init(void);
		void UnInit(void);
		// reset the basic element at this one ... remove all previous
		void SetBasicMatrix(etk::Matrix4& newOne);
		// this is the same system as openGL-ES-1 but in openGL-ES-2 (here is the abstraction)
		void SetMatrix(etk::Matrix4& newOne);
		void Push(void);
		void Pop(void);
		etk::Matrix4& GetMatrix(void);
	};
};

#ifdef __cplusplus
}
#endif


#endif
