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
#include <ewol/openGl.h>

char* ewol::GetCharTypeMoveEvent(eventKbMoveType_te type)
{
	char * returnValue = "?";
	switch(type) {
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
	glPushMatrix();
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
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrthoEwol(-tmpclipX/2, tmpclipX/2, -m_size.y/2, m_size.y/2, -1, 1);
		//glOrthoEwol(0., m_size.x, 0., -m_size.y, 1., 20.);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(-tmpclipX/2 - (tmpOriginX-m_origin.x), -m_size.y/2, -1.0);
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
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrthoEwol(-m_size.x/2, m_size.x/2, -m_size.y/2, m_size.y/2, -1, 1);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(-m_size.x/2, -m_size.y/2, -1.0);
		// Call the widget drawing methode
		displayProp.m_origin = m_origin;
		displayProp.m_size = m_size;
		OnDraw(displayProp);
	}
	glPopMatrix();
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
