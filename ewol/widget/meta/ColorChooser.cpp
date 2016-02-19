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
  signalChange(*this, "change"),
  propertyValue(*this, "value", etk::color::white, "color to select") {
	addObjectType("ewol::widget::ColorChooser");
}

void ewol::widget::ColorChooser::init() {
	ewol::widget::Sizer::init(ewol::widget::Sizer::modeVert);
	propertyLockExpand.set(bvec2(true,true));
		m_widgetColorBar = ewol::widget::ColorBar::create();
			m_widgetColorBar->signalChange.connect(shared_from_this(), &ewol::widget::ColorChooser::onCallbackColorChange);
			m_widgetColorBar->propertyFill.set(bvec2(true,true));
			subWidgetAdd(m_widgetColorBar);
		
		etk::Color<> sliderColor;
		sliderColor = etk::color::black;
		
		m_widgetRed = ewol::widget::Slider::create();
			m_widgetRed->signalChange.connect(shared_from_this(), &ewol::widget::ColorChooser::onCallbackColorChangeRed);
			m_widgetRed->propertyExpand.set(bvec2(true,false));
			m_widgetRed->propertyFill.set(bvec2(true,false));
			m_widgetRed->propertyMinimum.set(0);
			m_widgetRed->propertyMaximum.set(255);
			sliderColor = etk::Color<>(0xFF, 0x00, 0x00, 0xFF);
			m_widgetRed->setColor(sliderColor);
			subWidgetAdd(m_widgetRed);
		m_widgetGreen = ewol::widget::Slider::create();
			m_widgetGreen->signalChange.connect(shared_from_this(), &ewol::widget::ColorChooser::onCallbackColorChangeGreen);
			m_widgetGreen->propertyExpand.set(bvec2(true,false));
			m_widgetGreen->propertyFill.set(bvec2(true,false));
			m_widgetGreen->propertyMinimum.set(0);
			m_widgetGreen->propertyMaximum.set(255);
			sliderColor = etk::Color<>(0x00, 0xFF, 0x00, 0xFF);
			m_widgetGreen->setColor(sliderColor);
			subWidgetAdd(m_widgetGreen);
		m_widgetBlue = ewol::widget::Slider::create();
			m_widgetBlue->signalChange.connect(shared_from_this(), &ewol::widget::ColorChooser::onCallbackColorChangeBlue);
			m_widgetBlue->propertyExpand.set(bvec2(true,false));
			m_widgetBlue->propertyFill.set(bvec2(true,false));
			m_widgetBlue->propertyMinimum.set(0);
			m_widgetBlue->propertyMaximum.set(255);
			sliderColor = etk::Color<>(0x00, 0x00, 0xFF, 0xFF);
			m_widgetBlue->setColor(sliderColor);
			subWidgetAdd(m_widgetBlue);
		m_widgetAlpha = ewol::widget::Slider::create();
			m_widgetAlpha->signalChange.connect(shared_from_this(), &ewol::widget::ColorChooser::onCallbackColorChangeAlpha);
			m_widgetAlpha->propertyExpand.set(bvec2(true,false));
			m_widgetAlpha->propertyFill.set(bvec2(true,false));
			m_widgetAlpha->propertyMinimum.set(0);
			m_widgetAlpha->propertyMaximum.set(255);
			subWidgetAdd(m_widgetAlpha);
}


ewol::widget::ColorChooser::~ColorChooser() {
	
}


void ewol::widget::ColorChooser::onPropertyChangeValue(const eproperty::Ref& _paramPointer) {
	ewol::widget::Sizer::onPropertyChangeValue(_paramPointer);
	if (_paramPointer == propertyValue) {
		if (m_widgetRed != nullptr) {
			m_widgetRed->propertyValue.set(propertyValue->r());
		}
		if (m_widgetGreen != nullptr) {
			m_widgetGreen->propertyValue.set(propertyValue->g());
		}
		if (m_widgetBlue != nullptr) {
			m_widgetBlue->propertyValue.set(propertyValue->b());
		}
		if (m_widgetAlpha != nullptr) {
			m_widgetAlpha->propertyValue.set(propertyValue->a());
		}
		if (m_widgetColorBar != nullptr) {
			m_widgetColorBar->propertyValue.set(propertyValue);
		}
	}
}

void ewol::widget::ColorChooser::onCallbackColorChangeRed(const float& _newColor) {
	propertyValue.get().setR(_newColor);
	if (m_widgetColorBar != nullptr) {
		m_widgetColorBar->propertyValue.set(propertyValue);
	}
	signalChange.emit(propertyValue);
}

void ewol::widget::ColorChooser::onCallbackColorChangeGreen(const float& _newColor) {
	propertyValue.get().setG(_newColor);
	if (m_widgetColorBar != nullptr) {
		m_widgetColorBar->propertyValue.set(propertyValue);
	}
	signalChange.emit(propertyValue);
}

void ewol::widget::ColorChooser::onCallbackColorChangeBlue(const float& _newColor) {
	propertyValue.get().setB(_newColor);
	if (m_widgetColorBar != nullptr) {
		m_widgetColorBar->propertyValue.set(propertyValue);
	}
	signalChange.emit(propertyValue);
}

void ewol::widget::ColorChooser::onCallbackColorChangeAlpha(const float& _newColor) {
	propertyValue.get().setA(_newColor);
	if (m_widgetColorBar != nullptr) {
		m_widgetColorBar->propertyValue.set(propertyValue);
	}
	signalChange.emit(propertyValue);
}

void ewol::widget::ColorChooser::onCallbackColorChange(const etk::Color<>& _newColor) {
	// == > colorBar has change ...
	uint8_t tmpAlpha = propertyValue->a();
	propertyValue.get() = _newColor;
	propertyValue.get().setA(tmpAlpha);
	if (m_widgetRed != nullptr) {
		m_widgetRed->propertyValue.set(propertyValue->r());
	}
	if (m_widgetGreen != nullptr) {
		m_widgetGreen->propertyValue.set(propertyValue->g());
	}
	if (m_widgetBlue != nullptr) {
		m_widgetBlue->propertyValue.set(propertyValue->b());
	}
	if (m_widgetAlpha != nullptr) {
		m_widgetAlpha->propertyValue.set(propertyValue->a());
	}
	signalChange.emit(propertyValue);
}