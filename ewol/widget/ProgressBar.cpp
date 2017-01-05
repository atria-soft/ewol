/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/ProgressBar.hpp>

#include <ewol/compositing/Drawing.hpp>
#include <ewol/widget/Manager.hpp>

const int32_t dotRadius = 6;

ewol::widget::ProgressBar::ProgressBar() :
  propertyValue(this, "value",
                      0.0f, 0.0f, 1.0f,
                      "Value of the progress bar",
                      &ewol::widget::ProgressBar::onChangePropertyValue),
  propertyTextColorFg(this, "color-bg",
                            etk::color::black,
                            "Background color",
                            &ewol::widget::ProgressBar::onChangePropertyTextColorFg),
  propertyTextColorBgOn(this, "color-on",
                              etk::Color<>(0x00, 0xFF, 0x00, 0xFF),
                              "Color of the true value",
                              &ewol::widget::ProgressBar::onChangePropertyTextColorBgOn),
  propertyTextColorBgOff(this, "color-off",
                               etk::color::none,
                               "Color of the false value",
                               &ewol::widget::ProgressBar::onChangePropertyTextColorBgOff) {
	addObjectType("ewol::widget::ProgressBar");
}

void ewol::widget::ProgressBar::init() {
	ewol::Widget::init();
	propertyCanFocus.set(true);
}

ewol::widget::ProgressBar::~ProgressBar() {
	
}

void ewol::widget::ProgressBar::calculateMinMaxSize() {
	vec2 tmpMin = propertyMinSize->getPixel();
	m_minSize.setValue( std::max(tmpMin.x(), 40.0f),
	                    std::max(tmpMin.y(), dotRadius*2.0f) );
	markToRedraw();
}

void ewol::widget::ProgressBar::onDraw() {
	m_draw.draw();
}

void ewol::widget::ProgressBar::onRegenerateDisplay() {
	if (needRedraw() == false) {
		return;
	}
	// clean the object list ...
	m_draw.clear();
	
	m_draw.setColor(propertyTextColorFg);
	
	int32_t tmpSizeX = m_size.x() - 10;
	int32_t tmpSizeY = m_size.y() - 10;
	int32_t tmpOriginX = 5;
	int32_t tmpOriginY = 5;
	m_draw.setColor(propertyTextColorBgOn);
	m_draw.setPos(vec3(tmpOriginX, tmpOriginY, 0) );
	m_draw.rectangleWidth(vec3(tmpSizeX*propertyValue, tmpSizeY, 0) );
	m_draw.setColor(propertyTextColorBgOff);
	m_draw.setPos(vec3(tmpOriginX+tmpSizeX*propertyValue, tmpOriginY, 0) );
	m_draw.rectangleWidth(vec3(tmpSizeX*(1.0-propertyValue), tmpSizeY, 0) );
	
	// TODO : Create a better progress Bar ...
	//m_draw.setColor(propertyTextColorFg);
	//m_draw.rectangleBorder( tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY, 1);
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


