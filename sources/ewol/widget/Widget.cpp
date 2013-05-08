/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Widget.h>
#include <ewol/eObject/EObjectManager.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/ewol.h>
#include <ewol/renderer/openGL.h>
#include <ewol/renderer/os/eSystem.h>
#include <ewol/renderer/os/gui.h>


#undef __class__
#define __class__	"Widget"

ewol::Widget::Widget(void) :
	m_up(NULL),
	m_size(10,10),
	m_minSize(0,0),
	m_maxSize(vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE)),
	m_zoom(1.0f),
	m_origin(0,0),
	m_userMinSize(vec2(0,0),ewol::Dimension::Pixel),
	m_userMaxSize(vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE),ewol::Dimension::Pixel),
	m_userExpand(false,false),
	m_userFill(false,false),
	m_hide(false),
	m_hasFocus(false),
	m_canFocus(false),
	m_limitMouseEvent(3),
	m_allowRepeateKeyboardEvent(true),
	m_needRegenerateDisplay(true),
	m_grabCursor(false),
	m_cursorDisplay(ewol::cursorArrow)
{
	
}


ewol::Widget::~Widget(void)
{
	// Remove his own focus...
	ewol::widgetManager::Rm(this);
	// clean all the short-cut ...
	ShortCutClean();
}

void ewol::Widget::SetUpperWidget(ewol::Widget* _upper)
{
	if (NULL == _upper) {
		//just remove father :
		m_up = NULL;
		return;
	}
	if (NULL != m_up) {
		EWOL_WARNING("[" << GetId() << "] Replace upper widget of this one ...");
	}
	m_up = _upper;
}

void ewol::Widget::OnObjectRemove(ewol::EObject* _removeObject)
{
	if (_removeObject == m_up) {
		EWOL_WARNING("[" << GetId() << "] Remove upper widget befor removing this widget ...");
		m_up = NULL;
	}
}

void ewol::Widget::Hide(void)
{
	m_hide = true;
	MarkToRedraw();
	ewol::RequestUpdateSize();
}


void ewol::Widget::Show(void)
{
	m_hide = false;
	MarkToRedraw();
	ewol::RequestUpdateSize();
}


void ewol::Widget::CalculateSize(const vec2& _availlable)
{
	m_size = _availlable;
	MarkToRedraw();
}


bool ewol::Widget::SetFocus(void)
{
	if (true == m_canFocus) {
		m_hasFocus = true;
		OnGetFocus();
		return true;
	}
	return false;
}


bool ewol::Widget::RmFocus(void)
{
	if (true == m_canFocus) {
		m_hasFocus = false;
		OnLostFocus();
		return true;
	}
	return false;
}


void ewol::Widget::SetCanHaveFocus(bool _canFocusState)
{
	m_canFocus = _canFocusState;
	if (true == m_hasFocus) {
		(void)RmFocus();
	}
}


void ewol::Widget::KeepFocus(void)
{
	ewol::widgetManager::FocusKeep(this);
}


