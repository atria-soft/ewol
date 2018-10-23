/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */


#include <etk/types.hpp>
#include <egami/egami.hpp>
#include <gale/resource/Manager.hpp>
#include <ewol/resource/TextureFile.hpp>
#include <ewol/resource/Texture.hpp>
#include <ethread/Thread.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::resource::TextureFile);

const ivec2 ewol::resource::TextureFile::sizeAuto(-1,-1);
const ivec2 ewol::resource::TextureFile::sizeDefault(0,0);

/**
 * @brief get the next power 2 if the input
 * @param[in] _value Value that we want the next power of 2
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
	EWOL_CRITICAL("impossible CASE.... request P2 of " << _value);
	return val;
}


ewol::resource::TextureFile::TextureFile() {
	addResourceType("ewol::resource::Image");
	
}

void ewol::resource::TextureFile::init() {
	ethread::RecursiveLock lock(m_mutex);
	ewol::resource::Texture::init();
}

void ewol::resource::TextureFile::init(etk::String _genName, const etk::Uri& _uri, const ivec2& _size) {
	ethread::RecursiveLock lock(m_mutex);
	ewol::resource::Texture::init(_genName);
	EWOL_DEBUG("create a new resource::Image : _genName=" << _genName << " _uri=" << _uri << " size=" << _size);
	egami::Image tmp = egami::load(_uri, _size);
	set(etk::move(tmp));
	//m_lastSize = m_realImageSize;
	#ifdef GENERATE_DISTANCE_FIELD_MODE
		//egami::generateDistanceFieldFile(_uri, etk::String(_uri, 0, _uri.size()-4) + ".bmp");
		egami::generateDistanceFieldFile(_uri, etk::String(_uri, 0, _uri.size()-4) + ".edf");
	#endif
}

ememory::SharedPtr<ewol::resource::TextureFile> ewol::resource::TextureFile::create(const etk::Uri& _uri, ivec2 _size, ivec2 _sizeRegister) {
	EWOL_VERBOSE("KEEP: TextureFile: '" << _uri << "' size=" << _size << " sizeRegister=" << _sizeRegister);
	if (_uri.isEmpty() == true) {
		ememory::SharedPtr<ewol::resource::TextureFile> object(ETK_NEW(ewol::resource::TextureFile));
		if (object == null) {
			EWOL_ERROR("allocation error of a resource : ??TEX??");
			return null;
		}
		object->init();
		getManager().localAdd(object);
		return object;
	}
	if (_size.x() == 0) {
		_size.setX(-1);
		//EWOL_ERROR("Error Request the image size.x() =0 ???");
	}
	if (_size.y() == 0) {
		_size.setY(-1);
		//EWOL_ERROR("Error Request the image size.y() =0 ???");
	}
	etk::Uri tmpFilename = _uri;
	if (etk::toLower(_uri.getPath().getExtention()) != "svg") {
		_size = ewol::resource::TextureFile::sizeAuto;
	}
	if (_size.x()>0 && _size.y()>0) {
		EWOL_VERBOSE("     == > specific size : " << _size);
		_size.setValue(nextP2(_size.x()), nextP2(_size.y()));
		if (_sizeRegister != ewol::resource::TextureFile::sizeAuto) {
			if (_sizeRegister != ewol::resource::TextureFile::sizeDefault) {
				tmpFilename.getQuery().set("x", etk::toString(_size.x()));
				tmpFilename.getQuery().set("y", etk::toString(_size.y()));
			}
		}
	}
	
	EWOL_VERBOSE("KEEP: TextureFile: '" << tmpFilename << "' new size=" << _size);
	ememory::SharedPtr<ewol::resource::TextureFile> object = null;
	ememory::SharedPtr<gale::Resource> object2 = getManager().localKeep(tmpFilename.getString());
	if (object2 != null) {
		object = ememory::dynamicPointerCast<ewol::resource::TextureFile>(object2);
		if (object == null) {
			EWOL_CRITICAL("Request resource file : '" << tmpFilename << "' With the wrong type (dynamic cast error)");
			return null;
		}
	}
	if (object != null) {
		return object;
	}
	EWOL_INFO("CREATE: TextureFile: '" << tmpFilename << "' size=" << _size);
	// need to crate a new one ...
	object = ememory::SharedPtr<ewol::resource::TextureFile>(ETK_NEW(ewol::resource::TextureFile));
	if (object == null) {
		EWOL_ERROR("allocation error of a resource : " << _uri);
		return null;
	}
	object->init(tmpFilename.getString(), _uri, _size);
	getManager().localAdd(object);
	return object;
}
