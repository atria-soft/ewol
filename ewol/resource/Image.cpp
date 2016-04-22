/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */


#include <etk/types.h>
#include <egami/egami.h>
#include <gale/resource/Manager.h>
#include <ewol/resource/Image.h>
#include <ewol/resource/Texture.h>

const ivec2 ewol::resource::TextureFile::sizeAuto(-1,-1);
const ivec2 ewol::resource::TextureFile::sizeDefault(0,0);

ewol::resource::TextureFile::TextureFile() {
	addResourceType("ewol::resource::Image");
	
}

void ewol::resource::TextureFile::init() {
	std::unique_lock<std::recursive_mutex> lock(m_mutex);
	ewol::resource::Texture::init();
}

void ewol::resource::TextureFile::init(std::string _genName, const std::string& _tmpFilename, const ivec2& _size) {
	std::unique_lock<std::recursive_mutex> lock(m_mutex);
	ewol::resource::Texture::init(_genName);
	EWOL_DEBUG("create a new resource::Image : _genName=" << _genName << " _tmpFilename=" << _tmpFilename << " size=" << _size);
	if (egami::load(m_data, _tmpFilename, _size) == false) {
		EWOL_ERROR("ERROR when loading the image : " << _tmpFilename);
	}
	//egami::store(m_data, "tmpResult.bmp");
	ivec2 tmp = m_data.getSize();
	m_realImageSize = vec2(tmp.x(), tmp.y());
	#ifdef GENERATE_DISTANCE_FIELD_MODE
		//egami::generateDistanceFieldFile(_tmpFilename, std::string(_tmpFilename, 0, _tmpFilename.size()-4) + ".bmp");
		egami::generateDistanceFieldFile(_tmpFilename, std::string(_tmpFilename, 0, _tmpFilename.size()-4) + ".edf");
	#endif
	flush();
}


#ifdef __TARGET_OS__Android
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
#endif



std::shared_ptr<ewol::resource::TextureFile> ewol::resource::TextureFile::create(const std::string& _filename, ivec2 _size, ivec2 _sizeRegister) {
	EWOL_VERBOSE("KEEP: TextureFile: '" << _filename << "' size=" << _size << " sizeRegister=" << _sizeRegister);
	if (_filename == "") {
		std::shared_ptr<ewol::resource::TextureFile> object(new ewol::resource::TextureFile());
		if (nullptr == object) {
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
	std::string tmpFilename = _filename;
	if (etk::end_with(_filename, ".svg") == false) {
		_size = ewol::resource::TextureFile::sizeAuto;
	}
	#ifdef __TARGET_OS__MacOs
		EWOL_ERROR("TODO : remove this strange hack");
		_size = ivec2(64,64);
	#endif
	if (_size.x()>0 && _size.y()>0) {
		EWOL_VERBOSE("     == > specific size : " << _size);
		#ifdef __TARGET_OS__Android
			_size.setValue(nextP2(_size.x()), nextP2(_size.y()));
		#endif
		if (_sizeRegister != ewol::resource::TextureFile::sizeAuto) {
			if (_sizeRegister != ewol::resource::TextureFile::sizeDefault) {
				tmpFilename += ":";
				tmpFilename += etk::to_string(_size.x());
				tmpFilename += "x";
				tmpFilename += etk::to_string(_size.y());
			}
		}
	}
	
	EWOL_VERBOSE("KEEP: TextureFile: '" << tmpFilename << "' new size=" << _size);
	std::shared_ptr<ewol::resource::TextureFile> object = nullptr;
	std::shared_ptr<gale::Resource> object2 = getManager().localKeep(tmpFilename);
	if (object2 != nullptr) {
		object = std::dynamic_pointer_cast<ewol::resource::TextureFile>(object2);
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
	object = std::shared_ptr<ewol::resource::TextureFile>(new ewol::resource::TextureFile());
	if (object == nullptr) {
		EWOL_ERROR("allocation error of a resource : " << _filename);
		return nullptr;
	}
	object->init(tmpFilename, _filename, _size);
	getManager().localAdd(object);
	return object;
}
