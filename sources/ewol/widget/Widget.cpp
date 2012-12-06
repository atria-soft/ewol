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


#undef __class__
#define __class__	"Widget"

ewol::Widget::Widget(void) :
	m_userMaxSize(-1,-1)
{
	m_limitMouseEvent = 3;
	m_needRegenerateDisplay = true;
	m_origin.x = 0.0;
	m_origin.y = 0.0;
	m_size.x = 10.0;
	m_size.y = 10.0;
	m_minSize.x = -1.0;
	m_minSize.y = -1.0;
	// user settings :
	m_userMinSize.x = -1.0;
	m_userMinSize.y = -1.0;
	SetExpendX();
	SetExpendY();
	SetFillX();
	SetFillY();
	m_canFocus = false;
	m_hasFocus = false;
	m_hide = false;
	m_zoom = 1.0;
}


ewol::Widget::~Widget(void)
{
	// Remove his own focus...
	ewol::widgetManager::Rm(this);
	// clean all the short-cut ...
	ShortCutClean();
	
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


bool ewol::Widget::CalculateSize(float availlableX, float availlableY)
{
	m_size.x = availlableX;
	m_size.y = availlableY;
	MarkToRedraw();
	return true;
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


void ewol::Widget::SetCanHaveFocus(bool canFocusState)
{
	m_canFocus = canFocusState;
	if (true == m_hasFocus) {
		(void)RmFocus();
	}
}


void ewol::Widget::KeepFocus(void)
{
	ewol::widgetManager::FocusKeep(this);
}


void ewol::Widget::GenDraw(DrawProperty displayProp)
{
	if (true==m_hide){
		// widget is hidden ...
		return;
	}
	ewol::openGL::Push();
	if(    (displayProp.m_origin.x > m_origin.x)
	    || (displayProp.m_origin.x + displayProp.m_size.x < m_size.x + m_origin.x) ) {
		// here we invert the reference of the standard OpenGl view because the reference in the common display is Top left and not buttom left
		int32_t tmpOriginX = etk_max(displayProp.m_origin.x, m_origin.x);
		int32_t tmppp1 = displayProp.m_origin.x + displayProp.m_size.x;
		int32_t tmppp2 = m_origin.x + m_size.x;
		int32_t tmpclipX = etk_min(tmppp1, tmppp2) - tmpOriginX;
		
		int32_t tmpOriginY = etk_max(displayProp.m_origin.y, m_origin.y);
		tmppp1 = displayProp.m_origin.y + displayProp.m_size.y;
		tmppp2 = m_origin.y + m_size.y;
		//int32_t tmpclipY = etk_min(tmppp1, tmppp2) - tmpOriginX;
		
		glViewport( tmpOriginX,
		            tmpOriginY,
		            tmpclipX,
		            m_size.y);
		mat4 tmpTranslate = etk::Matrix4::Translate(vec3((float)(-tmpclipX/2 - (tmpOriginX-m_origin.x)), (float)(-m_size.y/2.0), -1.0f));
		mat4 tmpScale = etk::Matrix4::Scale(vec3(m_zoom, m_zoom, 1.0f));
		mat4 tmpProjection = etk::Matrix4::Perspective(-tmpclipX/2, tmpclipX/2, -m_size.y/2, m_size.y/2, -1, 1);
		mat4 tmpMat = tmpProjection * tmpScale * tmpTranslate;
		// set internal matrix system :
		ewol::openGL::SetMatrix(tmpMat);
		// Call the widget drawing methode
		displayProp.m_origin.x = tmpOriginX;
		displayProp.m_origin.y = tmpOriginY;
		displayProp.m_size.x = tmpclipX;
		displayProp.m_size.y = m_size.y;
		OnDraw(displayProp);
	} else {
		glViewport( m_origin.x,
		            m_origin.y,
		            m_size.x,
		            m_size.y);
		mat4 tmpTranslate = etk::Matrix4::Translate(vec3(-m_size.x/2, -m_size.y/2, -1.0f));
		mat4 tmpScale = etk::Matrix4::Scale(vec3(m_zoom, m_zoom, 1.0f));
		mat4 tmpProjection = etk::Matrix4::Perspective(-m_size.x/2, m_size.x/2, -m_size.y/2, m_size.y/2, -1, 1);
		mat4 tmpMat = tmpProjection * tmpScale * tmpTranslate;
		// set internal matrix system :
		ewol::openGL::SetMatrix(tmpMat);
		// Call the widget drawing methode
		displayProp.m_origin = m_origin;
		displayProp.m_size = m_size;
		OnDraw(displayProp);
	}
	ewol::openGL::Pop();
	return;
}


void ewol::Widget::PeriodicCallSet(bool statusToSet)
{
	if (true == statusToSet) {
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


void ewol::Widget::SetZoom(float newVal)
{
	m_zoom = newVal;
	MarkToRedraw();
}

float ewol::Widget::GetZoom(void)
{
	return m_zoom;
}

void ewol::Widget::SetOrigin(float x, float y)
{
	m_origin.x=x;
	m_origin.y=y;
}

vec2 ewol::Widget::GetOrigin(void)
{
	return m_origin;
}

vec2 ewol::Widget::RelativePosition(vec2 pos)
{
	pos.x -= m_origin.x;
	pos.y -= m_origin.y;
	return pos;
}

bool ewol::Widget::CalculateMinSize(void)
{
	m_minSize.x = m_userMinSize.x;
	m_minSize.y = m_userMinSize.y;
	MarkToRedraw();
	return true;
}

void ewol::Widget::SetMinSize(float x, float y)
{
	m_userMinSize.x = x;
	m_userMinSize.y = y;
}

vec2 ewol::Widget::GetMinSize(void)
{
	if (false==IsHide()) {
		return m_minSize;
	}
	return vec2(0,0);
}

void ewol::Widget::SetMaxSize(vec2 size)
{
	m_userMaxSize = size;
	ewol::RequestUpdateSize();
}

vec2 ewol::Widget::GetMaxSize(void)
{
	return m_userMaxSize;
}

vec2 ewol::Widget::GetSize(void)
{
	if (false==IsHide()) {
		return m_size;
	}
	return vec2(0,0);
}

void ewol::Widget::SetExpendX(bool newExpend)
{
	m_userExpend.x = newExpend;
	ewol::RequestUpdateSize();
	MarkToRedraw();
}

bool ewol::Widget::CanExpentX(void)
{
	if (false==IsHide()) {
		return m_userExpend.x;
	}
	return false;
}

void ewol::Widget::SetExpendY(bool newExpend)
{
	m_userExpend.y = newExpend;
	ewol::RequestUpdateSize();
	MarkToRedraw();
}

bool ewol::Widget::CanExpentY(void)
{
	if (false==IsHide()) {
		return m_userExpend.y;
	}
	return false;
}

void ewol::Widget::SetFillX(bool newFill)
{
	m_userFill.x = newFill;
	MarkToRedraw();
}

bool ewol::Widget::CanFillX(void)
{
	return m_userFill.x;
}

void ewol::Widget::SetFillY(bool newFill)
{
	m_userFill.y = newFill;
	MarkToRedraw();
}

bool ewol::Widget::CanFillY(void)
{
	return m_userFill.y;
}

// ----------------------------------------------------------------------------------------------------------------
// -- Shortcut : management of the shortcut
// ----------------------------------------------------------------------------------------------------------------

void ewol::Widget::ShortCutAdd(const char * descriptiveString, const char * generateEventId, etk::UString data, bool broadcast)
{
	if(		NULL==descriptiveString
		||	0==strlen(descriptiveString))
	{
		EWOL_ERROR("try to add shortcut with no descriptive string ...");
		return;
	}
	
	EventShortCut* tmpElement = new EventShortCut();
	if (NULL == tmpElement) {
		EWOL_ERROR("allocation error ... Memory error ...");
		return;
	}
	tmpElement->broadcastEvent = broadcast;
	tmpElement->generateEventId = generateEventId;
	tmpElement->eventData = data;
	// parsing of the string :
	//"ctrl+shift+alt+meta+s"
	const char * tmp = strstr(descriptiveString, "ctrl");
	if(NULL != tmp) {
		tmpElement->specialKey.ctrl = true;
	}
	tmp = strstr(descriptiveString, "shift");
	if(NULL != tmp) {
		tmpElement->specialKey.shift = true;
	}
	tmp = strstr(descriptiveString, "alt");
	if(NULL != tmp) {
		tmpElement->specialKey.alt = true;
	}
	tmp = strstr(descriptiveString, "meta");
	if(NULL != tmp) {
		tmpElement->specialKey.meta = true;
	}
	if(NULL != strstr(descriptiveString, "F12") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF12;
	} else if(NULL != strstr(descriptiveString, "F11") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF11;
	} else if(NULL != strstr(descriptiveString, "F10") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF10;
	} else if(NULL != strstr(descriptiveString, "F9") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF9;
	} else if(NULL != strstr(descriptiveString, "F8") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF8;
	} else if(NULL != strstr(descriptiveString, "F7") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF7;
	} else if(NULL != strstr(descriptiveString, "F6") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF6;
	} else if(NULL != strstr(descriptiveString, "F5") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF5;
	} else if(NULL != strstr(descriptiveString, "F4") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF4;
	} else if(NULL != strstr(descriptiveString, "F3") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF3;
	} else if(NULL != strstr(descriptiveString, "F2") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF2;
	} else if(NULL != strstr(descriptiveString, "F1") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardF1;
	} else if(NULL != strstr(descriptiveString, "LEFT") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardLeft;
	} else if(NULL != strstr(descriptiveString, "RIGHT") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardRight;
	} else if(NULL != strstr(descriptiveString, "UP") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardUp;
	} else if(NULL != strstr(descriptiveString, "DOWN") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardDown;
	} else if(NULL != strstr(descriptiveString, "PAGE_UP") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardPageUp;
	} else if(NULL != strstr(descriptiveString, "PAGE_DOWN") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardPageDown;
	} else if(NULL != strstr(descriptiveString, "START") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardStart;
	} else if(NULL != strstr(descriptiveString, "END") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardEnd;
	} else if(NULL != strstr(descriptiveString, "PRINT") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardPrint;
	} else if(NULL != strstr(descriptiveString, "ARRET_DEFIL") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardStopDefil;
	} else if(NULL != strstr(descriptiveString, "WAIT") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardWait;
	} else if(NULL != strstr(descriptiveString, "INSERT") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardInsert;
	} else if(NULL != strstr(descriptiveString, "CAPLOCK") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardCapLock;
	} else if(NULL != strstr(descriptiveString, "CONTEXT_MENU") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardContextMenu;
	} else if(NULL != strstr(descriptiveString, "NUM_LOCK") ) {
		tmpElement->keyboardMoveValue = ewol::keyEvent::keyboardNumLock;
	} else {
		tmpElement->unicodeValue = descriptiveString[strlen(descriptiveString) -1];
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


bool ewol::Widget::OnEventShortCut(ewol::SpecialKey& special, uniChar_t unicodeValue, ewol::keyEvent::keyboard_te kbMove, bool isDown)
{
	if (unicodeValue >= 'A' && unicodeValue <='Z') {
		unicodeValue += 'a' - 'A';
	}
	//EWOL_INFO("Try to find generic shortcut ...");
	for(int32_t iii=m_localShortcut.Size()-1; iii>=0; iii--) {
		if(NULL != m_localShortcut[iii]) {
			if(    m_localShortcut[iii]->specialKey.shift == special.shift
			    && m_localShortcut[iii]->specialKey.ctrl  == special.ctrl
			    && m_localShortcut[iii]->specialKey.alt   == special.alt
			    && m_localShortcut[iii]->specialKey.meta  == special.meta
			    && (    (    m_localShortcut[iii]->keyboardMoveValue == ewol::keyEvent::keyboardUnknow
			              && m_localShortcut[iii]->unicodeValue == unicodeValue)
			         || (    m_localShortcut[iii]->keyboardMoveValue == kbMove
			              && m_localShortcut[iii]->unicodeValue == 0)
			       ) )
			{
				if (isDown) {
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


