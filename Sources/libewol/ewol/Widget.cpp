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

#include <ewol/Widget.h>
#include <ewol/EObjectManager.h>
#include <ewol/WidgetManager.h>
#include <ewol/ewol.h>
#include <ewol/importgl.h>

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
	m_currentDrawId = 0;
	m_currentCreateId = 1;
	m_needFlipFlop = true;
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
	for(int32_t iii=0 ; iii<NB_BOUBLE_BUFFER ; iii++) {
		m_hide[iii] = false;
	}
}


//!< EObject name :
extern const char * const ewol::TYPE_EOBJECT_WIDGET = "Widget";

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool ewol::Widget::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == ewol::TYPE_EOBJECT_WIDGET) {
		return true;
	} else {
		if(true == ewol::EObject::CheckObjectType(objectType)) {
			return true;
		}
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const ewol::Widget::GetObjectType(void)
{
	return ewol::TYPE_EOBJECT_WIDGET;
}


/**
 * @brief Set the widget hidden
 * @param ---
 * @return ---
 */
void ewol::Widget::Hide(void)
{
	m_hide[m_currentCreateId] = true;
	MarkToReedraw();
	ewol::RequestUpdateSize();
}


/**
 * @brief Set the widget visible
 * @param ---
 * @return ---
 */
void ewol::Widget::Show(void)
{
	m_hide[m_currentCreateId] = false;
	MarkToReedraw();
	ewol::RequestUpdateSize();
}


/**
 * @brief This will be equivalent at the destructor @ref ~Widget
 * @note this fuction "mark" the widget as removed an inform the widget manager that the widget has been removed by the user.
 * @note All the EObject are inform that an other EObject is removed ... @ref ewol::EObject
 * @param ---
 * @return ---
 */
void ewol::Widget::MarkToRemove(void)
{
	// Remove his own focus...
	ewol::widgetManager::Rm(this);
	// merk to remova at the next cycle
	ewol::EObjectManager::MarkToRemoved(this);
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
	MarkToReedraw();
	return true;
}


/**
 * @brief Event generated to inform a flip-flop has occured on the current widget
 * @param ---
 * @return ---
 */
void ewol::Widget::OnFlipFlopEvent(void)
{
	if (true == m_needFlipFlop) {
		bool save = m_hide[m_currentCreateId];
		
		m_currentDrawId++;
		if (NB_BOUBLE_BUFFER<=m_currentDrawId) {
			m_currentDrawId = 0;
		}
		m_currentCreateId++;
		if (NB_BOUBLE_BUFFER<=m_currentCreateId) {
			m_currentCreateId = 0;
		}
		m_needFlipFlop = false;
		
		m_hide[m_currentCreateId] = save;
	}
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
 * @param ---
 * @return ---
 */
void ewol::Widget::GenDraw(void)
{
	if (true==m_hide[m_currentDrawId]){
		// widget is hidden ...
		return;
	}
	glPushMatrix();
	// here we invert the reference of the standard OpenGl view because the reference in the common display is Top left and not buttom left
	glViewport(                                       m_origin.x,
	            ewol::GetCurrentHeight() - m_size.y - m_origin.y,
	            m_size.x,
	            m_size.y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthoEwol(-m_size.x/2, m_size.x/2, m_size.y/2, -m_size.y/2, -1, 1);
	//glOrthoEwol(0., m_size.x, 0., -m_size.y, 1., 20.);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-m_size.x/2, -m_size.y/2, -1.0);
	// Call the widget drawing methode
	OnDraw();
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
