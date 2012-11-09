/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/Debug.h>
#include <ewol/ClipBoard.h>
#include <ewol/os/gui.h>
#include <ewol/os/eSystem.h>

#undef __class__
#define __class__	"ClipBoard"


/*
note: la copy dans le :
        0 : copy standard
   [1..9] : copy interne
       10 : bouton du milieux
*/
//!< Local copy of the clipboards
static etk::UString mesCopy[ewol::clipBoard::TOTAL_OF_CLICKBOARD];


void ewol::clipBoard::Init(void)
{
	EWOL_INFO("Initialyse ClipBoards");
	for(int32_t i=0; i<ewol::clipBoard::TOTAL_OF_CLICKBOARD; i++) {
		mesCopy[i].Clear();
	}
}


void ewol::clipBoard::UnInit(void)
{
	EWOL_INFO("Initialyse ClipBoards");
	for(int32_t i=0; i<ewol::clipBoard::TOTAL_OF_CLICKBOARD; i++) {
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
	
	if(clipboardID >= ewol::clipBoard::TOTAL_OF_CLICKBOARD) {
		EWOL_WARNING("request ClickBoard id error");
		return;
	}
	
	ewol::clipBoard::SetSystem(clipboardID, data);
	
	if(    ewol::clipBoard::CLIPBOARD_STD == clipboardID
	    || ewol::clipBoard::CLIPBOARD_SELECTION == clipboardID) {
		guiInterface::ClipBoardSet(clipboardID);
	}
}


void ewol::clipBoard::Request(ewol::clipBoard::clipboardListe_te clipboardID)
{
	if(clipboardID >= ewol::clipBoard::TOTAL_OF_CLICKBOARD) {
		EWOL_WARNING("request ClickBoard id error");
		return;
	}
	
	if(    ewol::clipBoard::CLIPBOARD_STD == clipboardID
	    || ewol::clipBoard::CLIPBOARD_SELECTION == clipboardID) {
		guiInterface::ClipBoardGet(clipboardID);
	} else {
		// generate an event on the main thread ...
		eSystem::ClipBoardArrive(clipboardID);
	}
}


void ewol::clipBoard::SetSystem(ewol::clipBoard::clipboardListe_te clipboardID, etk::UString &data)
{
	if(clipboardID >= ewol::clipBoard::TOTAL_OF_CLICKBOARD) {
		EWOL_WARNING("request ClickBoard id error");
		return;
	}
	// Copy datas ...
	mesCopy[clipboardID] = data;
}


etk::UString ewol::clipBoard::Get(ewol::clipBoard::clipboardListe_te clipboardID)
{
	if(clipboardID >= ewol::clipBoard::TOTAL_OF_CLICKBOARD) {
		EWOL_WARNING("request ClickBoard id error");
		return "";
	}
	// Copy datas ...
	return mesCopy[clipboardID];
}




