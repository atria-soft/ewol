/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/object/Object.h>
#include <ewol/object/Manager.h>
#include <ewol/debug.h>
#include <ewol/context/Context.h>
#include <ewol/object/MultiCast.h>

#undef __class__
#define __class__ "Object"


const char* const ewol::Object::configName = "name";
size_t ewol::Object::m_valUID = 0;


void ewol::Object::objRefCountIncrement() {
	std::unique_lock<std::mutex> lock(m_lockRefCount);
	/*
	if (std::string("ewol::widget::Windows") == getObjectType()) {
		EWOL_ERROR("increment Windows count ++" << m_objRefCount);
		//etk::log::displayBacktrace();
	}
	*/
	m_objRefCount++;
}

void ewol::Object::objRefCountDecrement() {
	std::unique_lock<std::mutex> lock(m_lockRefCount);
	/*
	if (std::string("ewol::widget::Windows") == getObjectType()) {
		EWOL_ERROR("Decrement Windows count --" << m_objRefCount);
		//etk::log::displayBacktrace();
	}
	*/
	m_objRefCount--;
}

void ewol::Object::operator delete(void* _ptr, std::size_t _sz) {
	EWOL_VERBOSE("custom delete for size " << _sz);
	ewol::Object* obj = (ewol::Object*)_ptr;
	obj->objRefCountDecrement();
	if (obj->m_objRefCount <= 0) {
		EWOL_VERBOSE("    ==> real remove");
		::operator delete(_ptr);
	} else {
		EWOL_ERROR("    ==> Some user is link on it : " << obj->m_objRefCount);
		etk::log::displayBacktrace();
	}
}

void ewol::Object::operator delete[](void* _ptr, std::size_t _sz) {
	EWOL_CRITICAL("custom delete for size ==> not implemented ..." << _sz);
	::operator delete(_ptr);
}

void ewol::Object::autoDestroy() {
	EWOL_VERBOSE("Destroy object : [" << getId() << "] type:" << getTypeDescription());
	{
		std::unique_lock<std::mutex> lock(m_lockRefCount);
		if (m_isDestroyed == true) {
			EWOL_WARNING("Request remove of a removed object");
			return;
		}
		m_isDestroyed = true;
	}
	getObjectManager().remove(this);
}

void ewol::Object::removeObject() {
	autoDestroy();
}

void ewol::Object::respownObject() {
	std::unique_lock<std::mutex> lock(m_lockRefCount);
	if (m_isDestroyed == false) {
		EWOL_WARNING("Respawn an alive object");
		return;
	}
	m_isDestroyed = false;
	getObjectManager().respown(this);
}

ewol::Object::Object() :
  m_objRefCount(1),
  m_isDestroyed(false),
  m_static(false),
  m_isResource(false) {
	// note this is nearly atomic ... (but it is enough)
	m_uniqueId = m_valUID++;
	EWOL_DEBUG("new Object : [" << m_uniqueId << "]");
	getObjectManager().add(this);
	registerConfig(configName, "string", nullptr, "Object name, might be a unique reference in all the program");
}
ewol::Object::Object(const std::string& _name) :
  m_objRefCount(1),
  m_isDestroyed(false),
  m_static(false),
  m_name(_name),
  m_isResource(false) {
	// note this is nearly atomic ... (but it is enough)
	m_uniqueId = m_valUID++;
	EWOL_DEBUG("new Object : [" << m_uniqueId << "]");
	getObjectManager().add(this);
	registerConfig(configName, "string", nullptr, "Object name, might be a unique reference in all the program");
}

ewol::Object::~Object() {
	EWOL_DEBUG("delete Object : [" << m_uniqueId << "] : " << getTypeDescription() << " refcount=" << m_objRefCount);
	getMultiCast().rm(this);
	for (size_t iii=0; iii<m_externEvent.size(); ++iii) {
		if (m_externEvent[iii] != nullptr) {
			delete(m_externEvent[iii]);
			m_externEvent[iii] = nullptr;
		}
	}
	m_externEvent.clear();
	m_availlableEventId.clear();
	m_uniqueId = -1;
}

