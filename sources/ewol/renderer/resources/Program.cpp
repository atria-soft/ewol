/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/renderer/resources/Program.h>
#include <ewol/renderer/ResourceManager.h>
#include <etk/os/FSNode.h>
#include <ewol/ewol.h>

//#define LOCAL_DEBUG  EWOL_VERBOSE
#define LOCAL_DEBUG  EWOL_DEBUG

#undef __class__
#define __class__	"Program"

ewol::Program::Program(const etk::UString& filename) :
	ewol::Resource(filename),
	m_exist(false),
	m_program(0),
	m_hasTexture(false),
	m_hasTexture1(false)
{
	m_resourceLevel = 1;
	EWOL_DEBUG("OGL : load PROGRAM '" << m_name << "'");
	// load data from file "all the time ..."
	
	etk::FSNode file(m_name);
	if (false == file.Exist()) {
		EWOL_INFO("File does not Exist : \"" << file << "\" ==> automatic load of framment and shader with same names... ");
		etk::UString tmpFilename = m_name;
		// remove extention ...
		tmpFilename.Remove(tmpFilename.Size()-4, 4);
		ewol::Shader* tmpShader = NULL;
		if (false == ewol::resource::Keep(tmpFilename+"vert", tmpShader)) {
			EWOL_CRITICAL("Error while getting a specific shader filename : " << tmpFilename);
			return;
		} else {
			EWOL_DEBUG("Add shader on program : "<< tmpFilename);
			m_shaderList.PushBack(tmpShader);
		}
		if (false == ewol::resource::Keep(tmpFilename+"frag", tmpShader)) {
			EWOL_CRITICAL("Error while getting a specific shader filename : " << tmpFilename);
			return;
		} else {
			EWOL_DEBUG("Add shader on program : "<< tmpFilename);
			m_shaderList.PushBack(tmpShader);
		}
	} else {
		
		etk::UString fileExtention = file.FileGetExtention();
		if (fileExtention != "prog") {
			EWOL_ERROR("File does not have extention \".prog\" for program but : \"" << fileExtention << "\"");
			return;
		}
		if (false == file.FileOpenRead()) {
			EWOL_ERROR("Can not open the file : \"" << file << "\"");
			return;
		}
		#define MAX_LINE_SIZE   (2048)
		char tmpData[MAX_LINE_SIZE];
		while (file.FileGets(tmpData, MAX_LINE_SIZE) != NULL) {
			int32_t len = strlen(tmpData);
			if(    tmpData[len-1] == '\n'
				|| tmpData[len-1] == '\r') {
				tmpData[len-1] = '\0';
				len--;
			}
			EWOL_DEBUG(" Read data : \"" << tmpData << "\"");
			if (len == 0) {
				continue;
			}
			if (tmpData[0] == '#') {
				continue;
			}
			// get it with relative position :
			etk::UString tmpFilename = file.GetRelativeFolder() + tmpData;
			ewol::Shader* tmpShader = NULL;
			if (false == ewol::resource::Keep(tmpFilename, tmpShader)) {
				EWOL_CRITICAL("Error while getting a specific shader filename : " << tmpFilename);
			} else {
				EWOL_DEBUG("Add shader on program : "<< tmpFilename);
				m_shaderList.PushBack(tmpShader);
			}
			
		}
		// close the file:
		file.FileClose();
	}
	UpdateContext();
}


ewol::Program::~Program(void)
{
	for (int32_t iii=0; iii<m_shaderList.Size(); iii++) {
		ewol::resource::Release(m_shaderList[iii]);
		m_shaderList[iii] = 0;
	}
	m_shaderList.Clear();
	RemoveContext();
	m_elementList.Clear();
	m_hasTexture = false;
	m_hasTexture1 = false;
}

static void checkGlError(const char* op, int32_t localLine)
{
	for (GLint error = glGetError(); error; error = glGetError()) {
		EWOL_INFO("after " << op << "():" << localLine << " glError(" << error << ")");
	}
}

#define LOG_OGL_INTERNAL_BUFFER_LEN    (8192)
static char l_bufferDisplayError[LOG_OGL_INTERNAL_BUFFER_LEN] = "";


