/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/ewol.hpp>
#include <ewol/object/Object.hpp>
#include <ewol/object/Manager.hpp>
#include <ewol/context/Context.hpp>
#include <ewol/context/InputManager.hpp>
#include <ewol/resource/Texture.hpp>
#include <ewol/widget/Widget.hpp>
#include <ewol/widget/Windows.hpp>
#include <ewol/widget/Manager.hpp>



#define EVENT_DEBUG  EWOL_VERBOSE
//#define EVENT_DEBUG  EWOL_DEBUG

void ewol::context::InputManager::calculateLimit() {
	m_eventInputLimit.sepatateTime = echrono::Duration(std::chrono::milliseconds(300));
	m_eventInputLimit.DpiOffset = m_dpi*100;
	m_eventMouseLimit.sepatateTime = echrono::Duration(std::chrono::milliseconds(300));
	m_eventMouseLimit.DpiOffset = float(m_dpi)*0.1f;
}

void ewol::context::InputManager::setDpi(int32_t newDPI) {
	m_dpi = newDPI;
	// recalculate the DPI system ...
	calculateLimit();
}

bool ewol::context::InputManager::localEventInput(enum gale::key::type _type,
                                                  ewol::WidgetShared _destWidget,
                                                  int32_t _IdInput,
                                                  enum gale::key::status _status,
                                                  vec2 _pos) {
	if (_destWidget != nullptr) {
		if (    _type == gale::key::type::mouse
		     || _type == gale::key::type::finger) {
			// create the system Event :
			ewol::event::InputSystem tmpEventSystem(_type, _status, _IdInput, _pos, _destWidget, 0, m_specialKey); // TODO : set the real ID ...
			// generate the event :
			return _destWidget->systemEventInput(tmpEventSystem);
		} else {
			return false;
		}
	}
	return false;
}

void ewol::context::InputManager::abortElement(InputPoperty *_eventTable,
                                               int32_t _idInput,
                                               enum gale::key::type _type) {
	if (_eventTable == nullptr) {
		return;
	}
	if (_eventTable[_idInput].isUsed == true) {
		localEventInput(_type, 
		                _eventTable[_idInput].curentWidgetEvent.lock(),
		                _eventTable[_idInput].destinationInputId,
		                gale::key::status::abort,
		                _eventTable[_idInput].posEvent);
	}
}

void ewol::context::InputManager::cleanElement(InputPoperty *_eventTable,
                                               int32_t _idInput) {
	if (_eventTable == nullptr) {
		return;
	}
	//EWOL_INFO("CleanElement[" << idInput << "] = @" << (int64_t)eventTable);
	_eventTable[_idInput].isUsed = false;
	_eventTable[_idInput].destinationInputId = 0;
	_eventTable[_idInput].lastTimeEvent.reset();
	_eventTable[_idInput].curentWidgetEvent.reset();
	_eventTable[_idInput].origin.setValue(0,0);
	_eventTable[_idInput].size.setValue(99999999,99999999);
	_eventTable[_idInput].downStart.setValue(0,0);
	_eventTable[_idInput].isDown = false;
	_eventTable[_idInput].isInside = false;
	_eventTable[_idInput].nbClickEvent = 0;
	_eventTable[_idInput].posEvent.setValue(0,0);
}

