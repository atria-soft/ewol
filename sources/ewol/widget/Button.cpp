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


const char* const widget::Button::eventPressed    = "ewol-widget-button-event-Pressed";
const char* const widget::Button::eventDown       = "ewol-widget-button-event-down";
const char* const widget::Button::eventUp         = "ewol-widget-button-event-up";
const char* const widget::Button::eventEnter      = "ewol-widget-button-event-enter";
const char* const widget::Button::eventLeave      = "ewol-widget-button-event-leave";
const char* const widget::Button::eventValue      = "ewol-widget-button-event-value";

const char* const widget::Button::configToggle = "toggle";
const char* const widget::Button::configLock   = "lock";
const char* const widget::Button::configValue  = "value";
const char* const widget::Button::configShaper = "shaper";


// DEFINE for the shader display system :
#define STATUS_UP        (0)
#define STATUS_HOVER     (2)
#define STATUS_PRESSED   (1)
#define STATUS_DOWN      (3)


static ewol::Widget* Create(void) {
	return new widget::Button();
}

void widget::Button::init(ewol::WidgetManager& _widgetManager) {
	_widgetManager.addWidgetCreator(__class__,&Create);
}

widget::Button::Button(const etk::UString& _shaperName) :
  m_shaper(_shaperName),
  m_value(false),
  m_lock(widget::Button::lockNone),
  m_toggleMode(false),
  m_mouseHover(false),
  m_buttonPressed(false),
  m_selectableAreaPos(0,0),
  m_selectableAreaSize(0,0) {
	// by default set no widget :
	m_subWidget[0] = NULL;
	m_subWidget[1] = NULL;
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


widget::Button::~Button(void) {
	
}


void widget::Button::setShaperName(const etk::UString& _shaperName) {
	m_shaper.setSource(_shaperName);
	markToRedraw();
}

void widget::Button::setSubWidget(ewol::Widget* _subWidget) {
	int32_t idWidget=0;
	if (NULL!=m_subWidget[idWidget]) {
		delete(m_subWidget[idWidget]);
		// the pointer might already set at NULL:
		if (NULL != m_subWidget[idWidget]) {
			EWOL_ERROR("error while removing previous widget...");
			m_subWidget[idWidget]=NULL;
		}
	}
	EWOL_VERBOSE("Add button : " << idWidget << " element : " << (int64_t)_subWidget);
	m_subWidget[idWidget] = _subWidget;
	// element change ... We need to recalculate all the subElments :
	requestUpdateSize();
	markToRedraw();
}

void widget::Button::setSubWidgetToggle(ewol::Widget* _subWidget) {
	int32_t idWidget=1;
	if (NULL!=m_subWidget[idWidget]) {
		delete(m_subWidget[idWidget]);
		// the pointer might already set at NULL:
		if (NULL != m_subWidget[idWidget]) {
			EWOL_ERROR("error while removing previous widget...");
			m_subWidget[idWidget]=NULL;
		}
	}
	EWOL_VERBOSE("Add button : " << idWidget << " element : " << (int64_t)_subWidget);
	m_subWidget[idWidget] = _subWidget;
	// element change ... We need to recalculate all the subElments :
	requestUpdateSize();
}

void widget::Button::calculateSize(const vec2& _availlable) {
	vec2 padding = m_shaper.getPadding();
	// set minimal size
	m_size = m_minSize;
	
	/*
	if (m_origin.x()<0) {
		EWOL_ERROR("[" << getId() << "] position error ori=" << m_origin << " size=" << m_size);
		etk::displayBacktrace();
	}
	*/
	
	vec2 minimumSizeBase(0,0);
	vec2 minimumSizeToggle(0,0);
	// Checking the expand properties :
	if (m_userExpand.x() == true) {
		m_size.setX(_availlable.x());
	}
	if (m_userExpand.y() == true) {
		m_size.setY(_availlable.y());
	}
	// Checkin the filling properties  == > for the subElements:
	vec2 subElementSize = m_minSize;
	if (m_userFill.x() == true) {
		subElementSize.setX(m_size.x());
	}
	if (m_userFill.y() == true) {
		subElementSize.setY(m_size.y());
	}
	vec2 origin = (m_size - subElementSize)/2.0f + padding;
	subElementSize -= padding*2.0f;
	if (NULL!=m_subWidget[0]) {
		m_subWidget[0]->setOrigin(m_origin+origin);
		m_subWidget[0]->calculateSize(subElementSize);
	}
	if (NULL!=m_subWidget[1]) {
		m_subWidget[1]->setOrigin(m_origin+origin);
		m_subWidget[1]->calculateSize(subElementSize);
	}
	//EWOL_DEBUG(" configuring : origin=" << origin << " size=" << subElementSize << "");
	m_selectableAreaSize = subElementSize + (padding*2.0f);
	m_selectableAreaPos = origin-padding;
	markToRedraw();
}


void widget::Button::calculateMinMaxSize(void) {
	vec2 padding = m_shaper.getPadding();
	vec2 minimumSizeBase(0,0);
	vec2 minimumSizeToggle(0,0);
	if (NULL!=m_subWidget[0]) {
		m_subWidget[0]->calculateMinMaxSize();
		minimumSizeBase = m_subWidget[0]->getCalculateMinSize();
	}
	if (NULL!=m_subWidget[1]) {
		m_subWidget[1]->calculateMinMaxSize();
		minimumSizeToggle = m_subWidget[1]->getCalculateMinSize();
	}
	// get the maxixmum min size of the 2 sub-widget (if they are present indeed):
	m_minSize.setX(etk_max(minimumSizeBase.x(), minimumSizeToggle.x()) );
	m_minSize.setY(etk_max(minimumSizeBase.y(), minimumSizeToggle.y()) );
	
	// add padding :
	m_minSize += padding*2.0f;
	// verify the min max of the min size ...
	checkMinSize();
	//EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} Result min size : " <<  m_minSize);
	markToRedraw();
}

void widget::Button::systemDraw(const ewol::DrawProperty& _displayProp) {
	if (true == m_hide){
		// widget is hidden ...
		return;
	}
	ewol::Widget::systemDraw(_displayProp);
	// draw the widget that need something ...
	if(    false == m_toggleMode
	    || false == m_value
	    || NULL == m_subWidget[1]) {
		if (NULL!=m_subWidget[0]) {
			m_subWidget[0]->systemDraw(_displayProp);
		}
	} else {
		if (NULL!=m_subWidget[1]) {
			m_subWidget[1]->systemDraw(_displayProp);
		}
	}
}
void widget::Button::onDraw(void) {
	// draw the shaaper (if needed indeed)
	m_shaper.draw();
}

void widget::Button::onRegenerateDisplay(void) {
	if (true == needRedraw()) {
		vec2 padding = m_shaper.getPadding();
		m_shaper.clear();
		m_shaper.setOrigin(vec2ClipInt32(m_selectableAreaPos));
		m_shaper.setSize(vec2ClipInt32(m_selectableAreaSize));
		m_shaper.setInsidePos(vec2ClipInt32(m_selectableAreaPos+padding));
		m_shaper.setInsideSize(vec2ClipInt32(m_selectableAreaSize-padding*2.0f));
	}
	if(    false == m_toggleMode
	    || false == m_value
	    || NULL == m_subWidget[1]) {
		if (NULL!=m_subWidget[0]) {
			m_subWidget[0]->onRegenerateDisplay();
		}
	} else {
		if (NULL!=m_subWidget[1]) {
			m_subWidget[1]->onRegenerateDisplay();
		}
	}
}

void widget::Button::setLock(buttonLock_te _lock) {
	if (m_lock != _lock) {
		m_lock = _lock;
		if(widget::Button::lockAccess == _lock) {
			m_buttonPressed = false;
			m_mouseHover = false;
		}
		CheckStatus();
		markToRedraw();
	}
}

void widget::Button::setValue(bool _val) {
	if (m_value != _val) {
		m_value = _val;
		CheckStatus();
		markToRedraw();
	}
}

void widget::Button::setToggleMode(bool _togg) {
	if (m_toggleMode != _togg) {
		m_toggleMode = _togg;
		if (m_value == true) {
			m_value = false;
			// TODO : change display and send event ...
		}
		CheckStatus();
		markToRedraw();
	}
}

bool widget::Button::onEventInput(const ewol::EventInput& _event) {
	// disable event in the lock access mode :
	if(widget::Button::lockAccess == m_lock) {
		return false;
	}
	bool previousHoverState = m_mouseHover;
	if(    ewol::keyEvent::statusLeave == _event.getStatus()
	    || ewol::keyEvent::statusAbort == _event.getStatus()) {
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
	//EWOL_DEBUG("Event on BT ... mouse position : " << m_mouseHover);
	if (true == m_mouseHover) {
		if (1 == _event.getId()) {
			if(ewol::keyEvent::statusDown == _event.getStatus()) {
				//EWOL_DEBUG("Generate event : " << eventDown);
				generateEventId(eventDown);
				m_buttonPressed = true;
				markToRedraw();
			}
			if(ewol::keyEvent::statusUp == _event.getStatus()) {
				//EWOL_DEBUG("Generate event : " << eventUp);
				generateEventId(eventUp);
				m_buttonPressed = false;
				markToRedraw();
			}
			if(ewol::keyEvent::statusSingle == _event.getStatus()) {
				if(    (    m_value == true
				         && widget::Button::lockWhenPressed == m_lock)
				    || (    m_value == false
				         && widget::Button::lockWhenReleased == m_lock) ) {
					// nothing to do : Lock mode ...
					// user might set himself the new correct value with @ref setValue(xxx)
				} else {
					// inverse value :
					m_value = (m_value)?false:true;
					//EWOL_DEBUG("Generate event : " << eventPressed);
					generateEventId(eventPressed);
					//EWOL_DEBUG("Generate event : " << eventValue << " val=" << m_value);
					generateEventId(eventValue, m_value);
					if(    false == m_toggleMode
					    && true == m_value) {
						m_value = false;
						//EWOL_DEBUG("Generate event : " << widget::Button::eventValue << " val=" << m_value);
						generateEventId(eventValue, m_value);
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


bool widget::Button::onEventEntry(const ewol::EventEntry& _event) {
	//EWOL_DEBUG("BT PRESSED : \"" << UTF8_data << "\" size=" << strlen(UTF8_data));
	if(    _event.getType() == ewol::keyEvent::keyboardChar
	    && _event.getStatus() == ewol::keyEvent::statusDown
	    && _event.getChar() == '\r') {
		generateEventId(eventEnter);
		return true;
	}
	return false;
}

void widget::Button::CheckStatus(void) {
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

void widget::Button::changeStatusIn(int32_t _newStatusId) {
	if (true == m_shaper.changeStatusIn(_newStatusId) ) {
		periodicCallEnable();
		markToRedraw();
	}
}


void widget::Button::periodicCall(const ewol::EventTime& _event) {
	if (false == m_shaper.periodicCall(_event) ) {
		periodicCallDisable();
	}
	markToRedraw();
}


ewol::Widget* widget::Button::getWidgetNamed(const etk::UString& _widgetName) {
	ewol::Widget* tmpUpperWidget = ewol::Widget::getWidgetNamed(_widgetName);
	if (NULL!=tmpUpperWidget) {
		return tmpUpperWidget;
	}
	if (m_subWidget[0]!= NULL) {
		ewol::Widget* tmpWidget = m_subWidget[0]->getWidgetNamed(_widgetName);
		if (NULL != tmpWidget) {
			return tmpWidget;
		}
	}
	if (m_subWidget[1]!= NULL) {
		ewol::Widget* tmpWidget = m_subWidget[1]->getWidgetNamed(_widgetName);
		if (NULL != tmpWidget) {
			return tmpWidget;
		}
	}
	return NULL;
}


bool widget::Button::loadXML(exml::Element* _element) {
	if (NULL == _element) {
		return false;
	}
	// parse generic properties :
	ewol::Widget::loadXML(_element);
	// remove previous element :
	setSubWidget(NULL);
	setSubWidgetToggle(NULL);
	
	// parse all the elements :
	for(int32_t iii=0; iii< _element->size(); iii++) {
		exml::Element* pNode = _element->getElement(iii);
		if (pNode == NULL) {
			// trash here all that is not element
			continue;
		}
		etk::UString widgetName = pNode->getValue();
		if (getWidgetManager().exist(widgetName) == false) {
			EWOL_ERROR("(l "<<pNode->getPos()<<") Unknown basic node=\"" << widgetName << "\" not in : [" << getWidgetManager().list() << "]" );
			continue;
		}
		bool toogleMode=false;
		if (NULL != getSubWidget()) {
			toogleMode=true;
			if (NULL != getSubWidgetToggle()) {
				EWOL_ERROR("(l "<<pNode->getPos()<<") " << __class__ << " Can only have one subWidget ??? node=\"" << widgetName << "\"" );
				continue;
			}
		}
		EWOL_DEBUG("try to create subwidget : '" << widgetName << "'");
		ewol::Widget* tmpWidget = getWidgetManager().create(widgetName);
		if (tmpWidget == NULL) {
			EWOL_ERROR ("(l "<<pNode->getPos()<<") Can not create the widget : \"" << widgetName << "\"");
			continue;
		}
		// add widget :
		if (toogleMode == false) {
			setSubWidget(tmpWidget);
		} else {
			setToggleMode(true);
			setSubWidgetToggle(tmpWidget);
		}
		if (false == tmpWidget->loadXML(pNode)) {
			EWOL_ERROR ("(l "<<pNode->getPos()<<") can not load widget properties : \"" << widgetName << "\"");
			return false;
		}
	}
	return true;
}

bool widget::Button::onSetConfig(const ewol::EConfig& _conf) {
	if (true == ewol::Widget::onSetConfig(_conf)) {
		return true;
	}
	if (_conf.getConfig() == configToggle) {
		setToggleMode(_conf.getData().toBool());
		return true;
	}
	if (_conf.getConfig() == configLock) {
		buttonLock_te tmpLock = lockNone;
		if(    true == _conf.getData().compareNoCase("true")
		    || true == _conf.getData().compareNoCase("1")) {
			tmpLock = lockAccess;
		} else if(    true == _conf.getData().compareNoCase("down")
		           || true == _conf.getData().compareNoCase("pressed")) {
			tmpLock = lockWhenPressed;
		} else if(    true == _conf.getData().compareNoCase("up")
		           || true == _conf.getData().compareNoCase("released")) {
			tmpLock = lockWhenReleased;
		}
		setLock(tmpLock);
		return true;
	}
	if (_conf.getConfig() == configValue) {
		setValue(_conf.getData().toBool());
		return true;
	}
	if (_conf.getConfig() == configShaper) {
		setShaperName(_conf.getData());
		return true;
	}
	return false;
}

bool widget::Button::onGetConfig(const char* _config, etk::UString& _result) const {
	if (true == ewol::Widget::onGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configToggle) {
		if (true == getToggleMode()) {
			_result = "true";
		} else {
			_result = "false";
		}
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
		if (true == getValue()) {
			_result = "true";
		} else {
			_result = "false";
		}
		return true;
	}
	if (_config == configShaper) {
		_result = m_shaper.getSource();
		return true;
	}
	return false;
}


