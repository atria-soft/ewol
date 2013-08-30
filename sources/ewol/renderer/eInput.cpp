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
#include <ewol/renderer/eSystem.h>
#include <ewol/renderer/eSystemInput.h>
#include <ewol/renderer/resources/Texture.h>

#include <ewol/widget/Widget.h>
#include <ewol/widget/Windows.h>
#include <ewol/widget/WidgetManager.h>



#define EVENT_DEBUG  EWOL_VERBOSE
//#define EVENT_DEBUG  EWOL_DEBUG

void ewol::eInput::CalculateLimit(void)
{
	m_eventInputLimit.sepatateTime = 300000; // µs
	m_eventInputLimit.DpiOffset = m_dpi*100;
	m_eventMouseLimit.sepatateTime = 300000; // µs
	m_eventMouseLimit.DpiOffset = (float)m_dpi*(float)0.1;
}

void ewol::eInput::SetDpi(int32_t newDPI)
{
	m_dpi = newDPI;
	// recalculate the DPI system ...
	CalculateLimit();
}

bool ewol::eInput::localEventInput(ewol::keyEvent::type_te _type,
                                         ewol::Widget* _destWidget,
                                         int32_t _IdInput,
                                         ewol::keyEvent::status_te _status,
                                         vec2 _pos)
{
	if (NULL != _destWidget) {
		if (_type == ewol::keyEvent::typeMouse || _type == ewol::keyEvent::typeFinger) {
			// create the system Event :
			ewol::EventInputSystem tmpEventSystem(_type, _status, _IdInput, _pos, _destWidget, 0); // TODO : set the real ID ...
			// generate the event :
			return _destWidget->SystemEventInput(tmpEventSystem);
		} else {
			return false;
		}
	}
	return false;
}

void ewol::eInput::AbortElement(InputPoperty_ts *_eventTable, int32_t _idInput, ewol::keyEvent::type_te _type)
{
	if (NULL==_eventTable) {
		return;
	}
	if (_eventTable[_idInput].isUsed==true) {
		localEventInput(_type, 
		                _eventTable[_idInput].curentWidgetEvent,
		                _eventTable[_idInput].destinationInputId,
		                ewol::keyEvent::statusAbort,
		                _eventTable[_idInput].posEvent);
	}
}

void ewol::eInput::CleanElement(InputPoperty_ts *_eventTable, int32_t _idInput)
{
	if (NULL==_eventTable) {
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



void ewol::eInput::TransfertEvent(ewol::Widget* source, ewol::Widget* destination)
{
	if(    NULL == source
	    || NULL == destination) {
		// prevent errors ...
		return;
	}
	for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
		if (m_eventInputSaved[iii].curentWidgetEvent == source) {
			// inform the widget that it does not receive the event now
			EVENT_DEBUG("GUI : Input ID=" << iii << "==>" << m_eventInputSaved[iii].destinationInputId << " [EVENT_INPUT_TYPE_ABORT] " << m_eventInputSaved[iii].posEvent);
			localEventInput(ewol::keyEvent::typeFinger, m_eventInputSaved[iii].curentWidgetEvent, m_eventInputSaved[iii].destinationInputId, ewol::keyEvent::statusAbort, m_eventInputSaved[iii].posEvent);
			// set the new widget ...
			m_eventInputSaved[iii].curentWidgetEvent = destination;
			// inform the widget that he receive the event property now...
			EVENT_DEBUG("GUI : Input ID=" << iii << "==>" << m_eventInputSaved[iii].destinationInputId << " [EVENT_INPUT_TYPE_TRANSFERT] " << m_eventInputSaved[iii].posEvent);
			localEventInput(ewol::keyEvent::typeFinger, m_eventInputSaved[iii].curentWidgetEvent, m_eventInputSaved[iii].destinationInputId, ewol::keyEvent::statusTransfert, m_eventInputSaved[iii].posEvent);
		}
		if (m_eventMouseSaved[iii].curentWidgetEvent == source) {
			// inform the widget that it does not receive the event now
			EVENT_DEBUG("GUI : Input ID=" << iii << "==>" << m_eventMouseSaved[iii].destinationInputId << " [EVENT_INPUT_TYPE_ABORT] " << m_eventMouseSaved[iii].posEvent);
			localEventInput(ewol::keyEvent::typeMouse, m_eventMouseSaved[iii].curentWidgetEvent, m_eventMouseSaved[iii].destinationInputId, ewol::keyEvent::statusAbort, m_eventMouseSaved[iii].posEvent);
			// set the new widget ...
			m_eventMouseSaved[iii].curentWidgetEvent = destination;
			// inform the widget that he receive the event property now...
			EVENT_DEBUG("GUI : Input ID=" << iii << "==>" << m_eventMouseSaved[iii].destinationInputId << " [EVENT_INPUT_TYPE_TRANSFERT] " << m_eventMouseSaved[iii].posEvent);
			localEventInput(ewol::keyEvent::typeMouse, m_eventMouseSaved[iii].curentWidgetEvent, m_eventMouseSaved[iii].destinationInputId, ewol::keyEvent::statusTransfert, m_eventMouseSaved[iii].posEvent);
		}
	}
}

void ewol::eInput::GrabPointer(ewol::Widget* widget)
{
	if(NULL==widget) {
		return;
	}
	m_grabWidget = widget;
	m_context.GrabPointerEvents(true, widget->GetOrigin() + ivec2(widget->GetSize().x()/2.0f, widget->GetSize().y()/2.0f) );
}

void ewol::eInput::UnGrabPointer(void)
{
	m_grabWidget = NULL;
	m_context.GrabPointerEvents(false, vec2(0,0));
}

void ewol::eInput::OnObjectRemove(ewol::EObject * removeObject)
{
	for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
		if (m_eventInputSaved[iii].curentWidgetEvent == removeObject) {
			// remove the property of this input ...
			CleanElement(m_eventInputSaved, iii);
		}
		if (m_eventMouseSaved[iii].curentWidgetEvent == removeObject) {
			// remove the property of this input ...
			CleanElement(m_eventMouseSaved, iii);
		}
	}
}

