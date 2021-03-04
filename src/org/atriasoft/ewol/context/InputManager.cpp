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

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::context::InputManager);

#define EVENT_DEBUG  EWOL_VERBOSE
//#define EVENT_DEBUG  EWOL_DEBUG

void ewol::context::InputManager::calculateLimit() {
	this.eventInputLimit.sepatateTime = echrono::Duration(echrono::milliseconds(300));
	this.eventInputLimit.DpiOffset = this.dpi*100;
	this.eventMouseLimit.sepatateTime = echrono::Duration(echrono::milliseconds(300));
	this.eventMouseLimit.DpiOffset = float(this.dpi)*0.1f;
}

void ewol::context::InputManager::setDpi(int newDPI) {
	this.dpi = newDPI;
	// recalculate the DPI system ...
	calculateLimit();
}

boolean ewol::context::InputManager::localEventInput(KeyType _type,
                                                  Widget _destWidget,
                                                  int _IdInput,
                                                  KeyStatus _status,
                                                  Vector2f _pos) {
	if (_destWidget != null) {
		if (    _type == KeyType::mouse
		     || _type == KeyType::finger) {
			// create the system Event :
			ewol::event::InputSystem tmpEventSystem(_type, _status, _IdInput, _pos, _destWidget, 0, this.specialKey); // TODO : set the real ID ...
			// generate the event :
			return _destWidget.systemEventInput(tmpEventSystem);
		} else {
			return false;
		}
	}
	return false;
}

void ewol::context::InputManager::abortElement(InputPoperty *_eventTable,
                                               int _idInput,
                                               KeyType _type) {
	if (_eventTable == null) {
		return;
	}
	if (_eventTable[_idInput].isUsed == true) {
		localEventInput(_type, 
		                _eventTable[_idInput].curentWidgetEvent.lock(),
		                _eventTable[_idInput].destinationInputId,
		                KeyStatus::abort,
		                _eventTable[_idInput].posEvent);
	}
}

