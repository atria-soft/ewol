/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>
#include <etk/UString.h>

#include <ewol/debug.h>

#include <ewol/ewol.h>

#include <ewol/renderer/EObject.h>
#include <ewol/renderer/EObjectManager.h>
#include <ewol/renderer/eContext.h>
#include <ewol/renderer/eInput.h>
#include <ewol/resources/Texture.h>

#include <ewol/widget/Widget.h>
#include <ewol/widget/Windows.h>
#include <ewol/widget/WidgetManager.h>



#define EVENT_DEBUG  EWOL_VERBOSE
//#define EVENT_DEBUG  EWOL_DEBUG

void ewol::eInput::calculateLimit(void) {
	m_eventInputLimit.sepatateTime = 300000; // �s
	m_eventInputLimit.DpiOffset = m_dpi*100;
	m_eventMouseLimit.sepatateTime = 300000; // �s
	m_eventMouseLimit.DpiOffset = (float)m_dpi*(float)0.1;
}

void ewol::eInput::setDpi(int32_t newDPI) {
	m_dpi = newDPI;
	// recalculate the DPI system ...
	calculateLimit();
}

bool ewol::eInput::localEventInput(ewol::keyEvent::type_te _type,
                                         ewol::Widget* _destWidget,
                                         int32_t _IdInput,
                                         ewol::keyEvent::status_te _status,
                                         vec2 _pos) {
	if (NULL != _destWidget) {
		if (_type == ewol::keyEvent::typeMouse || _type == ewol::keyEvent::typeFinger) {
			// create the system Event :
			ewol::EventInputSystem tmpEventSystem(_type, _status, _IdInput, _pos, _destWidget, 0); // TODO : set the real ID ...
			// generate the event :
			return _destWidget->systemEventInput(tmpEventSystem);
		} else {
			return false;
		}
	}
	return false;
}

void ewol::eInput::abortElement(InputPoperty_ts *_eventTable,
                                int32_t _idInput,
                                ewol::keyEvent::type_te _type) {
	if (NULL == _eventTable) {
		return;
	}
	if (_eventTable[_idInput].isUsed == true) {
		localEventInput(_type, 
		                _eventTable[_idInput].curentWidgetEvent,
		                _eventTable[_idInput].destinationInputId,
		                ewol::keyEvent::statusAbort,
		                _eventTable[_idInput].posEvent);
	}
}

void ewol::eInput::cleanElement(InputPoperty_ts *_eventTable,
                                int32_t _idInput) {
	if (NULL == _eventTable) {
		return;
	}
	//EWOL_INFO("CleanElement[" << idInput << "] = @" << (int64_t)eventTable);
	_eventTable[_idInput].isUsed = false;
	_eventTable[_idInput].destinationInputId = 0;
	_eventTable[_idInput].lastTimeEvent = 0;
	_eventTable[_idInput].curentWidgetEvent = NULL;
	_eventTable[_idInput].origin.setValue(0,0);
	_eventTable[_idInput].size.setValue(99999999,99999999);
	_eventTable[_idInput].downStart.setValue(0,0);
	_eventTable[_idInput].isDown = false;
	_eventTable[_idInput].isInside = false;
	_eventTable[_idInput].nbClickEvent = 0;
	_eventTable[_idInput].posEvent.setValue(0,0);
}