int32_t ewol::Program::GetAttribute(etk::UString elementName)
{
	// check if it exist previously :
	for(int32_t iii=0; iii<m_elementList.Size(); iii++) {
		if (m_elementList[iii].m_name == elementName) {
			return iii;
		}
	}
	progAttributeElement tmp;
	tmp.m_name = elementName;
	tmp.m_isAttribute = true;
	tmp.m_elementId = glGetAttribLocation(m_program, tmp.m_name.c_str());
	tmp.m_isLinked = true;
	if (tmp.m_elementId<0) {
		checkGlError("glGetAttribLocation", __LINE__);
		EWOL_WARNING("glGetAttribLocation(\"" << tmp.m_name << "\") = " << tmp.m_elementId);
		tmp.m_isLinked = false;
	}
	m_elementList.PushBack(tmp);
	return m_elementList.Size()-1;
}



int32_t ewol::Program::GetUniform(etk::UString elementName)
{
	// check if it exist previously :
	for(int32_t iii=0; iii<m_elementList.Size(); iii++) {
		if (m_elementList[iii].m_name == elementName) {
			return iii;
		}
	}
	progAttributeElement tmp;
	tmp.m_name = elementName;
	tmp.m_isAttribute = false;
	tmp.m_elementId = glGetUniformLocation(m_program, tmp.m_name.c_str());
	tmp.m_isLinked = true;
	if (tmp.m_elementId<0) {
		checkGlError("glGetUniformLocation", __LINE__);
		EWOL_WARNING("glGetUniformLocation(\"" << tmp.m_name << "\") = " << tmp.m_elementId);
		tmp.m_isLinked = false;
	}
	m_elementList.PushBack(tmp);
	return m_elementList.Size()-1;
}


void ewol::Program::UpdateContext(void)
{
	if (true==m_exist) {
		// Do nothing ==> too dangerous ...
	} else {
		// create the Shader
		EWOL_INFO("Create the Program ... \"" << m_name << "\"");
		m_program = glCreateProgram();
		if (0 == m_program) {
			EWOL_ERROR("program creation return error ...");
			checkGlError("glCreateProgram", __LINE__);
			return;
		}
		EWOL_DEBUG("Create program with oglID=" << m_program);
		// first attach vertex shader, and after fragment shader
		for (int32_t iii=0; iii<m_shaderList.Size(); iii++) {
			if (NULL != m_shaderList[iii]) {
				if (m_shaderList[iii]->GetShaderType() == GL_VERTEX_SHADER) {
					glAttachShader(m_program, m_shaderList[iii]->GetGL_ID());
					checkGlError("glAttachShader", __LINE__);
				}
			}
		}
		for (int32_t iii=0; iii<m_shaderList.Size(); iii++) {
			if (NULL != m_shaderList[iii]) {
				if (m_shaderList[iii]->GetShaderType() == GL_FRAGMENT_SHADER) {
					glAttachShader(m_program, m_shaderList[iii]->GetGL_ID());
					checkGlError("glAttachShader", __LINE__);
				}
			}
		}
		glLinkProgram(m_program);
		checkGlError("glLinkProgram", __LINE__);
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(m_program, GL_LINK_STATUS, &linkStatus);
		checkGlError("glGetProgramiv", __LINE__);
		if (linkStatus != GL_TRUE) {
			GLint bufLength = 0;
			l_bufferDisplayError[0] = '\0';
			glGetProgramInfoLog(m_program, LOG_OGL_INTERNAL_BUFFER_LEN, &bufLength, l_bufferDisplayError);
			char tmpLog[256];
			int32_t idOut=0;
			EWOL_ERROR("Could not compile \"PROGRAM\": \"" << m_name << "\"");
			for (int32_t iii=0; iii<LOG_OGL_INTERNAL_BUFFER_LEN ; iii++) {
				tmpLog[idOut] = l_bufferDisplayError[iii];
				if (tmpLog[idOut] == '\n' || tmpLog[idOut] == '\0' || idOut>=256) {
					tmpLog[idOut] = '\0';
					EWOL_ERROR("    ==> " << tmpLog);
					idOut=0;
				} else {
					idOut++;
				}
				if (l_bufferDisplayError[iii] == '\0') {
					break;
				}
			}
			if (idOut != 0) {
				tmpLog[idOut] = '\0';
				EWOL_ERROR("    ==> " << tmpLog);
			}
			glDeleteProgram(m_program);
			checkGlError("glDeleteProgram", __LINE__);
			m_program = 0;
			return;
		}
		m_exist = true;
		// now get the old attribute requested priviously ...
		for(int32_t iii=0; iii<m_elementList.Size(); iii++) {
			if (true==m_elementList[iii].m_isAttribute) {
				m_elementList[iii].m_elementId = glGetAttribLocation(m_program, m_elementList[iii].m_name.c_str());
				m_elementList[iii].m_isLinked = true;
				if (m_elementList[iii].m_elementId<0) {
					m_elementList[iii].m_isLinked = false;
					checkGlError("glGetAttribLocation", __LINE__);
					EWOL_WARNING("glGetAttribLocation(\"" << m_elementList[iii].m_name << "\") = " << m_elementList[iii].m_elementId);
				}
			} else {
				m_elementList[iii].m_elementId = glGetUniformLocation(m_program, m_elementList[iii].m_name.c_str());
				m_elementList[iii].m_isLinked = true;
				if (m_elementList[iii].m_elementId<0) {
					m_elementList[iii].m_isLinked = false;
					checkGlError("glGetUniformLocation", __LINE__);
					EWOL_WARNING("glGetUniformLocation(\"" << m_elementList[iii].m_name << "\") = " << m_elementList[iii].m_elementId);
				}
			}
		}
	}
}

