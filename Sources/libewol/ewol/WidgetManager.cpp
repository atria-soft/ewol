/**
 *******************************************************************************
 * @file ewol/WidgetManager.cpp
 * @brief basic ewol Widget Manager (Sources)
 * @author Edouard DUPIN
 * @date 13/11/2011
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

#include <ewol/WidgetManager.h>
#include <ewol/widget/Joystick.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/ButtonColor.h>
//#include <ewol/widget/Scene.h>
#include <vector>

#undef __class__
#define __class__	"WidgetManager"

static pthread_mutex_t localMutex;
static bool IsInit = false;

// For the focus Management
static ewol::Widget * m_focusWidgetDefault = NULL;
static ewol::Widget * m_focusWidgetCurrent = NULL;
static std::vector<ewol::Widget*> l_listOfPeriodicWidget;
static bool                       l_havePeriodic = false;

void ewol::widgetManager::Init(void)
{
	EWOL_DEBUG("==> Init Widget-Manager");
	// create interface mutex :
	int ret = pthread_mutex_init(&localMutex, NULL);
	EWOL_ASSERT(ret == 0, "Error creating Mutex ...");
	// prevent android error ==> can create memory leak but I prefer
	m_focusWidgetDefault = NULL;
	m_focusWidgetCurrent = NULL;
	l_listOfPeriodicWidget.clear();
	l_havePeriodic = false;
	// init all the widget global parameters :
	ewol::WIDGET_JoystickInit();
	ewol::WIDGET_ButtonInit();
	ewol::WIDGET_ButtonColorInit();
	//ewol::WIDGET_SceneInit();
	IsInit = true;
}

void ewol::widgetManager::UnInit(void)
{
	EWOL_DEBUG("==> Un-Init Widget-Manager");
	EWOL_INFO("Realease all FOCUS");
	ewol::widgetManager::FocusSetDefault(NULL);
	ewol::widgetManager::FocusRelease();
	
	IsInit = false;
	
	l_listOfPeriodicWidget.clear();
	int ret = pthread_mutex_destroy(&localMutex);
	EWOL_ASSERT(ret == 0, "Error destroying Mutex ...");
}

void ewol::widgetManager::Rm(ewol::Widget * newWidget)
{
	PeriodicCallRm(newWidget);
	FocusRemoveIfRemove(newWidget);
}

/* *************************************************************************
 * Focus Area : 
 * *************************************************************************/

void ewol::widgetManager::FocusKeep(ewol::Widget * newWidget)
{
	if (NULL == newWidget) {
		// nothing to do ...
		return;
	}
	if (false == newWidget->CanHaveFocus()) {
		EWOL_VERBOSE("Widget can not have Focus, id=" << ewol::widgetManager::Get(newWidget));
		return;
	}
	if (newWidget == m_focusWidgetCurrent) {
		// nothing to do ...
		return;
	}
	if (NULL != m_focusWidgetCurrent) {
		EWOL_DEBUG("Rm Focus on WidgetID=" << m_focusWidgetCurrent->GetId() );
		m_focusWidgetCurrent->RmFocus();
	}
	m_focusWidgetCurrent = newWidget;
	if (NULL != m_focusWidgetCurrent) {
		EWOL_DEBUG("Set Focus on WidgetID=" << m_focusWidgetCurrent->GetId() );
		m_focusWidgetCurrent->SetFocus();
	}
}


void ewol::widgetManager::FocusSetDefault(ewol::Widget * newWidget)
{
	if (NULL != newWidget && false == newWidget->CanHaveFocus()) {
		EWOL_VERBOSE("Widget can not have Focus, id=" << newWidget->GetId() );
		return;
	}
	if (m_focusWidgetDefault == m_focusWidgetCurrent) {
		if (NULL != m_focusWidgetCurrent) {
			EWOL_DEBUG("Rm Focus on WidgetID=" << m_focusWidgetCurrent->GetId() );
			m_focusWidgetCurrent->RmFocus();
		}
		m_focusWidgetCurrent = newWidget;
		if (NULL != m_focusWidgetCurrent) {
			EWOL_DEBUG("Set Focus on WidgetID=" << m_focusWidgetCurrent->GetId() );
			m_focusWidgetCurrent->SetFocus();
		}
	}
	m_focusWidgetDefault = newWidget;
}