void ewol::Widget::GenDraw(DrawProperty _displayProp)
{
	if (true==m_hide){
		// widget is hidden ...
		return;
	}
	// check if the element is displayable in the windows : 
	if(    _displayProp.m_windowsSize.x() < m_origin.x()
	    || _displayProp.m_windowsSize.y() < m_origin.y() ) {
		// out of the windows ==> nothing to display ...
		return;
	}
	ewol::openGL::Push();
	if(    (_displayProp.m_origin.x() > m_origin.x())
	    || (_displayProp.m_origin.x() + _displayProp.m_size.x() < m_size.x() + m_origin.x()) ) {
		// here we invert the reference of the standard OpenGl view because the reference in the common display is Top left and not buttom left
		int32_t tmpOriginX = etk_max(_displayProp.m_origin.x(), m_origin.x());
		int32_t tmppp1 = _displayProp.m_origin.x() + _displayProp.m_size.x();
		int32_t tmppp2 = m_origin.x() + m_size.x();
		int32_t tmpclipX = etk_min(tmppp1, tmppp2) - tmpOriginX;
		
		int32_t tmpOriginY = etk_max(_displayProp.m_origin.y(), m_origin.y());
		tmppp1 = _displayProp.m_origin.y() + _displayProp.m_size.y();
		tmppp2 = m_origin.y() + m_size.y();
		//int32_t tmpclipY = etk_min(tmppp1, tmppp2) - tmpOriginX;
		
		glViewport( tmpOriginX,
		            tmpOriginY,
		            tmpclipX,
		            m_size.y());
		mat4 tmpTranslate = etk::matTranslate(vec3((float)(-tmpclipX/2 - (tmpOriginX-m_origin.x())), (float)(-m_size.y()/2.0), -1.0f));
		mat4 tmpScale = etk::matScale(vec3(m_zoom, m_zoom, 1));
		mat4 tmpProjection = etk::matOrtho(-tmpclipX/2, tmpclipX/2, -m_size.y()/2, m_size.y()/2, -1, 1);
		mat4 tmpMat = tmpProjection * tmpScale * tmpTranslate;
		// set internal matrix system :
		ewol::openGL::SetMatrix(tmpMat);
		// Call the widget drawing methode
		_displayProp.m_origin.setValue(tmpOriginX, tmpOriginY);
		_displayProp.m_size.setValue(tmpclipX, m_size.y());
		//int64_t ___startTime = ewol::GetTime();
		OnDraw(_displayProp);
		//float ___localTime = (float)(ewol::GetTime() - ___startTime) / 1000.0f;
		//EWOL_DEBUG("      Widget1  : " << ___localTime << "ms ");
	} else {
		glViewport( m_origin.x(),
		            m_origin.y(),
		            m_size.x(),
		            m_size.y());
		mat4 tmpTranslate = etk::matTranslate(vec3(-m_size.x()/2, -m_size.y()/2, -1.0f));
		mat4 tmpScale = etk::matScale(vec3(m_zoom, m_zoom, 1.0f));
		mat4 tmpProjection = etk::matOrtho(-m_size.x()/2, m_size.x()/2, -m_size.y()/2, m_size.y()/2, -1, 1);
		mat4 tmpMat = tmpProjection * tmpScale * tmpTranslate;
		// set internal matrix system :
		ewol::openGL::SetMatrix(tmpMat);
		// Call the widget drawing methode
		_displayProp.m_origin = m_origin;
		_displayProp.m_size = m_size;
		//int64_t ___startTime = ewol::GetTime();
		OnDraw(_displayProp);
		//float ___localTime = (float)(ewol::GetTime() - ___startTime) / 1000.0f;
		//EWOL_DEBUG("      Widget2  : " << ___localTime << "ms ");
	}
	ewol::openGL::Pop();
	return;
}


void ewol::Widget::PeriodicCallSet(bool _statusToSet)
{
	if (true == _statusToSet) {
		ewol::widgetManager::PeriodicCallAdd(this);
	} else {
		ewol::widgetManager::PeriodicCallRm(this);
	}
}


void ewol::Widget::MarkToRedraw(void)
{
	m_needRegenerateDisplay = true;
	ewol::widgetManager::MarkDrawingIsNeeded();
}


void ewol::Widget::SetZoom(float _newVal)
{
	m_zoom = etk_avg(0.0000001,_newVal,1000000.0);
	MarkToRedraw();
}

float ewol::Widget::GetZoom(void)
{
	return m_zoom;
}

void ewol::Widget::SetOrigin(const vec2& _pos)
{
	m_origin = _pos;
}

vec2 ewol::Widget::GetOrigin(void)
{
	return m_origin;
}

vec2 ewol::Widget::RelativePosition(const vec2& _pos)
{
	return _pos - m_origin;
}

