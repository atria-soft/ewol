/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
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

ewol::widget::ColorChooser::ColorChooser() :
  signalChange(this, "change", ""),
  propertyValue(this, "value",
                      etk::color::white,
                      "color to select",
                      &ewol::widget::ColorChooser::onChangePropertyValue) {
	addObjectType("ewol::widget::ColorChooser");
}

void ewol::widget::ColorChooser::init() {
	ewol::widget::Sizer::init();
	propertyMode.set(ewol::widget::Sizer::modeVert);
	propertyLockExpand.set(bvec2(true,true));
		m_widgetColorBar = ewol::widget::ColorBar::create();
			m_widgetColorBar->signalChange.connect(sharedFromThis(), &ewol::widget::ColorChooser::onCallbackColorChange);
			m_widgetColorBar->propertyFill.set(bvec2(true,true));
			subWidgetAdd(m_widgetColorBar);
		
		etk::Color<> sliderColor;
		sliderColor = etk::color::black;
		
		m_widgetRed = ewol::widget::Slider::create();
			m_widgetRed->signalChange.connect(sharedFromThis(), &ewol::widget::ColorChooser::onCallbackColorChangeRed);
			m_widgetRed->propertyExpand.set(bvec2(true,false));
			m_widgetRed->propertyFill.set(bvec2(true,false));
			m_widgetRed->propertyMinimum.set(0);
			m_widgetRed->propertyMaximum.set(255);
			sliderColor = etk::Color<>(0xFF, 0x00, 0x00, 0xFF);
			m_widgetRed->setColor(sliderColor);
			subWidgetAdd(m_widgetRed);
		m_widgetGreen = ewol::widget::Slider::create();
			m_widgetGreen->signalChange.connect(sharedFromThis(), &ewol::widget::ColorChooser::onCallbackColorChangeGreen);
			m_widgetGreen->propertyExpand.set(bvec2(true,false));
			m_widgetGreen->propertyFill.set(bvec2(true,false));
			m_widgetGreen->propertyMinimum.set(0);
			m_widgetGreen->propertyMaximum.set(255);
			sliderColor = etk::Color<>(0x00, 0xFF, 0x00, 0xFF);
			m_widgetGreen->setColor(sliderColor);
			subWidgetAdd(m_widgetGreen);
		m_widgetBlue = ewol::widget::Slider::create();
			m_widgetBlue->signalChange.connect(sharedFromThis(), &ewol::widget::ColorChooser::onCallbackColorChangeBlue);
			m_widgetBlue->propertyExpand.set(bvec2(true,false));
			m_widgetBlue->propertyFill.set(bvec2(true,false));
			m_widgetBlue->propertyMinimum.set(0);
			m_widgetBlue->propertyMaximum.set(255);
			sliderColor = etk::Color<>(0x00, 0x00, 0xFF, 0xFF);
			m_widgetBlue->setColor(sliderColor);
			subWidgetAdd(m_widgetBlue);
		m_widgetAlpha = ewol::widget::Slider::create();
			m_widgetAlpha->signalChange.connect(sharedFromThis(), &ewol::widget::ColorChooser::onCallbackColorChangeAlpha);
			m_widgetAlpha->propertyExpand.set(bvec2(true,false));
			m_widgetAlpha->propertyFill.set(bvec2(true,false));
			m_widgetAlpha->propertyMinimum.set(0);
			m_widgetAlpha->propertyMaximum.set(255);
			subWidgetAdd(m_widgetAlpha);
}


ewol::widget::ColorChooser::~ColorChooser() {
	
}


void ewol::widget::ColorChooser::onChangePropertyValue() {
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

void ewol::widget::ColorChooser::onCallbackColorChangeRed(const float& _newColor) {
	propertyValue.getDirect().setR(_newColor);
	if (m_widgetColorBar != nullptr) {
		m_widgetColorBar->propertyValue.set(propertyValue);
	}
	signalChange.emit(propertyValue);
}

void ewol::widget::ColorChooser::onCallbackColorChangeGreen(const float& _newColor) {
	propertyValue.getDirect().setG(_newColor);
	if (m_widgetColorBar != nullptr) {
		m_widgetColorBar->propertyValue.set(propertyValue);
	}
	signalChange.emit(propertyValue);
}

void ewol::widget::ColorChooser::onCallbackColorChangeBlue(const float& _newColor) {
	propertyValue.getDirect().setB(_newColor);
	if (m_widgetColorBar != nullptr) {
		m_widgetColorBar->propertyValue.set(propertyValue);
	}
	signalChange.emit(propertyValue);
}

void ewol::widget::ColorChooser::onCallbackColorChangeAlpha(const float& _newColor) {
	propertyValue.getDirect().setA(_newColor);
	if (m_widgetColorBar != nullptr) {
		m_widgetColorBar->propertyValue.set(propertyValue);
	}
	signalChange.emit(propertyValue);
}

void ewol::widget::ColorChooser::onCallbackColorChange(const etk::Color<>& _newColor) {
	// == > colorBar has change ...
	uint8_t tmpAlpha = propertyValue->a();
	propertyValue.getDirect() = _newColor;
	propertyValue.getDirect().setA(tmpAlpha);
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