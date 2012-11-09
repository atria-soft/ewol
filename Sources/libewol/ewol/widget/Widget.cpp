/**
 *******************************************************************************
 * @file ewol/Widget.cpp
 * @brief basic ewol Widget (Sources)
 * @author Edouard DUPIN
 * @date 18/10/2011
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#include <ewol/widget/Widget.h>
#include <ewol/eObject/EObjectManager.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/ewol.h>
#include <ewol/openGL/openGL.h>

char* ewol::GetCharTypeMoveEvent(eventKbMoveType_te type)
{
	char * returnValue = "?";
	switch(type) {
		case ewol::EVENT_KB_MOVE_TYPE_NONE:				returnValue = "---";			break;
		case ewol::EVENT_KB_MOVE_TYPE_LEFT:				returnValue = "LEFT";			break;
		case ewol::EVENT_KB_MOVE_TYPE_RIGHT:			returnValue = "RIGHT";			break;
		case ewol::EVENT_KB_MOVE_TYPE_UP:				returnValue = "UP";				break;
		case ewol::EVENT_KB_MOVE_TYPE_DOWN:				returnValue = "DOWN";			break;
		case ewol::EVENT_KB_MOVE_TYPE_PAGE_UP:			returnValue = "PAGE_UP";		break;
		case ewol::EVENT_KB_MOVE_TYPE_PAGE_DOWN:		returnValue = "PAGE_DOWN";		break;
		case ewol::EVENT_KB_MOVE_TYPE_START:			returnValue = "START";			break;
		case ewol::EVENT_KB_MOVE_TYPE_END:				returnValue = "END";			break;
		case ewol::EVENT_KB_MOVE_TYPE_CENTER:			returnValue = "CENTER";			break;
		case ewol::EVENT_KB_MOVE_TYPE_ARRET_DEFIL:		returnValue = "ARRET_DEFIL";	break;
		case ewol::EVENT_KB_MOVE_TYPE_WAIT:				returnValue = "WAIT";			break;
		case ewol::EVENT_KB_MOVE_TYPE_INSERT:			returnValue = "INSERT";			break;
		case ewol::EVENT_KB_MOVE_TYPE_F1:				returnValue = "F1";				break;
		case ewol::EVENT_KB_MOVE_TYPE_F2:				returnValue = "F2";				break;
		case ewol::EVENT_KB_MOVE_TYPE_F3:				returnValue = "F3";				break;
		case ewol::EVENT_KB_MOVE_TYPE_F4:				returnValue = "F4";				break;
		case ewol::EVENT_KB_MOVE_TYPE_F5:				returnValue = "F5";				break;
		case ewol::EVENT_KB_MOVE_TYPE_F6:				returnValue = "F6";				break;
		case ewol::EVENT_KB_MOVE_TYPE_F7:				returnValue = "F7";				break;
		case ewol::EVENT_KB_MOVE_TYPE_F8:				returnValue = "F8";				break;
		case ewol::EVENT_KB_MOVE_TYPE_F9:				returnValue = "F9";				break;
		case ewol::EVENT_KB_MOVE_TYPE_F10:				returnValue = "F10";			break;
		case ewol::EVENT_KB_MOVE_TYPE_F11:				returnValue = "F11";			break;
		case ewol::EVENT_KB_MOVE_TYPE_F12:				returnValue = "F12";			break;
		case ewol::EVENT_KB_MOVE_TYPE_CAPLOCK:			returnValue = "CAPLOCK";		break;
		case ewol::EVENT_KB_MOVE_TYPE_SHIFT_LEFT:		returnValue = "SHIFT_LEFT";		break;
		case ewol::EVENT_KB_MOVE_TYPE_SHIFT_RIGHT:		returnValue = "SHIFT_RIGHT";	break;
		case ewol::EVENT_KB_MOVE_TYPE_CTRL_LEFT:		returnValue = "CTRL_LEFT";		break;
		case ewol::EVENT_KB_MOVE_TYPE_CTRL_RIGHT:		returnValue = "CTRL_RIGHT";		break;
		case ewol::EVENT_KB_MOVE_TYPE_META_LEFT:		returnValue = "META_LEFT";		break;
		case ewol::EVENT_KB_MOVE_TYPE_META_RIGHT:		returnValue = "META_RIGHT";		break;
		case ewol::EVENT_KB_MOVE_TYPE_ALT:				returnValue = "ALT";			break;
		case ewol::EVENT_KB_MOVE_TYPE_ALT_GR:			returnValue = "ALT_GR";			break;
		case ewol::EVENT_KB_MOVE_TYPE_CONTEXT_MENU:		returnValue = "CONTEXT_MENU";	break;
		case ewol::EVENT_KB_MOVE_TYPE_VER_NUM:			returnValue = "VER_NUM";		break;
	}
	return returnValue;
}


#undef __class__
#define __class__	"Widget"

/**
 * @brief Constructor of the widget classes
 * @param ---
 * @return (no execption generated (not managed in embended platform))
 */
