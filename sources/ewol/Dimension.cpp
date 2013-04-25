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

// ratio in milimeter :
static vec2 ratio(9999999,888888);
static vec2 invRatio(1,1);
static ewol::Dimension windowsSize(vec2(9999999,888888), ewol::Dimension::Pixel);

static const float       inchToMillimeter = 25.4f;
static const float       footToMillimeter = 304.8f;
static const float      meterToMillimeter = 1000.0f;
static const float centimeterToMillimeter = 10.0f;
static const float  kilometerToMillimeter = 1000000.0f;
static const float millimeterToInch = 1.0f/25.4f;
static const float millimeterToFoot = 1.0f/304.8f;
static const float millimeterToMeter = 1.0f/1000.0f;
static const float millimeterToCentimeter = 1.0f/10.0f;
static const float millimeterToKilometer = 1.0f/1000000.0f;


void ewol::dimension::Init(void)
{
	ratio = vec2(96,96)*inchToMillimeter;
	invRatio.setValue(1.0f/ratio.x(),1.0f/ratio.y());
	windowsSize.Set(vec2(200,200), ewol::Dimension::Pixel);
}

void ewol::dimension::UnInit(void)
{
	ratio.setValue(9999999,888888);
	invRatio.setValue(1.0f/ratio.x(),1.0f/ratio.y());
	windowsSize.Set(vec2(9999999,88888), ewol::Dimension::Pixel);
}

void ewol::dimension::SetPixelRatio(const vec2& _ratio, ewol::Dimension::distance_te type)
{
	ewol::Dimension conversion(_ratio, type);
	ratio = conversion.GetMillimeter();
	invRatio.setValue(1.0f/ratio.x(),1.0f/ratio.y());
	EWOL_INFO("Set a new screen ratio for the screen : ratioMm=" << ratio);
}

void ewol::dimension::SetPixelWindowsSize(const vec2& size)
{
	windowsSize = size;
	EWOL_VERBOSE("Set a new Windows property size " << windowsSize << "px");
}

vec2 ewol::dimension::GetWindowsSize(ewol::Dimension::distance_te type)
{
	return windowsSize.Get(type);
}

float ewol::dimension::GetWindowsDiag(ewol::Dimension::distance_te type)
{
	vec2 size = ewol::dimension::GetWindowsSize(type);
	return size.length();
}





ewol::Dimension::Dimension(void) :
	m_data(0,0),
	m_type(ewol::Dimension::Pixel)
{
	// notinh to do ...
}

ewol::Dimension::Dimension(const vec2& size, ewol::Dimension::distance_te type)
{
	Set(size, type);
}

ewol::Dimension::~Dimension(void)
{
	// nothing to do ...
}

vec2 ewol::Dimension::Get(ewol::Dimension::distance_te type) const
{
	switch(m_type) {
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

void ewol::Dimension::Set(const vec2& _size, ewol::Dimension::distance_te type)
{
	// Set min max on input to limit error : 
	vec2 size(etk_avg(0.0f,_size.x(),9999999.0f),
	          etk_avg(0.0f,_size.y(),9999999.0f));
	switch(type) {
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
	m_type = type;
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

etk::UString ewol::Dimension::GetString(void)
{
	etk::UString ret;
	vec2 size = Get(GetType());
	ret += etk::UString(size.x());
	ret += ",";
	ret += etk::UString(size.y());
	switch(m_type) {
		case ewol::Dimension::Pourcent:
			ret += "%";
			break;
		case ewol::Dimension::Pixel:
			ret += "px";
			break;
		case ewol::Dimension::Meter:
			ret += "m";
			break;
		case ewol::Dimension::Centimeter:
			ret += "cm";
			break;
		case ewol::Dimension::Millimeter:
			ret += "mm";
			break;
		case ewol::Dimension::Kilometer:
			ret += "km";
			break;
		case ewol::Dimension::Inch:
			ret += "in";
			break;
		case ewol::Dimension::foot:
			ret += "ft";
			break;
	}
	return ret;
}


void ewol::Dimension::SetString(const etk::UString& value)
{
	etk::UString value2 = value;
	int32_t nbElementToRemove=0;
	vec2 data;
	distance_te type;
	if (value.EndWith("%")==true) {
		nbElementToRemove=1;
		type = ewol::Dimension::Pourcent;
	} else if (value.EndWith("px")==true) {
		nbElementToRemove=2;
		type = ewol::Dimension::Pixel;
	} else if (value.EndWith("cm")==true) {
		nbElementToRemove=2;
		type = ewol::Dimension::Centimeter;
	} else if (value.EndWith("mm")==true) {
		nbElementToRemove=2;
		type = ewol::Dimension::Millimeter;
	} else if (value.EndWith("km")==true) {
		nbElementToRemove=2;
		type = ewol::Dimension::Kilometer;
	} else if (value.EndWith("m")==true) {
		nbElementToRemove=1;
		type = ewol::Dimension::Meter;
	} else if (value.EndWith("in")==true) {
		nbElementToRemove=2;
		type = ewol::Dimension::Inch;
	} else if (value.EndWith("ft")==true) {
		nbElementToRemove=2;
		type = ewol::Dimension::foot;
	} else {
		EWOL_WARNING("you might st an unit at : \"" << value << "\"");
		nbElementToRemove=0;
		type = ewol::Dimension::Pixel;
	}
	value2.Remove(value2.Size()-nbElementToRemove, nbElementToRemove);
	data=value2;
	Set(data, type);
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

etk::CCout& ewol::operator <<(etk::CCout &os, const ewol::Dimension& obj)
{
	os << obj.Get(obj.GetType()) << obj.GetType();
	return os;
}