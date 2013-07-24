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
static etk::Vector<messageList_ts> m_messageList; // all widget allocated ==> all time increment ... never removed ...

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


static void MultiCastAdd(ewol::EObject* _object, const char* const _message)
{
	if (NULL == _object) {
		EWOL_ERROR("Add with NULL object");
		return;
	}
	if (NULL == _message) {
		EWOL_ERROR("Add with NULL Message");
		return;
	}
	messageList_ts tmpMessage;
	tmpMessage.object = _object;
	tmpMessage.message = _message;
	m_messageList.PushBack(tmpMessage);
	EWOL_DEBUG("SendMulticast ADD listener :" << _object->GetId() << " on \"" << _message << "\"" );
}


static void MultiCastRm(ewol::EObject* _object)
{
	if (NULL == _object) {
		EWOL_ERROR("Rm with NULL object");
		return;
	}
	// send the message at all registered widget ...
	for (int32_t iii=m_messageList.Size()-1; iii>=0; iii--) {
		if(m_messageList[iii].object == _object) {
			EWOL_DEBUG("SendMulticast RM listener :" << _object->GetId());
			m_messageList[iii].message = NULL;
			m_messageList[iii].object = NULL;
			m_messageList.Erase(iii);
		}
	}
}

static void MultiCastSend(ewol::EObject* _object, const char* const _message, const etk::UString& _data)
{
	EWOL_VERBOSE("SendMulticast message \"" << _message << "\" data=\"" << _data << "\" to :");
	
	// send the message at all registered widget ...
	for (int32_t iii=0; iii<m_messageList.Size(); iii++) {
		if(    m_messageList[iii].message == _message
		    && m_messageList[iii].object != _object)
		{
			if (NULL != m_messageList[iii].object) {
				EWOL_VERBOSE("        id = " << m_messageList[iii].object->GetId() << " type=" << m_messageList[iii].object->GetObjectType());
				// generate event ... (create message before ...
				ewol::EMessage tmpMsg(_object, m_messageList[iii].message, _data);
				m_messageList[iii].object->OnReceiveMessage(tmpMsg);
			}
		}
	}
}

void ewol::EObjectMessageMultiCast::AnonymousSend(const char* const _messageId, const etk::UString& _data)
{
	MultiCastSend(NULL, _messageId, _data);
}

#undef __class__
#define __class__	"ewol::EObject"


const char* const ewol::EObject::configName = "name";


