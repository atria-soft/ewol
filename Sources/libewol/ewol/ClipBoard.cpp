/**
 *******************************************************************************
 * @file ClipBoard.cpp
 * @brief ewol : copy / past main system (sources)
 * @author Edouard DUPIN
 * @date 04/04/2012
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

#include <ewol/Debug.h>
#include <ewol/ClipBoard.h>
#include <ewol/base/gui.h>
#include <ewol/base/eSystem.h>

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


/**
 * @brief Initialize the clipboard system (done by ewol)
 * @param ---
 * @return ---
 */
void ewol::clipBoard::Init(void)
{
	EWOL_INFO("Initialyse ClipBoards");
	for(int32_t i=0; i<ewol::clipBoard::TOTAL_OF_CLICKBOARD; i++) {
		mesCopy[i].Clear();
	}
}


/**
 * @brief Un-Initialize the clipboard system (done by ewol)
 * @param ---
 * @return ---
 */
void ewol::clipBoard::UnInit(void)
{
	EWOL_INFO("Initialyse ClipBoards");
	for(int32_t i=0; i<ewol::clipBoard::TOTAL_OF_CLICKBOARD; i++) {
		mesCopy[i].Clear();
	}
}


/**
 * @brief Set the string data on a specific clipboard. The Gui system is notify that the clipboard "SELECTION" and "COPY" are change
 * @param[in] clipboardID Select the specific ID of the clipboard
 * @param[in] data The string that might be send to the clipboard
 * @return ---
 */
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


/**
 * @brief Call system to request the current clipboard.
 * @note Due to some system that manage the clipboard request asynchronous (like X11) and ewol managing the system with only one thread,
 *       we need the call the system to send us the buffer, this is really ambigous, but the widget (who has focus) receive the 
 *       notification of the arrival of this buffer id
 * @param[in] clipboardID the needed clipboard ID
 * @return ---
 */
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


/**
 * @brief Set the ewol internal buffer (no notification at the GUI). This fuction might be use by the 
 *        Gui abstraction to set the buffer we receive. The end user must not use it.
 * @param[in] clipboardID selected clipboard ID
 * @param[in] data new buffer data
 * @return ---
 */
void ewol::clipBoard::SetSystem(ewol::clipBoard::clipboardListe_te clipboardID, etk::UString &data)
{
	if(clipboardID >= ewol::clipBoard::TOTAL_OF_CLICKBOARD) {
		EWOL_WARNING("request ClickBoard id error");
		return;
	}
	// Copy datas ...
	mesCopy[clipboardID] = data;
}


/**
 * @brief Get the ewol internal buffer of the curent clipboard. The end user can use it when he receive the event in 
 *        the widget : @ref OnEventClipboard ==> we can nothe this function is the only one which permit it.
 * @note if we call this fuction withoutcallin @ref ewol::clipBoard::Request, we only get the previous clipboard
 * @param[in] clipboardID selected clipboard ID
 * @return the requested buffer
 */
etk::UString ewol::clipBoard::Get(ewol::clipBoard::clipboardListe_te clipboardID)
{
	if(clipboardID >= ewol::clipBoard::TOTAL_OF_CLICKBOARD) {
		EWOL_WARNING("request ClickBoard id error");
		return "";
	}
	// Copy datas ...
	return mesCopy[clipboardID];
}