void ewol::eInput::transfertEvent(ewol::Widget* _source, ewol::Widget* _destination) {
	if(    NULL == _source
	    || NULL == _destination) {
		// prevent errors ...
		return;
	}
	for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
		if (m_eventInputSaved[iii].curentWidgetEvent == _source) {
			// inform the widget that it does not receive the event now
			EVENT_DEBUG("GUI : Input ID=" << iii << " == >" << m_eventInputSaved[iii].destinationInputId << " [EVENT_INPUT_TYPE_ABORT] " << m_eventInputSaved[iii].posEvent);
			localEventInput(ewol::keyEvent::typeFinger, m_eventInputSaved[iii].curentWidgetEvent, m_eventInputSaved[iii].destinationInputId, ewol::keyEvent::statusAbort, m_eventInputSaved[iii].posEvent);
			// set the new widget ...
			m_eventInputSaved[iii].curentWidgetEvent = _destination;
			// inform the widget that he receive the event property now...
			EVENT_DEBUG("GUI : Input ID=" << iii << " == >" << m_eventInputSaved[iii].destinationInputId << " [EVENT_INPUT_TYPE_TRANSFERT] " << m_eventInputSaved[iii].posEvent);
			localEventInput(ewol::keyEvent::typeFinger, m_eventInputSaved[iii].curentWidgetEvent, m_eventInputSaved[iii].destinationInputId, ewol::keyEvent::statusTransfert, m_eventInputSaved[iii].posEvent);
		}
		if (m_eventMouseSaved[iii].curentWidgetEvent == _source) {
			// inform the widget that it does not receive the event now
			EVENT_DEBUG("GUI : Input ID=" << iii << " == >" << m_eventMouseSaved[iii].destinationInputId << " [EVENT_INPUT_TYPE_ABORT] " << m_eventMouseSaved[iii].posEvent);
			localEventInput(ewol::keyEvent::typeMouse, m_eventMouseSaved[iii].curentWidgetEvent, m_eventMouseSaved[iii].destinationInputId, ewol::keyEvent::statusAbort, m_eventMouseSaved[iii].posEvent);
			// set the new widget ...
			m_eventMouseSaved[iii].curentWidgetEvent = _destination;
			// inform the widget that he receive the event property now...
			EVENT_DEBUG("GUI : Input ID=" << iii << " == >" << m_eventMouseSaved[iii].destinationInputId << " [EVENT_INPUT_TYPE_TRANSFERT] " << m_eventMouseSaved[iii].posEvent);
			localEventInput(ewol::keyEvent::typeMouse, m_eventMouseSaved[iii].curentWidgetEvent, m_eventMouseSaved[iii].destinationInputId, ewol::keyEvent::statusTransfert, m_eventMouseSaved[iii].posEvent);
		}
	}
}

void ewol::eInput::grabPointer(ewol::Widget* _widget) {
	if(NULL == _widget) {
		return;
	}
	m_grabWidget = _widget;
	m_context.grabPointerEvents(true,   _widget->getOrigin()
	                                  + ivec2(_widget->getSize().x()/2.0f,
	                                          _widget->getSize().y()/2.0f) );
}

void ewol::eInput::unGrabPointer(void) {
	m_grabWidget = NULL;
	m_context.grabPointerEvents(false, vec2(0,0));
}

void ewol::eInput::onObjectRemove(ewol::EObject * removeObject) {
	for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
		if (m_eventInputSaved[iii].curentWidgetEvent == removeObject) {
			// remove the property of this input ...
			cleanElement(m_eventInputSaved, iii);
		}
		if (m_eventMouseSaved[iii].curentWidgetEvent == removeObject) {
			// remove the property of this input ...
			cleanElement(m_eventMouseSaved, iii);
		}
	}
}

void ewol::eInput::newLayerSet(void) {
	for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
		// remove the property of this input ...
		abortElement(m_eventInputSaved, iii, ewol::keyEvent::typeFinger);
		cleanElement(m_eventInputSaved, iii);
		abortElement(m_eventMouseSaved, iii, ewol::keyEvent::typeMouse);
		cleanElement(m_eventMouseSaved, iii);
	}
}

ewol::eInput::eInput(ewol::eContext& _context) :
  m_grabWidget(NULL),
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

ewol::eInput::~eInput(void) {
	EWOL_INFO("Un-Init (start)");
	EWOL_INFO("Un-Init (end)");
}

