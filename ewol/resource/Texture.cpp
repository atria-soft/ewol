/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <etk/types.h>
#include <ewol/ewol.h>
#include <gale/renderer/openGL/openGL.h>
#include <gale/renderer/openGL/openGL-include.h>
#include <gale/resource/Manager.h>
#include <ewol/resource/Texture.h>

#undef __class__
#define __class__ "resource::Texture"

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


void ewol::resource::Texture::init(const std::string& _filename) {
	gale::Resource::init(_filename);
}
void ewol::resource::Texture::init() {
	gale::Resource::init();
}

ewol::resource::Texture::Texture() :
  m_texId(0),
  m_endPointSize(1,1),
  m_loaded(false) {
	addResourceType("ewol::compositing::Texture");
}

ewol::resource::Texture::~Texture() {
	removeContext();
}
#include <egami/wrapperBMP.h>

void ewol::resource::Texture::updateContext() {
	if (false == m_loaded) {
		// Request a new texture at openGl :
		glGenTextures(1, &m_texId);
	}
	EWOL_ERROR("plop : load the image:" << m_name);
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
	//egami::storeBMP("~/bbb_image.bmp", m_data);
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

void ewol::resource::Texture::removeContext() {
	if (true == m_loaded) {
		// Request remove texture ...
		EWOL_INFO("TEXTURE: Rm [" << getId() << "] texId=" << m_texId);
		glDeleteTextures(1, &m_texId);
		m_loaded = false;
	}
}

void ewol::resource::Texture::removeContextToLate() {
	m_loaded = false;
	m_texId=0;
}

void ewol::resource::Texture::flush() {
	// request to the manager to be call at the next update ...
	getManager().update(std::dynamic_pointer_cast<gale::Resource>(shared_from_this()));
}

void ewol::resource::Texture::setImageSize(ivec2 _newSize) {
	_newSize.setValue( nextP2(_newSize.x()), nextP2(_newSize.y()) );
	m_data.resize(_newSize);
}
