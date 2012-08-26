/**
 *******************************************************************************
 * @file os/eSystemInput.cpp
 * @brief Input (mouse,finger) abstraction layer (Sources)
 * @author Edouard DUPIN
 * @date 00/04/2011
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */


#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/oObject/OObject.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Windows.h>
#include <ewol/os/gui.h>

#include <ewol/Debug.h>
#include <etk/UString.h>
#include <ewol/eObject/EObject.h>
#include <ewol/eObject/EObjectManager.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/os/gui.h>
#include <ewol/ewol.h>
#include <ewol/texture/Texture.h>
#include <ewol/os/eSystem.h>
#include <ewol/os/eSystemInput.h>


#define EVENT_DEBUG  EWOL_VERBOSE
//#define EVENT_DEBUG  EWOL_DEBUG

void ewol::eSystemInput::CalculateLimit(void)
{
	m_eventInputLimit.sepatateTime = 300000; // �s
	m_eventInputLimit.DpiOffset = m_dpi*100;
	m_eventMouseLimit.sepatateTime = 300000; // �s
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
	eventTable[idInput].isUsed = false;
	eventTable[idInput].destinationInputId = 0;
	eventTable[idInput].lastTimeEvent = 0;
	eventTable[idInput].curentWidgetEvent = NULL;
	eventTable[idInput].origin.x = 0;
	eventTable[idInput].origin.y = 0;
	eventTable[idInput].size.x = 99999999;
	eventTable[idInput].size.y = 99999999;
	eventTable[idInput].downStart.x = 0;
	eventTable[idInput].downStart.y = 0;
	eventTable[idInput].isDown = false;
	eventTable[idInput].isInside = false;
	eventTable[idInput].nbClickEvent = 0;
}


/**
 * @brief Inform object that an other object is removed ...
 * @param[in] removeObject Pointer on the EObject removed ==> the user must remove all reference on this EObject
 * @note : Sub classes must call this class
 * @return ---
 */
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

/**
 * @brief a new layer on the windows is set ==> might remove all the property of the current element ...
 * @param ---
 * @return ---
 */
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

ewol::eSystemInput::eSystemInput(void)
{
	SetDpi(200);
	EWOL_INFO("Init");
	Reset();
}

ewol::eSystemInput::~eSystemInput(void)
{
	EWOL_INFO("Un-Init");
	Reset();
}


/**
 * @brief generate the event on the destinated widger
 * @param[in] type Type of the event that might be sended
 * @param[in] destWidget Pointer on the requested widget that element might be sended
 * @param[in] IdInput Id of the event (PC : [0..9] and touch : [1..9])
 * @param[in] typeEvent type of the eventg generated
 * @param[in] pos position of the event
 * @return true if event has been greped
 */
bool ewol::eSystemInput::localEventInput(ewol::inputType_te type, ewol::Widget* destWidget, int32_t IdInput, ewol::eventInputType_te typeEvent, Vector2D<float> pos)
{
	if (NULL != destWidget) {
		if (type == ewol::INPUT_TYPE_MOUSE || type == ewol::INPUT_TYPE_FINGER) {
			return destWidget->OnEventInput(type, IdInput, typeEvent, pos);
		} else {
			return false;
		}
	}
	return false;
}

/**
 * @brief Convert the system event id in the correct EWOL id depending of the system management mode
 * This function find the next input id unused on the specifiic widget ==> on PC, the ID does not change (IHM is not the same
 * @param[in] destWidget Pointer of the widget destination
 * @param[in] realInputId System Id
 * @return the ewol input id
 */