void ewol::Program::RemoveContext(void)
{
	if (true==m_exist) {
		glDeleteProgram(m_program);
		m_program = 0;
		m_exist = false;
		for(int32_t iii=0; iii<m_elementList.Size(); iii++) {
			m_elementList[iii].m_elementId=0;
		}
	}
}

void ewol::Program::RemoveContextToLate(void)
{
	m_exist = false;
	m_program = 0;
}

void ewol::Program::Reload(void)
{
/* TODO : ...
	etk::File file(m_name, etk::FILE_TYPE_DATA);
	if (false == file.Exist()) {
		EWOL_ERROR("File does not Exist : \"" << file << "\"");
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
	// Remove previous data ...
	if (NULL != m_fileData) {
		delete[] m_fileData;
		m_fileData = 0;
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
*/
	// now change the OGL context ...
	RemoveContext();
	UpdateContext();
}

//////////////////////////////////////////////////////////////////////////////////////////////

void ewol::Program::SendAttribute(int32_t idElem, int32_t nbElement, void* pointer, int32_t jumpBetweenSample)
{
	if (0==m_program) {
		return;
	}
	if (idElem<0 || idElem>m_elementList.Size()) {
		EWOL_ERROR("idElem = " << idElem << " not in [0.." << (m_elementList.Size()-1) << "]");
		return;
	}
	if (false == m_elementList[idElem].m_isLinked) {
		return;
	}
	glVertexAttribPointer(m_elementList[idElem].m_elementId,  // attribute ID of OpenGL
	                      nbElement,                          // number of elements per vertex, here (r,g,b,a)
	                      GL_FLOAT,                           // the type of each element
	                      GL_FALSE,                           // take our values as-is
	                      jumpBetweenSample,                  // no extra data between each position
	                      pointer);                           // Pointer on the buffer
	//checkGlError("glVertexAttribPointer", __LINE__);
	glEnableVertexAttribArray(m_elementList[idElem].m_elementId);
	//checkGlError("glEnableVertexAttribArray", __LINE__);
}

