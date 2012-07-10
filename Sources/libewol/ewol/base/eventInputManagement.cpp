/**
 *******************************************************************************
 * @file eventInputManagement.cpp
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
#include <ewol/OObject.h>
#include <ewol/Widget.h>
#include <ewol/Windows.h>
#include <ewol/base/gui.h>

#include <ewol/Debug.h>
#include <etk/UString.h>
#include <ewol/EObject.h>
#include <ewol/EObjectManager.h>
#include <ewol/WidgetManager.h>
#include <ewol/base/gui.h>
#include <ewol/ewol.h>
#include <ewol/Texture.h>
#include <ewol/base/MainThread.h>
#include <ewol/base/eventInputManagement.h>


typedef struct {
	bool          isUsed;
	int32_t       destinationInputId;
	int64_t       lastTimeEvent;
	ewol::Widget* curentWidgetEvent;
	Vector2D<float>    origin;
	Vector2D<float>    size;
	Vector2D<float>    downStart;
	bool          isDown;
	bool          isInside;
	int32_t       nbClickEvent; // 0 .. 1 .. 2 .. 3
} InputPoperty_ts;


#define MAX_MANAGE_INPUT         (10)
InputPoperty_ts eventInputSaved[MAX_MANAGE_INPUT];
InputPoperty_ts eventMouseSaved[MAX_MANAGE_INPUT];

static void CleanElement(InputPoperty_ts *eventTable, int32_t idInput)
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
void ewol::eventInput::OnObjectRemove(ewol::EObject * removeObject)
{
	for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
		if (eventInputSaved[iii].curentWidgetEvent == removeObject) {
			// remove the property of this input ...
			CleanElement(eventInputSaved, iii);
		}
		if (eventMouseSaved[iii].curentWidgetEvent == removeObject) {
			// remove the property of this input ...
			CleanElement(eventMouseSaved, iii);
		}
	}
}

/**
 * @brief a new layer on the windows is set ==> might remove all the property of the current element ...
 * @param ---
 * @return ---
 */
void ewol::eventInput::NewLayerSet(void)
{
	for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
		// remove the property of this input ...
		CleanElement(eventInputSaved, iii);
		CleanElement(eventMouseSaved, iii);
	}
}


void ewol::eventInput::Init(void)
{
	EWOL_INFO("Init");
	for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
		// remove the property of this input ...
		CleanElement(eventInputSaved, iii);
		CleanElement(eventMouseSaved, iii);
	}
}

void ewol::eventInput::UnInit(void)
{
	EWOL_INFO("Un-Init");
	for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
		// remove the property of this input ...
		CleanElement(eventInputSaved, iii);
		CleanElement(eventMouseSaved, iii);
	}
}


extern ewol::Windows* gui_uniqueWindows;

/**
 * @brief generate the event on the destinated widger
 * @param[in] type Type of the event that might be sended
 * @param[in] destWidget Pointer on the requested widget that element might be sended
 * @param[in] IdInput Id of the event (PC : [0..9] and touch : [1..9])
 * @param[in] typeEvent type of the eventg generated
 * @param[in] pos position of the event
 * @return true if event has been greped
 */
static bool localEventInput(ewol::inputType_te type, ewol::Widget* destWidget, int32_t IdInput, ewol::eventInputType_te typeEvent, Vector2D<float> pos)
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
static int32_t localGetDestinationId(ewol::inputType_te type, ewol::Widget* destWidget, int32_t realInputId)
{
	if (type == ewol::INPUT_TYPE_FINGER) {
		int32_t lastMinimum = 0;
		for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
			if (true==eventInputSaved[iii].isUsed) {
				if (eventInputSaved[iii].curentWidgetEvent == destWidget) {
					if (iii != realInputId) {
						lastMinimum = etk_max(lastMinimum, eventInputSaved[iii].destinationInputId);
					}
				}
			}
		}
		return lastMinimum+1;
	}
	return realInputId;
}



// defined by the platform specific file : 
extern int32_t separateClickTime;
extern int32_t offsetMoveClicked;
extern int32_t offsetMoveClickedDouble;

