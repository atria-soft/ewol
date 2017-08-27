/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/object/Object.hpp>
#include <ewol/object/Manager.hpp>
#include <ewol/debug.hpp>
#include <ewol/context/Context.hpp>

size_t ewol::Object::m_valUID = 0;

void ewol::Object::autoDestroy() {
	if (m_objectHasBeenInit == false) {
		EWOL_WARNING("try to auto destroy inside a constructor");
		return;
	}
	EWOL_VERBOSE("Destroy object: [" << getId() << "] type:" << getObjectType());
	ewol::ObjectShared parent = m_parent.lock();
	// TODO : set a signal to do this ...
	if (parent != nullptr) {
		EWOL_VERBOSE("Destroy object: Call parrent");
		parent->requestDestroyFromChild(sharedFromThis());
	}
	//if no parent ==> noting to do ...
	m_destroy = true;
}

bool ewol::Object::objectHasBeenCorectlyInit() {
	return m_objectHasBeenInit;
}

void ewol::Object::requestDestroyFromChild(const ewol::ObjectShared& _child) {
	EWOL_INFO("requestDestroyFromChild(...) is called when an object reference as a parent have a child that request quto-destroy ...");
	EWOL_CRITICAL("Call From Child with no effects ==> must implement : requestDestroyFromChild(...)");
}

void ewol::Object::destroy() {
	autoDestroy();
}

bool ewol::Object::isDestroyed() const {
	return m_destroy;
}

void ewol::Object::setParent(const ewol::ObjectShared& _newParent) {
	// TODO : Implement change of parent ...
	m_parent = _newParent;
}

void ewol::Object::removeParent() {
	m_parent.reset();
}

ewol::Object::Object() :
  propertyName(this, "name", "", "Object name, might be a unique reference in all the program"),
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
	getObjectManager().add(sharedFromThis());
	//parameterDisplay();
	m_objectHasBeenInit = true;
}

const char * const ewol::Object::getObjectType() const {
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
	m_listType.pushBack(_type);
}
etk::String ewol::Object::getTypeDescription() const {
	etk::String ret("ewol::Object");
	for(auto element : m_listType) {
		ret += "|";
		ret += element;
	}
	return ret;
}

bool ewol::Object::isTypeCompatible(const etk::String& _type) const {
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

bool ewol::Object::loadXMLAttributes(const exml::Element& _node) {
	if (_node.exist() == false) {
		return false;
	}
	bool errorOccured = false;
	
	for(const auto it : _node.attributes) {
		auto pair = it.getPair();
		if (pair.first == "") {
			continue;
		}
		if (properties.set(pair.first, pair.second) == false) {
			errorOccured = true;
		}
	}
	return errorOccured;
}

bool ewol::Object::loadXML(const exml::Element& _node) {
	return true; //loadXMLAttributes(_node);
}

bool ewol::Object::storeXML(exml::Element& _node) const {
	if (_node.exist() == false) {
		return false;
	}
	bool errorOccured = true;
	for (auto &it : properties.getAll(true)) {
		_node.attributes.set(it.first, it.second);
	}
	return errorOccured;
}

bool ewol::Object::propertySetOnWidgetNamed(const etk::String& _objectName, const etk::String& _config, const etk::String& _value) {
	ewol::ObjectShared object = getObjectManager().get(_objectName);
	if (object == nullptr) {
		return false;
	}
	return object->properties.set(_config, _value);
}

ewol::object::Manager& ewol::Object::getObjectManager() {
	ewol::object::Manager& tmp = ewol::getContext().getEObjectManager();
	return tmp;
}

ewol::Context& ewol::Object::getContext() {
	return ewol::getContext();
}

ewol::ObjectShared ewol::Object::getObjectNamed(const etk::String& _objectName) {
	return getObjectManager().getObjectNamed(_objectName);
}

ewol::ObjectShared ewol::Object::getSubObjectNamed(const etk::String& _objectName) {
	EWOL_VERBOSE("check if name : " << _objectName << " ?= " << propertyName.get());
	if (_objectName == propertyName.get()) {
		return sharedFromThis();
	}
	return nullptr;
}


bool ewol::propertySetOnObjectNamed(const etk::String& _objectName, const etk::String& _config, const etk::String& _value) {
	ewol::ObjectShared object = ewol::getContext().getEObjectManager().get(_objectName);
	if (object == nullptr) {
		return false;
	}
	return object->properties.set(_config, _value);
}

