
#ifndef __IMPORTGL_H__
#define __IMPORTGL_H__

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__TARGET_OS__Linux)
	#include <GL/gl.h>
	#include <GL/glu.h>
#elif defined(__TARGET_OS__Android)
	#include <GLES/gl.h>
#elif defined(__TARGET_OS__Windows)
	#include <GL/gl.h>
#elif defined(__TARGET_OS__MacOs)
	
#elif defined(__TARGET_OS__IOs)
	
#else
	#error you need to specify a __TAGET_OS__ ...
#endif

void glOrthoEwol(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearVal, GLfloat farVal);

#ifdef __cplusplus
}
#endif


#endif