void ewol::context::InputManager::transfertEvent(ewol::WidgetShared _source, ewol::WidgetShared _destination) {
	if(    _source == nullptr
	    || _destination == nullptr) {
		// prevent errors ...
		return;
	}
	for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
		ewol::WidgetShared tmpWidget = m_eventInputSaved[iii].curentWidgetEvent.lock();
		if (tmpWidget == _source) {
			// inform the widget that it does not receive the event now
			EVENT_DEBUG("GUI : Input ID=" << iii << " == >" << m_eventInputSaved[iii].destinationInputId << " [EVENT_INPUT_TYPE_ABORT] " << m_eventInputSaved[iii].posEvent);
			localEventInput(gale::key::type::finger, tmpWidget, m_eventInputSaved[iii].destinationInputId, gale::key::status::abort, m_eventInputSaved[iii].posEvent);
			// set the new widget ...
			m_eventInputSaved[iii].curentWidgetEvent = _destination;
			// inform the widget that he receive the event property now...
			EVENT_DEBUG("GUI : Input ID=" << iii << " == >" << m_eventInputSaved[iii].destinationInputId << " [EVENT_INPUT_TYPE_TRANSFERT] " << m_eventInputSaved[iii].posEvent);
			localEventInput(gale::key::type::finger, _destination, m_eventInputSaved[iii].destinationInputId, gale::key::status::transfert, m_eventInputSaved[iii].posEvent);
		}
		tmpWidget = m_eventMouseSaved[iii].curentWidgetEvent.lock();
		if (tmpWidget == _source) {
			// inform the widget that it does not receive the event now
			EVENT_DEBUG("GUI : Input ID=" << iii << " == >" << m_eventMouseSaved[iii].destinationInputId << " [EVENT_INPUT_TYPE_ABORT] " << m_eventMouseSaved[iii].posEvent);
			localEventInput(gale::key::type::mouse, tmpWidget, m_eventMouseSaved[iii].destinationInputId, gale::key::status::abort, m_eventMouseSaved[iii].posEvent);
			// set the new widget ...
			m_eventMouseSaved[iii].curentWidgetEvent = _destination;
			// inform the widget that he receive the event property now...
			EVENT_DEBUG("GUI : Input ID=" << iii << " == >" << m_eventMouseSaved[iii].destinationInputId << " [EVENT_INPUT_TYPE_TRANSFERT] " << m_eventMouseSaved[iii].posEvent);
			localEventInput(gale::key::type::mouse, _destination, m_eventMouseSaved[iii].destinationInputId, gale::key::status::transfert, m_eventMouseSaved[iii].posEvent);
		}
	}
}

void ewol::context::InputManager::grabPointer(ewol::WidgetShared _widget) {
	if(_widget == nullptr) {
		return;
	}
	m_grabWidget = _widget;
	/* TODO : 
	m_context.grabPointerEvents(true,   _widget->getOrigin()
	                                  + ivec2(_widget->getSize().x()/2.0f,
	                                          _widget->getSize().y()/2.0f) );
	*/
}

void ewol::context::InputManager::unGrabPointer() {
	m_grabWidget.reset();
	// TODO: m_context.grabPointerEvents(false, vec2(0,0));
}

void ewol::context::InputManager::newLayerSet() {
	for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
		// remove the property of this input ...
		abortElement(m_eventInputSaved, iii, gale::key::type::finger);
		cleanElement(m_eventInputSaved, iii);
		abortElement(m_eventMouseSaved, iii, gale::key::type::mouse);
		cleanElement(m_eventMouseSaved, iii);
	}
}

ewol::context::InputManager::InputManager(ewol::Context& _context) :
  m_grabWidget(),
  m_context(_context) {
	setDpi(200);
	EWOL_INFO("Init (start)");
	for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
		// remove the property of this input ...
		cleanElement(m_eventInputSaved, iii);
		cleanElement(m_eventMouseSaved, iii);
	}
	EWOL_INFO("Init (end)");
}

ewol::context::InputManager::~InputManager() {
	EWOL_INFO("Un-Init (start)");
	EWOL_INFO("Un-Init (end)");
}

int32_t ewol::context::InputManager::localGetDestinationId(enum gale::key::type _type,
                                                           ewol::WidgetShared _destWidget,
                                                           int32_t _realInputId) {
	if (_type == gale::key::type::finger) {
		int32_t lastMinimum = 0;
		for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
			if (true == m_eventInputSaved[iii].isUsed) {
				ewol::WidgetShared tmpWidget = m_eventInputSaved[iii].curentWidgetEvent.lock();
				if (tmpWidget == _destWidget) {
					if (iii != _realInputId) {
						lastMinimum = etk::max(lastMinimum, m_eventInputSaved[iii].destinationInputId);
					}
				}
			}
		}
		return lastMinimum+1;
	}
	return _realInputId;
}

