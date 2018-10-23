/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */


#include <etk/types.hpp>
#include <egami/egami.hpp>
#include <gale/resource/Manager.hpp>
#include <ewol/resource/ImageDF.hpp>
#include <ewol/resource/Texture.hpp>
#include <edtaa3/edtaa3func.h>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::resource::ImageDF);

ewol::resource::ImageDF::ImageDF() {
	addResourceType("ewol::resource::ImageDF");
}


void ewol::resource::ImageDF::init() {
	ethread::RecursiveLock lock(m_mutex);
	ewol::resource::Texture::init();
}

void ewol::resource::ImageDF::init(etk::String _genName, const etk::Uri& _uri, const ivec2& _size) {
	ethread::RecursiveLock lock(m_mutex);
	ewol::resource::Texture::init(_genName);
	EWOL_DEBUG("create a new resource::Image : _genName=" << _genName << " _uri=" << _uri << " size=" << _size);
	m_data = egami::load(_uri, _size);
	if (m_data.exist() == false) {
		EWOL_ERROR("ERROR when loading the image : " << _uri);
	}
	ivec2 tmp = m_data.getSize();
	m_realImageSize = vec2(tmp.x(), tmp.y());
	// distance field Generation
	// TODO : if it is not a .edf ==> generate dynamicly ...
	/*
	egami::ImageMono input;
	input.resize(tmp);
	for (size_t yyy = 0; yyy < tmp.y(); ++yyy) {
		for (size_t xxx = 0; xxx < tmp.x(); ++xxx) {
			input.set(ivec2(xxx, yyy), m_data.get(ivec2(xxx, yyy)).a() );
		}
	}
	generateDistanceField(input, m_data);
	*/
	flush();
}


