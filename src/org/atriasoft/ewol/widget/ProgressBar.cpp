/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/ProgressBar.hpp>

#include <ewol/compositing/Drawing.hpp>
#include <ewol/widget/Manager.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::ProgressBar);

 int dotRadius = 6;

ewol::widget::ProgressBar::ProgressBar() :
  propertyValue(this, "value",
                      0.0f, 0.0f, 1.0f,
                      "Value of the progress bar",
                      ewol::widget::ProgressBar::onChangePropertyValue),
  propertyTextColorFg(this, "color-bg",
                            etk::color::black,
                            "Background color",
                            ewol::widget::ProgressBar::onChangePropertyTextColorFg),
  propertyTextColorBgOn(this, "color-on",
                              etk::Color<>(0x00, 0xFF, 0x00, 0xFF),
                              "Color of the true value",
                              ewol::widget::ProgressBar::onChangePropertyTextColorBgOn),
  propertyTextColorBgOff(this, "color-off",
                               etk::color::none,
                               "Color of the false value",
                               ewol::widget::ProgressBar::onChangePropertyTextColorBgOff) {
	addObjectType("ewol::widget::ProgressBar");
}

void ewol::widget::ProgressBar::init() {
	Widget::init();
	propertyCanFocus.set(true);
}

ewol::widget::ProgressBar::~ProgressBar() {
	
}

void ewol::widget::ProgressBar::calculateMinMaxSize() {
	Vector2f tmpMin = propertyMinSize.getPixel();
	this.minSize.setValue( etk::max(tmpMin.x(), 40.0f),
	                    etk::max(tmpMin.y(), dotRadius*2.0f) );
	markToRedraw();
}

void ewol::widget::ProgressBar::onDraw() {
	this.draw.draw();
}

void ewol::widget::ProgressBar::onRegenerateDisplay() {
	if (needRedraw() == false) {
		return;
	}
	// clean the object list ...
	this.draw.clear();
	
	this.draw.setColor(propertyTextColorFg);
	
	int tmpSizeX = this.size.x() - 10;
	int tmpSizeY = this.size.y() - 10;
	int tmpOriginX = 5;
	int tmpOriginY = 5;
	this.draw.setColor(propertyTextColorBgOn);
	this.draw.setPos(Vector3f(tmpOriginX, tmpOriginY, 0) );
	this.draw.rectangleWidth(Vector3f(tmpSizeX*propertyValue, tmpSizeY, 0) );
	this.draw.setColor(propertyTextColorBgOff);
	this.draw.setPos(Vector3f(tmpOriginX+tmpSizeX*propertyValue, tmpOriginY, 0) );
	this.draw.rectangleWidth(Vector3f(tmpSizeX*(1.0-propertyValue), tmpSizeY, 0) );
	
	// TODO : Create a better progress Bar ...
	//this.draw.setColor(propertyTextColorFg);
	//this.draw.rectangleBorder( tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY, 1);
}

void ewol::widget::ProgressBar::onChangePropertyValue() {
	markToRedraw();
}

void ewol::widget::ProgressBar::onChangePropertyTextColorFg() {
	markToRedraw();
}

void ewol::widget::ProgressBar::onChangePropertyTextColorBgOn() {
	markToRedraw();
}

void ewol::widget::ProgressBar::onChangePropertyTextColorBgOff() {
	markToRedraw();
}


