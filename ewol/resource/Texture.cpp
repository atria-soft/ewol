/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etk/types.hpp>
#include <ewol/ewol.hpp>
#include <gale/renderer/openGL/openGL.hpp>
#include <gale/renderer/openGL/openGL-include.hpp>
#include <gale/resource/Manager.hpp>
#include <ewol/resource/Texture.hpp>
#include <echrono/Steady.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::resource::Texture);

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

void ewol::resource::Texture::init(const etk::String& _filename) {
	gale::Resource::init(_filename);
}
void ewol::resource::Texture::init() {
	gale::Resource::init();
}

ewol::resource::Texture::Texture() :
  m_texId(0),
  #ifdef EWOL_USE_FBO
    m_texPboId(0),
  #endif
  m_data(ivec2(32,32),egami::colorType::RGBA8),
  m_realImageSize(1,1),
  m_lastSize(1,1),
  m_loaded(false),
  m_lastTypeObject(0),
  m_lastSizeObject(0),
  m_repeat(false),
  m_filter(ewol::resource::TextureFilter::linear) {
	addResourceType("ewol::compositing::Texture");
}

ewol::resource::Texture::~Texture() {
	removeContext();
}


void ewol::resource::Texture::setRepeat(bool _value) {
	m_repeat = _value;
}

void ewol::resource::Texture::setFilterMode(enum ewol::resource::TextureFilter _filter) {
	m_filter = _filter;
}

#include <egami/egami.hpp>

bool ewol::resource::Texture::updateContext() {
	EWOL_VERBOSE("updateContext [START]");
	if (false) {
		echrono::Steady tic = echrono::Steady::now();
		gale::openGL::flush();
		echrono::Steady toc = echrono::Steady::now();
		EWOL_VERBOSE("    updateContext [FLUSH] ==> " << (toc - tic));
	}
	ethread::RecursiveLock lock(m_mutex, true);
	echrono::Steady tic = echrono::Steady::now();
	if (lock.tryLock() == false) {
		//Lock error ==> try later ...
		return false;
	}
	int32_t typeObject = GL_RGBA;
	int32_t sizeObject = GL_UNSIGNED_BYTE;
	int32_t sizeByte = 1;
	switch (m_data.getType()) {
		case egami::colorType::RGBA8:
			typeObject = GL_RGBA;
			sizeObject = GL_UNSIGNED_BYTE;
			sizeByte = 4;
			break;
		case egami::colorType::RGB8:
			typeObject = GL_RGB;
			sizeObject = GL_UNSIGNED_BYTE;
			sizeByte = 3;
			break;
		case egami::colorType::RGBAf:
			typeObject = GL_RGBA;
			sizeObject = GL_FLOAT;
			sizeByte = 16;
			break;
		case egami::colorType::RGBf:
			typeObject = GL_RGBA;
			sizeObject = GL_FLOAT;
			sizeByte = 12;
			break;
		case egami::colorType::unsignedInt16:
		case egami::colorType::unsignedInt32:
		case egami::colorType::float32:
		case egami::colorType::float64:
			EWOL_ERROR("Not manage the type " << m_data.getType() << " for texture");
			break;
	}
	if (m_loaded == true) {
		if (    m_lastTypeObject != typeObject
		     || m_lastSizeObject != sizeObject
		     || m_lastSize != m_data.getSize()) {
			EWOL_VERBOSE("TEXTURE: Rm [" << getId() << "] texId=" << m_texId);
			glDeleteTextures(1, &m_texId);
			m_loaded = false;
		}
	}
	if (m_loaded == false) {
		// Request a new texture at openGl :
		glGenTextures(1, &m_texId);
		
		#ifdef EWOL_USE_FBO
			EWOL_ERROR("CREATE PBO");
			glGenBuffers(1, &m_texPboId);
			EWOL_ERROR("CREATE PBO 1");
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_texPboId);
			EWOL_ERROR("CREATE PBO 2");
			glBufferData(GL_PIXEL_UNPACK_BUFFER, m_data.getGPUSize().x()*m_data.getGPUSize().y()*sizeByte, 0, GL_STREAM_DRAW);
			EWOL_ERROR("CREATE PBO 3");
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
			EWOL_ERROR("CREATE PBO 4 (done)");
		#endif
		m_lastSize = m_data.getSize();
		m_lastTypeObject = typeObject;
		m_lastSizeObject = sizeObject;
		EWOL_VERBOSE("TEXTURE: add [" << getId() << "]=" << m_data.getSize() << "=>" << m_data.getGPUSize() << " OGl_Id=" << m_texId << " type=" << m_data.getType());
	} else {
		EWOL_VERBOSE("TEXTURE: update [" << getId() << "]=" << m_data.getSize() << "=>" << m_data.getGPUSize() << " OGl_Id=" << m_texId << " type=" << m_data.getType());
	}
	// in all case we set the texture properties :
	// TODO : check error ???
	glBindTexture(GL_TEXTURE_2D, m_texId);
	if (m_loaded == false) {
		if (m_repeat == false) {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		}
		if (m_filter == ewol::resource::TextureFilter::linear) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		} else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
	}
	//glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	echrono::Steady toc1 = echrono::Steady::now();
	EWOL_VERBOSE("    BIND                 ==> " << (toc1 - tic));
	//egami::store(m_data, etk::String("~/texture_") + etk::toString(getId()) + ".bmp");
	#if    defined(__TARGET_OS__Android) \
	    || defined(__TARGET_OS__IOs)
		// On some embended target, the texture size must be square of 2:
		if (m_loaded == false) {
			// 1: Create the square 2 texture:
			int32_t bufferSize = m_data.getGPUSize().x() * m_data.getGPUSize().y() * 8;
			static etk::Vector<float> tmpData;
			if (tmpData.size() < bufferSize) {
				tmpData.resize(bufferSize, 0.0f);
			}
			EWOL_DEBUG("    CREATE texture ==> " << m_data.getGPUSize());
			// 2 create a new empty texture:
			#ifdef EWOL_USE_FBO
				glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_texPboId);
				void* pBuff = ::glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, m_data.getGPUSize().x() * m_data.getGPUSize().y() * sizeByte, GL_MAP_WRITE_BIT);
				memcpy(pBuff, &tmpData[0], m_data.getGPUSize().x()*m_data.getGPUSize().y()*sizeByte);
				glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
				glTexImage2D(GL_TEXTURE_2D, // Target
				             0, // Level
				             typeObject, // Format internal
				             m_data.getGPUSize().x(),
				             m_data.getGPUSize().y(),
				             0, // Border
				             typeObject, // format
				             sizeObject, // type
				             (void*)0 );
			#else
				glTexImage2D(GL_TEXTURE_2D, // Target
				             0, // Level
				             typeObject, // Format internal
				             m_data.getGPUSize().x(),
				             m_data.getGPUSize().y(),
				             0, // Border
				             typeObject, // format
				             sizeObject, // type
				             &tmpData[0] );
			#endif
		}
		#ifdef EWOL_USE_FBO
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_texPboId);
			void* pBuff = ::glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, m_data.getGPUSize().x() * m_data.getGPUSize().y() * sizeByte, GL_MAP_WRITE_BIT);
			memcpy(pBuff, m_data.getTextureDataPointer(), m_data.getWidth()*m_data.getHeight()*sizeByte);
			glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
			//3 Flush all time the data:
			glTexSubImage2D(GL_TEXTURE_2D, // Target
			                0, // Level
			                0, // x offset
			                0, // y offset
			                m_data.getWidth(),
			                m_data.getHeight(),
			                typeObject, // format
			                sizeObject, // type
			                (void *)0 );
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		#else
			//3 Flush all time the data:
			echrono::Steady tic1 = echrono::Steady::now();
			glTexSubImage2D(GL_TEXTURE_2D, // Target
			                0, // Level
			                0, // x offset
			                0, // y offset
			                m_data.getWidth(),
			                m_data.getHeight(),
			                typeObject, // format
			                sizeObject, // type
			                (void*)((char*)m_data.getTextureDataPointer()) );
			echrono::Steady toc2 = echrono::Steady::now();
			EWOL_INFO("    updateContext [STOP] ==> " << (toc2 - tic1));
		#endif
	#else
		// This is the normal case ==> set the image and after set just the update of the data
		if (m_loaded == false) {
			glTexImage2D(GL_TEXTURE_2D, // Target
			             0, // Level
			             typeObject, // Format internal
			             m_data.getWidth(),
			             m_data.getHeight(),
			             0, // Border
			             typeObject, // format
			             sizeObject, // type
			             m_data.getTextureDataPointer() );
		} else {
			glTexSubImage2D(GL_TEXTURE_2D, // Target
			                0, // Level
			                0, // x offset
			                0, // y offset
			                m_data.getWidth(),
			                m_data.getHeight(),
			                typeObject, // format
			                sizeObject, // type
			                m_data.getTextureDataPointer() );
		}
	#endif
	// now the data is loaded
	m_loaded = true;
	echrono::Steady toc = echrono::Steady::now();
	//EWOL_ERROR("    updateContext [STOP] ==> " << (toc - toc1));
	return true;
}