void ewol::Program::SendAttributePointer(int32_t idElem, int32_t nbElement, ewol::VirtualBufferObject* vbo, int32_t index, int32_t jumpBetweenSample, int32_t offset)
{
	if (0==m_program) {
		return;
	}
	if (idElem<0 || idElem>m_elementList.Size()) {
		EWOL_ERROR("idElem = " << idElem << " not in [0.." << (m_elementList.Size()-1) << "]");
		return;
	}
	if (false == m_elementList[idElem].m_isLinked) {
		return;
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo->GetGL_ID(index));
	glVertexAttribPointer(m_elementList[idElem].m_elementId,  // attribute ID of OpenGL
	                      nbElement,                          // number of elements per vertex, here (r,g,b,a)
	                      GL_FLOAT,                           // the type of each element
	                      GL_FALSE,                           // take our values as-is
	                      jumpBetweenSample,                  // no extra data between each position
	                      (GLvoid *)offset);                            // Pointer on the buffer
	//checkGlError("glVertexAttribPointer", __LINE__);
	glEnableVertexAttribArray(m_elementList[idElem].m_elementId);
	//checkGlError("glEnableVertexAttribArray", __LINE__);
}

//////////////////////////////////////////////////////////////////////////////////////////////

void ewol::Program::UniformMatrix4fv(int32_t idElem, int32_t nbElement, mat4 _matrix, bool transpose)
{
	if (0==m_program) {
		return;
	}
	if (idElem<0 || idElem>m_elementList.Size()) {
		EWOL_ERROR("idElem = " << idElem << " not in [0.." << (m_elementList.Size()-1) << "]");
		return;
	}
	if (false == m_elementList[idElem].m_isLinked) {
		return;
	}
	// note : Android des not supported the transposition of the matrix, then we will done it oursef:
	if (true == transpose) {
		_matrix.Transpose();
	}
	glUniformMatrix4fv(m_elementList[idElem].m_elementId, nbElement, GL_FALSE, _matrix.m_mat);
	//checkGlError("glUniformMatrix4fv", __LINE__);
}

//////////////////////////////////////////////////////////////////////////////////////////////

void ewol::Program::Uniform1f(int32_t idElem, float value1)
{
	if (0==m_program) {
		return;
	}
	if (idElem<0 || idElem>m_elementList.Size()) {
		EWOL_ERROR("idElem = " << idElem << " not in [0.." << (m_elementList.Size()-1) << "]");
		return;
	}
	if (false == m_elementList[idElem].m_isLinked) {
		return;
	}
	glUniform1f(m_elementList[idElem].m_elementId, value1);
	//checkGlError("glUniform1f", __LINE__);
}
void ewol::Program::Uniform2f(int32_t idElem, float value1, float value2)
{
	if (0==m_program) {
		return;
	}
	if (idElem<0 || idElem>m_elementList.Size()) {
		EWOL_ERROR("idElem = " << idElem << " not in [0.." << (m_elementList.Size()-1) << "]");
		return;
	}
	if (false == m_elementList[idElem].m_isLinked) {
		return;
	}
	glUniform2f(m_elementList[idElem].m_elementId, value1, value2);
	//checkGlError("glUniform2f", __LINE__);
}
void ewol::Program::Uniform3f(int32_t idElem, float value1, float value2, float value3)
{
	if (0==m_program) {
		return;
	}
	if (idElem<0 || idElem>m_elementList.Size()) {
		EWOL_ERROR("idElem = " << idElem << " not in [0.." << (m_elementList.Size()-1) << "]");
		return;
	}
	if (false == m_elementList[idElem].m_isLinked) {
		return;
	}
	glUniform3f(m_elementList[idElem].m_elementId, value1, value2, value3);
	//checkGlError("glUniform3f", __LINE__);
}
void ewol::Program::Uniform4f(int32_t idElem, float value1, float value2, float value3, float value4)
{
	if (0==m_program) {
		return;
	}
	if (idElem<0 || idElem>m_elementList.Size()) {
		EWOL_ERROR("idElem = " << idElem << " not in [0.." << (m_elementList.Size()-1) << "]");
		return;
	}
	if (false == m_elementList[idElem].m_isLinked) {
		return;
	}
	glUniform4f(m_elementList[idElem].m_elementId, value1, value2, value3, value4);
	//checkGlError("glUniform4f", __LINE__);
}

//////////////////////////////////////////////////////////////////////////////////////////////

