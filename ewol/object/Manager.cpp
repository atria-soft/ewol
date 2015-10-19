/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/object/Manager.h>
#include <ewol/context/Context.h>
#include <ewol/ewol.h>
#include <etk/stdTools.h>
#include <unistd.h>

#undef __class__
#define __class__ "ewol::object::Manager"

ewol::object::Manager::Manager(ewol::Context& _context) :
  m_context(_context),
  periodicCall(*this, "periodic", "Call every time system render", true),
  m_applWakeUpTime(0),
  m_lastPeriodicCallTime(0) {
	EWOL_DEBUG(" == > init Object-Manager");
	// set the basic time properties :
	m_applWakeUpTime = ewol::getTime();
	m_lastPeriodicCallTime = ewol::getTime();
}

ewol::object::Manager::~Manager() {
	m_workerList.clear();
	bool hasError = false;
	if (m_eObjectList.size()!=0) {
		EWOL_ERROR("Must not have anymore eObject !!!");
		hasError = true;
	}
	if (true == hasError) {
		EWOL_ERROR("Check if the function UnInit has been called !!!");
	}
	displayListObject();
}

void ewol::object::Manager::displayListObject() {
	EWOL_INFO("List loaded object : ");
	for (auto &it : m_eObjectList) {
		std::shared_ptr<ewol::Object> element = it.lock();
		if (element != nullptr) {
			EWOL_INFO("  [" << element->getId() << "] ref=" << element.use_count()-1 << " name='" << element->getName() << "' type=" << element->getObjectType());
		}
	}
}

void ewol::object::Manager::unInit() {
	EWOL_DEBUG(" == > Un-Init Object-Manager");
	if (m_workerList.size() > 0) {
		EWOL_DEBUG(" == > Remove all workers");
		m_workerList.clear();
	}
	for (auto &it : m_eObjectList) {
		std::shared_ptr<ewol::Object> element = it.lock();
		if (element != nullptr) {
			//it->removeObject();
		}
	}
	if (m_eObjectList.size() != 0) {
		EWOL_ERROR("Have " << m_eObjectList.size() << " active Object");
	}
	m_eObjectList.clear();
}

void ewol::object::Manager::add(const std::shared_ptr<ewol::Object>& _object) {
	if (_object == nullptr) {
		EWOL_ERROR("try to add an inexistant Object in manager");
	}
	m_eObjectList.push_back(_object);
}

int32_t ewol::object::Manager::getNumberObject() {
	return m_eObjectList.size();
}

// clean all Object that request an autoRemove ...
void ewol::object::Manager::cleanInternalRemoved() {
	size_t nbObject = m_eObjectList.size();
	EWOL_VERBOSE("Clean Object List (if needed) : " << m_eObjectList.size() << " elements");
	auto it(m_eObjectList.begin());
	while (it != m_eObjectList.end()) {
		if (it->expired() == true) {
			it = m_eObjectList.erase(it);
		} else {
			++it;
		}
	}
	if (m_eObjectList.size() != nbObject) {
		EWOL_VERBOSE(" remove " << nbObject - m_eObjectList.size() << " deprecated objects");
	}
}

std::shared_ptr<ewol::Object> ewol::object::Manager::get(const std::string& _name) {
	if (_name == "") {
		return nullptr;
	}
	for (auto &it : m_eObjectList) {
		std::shared_ptr<ewol::Object> element = it.lock();
		if (    element != nullptr
		     && element->getName() == _name) {
			return element;
		}
	}
	return nullptr;
}


std::shared_ptr<ewol::Object> ewol::object::Manager::getObjectNamed(const std::string& _name) {
	return ewol::object::Manager::get(_name);
}


void ewol::object::Manager::workerAdd(const std::shared_ptr<ewol::Object>& _worker) {
	m_workerList.push_back(_worker);
}

void ewol::object::Manager::workerRemove(const std::shared_ptr<ewol::Object>& _worker) {
	auto it(m_workerList.begin());
	while (it != m_workerList.end()) {
		if (*it == _worker) {
			it = m_workerList.erase(it);
		} else {
			++it;
		}
	}
}

void ewol::object::Manager::timeCall(int64_t _localTime) {
	int64_t previousTime = m_lastPeriodicCallTime;
	m_lastPeriodicCallTime = _localTime;
	if (periodicCall.getNumberConnected() <= 0) {
		return;
	}
	float deltaTime = (float)(_localTime - previousTime)/1000000.0;
	ewol::event::Time myTime(_localTime, m_applWakeUpTime, deltaTime, deltaTime);
	periodicCall.emit(myTime);
}

void ewol::object::Manager::timeCallResume(int64_t _localTime) {
	m_lastPeriodicCallTime = _localTime;
}

bool ewol::object::Manager::timeCallHave() {
	return periodicCall.getNumberConnected() > 0;
}
