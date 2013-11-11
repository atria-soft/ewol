/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>
#include <etk/os/FSNode.h>
#include <ewol/debug.h>
#include <ewol/resources/Shader.h>
#include <ewol/resources/ResourceManager.h>

#undef __class__
#define __class__ "Shader"

ewol::Shader::Shader(const etk::UString& _filename) :
  ewol::Resource(_filename),
  m_exist(false),
  m_fileData(NULL),
  m_shader(0),
  m_type(0) {
	m_resourceLevel = 0;
	EWOL_DEBUG("OGL : load SHADER \"" << _filename << "\"");
	// load data from file "all the time ..."
	
	if (true == m_name.endWith(".frag") ) {
		m_type = GL_FRAGMENT_SHADER;
	} else if (true == m_name.endWith(".vert") ) {
		m_type = GL_VERTEX_SHADER;
	} else {
		EWOL_ERROR("File does not have extention \".vert\" for Vertex Shader or \".frag\" for Fragment Shader. but : \"" << m_name << "\"");
		return;
	}
	reload();
}

ewol::Shader::~Shader(void) {
	if (NULL != m_fileData) {
		delete [] m_fileData;
		m_fileData = NULL;
	}
	if (0!=m_shader) {
		glDeleteShader(m_shader);
		m_shader = 0;
	}
	m_exist = false;
}

static void checkGlError(const char* _op) {
	for (GLint error = glGetError(); error; error = glGetError()) {
		EWOL_INFO("after " << _op << "() glError (" << error << ")");
	}
}
#define LOG_OGL_INTERNAL_BUFFER_LEN    (8192)
static char l_bufferDisplayError[LOG_OGL_INTERNAL_BUFFER_LEN] = "";

void ewol::Shader::updateContext(void) {
	if (true == m_exist) {
		// Do nothing  == > too dangerous ...
	} else {
		// create the Shader
		if (NULL == m_fileData) {
			m_shader = 0;
			return;
		}
		m_shader = glCreateShader(m_type);
		if (!m_shader) {
			EWOL_ERROR("glCreateShader return error ...");
			checkGlError("glCreateShader");
			return;
		} else {
			//EWOL_INFO("Creater shader with GLID=" << m_shader);
			glShaderSource(m_shader, 1, (const char**)&m_fileData, NULL);
			glCompileShader(m_shader);
			GLint compiled = 0;
			glGetShaderiv(m_shader, GL_COMPILE_STATUS, &compiled);
			if (!compiled) {
				GLint infoLen = 0;
				l_bufferDisplayError[0] = '\0';
				glGetShaderInfoLog(m_shader, LOG_OGL_INTERNAL_BUFFER_LEN, &infoLen, l_bufferDisplayError);
				const char * tmpShaderType = "GL_FRAGMENT_SHADER";
				if (m_type == GL_VERTEX_SHADER){
					tmpShaderType = "GL_VERTEX_SHADER";
				}
				EWOL_ERROR("Could not compile \"" << tmpShaderType << "\" name='" << m_name << "'");
				EWOL_ERROR("Error " << l_bufferDisplayError);
				std::vector<etk::UString> lines = etk::UString(m_fileData).split('\n');
				for (esize_t iii=0 ; iii<lines.size() ; iii++) {
					EWOL_ERROR("file " << (iii+1) << "|" << lines[iii]);
				}
				return;
			}
		}
		m_exist = true;
	}
}

void ewol::Shader::removeContext(void) {
	if (true == m_exist) {
		glDeleteShader(m_shader);
		m_exist = false;
		m_shader = 0;
	}
}

void ewol::Shader::removeContextToLate(void) {
	m_exist = false;
	m_shader = 0;
}

void ewol::Shader::reload(void) {
	etk::FSNode file(m_name);
	if (false == file.exist()) {
		EWOL_ERROR("File does not Exist : \"" << file << "\"");
		return;
	}
	
	int64_t fileSize = file.fileSize();
	if (0 == fileSize) {
		EWOL_ERROR("This file is empty : " << file);
		return;
	}
	if (false == file.fileOpenRead()) {
		EWOL_ERROR("Can not open the file : " << file);
		return;
	}
	// remove previous data ...
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
	file.fileRead(m_fileData, 1, fileSize);
	// close the file:
	file.fileClose();
	
	// now change the OGL context ...
	removeContext();
	updateContext();
}

ewol::Shader* ewol::Shader::keep(const etk::UString& _filename) {
	EWOL_VERBOSE("KEEP : Simpleshader : file : \"" << _filename << "\"");
	ewol::Shader* object = static_cast<ewol::Shader*>(getManager().localKeep(_filename));
	if (NULL != object) {
		return object;
	}
	// need to crate a new one ...
	object = new ewol::Shader(_filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << _filename);
		return NULL;
	}
	getManager().localAdd(object);
	return object;
}

void ewol::Shader::release(ewol::Shader*& _object) {
	if (NULL == _object) {
		return;
	}
	ewol::Resource* object2 = static_cast<ewol::Resource*>(_object);
	getManager().release(object2);
	_object = NULL;
}
