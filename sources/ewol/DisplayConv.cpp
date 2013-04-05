/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/DisplayConv.h>
#include <ewol/debug.h>

static vec2 ratioInch(9999999,888888);
static vec2 ratioMillimeter(9999999,888888);
static vec2 windowsSize(9999999,888888);

static const float numberOfMillimeterInOneInch = 25.4f;


void ewol::DC_Init(void)
{
	ratioInch.setValue(96,96);
	ratioMillimeter = ratioInch/numberOfMillimeterInOneInch;
	windowsSize.setValue(200,200);
}

vec2 ewol::InchToPixel(const vec2& inch)
{
	return vec2(inch.x()*ratioInch.x(), inch.y()*ratioInch.y());
}

vec2 ewol::MillemeterToPixel(const vec2& millimeter)
{
	return vec2(millimeter.x()*ratioMillimeter.x(), millimeter.y()*ratioMillimeter.y());
}

vec2 ewol::PourcentToPixel(const vec2& pourcent)
{
	return vec2(pourcent.x()*windowsSize.x()/100.0f, pourcent.y()*windowsSize.y()/100.0f);
}

vec2 ewol::PixelToInch(const vec2& pixel)
{
	return vec2(pixel.x()/ratioInch.x(), pixel.y()/ratioInch.y());
}

vec2 ewol::PixelToMillemeter(const vec2& pixel)
{
	return vec2(pixel.x()/ratioMillimeter.x(), pixel.y()/ratioMillimeter.y());
}

vec2 ewol::PixelToPourcent(const vec2& pixel)
{
	return vec2(pixel.x()*100.0f/windowsSize.x(), pixel.y()*100.0f/windowsSize.y());
}

void ewol::SetPixelPerInch(const vec2& ratio)
{
	ratioInch = ratio;
	ratioMillimeter = ratioInch/numberOfMillimeterInOneInch;
	EWOL_INFO("Set a new Inch/milimeter Ratio for the screen : ratioInch=" << ratioInch << " ratioMm=" << ratioMillimeter);
}

void ewol::SetPixelPerMillimeter(const vec2& ratio)
{
	ratioMillimeter = ratio;
	ratioInch = ratioMillimeter*numberOfMillimeterInOneInch;
	EWOL_INFO("Set a new Inch/milimeter Ratio for the screen : ratioInch=" << ratioInch << " ratioMm=" << ratioMillimeter);
}

void ewol::SetPixelWindowsSize(const vec2& size)
{
	windowsSize = size;
	EWOL_VERBOSE("Set a new Windows property size " << windowsSize << "px");
}

vec2 ewol::GetWindowsSizeMilimeter(void)
{
	return vec2(windowsSize.x()/ratioMillimeter.x(), windowsSize.y()/ratioMillimeter.y());
}

vec2 ewol::GetWindowsSizeInch(void)
{
	return vec2(windowsSize.x()/ratioInch.x(), windowsSize.y()/ratioInch.y());
}

float ewol::GetWindowsDiagSizeMilimeter(void)
{
	vec2 tmpSize = ewol::GetWindowsSizeMilimeter();
	return sqrtf(tmpSize.x()*tmpSize.x()+tmpSize.y()*tmpSize.y());
}

float ewol::GetWindowsDiagSizeInch(void)
{
	vec2 tmpSize = ewol::GetWindowsSizeInch();
	return sqrtf(tmpSize.x()*tmpSize.x()+tmpSize.y()*tmpSize.y());
}



