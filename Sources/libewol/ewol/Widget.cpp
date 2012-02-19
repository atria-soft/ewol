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
#include <ewol/WidgetManager.h>
#include <ewol/WidgetMessageMultiCast.h>
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
#define __class__	"ewol::Widget"

ewol::Widget::Widget(void)
{
	m_needRegenerateDisplay = true;
	m_currentDrawId = 0;
	m_currentCreateId = 1;
	m_needFlipFlop = false;
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
	ewol::widgetManager::Add(this);
	m_canFocus = false;
	m_hasFocus = false;
}

ewol::Widget::~Widget(void)
{
	ewol::widgetMessageMultiCast::Rm(GetWidgetId());
	ewol::widgetManager::Rm(this);
}

int32_t ewol::Widget::GetWidgetId(void)
{
	return ewol::widgetManager::Get(this);
}

coord2D_ts ewol::Widget::GetRealOrigin(void)
{
	coord2D_ts parentCoord;
	if( NULL != m_parrent) {
		parentCoord    = m_parrent->GetRealOrigin();
		parentCoord.x += m_origin.x;
		parentCoord.y += m_origin.y;
	} else {
		parentCoord.x = m_origin.x;
		parentCoord.y = m_origin.y;
	}
	return parentCoord;
}


bool ewol::Widget::CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY)
{
	m_size.x = availlableX;
	m_size.y = availlableY;
	MarkToReedraw();
	return true;
}


bool ewol::Widget::GenEventInput(int32_t IdInput, eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y)
{
	return OnEventInput(IdInput, typeEvent, x-m_origin.x, y-m_origin.y);
}

bool ewol::Widget::GenEventInputExternal(const char * generateEventId, etkFloat_t x, etkFloat_t y)
{
	bool ended = false;
	// For all external Event Requested : 
	for( int32_t jjj=0; jjj<m_externEvent.Size(); jjj++) {
		// Check if it is the same ID : 
		if (m_externEvent[jjj].generateEventId == generateEventId) {
			// get the widget Pointer:
			
			ewol::Widget * tmpWidget = ewol::widgetManager::Get(m_externEvent[jjj].widgetCall);
			if (NULL == tmpWidget) {
				EWOL_ERROR("Try to call an NULL Widget, it might be removed ... id=" << m_externEvent[jjj].widgetCall);
			} else {
				ended = tmpWidget->OnEventAreaExternal(GetWidgetId(), m_externEvent[jjj].generateEventIdExtern, NULL, x, y);
			}
			if (true == ended) {
				break;
			}
		}
	}
	return ended;
}

bool ewol::Widget::GenEventShortCut(bool shift, bool control, bool alt, bool meta, uint32_t unicodeValue)
{
	return false;
}


bool ewol::Widget::ExternLinkOnEvent(const char * eventName, int32_t widgetId, const char * eventExternId)
{
	if(NULL == eventName || 0 > widgetId) {
		EWOL_ERROR("Try to add extern event with wrong input ..");
		return false;
	}
	
	eventExtern_ts tmpEvent;
	// search the ID ...
	for(int32_t iii=0; iii < m_ListEventAvaillable.Size(); iii++) {
		if (strcmp(m_ListEventAvaillable[iii], eventName) == 0) {
			tmpEvent.generateEventId = m_ListEventAvaillable[iii];
			tmpEvent.widgetCall = widgetId;
			tmpEvent.generateEventIdExtern = eventExternId;
			m_externEvent.PushBack(tmpEvent);
			return true;
		}
	}
	EWOL_ERROR("Try to add extern event with Unknow EventID : \"" << eventName << "\"" );
	return false;
}


void ewol::Widget::DoubleBufferFlipFlop(void)
{
	if (true == m_needFlipFlop) {
		m_currentDrawId++;
		if (NB_BOUBLE_BUFFER<=m_currentDrawId) {
			m_currentDrawId = 0;
		}
		m_currentCreateId++;
		if (NB_BOUBLE_BUFFER<=m_currentCreateId) {
			m_currentCreateId = 0;
		}
		m_needFlipFlop = false;
	}
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

#define TEST_CLIPPING_SIZE       (10)
//#define TEST_CLIPPING_SIZE       (3)
//#define TEST_CLIPPING_SIZE       (0)

#ifdef __PLATFORM__Android
#	define   clipping_type       GLfloat
#	define   clipping_function   glClipPlanef
#else
#	define   clipping_type       GLdouble
#	define   clipping_function   glClipPlane
#endif
bool ewol::Widget::GenDraw(void)
{
	glPushMatrix();
#if 1
	glTranslatef(m_origin.x,m_origin.y, 0);
	//GLfloat
	clipping_type eqn1[4] = {0.0, 1.0, 0.0, -TEST_CLIPPING_SIZE}; // less the Y pos ...
	clipping_type eqn2[4] = {0.0, -1.0, 0.0, m_size.y-TEST_CLIPPING_SIZE}; // upper the y pos ...
	clipping_type eqn3[4] = {1.0, 0.0, 0.0, -TEST_CLIPPING_SIZE}; // less the x pos ...
	clipping_type eqn4[4] = {-1.0, 0.0, 0.0, m_size.x-TEST_CLIPPING_SIZE}; // upper the x pos ...
	//EWOL_DEBUG("widget size (" << m_size.x << "," << m_size.y << ")" );
	/* clip lower half -- y < 0 */
	glEnable(GL_CLIP_PLANE0);
	clipping_function(GL_CLIP_PLANE0, eqn1);
	glEnable(GL_CLIP_PLANE1);
	clipping_function(GL_CLIP_PLANE1, eqn2);
	glEnable(GL_CLIP_PLANE2);
	clipping_function(GL_CLIP_PLANE2, eqn3);
	glEnable(GL_CLIP_PLANE3);
	clipping_function(GL_CLIP_PLANE3, eqn4);
#else
	glTranslatef(m_origin.x,m_origin.y, 0);
	glEnable(GL_SCISSOR_TEST);
	glScissor(TEST_CLIPPING_SIZE, TEST_CLIPPING_SIZE, m_size.x-TEST_CLIPPING_SIZE, m_size.y-TEST_CLIPPING_SIZE);
#endif
	bool valRet = OnDraw();
	/*
	glDisable(GL_CLIP_PLANE3);
	glDisable(GL_CLIP_PLANE2);
	glDisable(GL_CLIP_PLANE1);
	glDisable(GL_CLIP_PLANE0);
	*/
	glPopMatrix();
	return valRet;
}
