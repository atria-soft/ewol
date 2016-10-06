/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <etk/types.hpp>
#include <ewol/ewol.hpp>
#include <gale/renderer/openGL/openGL.hpp>
#include <gale/renderer/openGL/openGL-include.hpp>
#include <gale/resource/Manager.hpp>
#include <ewol/resource/Texture.hpp>

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
  m_data(ivec2(32,32),egami::colorType::RGBA8),
  m_endPointSize(1,1),
  m_loaded(false) {
	addResourceType("ewol::compositing::Texture");
}

ewol::resource::Texture::~Texture() {
	removeContext();
}

#include <egami/egami.hpp>

bool ewol::resource::Texture::updateContext() {
	EWOL_DEBUG("updateContext [START]");
	std::unique_lock<std::recursive_mutex> lock(m_mutex, std::defer_lock);
	if (lock.try_lock() == false) {
		//Lock error ==> try later ...
		return false;
	}
	if (m_loaded == false) {
		// Request a new texture at openGl :
		glGenTextures(1, &m_texId);
	}
	EWOL_DEBUG("load the image:" << m_name);
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
	int32_t typeObject = GL_RGBA;
	int32_t sizeObject = GL_UNSIGNED_BYTE;
	switch (m_data.getType()) {
		case egami::colorType::RGBA8:
			typeObject = GL_RGBA;
			sizeObject = GL_UNSIGNED_BYTE;
			break;
		case egami::colorType::RGB8:
			typeObject = GL_RGB;
			sizeObject = GL_UNSIGNED_BYTE;
			break;
		case egami::colorType::RGBAf:
			typeObject = GL_RGBA;
			sizeObject = GL_FLOAT;
			break;
		case egami::colorType::RGBf:
			typeObject = GL_RGBA;
			sizeObject = GL_FLOAT;
			break;
		case egami::colorType::unsignedInt16:
		case egami::colorType::unsignedInt32:
		case egami::colorType::float32:
		case egami::colorType::float64:
			EWOL_ERROR("Not manage the type " << m_data.getType() << " for texture");
			break;
	}
	EWOL_DEBUG("TEXTURE: add [" << getId() << "]=" << m_data.getSize() << " OGl_Id=" << m_texId << " type=" << m_data.getType());
	//egami::store(m_data, std::string("~/texture_") + etk::to_string(getId()) + ".bmp");
	glTexImage2D(GL_TEXTURE_2D, // Target
	             0, // Level
	             typeObject, // Format internal
	             m_data.getWidth(),
	             m_data.getHeight(),
	             0, // Border
	             typeObject, // format
	             sizeObject, // type
	             m_data.getTextureDataPointer() );
	// now the data is loaded
	m_loaded = true;
	EWOL_DEBUG("updateContext [STOP]");
	return true;
}

void ewol::resource::Texture::removeContext() {
	std::unique_lock<std::recursive_mutex> lock(m_mutex);
	if (m_loaded == true) {
		// Request remove texture ...
		EWOL_INFO("TEXTURE: Rm [" << getId() << "] texId=" << m_texId);
		glDeleteTextures(1, &m_texId);
		m_loaded = false;
	}
}

void ewol::resource::Texture::removeContextToLate() {
	std::unique_lock<std::recursive_mutex> lock(m_mutex);
	m_loaded = false;
	m_texId=0;
}

void ewol::resource::Texture::flush() {
	std::unique_lock<std::recursive_mutex> lock(m_mutex);
	// request to the manager to be call at the next update ...
	EWOL_VERBOSE("Request UPDATE of Element");
	getManager().update(ememory::dynamicPointerCast<gale::Resource>(sharedFromThis()));
}

void ewol::resource::Texture::setImageSize(ivec2 _newSize) {
	std::unique_lock<std::recursive_mutex> lock(m_mutex);
	_newSize.setValue( nextP2(_newSize.x()), nextP2(_newSize.y()) );
	m_data.resize(_newSize);
}
