/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/ProgressBar.h>

#include <ewol/compositing/Drawing.h>
#include <ewol/widget/WidgetManager.h>

#undef __class__
#define __class__	"ProgressBar"

static ewol::Widget* create(void) {
	return new widget::ProgressBar();
}

void widget::ProgressBar::init(ewol::WidgetManager& _widgetManager) {
	_widgetManager.addWidgetCreator(__class__,&create);
}


const int32_t dotRadius = 6;

widget::ProgressBar::ProgressBar(void) {
	m_value = 0.0;
	
	m_textColorFg = etk::color::black;
	
	m_textColorBgOn = 0x00FF00FF;
	
	m_textColorBgOff = etk::color::black;
	m_textColorBgOff.setA(0x3F);
	setCanHaveFocus(true);
}

widget::ProgressBar::~ProgressBar(void) {
	
}

void widget::ProgressBar::calculateMinMaxSize(void) {
	vec2 tmpMin = m_userMinSize.getPixel();
	m_minSize.setValue( etk_max(tmpMin.x(), 40),
	                    etk_max(tmpMin.y(), dotRadius*2) );
	markToRedraw();
}

void widget::ProgressBar::valueSet(float _val) {
	m_value = etk_avg(0, _val, 1);
	markToRedraw();
}

float widget::ProgressBar::valueGet(void) {
	return m_value;
}

void widget::ProgressBar::onDraw(void) {
	m_draw.draw();
}

void widget::ProgressBar::onRegenerateDisplay(void) {
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



