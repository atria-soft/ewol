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

void ewol::widgetManager::Init(void)
{
	
}

void ewol::widgetManager::UnInit(void)
{
	for(int32_t iii=0; iii<m_widgetList.Size(); iii++) {
		ewol::widgetManager::Rm(iii);
	}
	m_widgetList.Clear();
}

void ewol::widgetManager::Add(ewol::Widget * newWidget)
{
	// Check existance
	int32_t tmpID = ewol::widgetManager::GetId(newWidget);
	if (-1 == tmpID) {
		m_widgetList.PushBack(newWidget);
	}
}

void ewol::widgetManager::Rm(ewol::Widget * newWidget)
{
	// check existance
	int32_t tmpID = ewol::widgetManager::GetId(newWidget);
	if (-1 == tmpID) {
		ewol::widgetManager::Rm(tmpID);
	} else {
		EWOL_ERROR("Widget already removed ...");
	}
}

void ewol::widgetManager::Rm(int32_t widgetId)
{
	if (0 <= widgetId && widgetId < m_widgetList.Size()) {
		if (m_widgetList[widgetId]!=NULL) {
			delete(m_widgetList[widgetId]);
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

ewol::Widget * Get(int32_t widgetId)
{
	if (0 <= widgetId && widgetId < m_widgetList.Size()) {
		return m_widgetList[widgetId];
	}
	return NULL;
}
