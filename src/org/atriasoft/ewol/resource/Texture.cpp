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
static int nextP2(int _value) {
	int val=1;
	for (int iii=1; iii<31; iii++) {
		if (_value <= val) {
			return val;
		}
		val *=2;
	}
	Log.critical("impossible CASE....");
	return val;
}

void ewol::resource::Texture::init( String _filename) {
	gale::Resource::init(_filename);
}
void ewol::resource::Texture::init() {
	gale::Resource::init();
}

ewol::resource::Texture::Texture() :
  this.texId(0),
  #ifdef EWOL_USE_FBO
    this.texPboId(0),
  #endif
  this.data(Vector2i(32,32),egami::colorType::RGBA8),
  this.realImageSize(1,1),
  this.lastSize(1,1),
  this.loaded(false),
  this.lastTypeObject(0),
  this.lastSizeObject(0),
  this.repeat(false),
  this.filter(ewol::resource::TextureFilter::linear) {
	addResourceType("ewol::compositing::Texture");
}

ewol::resource::Texture::~Texture() {
	removeContext();
}


void ewol::resource::Texture::setRepeat(boolean _value) {
	this.repeat = _value;
}

void ewol::resource::Texture::setFilterMode(enum ewol::resource::TextureFilter _filter) {
	this.filter = _filter;
}

#include <egami/egami.hpp>

boolean ewol::resource::Texture::updateContext() {
	Log.verbose("updateContext [START]");
	if (false) {
		echrono::Steady tic = echrono::Steady::now();
		gale::openGL::flush();
		echrono::Steady toc = echrono::Steady::now();
		Log.verbose("    updateContext [FLUSH] ==> " + (toc - tic));
	}
	ethread::RecursiveLock lock(this.mutex, true);
	echrono::Steady tic = echrono::Steady::now();
	if (lock.tryLock() == false) {
		//Lock error ==> try later ...
		return false;
	}
	int typeObject = GL_RGBA;
	int sizeObject = GL_UNSIGNED_BYTE;
	int sizeByte = 1;
	switch (this.data.getType()) {
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
			Log.error("Not manage the type " + this.data.getType() + " for texture");
			break;
	}
	if (this.loaded == true) {
		if (    this.lastTypeObject != typeObject
		     || this.lastSizeObject != sizeObject
		     || this.lastSize != this.data.getSize()) {
			Log.warning("TEXTURE: Rm [" + getId() + "] texId=" + this.texId);
			glDeleteTextures(1, this.texId);
			this.loaded = false;
		}
	}
	if (this.loaded == false) {
		// Request a new texture at openGl :
		glGenTextures(1, this.texId);
		
		#ifdef EWOL_USE_FBO
			Log.error("CREATE PBO");
			glGenBuffers(1, this.texPboId);
			Log.error("CREATE PBO 1");
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, this.texPboId);
			Log.error("CREATE PBO 2");
			glBufferData(GL_PIXEL_UNPACK_BUFFER, this.data.getGPUSize().x()*this.data.getGPUSize().y()*sizeByte, 0, GL_STREAM_DRAW);
			Log.error("CREATE PBO 3");
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
			Log.error("CREATE PBO 4 (done)");
		#endif
		this.lastSize = this.data.getSize();
		this.lastTypeObject = typeObject;
		this.lastSizeObject = sizeObject;
		Log.debug("TEXTURE: add [" + getId() + "]=" + this.data.getSize() + "=>" + this.data.getGPUSize() + " OGl_Id=" + this.texId + " type=" << this.data.getType());
	} else {
		Log.debug("TEXTURE: update [" + getId() + "]=" + this.data.getSize() + "=>" + this.data.getGPUSize() + " OGl_Id=" + this.texId + " type=" << this.data.getType());
	}
	// in all case we set the texture properties :
	// TODO : check error ???
	glBindTexture(GL_TEXTURE_2D, this.texId);
	if (this.loaded == false) {
		if (this.repeat == false) {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		}
		if (this.filter == ewol::resource::TextureFilter::linear) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		} else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
	}
	//glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	echrono::Steady toc1 = echrono::Steady::now();
	Log.verbose("    BIND                 ==> " + (toc1 - tic));
	//egami::store(this.data, String("~/texture_") + etk::toString(getId()) + ".bmp");
	#if    defined(__TARGET_OS__Android) \
	    || defined(__TARGET_OS__IOs)
		// On some embended target, the texture size must be square of 2:
		if (this.loaded == false) {
			// 1: Create the square 2 texture:
			int bufferSize = this.data.getGPUSize().x() * this.data.getGPUSize().y() * 8;
			static List<float> tmpData;
			if (tmpData.size() < bufferSize) {
				tmpData.resize(bufferSize, 0.0f);
			}
			Log.debug("    CREATE texture ==> " + this.data.getGPUSize());
			// 2 create a new empty texture:
			#ifdef EWOL_USE_FBO
				glBindBuffer(GL_PIXEL_UNPACK_BUFFER, this.texPboId);
				void* pBuff = ::glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, this.data.getGPUSize().x() * this.data.getGPUSize().y() * sizeByte, GL_MAP_WRITE_BIT);
				memcpy(pBuff, tmpData[0], this.data.getGPUSize().x()*this.data.getGPUSize().y()*sizeByte);
				glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
				glTexImage2D(GL_TEXTURE_2D, // Target
				             0, // Level
				             typeObject, // Format internal
				             this.data.getGPUSize().x(),
				             this.data.getGPUSize().y(),
				             0, // Border
				             typeObject, // format
				             sizeObject, // type
				             (void*)0 );
			#else
				glTexImage2D(GL_TEXTURE_2D, // Target
				             0, // Level
				             typeObject, // Format internal
				             this.data.getGPUSize().x(),
				             this.data.getGPUSize().y(),
				             0, // Border
				             typeObject, // format
				             sizeObject, // type
				             tmpData[0] );
			#endif
		}
		#ifdef EWOL_USE_FBO
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, this.texPboId);
			void* pBuff = ::glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, this.data.getGPUSize().x() * this.data.getGPUSize().y() * sizeByte, GL_MAP_WRITE_BIT);
			memcpy(pBuff, this.data.getTextureDataPointer(), this.data.getWidth()*this.data.getHeight()*sizeByte);
			glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
			//3 Flush all time the data:
			glTexSubImage2D(GL_TEXTURE_2D, // Target
			                0, // Level
			                0, // x offset
			                0, // y offset
			                this.data.getWidth(),
			                this.data.getHeight(),
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
			                this.data.getWidth(),
			                this.data.getHeight(),
			                typeObject, // format
			                sizeObject, // type
			                (void*)((char*)this.data.getTextureDataPointer()) );
			echrono::Steady toc2 = echrono::Steady::now();
			Log.info("    updateContext [STOP] ==> " + (toc2 - tic1));
		#endif
	#else
		// This is the normal case ==> set the image and after set just the update of the data
		if (this.loaded == false) {
			glTexImage2D(GL_TEXTURE_2D, // Target
			             0, // Level
			             typeObject, // Format internal
			             this.data.getWidth(),
			             this.data.getHeight(),
			             0, // Border
			             typeObject, // format
			             sizeObject, // type
			             this.data.getTextureDataPointer() );
		} else {
			glTexSubImage2D(GL_TEXTURE_2D, // Target
			                0, // Level
			                0, // x offset
			                0, // y offset
			                this.data.getWidth(),
			                this.data.getHeight(),
			                typeObject, // format
			                sizeObject, // type
			                this.data.getTextureDataPointer() );
		}
	#endif
	// now the data is loaded
	this.loaded = true;
	echrono::Steady toc = echrono::Steady::now();
	//Log.error("    updateContext [STOP] ==> " + (toc - toc1));
	return true;
}

