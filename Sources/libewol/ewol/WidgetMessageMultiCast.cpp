/**
 *******************************************************************************
 * @file ewol/WidgetMessageMulticast.cpp
 * @brief basic ewol Widget Message Multi-cast (Sources)
 * @author Edouard DUPIN
 * @date 31/01/2012
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

#include <ewol/WidgetMessageMultiCast.h>
#include <ewol/WidgetManager.h>

#undef __class__
#define __class__	"ewol::WidgetMessageMultiCast"

extern "C" {
	typedef struct {
		int32_t     widgetId;
		const char* message;
	} messageList_ts;
};


// internal element of the widget manager : 
static etk::VectorType<messageList_ts>   m_messageList;   // all widget allocated ==> all time increment ... never removed ...


void ewol::widgetMessageMultiCast::Init(void)
{
	EWOL_INFO("user widget message Multi-Cast");
}

void ewol::widgetMessageMultiCast::UnInit(void)
{
	EWOL_INFO("user widget message Multi-Cast");
	m_messageList.Clear();
}

void ewol::widgetMessageMultiCast::Add(int32_t widgetId, const char* const message)
{
	// TODO : Check if the message exist before ...
	messageList_ts tmpMessage;
	tmpMessage.widgetId = widgetId;
	tmpMessage.message = message;
	m_messageList.PushBack(tmpMessage);
	EWOL_DEBUG("SendMulticast ADD listener :" << widgetId << " on \"" << message << "\"" );
}

// TODO : Do this better ...
void ewol::widgetMessageMultiCast::Rm(int32_t widgetId)
{
	// send the message at all registered widget ...
	for (int32_t iii=0; iii<m_messageList.Size(); iii++) {
		if(m_messageList[iii].widgetId == widgetId) {
			EWOL_DEBUG("SendMulticast RM listener :" << widgetId);
			m_messageList[iii].message = NULL;
			m_messageList[iii].widgetId = -1;
		}
	}
}

void ewol::widgetMessageMultiCast::Send(int32_t widgetId, const char* const message, int32_t data)
{
	char tmpData[50];
	sprintf(tmpData, "%d", data);
	Send(widgetId, message, tmpData);
}

void ewol::widgetMessageMultiCast::Send(int32_t widgetId, const char* const message, const char * data)
{
	if (data!=NULL) {
		EWOL_DEBUG("SendMulticast message \"" << message << "\" data=\"" << data << "\" to :");
	} else {
		EWOL_DEBUG("SendMulticast message \"" << message << "\" data=NULL to :");
	}
	// send the message at all registered widget ...
	for (int32_t iii=0; iii<m_messageList.Size(); iii++) {
		if(    m_messageList[iii].message == message
		    && m_messageList[iii].widgetId != widgetId)
		{
			ewol::Widget * tmpWidget = ewol::widgetManager::Get(m_messageList[iii].widgetId);
			if (NULL != tmpWidget) {
				EWOL_DEBUG("        id = " << m_messageList[iii].widgetId);
				// generate event ...
				(void)tmpWidget->OnEventAreaExternal(widgetId, m_messageList[iii].message, data, 0, 0);
			}
		}
	}
}

void ewol::widgetMessageMultiCast::Send(int32_t widgetId, const char* const message, etk::UString& data)
{
	Send(widgetId, message, data.Utf8Data());
}

