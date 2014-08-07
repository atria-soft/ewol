/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/resource/Program.h>
#include <ewol/resource/Manager.h>
#include <etk/os/FSNode.h>
#include <ewol/ewol.h>

//#define LOCAL_DEBUG  EWOL_VERBOSE
#define LOCAL_DEBUG  EWOL_DEBUG

#undef __class__
#define __class__ "resource::Program"

ewol::resource::Program::Program() :
  ewol::Resource(),
  m_exist(false),
  m_program(0),
  m_hasTexture(false),
  m_hasTexture1(false) {
	addObjectType("ewol::resource::Program");
	m_resourceLevel = 1;
}

void ewol::resource::Program::init(const std::string& _filename) {
	ewol::Resource::init(_filename);
	EWOL_DEBUG("OGL : load PROGRAM '" << m_name << "'");
	// load data from file "all the time ..."
	
	etk::FSNode file(m_name);
	if (false == file.exist()) {
		EWOL_INFO("File does not Exist : \"" << file << "\"  == > automatic load of framment and shader with same names... ");
		std::string tmpFilename = m_name;
		// remove extention ...
		tmpFilename.erase(tmpFilename.size()-4, 4);
		std::shared_ptr<ewol::resource::Shader> tmpShader = ewol::resource::Shader::create(tmpFilename+"vert");
		if (nullptr == tmpShader) {
			EWOL_CRITICAL("Error while getting a specific shader filename : " << tmpFilename);
			return;
		} else {
			EWOL_DEBUG("Add shader on program : "<< tmpFilename << "vert");
			m_shaderList.push_back(tmpShader);
		}
		tmpShader = ewol::resource::Shader::create(tmpFilename+"frag");
		if (nullptr == tmpShader) {
			EWOL_CRITICAL("Error while getting a specific shader filename : " << tmpFilename);
			return;
		} else {
			EWOL_DEBUG("Add shader on program : "<< tmpFilename << "frag");
			m_shaderList.push_back(tmpShader);
		}
	} else {
		std::string fileExtention = file.fileGetExtention();
		if (fileExtention != "prog") {
			EWOL_ERROR("File does not have extention \".prog\" for program but : \"" << fileExtention << "\"");
			return;
		}
		if (false == file.fileOpenRead()) {
			EWOL_ERROR("Can not open the file : \"" << file << "\"");
			return;
		}
		#define MAX_LINE_SIZE   (2048)
		char tmpData[MAX_LINE_SIZE];
		while (file.fileGets(tmpData, MAX_LINE_SIZE) != nullptr) {
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
			std::string tmpFilename = file.getRelativeFolder() + tmpData;
			std::shared_ptr<ewol::resource::Shader> tmpShader = ewol::resource::Shader::create(tmpFilename);
			if (nullptr == tmpShader) {
				EWOL_CRITICAL("Error while getting a specific shader filename : " << tmpFilename);
			} else {
				EWOL_DEBUG("Add shader on program : "<< tmpFilename);
				m_shaderList.push_back(tmpShader);
			}
			
		}
		// close the file:
		file.fileClose();
	}
	updateContext();
}

ewol::resource::Program::~Program() {
	m_shaderList.clear();
	removeContext();
	m_elementList.clear();
	m_hasTexture = false;
	m_hasTexture1 = false;
}

static void checkGlError(const char* _op, int32_t _localLine) {
	for (GLint error = glGetError(); error; error = glGetError()) {
		EWOL_INFO("after " << _op << "():" << _localLine << " glError(" << error << ")");
	}
}

#define LOG_OGL_INTERNAL_BUFFER_LEN    (8192)
static char l_bufferDisplayError[LOG_OGL_INTERNAL_BUFFER_LEN] = "";