void ewol::widgetManager::FocusRelease(void)
{
	if (m_focusWidgetDefault == m_focusWidgetCurrent) {
		// nothink to do ...
		return;
	}
	if (NULL != m_focusWidgetCurrent) {
		EWOL_DEBUG("Rm Focus on WidgetID=" << m_focusWidgetCurrent->GetId() );
		m_focusWidgetCurrent->RmFocus();
	}
	m_focusWidgetCurrent = m_focusWidgetDefault;
	if (NULL != m_focusWidgetCurrent) {
		EWOL_DEBUG("Set Focus on WidgetID=" << m_focusWidgetCurrent->GetId() );
		m_focusWidgetCurrent->SetFocus();
	}
}


ewol::Widget * ewol::widgetManager::FocusGet(void)
{
	return m_focusWidgetCurrent;
}

void ewol::widgetManager::FocusRemoveIfRemove(ewol::Widget * newWidget)
{
	if (m_focusWidgetCurrent == newWidget) {
		EWOL_WARNING("Release Focus when remove widget");
		FocusRelease();
	}
	if (m_focusWidgetDefault == newWidget) {
		EWOL_WARNING("Release default Focus when remove widget");
		FocusSetDefault(NULL);
	}
}



void ewol::widgetManager::PeriodicCallAdd(ewol::Widget * pWidget)
{
	for (int32_t iii=0; iii < l_listOfPeriodicWidget.size(); iii++) {
		if (l_listOfPeriodicWidget[iii] == pWidget) {
			return;
		}
	}
	for (int32_t iii=0; iii < l_listOfPeriodicWidget.size(); iii++) {
		if (NULL == l_listOfPeriodicWidget[iii]) {
			l_listOfPeriodicWidget[iii] = pWidget;
			return;
		}
	}
	l_listOfPeriodicWidget.push_back(pWidget);
	l_havePeriodic = true;
}

void ewol::widgetManager::PeriodicCallRm(ewol::Widget * pWidget)
{
	int32_t nbElement = 0;
	for (int32_t iii=0; iii < l_listOfPeriodicWidget.size(); iii++) {
		if (l_listOfPeriodicWidget[iii] == pWidget) {
			l_listOfPeriodicWidget[iii] = NULL;
		} else {
			nbElement++;
		}
	}
	if (0 == nbElement) {
		l_havePeriodic = false;
	}
}

void ewol::widgetManager::PeriodicCall(int64_t localTime)
{
	for (int32_t iii=0; iii < l_listOfPeriodicWidget.size(); iii++) {
		if (NULL != l_listOfPeriodicWidget[iii]) {
			l_listOfPeriodicWidget[iii]->PeriodicCall(localTime);
		}
	}
}

bool ewol::widgetManager::PeriodicCallHave(void)
{
	return l_havePeriodic;
}


static bool needRedraw = true;

void ewol::widgetManager::DoubleBufferLock(void)
{
	if (IsInit) {
		//EWOL_DEBUG("DoubleBuffer-Lock");
		pthread_mutex_lock(&localMutex);
		//EWOL_DEBUG("DoubleBuffer-Lock (DONE)");
	}
}

void ewol::widgetManager::SetDoubleBufferNeedDraw(void)
{
	needRedraw = true;
}

bool ewol::widgetManager::GetDoubleBufferNeedDraw(void)
{
	if (true == needRedraw) {
		needRedraw = false;
		return true;
	}
	return false;
}

void ewol::widgetManager::DoubleBufferUnLock(void)
{
	if (IsInit) {
		//EWOL_DEBUG("DoubleBuffer-UnLock");
		pthread_mutex_unlock(&localMutex);
		//EWOL_DEBUG("DoubleBuffer-UnLock (DONE)");
	}
}


