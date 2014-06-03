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
#include <ewol/resource/Shader.h>
#include <ewol/resource/Manager.h>

#undef __class__
#define __class__ "resource::Shader"

ewol::resource::Shader::Shader(const std::string& _filename) :
  ewol::Resource(_filename),
  m_exist(false),
  m_fileData(nullptr),
  m_shader(0),
  m_type(0) {
	addObjectType("ewol::Shader");
	m_resourceLevel = 0;
	EWOL_DEBUG("OGL : load SHADER \"" << _filename << "\"");
	// load data from file "all the time ..."
	
	if (std::end_with(m_name, ".frag") == true) {
		m_type = GL_FRAGMENT_SHADER;
	} else if (std::end_with(m_name, ".vert") == true) {
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
		EWOL_INFO("after " << _op << "() glError (" << error << ")");
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
		m_shader = glCreateShader(m_type);
		if (!m_shader) {
			EWOL_ERROR("glCreateShader return error ...");
			checkGlError("glCreateShader");
			return;
		} else {
			//EWOL_INFO("Creater shader with GLID=" << m_shader);
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
				std::vector<std::string> lines = std::split(m_fileData, '\n');
				for (size_t iii=0 ; iii<lines.size() ; iii++) {
					EWOL_ERROR("file " << (iii+1) << "|" << lines[iii]);
				}
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
		EWOL_ERROR("File does not Exist : '" << file << "' : '" << file.getFileSystemName() << "'");
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
	file.fileRead(m_fileData, 1, fileSize);
	// close the file:
	file.fileClose();
	
	// now change the OGL context ...
	removeContext();
	updateContext();
}

ewol::object::Shared<ewol::resource::Shader> ewol::resource::Shader::keep(const std::string& _filename) {
	EWOL_VERBOSE("KEEP : Simpleshader : file : \"" << _filename << "\"");
	ewol::object::Shared<ewol::resource::Shader> object = nullptr;
	ewol::object::Shared<ewol::Resource> object2 = getManager().localKeep(_filename);
	if (nullptr != object2) {
		object = ewol::dynamic_pointer_cast<ewol::resource::Shader>(object2);
		if (nullptr == object) {
			EWOL_CRITICAL("Request resource file : '" << _filename << "' With the wrong type (dynamic cast error)");
			return nullptr;
		}
	}
	if (nullptr != object) {
		return object;
	}
	// need to crate a new one ...
	object = ewol::object::makeShared(new ewol::resource::Shader(_filename));
	if (nullptr == object) {
		EWOL_ERROR("allocation error of a resource : " << _filename);
		return nullptr;
	}
	getManager().localAdd(object);
	return object;
}
