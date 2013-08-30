/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/renderer/EObjectMessageMultiCast.h>
#include <ewol/renderer/eSystem.h>

#undef __class__
#define __class__	"EMultiCast"

ewol::EMultiCast::EMultiCast(void)
{
	EWOL_INFO("EObject message Multi-Cast");
}


ewol::EMultiCast::~EMultiCast(void)
{
	EWOL_INFO("EObject message Multi-Cast");
	m_messageList.Clear();
}


void ewol::EMultiCast::Add(ewol::EObject* _object, const char* const _message)
{
	if (NULL == _object) {
		EWOL_ERROR("Add with NULL object");
		return;
	}
	if (NULL == _message) {
		EWOL_ERROR("Add with NULL Message");
		return;
	}
	m_messageList.PushBack(MessageList(_message, _object));
	EWOL_DEBUG("SendMulticast ADD listener :" << _object->GetId() << " on \"" << _message << "\"" );
}


void ewol::EMultiCast::Rm(ewol::EObject* _object)
{
	if (NULL == _object) {
		EWOL_ERROR("Rm with NULL object");
		return;
	}
	// send the message at all registered widget ...
	for (int32_t iii=m_messageList.Size()-1; iii>=0; iii--) {
		if(m_messageList[iii].m_object == _object) {
			EWOL_DEBUG("SendMulticast RM listener :" << _object->GetId());
			m_messageList[iii].m_message = NULL;
			m_messageList[iii].m_object = NULL;
			m_messageList.Erase(iii);
		}
	}
}

void ewol::EMultiCast::Send(ewol::EObject* _object, const char* const _message, const etk::UString& _data)
{
	EWOL_VERBOSE("SendMulticast message \"" << _message << "\" data=\"" << _data << "\" to :");
	
	// send the message at all registered widget ...
	for (int32_t iii=0; iii<m_messageList.Size(); iii++) {
		if(    m_messageList[iii].m_message == _message
		    && m_messageList[iii].m_object != _object)
		{
			if (NULL != m_messageList[iii].m_object) {
				EWOL_VERBOSE("        id = " << m_messageList[iii].m_object->GetId() << " type=" << m_messageList[iii].m_object->GetObjectType());
				// generate event ... (create message before ...
				ewol::EMessage tmpMsg(_object, m_messageList[iii].m_message, _data);
				m_messageList[iii].m_object->OnReceiveMessage(tmpMsg);
			}
		}
	}
}

