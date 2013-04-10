/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/Dimension.h>
#include <ewol/debug.h>

// ratio in milimeter :
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
static const float millimeterToMeter = 1000.0f;
static const float millimeterToCentimeter = 10.0f;
static const float millimeterToKilometer = 1000000.0f;


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





ewol::Dimension::Dimension(const vec2& size, ewol::Dimension::distance_te type)
{
	Set(size, type);
}

ewol::Dimension::~Dimension(void)
{
	// nothing to do ...
}

vec2 ewol::Dimension::Get(ewol::Dimension::distance_te type)
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

void ewol::Dimension::Set(const vec2& size, ewol::Dimension::distance_te type)
{
	switch(type) {
		case ewol::Dimension::Pourcent:
			m_data = vec2(size.x()*0.01f, size.y()*0.01f);
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

vec2 ewol::Dimension::GetPixel(void)
{
	if (m_type!=ewol::Dimension::Pourcent) {
		return m_data;
	}
	vec2 windDim = windowsSize.GetPixel();
	return vec2(windDim.x()*m_data.x(), windDim.y()*m_data.y());
}

vec2 ewol::Dimension::GetPourcent(void)
{
	if (m_type!=ewol::Dimension::Pourcent) {
		vec2 windDim = windowsSize.GetPixel();
		return vec2(m_data.x()/windDim.x()*100.0f, m_data.y()/windDim.y()*100.0f);
	}
	return m_data*100.0f;
}

vec2 ewol::Dimension::GetMeter(void)
{
	return ewol::Dimension::GetMillimeter()*millimeterToMeter;
}

vec2 ewol::Dimension::GetCentimeter(void)
{
	return ewol::Dimension::GetMillimeter()*millimeterToMeter;
}

vec2 ewol::Dimension::GetMillimeter(void)
{
	return ewol::Dimension::GetPixel()*invRatio;
}

vec2 ewol::Dimension::GetKilometer(void)
{
	return ewol::Dimension::GetMillimeter()*millimeterToKilometer;
}

vec2 ewol::Dimension::GetInch(void)
{
	return ewol::Dimension::GetMillimeter()*millimeterToInch;
}

vec2 ewol::Dimension::GetFoot(void)
{
	return ewol::Dimension::GetMillimeter()*millimeterToFoot;
}