int32_t ewol::eInput::localGetDestinationId(ewol::keyEvent::type_te _type,
                                            ewol::Widget* _destWidget,
                                            int32_t _realInputId) {
	if (_type == ewol::keyEvent::typeFinger) {
		int32_t lastMinimum = 0;
		for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
			if (true == m_eventInputSaved[iii].isUsed) {
				if (m_eventInputSaved[iii].curentWidgetEvent == _destWidget) {
					if (iii != _realInputId) {
						lastMinimum = etk_max(lastMinimum, m_eventInputSaved[iii].destinationInputId);
					}
				}
			}
		}
		return lastMinimum+1;
	}
	return _realInputId;
}

// note if id<0  == > the it was finger event ...
void ewol::eInput::motion(ewol::keyEvent::type_te _type,
                          int _pointerID,
                          vec2 _pos) {
	EVENT_DEBUG("motion event : " << _type << " " << _pointerID << " " << _pos);
	if (MAX_MANAGE_INPUT <= _pointerID) {
		// reject pointer  == > out of IDs...
		return;
	}
	InputPoperty_ts *eventTable = NULL;
	if (_type == ewol::keyEvent::typeMouse) {
		eventTable = m_eventMouseSaved;
	} else if (_type == ewol::keyEvent::typeFinger) {
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
	ewol::Windows* tmpWindows = m_context.getWindows();
	// special case for the mouse event 0 that represent the hover event of the system :
	if (_type == ewol::keyEvent::typeMouse && _pointerID == 0) {
		// this event is all time on the good widget ... and manage the enter and leave ...
		// NOTE : the "layer widget" force us to get the widget at the specific position all the time :
		ewol::Widget* tmpWidget = NULL;
		if (m_grabWidget != NULL) {
			// grab all events ...
			tmpWidget = m_grabWidget;
		} else {
			if (NULL != tmpWindows) {
				tmpWidget = tmpWindows->getWidgetAtPos(_pos);
			}
		}
		if(    tmpWidget != eventTable[_pointerID].curentWidgetEvent
		    || (    true == eventTable[_pointerID].isInside
		         && (     eventTable[_pointerID].origin.x() > _pos.x()
		              ||  eventTable[_pointerID].origin.y() > _pos.y()
		              || (eventTable[_pointerID].origin.x() + eventTable[_pointerID].size.x()) < _pos.x()
		              || (eventTable[_pointerID].origin.y() + eventTable[_pointerID].size.y()) < _pos.y()) ) ) {
			eventTable[_pointerID].isInside = false;
			EVENT_DEBUG("GUI : Input ID=" << _pointerID << " == >" << eventTable[_pointerID].destinationInputId << " [LEAVE] " << _pos);
			eventTable[_pointerID].posEvent = _pos;
			localEventInput(_type,
			                eventTable[_pointerID].curentWidgetEvent,
			                eventTable[_pointerID].destinationInputId,
			                ewol::keyEvent::statusLeave,
			                _pos);
		}
		if (false == eventTable[_pointerID].isInside) {
			// set the element inside ...
			eventTable[_pointerID].isInside = true;
			// get destination widget :
			eventTable[_pointerID].curentWidgetEvent = tmpWidget;
			if (NULL == eventTable[_pointerID].curentWidgetEvent) {
				eventTable[_pointerID].isInside = false;
			}
			if (NULL != eventTable[_pointerID].curentWidgetEvent) {
				eventTable[_pointerID].origin = eventTable[_pointerID].curentWidgetEvent->getOrigin();
				eventTable[_pointerID].size = eventTable[_pointerID].curentWidgetEvent->getSize();
			}
			eventTable[_pointerID].destinationInputId = 0;
			EVENT_DEBUG("GUI : Input ID=" << _pointerID
			            << " == >" << eventTable[_pointerID].destinationInputId
			            << " [ENTER] " << _pos);
			eventTable[_pointerID].posEvent = _pos;
			localEventInput(_type,
			                eventTable[_pointerID].curentWidgetEvent,
			                eventTable[_pointerID].destinationInputId,
			                ewol::keyEvent::statusEnter,
			                _pos);
		}
		EVENT_DEBUG("GUI : Input ID=" << _pointerID
		            << " == >" << eventTable[_pointerID].destinationInputId
		            << " [MOVE]  " << _pos);
		eventTable[_pointerID].posEvent = _pos;
		localEventInput(_type,
		                eventTable[_pointerID].curentWidgetEvent,
		                eventTable[_pointerID].destinationInputId,
		                ewol::keyEvent::statusMove,
		                _pos);
	} else if (true == eventTable[_pointerID].isUsed) {
		if (true == eventTable[_pointerID].isInside) {
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
				                eventTable[_pointerID].curentWidgetEvent,
				                eventTable[_pointerID].destinationInputId,
				                ewol::keyEvent::statusLeave,
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
				                eventTable[_pointerID].curentWidgetEvent,
				                eventTable[_pointerID].destinationInputId,
				                ewol::keyEvent::statusEnter,
				                _pos);
			}
		}
		EVENT_DEBUG("GUI : Input ID=" << _pointerID
		            << " == >" << eventTable[_pointerID].destinationInputId
		            << " [MOVE]  " << _pos);
		eventTable[_pointerID].posEvent = _pos;
		localEventInput(_type,
		                eventTable[_pointerID].curentWidgetEvent,
		                eventTable[_pointerID].destinationInputId,
		                ewol::keyEvent::statusMove,
		                _pos);
	}
}

