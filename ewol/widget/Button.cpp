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


// DEFINE for the shader display system:
const static int32_t STATUS_UP(0);
const static int32_t STATUS_HOVER(2);
const static int32_t STATUS_PRESSED(1);
const static int32_t STATUS_DOWN(3);

ewol::widget::Button::Button() :
  signalPressed(this, "pressed", "Button is pressed"),
  signalDown(this, "down", "Button is DOWN"),
  signalUp(this, "up", "Button is UP"),
  signalEnter(this, "enter", "The cursor enter inside the button"),
  signalLeave(this, "leave", "the cursor leave the button"),
  signalValue(this, "value", "button value change"),
  propertyShape(this, "shaper", "", "The display name for config file", &ewol::widget::Button::onChangePropertyShape),
  propertyValue(this, "value", false, "Value of the Button", &ewol::widget::Button::onChangePropertyValue),
  propertyLock(this, "lock", lockNone, "Lock the button in a special state to permit changing state only by the coder", &ewol::widget::Button::onChangePropertyLock),
  propertyToggleMode(this, "toggle", false, "The Button can toogle", &ewol::widget::Button::onChangePropertyToggleMode),
  propertyEnableSingle(this, "enable-single", false, "If one element set in the Button ==> display only set", &ewol::widget::Button::onChangePropertyEnableSingle),
  m_mouseHover(false),
  m_buttonPressed(false),
  m_selectableAreaPos(0,0),
  m_selectableAreaSize(0,0) {
	addObjectType("ewol::widget::Button");
	
	// set property list:
	propertyLock.add(lockNone, "none");
	propertyLock.add(lockWhenPressed, "pressed");
	propertyLock.add(lockWhenReleased, "released");
	propertyLock.add(lockAccess, "access");
	
	// shaper satatus update:
	CheckStatus();
	// Limit event at 1:
	setMouseLimit(1);
}

void ewol::widget::Button::init(const std::string& _shaperName) {
	ewol::widget::Container2::init();
	propertyCanFocus.set(true);
	propertyShape.set(_shaperName);
}

ewol::widget::Button::~Button() {
	
}

void ewol::widget::Button::onChangeSize() {
	ewol::Padding padding = m_shaper.getPadding();
	ewol::Padding ret = onChangeSizePadded(padding);
	//EWOL_DEBUG(" configuring : origin=" << origin << " size=" << subElementSize << "");
	m_selectableAreaPos = vec2(ret.xLeft(), ret.yButtom());
	m_selectableAreaSize = m_size - (m_selectableAreaPos + vec2(ret.xRight(), ret.yTop()));
}


void ewol::widget::Button::calculateMinMaxSize() {
	ewol::Padding padding = m_shaper.getPadding();
	calculateMinMaxSizePadded(padding);
}

void ewol::widget::Button::onDraw() {
	// draw the shaaper (if needed indeed)
	m_shaper.draw();
}

void ewol::widget::Button::onRegenerateDisplay() {
	ewol::widget::Container2::onRegenerateDisplay();
	if (needRedraw() == false) {
		return;
	}
	ewol::Padding padding = m_shaper.getPadding();
	m_shaper.setShape(vec2(0,0),
	                  m_size,
	                  vec2ClipInt32(m_selectableAreaPos+vec2(padding.xLeft(),padding.yButtom()) ),
	                  vec2ClipInt32(m_selectableAreaSize-vec2(padding.x(),padding.y()) ) );
	//EWOL_ERROR("pos=" << m_origin << " size=" << m_size);
}

