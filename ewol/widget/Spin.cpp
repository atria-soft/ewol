/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#include <ewol/debug.h>
#include <ewol/ewol.h>
#include <ewol/widget/Spin.h>
#include <ewol/widget/ContextMenu.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Windows.h>

#undef __class__
#define __class__ "widget::Spin"


ewol::widget::Spin::Spin() :
  signalValue(*this, "value", "Spin value change"),
  signalValueDouble(*this, "valueDouble", "Spin value change value in 'double'"),
  m_value(*this, "value", 0, "Value of the Spin"),
  m_min(*this, "min", -9999999999, "Minimum value of the spin"),
  m_max(*this, "max", 9999999999, "Maximum value of the spin"),
  m_increment(*this, "increment", 1, "Increment value at each button event or keybord event"),
  m_mantis(*this, "mantis", 0, "fix-point mantis") {
	addObjectType("ewol::widget::Spin");
}

void ewol::widget::Spin::init(enum ewol::widget::spinPosition _mode,
                              const std::string& _shaperName) {
	EWOL_WARNING("init [START]");
	ewol::widget::SpinBase::init(_mode, _shaperName);
	markToRedraw();
	EWOL_WARNING("init [STOP]");
}

ewol::widget::Spin::~Spin() {
	
}

void ewol::widget::Spin::onParameterChangeValue(const ewol::parameter::Ref& _paramPointer) {
	ewol::widget::SpinBase::onParameterChangeValue(_paramPointer);
	if (_paramPointer == m_value) {
		markToRedraw();
		if (m_widgetEntry == nullptr) {
			EWOL_ERROR("Can not acces at entry ...");
			return;
		}
		checkValue(m_value.get());
	} else if (_paramPointer == m_min) {
		checkValue(m_value.get());
	} else if (_paramPointer == m_max) {
		checkValue(m_value.get());
	} else if (_paramPointer == m_increment) {
		
	} else if (_paramPointer == m_mantis) {
		
	}
}

void ewol::widget::Spin::updateGui() {
	EWOL_WARNING("updateGui [START]");
	ewol::widget::SpinBase::updateGui();
	if (m_widgetEntry != nullptr) {
		m_widgetEntry->signalUnBindAll(shared_from_this());
		
	}
	if (m_widgetButtonUp != nullptr) {
		m_widgetButtonUp->signalUnBindAll(shared_from_this());
		m_widgetButtonUp->signalPressed.bind(shared_from_this(), &ewol::widget::Spin::onCallbackUp);
	}
	if (m_widgetButtonDown != nullptr) {
		m_widgetButtonDown->signalUnBindAll(shared_from_this());
		m_widgetButtonDown->signalPressed.bind(shared_from_this(), &ewol::widget::Spin::onCallbackDown);
	}
	EWOL_WARNING("updateGui [STOP]");
	
}

void ewol::widget::Spin::checkValue(int64_t _value) {
	_value = std::avg(m_min.get(), _value, m_max.get());
	m_value.get() = _value;
	m_widgetEntry->setValue(etk::to_string(_value));
}

void ewol::widget::Spin::onCallbackUp() {
	int64_t value = m_value.get() + m_increment.get();
	checkValue(value);
}

void ewol::widget::Spin::onCallbackDown() {
	int64_t value = m_value.get() - m_increment.get();
	checkValue(value);
}

void ewol::widget::Spin::setValue(int64_t _val) {
	m_value.set(_val);
}

int64_t ewol::widget::Spin::getValue() const {
	return m_value;
}

void ewol::widget::Spin::setMinimum(int64_t _val) {
	m_min.set(_val);
}

int64_t ewol::widget::Spin::getMinimum() const {
	return m_min;
}

void ewol::widget::Spin::setMaximum(int64_t _val) {
	m_max.set(_val);
}

int64_t ewol::widget::Spin::getMaximum() const {
	return m_max;
}

void ewol::widget::Spin::setIncrement(int64_t _val) {
	m_increment.set(_val);
}

int64_t ewol::widget::Spin::getIncrement() const {
	return m_increment;
}

void ewol::widget::Spin::setMantis(int8_t _val) {
	m_mantis.set(_val);
}

int8_t ewol::widget::Spin::getMantis() const {
	return m_mantis;
}

