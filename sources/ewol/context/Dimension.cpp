/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/Dimension.h>
#include <ewol/debug.h>

#undef __class__
#define __class__	"Dimension"

// TODO : set this in a super class acced in a statin fuction...
// ratio in milimeter :
static bool isInit = false;
static vec2 ratio(9999999,888888);
static vec2 invRatio(1,1);
static ewol::context::Dimension windowsSize(vec2(9999999,888888), ewol::Dimension::Pixel);

static const float       inchToMillimeter = 1.0f/25.4f;
static const float       footToMillimeter = 1.0f/304.8f;
static const float      meterToMillimeter = 1.0f/1000.0f;
static const float centimeterToMillimeter = 1.0f/10.0f;
static const float  kilometerToMillimeter = 1.0f/1000000.0f;
static const float millimeterToInch = 25.4f;
static const float millimeterToFoot = 304.8f;
static const float millimeterToMeter =1000.0f;
static const float millimeterToCentimeter = 10.0f;
static const float millimeterToKilometer = 1000000.0f;


void ewol::context::Dimension::init(void) {
	if (true == isInit) {
		return;
	}
	ewol::context::Dimension conversion(vec2(72,72), ewol::context::Dimension::Inch);
	ratio = conversion.getMillimeter();
	invRatio.setValue(1.0f/ratio.x(),1.0f/ratio.y());
	windowsSize.set(vec2(200,200), ewol::context::Dimension::Pixel);
	isInit = true;
}

void ewol::context::Dimension::unInit(void) {
	isInit = false;
	ratio.setValue(9999999,888888);
	invRatio.setValue(1.0f/ratio.x(),1.0f/ratio.y());
	windowsSize.set(vec2(9999999,88888), ewol::context::Dimension::Pixel);
}

void ewol::context::Dimension::setPixelRatio(const vec2& _ratio, enum ewol::context::Dimension::distance _type) {
	ewol::context::Dimension::init();
	EWOL_INFO("Set a new screen ratio for the screen : ratio=" << _ratio << " type=" << _type);
	ewol::context::Dimension conversion(_ratio, _type);
	EWOL_INFO("     == > " << conversion);
	ratio = conversion.getMillimeter();
	invRatio.setValue(1.0f/ratio.x(),1.0f/ratio.y());
	EWOL_INFO("Set a new screen ratio for the screen : ratioMm=" << ratio);
}

void ewol::context::Dimension::setPixelWindowsSize(const vec2& _size) {
	windowsSize = _size;
	EWOL_VERBOSE("Set a new Windows property size " << windowsSize << "px");
}

vec2 ewol::context::Dimension::getWindowsSize(enum ewol::context::Dimension::distance _type) {
	return windowsSize.get(_type);
}

float ewol::context::Dimension::getWindowsDiag(enum ewol::context::Dimension::distance _type) {
	vec2 size = ewol::context::Dimension::getWindowsSize(_type);
	return size.length();
}

ewol::context::Dimension::Dimension(void) :
  m_data(0,0),
  m_type(ewol::context::Dimension::Pixel) {
	// notinh to do ...
}

ewol::context::Dimension::Dimension(const vec2& _size, enum ewol::context::Dimension::distance _type) :
  m_data(0,0),
  m_type(ewol::context::Dimension::Pixel) {
	set(_size, _type);
}

void ewol::context::Dimension::set(std::string _config) {
	m_data.setValue(0,0);
	m_type = ewol::context::Dimension::Pixel;
	enum distance type = ewol::context::Dimension::Pixel;
	if (end_with(_config, "%", false) == true) {
		type = ewol::context::Dimension::Pourcent;
		_config.erase(_config.size()-1, 1);
	} else if (end_with(_config, "px",false) == true) {
		type = ewol::context::Dimension::Pixel;
		_config.erase(_config.size()-2, 2);
	} else if (end_with(_config, "ft",false) == true) {
		type = ewol::context::Dimension::foot;
		_config.erase(_config.size()-2, 2);
	} else if (end_with(_config, "in",false) == true) {
		type = ewol::context::Dimension::Inch;
		_config.erase(_config.size()-2, 2);
	} else if (end_with(_config, "km",false) == true) {
		type = ewol::context::Dimension::Kilometer;
		_config.erase(_config.size()-2, 2);
	} else if (end_with(_config, "mm",false) == true) {
		type = ewol::context::Dimension::Millimeter;
		_config.erase(_config.size()-2, 2);
	} else if (end_with(_config, "cm",false) == true) {
		type = ewol::context::Dimension::Centimeter;
		_config.erase(_config.size()-2, 2);
	} else if (end_with(_config, "m",false) == true) {
		type = ewol::context::Dimension::Meter;
		_config.erase(_config.size()-1, 1);
	} else {
		EWOL_CRITICAL("Can not parse dimention : \"" << _config << "\"");
		return;
	}
	vec2 tmp = _config;
	set(tmp, type);
	EWOL_VERBOSE(" config dimention : \"" << _config << "\"  == > " << *this );
}

ewol::context::Dimension::~Dimension(void) {
	// nothing to do ...
}

ewol::context::Dimension::operator std::string(void) const {
	std::string str;
	str = get(getType());
	
	switch(getType()) {
		case ewol::context::Dimension::Pourcent:
			str += "%";
			break;
		case ewol::context::Dimension::Pixel:
			str += "px";
			break;
		case ewol::context::Dimension::Meter:
			str += "m";
			break;
		case ewol::context::Dimension::Centimeter:
			str += "cm";
			break;
		case ewol::context::Dimension::Millimeter:
			str += "mm";
			break;
		case ewol::context::Dimension::Kilometer:
			str += "km";
			break;
		case ewol::context::Dimension::Inch:
			str += "in";
			break;
		case ewol::context::Dimension::foot:
			str += "ft";
			break;
	}
	return str;
}