bool ewol::widget::Button::onEventInput(const ewol::event::Input& _event) {
	EWOL_VERBOSE("Event on BT : " << _event);
	// disable event in the lock access mode :
	if(ewol::widget::Button::lockAccess == *propertyLock) {
		return false;
	}
	if(    gale::key::status_leave == _event.getStatus()
	    || gale::key::status_abort == _event.getStatus()) {
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
			if(gale::key::status_down == _event.getStatus()) {
				EWOL_VERBOSE(*propertyName << " : Generate event : " << signalDown);
				signalDown.emit();
				m_buttonPressed = true;
				markToRedraw();
			}
			if(gale::key::status_up == _event.getStatus()) {
				EWOL_VERBOSE(*propertyName << " : Generate event : " << signalUp);
				signalUp.emit();
				m_buttonPressed = false;
				markToRedraw();
			}
			if(gale::key::status_single == _event.getStatus()) {
				if(    (    *propertyValue == true
				         && ewol::widget::Button::lockWhenPressed == propertyLock)
				    || (    *propertyValue == false
				         && ewol::widget::Button::lockWhenReleased == propertyLock) ) {
					// nothing to do : Lock mode ...
					// user might set himself the new correct value with @ref setValue(xxx)
				} else {
					// inverse value :
					propertyValue.set((*propertyValue)?false:true);
					EWOL_VERBOSE(*propertyName << " : Generate event : " << signalPressed);
					signalPressed.emit();
					EWOL_VERBOSE(*propertyName << " : Generate event : " << signalValue << " val=" << *propertyValue );
					signalValue.emit(*propertyValue);
					if(    *propertyToggleMode == false
					    && *propertyValue == true) {
						propertyValue.set(false);
						EWOL_VERBOSE(*propertyName << " : Generate event : " << signalValue << " val=" << *propertyValue);
						signalValue.emit(*propertyValue);
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
	if(    _event.getType() == gale::key::keyboard_char
	    && _event.getStatus() == gale::key::status_down
	    && _event.getChar() == '\r') {
		signalEnter.emit();
		return true;
	}
	return false;
}

void ewol::widget::Button::onLostFocus() {
	m_buttonPressed = false;
	EWOL_VERBOSE(propertyName.get() << " : Remove Focus ...");
	CheckStatus();
}

void ewol::widget::Button::CheckStatus() {
	if (m_buttonPressed == true) {
		changeStatusIn(STATUS_PRESSED);
		return;
	}
	if (m_mouseHover == true) {
		changeStatusIn(STATUS_HOVER);
		return;
	}
	if (*propertyValue == true) {
		changeStatusIn(STATUS_DOWN);
	}
	changeStatusIn(STATUS_UP);
}

void ewol::widget::Button::changeStatusIn(int32_t _newStatusId) {
	if (m_shaper.changeStatusIn(_newStatusId) == true) {
		periodicCallEnable();
		markToRedraw();
	}
}


void ewol::widget::Button::periodicCall(const ewol::event::Time& _event) {
	if (m_shaper.periodicCall(_event) == false) {
		periodicCallDisable();
	}
	markToRedraw();
}

void ewol::widget::Button::onChangePropertyShape() {
	m_shaper.setSource(*propertyShape);
	markToRedraw();
}
void ewol::widget::Button::onChangePropertyValue() {
	if (*propertyToggleMode == true) {
		if (*propertyValue == false) {
			m_idWidgetDisplayed = 0;
		} else {
			m_idWidgetDisplayed = 1;
		}
	}
	if (*propertyEnableSingle == true) {
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
}

void ewol::widget::Button::onChangePropertyLock() {
	if(ewol::widget::Button::lockAccess == *propertyLock) {
		m_buttonPressed = false;
		m_mouseHover = false;
	}
	CheckStatus();
	markToRedraw();
}

void ewol::widget::Button::onChangePropertyToggleMode() {
	if (*propertyValue == true) {
		propertyValue.setDirect(false);
		// TODO : change display and send event ...
	}
	if (*propertyToggleMode == false) {
		m_idWidgetDisplayed = 0;
	} else {
		if (*propertyValue == false) {
			m_idWidgetDisplayed = 0;
		} else {
			m_idWidgetDisplayed = 1;
		}
	}
	if (*propertyEnableSingle == true) {
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
}

void ewol::widget::Button::onChangePropertyEnableSingle() {
	if (*propertyEnableSingle == true) {
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