void ewol::Program::Uniform1i(int32_t idElem, int32_t value1)
{
	if (0==m_program) {
		return;
	}
	if (idElem<0 || idElem>m_elementList.Size()) {
		EWOL_ERROR("idElem = " << idElem << " not in [0.." << (m_elementList.Size()-1) << "]");
		return;
	}
	if (false == m_elementList[idElem].m_isLinked) {
		return;
	}
	glUniform1i(m_elementList[idElem].m_elementId, value1);
	//checkGlError("glUniform1i", __LINE__);
}
void ewol::Program::Uniform2i(int32_t idElem, int32_t value1, int32_t value2)
{
	if (0==m_program) {
		return;
	}
	if (idElem<0 || idElem>m_elementList.Size()) {
		EWOL_ERROR("idElem = " << idElem << " not in [0.." << (m_elementList.Size()-1) << "]");
		return;
	}
	if (false == m_elementList[idElem].m_isLinked) {
		return;
	}
	glUniform2i(m_elementList[idElem].m_elementId, value1, value2);
	//checkGlError("glUniform2i", __LINE__);
}
void ewol::Program::Uniform3i(int32_t idElem, int32_t value1, int32_t value2, int32_t value3)
{
	if (0==m_program) {
		return;
	}
	if (idElem<0 || idElem>m_elementList.Size()) {
		EWOL_ERROR("idElem = " << idElem << " not in [0.." << (m_elementList.Size()-1) << "]");
		return;
	}
	if (false == m_elementList[idElem].m_isLinked) {
		return;
	}
	glUniform3i(m_elementList[idElem].m_elementId, value1, value2, value3);
	//checkGlError("glUniform3i", __LINE__);
}
void ewol::Program::Uniform4i(int32_t idElem, int32_t value1, int32_t value2, int32_t value3, int32_t value4)
{
	if (0==m_program) {
		return;
	}
	if (idElem<0 || idElem>m_elementList.Size()) {
		EWOL_ERROR("idElem = " << idElem << " not in [0.." << (m_elementList.Size()-1) << "]");
		return;
	}
	if (false == m_elementList[idElem].m_isLinked) {
		return;
	}
	glUniform4i(m_elementList[idElem].m_elementId, value1, value2, value3, value4);
	//checkGlError("glUniform4i", __LINE__);
}


//////////////////////////////////////////////////////////////////////////////////////////////

void ewol::Program::Uniform1fv(int32_t idElem, int32_t nbElement, const float *value)
{
	if (0==m_program) {
		return;
	}
	if (idElem<0 || idElem>m_elementList.Size()) {
		EWOL_ERROR("idElem = " << idElem << " not in [0.." << (m_elementList.Size()-1) << "]");
		return;
	}
	if (false == m_elementList[idElem].m_isLinked) {
		return;
	}
	if (0==nbElement) {
		EWOL_ERROR("No element to send at open GL ...");
		return;
	}
	if (NULL==value) {
		EWOL_ERROR("NULL Input pointer to send at open GL ...");
		return;
	}
	glUniform1fv(m_elementList[idElem].m_elementId, nbElement, value);
	//checkGlError("glUniform1fv", __LINE__);
}
void ewol::Program::Uniform2fv(int32_t idElem, int32_t nbElement, const float *value)
{
	if (0==m_program) {
		return;
	}
	if (idElem<0 || idElem>m_elementList.Size()) {
		EWOL_ERROR("idElem = " << idElem << " not in [0.." << (m_elementList.Size()-1) << "]");
		return;
	}
	if (false == m_elementList[idElem].m_isLinked) {
		return;
	}
	if (0==nbElement) {
		EWOL_ERROR("No element to send at open GL ...");
		return;
	}
	if (NULL==value) {
		EWOL_ERROR("NULL Input pointer to send at open GL ...");
		return;
	}
	glUniform2fv(m_elementList[idElem].m_elementId, nbElement, value);
	//checkGlError("glUniform2fv", __LINE__);
}
void ewol::Program::Uniform3fv(int32_t idElem, int32_t nbElement, const float *value)
{
	if (0==m_program) {
		return;
	}
	if (idElem<0 || idElem>m_elementList.Size()) {
		EWOL_ERROR("idElem = " << idElem << " not in [0.." << (m_elementList.Size()-1) << "]");
		return;
	}
	if (false == m_elementList[idElem].m_isLinked) {
		return;
	}
	if (0==nbElement) {
		EWOL_ERROR("No element to send at open GL ...");
		return;
	}
	if (NULL==value) {
		EWOL_ERROR("NULL Input pointer to send at open GL ...");
		return;
	}
	glUniform3fv(m_elementList[idElem].m_elementId, nbElement, value);
	//checkGlError("glUniform3fv", __LINE__);
}
void ewol::Program::Uniform4fv(int32_t idElem, int32_t nbElement, const float *value)
{
	if (0==m_program) {
		return;
	}
	if (idElem<0 || idElem>m_elementList.Size()) {
		EWOL_ERROR("idElem = " << idElem << " not in [0.." << (m_elementList.Size()-1) << "]");
		return;
	}
	if (false == m_elementList[idElem].m_isLinked) {
		return;
	}
	if (0==nbElement) {
		EWOL_ERROR("No element to send at open GL ...");
		return;
	}
	if (NULL==value) {
		EWOL_ERROR("NULL Input pointer to send at open GL ...");
		return;
	}
	glUniform4fv(m_elementList[idElem].m_elementId, nbElement, value);
	//checkGlError("glUniform4fv", __LINE__);
}