ewol::EObject::EObject(void) :
	m_static(false)
{
	static int32_t ss_globalUniqueId = 0;
	// note this is nearly atomic ... (but it is enough)
	m_uniqueId = ss_globalUniqueId++;
	EWOL_DEBUG("new EObject : [" << m_uniqueId << "]");
	ewol::EObjectManager::Add(this);
	RegisterConfig(ewol::EObject::configName, "string", NULL, "EObject name, might be a unique reference in all the program");
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

void ewol::EObject::AddEventId(const char * _generateEventId)
{
	if (NULL != _generateEventId) {
		m_availlableEventId.PushBack(_generateEventId);
	}
}

void ewol::EObject::GenerateEventId(const char * _generateEventId, const etk::UString& _data)
{
	int32_t nbObject = ewol::EObjectManager::GetNumberObject();
	// for every element registered ...
	for (int32_t iii=0; iii<m_externEvent.Size(); iii++) {
		if (NULL!=m_externEvent[iii]) {
			// if we find the event ...
			if (m_externEvent[iii]->localEventId == _generateEventId) {
				if (NULL != m_externEvent[iii]->destEObject) {
					if (m_externEvent[iii]->overloadData.Size()<=0){
						ewol::EMessage tmpMsg(this, m_externEvent[iii]->destEventId, _data);
						m_externEvent[iii]->destEObject->OnReceiveMessage(tmpMsg);
					} else {
						// set the user requested data ...
						ewol::EMessage tmpMsg(this, m_externEvent[iii]->destEventId, m_externEvent[iii]->overloadData);
						m_externEvent[iii]->destEObject->OnReceiveMessage(tmpMsg);
					}
				}
			}
		}
	}
	if (nbObject > ewol::EObjectManager::GetNumberObject()) {
		EWOL_CRITICAL("It if really dangerous ro remove (delete) element inside a callback ... use ->RemoveObject() which is asynchronous");
	}
}

void ewol::EObject::SendMultiCast(const char* const _messageId, const etk::UString& _data)
{
	int32_t nbObject = ewol::EObjectManager::GetNumberObject();
	MultiCastSend(this, _messageId, _data);
	if (nbObject > ewol::EObjectManager::GetNumberObject()) {
		EWOL_CRITICAL("It if really dangerous ro remove (delete) element inside a callback ... use ->RemoveObject() which is asynchronous");
	}
}

void ewol::EObject::RegisterMultiCast(const char* const _messageId)
{
	MultiCastAdd(this, _messageId);
}

void ewol::EObject::RegisterOnEvent(ewol::EObject * _destinationObject,
                                    const char * _eventId,
                                    const char * _eventIdgenerated,
                                    const etk::UString& _overloadData)
{
	if (NULL == _destinationObject) {
		EWOL_ERROR("Input ERROR NULL pointer EObject ...");
		return;
	}
	if (NULL == _eventId) {
		EWOL_ERROR("Input ERROR NULL pointer Event Id...");
		return;
	}
	// check if event existed :
	bool findIt = false;
	for(int32_t iii=0; iii<m_availlableEventId.Size(); iii++) {
		if (m_availlableEventId[iii] == _eventId) {
			findIt = true;
			break;
		}
	}
	if (false == findIt) {
		EWOL_DEBUG("Try to register with a NON direct string name");
		for(int32_t iii=0; iii<m_availlableEventId.Size(); iii++) {
			if (0 == strncmp(m_availlableEventId[iii], _eventId, 1024)) {
				findIt = true;
				_eventIdgenerated = m_availlableEventId[iii];
				break;
			}
		}
	}
	if (false == findIt) {
		EWOL_ERROR("Can not register event on this WidgetType=" << GetObjectType() << " event=\"" << _eventId << "\" ==> unknow event");
		return;
	}
	ewol::EventExtGen * tmpEvent = new ewol::EventExtGen();
	if (NULL == tmpEvent) {
		EWOL_ERROR("Allocation error in Register Event...");
		return;
	}
	tmpEvent->localEventId = _eventId;
	tmpEvent->destEObject = _destinationObject;
	tmpEvent->overloadData = _overloadData;
	if (NULL != _eventIdgenerated) {
		tmpEvent->destEventId = _eventIdgenerated;
	} else {
		tmpEvent->destEventId = _eventId;
	}
	m_externEvent.PushBack(tmpEvent);
}


void ewol::EObject::OnObjectRemove(ewol::EObject * _removeObject)
{
	for(int32_t iii=m_externEvent.Size()-1; iii>=0; iii--) {
		if (NULL==m_externEvent[iii]) {
			m_externEvent.Erase(iii);
		} else if (m_externEvent[iii]->destEObject == _removeObject) {
			m_externEvent.Erase(iii);
		}
	}
}


void ewol::EObject::RegisterConfig(const char* _config, const char* _type, const char* _control, const char* _description, const char* _default)
{
	if (NULL == _config) {
		EWOL_ERROR("Try to add NULL config");
		return;
	}
	for(int32_t iii=0 ; iii<m_listConfig.Size() ; iii++) {
		if (NULL != m_listConfig[iii].GetConfig()) {
			if (0==strcmp(m_listConfig[iii].GetConfig(), _config) ) {
				EWOL_ERROR("Try to add config already added : " << _config << " at pos=" << iii);
			}
		}
	}
	m_listConfig.PushBack(ewol::EConfigElement(_config, _type, _control, _description, _default));
}


bool ewol::EObject::LoadXML(exml::Element* _node)
{
	if (NULL==_node) {
		return false;
	}
	bool errorOccured = true;
	for(int32_t iii=0 ; iii<m_listConfig.Size() ; iii++) {
		if (m_listConfig[iii].GetConfig() == NULL) {
			continue;
		}
		etk::UString value = _node->GetAttribute(m_listConfig[iii].GetConfig());
		// check existance :
		if (value.Size()==0) {
			continue;
		}
		if (false==SetConfig(ewol::EConfig(m_listConfig[iii].GetConfig(), value) ) ) {
			errorOccured = false;
		}
	}
	return errorOccured;
}

bool ewol::EObject::StoreXML(exml::Element* _node) const
{
	if (NULL==_node) {
		return false;
	}
	bool errorOccured = true;
	for(int32_t iii=0 ; iii<m_listConfig.Size() ; iii++) {
		if (m_listConfig[iii].GetConfig() == NULL) {
			continue;
		}
		etk::UString value = GetConfig(m_listConfig[iii].GetConfig());
		if (NULL != m_listConfig[iii].GetDefault() ) {
			if (value == m_listConfig[iii].GetDefault() ) {
				// nothing to add on the XML :
				continue;
			}
		}
		// add attribute ... ==> note : Add special element when '"' element detected ...
		_node->SetAttribute(m_listConfig[iii].GetConfig(), value);
	}
	return errorOccured;
}


bool ewol::EObject::OnSetConfig(const ewol::EConfig& _conf)
{
	EWOL_VERBOSE("[" << GetId() << "] {" << GetObjectType() << "} set config : " << _conf);
	if (_conf.GetConfig() == ewol::EObject::configName) {
		EWOL_VERBOSE("[" << GetId() << "] {" << GetObjectType() << "} Set config name : \"" << _conf.GetData() << "\"");
		SetName(_conf.GetData());
		return true;
	}
	return false;
}

bool ewol::EObject::OnGetConfig(const char* _config, etk::UString& _result) const
{
	if (_config == ewol::EObject::configName) {
		_result = GetName();
		return true;
	}
	return false;
};

bool ewol::EObject::SetConfig(const etk::UString& _config, const etk::UString& _value)
{
	for(int32_t iii=0 ; iii<m_listConfig.Size() ; iii++) {
		if (NULL != m_listConfig[iii].GetConfig()) {
			if (_config == m_listConfig[iii].GetConfig() ) {
				// call config with standard parameter
				return SetConfig(ewol::EConfig(m_listConfig[iii].GetConfig(), _value));
			}
		}
	}
	EWOL_ERROR(" parameter is not in the list : \"" << _config << "\"" );
	return false;
}

etk::UString ewol::EObject::GetConfig(const char* _config) const
{
	etk::UString res="";
	if (NULL != _config) {
		(void)OnGetConfig(_config, res);
	}
	return res;
};

etk::UString ewol::EObject::GetConfig(const etk::UString& _config) const
{
	for(int32_t iii=0 ; iii<m_listConfig.Size() ; iii++) {
		if (NULL != m_listConfig[iii].GetConfig()) {
			if (_config == m_listConfig[iii].GetConfig() ) {
				// call config with standard parameter
				return GetConfig(m_listConfig[iii].GetConfig());
			}
		}
	}
	EWOL_ERROR(" parameter is not in the list : \"" << _config << "\"" );
	return "";
}


bool ewol::EObject::SetConfigNamed(const etk::UString& _name, const ewol::EConfig& _conf)
{
	ewol::EObject* object = ewol::EObjectManager::Get(_name);
	if (object == NULL) {
		return false;
	}
	return object->SetConfig(_conf);
}

bool ewol::EObject::SetConfigNamed(const etk::UString& _name, const etk::UString& _config, const etk::UString& _value)
{
	ewol::EObject* object = ewol::EObjectManager::Get(_name);
	if (object == NULL) {
		return false;
	}
	return object->SetConfig(_config, _value);
}


