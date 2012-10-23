/**
 *******************************************************************************
 * @file ewol/ShortCutManager.cpp
 * @brief ewol shortCut manager (Sources)
 * @author Edouard DUPIN
 * @date 22/02/2012
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


#include <ewol/ShortCutManager.h>
#include <ewol/eObject/EObject.h>
#include <ewol/ewol.h>

/*
class EventShortCut {
	public:
		const char *       generateEventId; // event generate ID (to be unique it was pointer on the string name)
		etk::UString       eventData;
		bool               shift;
		bool               control;
		bool               alt;
		bool               meta;
		uniChar_t          UnicodeValue;      // 0 if not used
		ewol::eventKbMoveType_te keyboardMoveValue; // 0 if not used
};
*/

static etk::Vector<ewol::EventShortCut *> l_inputShortCutEvent;  //!< generic short-cut event

namespace ewol {
	namespace shortCut {
		void Add(bool shift, bool control, bool alt, bool meta, 
		         uniChar_t unicodeValue,
		         ewol::eventKbMoveType_te kbMove,
		         const char * generateEventId,
		         etk::UString data);
	};
};

void ewol::shortCut::Add(bool shift, bool control, bool alt, bool meta, 
                         uniChar_t unicodeValue,
                         ewol::eventKbMoveType_te kbMove,
                         const char * generateEventId,
                         etk::UString data)
{
	ewol::EventShortCut * newEvent = new ewol::EventShortCut();
	if (NULL == newEvent) {
		EWOL_ERROR("Allocation Error on the shortcut ...");
		return;
	}
	newEvent->generateEventId = generateEventId;
	newEvent->specialKey.shift = shift;
	newEvent->specialKey.ctrl = control;
	newEvent->specialKey.alt = alt;
	newEvent->specialKey.meta = meta;
	newEvent->unicodeValue = unicodeValue;
	newEvent->keyboardMoveValue = kbMove;
	newEvent->eventData = data;
	l_inputShortCutEvent.PushBack(newEvent);
	return;
}


void ewol::shortCut::Add(const char * descriptiveString, const char * generateEventId, etk::UString data)
{
	if(		NULL==descriptiveString
		||	0==strlen(descriptiveString))
	{
		return;
	}
	bool shift = false;
	bool control = false;
	bool alt = false;
	bool meta = false;
	uint32_t UnicodeValue = 0;
	
	// parsing of the string :
	//"ctrl+shift+alt+meta+s"
	const char * tmp = strstr(descriptiveString, "ctrl");
	if(NULL != tmp) {
		control = true;
	}
	tmp = strstr(descriptiveString, "shift");
	if(NULL != tmp) {
		shift = true;
	}
	tmp = strstr(descriptiveString, "alt");
	if(NULL != tmp) {
		alt = true;
	}
	tmp = strstr(descriptiveString, "meta");
	if(NULL != tmp) {
		meta = true;
	}
	if(NULL != strstr(descriptiveString, "F12") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_F12, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "F11") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_F11, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "F10") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_F10, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "F9") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_F9, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "F8") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_F8, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "F7") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_F7, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "F6") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_F6, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "F5") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_F5, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "F4") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_F4, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "F3") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_F3, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "F2") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_F2, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "F1") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_F1, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "LEFT") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_LEFT, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "RIGHT") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_RIGHT, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "UP") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_UP, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "DOWN") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_DOWN, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "PAGE_UP") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_PAGE_UP, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "PAGE_DOWN") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_PAGE_DOWN, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "START") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_START, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "END") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_END, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "CENTER") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_CENTER, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "ARRET_DEFIL") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_ARRET_DEFIL, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "WAIT") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_WAIT, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "INSERT") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_INSERT, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "CAPLOCK") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_CAPLOCK, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "CONTEXT_MENU") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_CONTEXT_MENU, generateEventId, data);
	} else if(NULL != strstr(descriptiveString, "VER_NUM") ) {
		ewol::shortCut::Add(shift, control, alt, meta, 0, ewol::EVENT_KB_MOVE_TYPE_VER_NUM, generateEventId, data);
	} else {
		UnicodeValue = descriptiveString[strlen(descriptiveString) -1];
		// add with generic Adding function ...
		ewol::shortCut::Add(shift, control, alt, meta, UnicodeValue, ewol::EVENT_KB_MOVE_TYPE_NONE, generateEventId, data);
	}
}


void ewol::shortCut::Init(void)
{
	if (l_inputShortCutEvent.Size()>0) {
		EWOL_WARNING("Old element error in the shortCut system");
		for(int32_t iii=0; iii< l_inputShortCutEvent.Size(); iii++) {
			delete(l_inputShortCutEvent[iii]);
			l_inputShortCutEvent[iii] = NULL;
		}
	}
	l_inputShortCutEvent.Clear();
}

void ewol::shortCut::UnInit(void)
{
	if (l_inputShortCutEvent.Size()>0) {
		for(int32_t iii=0; iii< l_inputShortCutEvent.Size(); iii++) {
			delete(l_inputShortCutEvent[iii]);
			l_inputShortCutEvent[iii] = NULL;
		}
	}
	l_inputShortCutEvent.Clear();
}



bool ewol::shortCut::Process(ewol::specialKey_ts& special, uniChar_t unicodeValue, ewol::eventKbMoveType_te kbMove, bool isDown)
{
	if (unicodeValue >= 'A' && unicodeValue <='Z') {
		unicodeValue += 'a' - 'A';
	}
	//EWOL_INFO("Try to find generic shortcut ...");
	for(int32_t iii=l_inputShortCutEvent.Size()-1; iii>=0; iii--) {
		if(    l_inputShortCutEvent[iii]->specialKey.shift == special.shift
		    && l_inputShortCutEvent[iii]->specialKey.ctrl  == special.ctrl
		    && l_inputShortCutEvent[iii]->specialKey.alt   == special.alt
		    && l_inputShortCutEvent[iii]->specialKey.meta  == special.meta
		    && (    (    l_inputShortCutEvent[iii]->keyboardMoveValue == ewol::EVENT_KB_MOVE_TYPE_NONE
		              && l_inputShortCutEvent[iii]->unicodeValue == unicodeValue)
		         || (    l_inputShortCutEvent[iii]->keyboardMoveValue == kbMove
		              && l_inputShortCutEvent[iii]->unicodeValue == 0)
		       ) )
		{
			if (isDown) {
				ewol::EObjectMessageMultiCast::AnonymousSend(l_inputShortCutEvent[iii]->generateEventId, l_inputShortCutEvent[iii]->eventData);
			} // no else
			return true;
		}
	}
	return false;
}


