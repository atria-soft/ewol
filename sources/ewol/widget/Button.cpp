/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ewol/ewol.h>
#include <ewol/widget/Button.h>

#undef __class__
#define __class__ "Button"


const char* const ewol::widget::Button::eventPressed    = "pressed";
const char* const ewol::widget::Button::eventDown       = "down";
const char* const ewol::widget::Button::eventUp         = "up";
const char* const ewol::widget::Button::eventEnter      = "enter";
const char* const ewol::widget::Button::eventLeave      = "leave";
const char* const ewol::widget::Button::eventValue      = "value";

const char* const ewol::widget::Button::configToggle = "toggle";
const char* const ewol::widget::Button::configLock   = "lock";
const char* const ewol::widget::Button::configValue  = "value";
const char* const ewol::widget::Button::configShaper = "shaper";


// DEFINE for the shader display system :
#define STATUS_UP        (0)
#define STATUS_HOVER     (2)
#define STATUS_PRESSED   (1)
#define STATUS_DOWN      (3)


static ewol::Widget* Create(void) {
	return new ewol::widget::Button();
}

void ewol::widget::Button::init(ewol::widget::Manager& _widgetManager) {
	_widgetManager.addWidgetCreator(__class__,&Create);
}

ewol::widget::Button::Button(const std::string& _shaperName) :
  m_shaper(_shaperName),
  m_value(false),
  m_lock(ewol::widget::Button::lockNone),
  m_toggleMode(false),
  m_mouseHover(false),
  m_buttonPressed(false),
  m_selectableAreaPos(0,0),
  m_selectableAreaSize(0,0) {
	addObjectType("ewol::widget::Button");
	// add basic Event generated :
	addEventId(eventPressed);
	addEventId(eventDown);
	addEventId(eventUp);
	addEventId(eventEnter);
	addEventId(eventLeave);
	addEventId(eventValue);
	// add configuration
	registerConfig(configToggle, "bool", NULL, "The Button can toogle");
	registerConfig(configValue, "bool", NULL, "Basic value of the widget");
	registerConfig(configLock, "list", "none;true;released;pressed", "Lock the button in a special state to permit changing state only by the coder");
	registerConfig(configShaper, "string", NULL, "the display name for config file");
	
	// shaper satatus update:
	CheckStatus();
	// This widget can have the focus ...
	setCanHaveFocus(true);
	// Limit event at 1:
	setMouseLimit(1);
}


ewol::widget::Button::~Button(void) {
	
}


void ewol::widget::Button::setShaperName(const std::string& _shaperName) {
	EWOL_WARNING("set shaper name : '" << _shaperName << "'");
	m_shaper.setSource(_shaperName);
	markToRedraw();
}

void ewol::widget::Button::calculateSize(const vec2& _availlable) {
	vec2 padding = m_shaper.getPadding();
	ewol::Padding ret = calculateSizePadded(_availlable, ewol::Padding(padding.x(), padding.y(), padding.x(), padding.y()));
	//EWOL_DEBUG(" configuring : origin=" << origin << " size=" << subElementSize << "");
	/*
	m_selectableAreaSize = m_size;// - (padding*2.0f);
	m_selectableAreaPos = vec2(0,0);//vec2(ret.xLeft(), ret.yButtom())-padding;
	*/
	m_selectableAreaSize = vec2(ret.xRight(), ret.yButtom());
	m_selectableAreaPos = vec2(ret.xLeft(), ret.yTop());
}


void ewol::widget::Button::calculateMinMaxSize(void) {
	vec2 padding = m_shaper.getPadding();
	calculateMinMaxSizePadded(ewol::Padding(padding.x(), padding.y(), padding.x(), padding.y()));
}

void ewol::widget::Button::onDraw(void) {
	// draw the shaaper (if needed indeed)
	m_shaper.draw();
}

void ewol::widget::Button::onRegenerateDisplay(void) {
	ewol::widget::Container2::onRegenerateDisplay();
	if (true == needRedraw()) {
		vec2 padding = m_shaper.getPadding();
		m_shaper.clear();
		m_shaper.setOrigin(vec2ClipInt32(m_selectableAreaPos));
		m_shaper.setSize(vec2ClipInt32(m_selectableAreaSize));
		m_shaper.setInsidePos(vec2ClipInt32(m_selectableAreaPos+padding));
		m_shaper.setInsideSize(vec2ClipInt32(m_selectableAreaSize-padding*2.0f));
	}
}

void ewol::widget::Button::setLock(enum buttonLock _lock) {
	if (m_lock != _lock) {
		m_lock = _lock;
		if(ewol::widget::Button::lockAccess == _lock) {
			m_buttonPressed = false;
			m_mouseHover = false;
		}
		CheckStatus();
		markToRedraw();
	}
}

void ewol::widget::Button::setValue(bool _val) {
	if (m_value != _val) {
		m_value = _val;
		if (m_toggleMode == true) {
			if (m_value == false) {
				m_idWidgetDisplayed = 0;
			} else {
				m_idWidgetDisplayed = 1;
			}
		}
		CheckStatus();
		markToRedraw();
	}
}

