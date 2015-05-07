/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */


#include <ewol/ewol.h>
#include <ewol/widget/Button.h>

#undef __class__
#define __class__ "Button"


// DEFINE for the shader display system :
#define STATUS_UP        (0)
#define STATUS_HOVER     (2)
#define STATUS_PRESSED   (1)
#define STATUS_DOWN      (3)

ewol::widget::Button::Button() :
  signalPressed(*this, "pressed", "Button is pressed"),
  signalDown(*this, "down", "Button is DOWN"),
  signalUp(*this, "up", "Button is UP"),
  signalEnter(*this, "enter", "The cursor enter inside the button"),
  signalLeave(*this, "leave", "the cursor leave the button"),
  signalValue(*this, "value", "button value change"),
  m_shaper(*this, "shaper", "The display name for config file"),
  m_value(*this, "value", false, "Value of the Button"),
  m_lock(*this, "lock", lockNone, "Lock the button in a special state to permit changing state only by the coder"),
  m_toggleMode(*this, "toggle", false, "The Button can toogle"),
  m_enableSingle(*this, "enable-single", false, "If one element set in the Button ==> display only set"),
  m_mouseHover(false),
  m_buttonPressed(false),
  m_selectableAreaPos(0,0),
  m_selectableAreaSize(0,0) {
	addObjectType("ewol::widget::Button");
	
	// set property list:
	m_lock.add(lockNone, "none");
	m_lock.add(lockWhenPressed, "pressed");
	m_lock.add(lockWhenReleased, "released");
	m_lock.add(lockAccess, "access");
	
	// shaper satatus update:
	CheckStatus();
	// This widget can have the focus ...
	setCanHaveFocus(true);
	// Limit event at 1:
	setMouseLimit(1);
}

void ewol::widget::Button::init(const std::string& _shaperName) {
	ewol::widget::Container2::init();
	m_shaper->setSource(_shaperName);
}


ewol::widget::Button::~Button() {
	
}


void ewol::widget::Button::calculateSize(const vec2& _availlable) {
	ewol::Padding padding = m_shaper->getPadding();
	ewol::Padding ret = calculateSizePadded(_availlable, padding);
	//EWOL_DEBUG(" configuring : origin=" << origin << " size=" << subElementSize << "");
	m_selectableAreaPos = vec2(ret.xLeft(), ret.yButtom());
	m_selectableAreaSize = m_size - (m_selectableAreaPos + vec2(ret.xRight(), ret.yTop()));
}


void ewol::widget::Button::calculateMinMaxSize() {
	ewol::Padding padding = m_shaper->getPadding();
	calculateMinMaxSizePadded(padding);
}

void ewol::widget::Button::onDraw() {
	// draw the shaaper (if needed indeed)
	m_shaper->draw();
}

void ewol::widget::Button::onRegenerateDisplay() {
	ewol::widget::Container2::onRegenerateDisplay();
	if (needRedraw() == false) {
		return;
	}
	ewol::Padding padding = m_shaper->getPadding();
	m_shaper->setShape(vec2(0,0),
	                  m_size,
	                  vec2ClipInt32(m_selectableAreaPos+vec2(padding.xLeft(),padding.yButtom()) ),
	                  vec2ClipInt32(m_selectableAreaSize-vec2(padding.x(),padding.y()) ) );
	//EWOL_ERROR("pos=" << m_origin << " size=" << m_size);
}

bool ewol::widget::Button::onEventInput(const ewol::event::Input& _event) {
	EWOL_VERBOSE("Event on BT : " << _event);
	// disable event in the lock access mode :
	if(ewol::widget::Button::lockAccess == m_lock) {
		return false;
	}
	if(    ewol::key::statusLeave == _event.getStatus()
	    || ewol::key::statusAbort == _event.getStatus()) {
		m_mouseHover = false;
		m_buttonPressed = false;
	} else {
		vec2 relativePos = relativePosition(_event.getPos());
		// prevent error from ouside the button
		if(    relativePos.x() < m_selectableAreaPos.x()
		    || relativePos.y() < m_selectableAreaPos.y()
		    || relativePos.x() > m_selectableAreaPos.x() + m_selectableAreaSize.x()
		    || relativePos.y() > m_selectableAreaPos.y() + m_selectableAreaSize.y() ) {
			m_mouseHover = false;
			m_buttonPressed = false;
		} else {
			m_mouseHover = true;
		}
	}
	EWOL_VERBOSE("Event on BT ... mouse hover : " << m_mouseHover);
	if (true == m_mouseHover) {
		if (1 == _event.getId()) {
			if(ewol::key::statusDown == _event.getStatus()) {
				EWOL_VERBOSE(getName() << " : Generate event : " << signalDown);
				signalDown.emit();
				m_buttonPressed = true;
				markToRedraw();
			}
			if(ewol::key::statusUp == _event.getStatus()) {
				EWOL_VERBOSE(getName() << " : Generate event : " << signalUp);
				signalUp.emit();
				m_buttonPressed = false;
				markToRedraw();
			}
			if(ewol::key::statusSingle == _event.getStatus()) {
				if(    (    m_value.get() == true
				         && ewol::widget::Button::lockWhenPressed == m_lock)
				    || (    m_value.get() == false
				         && ewol::widget::Button::lockWhenReleased == m_lock) ) {
					// nothing to do : Lock mode ...
					// user might set himself the new correct value with @ref setValue(xxx)
				} else {
					// inverse value :
					setValue((m_value.get())?false:true);
					EWOL_VERBOSE(getName() << " : Generate event : " << signalPressed);
					signalPressed.emit();
					EWOL_VERBOSE(getName() << " : Generate event : " << signalValue << " val=" << m_value );
					signalValue.emit(m_value.get());
					if(    m_toggleMode.get() == false
					    && m_value.get() == true) {
						setValue(false);
						EWOL_VERBOSE(getName() << " : Generate event : " << signalValue << " val=" << m_value);
						signalValue.emit(m_value.get());
					}
				}
				markToRedraw();
			}
		}
	}
	CheckStatus();
	return m_mouseHover;
}


