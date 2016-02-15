/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/object/Object.h>
#include <ewol/object/Manager.h>
#include <ewol/debug.h>
#include <ewol/context/Context.h>

#undef __class__
#define __class__ "Object"

size_t ewol::Object::m_valUID = 0;

void ewol::Object::autoDestroy() {
	if (m_objectHasBeenInit == false) {
		EWOL_WARNING("try to auto destroy inside a constructor");
		return;
	}
	EWOL_VERBOSE("Destroy object : [" << getId() << "] type:" << getObjectType());
	std::shared_ptr<ewol::Object> parent = m_parent.lock();
	// TODO : set a signal to do this ...
	if (parent != nullptr) {
		parent->requestDestroyFromChild(shared_from_this());
	}
	//if no parent ==> noting to do ...
	m_destroy = true;
}

void ewol::Object::requestDestroyFromChild(const std::shared_ptr<ewol::Object>& _child) {
	EWOL_INFO("requestDestroyFromChild(...) is called when an object reference as a parent have a child that request quto-destroy ...");
	EWOL_CRITICAL("Call From Child with no effects ==> must implement : requestDestroyFromChild(...)");
}

void ewol::Object::setParent(const std::shared_ptr<ewol::Object>& _newParent) {
	// TODO : Implement change of parent ...
	m_parent = _newParent;
}

void ewol::Object::removeParent() {
	m_parent.reset();
}

ewol::Object::Object() :
  propertyName(*this, "name", "", "Object name, might be a unique reference in all the program"),
  m_objectHasBeenInit(false),
  m_destroy(false),
  m_static(false),
  m_isResource(false) {
	// note this is nearly atomic ... (but it is enough)
	m_uniqueId = m_valUID++;
	EWOL_DEBUG("new Object : [" << m_uniqueId << "]");
}

ewol::Object::~Object() {
	EWOL_DEBUG("delete Object : [" << m_uniqueId << "] : " << getTypeDescription());
	m_uniqueId = -1;
}


void ewol::Object::init() {
	getObjectManager().add(shared_from_this());
	//parameterDisplay();
	m_objectHasBeenInit = true;
}

void ewol::Object::init(const std::string& _name) {
	init();
	propertyName.set(_name);
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

bool ewol::Object::loadXML(const std::shared_ptr<const exml::Element>& _node) {
	if (_node == nullptr) {
		return false;
	}
	bool errorOccured = false;
	
	for(size_t iii=0 ; iii<_node->sizeAttribute() ; iii++) {
		auto pair = _node->getAttrPair(iii);
		if (pair.first == "") {
			continue;
		}
		if (propertySet(pair.first, pair.second) == false) {
			errorOccured = true;
		}
	}
	return errorOccured;
}

bool ewol::Object::storeXML(const std::shared_ptr<exml::Element>& _node) const {
	if (nullptr == _node) {
		return false;
	}
	bool errorOccured = true;
	for (auto &it : propertyGetAll(true)) {
		_node->setAttribute(it.first, it.second);
	}
	return errorOccured;
}

bool ewol::Object::propertySetOnWidgetNamed(const std::string& _objectName, const std::string& _config, const std::string& _value) {
	std::shared_ptr<ewol::Object> object = getObjectManager().get(_objectName);
	if (object == nullptr) {
		return false;
	}
	return object->propertySet(_config, _value);
}

ewol::object::Manager& ewol::Object::getObjectManager() {
	ewol::object::Manager& tmp = ewol::getContext().getEObjectManager();
	return tmp;
}

ewol::Context& ewol::Object::getContext() {
	return ewol::getContext();
}

std::shared_ptr<ewol::Object> ewol::Object::getObjectNamed(const std::string& _objectName) {
	return getObjectManager().getObjectNamed(_objectName);
}

std::shared_ptr<ewol::Object> ewol::Object::getSubObjectNamed(const std::string& _objectName) {
	EWOL_VERBOSE("check if name : " << _objectName << " ?= " << propertyName.get());
	if (_objectName == propertyName.get()) {
		return shared_from_this();
	}
	return nullptr;
}


bool ewol::propertySetOnObjectNamed(const std::string& _objectName, const std::string& _config, const std::string& _value) {
	std::shared_ptr<ewol::Object> object = ewol::getContext().getEObjectManager().get(_objectName);
	if (object == nullptr) {
		return false;
	}
	return object->propertySet(_config, _value);
}