vec2 ewol::context::Dimension::get(enum ewol::context::Dimension::distance _type) const {
	switch(_type) {
		case ewol::context::Dimension::Pourcent:
			return getPourcent();
		case ewol::context::Dimension::Pixel:
			return getPixel();
		case ewol::context::Dimension::Meter:
			return getMeter();
		case ewol::context::Dimension::Centimeter:
			return getCentimeter();
		case ewol::context::Dimension::Millimeter:
			return getMillimeter();
		case ewol::context::Dimension::Kilometer:
			return getKilometer();
		case ewol::context::Dimension::Inch:
			return getInch();
		case ewol::context::Dimension::foot:
			return getFoot();
	}
	return vec2(0,0);
}

void ewol::context::Dimension::set(const vec2& _size, enum ewol::context::Dimension::distance _type) {
	// set min max on input to limit error : 
	vec2 size(etk_avg(0.0f,_size.x(),9999999.0f),
	          etk_avg(0.0f,_size.y(),9999999.0f));
	switch(_type) {
		case ewol::context::Dimension::Pourcent: {
			vec2 size2(etk_avg(0.0f,_size.x(),100.0f),
			           etk_avg(0.0f,_size.y(),100.0f));
			m_data = vec2(size2.x()*0.01f, size2.y()*0.01f);
			//EWOL_VERBOSE("Set % : " << size2 << "  == > " << m_data);
			break;
		}
		case ewol::context::Dimension::Pixel:
			m_data = size;
			break;
		case ewol::context::Dimension::Meter:
			m_data = vec2(size.x()*meterToMillimeter*ratio.x(), size.y()*meterToMillimeter*ratio.y());
			break;
		case ewol::context::Dimension::Centimeter:
			m_data = vec2(size.x()*centimeterToMillimeter*ratio.x(), size.y()*centimeterToMillimeter*ratio.y());
			break;
		case ewol::context::Dimension::Millimeter:
			m_data = vec2(size.x()*ratio.x(), size.y()*ratio.y());
			break;
		case ewol::context::Dimension::Kilometer:
			m_data = vec2(size.x()*kilometerToMillimeter*ratio.x(), size.y()*kilometerToMillimeter*ratio.y());
			break;
		case ewol::context::Dimension::Inch:
			m_data = vec2(size.x()*inchToMillimeter*ratio.x(), size.y()*inchToMillimeter*ratio.y());
			break;
		case ewol::context::Dimension::foot:
			m_data = vec2(size.x()*footToMillimeter*ratio.x(), size.y()*footToMillimeter*ratio.y());
			break;
	}
	m_type = _type;
}

vec2 ewol::context::Dimension::getPixel(void) const {
	if (m_type!=ewol::context::Dimension::Pourcent) {
		return m_data;
	} else {
		vec2 windDim = windowsSize.getPixel();
		vec2 res = vec2(windDim.x()*m_data.x(), windDim.y()*m_data.y());
		//EWOL_DEBUG("Get % : " << m_data << " / " << windDim << " == > " << res);
		return res;
	}
}

vec2 ewol::context::Dimension::getPourcent(void) const {
	if (m_type!=ewol::context::Dimension::Pourcent) {
		vec2 windDim = windowsSize.getPixel();
		//EWOL_DEBUG(" windows dimention : " /*<< windowsSize*/ << "  == > " << windDim << "px"); // ==> infinite loop ...
		//printf(" windows dimention : %f,%f", windDim.x(),windDim.y());
		//printf(" data : %f,%f", m_data.x(),m_data.y());
		return vec2((m_data.x()/windDim.x())*100.0f, (m_data.y()/windDim.y())*100.0f);
	}
	return vec2(m_data.x()*100.0f, m_data.y()*100.0f);;
}

vec2 ewol::context::Dimension::getMeter(void) const {
	return ewol::context::Dimension::getMillimeter()*millimeterToMeter;
}

vec2 ewol::context::Dimension::getCentimeter(void) const {
	return ewol::context::Dimension::getMillimeter()*millimeterToCentimeter;
}

vec2 ewol::context::Dimension::getMillimeter(void) const {
	return ewol::context::Dimension::getPixel()*invRatio;
}

vec2 ewol::context::Dimension::getKilometer(void) const {
	return ewol::context::Dimension::getMillimeter()*millimeterToKilometer;
}

vec2 ewol::context::Dimension::getInch(void) const {
	return ewol::context::Dimension::getMillimeter()*millimeterToInch;
}

vec2 ewol::context::Dimension::getFoot(void) const {
	return ewol::context::Dimension::getMillimeter()*millimeterToFoot;
}

etk::CCout& ewol::operator <<(etk::CCout& _os, enum ewol::context::Dimension::distance _obj) {
	switch(_obj) {
		case ewol::context::Dimension::Pourcent:
			_os << "%";
			break;
		case ewol::context::Dimension::Pixel:
			_os << "px";
			break;
		case ewol::context::Dimension::Meter:
			_os << "m";
			break;
		case ewol::context::Dimension::Centimeter:
			_os << "cm";
			break;
		case ewol::context::Dimension::Millimeter:
			_os << "mm";
			break;
		case ewol::context::Dimension::Kilometer:
			_os << "km";
			break;
		case ewol::context::Dimension::Inch:
			_os << "in";
			break;
		case ewol::context::Dimension::foot:
			_os << "ft";
			break;
	}
	return _os;
}

etk::CCout& ewol::operator <<(etk::CCout& _os, const ewol::context::Dimension& _obj) {
	_os << _obj.get(_obj.getType()) << _obj.getType();
	return _os;
}
