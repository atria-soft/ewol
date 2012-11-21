/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
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
		/**
		 * @brief Initialize the open gl system (all the data register in the graphic card is all time duplicate in the memory)
		 * this is due to the fact of some operating system destroy sometime the opengl context
		 */
		void Init(void);
		/**
		 * @brief un-init the opengl element from the graphic card
		 */
		void UnInit(void);
		/**
		 * @brief When you will done an opengl rendering, you might call this reset matrix first. It remove all the stach of the matrix pushed.
		 * @param[in] newOne the default matrix that might be set for the graphic card for renderer. if too more pop will be done, this is the last that mmight survived
		 */
		void SetBasicMatrix(etk::Matrix4& newOne);
		/**
		 * @brief this funtion configure the current use matrix for the renderer (call @ref Push before, and @ref Pop when no more needed).
		 * @param[in] newOne The new current matrix use for the render.
		 * @note We did not use opengl standard system, due to the fact that is not supported in opengl ES-2
		 */
		void SetMatrix(etk::Matrix4& newOne);
		/**
		 * @brief Store current matrix in the matrix stack.
		 */
		void Push(void);
		/**
		 * @brief Remove the current matrix and get the last one from the matrix stack.
		 */
		void Pop(void);
		/**
		 * @brief Get a reference on the current matrix destinate to opengl renderer.
		 * @return The requested matrix.
		 */
		etk::Matrix4& GetMatrix(void);
	};
};

#ifdef __cplusplus
}
#endif


#endif