// note if id<0 ==> the it was finger event ...
void ewol::eventInput::Motion(ewol::inputType_te type, int pointerID, Vector2D<float> pos)
{
	InputPoperty_ts *eventTable = NULL;
	if (type == ewol::INPUT_TYPE_MOUSE) {
		eventTable = eventMouseSaved;
	} else if (type == ewol::INPUT_TYPE_FINGER) {
		eventTable = eventInputSaved;
	} else {
		EWOL_ERROR("Unknown type of event");
		return;
	}
	if(    pointerID > MAX_MANAGE_INPUT
	    || pointerID < 0) {
		// not manage input
		return;
	}
	// special case for the mouse event 0 that represent the hover event of the system :
	if (type == ewol::INPUT_TYPE_MOUSE && pointerID == 0) {
		// this event is all time on the good widget ... and manage the enter and leave ...
		// NOTE : the "layer widget" force us to get the widget at the specific position all the time :
		ewol::Widget* tmpWidget = gui_uniqueWindows->GetWidgetAtPos(pos);
		if(    tmpWidget != eventTable[pointerID].curentWidgetEvent
		    || (    true == eventTable[pointerID].isInside
		         && (     eventTable[pointerID].origin.x > pos.x
		              ||  eventTable[pointerID].origin.y > pos.y
		              || (eventTable[pointerID].origin.x + eventTable[pointerID].size.x) < pos.x
		              || (eventTable[pointerID].origin.y + eventTable[pointerID].size.y) < pos.y) ) ) {
			eventTable[pointerID].isInside = false;
			EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [LEAVE] " << pos);
			localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_LEAVE, pos);
		}
		if (false == eventTable[pointerID].isInside) {
			// Set the element inside ...
			eventTable[pointerID].isInside = true;
			// get destination widget :
			if(NULL != gui_uniqueWindows) {
				eventTable[pointerID].curentWidgetEvent = gui_uniqueWindows->GetWidgetAtPos(pos);
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
			EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [ENTER] " << pos);
			localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_ENTER, pos);
		}
		EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [MOVE]  " << pos);
		localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_MOVE, pos);
	} else if (true == eventTable[pointerID].isUsed) {
		if (true == eventTable[pointerID].isInside) {
			if(     eventTable[pointerID].origin.x > pos.x
			    ||  eventTable[pointerID].origin.y > pos.y
			    || (eventTable[pointerID].origin.x + eventTable[pointerID].size.x) < pos.x
			    || (eventTable[pointerID].origin.y + eventTable[pointerID].size.y) < pos.y) {
				eventTable[pointerID].isInside = false;
				EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [LEAVE] " << pos);
				localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_LEAVE, pos);
			}
		} else {
			if(    (     eventTable[pointerID].origin.x <= pos.x
			         && (eventTable[pointerID].origin.x + eventTable[pointerID].size.x) >= pos.x )
			    && (     eventTable[pointerID].origin.y <= pos.y
			         && (eventTable[pointerID].origin.y + eventTable[pointerID].size.y) >= pos.y ) ) {
				eventTable[pointerID].isInside = true;
				EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [ENTER] " << pos);
				localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_ENTER, pos);
			}
		}
		EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [MOVE]  " << pos);
		localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_MOVE, pos);
	}
}

void ewol::eventInput::State(ewol::inputType_te type, int pointerID, bool isDown, Vector2D<float> pos)
{
	InputPoperty_ts *eventTable = NULL;
	if (type == ewol::INPUT_TYPE_MOUSE) {
		eventTable = eventMouseSaved;
	} else if (type == ewol::INPUT_TYPE_FINGER) {
		eventTable = eventInputSaved;
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
	int64_t currentTime = GetCurrentTime();
	
	if (true == isDown) {
		EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [DOWN] " << pos);
		if(true == eventTable[pointerID].isUsed) {
			// we have an event previously ... check delay between click and offset position
			if (currentTime - eventTable[pointerID].lastTimeEvent > separateClickTime) {
				CleanElement(eventTable, pointerID);
			} else if(    abs(eventTable[pointerID].downStart.x - pos.x) >= offsetMoveClicked
			           || abs(eventTable[pointerID].downStart.y - pos.y) >= offsetMoveClicked ){
				CleanElement(eventTable, pointerID);
			}
		}
		if(true == eventTable[pointerID].isUsed) {
			// save start time
			eventTable[pointerID].lastTimeEvent = currentTime;
			// generate DOWN Event
			EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [DOWN]   " << pos);
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
			if(NULL != gui_uniqueWindows) {
				eventTable[pointerID].curentWidgetEvent = gui_uniqueWindows->GetWidgetAtPos(pos);
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
			EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [DOWN]   " << pos);
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
			EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [UP]     " << pos);
			localEventInput(type, eventTable[pointerID].curentWidgetEvent, pointerID, ewol::EVENT_INPUT_TYPE_UP, pos);
			// generate event (single)
			if(    abs(eventTable[pointerID].downStart.x - pos.x) < offsetMoveClicked
			    && abs(eventTable[pointerID].downStart.y - pos.y) < offsetMoveClicked ){
				// Save current position :
				eventTable[pointerID].downStart = pos;
				// save start time
				eventTable[pointerID].lastTimeEvent = currentTime;
				if(    eventTable[pointerID].nbClickEvent == 0
				    && eventTable[pointerID].curentWidgetEvent != NULL
				    && eventTable[pointerID].curentWidgetEvent->GetMouseLimit()>0 ) {
					// generate event SINGLE :
					eventTable[pointerID].nbClickEvent++;
					EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [SINGLE] " << pos);
					localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_SINGLE, pos);
				} else if(    eventTable[pointerID].nbClickEvent == 1
				           && eventTable[pointerID].curentWidgetEvent != NULL
				           && eventTable[pointerID].curentWidgetEvent->GetMouseLimit()>1 ) {
					// generate event DOUBLE :
					eventTable[pointerID].nbClickEvent++;
					EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [DOUBLE] " << pos);
					localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_DOUBLE, pos);
				} else if(    eventTable[pointerID].nbClickEvent == 2
				           && eventTable[pointerID].curentWidgetEvent != NULL
				           && eventTable[pointerID].curentWidgetEvent->GetMouseLimit()>2 ) {
					// generate event TRIPLE :
					eventTable[pointerID].nbClickEvent++;
					EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventTable[pointerID].destinationInputId << " [TRIPLE] " << pos);
					localEventInput(type, eventTable[pointerID].curentWidgetEvent, eventTable[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_TRIPLE, pos);
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