void ewol::Widget::CalculateMinMaxSize(void)
{
	m_minSize = m_userMinSize.GetPixel();
	m_maxSize = m_userMaxSize.GetPixel();
	MarkToRedraw();
}

vec2 ewol::Widget::GetCalculateMinSize(void)
{
	if (false==IsHide()) {
		return m_minSize;
	}
	return vec2(0,0);
}

vec2 ewol::Widget::GetCalculateMaxSize(void)
{
	if (false==IsHide()) {
		return m_maxSize;
	}
	return vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE);
}

void ewol::Widget::SetMinSize(const ewol::Dimension& _size)
{
	vec2 pixelMin = _size.GetPixel();
	vec2 pixelMax = m_userMaxSize.GetPixel();
	// check minimum & maximum compatibility :
	bool error=false;
	if (pixelMin.x()>pixelMax.x()) {
		error=true;
	}
	if (pixelMin.y()>pixelMax.y()) {
		error=true;
	}
	if (error==true) {
		EWOL_ERROR("Can not set a 'min Size' > 'max size' set nothing ...");
		return;
	}
	m_userMinSize = _size;
	ewol::RequestUpdateSize();
}

void ewol::Widget::SetNoMinSize(void)
{
	m_userMinSize.Set(vec2(0,0),ewol::Dimension::Pixel);
}

void ewol::Widget::CheckMinSize(void)
{
	vec2 pixelSize = m_userMinSize.GetPixel();
	m_minSize.setX(etk_max(m_minSize.x(), pixelSize.x()));
	m_minSize.setY(etk_max(m_minSize.y(), pixelSize.y()));
}

void ewol::Widget::SetMaxSize(const ewol::Dimension& _size)
{
	vec2 pixelMin = m_userMinSize.GetPixel();
	vec2 pixelMax = _size.GetPixel();
	// check minimum & maximum compatibility :
	bool error=false;
	if (pixelMin.x()>pixelMax.x()) {
		error=true;
	}
	if (pixelMin.y()>pixelMax.y()) {
		error=true;
	}
	if (error==true) {
		EWOL_ERROR("Can not set a 'min Size' > 'max size' set nothing ...");
		return;
	}
	m_userMaxSize = _size;
	ewol::RequestUpdateSize();
}

void ewol::Widget::SetNoMaxSize(void)
{
	m_userMaxSize.Set(vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE),ewol::Dimension::Pixel);
}

void ewol::Widget::CheckMaxSize(void)
{
	vec2 pixelSize = m_userMaxSize.GetPixel();
	m_maxSize.setX(etk_min(m_maxSize.x(), pixelSize.x()));
	m_maxSize.setY(etk_min(m_maxSize.y(), pixelSize.y()));
}

vec2 ewol::Widget::GetSize(void)
{
	if (false==IsHide()) {
		return m_size;
	}
	return vec2(0,0);
}

void ewol::Widget::SetExpand(const bvec2& _newExpand)
{
	if(    m_userExpand.x() != _newExpand.x()
	    || m_userExpand.y() != _newExpand.y()) {
		m_userExpand = _newExpand;
		ewol::RequestUpdateSize();
		MarkToRedraw();
	}
}

bvec2 ewol::Widget::CanExpand(void)
{
	if (false==IsHide()) {
		return m_userExpand;
	}
	return bvec2(false,false);
}


void ewol::Widget::SetFill(const bvec2& _newFill)
{
	if(    m_userFill.x() != _newFill.x()
	    || m_userFill.y() != _newFill.y()) {
		m_userFill = _newFill;
		ewol::RequestUpdateSize();
		MarkToRedraw();
	}
}

const bvec2& ewol::Widget::CanFill(void)
{
	return m_userFill;
}

// ----------------------------------------------------------------------------------------------------------------
// -- Shortcut : management of the shortcut
// ----------------------------------------------------------------------------------------------------------------