ewol::Widget::Widget(void)
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


/**
 * @brief Destructor of the widget classes
 * @param ---
 * @return (no execption generated (not managed in embended platform))
 */
ewol::Widget::~Widget(void)
{
	// Remove his own focus...
	ewol::widgetManager::Rm(this);
	// clean all the short-cut ...
	ShortCutClean();
	
}


/**
 * @brief Set the widget hidden
 * @param ---
 * @return ---
 */
void ewol::Widget::Hide(void)
{
	m_hide = true;
	MarkToRedraw();
	ewol::RequestUpdateSize();
}


/**
 * @brief Set the widget visible
 * @param ---
 * @return ---
 */
void ewol::Widget::Show(void)
{
	m_hide = false;
	MarkToRedraw();
	ewol::RequestUpdateSize();
}


/**
 * @brief Parrent set the possible diplay size of the current widget whith his own possibilities
 *        By default this save the widget availlable size in the widget size
 * @param[in] availlableX Availlable horisantal pixel size
 * @param[in] availlableY Availlable vertical pixel size
 * @return ---
 */
bool ewol::Widget::CalculateSize(float availlableX, float availlableY)
{
	m_size.x = availlableX;
	m_size.y = availlableY;
	MarkToRedraw();
	return true;
}


/**
 * @brief Set focus on this widget
 * @param ---
 * @return return true if the widget keep the focus
 */
bool ewol::Widget::SetFocus(void)
{
	if (true == m_canFocus) {
		m_hasFocus = true;
		OnGetFocus();
		return true;
	}
	return false;
}


/**
 * @brief Remove the focus on this widget
 * @param ---
 * @return return true if the widget have release his focus (if he has it)
 */
bool ewol::Widget::RmFocus(void)
{
	if (true == m_canFocus) {
		m_hasFocus = false;
		OnLostFocus();
		return true;
	}
	return false;
}


/**
 * @brief Set the capability to have the focus
 * @param[in] canFocusState new focus capability
 * @return ---
 */
void ewol::Widget::SetCanHaveFocus(bool canFocusState)
{
	m_canFocus = canFocusState;
	if (true == m_hasFocus) {
		(void)RmFocus();
	}
}

/**
 * @brief Keep the focus on this widget ==> this remove the previous focus on all other widget
 * @param ---
 * @return ---
 */
void ewol::Widget::KeepFocus(void)
{
	ewol::widgetManager::FocusKeep(this);
}

/**
 * @brief extern interface to request a draw ...  (called by the drawing thread [Android, X11, ...])
 * This function generate a clipping with the viewport openGL system. Like this a widget draw can not draw over an other widget
 * @note This function is virtual for the scrolled widget, and the more complicated OpenGl widget
 * @param[in] displayProp properties of the current display
 * @return ---
 */
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
		etk::Matrix4 tmpTranslate = etk::matrix::Translate(-tmpclipX/2 - (tmpOriginX-m_origin.x), -m_size.y/2, -1.0);
		etk::Matrix4 tmpScale = etk::matrix::Scale(m_zoom, m_zoom, 1.0);
		etk::Matrix4 tmpProjection = etk::matrix::Perspective(-tmpclipX/2, tmpclipX/2, -m_size.y/2, m_size.y/2, -1, 1);
		etk::Matrix4 tmpMat = tmpProjection * tmpScale * tmpTranslate;
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
		#if 1
			etk::Matrix4 tmpTranslate = etk::matrix::Translate(-m_size.x/2, -m_size.y/2, -1.0);
			etk::Matrix4 tmpScale = etk::matrix::Scale(m_zoom, m_zoom, 1.0);
			etk::Matrix4 tmpProjection = etk::matrix::Perspective(-m_size.x/2, m_size.x/2, -m_size.y/2, m_size.y/2, -1, 1);
			etk::Matrix4 tmpMat = tmpProjection * tmpScale * tmpTranslate;
		#else
			etk::Matrix4 tmpMat = etk::matrix::Perspective(0, m_size.x, 0, m_size.y, -1, 1);
		#endif
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

/**
 * @brief Request that the current widegt have a periodic call
 * @param statusToSet true if the periodic call is needed
 * @return ---
 */
void ewol::Widget::PeriodicCallSet(bool statusToSet)
{
	if (true == statusToSet) {
		ewol::widgetManager::PeriodicCallAdd(this);
	} else {
		ewol::widgetManager::PeriodicCallRm(this);
	}
}

/**
 * @brief The widget mark itself that it need to regenerate the nest time.
 * @param ---
 * @return ---
 */