const char * const ewol::Object::getObjectType() {
	if (m_listType.size() == 0) {
		return "ewol::Object";
	}
	return m_listType.back();
}

void ewol::Object::addObjectType(const char* _type) {
	if (_type == nullptr) {
		EWOL_ERROR(" try to add a type with no value...");
		return;
	}
	m_listType.push_back(_type);
}
std::string ewol::Object::getTypeDescription() {
	std::string ret("ewol::Object");
	for(auto element : m_listType) {
		ret += "|";
		ret += element;
	}
	return ret;
}

bool ewol::Object::isTypeCompatible(const std::string& _type) {
	if (_type == "ewol::Object") {
		return true;
	}
	for(auto element : m_listType) {
		if (_type == element) {
			return true;
		}
	}
	return false;
}

void ewol::Object::addEventId(const char * _generateEventId) {
	if (nullptr != _generateEventId) {
		m_availlableEventId.push_back(_generateEventId);
	}
}

void ewol::Object::generateEventId(const char * _generateEventId, const std::string& _data) {
	int32_t nbObject = getObjectManager().getNumberObject();
	EWOL_VERBOSE("try send message '" << _generateEventId << "'");
	// for every element registered ...
	for (size_t iii=0; iii<m_externEvent.size(); ++iii) {
		if (nullptr==m_externEvent[iii]) {
			EWOL_VERBOSE("    Null pointer");
			continue;
		}
		// if we find the event ...
		if (m_externEvent[iii]->localEventId != _generateEventId) {
			EWOL_VERBOSE("    wrong event '" << m_externEvent[iii]->localEventId << "' != '" << _generateEventId << "'");
			continue;
		}
		if (m_externEvent[iii]->destObject == nullptr) {
			EWOL_VERBOSE("    nullptr dest");
			continue;
		}
		if (m_externEvent[iii]->overloadData.size() <= 0){
			ewol::object::Message tmpMsg(this, m_externEvent[iii]->destEventId, _data);
			EWOL_VERBOSE("send message " << tmpMsg);
			m_externEvent[iii]->destObject->onReceiveMessage(tmpMsg);
		} else {
			// set the user requested data ...
			ewol::object::Message tmpMsg(this, m_externEvent[iii]->destEventId, m_externEvent[iii]->overloadData);
			EWOL_VERBOSE("send message " << tmpMsg);
			m_externEvent[iii]->destObject->onReceiveMessage(tmpMsg);
		}
	}
	if (nbObject > getObjectManager().getNumberObject()) {
		EWOL_CRITICAL("It if really dangerous ro remove (delete) element inside a callback ... use ->removObject() which is asynchronous");
	}
}

void ewol::Object::sendMultiCast(const char* const _messageId, const std::string& _data) {
	int32_t nbObject = getObjectManager().getNumberObject();
	getMultiCast().send(this, _messageId, _data);
	if (nbObject > getObjectManager().getNumberObject()) {
		EWOL_CRITICAL("It if really dangerous ro remove (delete) element inside a callback ... use ->removObject() which is asynchronous");
	}
}

void ewol::Object::registerMultiCast(const char* const _messageId) {
	getMultiCast().add(this, _messageId);
}