void ewol::Widget::ShortCutAdd(const char * _descriptiveString, const char * _generateEventId, etk::UString _data, bool _broadcast)
{
	if(		NULL==_descriptiveString
		||	0==strlen(_descriptiveString))
	{
		EWOL_ERROR("try to add shortcut with no descriptive string ...");
		return;
	}
	
	EventShortCut* tmpElement = new EventShortCut();
	if (NULL == tmpElement) {
		EWOL_ERROR("allocation error ... Memory error ...");
		return;
	}
	tmpElement->broadcastEvent = _broadcast;
	tmpElement->generateEventId = _generateEventId;
	tmpElement->eventData = _data;
	// parsing of the string :
	//"ctrl+shift+alt+meta+s"
	const char * tmp = strstr(_descriptiveString, "ctrl");
	if(NULL != tmp) {
		tmpElement->specialKey.ctrl = true;
	}
	tmp = strstr(_descriptiveString, "shift");
	if(NULL != tmp) {
		tmpElement->specialKey.shift = true;
	}
	tmp = strstr(_descriptiveString, "alt");
	if(NULL != tmp) {
		tmpElement->specialKey.alt = true;
	}
	tmp = strstr(_descriptiveString, "meta");
	if(NULL != tmp) {
		tmpElement->specialKey.meta = true;
	}
	if(NULL != strstr(_descriptiveString, "F12") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF12;
	} else if(NULL != strstr(_descriptiveString, "F11") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF11;
	} else if(NULL != strstr(_descriptiveString, "F10") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF10;
	} else if(NULL != strstr(_descriptiveString, "F9") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF9;
	} else if(NULL != strstr(_descriptiveString, "F8") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF8;
	} else if(NULL != strstr(_descriptiveString, "F7") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF7;
	} else if(NULL != strstr(_descriptiveString, "F6") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF6;
	} else if(NULL != strstr(_descriptiveString, "F5") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF5;
	} else if(NULL != strstr(_descriptiveString, "F4") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF4;
	} else if(NULL != strstr(_descriptiveString, "F3") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF3;
	} else if(NULL != strstr(_descriptiveString, "F2") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF2;
	} else if(NULL != strstr(_descriptiveString, "F1") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF1;
	} else if(NULL != strstr(_descriptiveString, "LEFT") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardLeft;
	} else if(NULL != strstr(_descriptiveString, "RIGHT") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardRight;
	} else if(NULL != strstr(_descriptiveString, "UP") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardUp;
	} else if(NULL != strstr(_descriptiveString, "DOWN") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardDown;
	} else if(NULL != strstr(_descriptiveString, "PAGE_UP") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardPageUp;
	} else if(NULL != strstr(_descriptiveString, "PAGE_DOWN") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardPageDown;
	} else if(NULL != strstr(_descriptiveString, "START") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardStart;
	} else if(NULL != strstr(_descriptiveString, "END") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardEnd;
	} else if(NULL != strstr(_descriptiveString, "PRINT") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardPrint;
	} else if(NULL != strstr(_descriptiveString, "ARRET_DEFIL") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardStopDefil;
	} else if(NULL != strstr(_descriptiveString, "WAIT") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardWait;
	} else if(NULL != strstr(_descriptiveString, "INSERT") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardInsert;
	} else if(NULL != strstr(_descriptiveString, "CAPLOCK") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardCapLock;
	} else if(NULL != strstr(_descriptiveString, "CONTEXT_MENU") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardContextMenu;
	} else if(NULL != strstr(_descriptiveString, "NUM_LOCK") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardNumLock;
	} else {
		tmpElement->unicodeValue = _descriptiveString[strlen(_descriptiveString) -1];
	}
	// add it on the List ...
	m_localShortcut.PushBack(tmpElement);
}


