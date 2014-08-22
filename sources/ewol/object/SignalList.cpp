/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <memory>
#include <ewol/debug.h>
#include <ewol/object/Object.h>
#include <ewol/object/SignalList.h>
#include <ewol/object/SignalBase.h>
#include <ewol/context/Context.h>

ewol::object::SignalList::SignalList() {
	
}

ewol::object::SignalList::~SignalList() {
	m_list.clear();
}

// note this pointer is not allocated and not free at the end of the class
void ewol::object::SignalList::signalAdd(SignalBase* _pointerOnSignal) {
	if (_pointerOnSignal == nullptr) {
		EWOL_ERROR("Try to link a nullptr parameters");
		return;
	}
	m_list.push_back(_pointerOnSignal);
}

std::vector<std::string> ewol::object::SignalList::signalGetAll() const {
	std::vector<std::string> out;
	for (auto &it : m_list) {
		if(it != nullptr) {
			out.push_back(it->getName());
		}
	}
	return out;
}

void ewol::object::SignalList::registerOnObjectEvent(const std::shared_ptr<ewol::Object>& _destinationObject,
                                                     const std::string& _objectName,
                                                     const char * _eventId,
                                                     const char * _eventIdgenerated,
                                                     const std::string& _overloadData) {
	EWOL_TODO("RegisterOnEvent ... + name");
	/*ewol::object::Manager& tmp = ewol::getContext().getEObjectManager();
	std::shared_ptr<ewol::Object> tmpObject = tmp.getObjectNamed(_objectName);
	if (nullptr != tmpObject) {
		EWOL_DEBUG("Find widget named : '" << _objectName << "' register event='" << _eventId << "'");
		tmpObject->registerOnEvent(_destinationObject, _eventId, _eventIdgenerated, _overloadData);
	} else {
		EWOL_WARNING(" Can not register event : '" << _eventId << "' the object named='" << _objectName << "' does not exist");
	}
	*/
}

void ewol::object::SignalList::registerOnEvent(const std::shared_ptr<ewol::Object>& _destinationObject,
                                               const char * _eventId,
                                               const char * _eventIdgenerated,
                                               const std::string& _overloadData) {
	EWOL_TODO("RegisterOnEvent ...");
	/*
	if (_destinationObject == nullptr) {
		EWOL_ERROR("Input ERROR nullptr pointer Object ...");
		return;
	}
	if (_eventId == nullptr) {
		EWOL_ERROR("Input ERROR nullptr pointer Event Id...");
		return;
	}
	if (    _eventId[0] == '*'
	     && _eventId[1] == '\0') {
		EWOL_VERBOSE("Register on all event ...");
		for(auto &it : m_list) {
			if (it == nullptr) {
				continue;
			}
			it->connect(_destinationObject, _eventIdgenerated, _overloadData);
		}
		return;
	}
	// check if event existed :
	bool findIt = false;
	for(auto &it : m_list) {
		if (it == nullptr) {
			continue;
		}
		if (it->getName() == _eventId) {
			findIt = true;
			it->connect(_destinationObject, _eventIdgenerated, _overloadData);
		}
	}
	if (false == findIt) {
		EWOL_ERROR("Can not register event on this event=\"" << _eventId << "\"  == > unknow event");
		return;
	}
	*/
}

void ewol::object::SignalList::unRegisterOnEvent(const std::shared_ptr<ewol::Object>& _destinationObject,
                                                 const char * _eventId) {
	EWOL_TODO("unRegisterOnEvent ...");
	/*
	if (_destinationObject == nullptr) {
		EWOL_ERROR("Input ERROR nullptr pointer Object ...");
		return;
	}
	for(auto &it : m_list) {
		if (it == nullptr) {
			continue;
		}
		if (    it->getName() == _eventId
		     || (    _eventId[0] == '*'
		          && _eventId[1] == '\0') ) {
			it->release(_destinationObject);
		}
	}
	*/
}
