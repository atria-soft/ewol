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
#include <ewol/WidgetManager.h>



#undef __class__
#define __class__	"ewol::Windows"


//list of local events : 
extern const char * const ewolEventWindowsHideKeyboard   = "ewol Windows hideKeyboard";


ewol::Windows::Windows(void)
{
	SetCanHaveFocus(true);
	for(int32_t iii=0; iii<NB_BOUBLE_BUFFER; iii++) {
		m_subWidget[iii] = NULL;
	}
	m_keyBoardwidget = NULL;
	SetDecorationDisable();
	//KeyboardShow(KEYBOARD_MODE_CODE);
}

ewol::Windows::~Windows(void)
{
	if (NULL != m_subWidget[m_currentCreateId]) {
		m_subWidget[m_currentCreateId]->MarkToRemove();
		m_subWidget[m_currentCreateId]=NULL;
	}
	
	for(int32_t iii=0; iii<m_popUpWidgetList[m_currentCreateId].Size(); iii++) {
		if (NULL != m_popUpWidgetList[m_currentCreateId][iii]) {
			m_popUpWidgetList[m_currentCreateId][iii]->MarkToRemove();
			m_popUpWidgetList[m_currentCreateId][iii]=NULL;
		}
	}
	m_popUpWidgetList[m_currentCreateId].Clear();
	
	if (NULL != m_keyBoardwidget) {
		m_keyBoardwidget->MarkToRemove();
		m_keyBoardwidget=NULL;
	}
}

bool ewol::Windows::CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY)
{
	//EWOL_DEBUG("calculateMinSize on : " << m_currentCreateId);
	m_size.x = availlableX;
	m_size.y = availlableY;
	int32_t keyboardHigh = 0;
	if (NULL != m_keyBoardwidget && false == m_keyBoardwidget->IsHide() ) {
		m_keyBoardwidget->CalculateMinSize();
		coord2D_ts tmpSize = m_keyBoardwidget->GetMinSize();
		keyboardHigh = (int32_t)tmpSize.y;
		m_keyBoardwidget->SetOrigin(0, m_size.y - keyboardHigh);
		m_keyBoardwidget->CalculateSize(m_size.x, keyboardHigh);
	}
	if (NULL != m_subWidget[m_currentCreateId]) {
		m_subWidget[m_currentCreateId]->CalculateMinSize();
		// TODO : Check if min Size is possible ...
		// TODO : Herited from MinSize .. and expand ???
		m_subWidget[m_currentCreateId]->CalculateSize(m_size.x, m_size.y - keyboardHigh);
	}
	for(int32_t iii=0; iii<m_popUpWidgetList[m_currentCreateId].Size(); iii++) {
		if (NULL != m_popUpWidgetList[m_currentCreateId][iii]) {
			m_popUpWidgetList[m_currentCreateId][iii]->CalculateMinSize();
			m_popUpWidgetList[m_currentCreateId][iii]->CalculateSize(m_size.x, m_size.y - keyboardHigh);
		}
	}
	return true;
}


bool ewol::Windows::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, eventPosition_ts pos)
{
	if (NULL != m_keyBoardwidget && false == m_keyBoardwidget->IsHide() ) {
		coord2D_ts tmpSize = m_keyBoardwidget->GetMinSize();
		if (pos.local.y > m_size.y - tmpSize.y) {
			m_keyBoardwidget->GenEventInput(IdInput, typeEvent, pos.abs);
			return true;
		}
	}
	// event go directly on the pop-up
	if (0 < m_popUpWidgetList[m_currentCreateId].Size()) {
		if (NULL == m_popUpWidgetList[m_currentCreateId][m_popUpWidgetList[m_currentCreateId].Size()-1]) {
			m_popUpWidgetList[m_currentCreateId].PopBack();
		} else {
			m_popUpWidgetList[m_currentCreateId][m_popUpWidgetList[m_currentCreateId].Size()-1]->GenEventInput(IdInput, typeEvent, pos.abs);
		}
	// otherwise in the normal windows
	} else if (NULL != m_subWidget[m_currentCreateId]) {
		m_subWidget[m_currentCreateId]->GenEventInput(IdInput, typeEvent, pos.abs);
	}
	return true;
}

void ewol::Windows::SysDraw(void)
{

	//EWOL_DEBUG("Drow on (" << m_size.x << "," << m_size.y << ")");
	// set the size of the open GL system
	glViewport(0,0,m_size.x,m_size.y);
/*
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthoEwol(-m_size.x/2, m_size.x/2, m_size.y/2, -m_size.y/2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-m_size.x/2, -m_size.y/2, -1.0);
	
#else
	glOrtho(0., m_size.x, 0., -m_size.y, 1., 20.);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, -m_size.y, -5);
#endif
*/
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GenDraw();

	glDisable(GL_BLEND);
	return;
}

