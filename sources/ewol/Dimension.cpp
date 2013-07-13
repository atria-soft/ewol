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

// TODO : Set this in a super class acced in a statin fuction...
// ratio in milimeter :
static bool isInit = false;
static vec2 ratio(9999999,888888);
static vec2 invRatio(1,1);
static ewol::Dimension windowsSize(vec2(9999999,888888), ewol::Dimension::Pixel);

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


void ewol::dimension::Init(void)
{
	if (true==isInit) {
		return;
	}
	ewol::Dimension conversion(vec2(72,72), ewol::Dimension::Inch);
	ratio = conversion.GetMillimeter();
	invRatio.setValue(1.0f/ratio.x(),1.0f/ratio.y());
	windowsSize.Set(vec2(200,200), ewol::Dimension::Pixel);
	isInit = true;
}

void ewol::dimension::UnInit(void)
{
	isInit = false;
	ratio.setValue(9999999,888888);
	invRatio.setValue(1.0f/ratio.x(),1.0f/ratio.y());
	windowsSize.Set(vec2(9999999,88888), ewol::Dimension::Pixel);
}

void ewol::dimension::SetPixelRatio(const vec2& _ratio, ewol::Dimension::distance_te _type)
{
	ewol::dimension::Init();
	EWOL_INFO("Set a new screen ratio for the screen : ratio=" << _ratio << " type=" << _type);
	ewol::Dimension conversion(_ratio, _type);
	EWOL_INFO("    ==> " << conversion);
	ratio = conversion.GetMillimeter();
	invRatio.setValue(1.0f/ratio.x(),1.0f/ratio.y());
	EWOL_INFO("Set a new screen ratio for the screen : ratioMm=" << ratio);
}

void ewol::dimension::SetPixelWindowsSize(const vec2& _size)
{
	windowsSize = _size;
	EWOL_VERBOSE("Set a new Windows property size " << windowsSize << "px");
}

vec2 ewol::dimension::GetWindowsSize(ewol::Dimension::distance_te _type)
{
	return windowsSize.Get(_type);
}

float ewol::dimension::GetWindowsDiag(ewol::Dimension::distance_te _type)
{
	vec2 size = ewol::dimension::GetWindowsSize(_type);
	return size.length();
}





ewol::Dimension::Dimension(void) :
	m_data(0,0),
	m_type(ewol::Dimension::Pixel)
{
	// notinh to do ...
}

ewol::Dimension::Dimension(const vec2& _size, ewol::Dimension::distance_te _type) :
	m_data(0,0),
	m_type(ewol::Dimension::Pixel)
{
	Set(_size, _type);
}

void ewol::Dimension::Set(etk::UString _config)
{
	m_data.setValue(0,0);
	m_type = ewol::Dimension::Pixel;
	distance_te type = ewol::Dimension::Pixel;
	if (_config.EndWith("%",false)==true) {
		type = ewol::Dimension::Pourcent;
		_config.Remove(_config.Size()-1, 1);
	} else if (_config.EndWith("px",false)==true) {
		type = ewol::Dimension::Pixel;
		_config.Remove(_config.Size()-2, 2);
	} else if (_config.EndWith("ft",false)==true) {
		type = ewol::Dimension::foot;
		_config.Remove(_config.Size()-2, 2);
	} else if (_config.EndWith("in",false)==true) {
		type = ewol::Dimension::Inch;
		_config.Remove(_config.Size()-2, 2);
	} else if (_config.EndWith("km",false)==true) {
		type = ewol::Dimension::Kilometer;
		_config.Remove(_config.Size()-2, 2);
	} else if (_config.EndWith("mm",false)==true) {
		type = ewol::Dimension::Millimeter;
		_config.Remove(_config.Size()-2, 2);
	} else if (_config.EndWith("cm",false)==true) {
		type = ewol::Dimension::Centimeter;
		_config.Remove(_config.Size()-2, 2);
	} else if (_config.EndWith("m",false)==true) {
		type = ewol::Dimension::Meter;
		_config.Remove(_config.Size()-1, 1);
	} else {
		EWOL_CRITICAL("Can not parse dimention : \"" << _config << "\"");
		return;
	}
	vec2 tmp = _config;
	Set(tmp, type);
	EWOL_VERBOSE(" config dimention : \"" << _config << "\" ==> " << *this );
}

ewol::Dimension::~Dimension(void)
{
	// nothing to do ...
}

ewol::Dimension::operator etk::UString(void) const
{
	etk::UString str;
	str = Get(GetType());
	
	switch(GetType()) {
		case ewol::Dimension::Pourcent:
			str += "%";
			break;
		case ewol::Dimension::Pixel:
			str += "px";
			break;
		case ewol::Dimension::Meter:
			str += "m";
			break;
		case ewol::Dimension::Centimeter:
			str += "cm";
			break;
		case ewol::Dimension::Millimeter:
			str += "mm";
			break;
		case ewol::Dimension::Kilometer:
			str += "km";
			break;
		case ewol::Dimension::Inch:
			str += "in";
			break;
		case ewol::Dimension::foot:
			str += "ft";
			break;
	}
	return str;
}

