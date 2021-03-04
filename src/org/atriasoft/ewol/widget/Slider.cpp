/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/Slider.hpp>

#include <ewol/widget/Manager.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::Slider);

 int dotRadius = 6;

ewol::widget::Slider::Slider() :
  signalChange(this, "change", ""),
  propertyValue(this, "value",
                      0.0f,
                      "Value of the Slider",
                      ewol::widget::Slider::onChangePropertyValue),
  propertyMinimum(this, "min",
                        0.0f,
                        "Minium value",
                        ewol::widget::Slider::onChangePropertyMinimum),
  propertyMaximum(this, "max",
                        10.0f,
                        "Maximum value",
                        ewol::widget::Slider::onChangePropertyMaximum),
  propertyStep(this, "step",
                     1.0f,
                     "Step size",
                     ewol::widget::Slider::onChangePropertyStep) {
	addObjectType("ewol::widget::Slider");
	
	this.textColorFg = etk::color::black;
	
	this.textColorBg = etk::color::black;
	this.textColorBg.setA(0x3F);
	
	propertyCanFocus.setDirectCheck(true);
	// Limit event at 1:
	setMouseLimit(1);
}

ewol::widget::Slider::~Slider() {
	
}

void ewol::widget::Slider::calculateMinMaxSize() {
	Vector2f minTmp = propertyMinSize.getPixel();
	this.minSize.setValue(etk::max(minTmp.x(), 40.0f),
	                   etk::max(minTmp.y(), dotRadius*2.0f) );
	markToRedraw();
}

void ewol::widget::Slider::onDraw() {
	this.draw.draw();
}

void ewol::widget::Slider::onRegenerateDisplay() {
	if (needRedraw() == false) {
		return;
	}
	// clean the object list ...
	this.draw.clear();
	this.draw.setColor(this.textColorFg);
	// draw a line :
	this.draw.setThickness(1);
	this.draw.setPos(Vector3f(dotRadius, this.size.y()/2, 0) );
	this.draw.lineTo(Vector3f(this.size.x()-dotRadius, this.size.y()/2, 0) );
	this.draw.setThickness(0);
	
	etk::Color<> borderDot = this.textColorFg;
	borderDot.setA(borderDot.a()/2);
	this.draw.setPos(Vector3f(4+((propertyValue-propertyMinimum)/(propertyMaximum-propertyMinimum))*(this.size.x()-2*dotRadius), this.size.y()/2, 0) );
	this.draw.setColorBg(borderDot);
	this.draw.circle(dotRadius);
	this.draw.setColorBg(this.textColorFg);
	this.draw.circle(dotRadius/1.6);
}

boolean ewol::widget::Slider::onEventInput( ewol::event::Input _event) {
	Vector2f relativePos = relativePosition(_event.getPos());
	//Log.debug("Event on Slider ..." + _event);
	if (1 == _event.getId()) {
		if(    KeyStatus::pressSingle == _event.getStatus()
		    || KeyStatus::move   == _event.getStatus()) {
			// get the new position :
			Log.verbose("Event on Slider (" + relativePos.x() + "," + relativePos.y() + ")");
			float oldValue = *propertyValue;
			updateValue(*propertyMinimum + (float)(relativePos.x() - dotRadius) / (this.size.x()-2*dotRadius) * (*propertyMaximum-*propertyMinimum));
			if (oldValue != *propertyValue) {
				Log.verbose(" new value : " + *propertyValue + " in [" + *propertyMinimum + ".." + *propertyMaximum + "]");
				signalChange.emit(*propertyValue);
			}
			return true;
		}
	}
	return false;
}

void ewol::widget::Slider::updateValue(float _newValue) {
	_newValue = etk::max(etk::min(_newValue, *propertyMaximum), *propertyMinimum);
	if (*propertyStep == 0.0f) {
		propertyValue.setDirect(_newValue);
	} else {
		float basicVal = (long)(_newValue / *propertyStep);
		propertyValue.setDirect(basicVal * *propertyStep);
	}
	markToRedraw();
}


void ewol::widget::Slider::onChangePropertyValue() {
	updateValue(*propertyValue);
	return;
}

void ewol::widget::Slider::onChangePropertyMinimum() {
	updateValue(*propertyValue);
	return;
}

void ewol::widget::Slider::onChangePropertyMaximum() {
	updateValue(*propertyValue);
	return;
}

void ewol::widget::Slider::onChangePropertyStep() {
	updateValue(*propertyValue);
	return;
}


