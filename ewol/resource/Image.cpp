/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */


#include <etk/types.hpp>
#include <egami/egami.hpp>
#include <gale/resource/Manager.hpp>
#include <ewol/resource/Image.hpp>
#include <ewol/resource/Texture.hpp>
#include <ethread/Thread.hpp>

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

void ewol::resource::TextureFile::init(etk::String _genName, const etk::String& _tmpFilename, const ivec2& _size) {
	ethread::RecursiveLock lock(m_mutex);
	ewol::resource::Texture::init(_genName);
	EWOL_DEBUG("create a new resource::Image : _genName=" << _genName << " _tmpFilename=" << _tmpFilename << " size=" << _size);
	m_data = egami::load(_tmpFilename, _size);
	if (m_data.exist() == false) {
		EWOL_ERROR("ERROR when loading the image : " << _tmpFilename);
	}
	//egami::store(m_data, "tmpResult.bmp");
	ivec2 tmp = m_data.getSize();
	m_realImageSize = vec2(tmp.x(), tmp.y());
	vec2 compatibilityHWSize = vec2(nextP2(tmp.x()), nextP2(tmp.y()));
	if (m_realImageSize != compatibilityHWSize) {
		EWOL_ERROR("RESIZE Image for HArwareCompatibility:" << m_realImageSize << " => " << compatibilityHWSize);
		m_data.resize(ivec2(compatibilityHWSize.x(),compatibilityHWSize.y()));
	}
	m_lastSize = m_realImageSize;
	#ifdef GENERATE_DISTANCE_FIELD_MODE
		//egami::generateDistanceFieldFile(_tmpFilename, etk::String(_tmpFilename, 0, _tmpFilename.size()-4) + ".bmp");
		egami::generateDistanceFieldFile(_tmpFilename, etk::String(_tmpFilename, 0, _tmpFilename.size()-4) + ".edf");
	#endif
	flush();
}

ememory::SharedPtr<ewol::resource::TextureFile> ewol::resource::TextureFile::create(const etk::String& _filename, ivec2 _size, ivec2 _sizeRegister) {
	EWOL_VERBOSE("KEEP: TextureFile: '" << _filename << "' size=" << _size << " sizeRegister=" << _sizeRegister);
	if (_filename == "") {
		ememory::SharedPtr<ewol::resource::TextureFile> object(new ewol::resource::TextureFile());
		if (object == nullptr) {
			EWOL_ERROR("allocation error of a resource : ??TEX??");
			return nullptr;
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
	etk::String tmpFilename = _filename;
	if (etk::end_with(_filename, ".svg") == false) {
		_size = ewol::resource::TextureFile::sizeAuto;
	}
	if (_size.x()>0 && _size.y()>0) {
		EWOL_VERBOSE("     == > specific size : " << _size);
		_size.setValue(nextP2(_size.x()), nextP2(_size.y()));
		if (_sizeRegister != ewol::resource::TextureFile::sizeAuto) {
			if (_sizeRegister != ewol::resource::TextureFile::sizeDefault) {
				tmpFilename += ":";
				tmpFilename += etk::toString(_size.x());
				tmpFilename += "x";
				tmpFilename += etk::toString(_size.y());
			}
		}
	}
	
	EWOL_VERBOSE("KEEP: TextureFile: '" << tmpFilename << "' new size=" << _size);
	ememory::SharedPtr<ewol::resource::TextureFile> object = nullptr;
	ememory::SharedPtr<gale::Resource> object2 = getManager().localKeep(tmpFilename);
	if (object2 != nullptr) {
		object = ememory::dynamicPointerCast<ewol::resource::TextureFile>(object2);
		if (object == nullptr) {
			EWOL_CRITICAL("Request resource file : '" << tmpFilename << "' With the wrong type (dynamic cast error)");
			return nullptr;
		}
	}
	if (object != nullptr) {
		return object;
	}
	EWOL_INFO("CREATE: TextureFile: '" << tmpFilename << "' size=" << _size);
	// need to crate a new one ...
	object = ememory::SharedPtr<ewol::resource::TextureFile>(new ewol::resource::TextureFile());
	if (object == nullptr) {
		EWOL_ERROR("allocation error of a resource : " << _filename);
		return nullptr;
	}
	object->init(tmpFilename, _filename, _size);
	getManager().localAdd(object);
	return object;
}
