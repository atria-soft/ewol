
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
	coord2D_ts    lastEventPos;
	bool          isDown;
	int32_t       nbClickEvent; // 0 .. 1 .. 2 .. 3
} InputPoperty_ts;


#define MAX_MANAGE_INPUT    (10)
InputPoperty_ts eventInputSaved[MAX_MANAGE_INPUT];

/**
 * @brief Inform object that an other object is removed ...
 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
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
	eventInputSaved[idInput].size.x = 0;
	eventInputSaved[idInput].size.y = 0;
	eventInputSaved[idInput].downStart.x = 0;
	eventInputSaved[idInput].downStart.y = 0;
	eventInputSaved[idInput].lastEventPos.x = 0;
	eventInputSaved[idInput].lastEventPos.y = 0;
	eventInputSaved[idInput].isDown = false;
	eventInputSaved[idInput].nbClickEvent = 0;
}

void ewol::eventInput::Init(void)
{
	for(int32_t iii=0; iii<MAX_MANAGE_INPUT; iii++) {
		eventInputSaved[iii].isUsed = false;
		eventInputSaved[iii].destinationInputId = 0;
		eventInputSaved[iii].lastTimeEvent = 0;
		eventInputSaved[iii].curentWidgetEvent = NULL;
		eventInputSaved[iii].origin.x = 0;
		eventInputSaved[iii].origin.y = 0;
		eventInputSaved[iii].size.x = 0;
		eventInputSaved[iii].size.y = 0;
		eventInputSaved[iii].downStart.x = 0;
		eventInputSaved[iii].downStart.y = 0;
		eventInputSaved[iii].lastEventPos.x = 0;
		eventInputSaved[iii].lastEventPos.y = 0;
		eventInputSaved[iii].isDown = false;
		eventInputSaved[iii].nbClickEvent = 0;
	}
}

void ewol::eventInput::UnInit(void)
{

}




extern ewol::Windows* gui_uniqueWindows;

bool localEventInput(ewol::Widget* destWidget, int32_t IdInput, ewol::eventInputType_te typeEvent, coord2D_ts pos)
{
	if (NULL != destWidget) {
		ewol::eventPosition_ts tmpEventPosition;
		tmpEventPosition.abs = pos;
		tmpEventPosition.local = destWidget->RelativePosition(pos);
		return destWidget->OnEventInput(IdInput, typeEvent, tmpEventPosition);
	}
	return false;
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
	if (true == eventInputSaved[pointerID].isUsed) {
		EWOL_DEBUG("GUI : Input ID=" << pointerID << " [MOVE] (" << pos.x << "," << pos.y << ")");
		localEventInput(eventInputSaved[pointerID].curentWidgetEvent, pointerID, ewol::EVENT_INPUT_TYPE_MOVE, pos);
	}
}

void ewol::eventInput::State(int pointerID, bool isDown, coord2D_ts pos)
{
	if(    pointerID > MAX_MANAGE_INPUT
	    || pointerID < 0) {
		// not manage input
		return;
	}
	// get the curent time ...
	int64_t currentTime = GetCurrentTime();
	
	if (true == isDown) {
		EWOL_VERBOSE("GUI : Input ID=" << pointerID << " [DOWN] x=" << pos.x << " y=" << pos.y);
		if(true == eventInputSaved[pointerID].isUsed) {
			// bad case ... ???
		} else {
			// Mark it used :
			eventInputSaved[pointerID].isUsed = true;
			// Save current position :
			eventInputSaved[pointerID].downStart = pos;
			// save start time
			eventInputSaved[pointerID].lastTimeEvent = currentTime;
			// get destination widget :
			if(NULL != gui_uniqueWindows) {
				eventInputSaved[pointerID].curentWidgetEvent = gui_uniqueWindows->GetWidgetAtPos(pos);
			} else {
				eventInputSaved[pointerID].curentWidgetEvent = NULL;
			}
			// generate DOWN Event
			EWOL_DEBUG("GUI : Input ID=" << pointerID << " [DOWN]   (" << pos.x << "," << pos.y << ")");
			localEventInput(eventInputSaved[pointerID].curentWidgetEvent, pointerID, ewol::EVENT_INPUT_TYPE_DOWN, pos);
		}
	} else {
		EWOL_VERBOSE("GUI : Input ID=" << pointerID << " [UP]     (" << pos.x << "," << pos.y << ")");
		if(false == eventInputSaved[pointerID].isUsed) {
			// bad case ... ???
			EWOL_WARNING("Up event without previous down ... ");
			// Mark it un-used :
			eventInputSaved[pointerID].isUsed = false;
			// revove the widget ...
			eventInputSaved[pointerID].curentWidgetEvent = NULL;
		} else {
			// generate UP Event
			EWOL_DEBUG("GUI : Input ID=" << pointerID << " [UP]     (" << pos.x << "," << pos.y << ")");
			localEventInput(eventInputSaved[pointerID].curentWidgetEvent, pointerID, ewol::EVENT_INPUT_TYPE_UP, pos);
			// generate event (single)
			if(    abs(eventInputSaved[pointerID].downStart.x - pos.x) < offsetMoveClicked
			    && abs(eventInputSaved[pointerID].downStart.y - pos.y) < offsetMoveClicked ){
				// Save current position :
				eventInputSaved[pointerID].downStart = pos;
				// save start time
				eventInputSaved[pointerID].lastTimeEvent = currentTime;
				// generate event :
				EWOL_DEBUG("GUI : Input ID=" << pointerID << " [SINGLE] (" << pos.x << "," << pos.y << ")");
				localEventInput(eventInputSaved[pointerID].curentWidgetEvent, pointerID, ewol::EVENT_INPUT_TYPE_SINGLE, pos);
			}
			// Mark it un-used :
			eventInputSaved[pointerID].isUsed = false;
			// revove the widget ...
			eventInputSaved[pointerID].curentWidgetEvent = NULL;
		}
	}
}