int32_t ewol::eSystemInput::localGetDestinationId(ewol::inputType_te type, ewol::Widget* destWidget, int32_t realInputId)
{
	if (type == ewol::INPUT_TYPE_FINGER) {
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
void ewol::eSystemInput::Motion(ewol::inputType_te type, int pointerID, Vector2D<float> pos)
{
	// convert position in Open-GL coordonates ...
	pos.y = ewol::GetCurrentHeight() - pos.y;
	
	InputPoperty_ts *eventTable = NULL;
	if (type == ewol::INPUT_TYPE_MOUSE) {
		eventTable = m_eventMouseSaved;
	} else if (type == ewol::INPUT_TYPE_FINGER) {
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
	if (type == ewol::INPUT_TYPE_MOUSE && pointerID == 0) {
		// this event is all time on the good widget ... and manage the enter and leave ...
		// NOTE : the "layer widget" force us to get the widget at the specific position all the time :
		ewol::Widget* tmpWidget = NULL;
		if (NULL != tmpWindows) {
			tmpWidget = tmpWindows->GetWidgetAtPos(pos);
		}
		if(    tmpWidget != eventTable[pointerID].curentWidgetEvent
		    || (    true == eventTable[pointerID].isInside
		         && (     eventTable[pointerID].origin.x > pos.x
		              ||  eventTable[pointerID].origin.y > pos.y
		              || (eventTable[pointerID].origin.x + eventTable[pointerID].size.x) < pos.x
		              || (eventTable[pointerID].origin.y + eventTable[pointerID].size.y) < pos.y) ) ) {
			eventTable[pointerID].isInside = false;
			EVENT_DEBUG("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [LEAVE] " << pos);
			localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_LEAVE, pos);
		}
		if (false == eventTable[pointerID].isInside) {
			// Set the element inside ...
			eventTable[pointerID].isInside = true;
			// get destination widget :
			if(NULL != tmpWindows) {
				eventTable[pointerID].curentWidgetEvent = tmpWindows->GetWidgetAtPos(pos);
			} else {
				eventTable[pointerID].curentWidgetEvent = NULL;
			}
			if (NULL == eventTable[pointerID].curentWidgetEvent) {
				eventTable[pointerID].isInside = false;
			}
			if (NULL != eventTable[pointerID].curentWidgetEvent) {
				eventTable[pointerID].origin = eventTable[pointerID].curentWidgetEvent->GetOrigin();
				eventTable[pointerID].size = eventTable[pointerID].curentWidgetEvent->GetSize();
			}
			eventTable[pointerID].destinationInputId = 0;
			EVENT_DEBUG("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [ENTER] " << pos);
			localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_ENTER, pos);
		}
		EVENT_DEBUG("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [MOVE]  " << pos);
		localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_MOVE, pos);
	} else if (true == eventTable[pointerID].isUsed) {
		if (true == eventTable[pointerID].isInside) {
			if(     eventTable[pointerID].origin.x > pos.x
			    ||  eventTable[pointerID].origin.y > pos.y
			    || (eventTable[pointerID].origin.x + eventTable[pointerID].size.x) < pos.x
			    || (eventTable[pointerID].origin.y + eventTable[pointerID].size.y) < pos.y) {
				eventTable[pointerID].isInside = false;
				EVENT_DEBUG("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [LEAVE] " << pos);
				localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_LEAVE, pos);
			}
		} else {
			if(    (     eventTable[pointerID].origin.x <= pos.x
			         && (eventTable[pointerID].origin.x + eventTable[pointerID].size.x) >= pos.x )
			    && (     eventTable[pointerID].origin.y <= pos.y
			         && (eventTable[pointerID].origin.y + eventTable[pointerID].size.y) >= pos.y ) ) {
				eventTable[pointerID].isInside = true;
				EVENT_DEBUG("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [ENTER] " << pos);
				localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_ENTER, pos);
			}
		}
		EVENT_DEBUG("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [MOVE]  " << pos);
		localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_MOVE, pos);
	}
}

void ewol::eSystemInput::State(ewol::inputType_te type, int pointerID, bool isDown, Vector2D<float> pos)
{
	// convert position in Open-GL coordonates ...
	pos.y = ewol::GetCurrentHeight() - pos.y;
	InputPoperty_ts *eventTable = NULL;
	inputLimit_ts   localLimit;
	if (type == ewol::INPUT_TYPE_MOUSE) {
		eventTable = m_eventMouseSaved;
		localLimit = m_eventMouseLimit;
	} else if (type == ewol::INPUT_TYPE_FINGER) {
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
			} else if(    abs(eventTable[pointerID].downStart.x - pos.x) >= localLimit.DpiOffset
			           || abs(eventTable[pointerID].downStart.y - pos.y) >= localLimit.DpiOffset ){
				CleanElement(eventTable, pointerID);
			}
		}
		if(true == eventTable[pointerID].isUsed) {
			// save start time
			eventTable[pointerID].lastTimeEvent = currentTime;
			// generate DOWN Event
			EVENT_DEBUG("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [DOWN]   " << pos);
			localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_DOWN, pos);
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
				eventTable[pointerID].curentWidgetEvent = tmpWindows->GetWidgetAtPos(pos);
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
			localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_DOWN, pos);
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
			localEventInput(type, eventTable[pointerID].curentWidgetEvent, pointerID, ewol::EVENT_INPUT_TYPE_UP, pos);
			// generate event (single)
			if(    abs(eventTable[pointerID].downStart.x - pos.x) < localLimit.DpiOffset
			    && abs(eventTable[pointerID].downStart.y - pos.y) < localLimit.DpiOffset ){
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
				if(eventTable[pointerID].nbClickEvent < nbClickMax) {
					// generate event SINGLE :
					eventTable[pointerID].nbClickEvent++;
					EVENT_DEBUG("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [" << eventTable[pointerID].nbClickEvent << "] " << pos);
					localEventInput(type,
					                eventTable[pointerID].curentWidgetEvent,
					                eventTable[pointerID].destinationInputId,
					                (ewol::eventInputType_te)(ewol::EVENT_INPUT_TYPE_SINGLE + eventTable[pointerID].nbClickEvent-1),
					                pos);
					if( eventTable[pointerID].nbClickEvent >= nbClickMax) {
						eventTable[pointerID].nbClickEvent = 0;
					}
				} else {
					eventTable[pointerID].nbClickEvent = 0;
				}
			}
			// specific for tuch event
			if (type == ewol::INPUT_TYPE_FINGER) {
				CleanElement(eventTable, pointerID);
			}
		}
	}
}

