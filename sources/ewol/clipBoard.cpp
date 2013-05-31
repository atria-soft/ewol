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
#include <ewol/renderer/os/gui.h>
#include <ewol/renderer/os/eSystem.h>

#undef __class__
#define __class__	"ClipBoard"


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

etk::CCout& ewol::clipBoard::operator <<(etk::CCout& _os, const ewol::clipBoard::clipboardListe_te _obj)
{
	if (_obj>=0 && _obj <ewol::clipBoard::clipboardCount) {
		_os << clipboardDescriptionString[_obj];
	} else {
		_os << "[ERROR]";
	}
	return _os;
}


void ewol::clipBoard::Init(void)
{
	EWOL_INFO("Initialyse ClipBoards");
	for(int32_t i=0; i<ewol::clipBoard::clipboardCount; i++) {
		mesCopy[i].Clear();
	}
}


void ewol::clipBoard::UnInit(void)
{
	EWOL_INFO("Initialyse ClipBoards");
	for(int32_t i=0; i<ewol::clipBoard::clipboardCount; i++) {
		mesCopy[i].Clear();
	}
}


void ewol::clipBoard::Set(ewol::clipBoard::clipboardListe_te _clipboardID, const etk::UString& _data)
{
	// check if ID is correct
	if(0 == _data.Size()) {
		EWOL_INFO("request a copy of nothing");
		return;
	} else 
	
	if(_clipboardID >= ewol::clipBoard::clipboardCount) {
		EWOL_WARNING("request ClickBoard id error");
		return;
	}
	
	ewol::clipBoard::SetSystem(_clipboardID, _data);
	
	if(    ewol::clipBoard::clipboardStd == _clipboardID
	    || ewol::clipBoard::clipboardSelection == _clipboardID) {
		guiInterface::ClipBoardSet(_clipboardID);
	}
}


void ewol::clipBoard::Request(ewol::clipBoard::clipboardListe_te _clipboardID)
{
	if(_clipboardID >= ewol::clipBoard::clipboardCount) {
		EWOL_WARNING("request ClickBoard id error");
		return;
	}
	
	if(    ewol::clipBoard::clipboardStd == _clipboardID
	    || ewol::clipBoard::clipboardSelection == _clipboardID) {
		guiInterface::ClipBoardGet(_clipboardID);
	} else {
		// generate an event on the main thread ...
		eSystem::ClipBoardArrive(_clipboardID);
	}
}


void ewol::clipBoard::SetSystem(ewol::clipBoard::clipboardListe_te _clipboardID, const etk::UString& _data)
{
	if(_clipboardID >= ewol::clipBoard::clipboardCount) {
		EWOL_WARNING("request ClickBoard id error");
		return;
	}
	// Copy datas ...
	mesCopy[_clipboardID] = _data;
}


const etk::UString& ewol::clipBoard::Get(ewol::clipBoard::clipboardListe_te _clipboardID)
{
	if(_clipboardID >= ewol::clipBoard::clipboardCount) {
		EWOL_WARNING("request ClickBoard id error");
		return "";
	}
	// Copy datas ...
	return mesCopy[_clipboardID];
}





