/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ewol/ewol.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/WidgetManager.h>

#undef __class__
#define __class__	"Button"


const char* const widget::Button::eventPressed    = "ewol-widget-button-event-Pressed";
const char* const widget::Button::eventDown       = "ewol-widget-button-event-down";
const char* const widget::Button::eventUp         = "ewol-widget-button-event-up";
const char* const widget::Button::eventEnter      = "ewol-widget-button-event-enter";
const char* const widget::Button::eventLeave      = "ewol-widget-button-event-leave";
const char* const widget::Button::eventValue      = "ewol-widget-button-event-value";

const char* const widget::Button::configToggle = "toggle";
const char* const widget::Button::configLock   = "lock";
const char* const widget::Button::configValue  = "value";


// DEFINE for the shader display system :
#define STATUS_UP        (0)
#define STATUS_HOVER     (2)
#define STATUS_PRESSED   (1)
#define STATUS_DOWN      (3)


static ewol::Widget* Create(void)
{
	return new widget::Button();
}

void widget::Button::Init(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,&Create);
}

void widget::Button::UnInit(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,NULL);
}

widget::Button::Button(const etk::UString& _shaperName) :
	m_shaper(_shaperName),
	m_value(false),
	m_lock(widget::Button::lockNone),
	m_toggleMode(false),
	m_mouseHover(false),
	m_buttonPressed(false),
	m_selectableAreaPos(0,0),
	m_selectableAreaSize(0,0)
{
	// by default set no widget :
	m_subWidget[0] = NULL;
	m_subWidget[1] = NULL;
	// add basic Event generated :
	AddEventId(eventPressed);
	AddEventId(eventDown);
	AddEventId(eventUp);
	AddEventId(eventEnter);
	AddEventId(eventLeave);
	AddEventId(eventValue);
	// Add configuration
	RegisterConfig(configToggle, "bool", NULL, "The Button can toogle");
	RegisterConfig(configValue, "bool", NULL, "Basic value of the widget");
	RegisterConfig(configLock, "list", "none;true;released;pressed", "Lock the button in a special state to permit changing state only by the coder");
	
	// shaper satatus update:
	CheckStatus();
	// This widget can have the focus ...
	SetCanHaveFocus(true);
	// Limit event at 1:
	SetMouseLimit(1);
}


widget::Button::~Button(void)
{
	
}


void widget::Button::SetShaperName(const etk::UString& _shaperName)
{
	m_shaper.SetSource(_shaperName);
}

void widget::Button::SetSubWidget(ewol::Widget* _subWidget)
{
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
	ewol::RequestUpdateSize();
	MarkToRedraw();
}

void widget::Button::SetSubWidgetToggle(ewol::Widget* _subWidget)
{
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
	ewol::RequestUpdateSize();
}

