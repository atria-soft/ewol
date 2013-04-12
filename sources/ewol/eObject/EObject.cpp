/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/eObject/EObject.h>
#include <ewol/eObject/EObjectManager.h>
#include <ewol/debug.h>


#undef __class__
#define __class__	"EObjectMessageMultiCast"

extern "C" {
	typedef struct {
		const char*    message;
		ewol::EObject* object;
	} messageList_ts;
};

// internal element of the widget manager : 
static etk::Vector<messageList_ts>   m_messageList;   // all widget allocated ==> all time increment ... never removed ...


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
	for (int32_t iii=m_messageList.Size()-1; iii>=0; iii--) {
		if(m_messageList[iii].object == object) {
			EWOL_DEBUG("SendMulticast RM listener :" << object->GetId());
			m_messageList[iii].message = NULL;
			m_messageList[iii].object = NULL;
			m_messageList.Erase(iii);
		}
	}
}

static void MultiCastSend(ewol::EObject* object, const char* const message, const etk::UString& data)
{
	EWOL_VERBOSE("SendMulticast message \"" << message << "\" data=\"" << data << "\" to :");
	
	// send the message at all registered widget ...
	for (int32_t iii=0; iii<m_messageList.Size(); iii++) {
		if(    m_messageList[iii].message == message
		    && m_messageList[iii].object != object)
		{
			if (NULL != m_messageList[iii].object) {
				EWOL_VERBOSE("        id = " << m_messageList[iii].object->GetId() << " type=" << m_messageList[iii].object->GetObjectType());
				// generate event ...
				m_messageList[iii].object->OnReceiveMessage(object, m_messageList[iii].message, data);
			}
		}
	}
}

void ewol::EObjectMessageMultiCast::AnonymousSend(const char* const messageId, const etk::UString& data)
{
	MultiCastSend(NULL, messageId, data);
}

#undef __class__
#define __class__	"ewol::EObject"



ewol::EObject::EObject(void)
{
	static int32_t ss_globalUniqueId = 0;
	// note this is nearly atomic ... (but it is enough)
	m_uniqueId = ss_globalUniqueId++;
	EWOL_DEBUG("new EObject : [" << m_uniqueId << "]");
	ewol::EObjectManager::Add(this);
}

ewol::EObject::~EObject(void)
{
	EWOL_DEBUG("delete EObject : [" << m_uniqueId << "]");
	ewol::EObjectManager::Rm(this);
	MultiCastRm(this);
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


void ewol::EObject::AutoDestroy(void)
{
	ewol::EObjectManager::AutoRemove(this);
}
void ewol::EObject::RemoveObject(void)
{
	ewol::EObjectManager::AutoRemove(this);
}

void ewol::EObject::AddEventId(const char * generateEventId)
{
	if (NULL != generateEventId) {
		m_availlableEventId.PushBack(generateEventId);
	}
}

void ewol::EObject::GenerateEventId(const char * generateEventId, const etk::UString& data)
{
	// for every element registered ...
	for (int32_t iii=0; iii<m_externEvent.Size(); iii++) {
		if (NULL!=m_externEvent[iii]) {
			// if we find the event ...
			if (m_externEvent[iii]->localEventId == generateEventId) {
				if (NULL != m_externEvent[iii]->destEObject) {
					m_externEvent[iii]->destEObject->OnReceiveMessage(this, m_externEvent[iii]->destEventId, data);
				}
			}
		}
	}
}

void ewol::EObject::SendMultiCast(const char* const messageId, const etk::UString& data)
{
	MultiCastSend(this, messageId, data);
}

void ewol::EObject::RegisterMultiCast(const char* const messageId)
{
	MultiCastAdd(this, messageId);
}

void ewol::EObject::RegisterOnEvent(ewol::EObject * destinationObject, const char * eventId, const char * eventIdgenerated)
{
	if (NULL == destinationObject) {
		EWOL_ERROR("Input ERROR NULL pointer EObject ...");
		return;
	}
	if (NULL == eventId) {
		EWOL_ERROR("Input ERROR NULL pointer Event Id...");
		return;
	}
	// check if event existed :
	bool findIt = false;
	for(int32_t iii=0; iii<m_availlableEventId.Size(); iii++) {
		if (m_availlableEventId[iii] == eventId) {
			findIt = true;
			break;
		}
	}
	if (false == findIt) {
		EWOL_DEBUG("Try to register with a NON direct string name");
		for(int32_t iii=0; iii<m_availlableEventId.Size(); iii++) {
			if (0 == strncmp(m_availlableEventId[iii], eventId, 1024)) {
				findIt = true;
				eventIdgenerated = m_availlableEventId[iii];
				break;
			}
		}
	}
	if (false == findIt) {
		EWOL_ERROR("Can not register event on this WidgetType=" << GetObjectType() << " event=\"" << eventId << "\" ==> unknow event");
		return;
	}
	ewol::EventExtGen * tmpEvent = new ewol::EventExtGen();
	if (NULL == tmpEvent) {
		EWOL_ERROR("Allocation error in Register Event...");
		return;
	}
	tmpEvent->localEventId = eventId;
	tmpEvent->destEObject = destinationObject;
	if (NULL != eventIdgenerated) {
		tmpEvent->destEventId = eventIdgenerated;
	} else {
		tmpEvent->destEventId = eventId;
	}
	m_externEvent.PushBack(tmpEvent);
}


void ewol::EObject::OnObjectRemove(ewol::EObject * removeObject)
{
	for(int32_t iii=m_externEvent.Size()-1; iii>=0; iii--) {
		if (NULL==m_externEvent[iii]) {
			m_externEvent.Erase(iii);
		} else if (m_externEvent[iii]->destEObject == removeObject) {
			m_externEvent.Erase(iii);
		}
	}
}