int32_t ewol::resource::Program::getAttribute(std::string _elementName) {
	// check if it exist previously :
	for(size_t iii=0; iii<m_elementList.size(); iii++) {
		if (m_elementList[iii].m_name == _elementName) {
			return iii;
		}
	}
	progAttributeElement tmp;
	tmp.m_name = _elementName;
	tmp.m_isAttribute = true;
	tmp.m_elementId = glGetAttribLocation(m_program, tmp.m_name.c_str());
	tmp.m_isLinked = true;
	if (tmp.m_elementId<0) {
		checkGlError("glGetAttribLocation", __LINE__);
		EWOL_WARNING("glGetAttribLocation(\"" << tmp.m_name << "\") = " << tmp.m_elementId);
		tmp.m_isLinked = false;
	}
	m_elementList.push_back(tmp);
	return m_elementList.size()-1;
}

int32_t ewol::resource::Program::getUniform(std::string _elementName) {
	// check if it exist previously :
	for(size_t iii=0; iii<m_elementList.size(); iii++) {
		if (m_elementList[iii].m_name == _elementName) {
			return iii;
		}
	}
	progAttributeElement tmp;
	tmp.m_name = _elementName;
	tmp.m_isAttribute = false;
	tmp.m_elementId = glGetUniformLocation(m_program, tmp.m_name.c_str());
	tmp.m_isLinked = true;
	if (tmp.m_elementId<0) {
		checkGlError("glGetUniformLocation", __LINE__);
		EWOL_WARNING("glGetUniformLocation(\"" << tmp.m_name << "\") = " << tmp.m_elementId);
		tmp.m_isLinked = false;
	}
	m_elementList.push_back(tmp);
	return m_elementList.size()-1;
}

