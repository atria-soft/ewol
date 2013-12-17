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
#define __class__	"ewol::Object"


const char* const ewol::Object::configName = "name";
size_t ewol::Object::m_valUID = 0;

ewol::Object::Object(void) :
  m_static(false),
  m_isResource(false) {
	// note this is nearly atomic ... (but it is enough)
	m_uniqueId = m_valUID++;
	EWOL_DEBUG("new Object : [" << m_uniqueId << "]");
	getObjectManager().add(this);
	registerConfig(ewol::Object::configName, "string", NULL, "Object name, might be a unique reference in all the program");
}
ewol::Object::Object(const std::string& _name) :
  m_static(false),
  m_name(_name),
  m_isResource(false) {
	// note this is nearly atomic ... (but it is enough)
	m_uniqueId = m_valUID++;
	EWOL_DEBUG("new Object : [" << m_uniqueId << "]");
	getObjectManager().add(this);
	registerConfig(ewol::Object::configName, "string", NULL, "Object name, might be a unique reference in all the program");
}

ewol::Object::~Object(void) {
	EWOL_DEBUG("delete Object : [" << m_uniqueId << "] : " << getTypeDescription());
	getObjectManager().rm(this);
	getMultiCast().rm(this);
	for (size_t iii=0; iii<m_externEvent.size(); iii++) {
		if (NULL!=m_externEvent[iii]) {
			delete(m_externEvent[iii]);
			m_externEvent[iii] = NULL;
		}
	}
	m_externEvent.clear();
	m_availlableEventId.clear();
	m_uniqueId = -1;
}

const char * const ewol::Object::getObjectType(void) {
	if (m_listType.size() == 0) {
		return "ewol::Object";
	}
	return m_listType.back();
}

