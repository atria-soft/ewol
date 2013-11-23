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
size_t ewol::EObject::m_valUID = 0;

ewol::EObject::EObject(void) :
  m_static(false),
  m_isResource(false) {
	// note this is nearly atomic ... (but it is enough)
	m_uniqueId = m_valUID++;
	EWOL_DEBUG("new EObject : [" << m_uniqueId << "]");
	getEObjectManager().add(this);
	registerConfig(ewol::EObject::configName, "string", NULL, "EObject name, might be a unique reference in all the program");
}
ewol::EObject::EObject(const std::string& _name) :
  m_static(false),
  m_name(_name),
  m_isResource(false) {
	// note this is nearly atomic ... (but it is enough)
	m_uniqueId = m_valUID++;
	EWOL_DEBUG("new EObject : [" << m_uniqueId << "]");
	getEObjectManager().add(this);
	registerConfig(ewol::EObject::configName, "string", NULL, "EObject name, might be a unique reference in all the program");
}

ewol::EObject::~EObject(void) {
	EWOL_DEBUG("delete EObject : [" << m_uniqueId << "] : " << getTypeDescription());
	getEObjectManager().rm(this);
	getMultiCast().rm(this);
	for (int32_t iii=0; iii<m_externEvent.size(); iii++) {
		if (NULL!=m_externEvent[iii]) {
			delete(m_externEvent[iii]);
			m_externEvent[iii] = NULL;
		}
	}
	m_externEvent.clear();
	m_availlableEventId.clear();
	m_uniqueId = -1;
}

const char * const ewol::EObject::getObjectType(void) {
	if (m_listType.size() == 0) {
		return "ewol::EObject";
	}
	return m_listType.back();
}

void ewol::EObject::addObjectType(const char* _type) {
	if (_type == NULL) {
		EWOL_ERROR(" try to add a type with no value...");
		return;
	}
	m_listType.push_back(_type);
}
std::string ewol::EObject::getTypeDescription(void) {
	std::string ret("ewol::EObject");
	for(auto element : m_listType) {
		ret += "|";
		ret += element;
	}
	return ret;
}

bool ewol::EObject::isTypeCompatible(const std::string& _type) {
	if (_type == "ewol::EObject") {
		return true;
	}
	for(auto element : m_listType) {
		if (_type == element) {
			return true;
		}
	}
	return false;
}

void ewol::EObject::autoDestroy(void) {
	getEObjectManager().autoRemove(this);
}

void ewol::EObject::removeObject(void) {
	getEObjectManager().autoRemove(this);
}

void ewol::EObject::addEventId(const char * _generateEventId) {
	if (NULL != _generateEventId) {
		m_availlableEventId.push_back(_generateEventId);
	}
}

void ewol::EObject::generateEventId(const char * _generateEventId, const std::string& _data) {
	int32_t nbObject = getEObjectManager().getNumberObject();
	// for every element registered ...
	for (int32_t iii=0; iii<m_externEvent.size(); iii++) {
		if (NULL!=m_externEvent[iii]) {
			// if we find the event ...
			if (m_externEvent[iii]->localEventId == _generateEventId) {
				if (NULL != m_externEvent[iii]->destEObject) {
					if (m_externEvent[iii]->overloadData.size() <= 0){
						ewol::EMessage tmpMsg(this, m_externEvent[iii]->destEventId, _data);
						EWOL_VERBOSE("send message " << tmpMsg);
						m_externEvent[iii]->destEObject->onReceiveMessage(tmpMsg);
					} else {
						// set the user requested data ...
						ewol::EMessage tmpMsg(this, m_externEvent[iii]->destEventId, m_externEvent[iii]->overloadData);
						EWOL_VERBOSE("send message " << tmpMsg);
						m_externEvent[iii]->destEObject->onReceiveMessage(tmpMsg);
					}
				}
			}
		}
	}
	if (nbObject > getEObjectManager().getNumberObject()) {
		EWOL_CRITICAL("It if really dangerous ro remove (delete) element inside a callback ... use ->removeObject() which is asynchronous");
	}
}