void ewol::widget::Button::setToggleMode(bool _togg) {
	if (m_toggleMode != _togg) {
		m_toggleMode = _togg;
		if (m_value == true) {
			m_value = false;
			// TODO : change display and send event ...
		}
		if (m_toggleMode == false) {
			m_idWidgetDisplayed = 0;
		} else {
			if (m_value == false) {
				m_idWidgetDisplayed = 0;
			} else {
				m_idWidgetDisplayed = 1;
			}
		}
		CheckStatus();
		markToRedraw();
	}
}

bool ewol::widget::Button::onEventInput(const ewol::event::Input& _event) {
	EWOL_VERBOSE("Event on BT : " << _event);
	// disable event in the lock access mode :
	if(ewol::widget::Button::lockAccess == m_lock) {
		return false;
	}
	bool previousHoverState = m_mouseHover;
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
	bool previousPressed = m_buttonPressed;
	EWOL_VERBOSE("Event on BT ... mouse hover : " << m_mouseHover);
	if (true == m_mouseHover) {
		if (1 == _event.getId()) {
			if(ewol::key::statusDown == _event.getStatus()) {
				EWOL_VERBOSE(getName() << " : Generate event : " << eventDown);
				generateEventId(eventDown);
				m_buttonPressed = true;
				markToRedraw();
			}
			if(ewol::key::statusUp == _event.getStatus()) {
				EWOL_VERBOSE(getName() << " : Generate event : " << eventUp);
				generateEventId(eventUp);
				m_buttonPressed = false;
				markToRedraw();
			}
			if(ewol::key::statusSingle == _event.getStatus()) {
				if(    (    m_value == true
				         && ewol::widget::Button::lockWhenPressed == m_lock)
				    || (    m_value == false
				         && ewol::widget::Button::lockWhenReleased == m_lock) ) {
					// nothing to do : Lock mode ...
					// user might set himself the new correct value with @ref setValue(xxx)
				} else {
					// inverse value :
					setValue((m_value)?false:true);
					EWOL_VERBOSE(getName() << " : Generate event : " << eventPressed);
					generateEventId(eventPressed);
					EWOL_VERBOSE(getName() << " : Generate event : " << eventValue << " val=" << m_value );
					generateEventId(eventValue, std::to_string(m_value));
					if(    false == m_toggleMode
					    && true == m_value) {
						setValue(false);
						EWOL_VERBOSE(getName() << " : Generate event : " << ewol::widget::Button::eventValue << " val=" << m_value);
						generateEventId(eventValue, std::to_string(m_value));
					}
				}
				markToRedraw();
			}
		}
	}
	if(    m_mouseHover != previousHoverState
	    || m_buttonPressed != previousPressed) {
		CheckStatus();
	}
	return m_mouseHover;
}


bool ewol::widget::Button::onEventEntry(const ewol::event::Entry& _event) {
	//EWOL_DEBUG("BT PRESSED : \"" << UTF8_data << "\" size=" << strlen(UTF8_data));
	if(    _event.getType() == ewol::key::keyboardChar
	    && _event.getStatus() == ewol::key::statusDown
	    && _event.getChar() == '\r') {
		generateEventId(eventEnter);
		return true;
	}
	return false;
}

void ewol::widget::Button::CheckStatus(void) {
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
	if (true == m_shaper.changeStatusIn(_newStatusId) ) {
		periodicCallEnable();
		markToRedraw();
	}
}


void ewol::widget::Button::periodicCall(const ewol::event::Time& _event) {
	if (false == m_shaper.periodicCall(_event) ) {
		periodicCallDisable();
	}
	markToRedraw();
}


bool ewol::widget::Button::onSetConfig(const ewol::object::Config& _conf) {
	if (true == ewol::widget::Container2::onSetConfig(_conf)) {
		return true;
	}
	if (_conf.getConfig() == configToggle) {
		setToggleMode(std::stob(_conf.getData()));
		return true;
	}
	if (_conf.getConfig() == configLock) {
		enum buttonLock tmpLock = lockNone;
		if(    compare_no_case(_conf.getData(), "true") == true
		    || compare_no_case(_conf.getData(), "1") == true) {
			tmpLock = lockAccess;
		} else if(    compare_no_case(_conf.getData(), "down") == true
		           || compare_no_case(_conf.getData(), "pressed") == true) {
			tmpLock = lockWhenPressed;
		} else if(    compare_no_case(_conf.getData(), "up") == true
		           || compare_no_case(_conf.getData(), "released") == true) {
			tmpLock = lockWhenReleased;
		}
		setLock(tmpLock);
		return true;
	}
	if (_conf.getConfig() == configValue) {
		setValue(std::stob(_conf.getData()));
		return true;
	}
	if (_conf.getConfig() == configShaper) {
		setShaperName(_conf.getData());
		return true;
	}
	return false;
}

bool ewol::widget::Button::onGetConfig(const char* _config, std::string& _result) const {
	if (true == ewol::widget::Container2::onGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configToggle) {
		_result = std::to_string(getToggleMode());
		return true;
	}
	if (_config == configLock) {
		switch(getLock()){
			default:
			case lockNone:
				_result = "none";
				break;
			case lockAccess:
				_result = "true";
				break;
			case lockWhenPressed:
				_result = "pressed";
				break;
			case lockWhenReleased:
				_result = "released";
				break;
		}
		return true;
	}
	if (_config == configValue) {
		_result = std::to_string(getValue());
		return true;
	}
	if (_config == configShaper) {
		_result = m_shaper.getSource();
		return true;
	}
	return false;
}


