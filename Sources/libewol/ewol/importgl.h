
#ifndef __IMPORTGL_H__
#define __IMPORTGL_H__

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__PLATFORM__Linux)
	#include <GL/gl.h>
	#include <GL/glu.h>
#elif defined(__PLATFORM__DoubleBuffer)
	
#elif defined(__PLATFORM__Android)
	#include <GLES/gl.h>
#elif defined(__PLATFORM__AndroidTablet)
	#include <GLES/gl.h>
#elif defined(__PLATFORM__IPhone)
	
#elif defined(__PLATFORM__IPad)
	
#else
	#error you need to specify a platform ...
#endif

void glOrthoEwol(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearVal, GLfloat farVal);

#ifdef __cplusplus
}
#endif


#endif