void ewol::Object::registerOnEvent(const ewol::object::Shared<ewol::Object>& _destinationObject,
                                    const char * _eventId,
                                    const char * _eventIdgenerated,
                                    const std::string& _overloadData) {
	if (nullptr == _destinationObject) {
		EWOL_ERROR("Input ERROR nullptr pointer Object ...");
		return;
	}
	if (nullptr == _eventId) {
		EWOL_ERROR("Input ERROR nullptr pointer Event Id...");
		return;
	}
	if (    _eventId[0] == '*'
	     && _eventId[1] == '\0') {
		EWOL_VERBOSE("Register on all event ...");
		for(size_t iii=0; iii<m_availlableEventId.size(); iii++) {
			ewol::object::EventExtGen * tmpEvent = new ewol::object::EventExtGen();
			if (nullptr == tmpEvent) {
				EWOL_ERROR("Allocation error in Register Event...");
				continue;
			}
			tmpEvent->localEventId = m_availlableEventId[iii];
			tmpEvent->destObject = _destinationObject;
			tmpEvent->overloadData = _overloadData;
			if (nullptr != _eventIdgenerated) {
				tmpEvent->destEventId = _eventIdgenerated;
			} else {
				tmpEvent->destEventId = m_availlableEventId[iii];
			}
			m_externEvent.push_back(tmpEvent);
		}
		return;
	}
	// check if event existed :
	bool findIt = false;
	for(size_t iii=0; iii<m_availlableEventId.size(); iii++) {
		if (m_availlableEventId[iii] == _eventId) {
			findIt = true;
			break;
		}
	}
	if (false == findIt) {
		EWOL_VERBOSE("Try to register with a NON direct string name");
		for(size_t iii=0; iii<m_availlableEventId.size(); iii++) {
			if (0 == strncmp(m_availlableEventId[iii], _eventId, 1024)) {
				findIt = true;
				_eventId = m_availlableEventId[iii];
				EWOL_VERBOSE("find event ID : '" << _eventId << "' ==> '" << _eventIdgenerated << "'");
				break;
			}
		}
	}
	if (false == findIt) {
		EWOL_ERROR("Can not register event on this Type=" << getObjectType() << " event=\"" << _eventId << "\"  == > unknow event");
		return;
	}
	ewol::object::EventExtGen * tmpEvent = new ewol::object::EventExtGen();
	if (nullptr == tmpEvent) {
		EWOL_ERROR("Allocation error in Register Event...");
		return;
	}
	tmpEvent->localEventId = _eventId;
	tmpEvent->destObject = _destinationObject;
	tmpEvent->overloadData = _overloadData;
	if (nullptr != _eventIdgenerated) {
		tmpEvent->destEventId = _eventIdgenerated;
	} else {
		tmpEvent->destEventId = _eventId;
	}
	m_externEvent.push_back(tmpEvent);
}

void ewol::Object::unRegisterOnEvent(const ewol::object::Shared<ewol::Object>& _destinationObject,
                                      const char * _eventId) {
	if (nullptr == _destinationObject) {
		EWOL_ERROR("Input ERROR nullptr pointer Object ...");
		return;
	}
	// check if event existed :
	for(int64_t iii = m_externEvent.size()-1; iii >= 0; --iii) {
		if (m_externEvent[iii] == nullptr) {
			continue;
		}
		if (m_externEvent[iii]->destObject != _destinationObject) {
			continue;
		}
		if (_eventId == nullptr) {
			m_externEvent.erase(m_externEvent.begin()+iii);
		} else if (m_externEvent[iii]->localEventId == _eventId) {
			m_externEvent.erase(m_externEvent.begin()+iii);
		}
	}
}

void ewol::Object::onObjectRemove(const ewol::object::Shared<ewol::Object>& _object) {
	EWOL_VERBOSE("[" << getId() << "] onObjectRemove(" << _object->getId() << ")");
	for(int32_t iii=m_externEvent.size()-1; iii >= 0; iii--) {
		if (m_externEvent[iii] != nullptr) {
			m_externEvent.erase(m_externEvent.begin()+iii);
			EWOL_VERBOSE("[" << getId() << "] Remove extern event : to object id=" << _object->getId());
		}
	}
}


void ewol::Object::registerConfig(const char* _config,
                                  const char* _type,
                                  const char* _control,
                                  const char* _description,
                                  const char* _default) {
	if (nullptr == _config) {
		EWOL_ERROR("Try to add nullptr config");
		return;
	}
	for(size_t iii=0 ; iii<m_listConfig.size() ; iii++) {
		if (nullptr != m_listConfig[iii].getConfig()) {
			if (0 == strcmp(m_listConfig[iii].getConfig(), _config) ) {
				EWOL_ERROR("Try to add config already added : " << _config << " at pos=" << iii);
			}
		}
	}
	m_listConfig.push_back(ewol::object::ConfigElement(_config, _type, _control, _description, _default));
}