void ewol::eInput::state(ewol::keyEvent::type_te _type,
                         int _pointerID,
                         bool _isDown,
                         vec2 _pos)
{
	if (MAX_MANAGE_INPUT <= _pointerID) {
		// reject pointer  == > out of IDs...
		return;
	}
	EVENT_DEBUG("event pointerId=" << _pointerID);
	// convert position in open-GL coordonates ...
	InputPoperty_ts *eventTable = NULL;
	inputLimit_ts   localLimit;
	if (_type == ewol::keyEvent::typeMouse) {
		eventTable = m_eventMouseSaved;
		localLimit = m_eventMouseLimit;
	} else if (_type == ewol::keyEvent::typeFinger) {
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
	int64_t currentTime = ewol::getTime();
	ewol::Windows* tmpWindows = m_context.getWindows();
	
	if (true == _isDown) {
		EVENT_DEBUG("GUI : Input ID=" << _pointerID
		             << " == >" << eventTable[_pointerID].destinationInputId
		             << " [DOWN] " << _pos);
		if(true == eventTable[_pointerID].isUsed) {
			// we have an event previously ... check delay between click and offset position
			if (currentTime - eventTable[_pointerID].lastTimeEvent > localLimit.sepatateTime) {
				cleanElement(eventTable, _pointerID);
			} else if(    abs(eventTable[_pointerID].downStart.x() - _pos.x()) >= localLimit.DpiOffset
			           || abs(eventTable[_pointerID].downStart.y() - _pos.y()) >= localLimit.DpiOffset ){
				cleanElement(eventTable, _pointerID);
			}
		}
		if(true == eventTable[_pointerID].isUsed) {
			// save start time
			eventTable[_pointerID].lastTimeEvent = currentTime;
			// generate DOWN Event
			EVENT_DEBUG("GUI : Input ID=" << _pointerID
			            << " == >" << eventTable[_pointerID].destinationInputId
			            << " [DOWN]   " << _pos);
			eventTable[_pointerID].posEvent = _pos;
			localEventInput(_type,
			                eventTable[_pointerID].curentWidgetEvent,
			                eventTable[_pointerID].destinationInputId,
			                ewol::keyEvent::statusDown,
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
			// get destination widget :
			if(NULL != tmpWindows) {
				if (m_grabWidget != NULL && _type == ewol::keyEvent::typeMouse) {
					eventTable[_pointerID].curentWidgetEvent = m_grabWidget;
				} else {
					eventTable[_pointerID].curentWidgetEvent = tmpWindows->getWidgetAtPos(_pos);
				}
			} else {
				eventTable[_pointerID].curentWidgetEvent = NULL;
			}
			if (NULL != eventTable[_pointerID].curentWidgetEvent) {
				eventTable[_pointerID].origin = eventTable[_pointerID].curentWidgetEvent->getOrigin();
				eventTable[_pointerID].size = eventTable[_pointerID].curentWidgetEvent->getSize();
				eventTable[_pointerID].destinationInputId = localGetDestinationId(_type, eventTable[_pointerID].curentWidgetEvent, _pointerID);
			} else {
				eventTable[_pointerID].destinationInputId = -1;
			}
			// generate DOWN Event
			EVENT_DEBUG("GUI : Input ID=" << _pointerID
			            << " == >" << eventTable[_pointerID].destinationInputId
			            << " [DOWN]   " << _pos);
			eventTable[_pointerID].posEvent = _pos;
			localEventInput(_type,
			                eventTable[_pointerID].curentWidgetEvent,
			                eventTable[_pointerID].destinationInputId,
			                ewol::keyEvent::statusDown,
			                _pos);
		}
	} else {
		EVENT_DEBUG("GUI : Input ID=" << _pointerID
		             << " == >" << eventTable[_pointerID].destinationInputId
		             << " [UP]     " << _pos);
		if(false == eventTable[_pointerID].isUsed) {
			// bad case ... ???
			EWOL_DEBUG("Up event without previous down ... ");
			// Mark it un-used :
			eventTable[_pointerID].isUsed = false;
			// revove the widget ...
			eventTable[_pointerID].curentWidgetEvent = NULL;
		} else {
			// generate UP Event
			EVENT_DEBUG("GUI : Input ID=" << _pointerID
			            << " == >" << eventTable[_pointerID].destinationInputId
			            << " [UP]     " << _pos);
			eventTable[_pointerID].posEvent = _pos;
			localEventInput(_type,
			                eventTable[_pointerID].curentWidgetEvent,
			                _pointerID,
			                ewol::keyEvent::statusUp,
			                _pos);
			// generate event (single)
			if(    abs(eventTable[_pointerID].downStart.x() - _pos.x()) < localLimit.DpiOffset
			    && abs(eventTable[_pointerID].downStart.y() - _pos.y()) < localLimit.DpiOffset ){
				// Save current position :
				eventTable[_pointerID].downStart = _pos;
				// save start time
				eventTable[_pointerID].lastTimeEvent = currentTime;
				int32_t nbClickMax = 0;
				if(eventTable[_pointerID].curentWidgetEvent != NULL) {
					nbClickMax = eventTable[_pointerID].curentWidgetEvent->getMouseLimit();
					if (nbClickMax>5) {
						nbClickMax = 5;
					}
				}
				// in grab mode the single to quinte event are not generated ....
				if(    (    m_grabWidget == NULL
				         || _type != ewol::keyEvent::typeMouse )
				    && eventTable[_pointerID].nbClickEvent < nbClickMax) {
					// generate event SINGLE :
					eventTable[_pointerID].nbClickEvent++;
					EVENT_DEBUG("GUI : Input ID=" << _pointerID
					            << " == >" << eventTable[_pointerID].destinationInputId
					            << " [" << eventTable[_pointerID].nbClickEvent << "] " << _pos);
					eventTable[_pointerID].posEvent = _pos;
					localEventInput(_type,
					                eventTable[_pointerID].curentWidgetEvent,
					                eventTable[_pointerID].destinationInputId,
					                (ewol::keyEvent::status_te)(ewol::keyEvent::statusSingle + eventTable[_pointerID].nbClickEvent-1),
					                _pos);
					if( eventTable[_pointerID].nbClickEvent >= nbClickMax) {
						eventTable[_pointerID].nbClickEvent = 0;
					}
				} else {
					eventTable[_pointerID].nbClickEvent = 0;
				}
			}
			// specific for tuch event
			if (_type == ewol::keyEvent::typeFinger) {
				cleanElement(eventTable, _pointerID);
			}
		}
	}
}