void ewol::Object::addObjectType(const char* _type) {
	if (_type == NULL) {
		EWOL_ERROR(" try to add a type with no value...");
		return;
	}
	m_listType.push_back(_type);
}
std::string ewol::Object::getTypeDescription(void) {
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

void ewol::Object::autoDestroy(void) {
	getObjectManager().autoRemove(this);
}

void ewol::Object::removeObject(void) {
	getObjectManager().autoRemove(this);
}

void ewol::Object::addEventId(const char * _generateEventId) {
	if (NULL != _generateEventId) {
		m_availlableEventId.push_back(_generateEventId);
	}
}

void ewol::Object::generateEventId(const char * _generateEventId, const std::string& _data) {
	int32_t nbObject = getObjectManager().getNumberObject();
	EWOL_VERBOSE("try send message '" << _generateEventId << "'");
	// for every element registered ...
	for (size_t iii=0; iii<m_externEvent.size(); iii++) {
		if (NULL==m_externEvent[iii]) {
			EWOL_VERBOSE("    Null pointer");
			continue;
		}
		// if we find the event ...
		if (m_externEvent[iii]->localEventId != _generateEventId) {
			EWOL_VERBOSE("    wrong event '" << m_externEvent[iii]->localEventId << "' != '" << _generateEventId << "'");
			continue;
		}
		if (m_externEvent[iii]->destObject == NULL) {
			EWOL_VERBOSE("    NULL dest");
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

void ewol::Object::registerOnEvent(ewol::Object * _destinationObject,
                                    const char * _eventId,
                                    const char * _eventIdgenerated,
                                    const std::string& _overloadData) {
	if (NULL == _destinationObject) {
		EWOL_ERROR("Input ERROR NULL pointer Object ...");
		return;
	}
	if (NULL == _eventId) {
		EWOL_ERROR("Input ERROR NULL pointer Event Id...");
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
	if (NULL == tmpEvent) {
		EWOL_ERROR("Allocation error in Register Event...");
		return;
	}
	tmpEvent->localEventId = _eventId;
	tmpEvent->destObject = _destinationObject;
	tmpEvent->overloadData = _overloadData;
	if (NULL != _eventIdgenerated) {
		tmpEvent->destEventId = _eventIdgenerated;
	} else {
		tmpEvent->destEventId = _eventId;
	}
	m_externEvent.push_back(tmpEvent);
}

void ewol::Object::unRegisterOnEvent(ewol::Object * _destinationObject,
                                      const char * _eventId) {
	if (NULL == _destinationObject) {
		EWOL_ERROR("Input ERROR NULL pointer Object ...");
		return;
	}
	// check if event existed :
	for(int64_t iii = m_externEvent.size()-1; iii >= 0; --iii) {
		if (m_externEvent[iii] == NULL) {
			continue;
		}
		if (m_externEvent[iii]->destObject != _destinationObject) {
			continue;
		}
		if (_eventId == NULL) {
			m_externEvent.erase(m_externEvent.begin()+iii);
		} else if (m_externEvent[iii]->localEventId == _eventId) {
			m_externEvent.erase(m_externEvent.begin()+iii);
		}
	}
}

void ewol::Object::onObjectRemove(ewol::Object * _removObject) {
	for(int32_t iii=m_externEvent.size()-1; iii >= 0; iii--) {
		if (NULL == m_externEvent[iii]) {
			m_externEvent.erase(m_externEvent.begin()+iii);
		} else if (m_externEvent[iii]->destObject == _removObject) {
			m_externEvent.erase(m_externEvent.begin()+iii);
		}
	}
}


void ewol::Object::registerConfig(const char* _config,
                                   const char* _type,
                                   const char* _control,
                                   const char* _description,
                                   const char* _default) {
	if (NULL == _config) {
		EWOL_ERROR("Try to add NULL config");
		return;
	}
	for(size_t iii=0 ; iii<m_listConfig.size() ; iii++) {
		if (NULL != m_listConfig[iii].getConfig()) {
			if (0 == strcmp(m_listConfig[iii].getConfig(), _config) ) {
				EWOL_ERROR("Try to add config already added : " << _config << " at pos=" << iii);
			}
		}
	}
	m_listConfig.push_back(ewol::object::ConfigElement(_config, _type, _control, _description, _default));
}


bool ewol::Object::loadXML(exml::Element* _node) {
	if (NULL == _node) {
		return false;
	}
	bool errorOccured = true;
	for(size_t iii=0 ; iii<m_listConfig.size() ; iii++) {
		if (m_listConfig[iii].getConfig() == NULL) {
			continue;
		}
		std::string value = _node->getAttribute(m_listConfig[iii].getConfig());
		// check existance :
		if (value.size() == 0) {
			continue;
		}
		if (false == setConfig(ewol::object::Config(m_listConfig[iii].getConfig(), value) ) ) {
			errorOccured = false;
		}
	}
	return errorOccured;
}

bool ewol::Object::storeXML(exml::Element* _node) const {
	if (NULL == _node) {
		return false;
	}
	bool errorOccured = true;
	for(size_t iii=0 ; iii<m_listConfig.size() ; iii++) {
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


bool ewol::Object::onSetConfig(const ewol::object::Config& _conf) {
	EWOL_VERBOSE("[" << getId() << "] {" << getObjectType() << "} set config : " << _conf);
	if (_conf.getConfig() == ewol::Object::configName) {
		EWOL_VERBOSE("[" << getId() << "] {" << getObjectType() << "} set config name : \"" << _conf.getData() << "\"");
		setName(_conf.getData());
		return true;
	}
	return false;
}

bool ewol::Object::onGetConfig(const char* _config, std::string& _result) const {
	if (_config == ewol::Object::configName) {
		_result = getName();
		return true;
	}
	return false;
}

bool ewol::Object::setConfig(const std::string& _config, const std::string& _value) {
	for(size_t iii=0 ; iii<m_listConfig.size() ; iii++) {
		if (NULL != m_listConfig[iii].getConfig()) {
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
	if (NULL != _config) {
		(void)onGetConfig(_config, res);
	}
	return res;
}

std::string ewol::Object::getConfig(const std::string& _config) const {
	for(size_t iii=0 ; iii<m_listConfig.size() ; iii++) {
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

bool ewol::Object::setConfigNamed(const std::string& _objectName, const ewol::object::Config& _conf) {
	ewol::Object* object = getObjectManager().get(_objectName);
	if (object == NULL) {
		return false;
	}
	return object->setConfig(_conf);
}

bool ewol::Object::setConfigNamed(const std::string& _objectName, const std::string& _config, const std::string& _value) {
	ewol::Object* object = getObjectManager().get(_objectName);
	if (object == NULL) {
		return false;
	}
	return object->setConfig(_config, _value);
}

ewol::object::Manager& ewol::Object::getObjectManager(void) {
	return ewol::getContext().getEObjectManager();
}

ewol::object::MultiCast& ewol::Object::getMultiCast(void) {
	return ewol::getContext().getEObjectManager().multiCast();
}

ewol::Context& ewol::Object::getContext(void) {
	return ewol::getContext();
}