void ewol::EObject::sendMultiCast(const char* const _messageId, const std::string& _data) {
	int32_t nbObject = getEObjectManager().getNumberObject();
	getMultiCast().send(this, _messageId, _data);
	if (nbObject > getEObjectManager().getNumberObject()) {
		EWOL_CRITICAL("It if really dangerous ro remove (delete) element inside a callback ... use ->removeObject() which is asynchronous");
	}
}

void ewol::EObject::registerMultiCast(const char* const _messageId) {
	getMultiCast().add(this, _messageId);
}

void ewol::EObject::registerOnEvent(ewol::EObject * _destinationObject,
                                    const char * _eventId,
                                    const char * _eventIdgenerated,
                                    const std::string& _overloadData) {
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
	for(int32_t iii=0; iii<m_availlableEventId.size(); iii++) {
		if (m_availlableEventId[iii] == _eventId) {
			findIt = true;
			break;
		}
	}
	if (false == findIt) {
		EWOL_DEBUG("Try to register with a NON direct string name");
		for(int32_t iii=0; iii<m_availlableEventId.size(); iii++) {
			if (0 == strncmp(m_availlableEventId[iii], _eventId, 1024)) {
				findIt = true;
				_eventIdgenerated = m_availlableEventId[iii];
				break;
			}
		}
	}
	if (false == findIt) {
		EWOL_ERROR("Can not register event on this Type=" << getObjectType() << " event=\"" << _eventId << "\"  == > unknow event");
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
	m_externEvent.push_back(tmpEvent);
}

void ewol::EObject::unRegisterOnEvent(ewol::EObject * _destinationObject,
                                      const char * _eventId) {
	if (NULL == _destinationObject) {
		EWOL_ERROR("Input ERROR NULL pointer EObject ...");
		return;
	}
	// check if event existed :
	for(int64_t iii = m_externEvent.size()-1; iii >= 0; --iii) {
		if (m_externEvent[iii] == NULL) {
			continue;
		}
		if (m_externEvent[iii]->destEObject != _destinationObject) {
			continue;
		}
		if (_eventId == NULL) {
			m_externEvent.erase(m_externEvent.begin()+iii);
		} else if (m_externEvent[iii]->localEventId == _eventId) {
			m_externEvent.erase(m_externEvent.begin()+iii);
		}
	}
}

void ewol::EObject::onObjectRemove(ewol::EObject * _removeObject) {
	for(int32_t iii=m_externEvent.size()-1; iii >= 0; iii--) {
		if (NULL == m_externEvent[iii]) {
			m_externEvent.erase(m_externEvent.begin()+iii);
		} else if (m_externEvent[iii]->destEObject == _removeObject) {
			m_externEvent.erase(m_externEvent.begin()+iii);
		}
	}
}


void ewol::EObject::registerConfig(const char* _config,
                                   const char* _type,
                                   const char* _control,
                                   const char* _description,
                                   const char* _default) {
	if (NULL == _config) {
		EWOL_ERROR("Try to add NULL config");
		return;
	}
	for(int32_t iii=0 ; iii<m_listConfig.size() ; iii++) {
		if (NULL != m_listConfig[iii].getConfig()) {
			if (0 == strcmp(m_listConfig[iii].getConfig(), _config) ) {
				EWOL_ERROR("Try to add config already added : " << _config << " at pos=" << iii);
			}
		}
	}
	m_listConfig.push_back(ewol::EConfigElement(_config, _type, _control, _description, _default));
}


bool ewol::EObject::loadXML(exml::Element* _node) {
	if (NULL == _node) {
		return false;
	}
	bool errorOccured = true;
	for(int32_t iii=0 ; iii<m_listConfig.size() ; iii++) {
		if (m_listConfig[iii].getConfig() == NULL) {
			continue;
		}
		std::string value = _node->getAttribute(m_listConfig[iii].getConfig());
		// check existance :
		if (value.size() == 0) {
			continue;
		}
		if (false == setConfig(ewol::EConfig(m_listConfig[iii].getConfig(), value) ) ) {
			errorOccured = false;
		}
	}
	return errorOccured;
}

bool ewol::EObject::storeXML(exml::Element* _node) const {
	if (NULL == _node) {
		return false;
	}
	bool errorOccured = true;
	for(int32_t iii=0 ; iii<m_listConfig.size() ; iii++) {
		if (m_listConfig[iii].getConfig() == NULL) {
			continue;
		}
		std::string value = getConfig(m_listConfig[iii].getConfig());
		if (NULL != m_listConfig[iii].getDefault() ) {
			if (value == m_listConfig[iii].getDefault() ) {
				// nothing to add on the XML :
				continue;
			}
		}
		// add attribute ...  == > note : add special element when '"' element detected ...
		_node->setAttribute(m_listConfig[iii].getConfig(), value);
	}
	return errorOccured;
}


bool ewol::EObject::onSetConfig(const ewol::EConfig& _conf) {
	EWOL_VERBOSE("[" << getId() << "] {" << getObjectType() << "} set config : " << _conf);
	if (_conf.getConfig() == ewol::EObject::configName) {
		EWOL_VERBOSE("[" << getId() << "] {" << getObjectType() << "} set config name : \"" << _conf.getData() << "\"");
		setName(_conf.getData());
		return true;
	}
	return false;
}

bool ewol::EObject::onGetConfig(const char* _config, std::string& _result) const {
	if (_config == ewol::EObject::configName) {
		_result = getName();
		return true;
	}
	return false;
}

bool ewol::EObject::setConfig(const std::string& _config, const std::string& _value) {
	for(int32_t iii=0 ; iii<m_listConfig.size() ; iii++) {
		if (NULL != m_listConfig[iii].getConfig()) {
			if (_config == m_listConfig[iii].getConfig() ) {
				// call config with standard parameter
				return setConfig(ewol::EConfig(m_listConfig[iii].getConfig(), _value));
			}
		}
	}
	EWOL_ERROR(" parameter is not in the list : \"" << _config << "\"" );
	return false;
}

std::string ewol::EObject::getConfig(const char* _config) const {
	std::string res="";
	if (NULL != _config) {
		(void)onGetConfig(_config, res);
	}
	return res;
}

std::string ewol::EObject::getConfig(const std::string& _config) const {
	for(int32_t iii=0 ; iii<m_listConfig.size() ; iii++) {
		if (NULL != m_listConfig[iii].getConfig()) {
			if (_config == m_listConfig[iii].getConfig() ) {
				// call config with standard parameter
				return getConfig(m_listConfig[iii].getConfig());
			}
		}
	}
	EWOL_ERROR(" parameter is not in the list : \"" << _config << "\"" );
	return "";
}

bool ewol::EObject::setConfigNamed(const std::string& _objectName, const ewol::EConfig& _conf) {
	ewol::EObject* object = getEObjectManager().get(_objectName);
	if (object == NULL) {
		return false;
	}
	return object->setConfig(_conf);
}

bool ewol::EObject::setConfigNamed(const std::string& _objectName, const std::string& _config, const std::string& _value) {
	ewol::EObject* object = getEObjectManager().get(_objectName);
	if (object == NULL) {
		return false;
	}
	return object->setConfig(_config, _value);
}

ewol::EObjectManager& ewol::EObject::getEObjectManager(void) {
	return ewol::getContext().getEObjectManager();
}

ewol::EMultiCast& ewol::EObject::getMultiCast(void) {
	return ewol::getContext().getEObjectManager().multiCast();
}

ewol::eContext& ewol::EObject::getContext(void) {
	return ewol::getContext();
}