void ewol::context::InputManager::cleanElement(InputPoperty *_eventTable,
                                               int _idInput) {
	if (_eventTable == null) {
		return;
	}
	//Log.info("CleanElement[" + idInput + "] = @" + (long)eventTable);
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

void ewol::context::InputManager::transfertEvent(Widget _source, Widget _destination) {
	if(    _source == null
	    || _destination == null) {
		// prevent errors ...
		return;
	}
	for(int iii=0; iii<MAX_MANAGE_INPUT; iii++) {
		Widget tmpWidget = this.eventInputSaved[iii].curentWidgetEvent.lock();
		if (tmpWidget == _source) {
			// inform the widget that it does not receive the event now
			EVENT_DEBUG("GUI : Input ID=" + iii + " == >" + this.eventInputSaved[iii].destinationInputId + " [EVENT_INPUT_TYPE_ABORT] " + this.eventInputSaved[iii].posEvent);
			localEventInput(KeyType::finger, tmpWidget, this.eventInputSaved[iii].destinationInputId, KeyStatus::abort, this.eventInputSaved[iii].posEvent);
			// set the new widget ...
			this.eventInputSaved[iii].curentWidgetEvent = _destination;
			// inform the widget that he receive the event property now...
			EVENT_DEBUG("GUI : Input ID=" + iii + " == >" + this.eventInputSaved[iii].destinationInputId + " [EVENT_INPUT_TYPE_TRANSFERT] " + this.eventInputSaved[iii].posEvent);
			localEventInput(KeyType::finger, _destination, this.eventInputSaved[iii].destinationInputId, KeyStatus::transfert, this.eventInputSaved[iii].posEvent);
		}
		tmpWidget = this.eventMouseSaved[iii].curentWidgetEvent.lock();
		if (tmpWidget == _source) {
			// inform the widget that it does not receive the event now
			EVENT_DEBUG("GUI : Input ID=" + iii + " == >" + this.eventMouseSaved[iii].destinationInputId + " [EVENT_INPUT_TYPE_ABORT] " + this.eventMouseSaved[iii].posEvent);
			localEventInput(KeyType::mouse, tmpWidget, this.eventMouseSaved[iii].destinationInputId, KeyStatus::abort, this.eventMouseSaved[iii].posEvent);
			// set the new widget ...
			this.eventMouseSaved[iii].curentWidgetEvent = _destination;
			// inform the widget that he receive the event property now...
			EVENT_DEBUG("GUI : Input ID=" + iii + " == >" + this.eventMouseSaved[iii].destinationInputId + " [EVENT_INPUT_TYPE_TRANSFERT] " + this.eventMouseSaved[iii].posEvent);
			localEventInput(KeyType::mouse, _destination, this.eventMouseSaved[iii].destinationInputId, KeyStatus::transfert, this.eventMouseSaved[iii].posEvent);
		}
	}
}

void ewol::context::InputManager::grabPointer(Widget _widget) {
	if(_widget == null) {
		return;
	}
	this.grabWidget = _widget;
	/* TODO : 
	this.context.grabPointerEvents(true,   _widget.getOrigin()
	                                  + Vector2i(_widget.getSize().x()/2.0f,
	                                          _widget.getSize().y()/2.0f) );
	*/
}

void ewol::context::InputManager::unGrabPointer() {
	this.grabWidget.reset();
	// TODO: this.context.grabPointerEvents(false, Vector2f(0,0));
}

void ewol::context::InputManager::newLayerSet() {
	for(int iii=0; iii<MAX_MANAGE_INPUT; iii++) {
		// remove the property of this input ...
		abortElement(this.eventInputSaved, iii, KeyType::finger);
		cleanElement(this.eventInputSaved, iii);
		abortElement(this.eventMouseSaved, iii, KeyType::mouse);
		cleanElement(this.eventMouseSaved, iii);
	}
}

ewol::context::InputManager::InputManager(EwolContext _context) :
  this.grabWidget(),
  this.context(_context) {
	setDpi(200);
	Log.info("Init (start)");
	for(int iii=0; iii<MAX_MANAGE_INPUT; iii++) {
		// remove the property of this input ...
		cleanElement(this.eventInputSaved, iii);
		cleanElement(this.eventMouseSaved, iii);
	}
	Log.info("Init (end)");
}

ewol::context::InputManager::~InputManager() {
	Log.info("Un-Init (start)");
	Log.info("Un-Init (end)");
}

int ewol::context::InputManager::localGetDestinationId(KeyType _type,
                                                           Widget _destWidget,
                                                           int _realInputId) {
	if (_type == KeyType::finger) {
		int lastMinimum = 0;
		for(int iii=0; iii<MAX_MANAGE_INPUT; iii++) {
			if (true == this.eventInputSaved[iii].isUsed) {
				Widget tmpWidget = this.eventInputSaved[iii].curentWidgetEvent.lock();
				if (tmpWidget == _destWidget) {
					if (iii != _realInputId) {
						lastMinimum = etk::max(lastMinimum, this.eventInputSaved[iii].destinationInputId);
					}
				}
			}
		}
		return lastMinimum+1;
	}
	return _realInputId;
}

// note if id<0  == > the it was finger event ...
void ewol::context::InputManager::motion(KeyType _type,
                                         int _pointerID,
                                         Vector2f _pos) {
	EVENT_DEBUG("motion event : " + _type + " " + _pointerID + " " + _pos);
	if (MAX_MANAGE_INPUT <= _pointerID) {
		// reject pointer  == > out of IDs...
		return;
	}
	InputPoperty *eventTable = null;
	if (_type == KeyType::mouse) {
		eventTable = this.eventMouseSaved;
	} else if (_type == KeyType::finger) {
		eventTable = this.eventInputSaved;
	} else {
		Log.error("Unknown type of event");
		return;
	}
	if(    _pointerID > MAX_MANAGE_INPUT
	    || _pointerID < 0) {
		// not manage input
		return;
	}
	ewol::widget::Windows tmpWindows = this.context.getWindows();
	// special case for the mouse event 0 that represent the hover event of the system :
	if (    _type == KeyType::mouse
	     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _pointerID == 0) {
		// this event is all time on the good widget ... and manage the enter and leave ...
		// NOTE : the "layer widget" force us to get the widget at the specific position all the time :
		Widget tmpWidget;
		if (this.grabWidget.lock() != null) {
			// grab all events ...
			tmpWidget = this.grabWidget.lock();
		} else {
			if (tmpWindows != null) {
				tmpWidget = tmpWindows.getWidgetAtPos(_pos);
			}
		}
		if(    tmpWidget != eventTable[_pointerID].curentWidgetEvent.lock()
		    || (    eventTable[_pointerID].isInside == true
		         LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (     eventTable[_pointerID].origin.x() > _pos.x()
		              ||  eventTable[_pointerID].origin.y() > _pos.y()
		              || (eventTable[_pointerID].origin.x() + eventTable[_pointerID].size.x()) < _pos.x()
		              || (eventTable[_pointerID].origin.y() + eventTable[_pointerID].size.y()) < _pos.y()) ) ) {
			eventTable[_pointerID].isInside = false;
			EVENT_DEBUG("GUI : Input ID=" + _pointerID + " == >" + eventTable[_pointerID].destinationInputId + " [LEAVE] " + _pos);
			eventTable[_pointerID].posEvent = _pos;
			localEventInput(_type,
			                eventTable[_pointerID].curentWidgetEvent.lock(),
			                eventTable[_pointerID].destinationInputId,
			                KeyStatus::leave,
			                _pos);
		}
		if (eventTable[_pointerID].isInside == false) {
			// set the element inside ...
			eventTable[_pointerID].isInside = true;
			// get destination widget :
			eventTable[_pointerID].curentWidgetEvent = tmpWidget;
			if (tmpWidget == null) {
				eventTable[_pointerID].isInside = false;
			} else {
				eventTable[_pointerID].origin = tmpWidget.getOrigin();
				eventTable[_pointerID].size = tmpWidget.getSize();
			}
			eventTable[_pointerID].destinationInputId = 0;
			EVENT_DEBUG("GUI : Input ID=" + _pointerID
			            + " == >" + eventTable[_pointerID].destinationInputId
			            + " [ENTER] " + _pos);
			eventTable[_pointerID].posEvent = _pos;
			localEventInput(_type,
			                tmpWidget,
			                eventTable[_pointerID].destinationInputId,
			                KeyStatus::enter,
			                _pos);
		}
		EVENT_DEBUG("GUI : Input ID=" + _pointerID
		            + " == >" + eventTable[_pointerID].destinationInputId
		            + " [MOVE]  " + _pos);
		eventTable[_pointerID].posEvent = _pos;
		localEventInput(_type,
		                tmpWidget,
		                eventTable[_pointerID].destinationInputId,
		                KeyStatus::move,
		                _pos);
	} else if (eventTable[_pointerID].isUsed == true) {
		if (eventTable[_pointerID].isInside == true) {
			if(     eventTable[_pointerID].origin.x() > _pos.x()
			    ||  eventTable[_pointerID].origin.y() > _pos.y()
			    || (eventTable[_pointerID].origin.x() + eventTable[_pointerID].size.x()) < _pos.x()
			    || (eventTable[_pointerID].origin.y() + eventTable[_pointerID].size.y()) < _pos.y()) {
				eventTable[_pointerID].isInside = false;
				EVENT_DEBUG("GUI : Input ID=" + _pointerID
				            + " == >" + eventTable[_pointerID].destinationInputId
				            + " [LEAVE] " + _pos);
				eventTable[_pointerID].posEvent = _pos;
				localEventInput(_type,
				                eventTable[_pointerID].curentWidgetEvent.lock(),
				                eventTable[_pointerID].destinationInputId,
				                KeyStatus::leave,
				                _pos);
			}
		} else {
			if(    (     eventTable[_pointerID].origin.x() <= _pos.x()
			         LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (eventTable[_pointerID].origin.x() + eventTable[_pointerID].size.x()) >= _pos.x() )
			    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (     eventTable[_pointerID].origin.y() <= _pos.y()
			         LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (eventTable[_pointerID].origin.y() + eventTable[_pointerID].size.y()) >= _pos.y() ) ) {
				eventTable[_pointerID].isInside = true;
				EVENT_DEBUG("GUI : Input ID=" + _pointerID
				            + " == >" + eventTable[_pointerID].destinationInputId
				            + " [ENTER] " + _pos);
				eventTable[_pointerID].posEvent = _pos;
				localEventInput(_type,
				                eventTable[_pointerID].curentWidgetEvent.lock(),
				                eventTable[_pointerID].destinationInputId,
				                KeyStatus::enter,
				                _pos);
			}
		}
		EVENT_DEBUG("GUI : Input ID=" + _pointerID
		            + " == >" + eventTable[_pointerID].destinationInputId
		            + " [MOVE]  " + _pos);
		eventTable[_pointerID].posEvent = _pos;
		localEventInput(_type,
		                eventTable[_pointerID].curentWidgetEvent.lock(),
		                eventTable[_pointerID].destinationInputId,
		                KeyStatus::move,
		                _pos);
	}
}

void ewol::context::InputManager::state(KeyType _type,
                                        int _pointerID,
                                        boolean _isDown,
                                        Vector2f _pos) {
	if (_pointerID >= MAX_MANAGE_INPUT) {
		// reject pointer  == > out of IDs...
		return;
	}
	EVENT_DEBUG("event pointerId=" + _pointerID);
	// convert position in open-GL coordonates ...
	InputPoperty *eventTable = null;
	InputLimit   localLimit;
	if (_type == KeyType::mouse) {
		eventTable = this.eventMouseSaved;
		localLimit = this.eventMouseLimit;
	} else if (_type == KeyType::finger) {
		eventTable = this.eventInputSaved;
		localLimit = this.eventInputLimit;
	} else {
		Log.error("Unknown type of event");
		return;
	}
	if(    _pointerID > MAX_MANAGE_INPUT
	    || _pointerID <= 0) {
		// not manage input
		return;
	}
	// get the curent time ...
	echrono::Clock currentTime = echrono::Clock::now();
	ewol::widget::Windows tmpWindows = this.context.getWindows();
	
	if (_isDown == true) {
		EVENT_DEBUG("GUI : Input ID=" + _pointerID
		             + " == >" + eventTable[_pointerID].destinationInputId
		             + " [DOWN] " + _pos);
		if(eventTable[_pointerID].isUsed == true) {
			// we have an event previously ... check delay between click and offset position
			if (currentTime - eventTable[_pointerID].lastTimeEvent > localLimit.sepatateTime) {
				cleanElement(eventTable, _pointerID);
			} else if(    etk::abs(eventTable[_pointerID].downStart.x() - _pos.x()) >= localLimit.DpiOffset
			           || etk::abs(eventTable[_pointerID].downStart.y() - _pos.y()) >= localLimit.DpiOffset ){
				cleanElement(eventTable, _pointerID);
			}
		}
		if(eventTable[_pointerID].isUsed == true) {
			// save start time
			eventTable[_pointerID].lastTimeEvent = currentTime;
			// generate DOWN Event
			EVENT_DEBUG("GUI : Input ID=" + _pointerID
			            + " == >" + eventTable[_pointerID].destinationInputId
			            + " [DOWN]   " + _pos);
			eventTable[_pointerID].posEvent = _pos;
			localEventInput(_type,
			                eventTable[_pointerID].curentWidgetEvent.lock(),
			                eventTable[_pointerID].destinationInputId,
			                KeyStatus::down,
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
			Widget tmpWidget = this.grabWidget.lock();
			// get destination widget :
			if(tmpWindows != null) {
				if (    tmpWidget != null
				     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _type == KeyType::mouse) {
					eventTable[_pointerID].curentWidgetEvent = tmpWidget;
				} else {
					tmpWidget = tmpWindows.getWidgetAtPos(_pos);
					eventTable[_pointerID].curentWidgetEvent = tmpWidget;
					if (tmpWidget != null) {
						EVENT_DEBUG("Get widget at pos=" + _pos + " type: " + tmpWidget.getObjectType());
					} else {
						EVENT_DEBUG("Get widget at pos=" + _pos + " NO WIDGET");
					}
				}
			} else {
				eventTable[_pointerID].curentWidgetEvent.reset();
			}
			tmpWidget = eventTable[_pointerID].curentWidgetEvent.lock();
			if (tmpWidget != null) {
				eventTable[_pointerID].origin = tmpWidget.getOrigin();
				eventTable[_pointerID].size = tmpWidget.getSize();
				eventTable[_pointerID].destinationInputId = localGetDestinationId(_type, tmpWidget, _pointerID);
			} else {
				eventTable[_pointerID].destinationInputId = -1;
			}
			// generate DOWN Event
			EVENT_DEBUG("GUI : Input ID=" + _pointerID
			            + " == >" + eventTable[_pointerID].destinationInputId
			            + " [DOWN]   " + _pos);
			eventTable[_pointerID].posEvent = _pos;
			localEventInput(_type,
			                tmpWidget,
			                eventTable[_pointerID].destinationInputId,
			                KeyStatus::down,
			                _pos);
		}
	} else {
		EVENT_DEBUG("GUI : Input ID=" + _pointerID
		             + " == >" + eventTable[_pointerID].destinationInputId
		             + " [UP]     " + _pos);
		Widget tmpWidget = eventTable[_pointerID].curentWidgetEvent.lock();
		if(eventTable[_pointerID].isUsed == false) {
			// bad case ... ???
			Log.debug("Up event without previous down ... ");
			// Mark it un-used :
			eventTable[_pointerID].isUsed = false;
			// revove the widget ...
			eventTable[_pointerID].curentWidgetEvent.reset();
		} else if (tmpWidget == null) {
			// The widget has been removed:
			EVENT_DEBUG("    Object Removed ...");
			// Mark it un-used :
			eventTable[_pointerID].isUsed = false;
			// revove the widget ...
			eventTable[_pointerID].curentWidgetEvent.reset();
		} else {
			// generate UP Event
			EVENT_DEBUG("GUI : Input ID=" + _pointerID
			            + " == >" + eventTable[_pointerID].destinationInputId
			            + " [UP]     " + _pos);
			eventTable[_pointerID].posEvent = _pos;
			// send up event after the single event to prevent multiple widget getting elements
			localEventInput(_type,
			                tmpWidget,
			                _pointerID,
			                KeyStatus::up,
			                _pos);
			// generate event (single)
			if(    etk::abs(eventTable[_pointerID].downStart.x() - _pos.x()) < localLimit.DpiOffset
			    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM etk::abs(eventTable[_pointerID].downStart.y() - _pos.y()) < localLimit.DpiOffset ){
				// Save current position :
				eventTable[_pointerID].downStart = _pos;
				// save start time
				eventTable[_pointerID].lastTimeEvent = currentTime;
				int nbClickMax = 0;
				if(tmpWidget != null) {
					nbClickMax = tmpWidget.getMouseLimit();
					if (nbClickMax>5) {
						nbClickMax = 5;
					}
				}
				// in grab mode the single to quinte event are not generated ....
				if(    (    this.grabWidget.lock() == null
				         || _type != KeyType::mouse )
				    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM eventTable[_pointerID].nbClickEvent < nbClickMax) {
					// generate event SINGLE :
					eventTable[_pointerID].nbClickEvent++;
					EVENT_DEBUG("GUI : Input ID=" + _pointerID
					            + " == >" + eventTable[_pointerID].destinationInputId
					            + " [" + eventTable[_pointerID].nbClickEvent + "] " + _pos);
					eventTable[_pointerID].posEvent = _pos;
					localEventInput(_type,
					                tmpWidget,
					                eventTable[_pointerID].destinationInputId,
					                (KeyStatus)(uint(KeyStatus::pressSingle) + eventTable[_pointerID].nbClickEvent-1),
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
			                KeyStatus::upAfter,
			                _pos);
			// specific for tuch event
			if (_type == KeyType::finger) {
				cleanElement(eventTable, _pointerID);
			}
		}
	}
}


