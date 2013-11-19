/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>
#include <ewol/ewol.h>
#include <ewol/renderer/openGL.h>
#include <ewol/resources/ResourceManager.h>
#include <ewol/resources/Texture.h>
#include <ewol/renderer/eContext.h>

#undef __class__
#define __class__ "Texture"

/**
 * @brief get the next power 2 if the input
 * @param[in] value Value that we want the next power of 2
 * @return result value
 */
static int32_t nextP2(int32_t _value) {
	int32_t val=1;
	for (int32_t iii=1; iii<31; iii++) {
		if (_value <= val) {
			return val;
		}
		val *=2;
	}
	EWOL_CRITICAL("impossible CASE....");
	return val;
}


ewol::Texture::Texture(const std::string& _filename) :
 ewol::Resource(_filename) {
	addObjectType("ewol::Texture");
	m_loaded = false;
	m_texId = 0;
	m_endPointSize.setValue(1.0,1.0);
}

ewol::Texture::Texture(void) {
	addObjectType("ewol::Texture");
	m_loaded = false;
	m_texId = 0;
	m_endPointSize.setValue(1.0,1.0);
}

ewol::Texture::~Texture(void) {
	removeContext();
}

void ewol::Texture::updateContext(void) {
	if (false == m_loaded) {
		// Request a new texture at openGl :
		glGenTextures(1, &m_texId);
	}
	// in all case we set the texture properties :
	// TODO : check error ???
	glBindTexture(GL_TEXTURE_2D, m_texId);
	// TODO : Check error ???
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//--- mode nearest
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//--- Mode linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	EWOL_INFO("TEXTURE: add [" << getId() << "]=" << m_data.getSize() << " OGl_Id=" <<m_texId);
	glTexImage2D(GL_TEXTURE_2D, // Target
	             0, // Level
	             GL_RGBA, // Format internal
	             m_data.getWidth(),
	             m_data.getHeight(),
	             0, // Border
	             GL_RGBA, // format
	             GL_UNSIGNED_BYTE, // type
	             m_data.getTextureDataPointer() );
	// now the data is loaded
	m_loaded = true;
}

void ewol::Texture::removeContext(void) {
	if (true == m_loaded) {
		// Request remove texture ...
		EWOL_INFO("TEXTURE: Rm [" << getId() << "] texId=" << m_texId);
		glDeleteTextures(1, &m_texId);
		m_loaded = false;
	}
}

void ewol::Texture::removeContextToLate(void) {
	m_loaded = false;
	m_texId=0;
}

void ewol::Texture::flush(void) {
	// request to the manager to be call at the next update ...
	getManager().update(this);
}

void ewol::Texture::setImageSize(ivec2 _newSize) {
	_newSize.setValue( nextP2(_newSize.x()), nextP2(_newSize.y()) );
	m_data.resize(_newSize);
}

ewol::Texture* ewol::Texture::keep(void) {
	// this element create a new one every time ....
	ewol::Texture* object = new ewol::Texture();
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : ??TEX??");
		return NULL;
	}
	getManager().localAdd(object);
	return object;
}

void ewol::Texture::release(ewol::Texture*& _object) {
	if (NULL == _object) {
		return;
	}
	ewol::Resource* object2 = static_cast<ewol::Resource*>(_object);
	getManager().release(object2);
	_object = NULL;
}