void ewol::Windows::OnRegenerateDisplay(void)
{
	if (NULL != m_subWidget[m_currentCreateId]) {
		m_subWidget[m_currentCreateId]->OnRegenerateDisplay();
	}
	for(int32_t iii=0; iii<m_popUpWidgetList[m_currentCreateId].Size(); iii++) {
		if (NULL != m_popUpWidgetList[m_currentCreateId][iii]) {
			m_popUpWidgetList[m_currentCreateId][iii]->OnRegenerateDisplay();
		}
	}
	if (NULL != m_keyBoardwidget && false == m_keyBoardwidget->IsHide() ) {
		m_keyBoardwidget->OnRegenerateDisplay();
	}
}


bool ewol::Windows::OnDraw(void)
{

	// Clear the screen with transparency ...
	glClearColor(0.750, 0.750, 0.750, 0.5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//EWOL_WARNING(" WINDOWS draw on " << m_currentDrawId);
	// first display the windows on the display
	if (NULL != m_subWidget[m_currentDrawId]) {
		m_subWidget[m_currentDrawId]->GenDraw();
		//EWOL_DEBUG("Draw Windows");
	}
	// second display the pop-up
	for(int32_t iii=0; iii<m_popUpWidgetList[m_currentDrawId].Size(); iii++) {
		if (NULL != m_popUpWidgetList[m_currentDrawId][iii]) {
			m_popUpWidgetList[m_currentDrawId][iii]->GenDraw();
			//EWOL_DEBUG("Draw Pop-up");
		}
	}
	if (NULL != m_keyBoardwidget && false == m_keyBoardwidget->IsHide() ) {
		m_keyBoardwidget->GenDraw();
		//EWOL_DEBUG("Draw kewboard");
	}
	return true;
}



void ewol::Windows::SetSubWidget(ewol::Widget * widget)
{
	if (NULL != m_subWidget[m_currentCreateId]) {
		EWOL_INFO("Remove current main windows Widget...");
		m_subWidget[m_currentCreateId]->MarkToRemove();
		m_subWidget[m_currentCreateId] = NULL;
	}
	m_subWidget[m_currentCreateId] = widget;
	// Regenerate the size calculation :
	CalculateSize(m_size.x, m_size.y);
	m_needFlipFlop = true;
}


void ewol::Windows::PopUpWidgetPush(ewol::Widget * widget)
{
	m_popUpWidgetList[m_currentCreateId].PushBack(widget);
	// Regenerate the size calculation :
	CalculateSize(m_size.x, m_size.y);
	m_needFlipFlop = true;
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
#endif
}


void ewol::Windows::KeyboardHide(void)
{
	EWOL_INFO("Request Hide keyboard");
	if (NULL != m_keyBoardwidget) {
		m_keyBoardwidget->Hide();
	}
	CalculateSize(m_size.x, m_size.y);
}



void ewol::Windows::OnFlipFlopEvent(void)
{
	bool needFlipFlop = m_needFlipFlop;
	// call herited classes
	ewol::Widget::OnFlipFlopEvent();
	// internal saving
	if (true == needFlipFlop) {
		m_subWidget[m_currentCreateId]       = m_subWidget[m_currentDrawId];
		m_popUpWidgetList[m_currentCreateId] = m_popUpWidgetList[m_currentDrawId];
	}
	// in every case, we propagate the flip-flop EVENT
	if (NULL != m_subWidget[m_currentDrawId]) {
		m_subWidget[m_currentDrawId]->OnFlipFlopEvent();
	}
	for(int32_t iii=0; iii<m_popUpWidgetList[m_currentDrawId].Size(); iii++) {
		if(NULL != m_popUpWidgetList[m_currentDrawId][iii]) {
			m_popUpWidgetList[m_currentDrawId][iii]->OnFlipFlopEvent();
		}
	}
}

/**
 * @brief Inform object that an other object is removed ...
 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
 * @note : Sub classes must call this class
 * @return ---
 */
void ewol::Windows::OnObjectRemove(ewol::EObject * removeObject)
{
	// First step call parrent : 
	ewol::Widget::OnObjectRemove(removeObject);
	// second strep find if in alll the elements ...
	
	if (m_subWidget[m_currentCreateId] == removeObject) {
		EWOL_DEBUG("Remove main element of the windows ==> destroyed object");
		m_subWidget[m_currentCreateId] = NULL;
	}
	for(int32_t iii=m_popUpWidgetList[m_currentCreateId].Size(); iii>=0; iii--) {
		if(m_popUpWidgetList[m_currentCreateId][iii] == removeObject) {
			EWOL_DEBUG("Remove Pop-up [" << iii << "] element of the windows ==> destroyed object");
			m_popUpWidgetList[m_currentCreateId][iii] = NULL;
			m_popUpWidgetList[m_currentCreateId].Erase(iii);
		}
	}
	if (m_keyBoardwidget == removeObject) {
		EWOL_DEBUG("Remove Keyboard element of the windows ==> destroyed object");
		m_keyBoardwidget = NULL;
	}
}


