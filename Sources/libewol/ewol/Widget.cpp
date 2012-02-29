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

ewol::Widget::Widget(void)
{
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
}

ewol::Widget::~Widget(void)
{
	//ewol::widgetMessageMultiCast::Rm(GetWidgetId());
}


void ewol::Widget::MarkToRemove(void)
{
	// Remove his own focus...
	ewol::widgetManager::Rm(this);
	// merk to remova at the next cycle
	ewol::EObjectManager::MarkToRemoved(this);
}


bool ewol::Widget::CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY)
{
	m_size.x = availlableX;
	m_size.y = availlableY;
	MarkToReedraw();
	return true;
}


bool ewol::Widget::GenEventInput(int32_t IdInput, eventInputType_te typeEvent, coord2D_ts pos)
{
	eventPosition_ts eventPos;
	eventPos.abs   = pos;
	eventPos.local.x = pos.x - m_origin.x;
	eventPos.local.y = pos.y - m_origin.y;
	//EWOL_DEBUG(" event on abs=(" << eventPos.abs.x << "," << eventPos.abs.y << ") local=(" << eventPos.local.x << "," << eventPos.local.y << ")");
	return OnEventInput(IdInput, typeEvent, eventPos);
}

bool ewol::Widget::GenEventShortCut(bool shift, bool control, bool alt, bool meta, uint32_t unicodeValue)
{
	return false;
}


void ewol::Widget::OnFlipFlopEvent(void)
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

//#define TEST_CLIPPING_SIZE       (10)
//#define TEST_CLIPPING_SIZE       (3)
#define TEST_CLIPPING_SIZE       (0)

bool ewol::Widget::GenDraw(void)
{
	glPushMatrix();
	etkFloat_t testSizeX = m_size.x-TEST_CLIPPING_SIZE*2;
	etkFloat_t testSizeY = m_size.y-TEST_CLIPPING_SIZE*2;
	// here we invert the reference of the standard OpenGl view because the reference in the common display is Top left and not buttom left
	glViewport(                                       m_origin.x + TEST_CLIPPING_SIZE,
	            ewol::GetCurrentHeight() - m_size.y - m_origin.y + TEST_CLIPPING_SIZE,
	            testSizeX,
	            testSizeY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthoEwol(-testSizeX/2, testSizeX/2, testSizeY/2, -testSizeY/2, -1, 1);
	//glOrthoEwol(0., m_size.x, 0., -m_size.y, 1., 20.);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-testSizeX/2, -testSizeY/2, -1.0);

	bool valRet = OnDraw();
	glPopMatrix();
	return valRet;
}