void ewol::eInput::NewLayerSet(void)
{
	for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
		// remove the property of this input ...
		AbortElement(m_eventInputSaved, iii, ewol::keyEvent::typeFinger);
		CleanElement(m_eventInputSaved, iii);
		AbortElement(m_eventMouseSaved, iii, ewol::keyEvent::typeMouse);
		CleanElement(m_eventMouseSaved, iii);
	}
}

ewol::eInput::eInput(ewol::eContext& _context) :
	m_grabWidget(NULL),
	m_context(_context)
{
	SetDpi(200);
	EWOL_INFO("Init (start)");
	for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
		// remove the property of this input ...
		CleanElement(m_eventInputSaved, iii);
		CleanElement(m_eventMouseSaved, iii);
	}
	EWOL_INFO("Init (end)");
}

ewol::eInput::~eInput(void)
{
	EWOL_INFO("Un-Init (start)");
	EWOL_INFO("Un-Init (end)");
}


int32_t ewol::eInput::localGetDestinationId(ewol::keyEvent::type_te _type, ewol::Widget* _destWidget, int32_t _realInputId)
{
	if (_type == ewol::keyEvent::typeFinger) {
		int32_t lastMinimum = 0;
		for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
			if (true==m_eventInputSaved[iii].isUsed) {
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

// note if id<0 ==> the it was finger event ...
void ewol::eInput::Motion(ewol::keyEvent::type_te type, int pointerID, vec2 pos)
{
	EVENT_DEBUG("motion event : " << type << " " << pointerID << " " << pos);
	if (MAX_MANAGE_INPUT<=pointerID) {
		// reject pointer ==> out of IDs...
		return;
	}
	InputPoperty_ts *eventTable = NULL;
	if (type == ewol::keyEvent::typeMouse) {
		eventTable = m_eventMouseSaved;
	} else if (type == ewol::keyEvent::typeFinger) {
		eventTable = m_eventInputSaved;
	} else {
		EWOL_ERROR("Unknown type of event");
		return;
	}
	if(    pointerID > MAX_MANAGE_INPUT
	    || pointerID < 0) {
		// not manage input
		return;
	}
	ewol::Windows* tmpWindows = m_context.GetWindows();
	// special case for the mouse event 0 that represent the hover event of the system :
	if (type == ewol::keyEvent::typeMouse && pointerID == 0) {
		// this event is all time on the good widget ... and manage the enter and leave ...
		// NOTE : the "layer widget" force us to get the widget at the specific position all the time :
		ewol::Widget* tmpWidget = NULL;
		if (m_grabWidget != NULL) {
			// grab all events ...
			tmpWidget = m_grabWidget;
		} else {
			if (NULL != tmpWindows) {
				tmpWidget = tmpWindows->GetWidgetAtPos(pos);
			}
		}
		if(    tmpWidget != eventTable[pointerID].curentWidgetEvent
		    || (    true == eventTable[pointerID].isInside
		         && (     eventTable[pointerID].origin.x() > pos.x()
		              ||  eventTable[pointerID].origin.y() > pos.y()
		              || (eventTable[pointerID].origin.x() + eventTable[pointerID].size.x()) < pos.x()
		              || (eventTable[pointerID].origin.y() + eventTable[pointerID].size.y()) < pos.y()) ) ) {
			eventTable[pointerID].isInside = false;
			EVENT_DEBUG("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [LEAVE] " << pos);
			eventTable[pointerID].posEvent = pos;
			localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::keyEvent::statusLeave, pos);
		}
		if (false == eventTable[pointerID].isInside) {
			// Set the element inside ...
			eventTable[pointerID].isInside = true;
			// get destination widget :
			eventTable[pointerID].curentWidgetEvent = tmpWidget;
			if (NULL == eventTable[pointerID].curentWidgetEvent) {
				eventTable[pointerID].isInside = false;
			}
			if (NULL != eventTable[pointerID].curentWidgetEvent) {
				eventTable[pointerID].origin = eventTable[pointerID].curentWidgetEvent->GetOrigin();
				eventTable[pointerID].size = eventTable[pointerID].curentWidgetEvent->GetSize();
			}
			eventTable[pointerID].destinationInputId = 0;
			EVENT_DEBUG("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [ENTER] " << pos);
			eventTable[pointerID].posEvent = pos;
			localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::keyEvent::statusEnter, pos);
		}
		EVENT_DEBUG("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [MOVE]  " << pos);
		eventTable[pointerID].posEvent = pos;
		localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::keyEvent::statusMove, pos);
	} else if (true == eventTable[pointerID].isUsed) {
		if (true == eventTable[pointerID].isInside) {
			if(     eventTable[pointerID].origin.x() > pos.x()
			    ||  eventTable[pointerID].origin.y() > pos.y()
			    || (eventTable[pointerID].origin.x() + eventTable[pointerID].size.x()) < pos.x()
			    || (eventTable[pointerID].origin.y() + eventTable[pointerID].size.y()) < pos.y()) {
				eventTable[pointerID].isInside = false;
				EVENT_DEBUG("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [LEAVE] " << pos);
				eventTable[pointerID].posEvent = pos;
				localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::keyEvent::statusLeave, pos);
			}
		} else {
			if(    (     eventTable[pointerID].origin.x() <= pos.x()
			         && (eventTable[pointerID].origin.x() + eventTable[pointerID].size.x()) >= pos.x() )
			    && (     eventTable[pointerID].origin.y() <= pos.y()
			         && (eventTable[pointerID].origin.y() + eventTable[pointerID].size.y()) >= pos.y() ) ) {
				eventTable[pointerID].isInside = true;
				EVENT_DEBUG("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [ENTER] " << pos);
				eventTable[pointerID].posEvent = pos;
				localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::keyEvent::statusEnter, pos);
			}
		}
		EVENT_DEBUG("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [MOVE]  " << pos);
		eventTable[pointerID].posEvent = pos;
		localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::keyEvent::statusMove, pos);
	}
}

