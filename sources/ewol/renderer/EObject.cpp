/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/renderer/EObject.h>
#include <ewol/renderer/EObjectManager.h>
#include <ewol/debug.h>
#include <ewol/renderer/eContext.h>

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
	GetEObjectManager().Add(this);
	RegisterConfig(ewol::EObject::configName, "string", NULL, "EObject name, might be a unique reference in all the program");
}

ewol::EObject::~EObject(void)
{
	EWOL_DEBUG("delete EObject : [" << m_uniqueId << "]");
	GetEObjectManager().Rm(this);
	GetEObjectMessageMultiCast().Rm(this);
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
	GetEObjectManager().AutoRemove(this);
}
void ewol::EObject::RemoveObject(void)
{
	GetEObjectManager().AutoRemove(this);
}

void ewol::EObject::AddEventId(const char * _generateEventId)
{
	if (NULL != _generateEventId) {
		m_availlableEventId.PushBack(_generateEventId);
	}
}

void ewol::EObject::GenerateEventId(const char * _generateEventId, const etk::UString& _data)
{
	int32_t nbObject = GetEObjectManager().GetNumberObject();
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
	if (nbObject > GetEObjectManager().GetNumberObject()) {
		EWOL_CRITICAL("It if really dangerous ro remove (delete) element inside a callback ... use ->RemoveObject() which is asynchronous");
	}
}

void ewol::EObject::SendMultiCast(const char* const _messageId, const etk::UString& _data)
{
	int32_t nbObject = GetEObjectManager().GetNumberObject();
	GetEObjectMessageMultiCast().Send(this, _messageId, _data);
	if (nbObject > GetEObjectManager().GetNumberObject()) {
		EWOL_CRITICAL("It if really dangerous ro remove (delete) element inside a callback ... use ->RemoveObject() which is asynchronous");
	}
}

void ewol::EObject::RegisterMultiCast(const char* const _messageId)
{
	GetEObjectMessageMultiCast().Add(this, _messageId);
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
	ewol::EObject* object = GetEObjectManager().Get(_name);
	if (object == NULL) {
		return false;
	}
	return object->SetConfig(_conf);
}

bool ewol::EObject::SetConfigNamed(const etk::UString& _name, const etk::UString& _config, const etk::UString& _value)
{
	ewol::EObject* object = GetEObjectManager().Get(_name);
	if (object == NULL) {
		return false;
	}
	return object->SetConfig(_config, _value);
}


ewol::EObjectManager& ewol::EObject::GetEObjectManager(void)
{
	return ewol::GetContext().GetEObjectManager();
}

ewol::EMultiCast& ewol::EObject::GetEObjectMessageMultiCast(void)
{
	return ewol::GetContext().GetEObjectManager().MultiCast();
}

ewol::eSystem& ewol::EObject::GetSystem(void)
{
	return ewol::GetContext();
}