void ewol::resource::Texture::removeContext() {
	ethread::RecursiveLock lock(this.mutex);
	if (this.loaded == true) {
		// Request remove texture ...
		Log.debug("TEXTURE: Rm [" + getId() + "] texId=" + this.texId);
		// TODO: Check if we are in the correct thread
		glDeleteTextures(1, this.texId);
		this.loaded = false;
	}
}

void ewol::resource::Texture::removeContextToLate() {
	ethread::RecursiveLock lock(this.mutex);
	this.loaded = false;
	this.texId=0;
}

void ewol::resource::Texture::flush() {
	ethread::RecursiveLock lock(this.mutex);
	// request to the manager to be call at the next update ...
	Log.verbose("Request UPDATE of Element");
	getManager().update(ememory::dynamicPointerCast<gale::Resource>(sharedFromThis()));
}

void ewol::resource::Texture::setImageSize(Vector2i _newSize) {
	ethread::RecursiveLock lock(this.mutex);
	_newSize.setValue( nextP2(_newSize.x()), nextP2(_newSize.y()) );
	this.data.resize(_newSize);
}

void ewol::resource::Texture::set(egami::Image _image) {
	Log.debug("Set a new image in a texture:");
	ethread::RecursiveLock lock(this.mutex);
	if (_image.exist() == false) {
		Log.error("ERROR when loading the image : [raw data]");
		return;
	}
	Log.debug("    size=" + _image.getSize());
	etk::swap(this.data, _image);
	Vector2i tmp = this.data.getSize();
	this.realImageSize = Vector2f(tmp.x(), tmp.y());
	Vector2f compatibilityHWSize = Vector2f(nextP2(tmp.x()), nextP2(tmp.y()));
	if (this.realImageSize != compatibilityHWSize) {
		Log.verbose("RESIZE Image for HArwareCompatibility:" + this.realImageSize + " => " + compatibilityHWSize);
		this.data.resize(Vector2i(compatibilityHWSize.x(),compatibilityHWSize.y()));
	}
	flush();
}