void ewol::Widget::MarkToRedraw(void)
{
	m_needRegenerateDisplay = true;
	ewol::widgetManager::MarkDrawingIsNeeded();
};



// ----------------------------------------------------------------------------------------------------------------
// -- Shortcut : management of the shortcut
// ----------------------------------------------------------------------------------------------------------------

/**
 * @brief Add a specific shortcut with his description
 * @param[in] descriptiveString Description string of the shortcut
 * @param[in] generateEventId Event generic of the element
 * @param[in] data Associate data wit the event
 * @return ---
 */
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
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_F12;
	} else if(NULL != strstr(descriptiveString, "F11") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_F11;
	} else if(NULL != strstr(descriptiveString, "F10") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_F10;
	} else if(NULL != strstr(descriptiveString, "F9") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_F9;
	} else if(NULL != strstr(descriptiveString, "F8") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_F8;
	} else if(NULL != strstr(descriptiveString, "F7") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_F7;
	} else if(NULL != strstr(descriptiveString, "F6") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_F6;
	} else if(NULL != strstr(descriptiveString, "F5") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_F5;
	} else if(NULL != strstr(descriptiveString, "F4") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_F4;
	} else if(NULL != strstr(descriptiveString, "F3") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_F3;
	} else if(NULL != strstr(descriptiveString, "F2") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_F2;
	} else if(NULL != strstr(descriptiveString, "F1") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_F1;
	} else if(NULL != strstr(descriptiveString, "LEFT") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_LEFT;
	} else if(NULL != strstr(descriptiveString, "RIGHT") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_RIGHT;
	} else if(NULL != strstr(descriptiveString, "UP") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_UP;
	} else if(NULL != strstr(descriptiveString, "DOWN") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_DOWN;
	} else if(NULL != strstr(descriptiveString, "PAGE_UP") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_PAGE_UP;
	} else if(NULL != strstr(descriptiveString, "PAGE_DOWN") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_PAGE_DOWN;
	} else if(NULL != strstr(descriptiveString, "START") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_START;
	} else if(NULL != strstr(descriptiveString, "END") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_END;
	} else if(NULL != strstr(descriptiveString, "CENTER") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_CENTER;
	} else if(NULL != strstr(descriptiveString, "ARRET_DEFIL") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_ARRET_DEFIL;
	} else if(NULL != strstr(descriptiveString, "WAIT") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_WAIT;
	} else if(NULL != strstr(descriptiveString, "INSERT") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_INSERT;
	} else if(NULL != strstr(descriptiveString, "CAPLOCK") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_CAPLOCK;
	} else if(NULL != strstr(descriptiveString, "CONTEXT_MENU") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_CONTEXT_MENU;
	} else if(NULL != strstr(descriptiveString, "VER_NUM") ) {
		tmpElement->keyboardMoveValue = ewol::EVENT_KB_MOVE_TYPE_VER_NUM;
	} else {
		tmpElement->unicodeValue = descriptiveString[strlen(descriptiveString) -1];
	}
	// add it on the List ...
	m_localShortcut.PushBack(tmpElement);
}

/**
 * @brief Remove all curent shortCut
 * @param ---
 * @return ---
 */
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


/**
 * @brief Event on a short-cut of this Widget (in case of return false, the event on the keyevent will arrive in the function @ref OnEventKb)
 * @param[in] special all the special kay pressed at this time
 * @param[in] unicodeValue key pressed by the user not used if the kbMove!=ewol::EVENT_KB_MOVE_TYPE_NONE
 * @param[in] kbMove special key of the keyboard
 * @return true if the event has been used
 * @return false if the event has not been used
 * @note To prevent some error when you get an event get it if it is down and Up ... ==> like this it could not generate some ununderstanding error
 */
bool ewol::Widget::OnEventShortCut(ewol::specialKey_ts& special, uniChar_t unicodeValue, ewol::eventKbMoveType_te kbMove, bool isDown)
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
			    && (    (    m_localShortcut[iii]->keyboardMoveValue == ewol::EVENT_KB_MOVE_TYPE_NONE
			              && m_localShortcut[iii]->unicodeValue == unicodeValue)
			         || (    m_localShortcut[iii]->keyboardMoveValue == kbMove
			              && m_localShortcut[iii]->unicodeValue == 0)
			       ) )
			{
				if (isDown) {
					if (true == m_localShortcut[iii]->broadcastEvent) {
						// send message at all the widget
						SendMultiCast(m_localShortcut[iii]->generateEventId, m_localShortcut[iii]->eventData);
					} else {
						// send message direct to the current widget
						OnReceiveMessage(this, m_localShortcut[iii]->generateEventId, m_localShortcut[iii]->eventData);
					}
				} // no else
				return true;
			}
		}
	}
	return false;
}
