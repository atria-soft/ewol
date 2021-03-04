/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/object/Manager.hpp>
#include <ewol/context/Context.hpp>
#include <ewol/ewol.hpp>
#include <etk/stdTools.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ObjectManager);

ObjectManager::Manager(EwolContext _context) :
  this.context(_context),
  periodicCall(this, "periodic", "Call every time system render"),
  this.applWakeUpTime(0),
  this.lastPeriodicCallTime(0) {
	Log.debug(" == > init Object-Manager");
	periodicCall.setPeriodic(true);
	// set the basic time properties :
	this.applWakeUpTime = echrono::Clock::now();
	this.lastPeriodicCallTime = this.applWakeUpTime;
}

ObjectManager::~Manager() {
	ethread::RecursiveLock lock(this.mutex);
	this.workerList.clear();
	boolean hasError = false;
	if (this.eObjectList.size()!=0) {
		Log.error("Must not have anymore eObject !!!");
		hasError = true;
	}
	if (hasError == true) {
		Log.error("Check if the function UnInit has been called !!!");
	}
	displayListObject();
}

void ObjectManager::displayListObject() {
	ethread::RecursiveLock lock(this.mutex);
	Log.info("List loaded object : ");
	for (auto it : this.eObjectList) {
		EwolObject element = it.lock();
		if (element != null) {
			Log.info("  [" + element.getId() + "] ref=" + element.useCount()-1 + " name='" + element.propertyName.get() + "' type=" + element.getObjectType());
		}
	}
}

void ObjectManager::unInit() {
	ethread::RecursiveLock lock(this.mutex);
	Log.debug(" == > Un-Init Object-Manager");
	if (this.workerList.size() > 0) {
		Log.debug(" == > Remove all workers");
		this.workerList.clear();
	}
	for (auto it : this.eObjectList) {
		EwolObject element = it.lock();
		if (element != null) {
			//it.removeObject();
		}
	}
	if (this.eObjectList.size() != 0) {
		Log.error("Have " + this.eObjectList.size() + " active Object");
	}
	this.eObjectList.clear();
}

void ObjectManager::add( EwolObject _object) {
	ethread::RecursiveLock lock(this.mutex);
	if (_object == null) {
		Log.error("try to add an inexistant Object in manager");
	}
	this.eObjectList.pushBack(_object);
}

int ObjectManager::getNumberObject() {
	ethread::RecursiveLock lock(this.mutex);
	return this.eObjectList.size();
}

// clean all Object that request an autoRemove ...
void ObjectManager::cleanInternalRemoved() {
	ethread::RecursiveLock lock(this.mutex);
	int nbObject = this.eObjectList.size();
	Log.verbose("Clean Object List (if needed) : " + this.eObjectList.size() + " elements");
	auto it(this.eObjectList.begin());
	while (it != this.eObjectList.end()) {
		if (it.expired() == true) {
			it = this.eObjectList.erase(it);
		} else {
			++it;
		}
	}
	if (this.eObjectList.size() != nbObject) {
		Log.verbose(" remove " + nbObject - this.eObjectList.size() + " deprecated objects");
	}
}

EwolObject ObjectManager::get( String _name) {
	ethread::RecursiveLock lock(this.mutex);
	if (_name == "") {
		return null;
	}
	for (auto it : this.eObjectList) {
		EwolObject element = it.lock();
		if (    element != null
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM element.propertyName.get() == _name) {
			return element;
		}
	}
	return null;
}


EwolObject ObjectManager::getObjectNamed( String _name) {
	ethread::RecursiveLock lock(this.mutex);
	return ObjectManager::get(_name);
}


void ObjectManager::workerAdd( EwolObject _worker) {
	ethread::RecursiveLock lock(this.mutex);
	this.workerList.pushBack(_worker);
}

void ObjectManager::workerRemove( EwolObject _worker) {
	ethread::RecursiveLock lock(this.mutex);
	auto it(this.workerList.begin());
	while (it != this.workerList.end()) {
		if (*it == _worker) {
			it = this.workerList.erase(it);
		} else {
			++it;
		}
	}
}

void ObjectManager::timeCall( echrono::Clock _localTime) {
	ethread::RecursiveLock lock(this.mutex);
	echrono::Clock previousTime = this.lastPeriodicCallTime;
	this.lastPeriodicCallTime = _localTime;
	if (periodicCall.size() <= 0) {
		return;
	}
	echrono::Duration deltaTime = _localTime - previousTime;
	ewol::event::Time myTime(_localTime, this.applWakeUpTime, deltaTime, deltaTime);
	periodicCall.emit(myTime);
}

void ObjectManager::timeCallResume( echrono::Clock _localTime) {
	ethread::RecursiveLock lock(this.mutex);
	this.lastPeriodicCallTime = _localTime;
}

boolean ObjectManager::timeCallHave() {
	ethread::RecursiveLock lock(this.mutex);
	return periodicCall.size() > 0;
}
