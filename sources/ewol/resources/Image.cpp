/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <etk/types.h>
#include <egami/egami.h>
#include <ewol/resources/ResourceManager.h>
#include <ewol/resources/Image.h>
#include <ewol/resources/Texture.h>


#undef __class__
#define __class__ "TextureFile"

ewol::TextureFile::TextureFile(const etk::UString& _genName) :
	Texture(_genName)
{
	
}


ewol::TextureFile::TextureFile(etk::UString _genName, const etk::UString& _tmpfileName, const ivec2& _size) :
	Texture(_genName)
{
	if (false == egami::load(m_data, _tmpfileName, _size)) {
		EWOL_ERROR("ERROR when loading the image : " << _tmpfileName);
	}
	ivec2 tmp = m_data.getSize();
	m_realImageSize = vec2(tmp.x(), tmp.y());
	flush();
}


#ifdef __TARGET_OS__Android
/**
 * @brief get the next power 2 if the input
 * @param[in] _value Value that we want the next power of 2
 * @return result value
 */
static int32_t nextP2(int32_t _value)
{
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



ewol::TextureFile* ewol::TextureFile::keep(const etk::UString& _filename, ivec2 _size)
{
	EWOL_INFO("KEEP : TextureFile : file : " << _filename << " basic size=" << _size);
	if (_filename == "") {
		ewol::TextureFile* object = new ewol::TextureFile("");
		if (NULL == object) {
			EWOL_ERROR("allocation error of a resource : ??TEX??");
			return NULL;
		}
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
	etk::UString TmpFilename = _filename;
	if (false == _filename.endWith(".svg") ) {
		_size = ivec2(-1,-1);
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
		TmpFilename += ":";
		TmpFilename += _size.x();
		TmpFilename += "x";
		TmpFilename += _size.y();
	}
	
	EWOL_INFO("KEEP : TextureFile : file : \"" << TmpFilename << "\" new size=" << _size);
	ewol::TextureFile* object = static_cast<ewol::TextureFile*>(getManager().localKeep(TmpFilename));
	if (NULL != object) {
		return object;
	}
	EWOL_INFO("         == > create new one...");
	// need to crate a new one ...
	object = new ewol::TextureFile(TmpFilename, _filename, _size);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << _filename);
		return NULL;
	}
	getManager().localAdd(object);
	return object;
}


void ewol::TextureFile::release(ewol::TextureFile*& _object)
{
	if (NULL == _object) {
		return;
	}
	ewol::Resource* object2 = static_cast<ewol::Resource*>(_object);
	getManager().release(object2);
	_object = NULL;
}

