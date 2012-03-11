
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
	coord2D_ts    origin;
	coord2D_ts    size;
	coord2D_ts    downStart;
	bool          isDown;
	bool          isInside;
	int32_t       nbClickEvent; // 0 .. 1 .. 2 .. 3
} InputPoperty_ts;


#define MAX_MANAGE_INPUT    (10)
InputPoperty_ts eventInputSaved[MAX_MANAGE_INPUT];

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
			eventInputSaved[iii].curentWidgetEvent = NULL;
		}
	}
}

static void CleanInputElement(int32_t idInput)
{
	eventInputSaved[idInput].isUsed = false;
	eventInputSaved[idInput].destinationInputId = 0;
	eventInputSaved[idInput].lastTimeEvent = 0;
	eventInputSaved[idInput].curentWidgetEvent = NULL;
	eventInputSaved[idInput].origin.x = 0;
	eventInputSaved[idInput].origin.y = 0;
	eventInputSaved[idInput].size.x = 99999999;
	eventInputSaved[idInput].size.y = 99999999;
	eventInputSaved[idInput].downStart.x = 0;
	eventInputSaved[idInput].downStart.y = 0;
	eventInputSaved[idInput].isDown = false;
	eventInputSaved[idInput].isInside = true;
	eventInputSaved[idInput].nbClickEvent = 0;
}

void ewol::eventInput::Init(void)
{
	EWOL_INFO("Init");
	for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
		eventInputSaved[iii].isUsed = false;
		eventInputSaved[iii].destinationInputId = 0;
		eventInputSaved[iii].lastTimeEvent = 0;
		eventInputSaved[iii].curentWidgetEvent = NULL;
		eventInputSaved[iii].origin.x = 0;
		eventInputSaved[iii].origin.y = 0;
		eventInputSaved[iii].size.x = 99999999;
		eventInputSaved[iii].size.y = 99999999;
		eventInputSaved[iii].downStart.x = 0;
		eventInputSaved[iii].downStart.y = 0;
		eventInputSaved[iii].isDown = false;
		eventInputSaved[iii].isInside = true;
		eventInputSaved[iii].nbClickEvent = 0;
	}
}

void ewol::eventInput::UnInit(void)
{
	EWOL_INFO("Un-Init");
}




extern ewol::Windows* gui_uniqueWindows;

static bool localEventInput(ewol::Widget* destWidget, int32_t IdInput, ewol::eventInputType_te typeEvent, coord2D_ts pos)
{
	if (NULL != destWidget) {
		return destWidget->OnEventInput(IdInput, typeEvent, pos);
	}
	return false;
}

/**
 * @brief Convert the system event id in the correct EWOL id depending of the system management mode
 *
 * This function find the next input id unused on the specifiic widget ==> on PC, the ID does not change (IHM is not the same
 *
 * @param[in] destWidget Pointer of the widget destination
 * @param[in] realInputId System Id
 *
 * @return the ewol input id
 */
static int32_t localGetDestinationId(ewol::Widget* destWidget, int32_t realInputId)
{
	#ifdef __MODE__Touch
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
	#else
		return realInputId;
	#endif
}



// defined by the platform specific file : 
extern int32_t separateClickTime;
extern int32_t offsetMoveClicked;
extern int32_t offsetMoveClickedDouble;

void ewol::eventInput::Motion(int pointerID, coord2D_ts pos)
{
	if(    pointerID > MAX_MANAGE_INPUT
	    || pointerID < 0) {
		// not manage input
		return;
	}
	// special PC State : 
	if(pointerID == 0) {
		ewol::Widget* destWidget = NULL;
		if(NULL != gui_uniqueWindows) {
			destWidget = gui_uniqueWindows->GetWidgetAtPos(pos);
		}
		if (NULL != destWidget) {
			destWidget->OnEventInput(0, ewol::EVENT_INPUT_TYPE_MOVE, pos);
		}
		return;
	}
	if (true == eventInputSaved[pointerID].isUsed) {
	
		if (true == eventInputSaved[pointerID].isInside) {
			if(     eventInputSaved[pointerID].origin.x > pos.x
			    ||  eventInputSaved[pointerID].origin.y > pos.y
			    || (eventInputSaved[pointerID].origin.x + eventInputSaved[pointerID].size.x) < pos.x
			    || (eventInputSaved[pointerID].origin.y + eventInputSaved[pointerID].size.y) < pos.y) {
				eventInputSaved[pointerID].isInside = false;
				EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventInputSaved[pointerID].destinationInputId << " [LEAVE] " << pos);
				localEventInput(eventInputSaved[pointerID].curentWidgetEvent, eventInputSaved[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_LEAVE, pos);
			}
		} else {
			if(    (     eventInputSaved[pointerID].origin.x <= pos.x
			         && (eventInputSaved[pointerID].origin.x + eventInputSaved[pointerID].size.x) >= pos.x )
			    && (     eventInputSaved[pointerID].origin.y <= pos.y
			         && (eventInputSaved[pointerID].origin.y + eventInputSaved[pointerID].size.y) >= pos.y ) ) {
				eventInputSaved[pointerID].isInside = true;
				EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventInputSaved[pointerID].destinationInputId << " [ENTER] " << pos);
				localEventInput(eventInputSaved[pointerID].curentWidgetEvent, eventInputSaved[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_ENTER, pos);
			}
		}
		EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventInputSaved[pointerID].destinationInputId << " [MOVE]  " << pos);
		localEventInput(eventInputSaved[pointerID].curentWidgetEvent, eventInputSaved[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_MOVE, pos);
	}
}

