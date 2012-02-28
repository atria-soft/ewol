/**
 *******************************************************************************
 * @file ewol/EObject.cpp
 * @brief basic ewol object (Sources)
 * @author Edouard DUPIN
 * @date 24/02/2012
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

#include <ewol/EObject.h>
#include <ewol/EObjectManager.h>
#include <ewol/Debug.h>


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
	m_messageList.Clear();
}


void ewol::EObjectMessageMultiCast::UnInit(void)
{
	EWOL_INFO("EObject message Multi-Cast");
	m_messageList.Clear();
}


static void MultiCastAdd(ewol::EObject* object, const char* const message)
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


static void MultiCastRm(ewol::EObject* object)
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

static void MultiCastSend(ewol::EObject* object, const char* const message, etk::UString& data)
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

void ewol::EObjectMessageMultiCast::AnonymousSend(const char* const messageId, etk::UString& data)
{
	MultiCastSend(NULL, messageId, data);
}

#undef __class__
#define __class__	"ewol::EObject"



/**
 * @brief Constructor
 */
ewol::EObject::EObject(void)
{
	static int32_t ss_globalUniqueId = 0;
	// note this is nearly atomic ... (but it is enough)
	m_uniqueId = ss_globalUniqueId++;
	EWOL_DEBUG("new EObject : [" << m_uniqueId << "]");
	ewol::EObjectManager::Add(this);
}

/**
 * @brief Destructor
 */
ewol::EObject::~EObject(void)
{
	ewol::EObjectManager::Rm(this);
	MultiCastRm(this);
	EWOL_DEBUG("delete EObject : [" << m_uniqueId << "]");
	for (int32_t iii=0; iii<m_externEvent.Size(); iii++) {
		if (NULL!=m_externEvent[iii]) {
			delete(m_externEvent[iii]);
			m_externEvent[iii] = NULL;
		}
	}
	m_externEvent.Clear();
	m_availlableEventId.Clear();
	m_uniqueId = -1;
}


/**
 * @brief Get the UniqueId of the EObject
 * @param ---
 * @return the requested ID
 */
int32_t ewol::EObject::GetId(void)
{
	return m_uniqueId;
};


/**
 * @brief Add a specific event Id in the list to prevent wrong link on a EObject
 * @param[in] generateEventId event Id to add
 * @return ---
 */
void ewol::EObject::AddEventId(const char * generateEventId)
{
	if (NULL != generateEventId) {
		m_availlableEventId.PushBack(generateEventId);
	}
}


/**
 * @brief Generate event on all registered EObject
 * @param[in] generateEventId event Id that is curetly generated
 * @return ---
 */
void ewol::EObject::GenerateEventId(const char * generateEventId)
{
	// for every element registered ...
	for (int32_t iii=0; iii<m_externEvent.Size(); iii++) {
		if (NULL!=m_externEvent[iii]) {
			// if we find the event ...
			if (m_externEvent[iii]->localEventId == generateEventId) {
				if (NULL != m_externEvent[iii]->destEObject) {
					m_externEvent[iii]->destEObject->OnReceiveMessage(this, m_externEvent[iii]->destEventId, m_externEvent[iii]->destData);
				}
			}
		}
	}
}

/**
 * @brief Generate Multicast event on all EObject requested the event
 * @param[in] messageId Event Id that is generated
 * @param[in] data Interger which is transform in etk::UString
 * @return ---
 */
void ewol::EObject::SendMultiCast(const char* const messageId, int32_t data)
{
	etk::UString tmpData(data);
	MultiCastSend(this, messageId, tmpData);
}

/**
 * @brief Generate Multicast event on all EObject requested the event
 * @param[in] messageId Event Id that is generated
 * @param[in] data String that is send at all the destinations
 * @return ---
 */
void ewol::EObject::SendMultiCast(const char* const messageId, etk::UString& data)
{
	MultiCastSend(this, messageId, data);
}

/**
 * @brief Register of the arrival of a Multicast message
 * @param[in] messageId Event Id waiting for...
 * @return ---
 */
void ewol::EObject::RegisterMultiCast(const char* const messageId)
{
	MultiCastAdd(this, messageId);
}

/**
 * @brief Register an EObject over an other to get event on the second...
 * @param[in] destinationObject pointer on the object that might be call when an event is generated
 * @param[in] eventId Event generate inside the object
 * @param[in] eventIdgenerated event generated when call the distant EObject.OnReceiveMessage(...)
 * @param[in] data data associated with the event
 * @return true if register corectly done
 */
void ewol::EObject::RegisterOnEvent(ewol::EObject * destinationObject, const char * eventId, const char * eventIdgenerated, etk::UString data)
{
	if (NULL == destinationObject) {
		EWOL_ERROR("Input ERROR NULL pointer EObject ...");
		return;
	}
	if (NULL == eventId) {
		EWOL_ERROR("Input ERROR NULL pointer Event Id...");
		return;
	}
	if (NULL == eventIdgenerated) {
		EWOL_ERROR("Input ERROR NULL pointer destination Event Id...");
		return;
	}
	ewol::EventExtGen * tmpEvent = new ewol::EventExtGen();
	if (NULL == tmpEvent) {
		EWOL_ERROR("Allocation error in Register Event...");
		return;
	}
	tmpEvent->localEventId = eventId;
	tmpEvent->destEObject = destinationObject;
	tmpEvent->destEventId = eventIdgenerated;
	tmpEvent->destData = data;
	m_externEvent.PushBack(tmpEvent);
}


/**
 * @brief Inform object that an other object is removed ...
 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
 * @return ---
 */
void ewol::EObject::OnObjectRemove(ewol::EObject * removeObject)
{
	int32_t iii = m_externEvent.Size()-1;
	while(iii>=0) {
		if (NULL==m_externEvent[iii]) {
			m_externEvent.Erase(iii);
		} else if (m_externEvent[iii]->destEObject == removeObject) {
			m_externEvent.Erase(iii);
		} else {
			iii--;
		}
	}
}


/**
 * @brief Receive a message from an other EObject with a specific eventId and data
 * @param[in] CallerObject Pointer on the EObject that information came from
 * @param[in] eventId Message registered by this class
 * @param[in] data Data registered by this class
 * @return ---
 */
void ewol::EObject::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
{
	// here nothing to do ...
}


