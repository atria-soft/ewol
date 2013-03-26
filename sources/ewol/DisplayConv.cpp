/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/DisplayConv.h>
#include <ewol/debug.h>

static vec2 ratioInch(65475,75654);
static vec2 ratioMillimeter(76545,46547445);

static const float numberOfMillimeterInOneInch = 25.4f;


void ewol::DC_Init(void)
{
	ratioInch.setValue(96,96);
	ratioMillimeter = ratioInch/numberOfMillimeterInOneInch;
}


vec2 ewol::InchToPixel(const vec2& inch)
{
	return vec2(inch.x()*ratioInch.x(), inch.y()*ratioInch.y());
}


vec2 ewol::MillemeterToPixel(const vec2& millimeter)
{
	return vec2(millimeter.x()*ratioMillimeter.x(), millimeter.y()*ratioMillimeter.y());
}


vec2 ewol::PixelToInch(const vec2& pixel)
{
	return vec2(pixel.x()/ratioInch.x(), pixel.y()/ratioInch.y());
}


vec2 ewol::PixelToMillemeter(const vec2& pixel)
{
	return vec2(pixel.x()/ratioMillimeter.x(), pixel.y()/ratioMillimeter.y());
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