bool ewol::widget::Button::onEventEntry(const ewol::event::Entry& _event) {
	//EWOL_DEBUG("BT PRESSED : \"" << UTF8_data << "\" size=" << strlen(UTF8_data));
	if(    _event.getType() == ewol::key::keyboardChar
	    && _event.getStatus() == ewol::key::statusDown
	    && _event.getChar() == '\r') {
		signalEnter.emit();
		return true;
	}
	return false;
}

void ewol::widget::Button::onLostFocus() {
	m_buttonPressed = false;
	EWOL_VERBOSE(getName() << " : Remove Focus ...");
	CheckStatus();
}

void ewol::widget::Button::CheckStatus() {
	if (true == m_buttonPressed) {
		changeStatusIn(STATUS_PRESSED);
	} else {
		if (true == m_mouseHover) {
			changeStatusIn(STATUS_HOVER);
		} else {
			if (true == m_value) {
				changeStatusIn(STATUS_DOWN);
			} else {
				changeStatusIn(STATUS_UP);
			}
		}
	}
}

void ewol::widget::Button::changeStatusIn(int32_t _newStatusId) {
	if (true == m_shaper->changeStatusIn(_newStatusId) ) {
		periodicCallEnable();
		markToRedraw();
	}
}


void ewol::widget::Button::periodicCall(const ewol::event::Time& _event) {
	if (false == m_shaper->periodicCall(_event) ) {
		periodicCallDisable();
	}
	markToRedraw();
}

void ewol::widget::Button::onParameterChangeValue(const ewol::parameter::Ref& _paramPointer) {
	ewol::widget::Container2::onParameterChangeValue(_paramPointer);
	if (_paramPointer == m_shaper) {
		markToRedraw();
	} else if (_paramPointer == m_value) {
		if (m_toggleMode == true) {
			if (m_value.get() == false) {
				m_idWidgetDisplayed = 0;
			} else {
				m_idWidgetDisplayed = 1;
			}
		}
		if (m_enableSingle == true) {
			if (    m_idWidgetDisplayed == 0
			     && m_subWidget[0] == nullptr
			     && m_subWidget[1] != nullptr) {
				m_idWidgetDisplayed = 1;
			} else if (    m_idWidgetDisplayed == 1
			            && m_subWidget[1] == nullptr
			            && m_subWidget[0] != nullptr) {
				m_idWidgetDisplayed = 0;
			}
		}
		CheckStatus();
		markToRedraw();
	} else if (_paramPointer == m_lock) {
		if(ewol::widget::Button::lockAccess == m_lock.get()) {
			m_buttonPressed = false;
			m_mouseHover = false;
		}
		CheckStatus();
		markToRedraw();
	} else if (_paramPointer == m_toggleMode) {
		if (m_value.get() == true) {
			m_value.get() = false;
			// TODO : change display and send event ...
		}
		if (m_toggleMode.get() == false) {
			m_idWidgetDisplayed = 0;
		} else {
			if (m_value.get() == false) {
				m_idWidgetDisplayed = 0;
			} else {
				m_idWidgetDisplayed = 1;
			}
		}
		if (m_enableSingle.get() == true) {
			if (    m_idWidgetDisplayed == 0
			     && m_subWidget[0] == nullptr
			     && m_subWidget[1] != nullptr) {
				m_idWidgetDisplayed = 1;
			} else if (    m_idWidgetDisplayed == 1
			            && m_subWidget[1] == nullptr
			            && m_subWidget[0] != nullptr) {
				m_idWidgetDisplayed = 0;
			}
		}
		CheckStatus();
		markToRedraw();
	} else if (_paramPointer == m_enableSingle) {
		if (m_enableSingle == true) {
			if (    m_idWidgetDisplayed == 0
			     && m_subWidget[0] == nullptr
			     && m_subWidget[1] != nullptr) {
				m_idWidgetDisplayed = 1;
			} else if (    m_idWidgetDisplayed == 1
			            && m_subWidget[1] == nullptr
			            && m_subWidget[0] != nullptr) {
				m_idWidgetDisplayed = 0;
			} else if (    m_subWidget[0] == nullptr
			            && m_subWidget[1] == nullptr) {
				m_idWidgetDisplayed = 0;
			}
		}
	}
}
