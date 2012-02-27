/**
 *******************************************************************************
 * @file ewol/WidgetMessageMultiCast.cpp
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

#include <ewol/EObjectMessageMulticast.h>
#include <ewol/EObjectManager.h>

#undef __class__
#define __class__	"ewol::EObjectMessageMultiCast"

extern "C" {
	typedef struct {
		const char*    message;
		ewol::EObject* object;
	} messageList_ts;
};


// internal element of the widget manager : 
static etk::VectorType<messageList_ts>   m_messageList;   // all widget allocated ==> all time increment ... never removed ...


void ewol::EObjectMessageMultiCast::Init(void)
{
	EWOL_INFO("EObject message Multi-Cast");
}


void ewol::EObjectMessageMultiCast::UnInit(void)
{
	EWOL_INFO("EObject message Multi-Cast");
	m_messageList.Clear();
}


void ewol::EObjectMessageMultiCast::Add(ewol::EObject* object, const char* const message)
{
	if (NULL == object) {
		EWOL_ERROR("Add with NULL object");
		return;
	}
	if (NULL == message) {
		EWOL_ERROR("Add with NULL Message");
		return;
	}
	messageList_ts tmpMessage;
	tmpMessage.object = object;
	tmpMessage.message = message;
	m_messageList.PushBack(tmpMessage);
	EWOL_DEBUG("SendMulticast ADD listener :" << object->GetId() << " on \"" << message << "\"" );
}


void ewol::EObjectMessageMultiCast::Rm(ewol::EObject* object)
{
	if (NULL == object) {
		EWOL_ERROR("Rm with NULL object");
		return;
	}
	// send the message at all registered widget ...
	for (int32_t iii=m_messageList.Size(); iii>=0; iii--) {
		if(m_messageList[iii].object == object) {
			EWOL_DEBUG("SendMulticast RM listener :" << object->GetId());
			m_messageList[iii].message = NULL;
			m_messageList[iii].object = NULL;
			m_messageList.Erase(iii);
		}
	}
}

void ewol::EObjectMessageMultiCast::Send(ewol::EObject* object, const char* const message, int32_t data)
{
	etk::UString tmpData(data);
	Send(object, message, tmpData);
}



void ewol::EObjectMessageMultiCast::Send(ewol::EObject* object, const char* const message, etk::UString& data)
{
	EWOL_DEBUG("SendMulticast message \"" << message << "\" data=\"" << data << "\" to :");
	
	// send the message at all registered widget ...
	for (int32_t iii=0; iii<m_messageList.Size(); iii++) {
		if(    m_messageList[iii].message == message
		    && m_messageList[iii].object != object)
		{
			if (NULL != m_messageList[iii].object) {
				EWOL_DEBUG("        id = " << m_messageList[iii].object->GetId());
				// generate event ...
				m_messageList[iii].object->OnReceiveMessage(object, m_messageList[iii].message, data);
			}
		}
	}
}

