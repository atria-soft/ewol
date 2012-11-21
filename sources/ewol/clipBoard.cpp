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


static const char* clipboardDescriptionString[clipboardCount] = {
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

etk::CCout& ewol::clipBoard::operator <<(etk::CCout &os, const ewol::clipBoard::clipboardListe_te obj)
{
	if (obj>=0 && obj <clipboardCount) {
		os << clipboardDescriptionString[obj];
	} else {
		os << "[ERROR]";
	}
	return os;
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


void ewol::clipBoard::Set(ewol::clipBoard::clipboardListe_te clipboardID, etk::UString &data)
{
	// check if ID is correct
	if(0 == data.Size()) {
		EWOL_INFO("request a copy of nothing");
		return;
	} else 
	
	if(clipboardID >= ewol::clipBoard::clipboardCount) {
		EWOL_WARNING("request ClickBoard id error");
		return;
	}
	
	ewol::clipBoard::SetSystem(clipboardID, data);
	
	if(    ewol::clipBoard::clipboardStd == clipboardID
	    || ewol::clipBoard::clipboardSelection == clipboardID) {
		guiInterface::ClipBoardSet(clipboardID);
	}
}


void ewol::clipBoard::Request(ewol::clipBoard::clipboardListe_te clipboardID)
{
	if(clipboardID >= ewol::clipBoard::clipboardCount) {
		EWOL_WARNING("request ClickBoard id error");
		return;
	}
	
	if(    ewol::clipBoard::clipboardStd == clipboardID
	    || ewol::clipBoard::clipboardSelection == clipboardID) {
		guiInterface::ClipBoardGet(clipboardID);
	} else {
		// generate an event on the main thread ...
		eSystem::ClipBoardArrive(clipboardID);
	}
}


void ewol::clipBoard::SetSystem(ewol::clipBoard::clipboardListe_te clipboardID, etk::UString &data)
{
	if(clipboardID >= ewol::clipBoard::clipboardCount) {
		EWOL_WARNING("request ClickBoard id error");
		return;
	}
	// Copy datas ...
	mesCopy[clipboardID] = data;
}


etk::UString ewol::clipBoard::Get(ewol::clipBoard::clipboardListe_te clipboardID)
{
	if(clipboardID >= ewol::clipBoard::clipboardCount) {
		EWOL_WARNING("request ClickBoard id error");
		return "";
	}
	// Copy datas ...
	return mesCopy[clipboardID];
}