void ewol::resource::Program::updateContext() {
	if (true == m_exist) {
		// Do nothing  == > too dangerous ...
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
		for (size_t iii=0; iii<m_shaderList.size(); iii++) {
			if (nullptr != m_shaderList[iii]) {
				if (m_shaderList[iii]->getShaderType() == GL_VERTEX_SHADER) {
					glAttachShader(m_program, m_shaderList[iii]->getGL_ID());
					checkGlError("glAttachShader", __LINE__);
				}
			}
		}
		for (size_t iii=0; iii<m_shaderList.size(); iii++) {
			if (nullptr != m_shaderList[iii]) {
				if (m_shaderList[iii]->getShaderType() == GL_FRAGMENT_SHADER) {
					glAttachShader(m_program, m_shaderList[iii]->getGL_ID());
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
			for (size_t iii=0; iii<LOG_OGL_INTERNAL_BUFFER_LEN ; iii++) {
				tmpLog[idOut] = l_bufferDisplayError[iii];
				if (tmpLog[idOut] == '\n' || tmpLog[idOut] == '\0' || idOut >= 256) {
					tmpLog[idOut] = '\0';
					EWOL_ERROR("     == > " << tmpLog);
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
				EWOL_ERROR("     == > " << tmpLog);
			}
			glDeleteProgram(m_program);
			checkGlError("glDeleteProgram", __LINE__);
			m_program = 0;
			return;
		}
		m_exist = true;
		// now get the old attribute requested priviously ...
		for(size_t iii=0; iii<m_elementList.size(); iii++) {
			if (true == m_elementList[iii].m_isAttribute) {
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

void ewol::resource::Program::removeContext() {
	if (true == m_exist) {
		glDeleteProgram(m_program);
		m_program = 0;
		m_exist = false;
		for(size_t iii=0; iii<m_elementList.size(); iii++) {
			m_elementList[iii].m_elementId=0;
		}
	}
}

void ewol::resource::Program::removeContextToLate() {
	m_exist = false;
	m_program = 0;
}

void ewol::resource::Program::reload() {
/* TODO : ...
	etk::file file(m_name, etk::FILE_TYPE_DATA);
	if (false == file.Exist()) {
		EWOL_ERROR("File does not Exist : \"" << file << "\"");
		return;
	}
	
	int32_t fileSize = file.size();
	if (0 == fileSize) {
		EWOL_ERROR("This file is empty : " << file);
		return;
	}
	if (false == file.fOpenRead()) {
		EWOL_ERROR("Can not open the file : " << file);
		return;
	}
	// remove previous data ...
	if (nullptr != m_fileData) {
		delete[] m_fileData;
		m_fileData = 0;
	}
	// allocate data
	m_fileData = new char[fileSize+5];
	if (nullptr == m_fileData) {
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
	removeContext();
	updateContext();
}

//////////////////////////////////////////////////////////////////////////////////////////////

void ewol::resource::Program::sendAttribute(int32_t _idElem,
                                            int32_t _nbElement,
                                            const void* _pointer,
                                            int32_t _jumpBetweenSample) {
	if (0 == m_program) {
		return;
	}
	if (    _idElem < 0
	     || (size_t)_idElem > m_elementList.size()) {
		EWOL_ERROR("idElem = " << _idElem << " not in [0.." << (m_elementList.size()-1) << "]");
		return;
	}
	if (m_elementList[_idElem].m_isLinked == false) {
		return;
	}
	glVertexAttribPointer(m_elementList[_idElem].m_elementId, // attribute ID of openGL
	                      _nbElement, // number of elements per vertex, here (r,g,b,a)
	                      GL_FLOAT, // the type of each element
	                      GL_FALSE, // take our values as-is
	                      _jumpBetweenSample, // no extra data between each position
	                      _pointer); // Pointer on the buffer
	//checkGlError("glVertexAttribPointer", __LINE__);
	glEnableVertexAttribArray(m_elementList[_idElem].m_elementId);
	//checkGlError("glEnableVertexAttribArray", __LINE__);
}

void ewol::resource::Program::sendAttributePointer(int32_t _idElem,
                                                   int32_t _nbElement,
                                                   const std::shared_ptr<ewol::resource::VirtualBufferObject>& _vbo,
                                                   int32_t _index,
                                                   int32_t _jumpBetweenSample,
                                                   int32_t _offset) {
	if (0 == m_program) {
		return;
	}
	if (_idElem<0 || (size_t)_idElem>m_elementList.size()) {
		EWOL_ERROR("idElem = " << _idElem << " not in [0.." << (m_elementList.size()-1) << "]");
		return;
	}
	if (false == m_elementList[_idElem].m_isLinked) {
		return;
	}
	glBindBuffer(GL_ARRAY_BUFFER, _vbo->getGL_ID(_index));
	glVertexAttribPointer(m_elementList[_idElem].m_elementId, // attribute ID of openGL
	                      _nbElement, // number of elements per vertex, here (r,g,b,a)
	                      GL_FLOAT, // the type of each element
	                      GL_FALSE, // take our values as-is
	                      _jumpBetweenSample, // no extra data between each position
	                      (GLvoid *)_offset); // Pointer on the buffer
	//checkGlError("glVertexAttribPointer", __LINE__);
	glEnableVertexAttribArray(m_elementList[_idElem].m_elementId);
	//checkGlError("glEnableVertexAttribArray", __LINE__);
}

//////////////////////////////////////////////////////////////////////////////////////////////

void ewol::resource::Program::uniformMatrix4fv(int32_t _idElem, int32_t _nbElement, mat4 _matrix, bool _transpose) {
	if (0 == m_program) {
		return;
	}
	if (_idElem<0 || (size_t)_idElem>m_elementList.size()) {
		EWOL_ERROR("idElem = " << _idElem << " not in [0.." << (m_elementList.size()-1) << "]");
		return;
	}
	if (false == m_elementList[_idElem].m_isLinked) {
		return;
	}
	// note : Android des not supported the transposition of the matrix, then we will done it oursef:
	if (true == _transpose) {
		_matrix.transpose();
	}
	glUniformMatrix4fv(m_elementList[_idElem].m_elementId, _nbElement, GL_FALSE, _matrix.m_mat);
	//checkGlError("glUniformMatrix4fv", __LINE__);
}

//////////////////////////////////////////////////////////////////////////////////////////////

void ewol::resource::Program::uniform1f(int32_t _idElem, float _value1) {
	if (0 == m_program) {
		return;
	}
	if (_idElem<0 || (size_t)_idElem>m_elementList.size()) {
		EWOL_ERROR("idElem = " << _idElem << " not in [0.." << (m_elementList.size()-1) << "]");
		return;
	}
	if (false == m_elementList[_idElem].m_isLinked) {
		return;
	}
	glUniform1f(m_elementList[_idElem].m_elementId, _value1);
	//checkGlError("glUniform1f", __LINE__);
}
void ewol::resource::Program::uniform2f(int32_t _idElem, float  _value1, float _value2) {
	if (0 == m_program) {
		return;
	}
	if (_idElem<0 || (size_t)_idElem>m_elementList.size()) {
		EWOL_ERROR("idElem = " << _idElem << " not in [0.." << (m_elementList.size()-1) << "]");
		return;
	}
	if (false == m_elementList[_idElem].m_isLinked) {
		return;
	}
	glUniform2f(m_elementList[_idElem].m_elementId, _value1, _value2);
	//checkGlError("glUniform2f", __LINE__);
}
void ewol::resource::Program::uniform3f(int32_t _idElem, float _value1, float _value2, float _value3) {
	if (0 == m_program) {
		return;
	}
	if (_idElem<0 || (size_t)_idElem>m_elementList.size()) {
		EWOL_ERROR("idElem = " << _idElem << " not in [0.." << (m_elementList.size()-1) << "]");
		return;
	}
	if (false == m_elementList[_idElem].m_isLinked) {
		return;
	}
	glUniform3f(m_elementList[_idElem].m_elementId, _value1, _value2, _value3);
	//checkGlError("glUniform3f", __LINE__);
}
void ewol::resource::Program::uniform4f(int32_t _idElem, float _value1, float _value2, float _value3, float _value4) {
	if (0 == m_program) {
		return;
	}
	if (_idElem<0 || (size_t)_idElem>m_elementList.size()) {
		EWOL_ERROR("idElem = " << _idElem << " not in [0.." << (m_elementList.size()-1) << "]");
		return;
	}
	if (false == m_elementList[_idElem].m_isLinked) {
		return;
	}
	glUniform4f(m_elementList[_idElem].m_elementId, _value1, _value2, _value3, _value4);
	//checkGlError("glUniform4f", __LINE__);
}

//////////////////////////////////////////////////////////////////////////////////////////////

void ewol::resource::Program::uniform1i(int32_t _idElem, int32_t _value1) {
	if (0 == m_program) {
		return;
	}
	if (_idElem<0 || (size_t)_idElem>m_elementList.size()) {
		EWOL_ERROR("idElem = " << _idElem << " not in [0.." << (m_elementList.size()-1) << "]");
		return;
	}
	if (false == m_elementList[_idElem].m_isLinked) {
		return;
	}
	glUniform1i(m_elementList[_idElem].m_elementId, _value1);
	//checkGlError("glUniform1i", __LINE__);
}
void ewol::resource::Program::uniform2i(int32_t _idElem, int32_t _value1, int32_t _value2) {
	if (0 == m_program) {
		return;
	}
	if (_idElem<0 || (size_t)_idElem>m_elementList.size()) {
		EWOL_ERROR("idElem = " << _idElem << " not in [0.." << (m_elementList.size()-1) << "]");
		return;
	}
	if (false == m_elementList[_idElem].m_isLinked) {
		return;
	}
	glUniform2i(m_elementList[_idElem].m_elementId, _value1, _value2);
	//checkGlError("glUniform2i", __LINE__);
}
void ewol::resource::Program::uniform3i(int32_t _idElem, int32_t _value1, int32_t _value2, int32_t _value3) {
	if (0 == m_program) {
		return;
	}
	if (_idElem<0 || (size_t)_idElem>m_elementList.size()) {
		EWOL_ERROR("idElem = " << _idElem << " not in [0.." << (m_elementList.size()-1) << "]");
		return;
	}
	if (false == m_elementList[_idElem].m_isLinked) {
		return;
	}
	glUniform3i(m_elementList[_idElem].m_elementId, _value1, _value2, _value3);
	//checkGlError("glUniform3i", __LINE__);
}
void ewol::resource::Program::uniform4i(int32_t _idElem, int32_t _value1, int32_t _value2, int32_t _value3, int32_t _value4) {
	if (0 == m_program) {
		return;
	}
	if (_idElem<0 || (size_t)_idElem>m_elementList.size()) {
		EWOL_ERROR("idElem = " << _idElem << " not in [0.." << (m_elementList.size()-1) << "]");
		return;
	}
	if (false == m_elementList[_idElem].m_isLinked) {
		return;
	}
	glUniform4i(m_elementList[_idElem].m_elementId, _value1, _value2, _value3, _value4);
	//checkGlError("glUniform4i", __LINE__);
}


//////////////////////////////////////////////////////////////////////////////////////////////

void ewol::resource::Program::uniform1fv(int32_t _idElem, int32_t _nbElement, const float *_value) {
	if (0 == m_program) {
		return;
	}
	if (_idElem<0 || (size_t)_idElem>m_elementList.size()) {
		EWOL_ERROR("idElem = " << _idElem << " not in [0.." << (m_elementList.size()-1) << "]");
		return;
	}
	if (false == m_elementList[_idElem].m_isLinked) {
		return;
	}
	if (0 == _nbElement) {
		EWOL_ERROR("No element to send at open GL ...");
		return;
	}
	if (nullptr == _value) {
		EWOL_ERROR("nullptr Input pointer to send at open GL ...");
		return;
	}
	glUniform1fv(m_elementList[_idElem].m_elementId, _nbElement, _value);
	//checkGlError("glUniform1fv", __LINE__);
}
void ewol::resource::Program::uniform2fv(int32_t _idElem, int32_t _nbElement, const float *_value) {
	if (0 == m_program) {
		return;
	}
	if (_idElem<0 || (size_t)_idElem>m_elementList.size()) {
		EWOL_ERROR("idElem = " << _idElem << " not in [0.." << (m_elementList.size()-1) << "]");
		return;
	}
	if (false == m_elementList[_idElem].m_isLinked) {
		return;
	}
	if (0 == _nbElement) {
		EWOL_ERROR("No element to send at open GL ...");
		return;
	}
	if (nullptr == _value) {
		EWOL_ERROR("nullptr Input pointer to send at open GL ...");
		return;
	}
	glUniform2fv(m_elementList[_idElem].m_elementId, _nbElement, _value);
	//checkGlError("glUniform2fv", __LINE__);
}
void ewol::resource::Program::uniform3fv(int32_t _idElem, int32_t _nbElement, const float *_value) {
	if (0 == m_program) {
		return;
	}
	if (_idElem<0 || (size_t)_idElem>m_elementList.size()) {
		EWOL_ERROR("idElem = " << _idElem << " not in [0.." << (m_elementList.size()-1) << "]");
		return;
	}
	if (false == m_elementList[_idElem].m_isLinked) {
		return;
	}
	if (0 == _nbElement) {
		EWOL_ERROR("No element to send at open GL ...");
		return;
	}
	if (nullptr == _value) {
		EWOL_ERROR("nullptr Input pointer to send at open GL ...");
		return;
	}
	glUniform3fv(m_elementList[_idElem].m_elementId, _nbElement, _value);
	//checkGlError("glUniform3fv", __LINE__);
}
void ewol::resource::Program::uniform4fv(int32_t _idElem, int32_t _nbElement, const float *_value) {
	if (0 == m_program) {
		return;
	}
	if (_idElem<0 || (size_t)_idElem>m_elementList.size()) {
		EWOL_ERROR("idElem = " << _idElem << " not in [0.." << (m_elementList.size()-1) << "]");
		return;
	}
	if (false == m_elementList[_idElem].m_isLinked) {
		return;
	}
	if (0 == _nbElement) {
		EWOL_ERROR("No element to send at open GL ...");
		return;
	}
	if (nullptr == _value) {
		EWOL_ERROR("nullptr Input pointer to send at open GL ...");
		return;
	}
	glUniform4fv(m_elementList[_idElem].m_elementId, _nbElement, _value);
	//checkGlError("glUniform4fv", __LINE__);
}

//////////////////////////////////////////////////////////////////////////////////////////////

void ewol::resource::Program::uniform1iv(int32_t _idElem, int32_t _nbElement, const int32_t *_value) {
	if (0 == m_program) {
		return;
	}
	if (_idElem<0 || (size_t)_idElem>m_elementList.size()) {
		EWOL_ERROR("idElem = " << _idElem << " not in [0.." << (m_elementList.size()-1) << "]");
		return;
	}
	if (false == m_elementList[_idElem].m_isLinked) {
		return;
	}
	if (0 == _nbElement) {
		EWOL_ERROR("No element to send at open GL ...");
		return;
	}
	if (nullptr == _value) {
		EWOL_ERROR("nullptr Input pointer to send at open GL ...");
		return;
	}
	glUniform1iv(m_elementList[_idElem].m_elementId, _nbElement, _value);
	//checkGlError("glUniform1iv", __LINE__);
}
void ewol::resource::Program::uniform2iv(int32_t _idElem, int32_t _nbElement, const int32_t *_value) {
	if (0 == m_program) {
		return;
	}
	if (_idElem<0 || (size_t)_idElem>m_elementList.size()) {
		EWOL_ERROR("idElem = " << _idElem << " not in [0.." << (m_elementList.size()-1) << "]");
		return;
	}
	if (false == m_elementList[_idElem].m_isLinked) {
		return;
	}
	if (0 == _nbElement) {
		EWOL_ERROR("No element to send at open GL ...");
		return;
	}
	if (nullptr == _value) {
		EWOL_ERROR("nullptr Input pointer to send at open GL ...");
		return;
	}
	glUniform2iv(m_elementList[_idElem].m_elementId, _nbElement, _value);
	//checkGlError("glUniform2iv", __LINE__);
}
void ewol::resource::Program::uniform3iv(int32_t _idElem, int32_t _nbElement, const int32_t *_value) {
	if (0 == m_program) {
		return;
	}
	if (_idElem<0 || (size_t)_idElem>m_elementList.size()) {
		EWOL_ERROR("idElem = " << _idElem << " not in [0.." << (m_elementList.size()-1) << "]");
		return;
	}
	if (false == m_elementList[_idElem].m_isLinked) {
		return;
	}
	if (0 == _nbElement) {
		EWOL_ERROR("No element to send at open GL ...");
		return;
	}
	if (nullptr == _value) {
		EWOL_ERROR("nullptr Input pointer to send at open GL ...");
		return;
	}
	glUniform3iv(m_elementList[_idElem].m_elementId, _nbElement, _value);
	//checkGlError("glUniform3iv", __LINE__);
}
void ewol::resource::Program::uniform4iv(int32_t _idElem, int32_t _nbElement, const int32_t *_value) {
	if (0 == m_program) {
		return;
	}
	if (_idElem<0 || (size_t)_idElem>m_elementList.size()) {
		EWOL_ERROR("idElem = " << _idElem << " not in [0.." << (m_elementList.size()-1) << "]");
		return;
	}
	if (false == m_elementList[_idElem].m_isLinked) {
		return;
	}
	if (0 == _nbElement) {
		EWOL_ERROR("No element to send at open GL ...");
		return;
	}
	if (nullptr == _value) {
		EWOL_ERROR("nullptr Input pointer to send at open GL ...");
		return;
	}
	glUniform4iv(m_elementList[_idElem].m_elementId, _nbElement, _value);
	//checkGlError("glUniform4iv", __LINE__);
}

//#define PROGRAM_DISPLAY_SPEED

//////////////////////////////////////////////////////////////////////////////////////////////
#ifdef PROGRAM_DISPLAY_SPEED
	int64_t g_startTime = 0;
#endif


void ewol::resource::Program::use() {
	#ifdef PROGRAM_DISPLAY_SPEED
		g_startTime = ewol::getTime();
	#endif
	// event if it was 0  == > set it to prevent other use of the previous shader display ...
	ewol::openGL::useProgram(m_program);
	//checkGlError("glUseProgram", __LINE__);
}


void ewol::resource::Program::setTexture0(int32_t _idElem, GLint _textureOpenGlID) {
	if (0 == m_program) {
		return;
	}
	if (_idElem<0 || (size_t)_idElem>m_elementList.size()) {
		return;
	}
	if (false == m_elementList[_idElem].m_isLinked) {
		return;
	}
	#if 0
		ewol::openGL::enable(GL_TEXTURE_2D);
		checkGlError("glEnable", __LINE__);
	#endif
	ewol::openGL::activeTexture(GL_TEXTURE0);
	//checkGlError("glActiveTexture", __LINE__);
	// set the textureID
	glBindTexture(GL_TEXTURE_2D, _textureOpenGlID);
	//checkGlError("glBindTexture", __LINE__);
	// set the texture on the uniform attribute
	glUniform1i(m_elementList[_idElem].m_elementId, /*GL_TEXTURE*/0);
	//checkGlError("glUniform1i", __LINE__);
	m_hasTexture = true;
}

void ewol::resource::Program::setTexture1(int32_t _idElem, GLint _textureOpenGlID) {
	if (0 == m_program) {
		return;
	}
	if (_idElem<0 || (size_t)_idElem>m_elementList.size()) {
		return;
	}
	if (false == m_elementList[_idElem].m_isLinked) {
		return;
	}
	#if 0
		ewol::openGL::enable(GL_TEXTURE_2D);
		checkGlError("glEnable", __LINE__);
	#endif
	ewol::openGL::activeTexture(GL_TEXTURE1);
	//checkGlError("glActiveTexture", __LINE__);
	// set the textureID
	glBindTexture(GL_TEXTURE_2D, _textureOpenGlID);
	//checkGlError("glBindTexture", __LINE__);
	// set the texture on the uniform attribute
	glUniform1i(m_elementList[_idElem].m_elementId, /*GL_TEXTURE*/1);
	//checkGlError("glUniform1i", __LINE__);
	m_hasTexture1 = true;
}


void ewol::resource::Program::unUse() {
	//EWOL_WARNING("Will use program : " << m_program);
	if (0 == m_program) {
		return;
	}
	#if 0
	if (true == m_hasTexture) {
		ewol::openGL::disable(GL_TEXTURE_2D);
		//checkGlError("glDisable", __LINE__);
		m_hasTexture = false;
	}
	#endif
	// no need to disable program  == > this only generate perturbation on speed ...
	ewol::openGL::useProgram(-1);
	#ifdef PROGRAM_DISPLAY_SPEED
		float localTime = (float)(ewol::getTime() - g_startTime) / 1000.0f;
		if (localTime>1) {
			EWOL_ERROR("      prog : " << localTime << "ms    resource=\"" << m_name << "\"");
		} else {
			EWOL_DEBUG("      prog : " << localTime << "ms    resource=\"" << m_name << "\"");
		}
	#endif
	//checkGlError("glUseProgram", __LINE__);
}