void widget::Button::CalculateSize(const vec2& _availlable)
{
	vec2 padding = m_shaper.GetPadding();
	// set minimal size
	m_size = m_minSize;
	
	/*
	if (m_origin.x()<0) {
		EWOL_ERROR("[" << GetId() << "] position error ori=" << m_origin << " size=" << m_size);
		etk::DisplayBacktrace();
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
	// Checkin the filling properties ==> for the subElements:
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
		m_subWidget[0]->SetOrigin(m_origin+origin);
		m_subWidget[0]->CalculateSize(subElementSize);
	}
	if (NULL!=m_subWidget[1]) {
		m_subWidget[1]->SetOrigin(m_origin+origin);
		m_subWidget[1]->CalculateSize(subElementSize);
	}
	//EWOL_DEBUG(" configuring : origin=" << origin << " size=" << subElementSize << "");
	m_selectableAreaSize = subElementSize + (padding*2.0f);
	m_selectableAreaPos = origin-padding;
	MarkToRedraw();
}


void widget::Button::CalculateMinMaxSize(void)
{
	vec2 padding = m_shaper.GetPadding();
	vec2 minimumSizeBase(0,0);
	vec2 minimumSizeToggle(0,0);
	if (NULL!=m_subWidget[0]) {
		m_subWidget[0]->CalculateMinMaxSize();
		minimumSizeBase = m_subWidget[0]->GetCalculateMinSize();
	}
	if (NULL!=m_subWidget[1]) {
		m_subWidget[1]->CalculateMinMaxSize();
		minimumSizeToggle = m_subWidget[1]->GetCalculateMinSize();
	}
	// get the maxixmum min size of the 2 sub-widget (if they are present indeed):
	m_minSize.setX(etk_max(minimumSizeBase.x(), minimumSizeToggle.x()) );
	m_minSize.setY(etk_max(minimumSizeBase.y(), minimumSizeToggle.y()) );
	
	// add padding :
	m_minSize += padding*2.0f;
	// verify the min max of the min size ...
	CheckMinSize();
	//EWOL_ERROR("[" << GetId() << "] {" << GetObjectType() << "} Result min size : " <<  m_minSize);
	MarkToRedraw();
}

void widget::Button::SystemDraw(const ewol::DrawProperty& _displayProp)
{
	if (true==m_hide){
		// widget is hidden ...
		return;
	}
	ewol::Widget::SystemDraw(_displayProp);
	// draw the widget that need something ...
	if(    false == m_toggleMode
	    || false == m_value
	    || NULL==m_subWidget[1]) {
		if (NULL!=m_subWidget[0]) {
			m_subWidget[0]->SystemDraw(_displayProp);
		}
	} else {
		if (NULL!=m_subWidget[1]) {
			m_subWidget[1]->SystemDraw(_displayProp);
		}
	}
}
void widget::Button::OnDraw(void)
{
	// draw the shaaper (if needed indeed)
	m_shaper.Draw();
}

void widget::Button::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		vec2 padding = m_shaper.GetPadding();
		m_shaper.Clear();
		m_shaper.SetOrigin(vec2ClipInt32(m_selectableAreaPos));
		m_shaper.SetSize(vec2ClipInt32(m_selectableAreaSize));
		m_shaper.SetInsidePos(vec2ClipInt32(m_selectableAreaPos+padding));
		m_shaper.SetInsideSize(vec2ClipInt32(m_selectableAreaSize-padding*2.0f));
	}
	if(    false == m_toggleMode
	    || false == m_value
	    || NULL==m_subWidget[1]) {
		if (NULL!=m_subWidget[0]) {
			m_subWidget[0]->OnRegenerateDisplay();
		}
	} else {
		if (NULL!=m_subWidget[1]) {
			m_subWidget[1]->OnRegenerateDisplay();
		}
	}
}

void widget::Button::SetLock(buttonLock_te _lock)
{
	if (m_lock != _lock) {
		m_lock = _lock;
		if(widget::Button::lockAccess == _lock) {
			m_buttonPressed = false;
			m_mouseHover = false;
		}
		CheckStatus();
		MarkToRedraw();
	}
}

void widget::Button::SetValue(bool _val)
{
	if (m_value != _val) {
		m_value = _val;
		CheckStatus();
		MarkToRedraw();
	}
}

void widget::Button::SetToggleMode(bool _togg)
{
	if (m_toggleMode != _togg) {
		m_toggleMode = _togg;
		if (m_value == true) {
			m_value = false;
			// TODO : Change display and send event ...
		}
		CheckStatus();
		MarkToRedraw();
	}
}

bool widget::Button::OnEventInput(const ewol::EventInput& _event)
{
	// disable event in the lock access mode :
	if(widget::Button::lockAccess == m_lock) {
		return false;
	}
	bool previousHoverState = m_mouseHover;
	if(    ewol::keyEvent::statusLeave == _event.GetStatus()
	    || ewol::keyEvent::statusAbort == _event.GetStatus()) {
		m_mouseHover = false;
		m_buttonPressed = false;
	} else {
		vec2 relativePos = RelativePosition(_event.GetPos());
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
		if (1 == _event.GetId()) {
			if(ewol::keyEvent::statusDown == _event.GetStatus()) {
				//EWOL_DEBUG("Generate event : " << eventDown);
				GenerateEventId(eventDown);
				m_buttonPressed = true;
				MarkToRedraw();
			}
			if(ewol::keyEvent::statusUp == _event.GetStatus()) {
				//EWOL_DEBUG("Generate event : " << eventUp);
				GenerateEventId(eventUp);
				m_buttonPressed = false;
				MarkToRedraw();
			}
			if(ewol::keyEvent::statusSingle == _event.GetStatus()) {
				if(    (    m_value == true
				         && widget::Button::lockWhenPressed == m_lock)
				    || (    m_value == false
				         && widget::Button::lockWhenReleased == m_lock) ) {
					// nothing to do : Lock mode ...
					// user might set himself the new correct value with @ref SetValue(xxx)
				} else {
					// inverse value :
					m_value = (m_value)?false:true;
					//EWOL_DEBUG("Generate event : " << eventPressed);
					GenerateEventId(eventPressed);
					//EWOL_DEBUG("Generate event : " << eventValue << " val=" << m_value);
					GenerateEventId(eventValue, m_value);
					if(    false == m_toggleMode
					    && true == m_value) {
						m_value = false;
						//EWOL_DEBUG("Generate event : " << widget::Button::eventValue << " val=" << m_value);
						GenerateEventId(eventValue, m_value);
					}
				}
				MarkToRedraw();
			}
		}
	}
	if(    m_mouseHover != previousHoverState
	    || m_buttonPressed != previousPressed) {
		CheckStatus();
	}
	return m_mouseHover;
}


bool widget::Button::OnEventEntry(const ewol::EventEntry& _event)
{
	//EWOL_DEBUG("BT PRESSED : \"" << UTF8_data << "\" size=" << strlen(UTF8_data));
	if(    _event.GetType() == ewol::keyEvent::keyboardChar
	    && _event.GetStatus() == ewol::keyEvent::statusDown
	    && _event.GetChar() == '\r') {
		GenerateEventId(eventEnter);
		return true;
	}
	return false;
}

void widget::Button::CheckStatus(void)
{
	if (true==m_buttonPressed) {
		ChangeStatusIn(STATUS_PRESSED);
	} else {
		if (true==m_mouseHover) {
			ChangeStatusIn(STATUS_HOVER);
		} else {
			if (true == m_value) {
				ChangeStatusIn(STATUS_DOWN);
			} else {
				ChangeStatusIn(STATUS_UP);
			}
		}
	}
}

void widget::Button::ChangeStatusIn(int32_t _newStatusId)
{
	if (true == m_shaper.ChangeStatusIn(_newStatusId) ) {
		PeriodicCallSet(true);
		MarkToRedraw();
	}
}


void widget::Button::PeriodicCall(int64_t _localTime)
{
	if (false == m_shaper.PeriodicCall(_localTime) ) {
		PeriodicCallSet(false);
	}
	MarkToRedraw();
}


ewol::Widget* widget::Button::GetWidgetNamed(const etk::UString& _widgetName)
{
	ewol::Widget* tmpUpperWidget = ewol::Widget::GetWidgetNamed(_widgetName);
	if (NULL!=tmpUpperWidget) {
		return tmpUpperWidget;
	}
	if (m_subWidget[0]!= NULL) {
		ewol::Widget* tmpWidget = m_subWidget[0]->GetWidgetNamed(_widgetName);
		if (NULL != tmpWidget) {
			return tmpWidget;
		}
	}
	if (m_subWidget[1]!= NULL) {
		ewol::Widget* tmpWidget = m_subWidget[1]->GetWidgetNamed(_widgetName);
		if (NULL != tmpWidget) {
			return tmpWidget;
		}
	}
	return NULL;
}


bool widget::Button::LoadXML(TiXmlNode* _node)
{
	if (NULL==_node) {
		return false;
	}
	// parse generic properties :
	ewol::Widget::LoadXML(_node);
	// remove previous element :
	SetSubWidget(NULL);
	SetSubWidgetToggle(NULL);
	
	// parse all the elements :
	for(TiXmlNode * pNode = _node->FirstChild() ;
	    NULL != pNode ;
	    pNode = pNode->NextSibling() ) {
		if (pNode->Type()==TiXmlNode::TINYXML_COMMENT) {
			// nothing to do, just proceed to next step
			continue;
		}
		etk::UString widgetName = pNode->Value();
		if (ewol::widgetManager::Exist(widgetName) == false) {
			EWOL_ERROR("(l "<<pNode->Row()<<") Unknown basic node=\"" << widgetName << "\" not in : [" << ewol::widgetManager::List() << "]" );
			continue;
		}
		bool toogleMode=false;
		if (NULL != GetSubWidget()) {
			toogleMode=true;
			if (NULL != GetSubWidgetToggle()) {
				EWOL_ERROR("(l "<<pNode->Row()<<") " << __class__ << " Can only have one subWidget ??? node=\"" << widgetName << "\"" );
				continue;
			}
		}
		EWOL_DEBUG("try to create subwidget : '" << widgetName << "'");
		ewol::Widget* tmpWidget = ewol::widgetManager::Create(widgetName);
		if (tmpWidget == NULL) {
			EWOL_ERROR ("(l "<<pNode->Row()<<") Can not create the widget : \"" << widgetName << "\"");
			continue;
		}
		// add widget :
		if (toogleMode==false) {
			SetSubWidget(tmpWidget);
		} else {
			SetToggleMode(true);
			SetSubWidgetToggle(tmpWidget);
		}
		if (false == tmpWidget->LoadXML(pNode)) {
			EWOL_ERROR ("(l "<<pNode->Row()<<") can not load widget properties : \"" << widgetName << "\"");
			return false;
		}
	}
	return true;
}

bool widget::Button::OnSetConfig(const ewol::EConfig& _conf)
{
	if (true == ewol::Widget::OnSetConfig(_conf)) {
		return true;
	}
	if (_conf.GetConfig() == configToggle) {
		SetToggleMode(_conf.GetData().ToBool());
		return true;
	}
	if (_conf.GetConfig() == configLock) {
		buttonLock_te tmpLock = lockNone;
		if(    true == _conf.GetData().CompareNoCase("true")
		    || true == _conf.GetData().CompareNoCase("1")) {
			tmpLock = lockAccess;
		} else if(    true == _conf.GetData().CompareNoCase("down")
		           || true == _conf.GetData().CompareNoCase("pressed")) {
			tmpLock = lockWhenPressed;
		} else if(    true == _conf.GetData().CompareNoCase("up")
		           || true == _conf.GetData().CompareNoCase("released")) {
			tmpLock = lockWhenReleased;
		}
		SetLock(tmpLock);
		return true;
	}
	if (_conf.GetConfig() == configValue) {
		SetValue(_conf.GetData().ToBool());
		return true;
	}
	return false;
}

bool widget::Button::OnGetConfig(const char* _config, etk::UString& _result) const
{
	if (true == ewol::Widget::OnGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configToggle) {
		if (true==GetToggleMode()) {
			_result = "true";
		} else {
			_result = "false";
		}
		return true;
	}
	if (_config == configLock) {
		switch(GetLock()){
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
		if (true==GetValue()) {
			_result = "true";
		} else {
			_result = "false";
		}
		return true;
	}
	return false;
}


