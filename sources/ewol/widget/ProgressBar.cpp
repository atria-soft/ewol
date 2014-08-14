/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/widget/ProgressBar.h>

#include <ewol/compositing/Drawing.h>
#include <ewol/widget/Manager.h>

#undef __class__
#define __class__	"ProgressBar"

const int32_t dotRadius = 6;

ewol::widget::ProgressBar::ProgressBar() :
  m_value(*this, "value", 0.0f, "Value of the progress bar"),
  m_textColorFg(*this, "color-bg", etk::color::black, "Background color"),
  m_textColorBgOn(*this, "color-on", etk::Color<>(0x00, 0xFF, 0x00, 0xFF), "Color of the true value"),
  m_textColorBgOff(*this, "color-off", etk::color::none, "Color of the false value") {
	addObjectType("ewol::widget::ProgressBar");
	setCanHaveFocus(true);
}

void ewol::widget::ProgressBar::init() {
	ewol::Widget::init();
}

ewol::widget::ProgressBar::~ProgressBar() {
	
}

void ewol::widget::ProgressBar::calculateMinMaxSize() {
	vec2 tmpMin = m_userMinSize->getPixel();
	m_minSize.setValue( std::max(tmpMin.x(), 40.0f),
	                    std::max(tmpMin.y(), dotRadius*2.0f) );
	markToRedraw();
}

void ewol::widget::ProgressBar::setValue(float _val) {
	m_value = std::avg(0.0f, _val, 1.0f);
	markToRedraw();
}

void ewol::widget::ProgressBar::onDraw() {
	m_draw.draw();
}

void ewol::widget::ProgressBar::onRegenerateDisplay() {
	if (true == needRedraw()) {
		// clean the object list ...
		m_draw.clear();
		
		m_draw.setColor(m_textColorFg);
		
		int32_t tmpSizeX = m_size.x() - 10;
		int32_t tmpSizeY = m_size.y() - 10;
		int32_t tmpOriginX = 5;
		int32_t tmpOriginY = 5;
		m_draw.setColor(m_textColorBgOn);
		m_draw.setPos(vec3(tmpOriginX, tmpOriginY, 0) );
		m_draw.rectangleWidth(vec3(tmpSizeX*m_value, tmpSizeY, 0) );
		m_draw.setColor(m_textColorBgOff);
		m_draw.setPos(vec3(tmpOriginX+tmpSizeX*m_value, tmpOriginY, 0) );
		m_draw.rectangleWidth(vec3(tmpSizeX*(1.0-m_value), tmpSizeY, 0) );
		
		// TODO : Create a better progress Bar ...
		//m_draw.setColor(m_textColorFg);
		//m_draw.rectangleBorder( tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY, 1);
	}
}

void ewol::widget::ProgressBar::onParameterChangeValue(const ewol::object::ParameterRef& _paramPointer) {
	ewol::Widget::onParameterChangeValue(_paramPointer);
	if (_paramPointer == m_value) {
		markToRedraw();
	} else if (_paramPointer == m_textColorFg) {
		markToRedraw();
	} else if (_paramPointer == m_textColorBgOn) {
		markToRedraw();
	} else if (_paramPointer == m_textColorBgOff) {
		markToRedraw();
	}
}


