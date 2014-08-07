/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/ProgressBar.h>

#include <ewol/compositing/Drawing.h>
#include <ewol/widget/Manager.h>

#undef __class__
#define __class__	"ProgressBar"

const char* const ewol::widget::ProgressBar::configColorBg = "color-bg";
const char* const ewol::widget::ProgressBar::configColorFgOn = "color-on";
const char* const ewol::widget::ProgressBar::configColorFgOff = "color-off";
const char* const ewol::widget::ProgressBar::configValue = "value";

const int32_t dotRadius = 6;

ewol::widget::ProgressBar::ProgressBar() {
	addObjectType("ewol::widget::ProgressBar");
	m_value = 0.0;
	
	m_textColorFg = etk::color::black;
	
	m_textColorBgOn = etk::Color<>(0x00, 0xFF, 0x00, 0xFF);
	
	m_textColorBgOff = etk::color::black;
	m_textColorBgOff.setA(0x3F);
	setCanHaveFocus(true);
	registerConfig(configColorBg, "color", nullptr, "Background color");
	registerConfig(configColorFgOn, "color", nullptr, "Corlor of the true value");
	registerConfig(configColorFgOff, "color", nullptr, "Corlor of the false value");
	registerConfig(configValue, "integer", nullptr, "Value of the progress bar");
	
}

void ewol::widget::ProgressBar::init() {
	ewol::Widget::init();
}

ewol::widget::ProgressBar::~ProgressBar() {
	
}

void ewol::widget::ProgressBar::calculateMinMaxSize() {
	vec2 tmpMin = m_userMinSize.getPixel();
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



bool ewol::widget::ProgressBar::onSetConfig(const ewol::object::Config& _conf) {
	if (true == ewol::Widget::onSetConfig(_conf)) {
		return true;
	}
	if (_conf.getConfig() == configColorBg) {
		m_textColorFg = _conf.getData();
		markToRedraw();
		return true;
	}
	if (_conf.getConfig() == configColorFgOn) {
		m_textColorBgOn = _conf.getData();
		markToRedraw();
		return true;
	}
	if (_conf.getConfig() == configColorFgOff) {
		m_textColorBgOff = _conf.getData();
		markToRedraw();
		return true;
	}
	if (_conf.getConfig() == configValue) {
		m_value = stof(_conf.getData());
		markToRedraw();
		return true;
	}
	return false;
}

bool ewol::widget::ProgressBar::onGetConfig(const char* _config, std::string& _result) const {
	if (true == ewol::Widget::onGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configColorBg) {
		_result = m_textColorFg.getString();
		return true;
	}
	if (_config == configColorFgOn) {
		_result = m_textColorBgOn.getString();
		return true;
	}
	if (_config == configColorFgOff) {
		_result = m_textColorBgOff.getString();
		return true;
	}
	if (_config == configValue) {
		_result = m_value;
		return true;
	}
	return false;
}


