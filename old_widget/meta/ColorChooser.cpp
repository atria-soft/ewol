/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/meta/ColorChooser.hpp>
#include <ewol/widget/Sizer.hpp>
#include <ewol/widget/List.hpp>
#include <ewol/widget/Spacer.hpp>
#include <ewol/widget/Manager.hpp>
//#include <etk/Vector.hpp>
#include <etk/Vector.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::ColorChooser);

extern "C" {
	// file browsing ...
	#include <dirent.h>
}

ewol::widget::ColorChooser::ColorChooser() :
  signalChange(this, "change", ""),
  propertyValue(this, "value",
                      etk::color::white,
                      "color to select",
                      ewol::widget::ColorChooser::onChangePropertyValue) {
	addObjectType("ewol::widget::ColorChooser");
}

void ewol::widget::ColorChooser::init() {
	ewol::widget::Sizer::init();
	propertyMode.set(ewol::widget::Sizer::modeVert);
	propertyLockExpand.set(Vector2b(true,true));
		this.widgetColorBar = ewol::widget::ColorBar::create();
			this.widgetColorBar.signalChange.connect(sharedFromThis(), ewol::widget::ColorChooser::onCallbackColorChange);
			this.widgetColorBar.propertyFill.set(Vector2b(true,true));
			subWidgetAdd(this.widgetColorBar);
		
		etk::Color<> sliderColor;
		sliderColor = etk::color::black;
		
		this.widgetRed = ewol::widget::Slider::create();
			this.widgetRed.signalChange.connect(sharedFromThis(), ewol::widget::ColorChooser::onCallbackColorChangeRed);
			this.widgetRed.propertyExpand.set(Vector2b(true,false));
			this.widgetRed.propertyFill.set(Vector2b(true,false));
			this.widgetRed.propertyMinimum.set(0);
			this.widgetRed.propertyMaximum.set(255);
			sliderColor = etk::Color<>(0xFF, 0x00, 0x00, 0xFF);
			this.widgetRed.setColor(sliderColor);
			subWidgetAdd(this.widgetRed);
		this.widgetGreen = ewol::widget::Slider::create();
			this.widgetGreen.signalChange.connect(sharedFromThis(), ewol::widget::ColorChooser::onCallbackColorChangeGreen);
			this.widgetGreen.propertyExpand.set(Vector2b(true,false));
			this.widgetGreen.propertyFill.set(Vector2b(true,false));
			this.widgetGreen.propertyMinimum.set(0);
			this.widgetGreen.propertyMaximum.set(255);
			sliderColor = etk::Color<>(0x00, 0xFF, 0x00, 0xFF);
			this.widgetGreen.setColor(sliderColor);
			subWidgetAdd(this.widgetGreen);
		this.widgetBlue = ewol::widget::Slider::create();
			this.widgetBlue.signalChange.connect(sharedFromThis(), ewol::widget::ColorChooser::onCallbackColorChangeBlue);
			this.widgetBlue.propertyExpand.set(Vector2b(true,false));
			this.widgetBlue.propertyFill.set(Vector2b(true,false));
			this.widgetBlue.propertyMinimum.set(0);
			this.widgetBlue.propertyMaximum.set(255);
			sliderColor = etk::Color<>(0x00, 0x00, 0xFF, 0xFF);
			this.widgetBlue.setColor(sliderColor);
			subWidgetAdd(this.widgetBlue);
		this.widgetAlpha = ewol::widget::Slider::create();
			this.widgetAlpha.signalChange.connect(sharedFromThis(), ewol::widget::ColorChooser::onCallbackColorChangeAlpha);
			this.widgetAlpha.propertyExpand.set(Vector2b(true,false));
			this.widgetAlpha.propertyFill.set(Vector2b(true,false));
			this.widgetAlpha.propertyMinimum.set(0);
			this.widgetAlpha.propertyMaximum.set(255);
			subWidgetAdd(this.widgetAlpha);
}


ewol::widget::ColorChooser::~ColorChooser() {
	
}


void ewol::widget::ColorChooser::onChangePropertyValue() {
	if (this.widgetRed != null) {
		this.widgetRed.propertyValue.set(propertyValue.r());
	}
	if (this.widgetGreen != null) {
		this.widgetGreen.propertyValue.set(propertyValue.g());
	}
	if (this.widgetBlue != null) {
		this.widgetBlue.propertyValue.set(propertyValue.b());
	}
	if (this.widgetAlpha != null) {
		this.widgetAlpha.propertyValue.set(propertyValue.a());
	}
	if (this.widgetColorBar != null) {
		this.widgetColorBar.propertyValue.set(propertyValue);
	}
}

void ewol::widget::ColorChooser::onCallbackColorChangeRed( float _newColor) {
	propertyValue.getDirect().setR(_newColor);
	if (this.widgetColorBar != null) {
		this.widgetColorBar.propertyValue.set(propertyValue);
	}
	signalChange.emit(propertyValue);
}

void ewol::widget::ColorChooser::onCallbackColorChangeGreen( float _newColor) {
	propertyValue.getDirect().setG(_newColor);
	if (this.widgetColorBar != null) {
		this.widgetColorBar.propertyValue.set(propertyValue);
	}
	signalChange.emit(propertyValue);
}

void ewol::widget::ColorChooser::onCallbackColorChangeBlue( float _newColor) {
	propertyValue.getDirect().setB(_newColor);
	if (this.widgetColorBar != null) {
		this.widgetColorBar.propertyValue.set(propertyValue);
	}
	signalChange.emit(propertyValue);
}

void ewol::widget::ColorChooser::onCallbackColorChangeAlpha( float _newColor) {
	propertyValue.getDirect().setA(_newColor);
	if (this.widgetColorBar != null) {
		this.widgetColorBar.propertyValue.set(propertyValue);
	}
	signalChange.emit(propertyValue);
}

void ewol::widget::ColorChooser::onCallbackColorChange( etk::Color<> _newColor) {
	// == > colorBar has change ...
	int tmpAlpha = propertyValue.a();
	propertyValue.getDirect() = _newColor;
	propertyValue.getDirect().setA(tmpAlpha);
	if (this.widgetRed != null) {
		this.widgetRed.propertyValue.set(propertyValue.r());
	}
	if (this.widgetGreen != null) {
		this.widgetGreen.propertyValue.set(propertyValue.g());
	}
	if (this.widgetBlue != null) {
		this.widgetBlue.propertyValue.set(propertyValue.b());
	}
	if (this.widgetAlpha != null) {
		this.widgetAlpha.propertyValue.set(propertyValue.a());
	}
	signalChange.emit(propertyValue);
}