void ewol::eInput::State(ewol::keyEvent::type_te type, int pointerID, bool isDown, vec2 pos)
{
	if (MAX_MANAGE_INPUT<=pointerID) {
		// reject pointer ==> out of IDs...
		return;
	}
	EWOL_DEBUG("event pointerId=" << pointerID);
	// convert position in Open-GL coordonates ...
	InputPoperty_ts *eventTable = NULL;
	inputLimit_ts   localLimit;
	if (type == ewol::keyEvent::typeMouse) {
		eventTable = m_eventMouseSaved;
		localLimit = m_eventMouseLimit;
	} else if (type == ewol::keyEvent::typeFinger) {
		eventTable = m_eventInputSaved;
		localLimit = m_eventInputLimit;
	} else {
		EWOL_ERROR("Unknown type of event");
		return;
	}
	if(    pointerID > MAX_MANAGE_INPUT
	    || pointerID <= 0) {
		// not manage input
		return;
	}
	// get the curent time ...
	int64_t currentTime = ewol::GetTime();
	ewol::Windows* tmpWindows = m_context.GetWindows();
	
	if (true == isDown) {
		EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [DOWN] " << pos);
		if(true == eventTable[pointerID].isUsed) {
			// we have an event previously ... check delay between click and offset position
			if (currentTime - eventTable[pointerID].lastTimeEvent > localLimit.sepatateTime) {
				CleanElement(eventTable, pointerID);
			} else if(    abs(eventTable[pointerID].downStart.x() - pos.x()) >= localLimit.DpiOffset
			           || abs(eventTable[pointerID].downStart.y() - pos.y()) >= localLimit.DpiOffset ){
				CleanElement(eventTable, pointerID);
			}
		}
		if(true == eventTable[pointerID].isUsed) {
			// save start time
			eventTable[pointerID].lastTimeEvent = currentTime;
			// generate DOWN Event
			EVENT_DEBUG("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [DOWN]   " << pos);
			eventTable[pointerID].posEvent = pos;
			localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::keyEvent::statusDown, pos);
		} else {
			// Mark it used :
			eventTable[pointerID].isUsed = true;
			// Save current position :
			eventTable[pointerID].downStart = pos;
			// save start time
			eventTable[pointerID].lastTimeEvent = currentTime;
			// Set the element inside ...
			eventTable[pointerID].isInside = true;
			// get destination widget :
			if(NULL != tmpWindows) {
				if (m_grabWidget != NULL && type == ewol::keyEvent::typeMouse) {
					eventTable[pointerID].curentWidgetEvent = m_grabWidget;
				} else {
					eventTable[pointerID].curentWidgetEvent = tmpWindows->GetWidgetAtPos(pos);
				}
			} else {
				eventTable[pointerID].curentWidgetEvent = NULL;
			}
			if (NULL != eventTable[pointerID].curentWidgetEvent) {
				eventTable[pointerID].origin = eventTable[pointerID].curentWidgetEvent->GetOrigin();
				eventTable[pointerID].size = eventTable[pointerID].curentWidgetEvent->GetSize();
				eventTable[pointerID].destinationInputId = localGetDestinationId(type, eventTable[pointerID].curentWidgetEvent, pointerID);
			} else {
				eventTable[pointerID].destinationInputId = -1;
			}
			// generate DOWN Event
			EVENT_DEBUG("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [DOWN]   " << pos);
			eventTable[pointerID].posEvent = pos;
			localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::keyEvent::statusDown, pos);
		}
	} else {
		EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [UP]     " << pos);
		if(false == eventTable[pointerID].isUsed) {
			// bad case ... ???
			EWOL_DEBUG("Up event without previous down ... ");
			// Mark it un-used :
			eventTable[pointerID].isUsed = false;
			// revove the widget ...
			eventTable[pointerID].curentWidgetEvent = NULL;
		} else {
			// generate UP Event
			EVENT_DEBUG("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [UP]     " << pos);
			eventTable[pointerID].posEvent = pos;
			localEventInput(type, eventTable[pointerID].curentWidgetEvent, pointerID, ewol::keyEvent::statusUp, pos);
			// generate event (single)
			if(    abs(eventTable[pointerID].downStart.x() - pos.x()) < localLimit.DpiOffset
			    && abs(eventTable[pointerID].downStart.y() - pos.y()) < localLimit.DpiOffset ){
				// Save current position :
				eventTable[pointerID].downStart = pos;
				// save start time
				eventTable[pointerID].lastTimeEvent = currentTime;
				int32_t nbClickMax = 0;
				if(eventTable[pointerID].curentWidgetEvent != NULL) {
					nbClickMax = eventTable[pointerID].curentWidgetEvent->GetMouseLimit();
					if (nbClickMax>5) {
						nbClickMax = 5;
					}
				}
				// in grab mode the single to quinte event are not generated ....
				if(    (    m_grabWidget == NULL
				         || type != ewol::keyEvent::typeMouse )
				    && eventTable[pointerID].nbClickEvent < nbClickMax) {
					// generate event SINGLE :
					eventTable[pointerID].nbClickEvent++;
					EVENT_DEBUG("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [" << eventTable[pointerID].nbClickEvent << "] " << pos);
					eventTable[pointerID].posEvent = pos;
					localEventInput(type,
					                eventTable[pointerID].curentWidgetEvent,
					                eventTable[pointerID].destinationInputId,
					                (ewol::keyEvent::status_te)(ewol::keyEvent::statusSingle + eventTable[pointerID].nbClickEvent-1),
					                pos);
					if( eventTable[pointerID].nbClickEvent >= nbClickMax) {
						eventTable[pointerID].nbClickEvent = 0;
					}
				} else {
					eventTable[pointerID].nbClickEvent = 0;
				}
			}
			// specific for tuch event
			if (type == ewol::keyEvent::typeFinger) {
				CleanElement(eventTable, pointerID);
			}
		}
	}
}