//////////////////////////////////////////////////////////////////////////////////////////////

void ewol::Program::Uniform1iv(int32_t idElem, int32_t nbElement, const int32_t *value)
{
	if (0==m_program) {
		return;
	}
	if (idElem<0 || idElem>m_elementList.Size()) {
		EWOL_ERROR("idElem = " << idElem << " not in [0.." << (m_elementList.Size()-1) << "]");
		return;
	}
	if (false == m_elementList[idElem].m_isLinked) {
		return;
	}
	if (0==nbElement) {
		EWOL_ERROR("No element to send at open GL ...");
		return;
	}
	if (NULL==value) {
		EWOL_ERROR("NULL Input pointer to send at open GL ...");
		return;
	}
	glUniform1iv(m_elementList[idElem].m_elementId, nbElement, value);
	//checkGlError("glUniform1iv", __LINE__);
}
void ewol::Program::Uniform2iv(int32_t idElem, int32_t nbElement, const int32_t *value)
{
	if (0==m_program) {
		return;
	}
	if (idElem<0 || idElem>m_elementList.Size()) {
		EWOL_ERROR("idElem = " << idElem << " not in [0.." << (m_elementList.Size()-1) << "]");
		return;
	}
	if (false == m_elementList[idElem].m_isLinked) {
		return;
	}
	if (0==nbElement) {
		EWOL_ERROR("No element to send at open GL ...");
		return;
	}
	if (NULL==value) {
		EWOL_ERROR("NULL Input pointer to send at open GL ...");
		return;
	}
	glUniform2iv(m_elementList[idElem].m_elementId, nbElement, value);
	//checkGlError("glUniform2iv", __LINE__);
}
void ewol::Program::Uniform3iv(int32_t idElem, int32_t nbElement, const int32_t *value)
{
	if (0==m_program) {
		return;
	}
	if (idElem<0 || idElem>m_elementList.Size()) {
		EWOL_ERROR("idElem = " << idElem << " not in [0.." << (m_elementList.Size()-1) << "]");
		return;
	}
	if (false == m_elementList[idElem].m_isLinked) {
		return;
	}
	if (0==nbElement) {
		EWOL_ERROR("No element to send at open GL ...");
		return;
	}
	if (NULL==value) {
		EWOL_ERROR("NULL Input pointer to send at open GL ...");
		return;
	}
	glUniform3iv(m_elementList[idElem].m_elementId, nbElement, value);
	//checkGlError("glUniform3iv", __LINE__);
}
void ewol::Program::Uniform4iv(int32_t idElem, int32_t nbElement, const int32_t *value)
{
	if (0==m_program) {
		return;
	}
	if (idElem<0 || idElem>m_elementList.Size()) {
		EWOL_ERROR("idElem = " << idElem << " not in [0.." << (m_elementList.Size()-1) << "]");
		return;
	}
	if (false == m_elementList[idElem].m_isLinked) {
		return;
	}
	if (0==nbElement) {
		EWOL_ERROR("No element to send at open GL ...");
		return;
	}
	if (NULL==value) {
		EWOL_ERROR("NULL Input pointer to send at open GL ...");
		return;
	}
	glUniform4iv(m_elementList[idElem].m_elementId, nbElement, value);
	//checkGlError("glUniform4iv", __LINE__);
}