vec2 ewol::Dimension::Get(ewol::Dimension::distance_te _type) const
{
	switch(_type) {
		case ewol::Dimension::Pourcent:
			return GetPourcent();
		case ewol::Dimension::Pixel:
			return GetPixel();
		case ewol::Dimension::Meter:
			return GetMeter();
		case ewol::Dimension::Centimeter:
			return GetCentimeter();
		case ewol::Dimension::Millimeter:
			return GetMillimeter();
		case ewol::Dimension::Kilometer:
			return GetKilometer();
		case ewol::Dimension::Inch:
			return GetInch();
		case ewol::Dimension::foot:
			return GetFoot();
	}
}

void ewol::Dimension::Set(const vec2& _size, ewol::Dimension::distance_te _type)
{
	// Set min max on input to limit error : 
	vec2 size(etk_avg(0.0f,_size.x(),9999999.0f),
	          etk_avg(0.0f,_size.y(),9999999.0f));
	switch(_type) {
		case ewol::Dimension::Pourcent:
			{
				vec2 size2(etk_avg(0.0f,_size.x(),100.0f),
				           etk_avg(0.0f,_size.y(),100.0f));
				m_data = vec2(size2.x()*0.01f, size2.y()*0.01f);
				//EWOL_VERBOSE("Set % : " << size2 << " ==> " << m_data);
			}
			break;
		case ewol::Dimension::Pixel:
			m_data = size;
			break;
		case ewol::Dimension::Meter:
			m_data = vec2(size.x()*meterToMillimeter*ratio.x(), size.y()*meterToMillimeter*ratio.y());
			break;
		case ewol::Dimension::Centimeter:
			m_data = vec2(size.x()*centimeterToMillimeter*ratio.x(), size.y()*centimeterToMillimeter*ratio.y());
			break;
		case ewol::Dimension::Millimeter:
			m_data = vec2(size.x()*ratio.x(), size.y()*ratio.y());
			break;
		case ewol::Dimension::Kilometer:
			m_data = vec2(size.x()*kilometerToMillimeter*ratio.x(), size.y()*kilometerToMillimeter*ratio.y());
			break;
		case ewol::Dimension::Inch:
			m_data = vec2(size.x()*inchToMillimeter*ratio.x(), size.y()*inchToMillimeter*ratio.y());
			break;
		case ewol::Dimension::foot:
			m_data = vec2(size.x()*footToMillimeter*ratio.x(), size.y()*footToMillimeter*ratio.y());
			break;
	}
	m_type = _type;
}

vec2 ewol::Dimension::GetPixel(void) const
{
	if (m_type!=ewol::Dimension::Pourcent) {
		return m_data;
	} else {
		vec2 windDim = windowsSize.GetPixel();
		vec2 res = vec2(windDim.x()*m_data.x(), windDim.y()*m_data.y());
		//EWOL_DEBUG("Get % : " << m_data << " / " << windDim << "==> " << res);
		return res;
	}
}

vec2 ewol::Dimension::GetPourcent(void) const
{
	if (m_type!=ewol::Dimension::Pourcent) {
		vec2 windDim = windowsSize.GetPixel();
		//EWOL_DEBUG(" windows dimention : " /*<< windowsSize*/ << " ==> " << windDim << "px"); // ==> infinite loop ...
		//printf(" windows dimention : %f,%f", windDim.x(),windDim.y());
		//printf(" data : %f,%f", m_data.x(),m_data.y());
		return vec2((m_data.x()/windDim.x())*100.0f, (m_data.y()/windDim.y())*100.0f);
	}
	return vec2(m_data.x()*100.0f, m_data.y()*100.0f);;
}

vec2 ewol::Dimension::GetMeter(void) const
{
	return ewol::Dimension::GetMillimeter()*millimeterToMeter;
}

vec2 ewol::Dimension::GetCentimeter(void) const
{
	return ewol::Dimension::GetMillimeter()*millimeterToCentimeter;
}

vec2 ewol::Dimension::GetMillimeter(void) const
{
	return ewol::Dimension::GetPixel()*invRatio;
}

vec2 ewol::Dimension::GetKilometer(void) const
{
	return ewol::Dimension::GetMillimeter()*millimeterToKilometer;
}

vec2 ewol::Dimension::GetInch(void) const
{
	return ewol::Dimension::GetMillimeter()*millimeterToInch;
}

vec2 ewol::Dimension::GetFoot(void) const
{
	return ewol::Dimension::GetMillimeter()*millimeterToFoot;
}

etk::CCout& ewol::operator <<(etk::CCout &os, const ewol::Dimension::distance_te& obj)
{
	switch(obj) {
		case ewol::Dimension::Pourcent:
			os << "%";
			break;
		case ewol::Dimension::Pixel:
			os << "px";
			break;
		case ewol::Dimension::Meter:
			os << "m";
			break;
		case ewol::Dimension::Centimeter:
			os << "cm";
			break;
		case ewol::Dimension::Millimeter:
			os << "mm";
			break;
		case ewol::Dimension::Kilometer:
			os << "km";
			break;
		case ewol::Dimension::Inch:
			os << "in";
			break;
		case ewol::Dimension::foot:
			os << "ft";
			break;
	}
	return os;
}

etk::CCout& ewol::operator <<(etk::CCout& _os, const ewol::Dimension& _obj)
{
	_os << _obj.Get(_obj.GetType()) << _obj.GetType();
	return _os;
}
