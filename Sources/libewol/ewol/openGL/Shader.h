/**
 *******************************************************************************
 * @file ewol/openGL/Shader.h
 * @brief ewol openGl Shader system (header)
 * @author Edouard DUPIN
 * @date 24/08/2012
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

#ifndef __OPEN_GL__SHADER_H__
#define __OPEN_GL__SHADER_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/Resource.h>
#include <ewol/openGL/openGL.h>

namespace ewol
{
	class Shader : public ewol::Resource
	{
		private :
			GLuint         m_shader;
			GLenum         m_type;
		public:
			Shader(etk::UString& filename);
			virtual ~Shader(void);
			const char* GetType(void) { return "ewol::Shader"; };
			GLuint GetGL_ID(void) { return m_shader; };
			GLenum GetShaderType(void) { return m_type; };
	};
};


#endif

