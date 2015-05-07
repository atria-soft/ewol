/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/widget/meta/ColorChooser.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/List.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Manager.h>
//#include <vector>
#include <vector>

extern "C" {
	// file browsing ...
	#include <dirent.h>
}

#undef __class__
#define __class__	"ColorChooser"

static const char * const eventColorBarHasChange          = "event-color-bar-has-change";


ewol::widget::ColorChooser::ColorChooser() :
  signalChange(*this, "change") {
	addObjectType("ewol::widget::ColorChooser");
}

void ewol::widget::ColorChooser::init() {
	ewol::widget::Sizer::init(ewol::widget::Sizer::modeVert);
	lockExpand(bvec2(true,true));
		m_widgetColorBar = ewol::widget::ColorBar::create();
			m_widgetColorBar->signalChange.bind(shared_from_this(), &ewol::widget::ColorChooser::onCallbackColorChange);
			m_widgetColorBar->setFill(bvec2(true,true));
			subWidgetAdd(m_widgetColorBar);
		
		etk::Color<> sliderColor;
		sliderColor = etk::color::black;
		
		m_widgetRed = ewol::widget::Slider::create();
			m_widgetRed->signalChange.bind(shared_from_this(), &ewol::widget::ColorChooser::onCallbackColorChangeRed);
			m_widgetRed->setExpand(bvec2(true,false));
			m_widgetRed->setFill(bvec2(true,false));
			m_widgetRed->setMin(0);
			m_widgetRed->setMax(255);
			sliderColor = etk::Color<>(0xFF, 0x00, 0x00, 0xFF);
			m_widgetRed->setColor(sliderColor);
			subWidgetAdd(m_widgetRed);
		m_widgetGreen = ewol::widget::Slider::create();
			m_widgetGreen->signalChange.bind(shared_from_this(), &ewol::widget::ColorChooser::onCallbackColorChangeGreen);
			m_widgetGreen->setExpand(bvec2(true,false));
			m_widgetGreen->setFill(bvec2(true,false));
			m_widgetGreen->setMin(0);
			sliderColor = etk::Color<>(0x00, 0xFF, 0x00, 0xFF);
			m_widgetGreen->setColor(sliderColor);
			m_widgetGreen->setMax(255);
			subWidgetAdd(m_widgetGreen);
		m_widgetBlue = ewol::widget::Slider::create();
			m_widgetBlue->signalChange.bind(shared_from_this(), &ewol::widget::ColorChooser::onCallbackColorChangeBlue);
			m_widgetBlue->setExpand(bvec2(true,false));
			m_widgetBlue->setFill(bvec2(true,false));
			m_widgetBlue->setMin(0);
			sliderColor = etk::Color<>(0x00, 0x00, 0xFF, 0xFF);
			m_widgetBlue->setColor(sliderColor);
			m_widgetBlue->setMax(255);
			subWidgetAdd(m_widgetBlue);
		m_widgetAlpha = ewol::widget::Slider::create();
			m_widgetAlpha->signalChange.bind(shared_from_this(), &ewol::widget::ColorChooser::onCallbackColorChangeAlpha);
			m_widgetAlpha->setExpand(bvec2(true,false));
			m_widgetAlpha->setFill(bvec2(true,false));
			m_widgetAlpha->setMin(0);
			m_widgetAlpha->setMax(255);
			subWidgetAdd(m_widgetAlpha);
	
	m_currentColor = etk::color::white;
}


ewol::widget::ColorChooser::~ColorChooser() {
	
}


void ewol::widget::ColorChooser::setColor(etk::Color<> _newColor) {
	m_currentColor = _newColor;
	if (nullptr != m_widgetRed) {
		m_widgetRed->setValue(m_currentColor.r());
	}
	if (nullptr != m_widgetGreen) {
		m_widgetGreen->setValue(m_currentColor.g());
	}
	if (nullptr != m_widgetBlue) {
		m_widgetBlue->setValue(m_currentColor.b());
	}
	if (nullptr != m_widgetAlpha) {
		m_widgetAlpha->setValue(m_currentColor.a());
	}
	if (nullptr != m_widgetColorBar) {
		m_widgetColorBar->setCurrentColor(m_currentColor);
	}
}


etk::Color<> ewol::widget::ColorChooser::getColor() {
	return m_currentColor;
}

void ewol::widget::ColorChooser::onCallbackColorChangeRed(const float& _newColor) {
	m_currentColor.setR(_newColor);
	if (nullptr != m_widgetColorBar) {
		m_widgetColorBar->setCurrentColor(m_currentColor);
	}
	signalChange.emit(m_currentColor);
}
void ewol::widget::ColorChooser::onCallbackColorChangeGreen(const float& _newColor) {
	m_currentColor.setG(_newColor);
	if (nullptr != m_widgetColorBar) {
		m_widgetColorBar->setCurrentColor(m_currentColor);
	}
	signalChange.emit(m_currentColor);
}
void ewol::widget::ColorChooser::onCallbackColorChangeBlue(const float& _newColor) {
	m_currentColor.setB(_newColor);
	if (nullptr != m_widgetColorBar) {
		m_widgetColorBar->setCurrentColor(m_currentColor);
	}
	signalChange.emit(m_currentColor);
}
void ewol::widget::ColorChooser::onCallbackColorChangeAlpha(const float& _newColor) {
	m_currentColor.setA(_newColor);
	if (nullptr != m_widgetColorBar) {
		m_widgetColorBar->setCurrentColor(m_currentColor);
	}
	signalChange.emit(m_currentColor);
}
void ewol::widget::ColorChooser::onCallbackColorChange(const etk::Color<>& _newColor) {
	m_currentColor = _newColor;
	// == > colorBar has change ...
	uint8_t tmpAlpha = m_currentColor.a();
	m_currentColor = _newColor;
	m_currentColor.setA(tmpAlpha);
	if (nullptr != m_widgetRed) {
		m_widgetRed->setValue(m_currentColor.r());
	}
	if (nullptr != m_widgetGreen) {
		m_widgetGreen->setValue(m_currentColor.g());
	}
	if (nullptr != m_widgetBlue) {
		m_widgetBlue->setValue(m_currentColor.b());
	}
	if (nullptr != m_widgetAlpha) {
		m_widgetAlpha->setValue(m_currentColor.a());
	}
	signalChange.emit(m_currentColor);
}