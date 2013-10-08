/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>

#include <ewol/debug.h>
#include <ewol/clipBoard.h>
#include <ewol/renderer/eContext.h>

#undef __class__
#define __class__ "ClipBoard"


/*
note: la copy dans le :
        0 : copy standard
   [1..9] : copy interne
       10 : bouton du milieux
*/
//!< Local copy of the clipboards
static etk::UString mesCopy[ewol::clipBoard::clipboardCount];

static const char* clipboardDescriptionString[ewol::clipBoard::clipboardCount+1] = {
	"clipboard0",
	"clipboard1",
	"clipboard2",
	"clipboard3",
	"clipboard4",
	"clipboard5",
	"clipboard6",
	"clipboard7",
	"clipboard8",
	"clipboard9",
	"clipboardStd",
	"clipboardSelection",
	"clipboardCount"
};

etk::CCout& ewol::clipBoard::operator <<(etk::CCout& _os, const ewol::clipBoard::clipboardListe_te _obj) {
	if (_obj >= 0 && _obj <ewol::clipBoard::clipboardCount) {
		_os << clipboardDescriptionString[_obj];
	} else {
		_os << "[ERROR]";
	}
	return _os;
}


void ewol::clipBoard::init(void) {
	EWOL_INFO("Initialyse ClipBoards");
	for(int32_t i=0; i<ewol::clipBoard::clipboardCount; i++) {
		mesCopy[i].clear();
	}
}


void ewol::clipBoard::unInit(void) {
	EWOL_INFO("Initialyse ClipBoards");
	for(int32_t i=0; i<ewol::clipBoard::clipboardCount; i++) {
		mesCopy[i].clear();
	}
}


void ewol::clipBoard::set(ewol::clipBoard::clipboardListe_te _clipboardID, const etk::UString& _data) {
	// check if ID is correct
	if(0 == _data.size()) {
		EWOL_INFO("request a copy of nothing");
		return;
	} else 
	
	if(_clipboardID >= ewol::clipBoard::clipboardCount) {
		EWOL_WARNING("request ClickBoard id error");
		return;
	}
	
	ewol::clipBoard::setSystem(_clipboardID, _data);
	
	if(    ewol::clipBoard::clipboardStd == _clipboardID
	    || ewol::clipBoard::clipboardSelection == _clipboardID) {
		ewol::getContext().clipBoardSet(_clipboardID);
		EWOL_TODO("Set ClipBoard");
	}
}


void ewol::clipBoard::request(ewol::clipBoard::clipboardListe_te _clipboardID) {
	if(_clipboardID >= ewol::clipBoard::clipboardCount) {
		EWOL_WARNING("request ClickBoard id error");
		return;
	}
	
	if(    ewol::clipBoard::clipboardStd == _clipboardID
	    || ewol::clipBoard::clipboardSelection == _clipboardID) {
		ewol::getContext().clipBoardGet(_clipboardID);
		EWOL_TODO("Get ClipBoard");
	} else {
		// generate an event on the main thread ...
		ewol::getContext().OS_ClipBoardArrive(_clipboardID);
		EWOL_TODO("ClipBoard arrive");
	}
}


void ewol::clipBoard::setSystem(ewol::clipBoard::clipboardListe_te _clipboardID, const etk::UString& _data) {
	if(_clipboardID >= ewol::clipBoard::clipboardCount) {
		EWOL_WARNING("request ClickBoard id error");
		return;
	}
	// Copy datas ...
	mesCopy[_clipboardID] = _data;
}


const etk::UString& ewol::clipBoard::get(ewol::clipBoard::clipboardListe_te _clipboardID) {
	static const etk::UString emptyString("");
	if(_clipboardID >= ewol::clipBoard::clipboardCount) {
		EWOL_WARNING("request ClickBoard id error");
		return emptyString;
	}
	// Copy datas ...
	return mesCopy[_clipboardID];
}