// note if id<0  == > the it was finger event ...
void ewol::context::InputManager::motion(enum gale::key::type _type,
                                         int _pointerID,
                                         vec2 _pos) {
	EVENT_DEBUG("motion event : " << _type << " " << _pointerID << " " << _pos);
	if (MAX_MANAGE_INPUT <= _pointerID) {
		// reject pointer  == > out of IDs...
		return;
	}
	InputPoperty *eventTable = nullptr;
	if (_type == gale::key::type::mouse) {
		eventTable = m_eventMouseSaved;
	} else if (_type == gale::key::type::finger) {
		eventTable = m_eventInputSaved;
	} else {
		EWOL_ERROR("Unknown type of event");
		return;
	}
	if(    _pointerID > MAX_MANAGE_INPUT
	    || _pointerID < 0) {
		// not manage input
		return;
	}
	ewol::widget::WindowsShared tmpWindows = m_context.getWindows();
	// special case for the mouse event 0 that represent the hover event of the system :
	if (    _type == gale::key::type::mouse
	     && _pointerID == 0) {
		// this event is all time on the good widget ... and manage the enter and leave ...
		// NOTE : the "layer widget" force us to get the widget at the specific position all the time :
		ewol::WidgetShared tmpWidget;
		if (m_grabWidget.lock() != nullptr) {
			// grab all events ...
			tmpWidget = m_grabWidget.lock();
		} else {
			if (tmpWindows != nullptr) {
				tmpWidget = tmpWindows->getWidgetAtPos(_pos);
			}
		}
		if(    tmpWidget != eventTable[_pointerID].curentWidgetEvent.lock()
		    || (    eventTable[_pointerID].isInside == true
		         && (     eventTable[_pointerID].origin.x() > _pos.x()
		              ||  eventTable[_pointerID].origin.y() > _pos.y()
		              || (eventTable[_pointerID].origin.x() + eventTable[_pointerID].size.x()) < _pos.x()
		              || (eventTable[_pointerID].origin.y() + eventTable[_pointerID].size.y()) < _pos.y()) ) ) {
			eventTable[_pointerID].isInside = false;
			EVENT_DEBUG("GUI : Input ID=" << _pointerID << " == >" << eventTable[_pointerID].destinationInputId << " [LEAVE] " << _pos);
			eventTable[_pointerID].posEvent = _pos;
			localEventInput(_type,
			                eventTable[_pointerID].curentWidgetEvent.lock(),
			                eventTable[_pointerID].destinationInputId,
			                gale::key::status::leave,
			                _pos);
		}
		if (eventTable[_pointerID].isInside == false) {
			// set the element inside ...
			eventTable[_pointerID].isInside = true;
			// get destination widget :
			eventTable[_pointerID].curentWidgetEvent = tmpWidget;
			if (tmpWidget == nullptr) {
				eventTable[_pointerID].isInside = false;
			} else {
				eventTable[_pointerID].origin = tmpWidget->getOrigin();
				eventTable[_pointerID].size = tmpWidget->getSize();
			}
			eventTable[_pointerID].destinationInputId = 0;
			EVENT_DEBUG("GUI : Input ID=" << _pointerID
			            << " == >" << eventTable[_pointerID].destinationInputId
			            << " [ENTER] " << _pos);
			eventTable[_pointerID].posEvent = _pos;
			localEventInput(_type,
			                tmpWidget,
			                eventTable[_pointerID].destinationInputId,
			                gale::key::status::enter,
			                _pos);
		}
		EVENT_DEBUG("GUI : Input ID=" << _pointerID
		            << " == >" << eventTable[_pointerID].destinationInputId
		            << " [MOVE]  " << _pos);
		eventTable[_pointerID].posEvent = _pos;
		localEventInput(_type,
		                tmpWidget,
		                eventTable[_pointerID].destinationInputId,
		                gale::key::status::move,
		                _pos);
	} else if (eventTable[_pointerID].isUsed == true) {
		if (eventTable[_pointerID].isInside == true) {
			if(     eventTable[_pointerID].origin.x() > _pos.x()
			    ||  eventTable[_pointerID].origin.y() > _pos.y()
			    || (eventTable[_pointerID].origin.x() + eventTable[_pointerID].size.x()) < _pos.x()
			    || (eventTable[_pointerID].origin.y() + eventTable[_pointerID].size.y()) < _pos.y()) {
				eventTable[_pointerID].isInside = false;
				EVENT_DEBUG("GUI : Input ID=" << _pointerID
				            << " == >" << eventTable[_pointerID].destinationInputId
				            << " [LEAVE] " << _pos);
				eventTable[_pointerID].posEvent = _pos;
				localEventInput(_type,
				                eventTable[_pointerID].curentWidgetEvent.lock(),
				                eventTable[_pointerID].destinationInputId,
				                gale::key::status::leave,
				                _pos);
			}
		} else {
			if(    (     eventTable[_pointerID].origin.x() <= _pos.x()
			         && (eventTable[_pointerID].origin.x() + eventTable[_pointerID].size.x()) >= _pos.x() )
			    && (     eventTable[_pointerID].origin.y() <= _pos.y()
			         && (eventTable[_pointerID].origin.y() + eventTable[_pointerID].size.y()) >= _pos.y() ) ) {
				eventTable[_pointerID].isInside = true;
				EVENT_DEBUG("GUI : Input ID=" << _pointerID
				            << " == >" << eventTable[_pointerID].destinationInputId
				            << " [ENTER] " << _pos);
				eventTable[_pointerID].posEvent = _pos;
				localEventInput(_type,
				                eventTable[_pointerID].curentWidgetEvent.lock(),
				                eventTable[_pointerID].destinationInputId,
				                gale::key::status::enter,
				                _pos);
			}
		}
		EVENT_DEBUG("GUI : Input ID=" << _pointerID
		            << " == >" << eventTable[_pointerID].destinationInputId
		            << " [MOVE]  " << _pos);
		eventTable[_pointerID].posEvent = _pos;
		localEventInput(_type,
		                eventTable[_pointerID].curentWidgetEvent.lock(),
		                eventTable[_pointerID].destinationInputId,
		                gale::key::status::move,
		                _pos);
	}
}

