/**
 *******************************************************************************
 * @file ewolWidgetManager.cpp
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

#include <ewolWidgetManager.h>

#undef __class__
#define __class__	"ewol::WidgetManager"


// internal element of the widget manager : 
static etk::VectorType<ewol::Widget*>   m_widgetList;   // all widget allocated ==> all time increment ... never removed ...
// For the focus Management
static ewol::Widget * m_focusWidgetDefault = NULL;
static ewol::Widget * m_focusWidgetCurrent = NULL;

void ewol::widgetManager::Init(void)
{
	EWOL_INFO("user widget manager");
}

void ewol::widgetManager::UnInit(void)
{
	EWOL_INFO("Realease all FOCUS");
	ewol::widgetManager::FocusSetDefault(NULL);
	ewol::widgetManager::FocusRelease();
	
	EWOL_INFO(" Remove missing user widget");
	for(int32_t iii=0; iii<m_widgetList.Size(); iii++) {
		if (m_widgetList[iii]!=NULL) {
			EWOL_WARNING("Un-Removed widget ... id=" << iii);
			delete(m_widgetList[iii]);
			m_widgetList[iii]=NULL;
		}
	}
	m_widgetList.Clear();
}

void ewol::widgetManager::Add(ewol::Widget * newWidget)
{
	// Check existance
	int32_t tmpID = ewol::widgetManager::GetId(newWidget);
	if (-1 == tmpID) {
		m_widgetList.PushBack(newWidget);
	} else {
		EWOL_WARNING("Widget Already added to the widget manager, id=" << tmpID);
	}
}

void ewol::widgetManager::Rm(ewol::Widget * newWidget)
{
	// check existance
	int32_t tmpID = ewol::widgetManager::GetId(newWidget);
	if (-1 != tmpID) {
		ewol::widgetManager::Rm(tmpID);
	} else {
		EWOL_ERROR("Widget already removed ...");
	}
}

void ewol::widgetManager::Rm(int32_t widgetId)
{
	if (0 <= widgetId && widgetId < m_widgetList.Size()) {
		if (m_widgetList[widgetId]!=NULL) {
			if (m_focusWidgetCurrent==m_widgetList[widgetId]) {
				ewol::widgetManager::FocusRelease();
			}
			if (m_focusWidgetDefault==m_widgetList[widgetId]) {
				ewol::widgetManager::FocusSetDefault(NULL);
			}
			m_widgetList[widgetId]=NULL;
		}
	}
}

int32_t ewol::widgetManager::GetId(ewol::Widget * newWidget)
{
	if (NULL == newWidget) {
		return -1;
	}
	for(int32_t iii=0; iii<m_widgetList.Size(); iii++) {
		if (m_widgetList[iii] == newWidget) {
			return iii;
		}
	}
	return -1;
}

ewol::Widget * ewol::widgetManager::Get(int32_t widgetId)
{
	if (0 <= widgetId && widgetId < m_widgetList.Size()) {
		return m_widgetList[widgetId];
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
		EWOL_VERBOSE("Widget can not have Focus, id=" << ewol::widgetManager::GetId(newWidget));
		return;
	}
	if (newWidget == m_focusWidgetCurrent) {
		// nothing to do ...
		return;
	}
	if (NULL != m_focusWidgetCurrent) {
		EWOL_DEBUG("Rm Focus on WidgetID=" << ewol::widgetManager::GetId(m_focusWidgetCurrent));
		m_focusWidgetCurrent->RmFocus();
	}
	m_focusWidgetCurrent = newWidget;
	if (NULL != m_focusWidgetCurrent) {
		EWOL_DEBUG("Set Focus on WidgetID=" << ewol::widgetManager::GetId(m_focusWidgetCurrent));
		m_focusWidgetCurrent->SetFocus();
	}
}


void ewol::widgetManager::FocusSetDefault(ewol::Widget * newWidget)
{
	if (NULL != newWidget && false == newWidget->CanHaveFocus()) {
		EWOL_VERBOSE("Widget can not have Focus, id=" << ewol::widgetManager::GetId(newWidget));
		return;
	}
	if (m_focusWidgetDefault == m_focusWidgetCurrent) {
		if (NULL != m_focusWidgetCurrent) {
			EWOL_DEBUG("Rm Focus on WidgetID=" << ewol::widgetManager::GetId(m_focusWidgetCurrent));
			m_focusWidgetCurrent->RmFocus();
		}
		m_focusWidgetCurrent = newWidget;
		if (NULL != m_focusWidgetCurrent) {
			EWOL_DEBUG("Set Focus on WidgetID=" << ewol::widgetManager::GetId(m_focusWidgetCurrent));
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
		EWOL_DEBUG("Rm Focus on WidgetID=" << ewol::widgetManager::GetId(m_focusWidgetCurrent));
		m_focusWidgetCurrent->RmFocus();
	}
	m_focusWidgetCurrent = m_focusWidgetDefault;
	if (NULL != m_focusWidgetCurrent) {
		EWOL_DEBUG("Set Focus on WidgetID=" << ewol::widgetManager::GetId(m_focusWidgetCurrent));
		m_focusWidgetCurrent->SetFocus();
	}
}


ewol::Widget * ewol::widgetManager::FocusGet(void)
{
	return m_focusWidgetCurrent;
}