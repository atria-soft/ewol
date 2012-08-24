/**
 *******************************************************************************
 * @file ewol/openGL/Shader.cpp
 * @brief ewol openGl Shader system (Sources)
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

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/openGL/Shader.h>



ewol::Shader::Shader(etk::UString& filename): 
	ewol::Resource(filename),
	m_shader(0),
	m_type(0)
{
	
}

ewol::Shader::~Shader(void)
{
	
}


