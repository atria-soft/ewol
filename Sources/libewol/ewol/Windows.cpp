/**
 *******************************************************************************
 * @file ewol/Windows.cpp
 * @brief ewol window system (sources)
 * @author Edouard DUPIN
 * @date 20/10/2011
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

#include <etk/Types.h>
#include <etk/UString.h>
#include <ewol/Widget.h>
#include <ewol/Windows.h>
#include <ewol/OObject.h>
#include <ewol/Texture.h>
#include <ewol/Font.h>
#include <ewol/ewol.h>
#include <ewol/importgl.h>



#undef __class__
#define __class__	"ewol::Windows"


//list of local events : 
extern const char * const ewolEventWindowsHideKeyboard   = "ewol Windows hideKeyboard";


ewol::Windows::Windows(void)
{
	SetCanHaveFocus(true);
	m_subWidget = NULL;
	m_popUpWidget = NULL;
	m_keyBoardwidget = NULL;
	// enable specific drawing system ...
	SpecificDrawEnable();
	SetDecorationDisable();
	//KeyboardShow(KEYBOARD_MODE_CODE);
}

ewol::Windows::~Windows(void)
{
	if (NULL != m_subWidget) {
		delete(m_subWidget);
		m_subWidget=NULL;
	}
	if (NULL != m_popUpWidget) {
		delete(m_popUpWidget);
		m_popUpWidget=NULL;
	}
	if (NULL != m_keyBoardwidget) {
		delete(m_keyBoardwidget);
		m_keyBoardwidget=NULL;
	}
}

bool ewol::Windows::CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY)
{
	m_size.x = availlableX;
	m_size.y = availlableY;
	int32_t keyboardHigh = 0;
	if (NULL != m_keyBoardwidget && false == m_keyBoardwidget->IsHide() ) {
		m_keyBoardwidget->CalculateMinSize();
		coord tmpSize = m_keyBoardwidget->GetMinSize();
		keyboardHigh = (int32_t)tmpSize.y;
		m_keyBoardwidget->SetOrigin(0, m_size.y - keyboardHigh);
		m_keyBoardwidget->CalculateSize(m_size.x, keyboardHigh);
	}
	if (NULL != m_subWidget) {
		m_subWidget->CalculateMinSize();
		// TODO : Check if min Size is possible ...
		// TODO : Herited from MinSize .. and expand ???
		m_subWidget->CalculateSize(m_size.x, m_size.y - keyboardHigh);
	}
	if (NULL != m_popUpWidget) {
		m_popUpWidget->CalculateMinSize();
		m_popUpWidget->CalculateSize(m_size.x, m_size.y - keyboardHigh);
	}
	// regenerate all the display ...
	MarkToReedraw();
	return true;
}


bool ewol::Windows::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y)
{
	if (NULL != m_keyBoardwidget && false == m_keyBoardwidget->IsHide() ) {
		coord tmpSize = m_keyBoardwidget->GetMinSize();
		if (y > m_size.y - tmpSize.y) {
			m_keyBoardwidget->GenEventInput(IdInput, typeEvent, x, y);
			return true;
		}
	}
	// event go directly on the pop-up
	if (NULL != m_popUpWidget) {
		m_popUpWidget->GenEventInput(IdInput, typeEvent, x, y);
	// otherwise in the normal windows
	} else if (NULL != m_subWidget) {
		m_subWidget->GenEventInput(IdInput, typeEvent, x, y);
	}
	return true;
}

void ewol::Windows::SysDraw(void)
{

	//EWOL_DEBUG("Drow on (" << m_size.x << "," << m_size.y << ")");
	// set the size of the open GL system
	glViewport(0,0,m_size.x,m_size.y);
	
	// Clear the screen with transparency ...
	glClearColor(0.750, 0.750, 0.750, 0.5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
#if defined(__PLATFORM__Android)
	glOrtho(-m_size.x/2, m_size.x/2, m_size.y/2, -m_size.y/2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(-m_size.x/2, -m_size.y/2, -1.0);
#else
	glOrtho(0., m_size.x, 0., -m_size.y, 1., 20.);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, -m_size.y, -5);
#endif
	//http://www.khronos.org/opengles/documentation/opengles1_0/html/glBlendFunc.html
	
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_POLYGON_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);

	GenDraw();

	glDisable(GL_BLEND);
	return;
}

void ewol::Windows::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		// no decoration ...
	}
	if (NULL != m_subWidget) {
		m_subWidget->OnRegenerateDisplay();
	}
	if (NULL != m_popUpWidget) {
		m_popUpWidget->OnRegenerateDisplay();
	}
	if (NULL != m_keyBoardwidget && false == m_keyBoardwidget->IsHide() ) {
		m_keyBoardwidget->OnRegenerateDisplay();
	}
}


bool ewol::Windows::OnDraw(void)
{
	// first display the windows on the display
	if (NULL != m_subWidget) {
		m_subWidget->GenDraw();
		//EWOL_DEBUG("Draw Windows");
	}
	// second display the pop-up
	if (NULL != m_popUpWidget) {
		m_popUpWidget->GenDraw();
		//EWOL_DEBUG("Draw Pop-up");
	}
	if (NULL != m_keyBoardwidget && false == m_keyBoardwidget->IsHide() ) {
		m_keyBoardwidget->GenDraw();
		//EWOL_DEBUG("Draw Pop-up");
	}
	return true;
}



void ewol::Windows::SetSubWidget(ewol::Widget * widget)
{
	if (NULL != m_subWidget) {
		EWOL_INFO("Remove current main windows Widget...");
		delete(m_subWidget);
		m_subWidget = NULL;
	}
	m_subWidget = widget;
	// Regenerate the size calculation :
	CalculateSize(m_size.x, m_size.y);
}


void ewol::Windows::PopUpWidgetPush(ewol::Widget * widget)
{
	if (NULL != m_popUpWidget) {
		EWOL_INFO("Remove current pop-up Widget...");
		delete(m_popUpWidget);
		m_popUpWidget = NULL;
	}
	m_popUpWidget = widget;
	// Regenerate the size calculation :
	CalculateSize(m_size.x, m_size.y);
}


void ewol::Windows::PopUpWidgetPop(void)
{
	if (NULL != m_popUpWidget) {
		EWOL_INFO("Remove current pop-up Widget...");
		delete(m_popUpWidget);
		m_popUpWidget = NULL;
	}
}


bool ewol::Windows::OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * eventExternId, etkFloat_t x, etkFloat_t y)
{
	if(ewolEventWindowsHideKeyboard == generateEventId) {
		EWOL_INFO("Request Hide keyboard");
		KeyboardHide();
		return true;
	}
	return false;
}

void ewol::Windows::KeyboardShow(ewol::keyboardMode_te mode)
{
#if defined(__PLATFORM__Android)
	if (NULL == m_keyBoardwidget) {
		// Create the keyboard ...
		m_keyBoardwidget = new ewol::Keyboard();
		if (NULL == m_keyBoardwidget) {
			EWOL_ERROR("Fail to initialize memory");
		} else {
			m_keyBoardwidget->ExternLinkOnEvent("ewol event Keyboard request hide", GetWidgetId(), ewolEventWindowsHideKeyboard );
			m_keyBoardwidget->SetParrent(this);
		}
	}
	if (NULL != m_keyBoardwidget) {
		m_keyBoardwidget->Show();
	}
	CalculateSize(m_size.x, m_size.y);
	MarkToReedraw();
#endif
}


void ewol::Windows::KeyboardHide(void)
{
	EWOL_INFO("Request Hide keyboard");
	if (NULL != m_keyBoardwidget) {
		m_keyBoardwidget->Hide();
	}
	CalculateSize(m_size.x, m_size.y);
	MarkToReedraw();
}
