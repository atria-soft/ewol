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

#ifdef __VIDEO__OPENGL_ES_2

#include <etk/Types.h>
#include <etk/File.h>
#include <ewol/Debug.h>
#include <ewol/openGL/Shader.h>



ewol::Shader::Shader(etk::UString& filename): 
	ewol::Resource(filename),
	m_fileData(NULL),
	m_shader(0),
	m_type(0)
{
	EWOL_DEBUG("OGL : load SHADER \"" << filename << "\"");
	// load data from file "all the time ..."
	
	etk::File file(m_name, etk::FILE_TYPE_DATA);
	if (false == file.Exist()) {
		EWOL_ERROR("File does not Exist : \"" << file << "\"");
		return;
	}
	
	etk::UString fileExtention = file.GetExtention();
	if (fileExtention == "frag") {
		m_type = GL_FRAGMENT_SHADER;
	} else if (fileExtention == "vert") {
		m_type = GL_VERTEX_SHADER;
	} else {
		EWOL_ERROR("File does not have extention \".vert\" for Vertex Shader or \".frag\" for Fragment Shader. but : \"" << fileExtention << "\"");
		return;
	}
	
	int32_t fileSize = file.Size();
	if (0==fileSize) {
		EWOL_ERROR("This file is empty : " << file);
		return;
	}
	if (false == file.fOpenRead()) {
		EWOL_ERROR("Can not open the file : " << file);
		return;
	}
	// allocate data
	m_fileData = new char[fileSize+5];
	if (NULL == m_fileData) {
		EWOL_ERROR("Error Memory allocation size=" << fileSize);
		return;
	}
	memset(m_fileData, 0, (fileSize+5)*sizeof(char));
	// load data from the file :
	file.fRead(m_fileData, 1, fileSize);
	// close the file:
	file.fClose();
	
	CompileAndSendShader();
	
}


ewol::Shader::~Shader(void)
{
	if (NULL != m_fileData) {
		delete [] m_fileData;
		m_fileData = NULL;
	}
	if (0!=m_shader) {
		glDeleteShader(m_shader);
		m_shader = 0;
	}
}


static void checkGlError(const char* op)
{
	for (GLint error = glGetError(); error; error = glGetError()) {
		EWOL_INFO("after " << op << "() glError (" << error << ")");
	}
}
#define LOG_OGL_INTERNAL_BUFFER_LEN    (8192)
static char l_bufferDisplayError[LOG_OGL_INTERNAL_BUFFER_LEN] = "";

bool ewol::Shader::CompileAndSendShader(void)
{
	if (NULL == m_fileData) {
		m_shader = 0;
		return false;
	}
	EWOL_DEBUG("   --> Create");
	m_shader = glCreateShader(m_type);
	EWOL_DEBUG("   <-- Create");
	if (!m_shader) {
		EWOL_ERROR("glCreateShader return error ...");
		EWOL_DEBUG("   --> checkGlError");
		checkGlError("glCreateShader");
		EWOL_DEBUG("   <-- checkGlError");
		return false;
	} else {
		EWOL_DEBUG("   --> glShaderSource");
		glShaderSource(m_shader, 1, (const char**)&m_fileData, NULL);
		EWOL_DEBUG("   <-- glShaderSource");
		EWOL_DEBUG("   --> glCompileShader");
		glCompileShader(m_shader);
		EWOL_DEBUG("   <-- glCompileShader");
		GLint compiled = 0;
		EWOL_DEBUG("   --> glGetShaderiv");
		glGetShaderiv(m_shader, GL_COMPILE_STATUS, &compiled);
		EWOL_DEBUG("   <-- glGetShaderiv");
		if (!compiled) {
			GLint infoLen = 0;
			l_bufferDisplayError[0] = '\0';
			EWOL_DEBUG("   --> glGetShaderInfoLog");
			glGetShaderInfoLog(m_shader, LOG_OGL_INTERNAL_BUFFER_LEN, &infoLen, l_bufferDisplayError);
			EWOL_DEBUG("   <-- glGetShaderInfoLog");
			const char * tmpShaderType = "GL_FRAGMENT_SHADER";
			if (m_type == GL_VERTEX_SHADER){
				tmpShaderType = "GL_VERTEX_SHADER";
			}
			EWOL_ERROR("Could not compile \"" << tmpShaderType << "\": " << l_bufferDisplayError);
			return false;
		}
	}
	return true;
}

#endif