void ewol::resource::Texture::removeContext() {
	ethread::RecursiveLock lock(m_mutex);
	if (m_loaded == true) {
		// Request remove texture ...
		EWOL_DEBUG("TEXTURE: Rm [" << getId() << "] texId=" << m_texId);
		//glDeleteTextures(1, &m_texId);
		m_loaded = false;
	}
}

void ewol::resource::Texture::removeContextToLate() {
	ethread::RecursiveLock lock(m_mutex);
	m_loaded = false;
	m_texId=0;
}

void ewol::resource::Texture::flush() {
	ethread::RecursiveLock lock(m_mutex);
	// request to the manager to be call at the next update ...
	EWOL_VERBOSE("Request UPDATE of Element");
	getManager().update(ememory::dynamicPointerCast<gale::Resource>(sharedFromThis()));
}

void ewol::resource::Texture::setImageSize(ivec2 _newSize) {
	ethread::RecursiveLock lock(m_mutex);
	_newSize.setValue( nextP2(_newSize.x()), nextP2(_newSize.y()) );
	m_data.resize(_newSize);
}

void ewol::resource::Texture::set(egami::Image _image) {
	EWOL_WARNING("Set a new image in a texture:");
	ethread::RecursiveLock lock(m_mutex);
	if (_image.exist() == false) {
		EWOL_ERROR("ERROR when loading the image : [raw data]");
		return;
	}
	EWOL_WARNING("    size=" << _image.getSize());
	etk::swap(m_data, _image);
	ivec2 tmp = m_data.getSize();
	m_realImageSize = vec2(tmp.x(), tmp.y());
	vec2 compatibilityHWSize = vec2(nextP2(tmp.x()), nextP2(tmp.y()));
	if (m_realImageSize != compatibilityHWSize) {
		EWOL_ERROR("RESIZE Image for HArwareCompatibility:" << m_realImageSize << " => " << compatibilityHWSize);
		m_data.resize(ivec2(compatibilityHWSize.x(),compatibilityHWSize.y()));
	}
	flush();
}