void ewol::Widget::ShortCutClean(void)
{
	for (int32_t iii=0; iii<m_localShortcut.Size(); iii++) {
		if (NULL != m_localShortcut[iii]) {
			delete(m_localShortcut[iii]);
			m_localShortcut[iii]=NULL;
		}
	}
	m_localShortcut.Clear();
}


bool ewol::Widget::OnEventShortCut(ewol::SpecialKey& _special, uniChar_t _unicodeValue, ewol::keyEvent::keyboard_te _kbMove, bool _isDown)
{
	if (_unicodeValue >= 'A' && _unicodeValue <='Z') {
		_unicodeValue += 'a' - 'A';
	}
	//EWOL_INFO("Try to find generic shortcut ...");
	for(int32_t iii=m_localShortcut.Size()-1; iii>=0; iii--) {
		if(NULL != m_localShortcut[iii]) {
			if(    m_localShortcut[iii]->specialKey.shift == _special.shift
			    && m_localShortcut[iii]->specialKey.ctrl  == _special.ctrl
			    && m_localShortcut[iii]->specialKey.alt   == _special.alt
			    && m_localShortcut[iii]->specialKey.meta  == _special.meta
			    && (    (    m_localShortcut[iii]->keyboardMoveValue == ewol::keyEvent::keyboardUnknow
			              && m_localShortcut[iii]->unicodeValue == _unicodeValue)
			         || (    m_localShortcut[iii]->keyboardMoveValue == _kbMove
			              && m_localShortcut[iii]->unicodeValue == 0)
			       ) )
			{
				if (_isDown) {
					if (true == m_localShortcut[iii]->broadcastEvent) {
						// send message at all the widget (exepted this one)
						SendMultiCast(m_localShortcut[iii]->generateEventId, m_localShortcut[iii]->eventData);
					}
					// send message direct to the current widget (in every case, really useful for some generic windows shortcut)
					OnReceiveMessage(this, m_localShortcut[iii]->generateEventId, m_localShortcut[iii]->eventData);
				} // no else
				return true;
			}
		}
	}
	return false;
}


void ewol::Widget::GrabCursor(void)
{
	if (false == m_grabCursor) {
		eSystem::InputEventGrabPointer(this);
		m_grabCursor = true;
	}
}

void ewol::Widget::UnGrabCursor(void)
{
	if (true==m_grabCursor) {
		eSystem::InputEventUnGrabPointer();
		m_grabCursor = false;
	}
}


bool ewol::Widget::GetGrabStatus(void)
{
	return m_grabCursor;
}



void ewol::Widget::SetCursor(ewol::cursorDisplay_te _newCursor)
{
	EWOL_DEBUG("Change Cursor in " << _newCursor);
	m_cursorDisplay = _newCursor;
	guiInterface::SetCursor(m_cursorDisplay);
}

ewol::cursorDisplay_te ewol::Widget::GetCursor(void)
{
	return m_cursorDisplay;
}

