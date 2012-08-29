/**
 *******************************************************************************
 * @file ewol/openGL/Program.cpp
 * @brief ewol openGl Program shader system (Sources)
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
#include <ewol/Debug.h>
#include <ewol/openGL/Program.h>
#include <ewol/ResourceManager.h>

ewol::Program::Program(etk::UString& filename) :
	ewol::Resource(filename),
	m_program(0),
	m_hasTexture(false)
{
	
	// load data from file "all the time ..."
	
	etk::File file(m_name, etk::FILE_TYPE_DATA);
	if (false == file.Exist()) {
		EWOL_ERROR("File does not Exist : \"" << file << "\"");
		return;
	}
	
	etk::UString fileExtention = file.GetExtention();
	if (fileExtention != "prog") {
		EWOL_ERROR("File does not have extention \".prog\" for program but : \"" << fileExtention << "\"");
		return;
	}
	if (false == file.fOpenRead()) {
		EWOL_ERROR("Can not open the file : " << file);
		return;
	}
	#define MAX_LINE_SIZE   (2048)
	char tmpData[MAX_LINE_SIZE];
	while (file.fGets(tmpData, MAX_LINE_SIZE) != NULL) {
		int32_t len = strlen(tmpData);
		if(    tmpData[len-1] == '\n'
			|| tmpData[len-1] == '\r') {
			tmpData[len-1] = '\0';
			len--;
		}
		if (len == 0) {
			continue;
		}
		etk::UString tmpFilename = tmpData;
		ewol::Shader* tmpShader = NULL;
		if (false == ewol::resource::Keep(tmpFilename, tmpShader)) {
			EWOL_CRITICAL("Error while getting a specific shader filename : " << tmpFilename);
		} else {
			EWOL_DEBUG("Add shader on program : "<< tmpFilename);
			m_shaderList.PushBack(tmpShader);
		}
		
	}
	// close the file:
	file.fClose();
	
	CreateAndLink();
	
}

ewol::Program::~Program(void)
{
	for (int32_t iii=0; iii<m_shaderList.Size(); iii++) {
		ewol::resource::Release(m_shaderList[iii]);
		m_shaderList[iii] = 0;
	}
	m_shaderList.Clear();
	if (0!=m_program) {
		glDeleteProgram(m_program);
		m_program = 0;
	}
	m_elementList.Clear();
	m_hasTexture = false;
}

static void checkGlError(const char* op)
{
	for (GLint error = glGetError(); error; error = glGetError()) {
		EWOL_INFO("after " << op << "() glError (" << error << ")");
	}
}

#define LOG_OGL_INTERNAL_BUFFER_LEN    (8192)
static char l_bufferDisplayError[LOG_OGL_INTERNAL_BUFFER_LEN] = "";

bool ewol::Program::CreateAndLink(void)
{
	EWOL_INFO("Create the Program ...");
	m_program = glCreateProgram();
	if (0 == m_program) {
		EWOL_ERROR("program creation return error ...");
		return false;
	}
	
	for (int32_t iii=0; iii<m_shaderList.Size(); iii++) {
		if (NULL != m_shaderList[iii]) {
			glAttachShader(m_program, m_shaderList[iii]->GetGL_ID());
			checkGlError("glAttachShader");
		}
	}
	glLinkProgram(m_program);
	GLint linkStatus = GL_FALSE;
	glGetProgramiv(m_program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE) {
		GLint bufLength = 0;
		l_bufferDisplayError[0] = '\0';
		glGetProgramInfoLog(m_program, LOG_OGL_INTERNAL_BUFFER_LEN, &bufLength, l_bufferDisplayError);
		EWOL_ERROR("Could not compile \"PROGRAM\": " << l_bufferDisplayError);
		glDeleteProgram(m_program);
		m_program = 0;
		return false;
	}
	return true;
}

int32_t ewol::Program::GetAttribute(etk::UString tmpElement)
{
	// check if it exist previously :
	for(int32_t iii=0; iii<m_elementList.Size(); iii++) {
		if (m_elementList[iii].m_name == tmpElement) {
			return iii;
		}
	}
	progAttributeElement tmp;
	tmp.m_name = tmpElement;
	tmp.m_isAttribute = true;
	tmp.m_elementId = glGetAttribLocation(m_program, tmp.m_name.c_str());
	if (tmp.m_elementId<0) {
		checkGlError("glGetAttribLocation");
		EWOL_INFO("glGetAttribLocation(\"" << tmp.m_name << "\") = " << tmp.m_elementId);
		return -1;
	}
	m_elementList.PushBack(tmp);
	return m_elementList.Size()-1;
}

int32_t ewol::Program::GetUniform(etk::UString tmpElement)
{
	// check if it exist previously :
	for(int32_t iii=0; iii<m_elementList.Size(); iii++) {
		if (m_elementList[iii].m_name == tmpElement) {
			return iii;
		}
	}
	progAttributeElement tmp;
	tmp.m_name = tmpElement;
	tmp.m_isAttribute = false;
	tmp.m_elementId = glGetUniformLocation(m_program, tmp.m_name.c_str());
	if (tmp.m_elementId<0) {
		checkGlError("glGetUniformLocation");
		EWOL_INFO("glGetUniformLocation(\"" << tmp.m_name << "\") = " << tmp.m_elementId);
		return -1;
	}
	m_elementList.PushBack(tmp);
	return m_elementList.Size()-1;
}

void ewol::Program::SendAttribute(int32_t idElem, int32_t nbElement, void* pointer, int32_t jumpBetweenSample)
{
	if (idElem<0 || idElem>m_elementList.Size()) {
		EWOL_ERROR("idElem = " << idElem << " not in [0.." << (m_elementList.Size()-1) << "]");
		return;
	}
	glVertexAttribPointer(m_elementList[idElem].m_elementId,  // attribute ID of OpenGL
	                      nbElement,                          // number of elements per vertex, here (r,g,b,a)
	                      GL_FLOAT,                           // the type of each element
	                      GL_FALSE,                           // take our values as-is
	                      jumpBetweenSample,                  // no extra data between each position
	                      pointer);                           // Pointer on the buffer
	glEnableVertexAttribArray(m_elementList[idElem].m_elementId);
}


void ewol::Program::SendUniformMatrix4fv(int32_t idElem, int32_t nbElement, float* pointer)
{
	if (idElem<0 || idElem>m_elementList.Size()) {
		EWOL_ERROR("idElem = " << idElem << " not in [0.." << (m_elementList.Size()-1) << "]");
		return;
	}
	glUniformMatrix4fv(m_elementList[idElem].m_elementId, nbElement, GL_TRUE, pointer);
}


void ewol::Program::Use(void)
{
	glUseProgram(m_program);
	checkGlError("glUseProgram");
}


void ewol::Program::SetTexture0(int32_t idElem, GLint textureOpenGlID)
{
	if (idElem<0 || idElem>m_elementList.Size()) {
		return;
	}
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	// set the textureID
	glBindTexture(GL_TEXTURE_2D, textureOpenGlID);
	// Set the texture on the uniform attribute
	glUniform1i(m_elementList[idElem].m_elementId, /*GL_TEXTURE*/0);
	m_hasTexture = true;
}

void ewol::Program::UnUse(void)
{
	if (true == m_hasTexture) {
		glDisable(GL_TEXTURE_2D);
		m_hasTexture = false;
	}
	glUseProgram(0);
	checkGlError("glUseProgram");
}


#endif