bool ewol::Object::loadXML(exml::Element* _node) {
	if (nullptr == _node) {
		return false;
	}
	bool errorOccured = true;
	for(size_t iii=0 ; iii<m_listConfig.size() ; iii++) {
		if (m_listConfig[iii].getConfig() == nullptr) {
			continue;
		}
		if (_node->existAttribute(m_listConfig[iii].getConfig()) == false) {
			continue;
		}
		std::string value = _node->getAttribute(m_listConfig[iii].getConfig());
		if (false == setConfig(ewol::object::Config(m_listConfig[iii].getConfig(), value) ) ) {
			errorOccured = false;
		}
	}
	return errorOccured;
}

bool ewol::Object::storeXML(exml::Element* _node) const {
	if (nullptr == _node) {
		return false;
	}
	bool errorOccured = true;
	for(size_t iii=0 ; iii<m_listConfig.size() ; iii++) {
		if (m_listConfig[iii].getConfig() == nullptr) {
			continue;
		}
		std::string value = getConfig(m_listConfig[iii].getConfig());
		if (nullptr != m_listConfig[iii].getDefault() ) {
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


bool ewol::Object::onSetConfig(const ewol::object::Config& _conf) {
	EWOL_VERBOSE("[" << getId() << "] {" << getObjectType() << "} set config : " << _conf);
	if (_conf.getConfig() == configName) {
		EWOL_VERBOSE("[" << getId() << "] {" << getObjectType() << "} set config name : \"" << _conf.getData() << "\"");
		setName(_conf.getData());
		return true;
	}
	return false;
}

bool ewol::Object::onGetConfig(const char* _config, std::string& _result) const {
	if (_config == configName) {
		_result = getName();
		return true;
	}
	return false;
}

bool ewol::Object::setConfig(const std::string& _config, const std::string& _value) {
	for(size_t iii=0 ; iii<m_listConfig.size() ; iii++) {
		if (nullptr != m_listConfig[iii].getConfig()) {
			if (_config == m_listConfig[iii].getConfig() ) {
				// call config with standard parameter
				return setConfig(ewol::object::Config(m_listConfig[iii].getConfig(), _value));
			}
		}
	}
	EWOL_ERROR(" parameter is not in the list : \"" << _config << "\"" );
	return false;
}

std::string ewol::Object::getConfig(const char* _config) const {
	std::string res="";
	if (nullptr != _config) {
		onGetConfig(_config, res);
	}
	return res;
}

std::string ewol::Object::getConfig(const std::string& _config) const {
	for(size_t iii=0 ; iii<m_listConfig.size() ; iii++) {
		if (nullptr != m_listConfig[iii].getConfig()) {
			if (_config == m_listConfig[iii].getConfig() ) {
				// call config with standard parameter
				return getConfig(m_listConfig[iii].getConfig());
			}
		}
	}
	EWOL_ERROR(" parameter is not in the list : \"" << _config << "\"" );
	return "";
}

bool ewol::Object::setConfigNamed(const std::string& _objectName, const ewol::object::Config& _conf) {
	ewol::object::Shared<ewol::Object> object = getObjectManager().get(_objectName);
	if (object == nullptr) {
		return false;
	}
	return object->setConfig(_conf);
}

bool ewol::Object::setConfigNamed(const std::string& _objectName, const std::string& _config, const std::string& _value) {
	ewol::object::Shared<ewol::Object> object = getObjectManager().get(_objectName);
	if (object == nullptr) {
		return false;
	}
	return object->setConfig(_config, _value);
}

ewol::object::Manager& ewol::Object::getObjectManager() {
	return ewol::getContext().getEObjectManager();
}

ewol::object::MultiCast& ewol::Object::getMultiCast() {
	return ewol::getContext().getEObjectManager().multiCast();
}

ewol::Context& ewol::Object::getContext() {
	return ewol::getContext();
}