void ewol::eventInput::State(int pointerID, bool isDown, coord2D_ts pos)
{
	if(    pointerID > MAX_MANAGE_INPUT
	    || pointerID <= 0) {
		// not manage input
		return;
	}
	// get the curent time ...
	int64_t currentTime = GetCurrentTime();
	
	if (true == isDown) {
		EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventInputSaved[pointerID].destinationInputId << " [DOWN] " << pos);
		if(true == eventInputSaved[pointerID].isUsed) {
			// we have an event previously ... check delay between click and offset position
			if (currentTime - eventInputSaved[pointerID].lastTimeEvent > separateClickTime) {
				CleanInputElement(pointerID);
			} else if(    abs(eventInputSaved[pointerID].downStart.x - pos.x) >= offsetMoveClicked
			           || abs(eventInputSaved[pointerID].downStart.y - pos.y) >= offsetMoveClicked ){
				CleanInputElement(pointerID);
			}
		}
		if(true == eventInputSaved[pointerID].isUsed) {
			// save start time
			eventInputSaved[pointerID].lastTimeEvent = currentTime;
			// generate DOWN Event
			EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventInputSaved[pointerID].destinationInputId << " [DOWN]   " << pos);
			localEventInput(eventInputSaved[pointerID].curentWidgetEvent, eventInputSaved[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_DOWN, pos);
		} else {
			// Mark it used :
			eventInputSaved[pointerID].isUsed = true;
			// Save current position :
			eventInputSaved[pointerID].downStart = pos;
			// save start time
			eventInputSaved[pointerID].lastTimeEvent = currentTime;
			// Set the element inside ...
			eventInputSaved[pointerID].isInside = true;
			// get destination widget :
			if(NULL != gui_uniqueWindows) {
				eventInputSaved[pointerID].curentWidgetEvent = gui_uniqueWindows->GetWidgetAtPos(pos);
			} else {
				eventInputSaved[pointerID].curentWidgetEvent = NULL;
			}
			if (NULL != eventInputSaved[pointerID].curentWidgetEvent) {
				eventInputSaved[pointerID].origin = eventInputSaved[pointerID].curentWidgetEvent->GetOrigin();
				eventInputSaved[pointerID].size = eventInputSaved[pointerID].curentWidgetEvent->GetSize();
				eventInputSaved[pointerID].destinationInputId = localGetDestinationId(eventInputSaved[pointerID].curentWidgetEvent, pointerID);
			} else {
				eventInputSaved[pointerID].destinationInputId = -1;
			}
			// generate DOWN Event
			EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventInputSaved[pointerID].destinationInputId << " [DOWN]   " << pos);
			localEventInput(eventInputSaved[pointerID].curentWidgetEvent, eventInputSaved[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_DOWN, pos);
		}
	} else {
		EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventInputSaved[pointerID].destinationInputId << " [UP]     " << pos);
		if(false == eventInputSaved[pointerID].isUsed) {
			// bad case ... ???
			EWOL_VERBOSE("Up event without previous down ... ");
			// Mark it un-used :
			eventInputSaved[pointerID].isUsed = false;
			// revove the widget ...
			eventInputSaved[pointerID].curentWidgetEvent = NULL;
		} else {
			// generate UP Event
			EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventInputSaved[pointerID].destinationInputId << " [UP]     " << pos);
			localEventInput(eventInputSaved[pointerID].curentWidgetEvent, pointerID, ewol::EVENT_INPUT_TYPE_UP, pos);
			// generate event (single)
			if(    abs(eventInputSaved[pointerID].downStart.x - pos.x) < offsetMoveClicked
			    && abs(eventInputSaved[pointerID].downStart.y - pos.y) < offsetMoveClicked ){
				// Save current position :
				eventInputSaved[pointerID].downStart = pos;
				// save start time
				eventInputSaved[pointerID].lastTimeEvent = currentTime;
				if (eventInputSaved[pointerID].nbClickEvent == 0) {
					// generate event SINGLE :
					eventInputSaved[pointerID].nbClickEvent++;
					EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventInputSaved[pointerID].destinationInputId << " [SINGLE] " << pos);
					localEventInput(eventInputSaved[pointerID].curentWidgetEvent, eventInputSaved[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_SINGLE, pos);
				} else if (eventInputSaved[pointerID].nbClickEvent == 1) {
					// generate event DOUBLE :
					eventInputSaved[pointerID].nbClickEvent++;
					EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventInputSaved[pointerID].destinationInputId << " [DOUBLE] " << pos);
					localEventInput(eventInputSaved[pointerID].curentWidgetEvent, eventInputSaved[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_DOUBLE, pos);
				} else if (eventInputSaved[pointerID].nbClickEvent == 2) {
					// generate event TRIPLE :
					eventInputSaved[pointerID].nbClickEvent++;
					EWOL_VERBOSE("GUI : Input ID=" << pointerID << "==>" << eventInputSaved[pointerID].destinationInputId << " [TRIPLE] " << pos);
					localEventInput(eventInputSaved[pointerID].curentWidgetEvent, eventInputSaved[pointerID].destinationInputId, ewol::EVENT_INPUT_TYPE_TRIPLE, pos);
				} else {
					#if 0
					eventInputSaved[pointerID].nbClickEvent++;
					EWOL_DEBUG("GUI : Input ID=" << pointerID << "==>" << eventInputSaved[pointerID].destinationInputId << " [" << eventInputSaved[pointerID].nbClickEvent <<"] (" << pos.x << "," << pos.y << ")");
					#else
					eventInputSaved[pointerID].nbClickEvent = 0;
					#endif
				}
			}
			#ifdef __MODE__Touch
			CleanInputElement(pointerID);
			#endif
		}
	}
}


