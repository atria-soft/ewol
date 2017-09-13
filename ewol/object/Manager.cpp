/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/object/Manager.hpp>
#include <ewol/context/Context.hpp>
#include <ewol/ewol.hpp>
#include <etk/stdTools.hpp>


ewol::object::Manager::Manager(ewol::Context& _context) :
  m_context(_context),
  periodicCall(this, "periodic", "Call every time system render"),
  m_applWakeUpTime(0),
  m_lastPeriodicCallTime(0) {
	EWOL_DEBUG(" == > init Object-Manager");
	periodicCall.setPeriodic(true);
	// set the basic time properties :
	m_applWakeUpTime = echrono::Clock::now();
	m_lastPeriodicCallTime = echrono::Clock::now();
}

ewol::object::Manager::~Manager() {
	ethread::RecursiveLock lock(m_mutex);
	m_workerList.clear();
	bool hasError = false;
	if (m_eObjectList.size()!=0) {
		EWOL_ERROR("Must not have anymore eObject !!!");
		hasError = true;
	}
	if (hasError == true) {
		EWOL_ERROR("Check if the function UnInit has been called !!!");
	}
	displayListObject();
}

void ewol::object::Manager::displayListObject() {
	ethread::RecursiveLock lock(m_mutex);
	EWOL_INFO("List loaded object : ");
	for (auto &it : m_eObjectList) {
		ewol::ObjectShared element = it.lock();
		if (element != nullptr) {
			EWOL_INFO("  [" << element->getId() << "] ref=" << element.useCount()-1 << " name='" << element->propertyName.get() << "' type=" << element->getObjectType());
		}
	}
}

void ewol::object::Manager::unInit() {
	ethread::RecursiveLock lock(m_mutex);
	EWOL_DEBUG(" == > Un-Init Object-Manager");
	if (m_workerList.size() > 0) {
		EWOL_DEBUG(" == > Remove all workers");
		m_workerList.clear();
	}
	for (auto &it : m_eObjectList) {
		ewol::ObjectShared element = it.lock();
		if (element != nullptr) {
			//it->removeObject();
		}
	}
	if (m_eObjectList.size() != 0) {
		EWOL_ERROR("Have " << m_eObjectList.size() << " active Object");
	}
	m_eObjectList.clear();
}

void ewol::object::Manager::add(const ewol::ObjectShared& _object) {
	ethread::RecursiveLock lock(m_mutex);
	if (_object == nullptr) {
		EWOL_ERROR("try to add an inexistant Object in manager");
	}
	m_eObjectList.pushBack(_object);
}

int32_t ewol::object::Manager::getNumberObject() {
	ethread::RecursiveLock lock(m_mutex);
	return m_eObjectList.size();
}

// clean all Object that request an autoRemove ...
void ewol::object::Manager::cleanInternalRemoved() {
	ethread::RecursiveLock lock(m_mutex);
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

ewol::ObjectShared ewol::object::Manager::get(const etk::String& _name) {
	ethread::RecursiveLock lock(m_mutex);
	if (_name == "") {
		return nullptr;
	}
	for (auto &it : m_eObjectList) {
		ewol::ObjectShared element = it.lock();
		if (    element != nullptr
		     && element->propertyName.get() == _name) {
			return element;
		}
	}
	return nullptr;
}


ewol::ObjectShared ewol::object::Manager::getObjectNamed(const etk::String& _name) {
	ethread::RecursiveLock lock(m_mutex);
	return ewol::object::Manager::get(_name);
}


void ewol::object::Manager::workerAdd(const ewol::ObjectShared& _worker) {
	ethread::RecursiveLock lock(m_mutex);
	m_workerList.pushBack(_worker);
}

void ewol::object::Manager::workerRemove(const ewol::ObjectShared& _worker) {
	ethread::RecursiveLock lock(m_mutex);
	auto it(m_workerList.begin());
	while (it != m_workerList.end()) {
		if (*it == _worker) {
			it = m_workerList.erase(it);
		} else {
			++it;
		}
	}
}

void ewol::object::Manager::timeCall(const echrono::Clock& _localTime) {
	ethread::RecursiveLock lock(m_mutex);
	echrono::Clock previousTime = m_lastPeriodicCallTime;
	m_lastPeriodicCallTime = _localTime;
	if (periodicCall.size() <= 0) {
		return;
	}
	echrono::Duration deltaTime = _localTime - previousTime;
	ewol::event::Time myTime(_localTime, m_applWakeUpTime, deltaTime, deltaTime);
	periodicCall.emit(myTime);
}

void ewol::object::Manager::timeCallResume(const echrono::Clock& _localTime) {
	ethread::RecursiveLock lock(m_mutex);
	m_lastPeriodicCallTime = _localTime;
}

bool ewol::object::Manager::timeCallHave() {
	ethread::RecursiveLock lock(m_mutex);
	return periodicCall.size() > 0;
}