void ewol::resource::ImageDF::generateDistanceField(const egami::ImageMono& _input, egami::Image& _output) {
	ethread::RecursiveLock lock(m_mutex);
	int32_t size = _input.getSize().x() * _input.getSize().y();
	etk::Vector<short> xdist;
	etk::Vector<short> ydist;
	etk::Vector<double> gx;
	etk::Vector<double> gy;
	etk::Vector<double> data;
	etk::Vector<double> outside;
	etk::Vector<double> inside;
	xdist.resize(size, 0);
	ydist.resize(size, 0);
	gx.resize(size, 0.0);
	gy.resize(size, 0.0);
	data.resize(size, 0.0);
	outside.resize(size, 0.0);
	inside.resize(size, 0.0);
	// Convert img into double (data)
	double img_min = 255, img_max = -255;
	for (int32_t yyy = 0; yyy < _input.getSize().y(); ++yyy) {
		for (int32_t xxx = 0; xxx < _input.getSize().x(); ++xxx) {
			int32_t iii = yyy * _input.getSize().x() + xxx;
			double v = _input.get(ivec2(xxx, yyy));
			data[iii] = v;
			if (v > img_max) {
				img_max = v;
			}
			if (v < img_min) {
				img_min = v;
			}
		}
	}
	// Rescale image levels between 0 and 1
	for (int32_t yyy = 0; yyy < _input.getSize().y(); ++yyy) {
		for (int32_t xxx = 0; xxx < _input.getSize().x(); ++xxx) {
			int32_t iii = yyy * _input.getSize().x() + xxx;
			data[iii] = (_input.get(ivec2(xxx, yyy))-img_min)/img_max;
		}
	}
	
	// Compute outside = edtaa3(bitmap); % Transform background (0's)
	computegradient(&data[0], _input.getSize().x(), _input.getSize().y(), &gx[0], &gy[0]);
	edtaa3(&data[0], &gx[0], &gy[0], _input.getSize().x(), _input.getSize().y(), &xdist[0], &ydist[0], &outside[0]);
	for(size_t iii = 0; iii < outside.size(); ++iii) {
		if( outside[iii] < 0 ) {
			outside[iii] = 0.0;
		}
	}
	
	// Compute inside = edtaa3(1-bitmap); % Transform foreground (1's)
	for(size_t iii = 0; iii < gx.size(); ++iii) {
		gx[iii] = 0;
	}
	for(size_t iii = 0; iii < gy.size(); ++iii) {
		gy[iii] = 0;
	}
	for(size_t iii = 0; iii < data.size(); ++iii) {
		data[iii] = 1 - data[iii];
	}
	computegradient( &data[0], _input.getSize().x(), _input.getSize().y(), &gx[0], &gy[0]);
	edtaa3(&data[0], &gx[0], &gy[0], _input.getSize().x(), _input.getSize().y(), &xdist[0], &ydist[0], &inside[0]);
	for(size_t iii = 0; iii < inside.size(); ++iii) {
		if( inside[iii] < 0 ) {
			inside[iii] = 0.0;
		}
	}
	
	_output.resize(_input.getSize(), etk::Color<>(0));
	_output.clear(etk::Color<>(0));
	for (int32_t xxx = 0; xxx < _output.getSize().x(); ++xxx) {
		for (int32_t yyy = 0; yyy < _output.getSize().y(); ++yyy) {
			int32_t iii = yyy * _output.getSize().x() + xxx;
			outside[iii] -= inside[iii];
			outside[iii] = 128+outside[iii]*16;
			if( outside[iii] < 0 ) {
				outside[iii] = 0;
			}
			if( outside[iii] > 255 ) {
				outside[iii] = 255;
			}
			uint8_t val = 255 - (unsigned char) outside[iii];
			// TODO : Remove multiple size of the map ...
			_output.set(ivec2(xxx, yyy), etk::Color<>((int32_t)val,(int32_t)val,(int32_t)val,255));
		}
	}
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



ememory::SharedPtr<ewol::resource::ImageDF> ewol::resource::ImageDF::create(const etk::Uri& _uri, ivec2 _size) {
	EWOL_VERBOSE("KEEP: TextureFile: '" << _uri << "' size=" << _size);
	if (_uri.isEmpty() == true) {
		ememory::SharedPtr<ewol::resource::ImageDF> object(ETK_NEW(ewol::resource::ImageDF));
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
		_size = ivec2(-1,-1);
	}
	#ifdef __TARGET_OS__MacOs
		EWOL_ERROR("TODO : remove this strange hack");
		_size = ivec2(64,64);
	#endif
	if (    _size.x() > 0
	     && _size.y() > 0) {
		EWOL_VERBOSE("     == > specific size : " << _size);
		#ifdef __TARGET_OS__Android
			_size.setValue(nextP2(_size.x()), nextP2(_size.y()));
		#endif
		tmpFilename.getQuery().set("x", etk::toString(_size.x()));
		tmpFilename.getQuery().set("y", etk::toString(_size.y()));
	}
	
	EWOL_VERBOSE("KEEP: TextureFile: '" << tmpFilename << "' new size=" << _size);
	ememory::SharedPtr<ewol::resource::ImageDF> object = null;
	ememory::SharedPtr<gale::Resource> object2 = getManager().localKeep("DF__" + tmpFilename.getString());
	if (object2 != null) {
		object = ememory::dynamicPointerCast<ewol::resource::ImageDF>(object2);
		if (object == null) {
			EWOL_CRITICAL("Request resource file : '" << tmpFilename << "' With the wrong type (dynamic cast error)");
			return null;
		}
	}
	if (object != null) {
		return object;
	}
	EWOL_INFO("CREATE: ImageDF: '" << tmpFilename << "' size=" << _size);
	// need to crate a new one ...
	object = ememory::SharedPtr<ewol::resource::ImageDF>(ETK_NEW(ewol::resource::ImageDF));
	if (object == null) {
		EWOL_ERROR("allocation error of a resource : " << _uri);
		return null;
	}
	object->init("DF__" + tmpFilename.getString(), _uri, _size);
	getManager().localAdd(object);
	return object;
}

