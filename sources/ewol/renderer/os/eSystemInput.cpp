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

#include <ewol/eObject/EObject.h>
#include <ewol/eObject/EObjectManager.h>

#include <ewol/renderer/os/gui.h>
#include <ewol/renderer/os/eSystem.h>
#include <ewol/renderer/os/eSystemInput.h>
#include <ewol/renderer/resources/Texture.h>

#include <ewol/widget/Widget.h>
#include <ewol/widget/Windows.h>
#include <ewol/widget/WidgetManager.h>



#define EVENT_DEBUG  EWOL_VERBOSE
//#define EVENT_DEBUG  EWOL_DEBUG

void ewol::eSystemInput::CalculateLimit(void)
{
	m_eventInputLimit.sepatateTime = 300000; // µs
	m_eventInputLimit.DpiOffset = m_dpi*100;
	m_eventMouseLimit.sepatateTime = 300000; // µs
	m_eventMouseLimit.DpiOffset = (float)m_dpi*(float)0.1;
}

void ewol::eSystemInput::SetDpi(int32_t newDPI)
{
	m_dpi = newDPI;
	// recalculate the DPI system ...
	CalculateLimit();
}

void ewol::eSystemInput::CleanElement(InputPoperty_ts *eventTable, int32_t idInput)
{
	if (NULL==eventTable) {
		return;
	}
	//EWOL_INFO("CleanElement[" << idInput << "] = @" << (int64_t)eventTable);
	eventTable[idInput].isUsed = false;
	eventTable[idInput].destinationInputId = 0;
	eventTable[idInput].lastTimeEvent = 0;
	eventTable[idInput].curentWidgetEvent = NULL;
	eventTable[idInput].origin.setValue(0,0);
	eventTable[idInput].size.setValue(99999999,99999999);
	eventTable[idInput].downStart.setValue(0,0);
	eventTable[idInput].isDown = false;
	eventTable[idInput].isInside = false;
	eventTable[idInput].nbClickEvent = 0;
	eventTable[idInput].posEvent.setValue(0,0);
}


bool ewol::eSystemInput::localEventInput(ewol::keyEvent::type_te _type,
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


void ewol::eSystemInput::TransfertEvent(ewol::Widget* source, ewol::Widget* destination)
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

void ewol::eSystemInput::GrabPointer(ewol::Widget* widget)
{
	if(NULL==widget) {
		return;
	}
	m_grabWidget = widget;
	guiInterface::GrabPointerEvents(true, widget->GetOrigin() + ivec2(widget->GetSize().x()/2.0f, widget->GetSize().y()/2.0f) );
}

void ewol::eSystemInput::UnGrabPointer(void)
{
	m_grabWidget = NULL;
	guiInterface::GrabPointerEvents(false, vec2(0,0));
}

void ewol::eSystemInput::OnObjectRemove(ewol::EObject * removeObject)
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

void ewol::eSystemInput::NewLayerSet(void)
{
	for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
		// remove the property of this input ...
		CleanElement(m_eventInputSaved, iii);
		CleanElement(m_eventMouseSaved, iii);
	}
}

void ewol::eSystemInput::Reset(void)
{
	for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
		// remove the property of this input ...
		CleanElement(m_eventInputSaved, iii);
		CleanElement(m_eventMouseSaved, iii);
	}
}

ewol::eSystemInput::eSystemInput(void) :
	m_grabWidget(NULL)
{
	SetDpi(200);
	EWOL_INFO("Init (start)");
	Reset();
	EWOL_INFO("Init (end)");
}

ewol::eSystemInput::~eSystemInput(void)
{
	EWOL_INFO("Un-Init (start)");
	Reset();
	EWOL_INFO("Un-Init (end)");
}


int32_t ewol::eSystemInput::localGetDestinationId(ewol::keyEvent::type_te type, ewol::Widget* destWidget, int32_t realInputId)
{
	if (type == ewol::keyEvent::typeFinger) {
		int32_t lastMinimum = 0;
		for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
			if (true==m_eventInputSaved[iii].isUsed) {
				if (m_eventInputSaved[iii].curentWidgetEvent == destWidget) {
					if (iii != realInputId) {
						lastMinimum = etk_max(lastMinimum, m_eventInputSaved[iii].destinationInputId);
					}
				}
			}
		}
		return lastMinimum+1;
	}
	return realInputId;
}

// note if id<0 ==> the it was finger event ...
void ewol::eSystemInput::Motion(ewol::keyEvent::type_te type, int pointerID, vec2 pos)
{
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
	ewol::Windows* tmpWindows = eSystem::GetCurrentWindows();
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

void ewol::eSystemInput::State(ewol::keyEvent::type_te type, int pointerID, bool isDown, vec2 pos)
{
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
	ewol::Windows* tmpWindows = eSystem::GetCurrentWindows();
	
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


