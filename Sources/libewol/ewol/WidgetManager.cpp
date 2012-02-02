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

#undef __class__
#define __class__	"ewol::WidgetManager"

extern "C" {
	typedef struct {
		int32_t       widgetId;
		ewol::Widget* widgetPointer;
	} widgetList_ts;
};

static pthread_mutex_t localMutex;


// internal element of the widget manager : 
static etk::VectorType<widgetList_ts>   m_widgetList;   // all widget allocated ==> all time increment ... never removed ...
// For the focus Management
static ewol::Widget * m_focusWidgetDefault = NULL;
static ewol::Widget * m_focusWidgetCurrent = NULL;

void ewol::widgetManager::Init(void)
{
	EWOL_DEBUG("Init Widget manager");
	// create interface mutex :
	int ret = pthread_mutex_init(&localMutex, NULL);
	EWOL_ASSERT(ret == 0, "Error creating Mutex ...");
}

void ewol::widgetManager::UnInit(void)
{
	EWOL_DEBUG("Un-Init Widget manager");
	EWOL_INFO("Realease all FOCUS");
	ewol::widgetManager::FocusSetDefault(NULL);
	ewol::widgetManager::FocusRelease();
	
	EWOL_INFO(" Remove missing user widget");
	for(int32_t iii=0; iii<m_widgetList.Size(); iii++) {
		if (m_widgetList[iii].widgetPointer!=NULL) {
			EWOL_WARNING("Un-Removed widget ... id=" << iii);
			FocusRemoveIfRemove(m_widgetList[iii].widgetPointer);
			delete(m_widgetList[iii].widgetPointer);
			m_widgetList[iii].widgetPointer=NULL;
		}
	}
	m_widgetList.Clear();
	int ret = pthread_mutex_destroy(&localMutex);
	EWOL_ASSERT(ret == 0, "Error destroying Mutex ...");
}

void ewol::widgetManager::Add(ewol::Widget * newWidget)
{
	static int32_t UniqueWidgetId = 0;
	// Check existance
	int32_t tmpID = ewol::widgetManager::Get(newWidget);
	if (0 > tmpID) {
		widgetList_ts tmpElement;
		tmpElement.widgetId = UniqueWidgetId++;
		tmpElement.widgetPointer = newWidget;
		m_widgetList.PushBack(tmpElement);
	} else {
		EWOL_WARNING("Widget Already added to the widget manager, id=" << tmpID);
	}
}

void ewol::widgetManager::Rm(ewol::Widget * newWidget)
{
	if (NULL == newWidget) {
		EWOL_ERROR("Try to remove (NULL) Widget");
		return;
	}
	for (int32_t iii=0; iii<m_widgetList.Size(); iii++) {
		if (m_widgetList[iii].widgetPointer == newWidget) {
			FocusRemoveIfRemove(newWidget);
			// Remove Element
			m_widgetList.Erase(iii);
			return;
		}
	}
	EWOL_ERROR("Widget already removed ...");
}

int32_t ewol::widgetManager::Get(ewol::Widget * newWidget)
{
	if (NULL == newWidget) {
		return -1;
	}
	for(int32_t iii=0; iii<m_widgetList.Size(); iii++) {
		if (m_widgetList[iii].widgetPointer == newWidget) {
			return m_widgetList[iii].widgetId;
		}
	}
	return -1;
}

ewol::Widget * ewol::widgetManager::Get(int32_t widgetId)
{
	if (0 > widgetId) {
		return NULL;
	}
	for(int32_t iii=0; iii<m_widgetList.Size(); iii++) {
		if (m_widgetList[iii].widgetId == widgetId) {
			return m_widgetList[iii].widgetPointer;
		}
	}
	return NULL;
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
		EWOL_DEBUG("Rm Focus on WidgetID=" << ewol::widgetManager::Get(m_focusWidgetCurrent));
		m_focusWidgetCurrent->RmFocus();
	}
	m_focusWidgetCurrent = newWidget;
	if (NULL != m_focusWidgetCurrent) {
		EWOL_DEBUG("Set Focus on WidgetID=" << ewol::widgetManager::Get(m_focusWidgetCurrent));
		m_focusWidgetCurrent->SetFocus();
	}
}


void ewol::widgetManager::FocusSetDefault(ewol::Widget * newWidget)
{
	if (NULL != newWidget && false == newWidget->CanHaveFocus()) {
		EWOL_VERBOSE("Widget can not have Focus, id=" << ewol::widgetManager::Get(newWidget));
		return;
	}
	if (m_focusWidgetDefault == m_focusWidgetCurrent) {
		if (NULL != m_focusWidgetCurrent) {
			EWOL_DEBUG("Rm Focus on WidgetID=" << ewol::widgetManager::Get(m_focusWidgetCurrent));
			m_focusWidgetCurrent->RmFocus();
		}
		m_focusWidgetCurrent = newWidget;
		if (NULL != m_focusWidgetCurrent) {
			EWOL_DEBUG("Set Focus on WidgetID=" << ewol::widgetManager::Get(m_focusWidgetCurrent));
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
		EWOL_DEBUG("Rm Focus on WidgetID=" << ewol::widgetManager::Get(m_focusWidgetCurrent));
		m_focusWidgetCurrent->RmFocus();
	}
	m_focusWidgetCurrent = m_focusWidgetDefault;
	if (NULL != m_focusWidgetCurrent) {
		EWOL_DEBUG("Set Focus on WidgetID=" << ewol::widgetManager::Get(m_focusWidgetCurrent));
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


static bool needRedraw = true;

void ewol::widgetManager::GetDoubleBufferFlipFlop(void)
{
	pthread_mutex_lock(&localMutex);
	// flip/Flop all the widget registered :
	for(int32_t iii=0; iii<m_widgetList.Size(); iii++) {
		if (NULL != m_widgetList[iii].widgetPointer) {
			m_widgetList[iii].widgetPointer->DoubleBufferFlipFlop();
		}
	}
	needRedraw = true;
	pthread_mutex_unlock(&localMutex);
}

void ewol::widgetManager::GetDoubleBufferStartDraw(void)
{
	pthread_mutex_lock(&localMutex);
}

bool ewol::widgetManager::GetDoubleBufferNeedDraw(void)
{
	if (true == needRedraw) {
		needRedraw = false;
		return true;
	}
	return false;
}

void ewol::widgetManager::GetDoubleBufferStopDraw(void)
{
	pthread_mutex_unlock(&localMutex);
}