void ewol::context::InputManager::state(enum gale::key::type _type,
                                        int _pointerID,
                                        bool _isDown,
                                        vec2 _pos) {
	if (_pointerID >= MAX_MANAGE_INPUT) {
		// reject pointer  == > out of IDs...
		return;
	}
	EVENT_DEBUG("event pointerId=" << _pointerID);
	// convert position in open-GL coordonates ...
	InputPoperty *eventTable = nullptr;
	InputLimit   localLimit;
	if (_type == gale::key::type::mouse) {
		eventTable = m_eventMouseSaved;
		localLimit = m_eventMouseLimit;
	} else if (_type == gale::key::type::finger) {
		eventTable = m_eventInputSaved;
		localLimit = m_eventInputLimit;
	} else {
		EWOL_ERROR("Unknown type of event");
		return;
	}
	if(    _pointerID > MAX_MANAGE_INPUT
	    || _pointerID <= 0) {
		// not manage input
		return;
	}
	// get the curent time ...
	echrono::Clock currentTime = echrono::Clock::now();
	ewol::widget::WindowsShared tmpWindows = m_context.getWindows();
	
	if (_isDown == true) {
		EVENT_DEBUG("GUI : Input ID=" << _pointerID
		             << " == >" << eventTable[_pointerID].destinationInputId
		             << " [DOWN] " << _pos);
		if(eventTable[_pointerID].isUsed == true) {
			// we have an event previously ... check delay between click and offset position
			if (currentTime - eventTable[_pointerID].lastTimeEvent > localLimit.sepatateTime) {
				cleanElement(eventTable, _pointerID);
			} else if(    std::abs(eventTable[_pointerID].downStart.x() - _pos.x()) >= localLimit.DpiOffset
			           || std::abs(eventTable[_pointerID].downStart.y() - _pos.y()) >= localLimit.DpiOffset ){
				cleanElement(eventTable, _pointerID);
			}
		}
		if(eventTable[_pointerID].isUsed == true) {
			// save start time
			eventTable[_pointerID].lastTimeEvent = currentTime;
			// generate DOWN Event
			EVENT_DEBUG("GUI : Input ID=" << _pointerID
			            << " == >" << eventTable[_pointerID].destinationInputId
			            << " [DOWN]   " << _pos);
			eventTable[_pointerID].posEvent = _pos;
			localEventInput(_type,
			                eventTable[_pointerID].curentWidgetEvent.lock(),
			                eventTable[_pointerID].destinationInputId,
			                gale::key::status::down,
			                _pos);
		} else {
			// Mark it used :
			eventTable[_pointerID].isUsed = true;
			// Save current position :
			eventTable[_pointerID].downStart = _pos;
			// save start time
			eventTable[_pointerID].lastTimeEvent = currentTime;
			// set the element inside ...
			eventTable[_pointerID].isInside = true;
			ewol::WidgetShared tmpWidget = m_grabWidget.lock();
			// get destination widget :
			if(tmpWindows != nullptr) {
				if (    tmpWidget != nullptr
				     && _type == gale::key::type::mouse) {
					eventTable[_pointerID].curentWidgetEvent = tmpWidget;
				} else {
					tmpWidget = tmpWindows->getWidgetAtPos(_pos);
					eventTable[_pointerID].curentWidgetEvent = tmpWidget;
					if (tmpWidget != nullptr) {
						EVENT_DEBUG("Get widget at pos=" << _pos << " type: " << tmpWidget->getObjectType());
					} else {
						EVENT_DEBUG("Get widget at pos=" << _pos << " NO WIDGET");
					}
				}
			} else {
				eventTable[_pointerID].curentWidgetEvent.reset();
			}
			tmpWidget = eventTable[_pointerID].curentWidgetEvent.lock();
			if (tmpWidget != nullptr) {
				eventTable[_pointerID].origin = tmpWidget->getOrigin();
				eventTable[_pointerID].size = tmpWidget->getSize();
				eventTable[_pointerID].destinationInputId = localGetDestinationId(_type, tmpWidget, _pointerID);
			} else {
				eventTable[_pointerID].destinationInputId = -1;
			}
			// generate DOWN Event
			EVENT_DEBUG("GUI : Input ID=" << _pointerID
			            << " == >" << eventTable[_pointerID].destinationInputId
			            << " [DOWN]   " << _pos);
			eventTable[_pointerID].posEvent = _pos;
			localEventInput(_type,
			                tmpWidget,
			                eventTable[_pointerID].destinationInputId,
			                gale::key::status::down,
			                _pos);
		}
	} else {
		EVENT_DEBUG("GUI : Input ID=" << _pointerID
		             << " == >" << eventTable[_pointerID].destinationInputId
		             << " [UP]     " << _pos);
		ewol::WidgetShared tmpWidget = eventTable[_pointerID].curentWidgetEvent.lock();
		if(eventTable[_pointerID].isUsed == false) {
			// bad case ... ???
			EWOL_DEBUG("Up event without previous down ... ");
			// Mark it un-used :
			eventTable[_pointerID].isUsed = false;
			// revove the widget ...
			eventTable[_pointerID].curentWidgetEvent.reset();
		} else if (tmpWidget == nullptr) {
			// The widget has been removed:
			EVENT_DEBUG("    Object Removed ...");
			// Mark it un-used :
			eventTable[_pointerID].isUsed = false;
			// revove the widget ...
			eventTable[_pointerID].curentWidgetEvent.reset();
		} else {
			// generate UP Event
			EVENT_DEBUG("GUI : Input ID=" << _pointerID
			            << " == >" << eventTable[_pointerID].destinationInputId
			            << " [UP]     " << _pos);
			eventTable[_pointerID].posEvent = _pos;
			// send up event after the single event to prevent multiple widget getting elements
			localEventInput(_type,
			                tmpWidget,
			                _pointerID,
			                gale::key::status::up,
			                _pos);
			// generate event (single)
			if(    std::abs(eventTable[_pointerID].downStart.x() - _pos.x()) < localLimit.DpiOffset
			    && std::abs(eventTable[_pointerID].downStart.y() - _pos.y()) < localLimit.DpiOffset ){
				// Save current position :
				eventTable[_pointerID].downStart = _pos;
				// save start time
				eventTable[_pointerID].lastTimeEvent = currentTime;
				int32_t nbClickMax = 0;
				if(tmpWidget != nullptr) {
					nbClickMax = tmpWidget->getMouseLimit();
					if (nbClickMax>5) {
						nbClickMax = 5;
					}
				}
				// in grab mode the single to quinte event are not generated ....
				if(    (    m_grabWidget.lock() == nullptr
				         || _type != gale::key::type::mouse )
				    && eventTable[_pointerID].nbClickEvent < nbClickMax) {
					// generate event SINGLE :
					eventTable[_pointerID].nbClickEvent++;
					EVENT_DEBUG("GUI : Input ID=" << _pointerID
					            << " == >" << eventTable[_pointerID].destinationInputId
					            << " [" << eventTable[_pointerID].nbClickEvent << "] " << _pos);
					eventTable[_pointerID].posEvent = _pos;
					localEventInput(_type,
					                tmpWidget,
					                eventTable[_pointerID].destinationInputId,
					                (enum gale::key::status)(uint32_t(gale::key::status::pressSingle) + eventTable[_pointerID].nbClickEvent-1),
					                _pos);
					if( eventTable[_pointerID].nbClickEvent >= nbClickMax) {
						eventTable[_pointerID].nbClickEvent = 0;
					}
				} else {
					eventTable[_pointerID].nbClickEvent = 0;
				}
			}
			// send up event after the single event to prevent multiple widget getting elements
			localEventInput(_type,
			                tmpWidget,
			                _pointerID,
			                gale::key::status::upAfter,
			                _pos);
			// specific for tuch event
			if (_type == gale::key::type::finger) {
				cleanElement(eventTable, _pointerID);
			}
		}
	}
}