bool ewol::Widget::LoadXML(TiXmlNode* _node)
{
	if (NULL==_node) {
		return false;
	}
	bool ret = true;
	const char *tmpAttributeValue = _node->ToElement()->Attribute("name");
	if (NULL != tmpAttributeValue) {
		SetName(tmpAttributeValue);
	}
	tmpAttributeValue = _node->ToElement()->Attribute("fill");
	if (NULL != tmpAttributeValue) {
		if (strcmp("false,false", tmpAttributeValue)==0) {
			SetFill(bvec2(false,false));
		} else if (strcmp("false,true", tmpAttributeValue)==0) {
			SetFill(bvec2(false,true));
		} else if (strcmp("true,false", tmpAttributeValue)==0) {
			SetFill(bvec2(true,false));
		} else if (strcmp("false", tmpAttributeValue)==0) {
			SetFill(bvec2(false,false));
		} else {
			SetFill(bvec2(true,true));
		}
	}
	tmpAttributeValue = _node->ToElement()->Attribute("expand");
	if (NULL != tmpAttributeValue) {
		if (strcmp("false,false", tmpAttributeValue)==0) {
			SetExpand(bvec2(false,false));
		} else if (strcmp("false,true", tmpAttributeValue)==0) {
			SetExpand(bvec2(false,true));
		} else if (strcmp("true,false", tmpAttributeValue)==0) {
			SetExpand(bvec2(true,false));
		} else if (strcmp("false", tmpAttributeValue)==0) {
			SetExpand(bvec2(false,false));
		} else {
			SetExpand(bvec2(true,true));
		}
	}
	tmpAttributeValue = _node->ToElement()->Attribute("hide");
	if (NULL != tmpAttributeValue) {
		if (strcmp("true", tmpAttributeValue)==0) {
			Hide();
		} else {
			Show();
		}
	}
	tmpAttributeValue = _node->ToElement()->Attribute("focus");
	if (NULL != tmpAttributeValue) {
		if (strcmp("true", tmpAttributeValue)==0) {
			KeepFocus();
		}
	}
	tmpAttributeValue = _node->ToElement()->Attribute("min-size");
	if (NULL != tmpAttributeValue) {
		m_userMinSize.SetString(tmpAttributeValue);
	}
	tmpAttributeValue = _node->ToElement()->Attribute("max-size");
	if (NULL != tmpAttributeValue) {
		m_userMaxSize.SetString(tmpAttributeValue);
	}
	EWOL_DEBUG("Widget parse: m_hide=" << m_hide << "  m_userMinSize=" << m_userMinSize << "  m_userMaxSize=" << m_userMaxSize << "  m_userFill=" << m_userFill << "  m_userExpand=" << m_userExpand);
	MarkToRedraw();
	return ret;
}

bool ewol::Widget::StoreXML(TiXmlNode* _node)
{
	if (NULL==_node) {
		return false;
	}
	/*
	TiXmlElement * element = new TiXmlElement(__class__);
	if (NULL == element) {
		EWOL_ERROR("TinyXML node allocation error");
		return false;
	}
	_node->LinkEndChild(element);
	*/
	if (GetName().Size()!=0) {
		_node->ToElement()->SetAttribute("name", GetName().c_str() );
	}
	
	if (m_userMinSize.GetPixel() != vec2(0,0)) {
		_node->ToElement()->SetAttribute("min-size", m_userMinSize.GetString().c_str() );
	}
	if (m_userMaxSize.GetPixel() != vec2(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE)) {
		_node->ToElement()->SetAttribute("max-size", m_userMaxSize.GetString().c_str() );
	}
	if (m_userExpand != bvec2(false,false)) {
		etk::UString tmpVal = etk::UString(m_userExpand.x()) + "," + etk::UString(m_userExpand.y());
		_node->ToElement()->SetAttribute("expand", tmpVal.c_str() );
	}
	if (m_userFill != bvec2(false,false)) {
		etk::UString tmpVal = etk::UString(m_userFill.x()) + "," + etk::UString(m_userFill.y());
		_node->ToElement()->SetAttribute("fill", tmpVal.c_str() );
	}
	if (IsHide() != false) {
		_node->ToElement()->SetAttribute("hide", "true" );
	}
	return true;
}


ewol::Widget* ewol::Widget::GetWidgetNamed(const etk::UString& _widgetName)
{
	if (GetName()==_widgetName) {
		return this;
	}
	return NULL;
}


bool ewol::Widget::SystemEventEntry(ewol::EventEntrySystem& _event)
{
	if (NULL != m_up) {
		if (true==m_up->SystemEventEntry(_event)) {
			return true;
		}
	}
	return OnEventEntry(_event.m_event);
}

bool ewol::Widget::SystemEventInput(ewol::EventInputSystem& _event)
{
	if (NULL != m_up) {
		if (true==m_up->SystemEventInput(_event)) {
			return true;
		}
	}
	return OnEventInput(_event.m_event);
}

