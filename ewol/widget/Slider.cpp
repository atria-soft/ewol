/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/widget/Slider.h>

#include <ewol/widget/Manager.h>

#undef __class__
#define __class__ "Slider"

const int32_t dotRadius = 6;

ewol::widget::Slider::Slider() :
  signalChange(*this, "change"),
  m_value(*this, "value", 0.0f, "Value of the Slider"),
  m_min(*this, "min", 0.0f, "Minium value"),
  m_max(*this, "max", 10.0f, "Maximum value"),
  m_step(*this, "step", 1.0f, "Step size") {
	addObjectType("ewol::widget::Slider");
	
	m_textColorFg = etk::color::black;
	
	m_textColorBg = etk::color::black;
	m_textColorBg.setA(0x3F);
	setCanHaveFocus(true);
	// Limit event at 1:
	setMouseLimit(1);
}

void ewol::widget::Slider::init() {
	ewol::Widget::init();
}

ewol::widget::Slider::~Slider() {
	
}

void ewol::widget::Slider::calculateMinMaxSize() {
	vec2 minTmp = m_userMinSize->getPixel();
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
	m_draw.setPos(vec3(4+((m_value-m_min)/(m_max-m_min))*(m_size.x()-2*dotRadius), m_size.y()/2, 0) );
	m_draw.setColorBg(borderDot);
	m_draw.circle(dotRadius);
	m_draw.setColorBg(m_textColorFg);
	m_draw.circle(dotRadius/1.6);
}

bool ewol::widget::Slider::onEventInput(const ewol::event::Input& _event) {
	vec2 relativePos = relativePosition(_event.getPos());
	//EWOL_DEBUG("Event on Slider ..." << _event);
	if (1 == _event.getId()) {
		if(    gale::key::status_single == _event.getStatus()
		    || gale::key::status_move   == _event.getStatus()) {
			// get the new position :
			EWOL_VERBOSE("Event on Slider (" << relativePos.x() << "," << relativePos.y() << ")");
			float oldValue = m_value.get();
			updateValue(m_min + (float)(relativePos.x() - dotRadius) / (m_size.x()-2*dotRadius) * (m_max-m_min));
			if (oldValue != m_value) {
				EWOL_VERBOSE(" new value : " << m_value << " in [" << m_min << ".." << m_max << "]");
				signalChange.emit(m_value);
			}
			return true;
		}
	}
	return false;
}

void ewol::widget::Slider::updateValue(float _newValue) {
	_newValue = std::max(std::min(_newValue, m_max.get()), m_min.get());
	if (m_step.get() == 0.0f) {
		m_value = _newValue;
	} else {
		float basicVal = (int64_t)(_newValue / m_step.get());
		m_value = basicVal * m_step.get();
	}
	markToRedraw();
}

// TODO : Review this really bad things ...
void ewol::widget::Slider::onPropertyChangeValue(const eproperty::Ref& _paramPointer) {
	ewol::Widget::onPropertyChangeValue(_paramPointer);
	if (_paramPointer == m_value) {
		updateValue(m_value.get());
		return;
	}
	if (_paramPointer == m_min) {
		updateValue(m_value.get());
		return;
	}
	if (_paramPointer == m_max) {
		updateValue(m_value.get());
		return;
	}
	if (_paramPointer == m_step) {
		updateValue(m_value.get());
		return;
	}
}