//#define PROGRAM_DISPLAY_SPEED

//////////////////////////////////////////////////////////////////////////////////////////////
#ifdef PROGRAM_DISPLAY_SPEED
	int64_t g_startTime = 0;
#endif


void ewol::Program::Use(void)
{
	#ifdef PROGRAM_DISPLAY_SPEED
		g_startTime = ewol::GetTime();
	#endif
	// event if it was 0 ==> set it to prevent other use of the previous shader display ...
	ewol::openGL::UseProgram(m_program);
	//checkGlError("glUseProgram", __LINE__);
}


void ewol::Program::SetTexture0(int32_t idElem, GLint textureOpenGlID)
{
	if (0==m_program) {
		return;
	}
	if (idElem<0 || idElem>m_elementList.Size()) {
		return;
	}
	if (false == m_elementList[idElem].m_isLinked) {
		return;
	}
	#if 0
		ewol::openGL::Enable(GL_TEXTURE_2D);
		checkGlError("glEnable", __LINE__);
	#endif
	ewol::openGL::ActiveTexture(GL_TEXTURE0);
	//checkGlError("glActiveTexture", __LINE__);
	// set the textureID
	glBindTexture(GL_TEXTURE_2D, textureOpenGlID);
	//checkGlError("glBindTexture", __LINE__);
	// Set the texture on the uniform attribute
	glUniform1i(m_elementList[idElem].m_elementId, /*GL_TEXTURE*/0);
	//checkGlError("glUniform1i", __LINE__);
	m_hasTexture = true;
}

void ewol::Program::SetTexture1(int32_t idElem, GLint textureOpenGlID)
{
	if (0==m_program) {
		return;
	}
	if (idElem<0 || idElem>m_elementList.Size()) {
		return;
	}
	if (false == m_elementList[idElem].m_isLinked) {
		return;
	}
	#if 0
		ewol::openGL::Enable(GL_TEXTURE_2D);
		checkGlError("glEnable", __LINE__);
	#endif
	ewol::openGL::ActiveTexture(GL_TEXTURE1);
	//checkGlError("glActiveTexture", __LINE__);
	// set the textureID
	glBindTexture(GL_TEXTURE_2D, textureOpenGlID);
	//checkGlError("glBindTexture", __LINE__);
	// Set the texture on the uniform attribute
	glUniform1i(m_elementList[idElem].m_elementId, /*GL_TEXTURE*/1);
	//checkGlError("glUniform1i", __LINE__);
	m_hasTexture1 = true;
}


void ewol::Program::UnUse(void)
{
	if (0==m_program) {
		return;
	}
	#if 0
	if (true == m_hasTexture) {
		ewol::openGL::Disable(GL_TEXTURE_2D);
		//checkGlError("glDisable", __LINE__);
		m_hasTexture = false;
	}
	#endif
	// no need to disable program ==> this only generate perturbation on speed ...
	ewol::openGL::UseProgram(-1);
	#ifdef PROGRAM_DISPLAY_SPEED
		float localTime = (float)(ewol::GetTime() - g_startTime) / 1000.0f;
		if (localTime>1) {
			EWOL_ERROR("      prog : " << localTime << "ms    resource=\"" << m_name << "\"");
		} else {
			EWOL_DEBUG("      prog : " << localTime << "ms    resource=\"" << m_name << "\"");
		}
	#endif
	//checkGlError("glUseProgram", __LINE__);
}


