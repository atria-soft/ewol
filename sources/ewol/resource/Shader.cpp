/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <etk/types.h>
#include <etk/os/FSNode.h>
#include <ewol/debug.h>
#include <ewol/resource/Shader.h>
#include <ewol/resource/Manager.h>

#undef __class__
#define __class__ "resource::Shader"

ewol::resource::Shader::Shader() :
  ewol::Resource(),
  m_exist(false),
  m_fileData(nullptr),
  m_shader(0),
  m_type(0) {
	addObjectType("ewol::Shader");
	m_resourceLevel = 0;
}

void ewol::resource::Shader::init(const std::string& _filename) {
	ewol::Resource::init(_filename);
	EWOL_DEBUG("OGL : load SHADER \"" << _filename << "\"");
	// load data from file "all the time ..."
	
	if (etk::end_with(m_name, ".frag") == true) {
		m_type = GL_FRAGMENT_SHADER;
	} else if (etk::end_with(m_name, ".vert") == true) {
		m_type = GL_VERTEX_SHADER;
	} else {
		EWOL_ERROR("File does not have extention \".vert\" for Vertex Shader or \".frag\" for Fragment Shader. but : \"" << m_name << "\"");
		return;
	}
	reload();
}

ewol::resource::Shader::~Shader() {
	if (nullptr != m_fileData) {
		delete [] m_fileData;
		m_fileData = nullptr;
	}
	if (0!=m_shader) {
		glDeleteShader(m_shader);
		m_shader = 0;
	}
	m_exist = false;
}

static void checkGlError(const char* _op) {
	for (GLint error = glGetError(); error; error = glGetError()) {
		EWOL_ERROR("after " << _op << "() glError (" << error << ")");
	}
}
#define LOG_OGL_INTERNAL_BUFFER_LEN    (8192)
static char l_bufferDisplayError[LOG_OGL_INTERNAL_BUFFER_LEN] = "";

void ewol::resource::Shader::updateContext() {
	if (true == m_exist) {
		// Do nothing  == > too dangerous ...
	} else {
		// create the Shader
		if (nullptr == m_fileData) {
			m_shader = 0;
			return;
		}
		EWOL_INFO("Create Shader : '" << m_name << "'");
		m_shader = glCreateShader(m_type);
		if (!m_shader) {
			EWOL_ERROR("glCreateShader return error ...");
			checkGlError("glCreateShader");
			EWOL_CRITICAL(" can not load shader");
			return;
		} else {
			EWOL_INFO("Compile shader with GLID=" << m_shader);
			glShaderSource(m_shader, 1, (const char**)&m_fileData, nullptr);
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
				std::vector<std::string> lines = etk::split(m_fileData, '\n');
				for (size_t iii=0 ; iii<lines.size() ; iii++) {
					EWOL_ERROR("file " << (iii+1) << "|" << lines[iii]);
				}
				EWOL_CRITICAL(" can not load shader");
				return;
			}
		}
		m_exist = true;
	}
}

void ewol::resource::Shader::removeContext() {
	if (true == m_exist) {
		glDeleteShader(m_shader);
		m_exist = false;
		m_shader = 0;
	}
}

void ewol::resource::Shader::removeContextToLate() {
	m_exist = false;
	m_shader = 0;
}

void ewol::resource::Shader::reload() {
	etk::FSNode file(m_name);
	if (false == file.exist()) {
		EWOL_CRITICAL("File does not Exist : '" << file << "' : '" << file.getFileSystemName() << "'");
		return;
	}
	
	int64_t fileSize = file.fileSize();
	if (0 == fileSize) {
		EWOL_CRITICAL("This file is empty : " << file);
		return;
	}
	if (false == file.fileOpenRead()) {
		EWOL_CRITICAL("Can not open the file : " << file);
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
		EWOL_CRITICAL("Error Memory allocation size=" << fileSize);
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

