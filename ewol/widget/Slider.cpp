/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/Slider.hpp>

#include <ewol/widget/Manager.hpp>

const int32_t dotRadius = 6;

ewol::widget::Slider::Slider() :
  signalChange(this, "change", ""),
  propertyValue(this, "value",
                      0.0f,
                      "Value of the Slider",
                      &ewol::widget::Slider::onChangePropertyValue),
  propertyMinimum(this, "min",
                        0.0f,
                        "Minium value",
                        &ewol::widget::Slider::onChangePropertyMinimum),
  propertyMaximum(this, "max",
                        10.0f,
                        "Maximum value",
                        &ewol::widget::Slider::onChangePropertyMaximum),
  propertyStep(this, "step",
                     1.0f,
                     "Step size",
                     &ewol::widget::Slider::onChangePropertyStep) {
	addObjectType("ewol::widget::Slider");
	
	m_textColorFg = etk::color::black;
	
	m_textColorBg = etk::color::black;
	m_textColorBg.setA(0x3F);
	
	propertyCanFocus.setDirectCheck(true);
	// Limit event at 1:
	setMouseLimit(1);
}

ewol::widget::Slider::~Slider() {
	
}

void ewol::widget::Slider::calculateMinMaxSize() {
	vec2 minTmp = propertyMinSize->getPixel();
	m_minSize.setValue(std::max(minTmp.x(), 40.0f),
	                   std::max(minTmp.y(), dotRadius*2.0f) );
	markToRedraw();
}

void ewol::widget::Slider::onDraw() {
	m_draw.draw();
}

void ewol::widget::Slider::onRegenerateDisplay() {
	if (needRedraw() == false) {
		return;
	}
	// clean the object list ...
	m_draw.clear();
	m_draw.setColor(m_textColorFg);
	// draw a line :
	m_draw.setThickness(1);
	m_draw.setPos(vec3(dotRadius, m_size.y()/2, 0) );
	m_draw.lineTo(vec3(m_size.x()-dotRadius, m_size.y()/2, 0) );
	m_draw.setThickness(0);
	
	etk::Color<> borderDot = m_textColorFg;
	borderDot.setA(borderDot.a()/2);
	m_draw.setPos(vec3(4+((propertyValue-propertyMinimum)/(propertyMaximum-propertyMinimum))*(m_size.x()-2*dotRadius), m_size.y()/2, 0) );
	m_draw.setColorBg(borderDot);
	m_draw.circle(dotRadius);
	m_draw.setColorBg(m_textColorFg);
	m_draw.circle(dotRadius/1.6);
}

bool ewol::widget::Slider::onEventInput(const ewol::event::Input& _event) {
	vec2 relativePos = relativePosition(_event.getPos());
	//EWOL_DEBUG("Event on Slider ..." << _event);
	if (1 == _event.getId()) {
		if(    gale::key::status::pressSingle == _event.getStatus()
		    || gale::key::status::move   == _event.getStatus()) {
			// get the new position :
			EWOL_VERBOSE("Event on Slider (" << relativePos.x() << "," << relativePos.y() << ")");
			float oldValue = *propertyValue;
			updateValue(*propertyMinimum + (float)(relativePos.x() - dotRadius) / (m_size.x()-2*dotRadius) * (*propertyMaximum-*propertyMinimum));
			if (oldValue != *propertyValue) {
				EWOL_VERBOSE(" new value : " << *propertyValue << " in [" << *propertyMinimum << ".." << *propertyMaximum << "]");
				signalChange.emit(*propertyValue);
			}
			return true;
		}
	}
	return false;
}

void ewol::widget::Slider::updateValue(float _newValue) {
	_newValue = std::max(std::min(_newValue, *propertyMaximum), *propertyMinimum);
	if (*propertyStep == 0.0f) {
		propertyValue.setDirect(_newValue);
	} else {
		float basicVal = (int64_t)(_newValue / *propertyStep);
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


