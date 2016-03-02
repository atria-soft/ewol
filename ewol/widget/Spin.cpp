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
  signalValue(this, "value",
                    "Spin value change"),
  signalValueDouble(this, "valueDouble",
                          "Spin value change value in 'double'"),
  propertyValue(this, "value",
                      0,
                      "Value of the Spin",
                      &ewol::widget::Spin::onChangePropertyValue),
  propertyMin(this, "min",
                    -9999999999,
                    "Minimum value of the spin",
                    &ewol::widget::Spin::onChangePropertyMin),
  propertyMax(this, "max",
                    9999999999,
                    "Maximum value of the spin",
                    &ewol::widget::Spin::onChangePropertyMax),
  propertyIncrement(this, "increment",
                          1,
                          "Increment value at each button event or keybord event",
                          &ewol::widget::Spin::onChangePropertyIncrement),
  propertyMantis(this, "mantis",
                       0,
                       "fix-point mantis",
                       &ewol::widget::Spin::onChangePropertyMantis) {
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

void ewol::widget::Spin::onChangePropertyValue() {
	markToRedraw();
	if (m_widgetEntry == nullptr) {
		EWOL_ERROR("Can not acces at entry ...");
		return;
	}
	checkValue(*propertyValue);
}

void ewol::widget::Spin::onChangePropertyMin() {
	checkValue(*propertyValue);
}

void ewol::widget::Spin::onChangePropertyMax() {
	checkValue(*propertyValue);
}

void ewol::widget::Spin::onChangePropertyIncrement() {
	
}

void ewol::widget::Spin::onChangePropertyMantis() {
	
}

void ewol::widget::Spin::updateGui() {
	EWOL_WARNING("updateGui [START]");
	ewol::widget::SpinBase::updateGui();
	
	if (    m_widgetEntry != nullptr
	     && m_connectionEntry.isConnected() == false) {
		
	}
	if (    m_widgetButtonUp != nullptr
	     && m_connectionButtonUp.isConnected() == false) {
		m_connectionButtonUp = m_widgetButtonUp->signalPressed.connect(this, &ewol::widget::Spin::onCallbackUp);
	}
	if (    m_widgetButtonDown != nullptr
	     && m_connectionButtonDown.isConnected() == false) {
		m_connectionButtonDown = m_widgetButtonDown->signalPressed.connect(this, &ewol::widget::Spin::onCallbackDown);
	}
	EWOL_WARNING("updateGui [STOP]");
}

void ewol::widget::Spin::checkValue(int64_t _value) {
	_value = std::avg(propertyMin.get(), _value, propertyMax.get());
	propertyValue.setDirect(_value);
	m_widgetEntry->propertyValue.set(etk::to_string(_value));
}

void ewol::widget::Spin::onCallbackUp() {
	int64_t value = propertyValue.get() + propertyIncrement.get();
	checkValue(value);
}

void ewol::widget::Spin::onCallbackDown() {
	int64_t value = propertyValue.get() - propertyIncrement.get();
	checkValue(value);
}
