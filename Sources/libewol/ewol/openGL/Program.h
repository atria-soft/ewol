/**
 *******************************************************************************
 * @file ewol/openGL/Program.h
 * @brief ewol openGl Program shader system (header)
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

#ifndef __OPEN_GL__PROGRAM_H__
#define __OPEN_GL__PROGRAM_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/Resource.h>

namespace ewol
{
	Class Program : public ewol::Resource
	{
		private :
			GLuint         m_program;
			bool           m_needToReleaseShader;
		public:
			Program(etk::UString& filename);
			virtual ~Program(void);
			GLuint GetGL_ID(void) { return m_program; };
	};
};


#endif

