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

#undef __class__
#define __class__	"ClipBoard"


/*
note: la copy dans le :
        0 : copy standard
   [1..9] : copy interne
       10 : bouton du milieux
*/
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



void ewol::clipBoard::Set(uint8_t clipboardID, etk::UString &data)
{
	// check if ID is correct
	if(0 == data.Size()) {
		EWOL_INFO("request a copy of nothing");
		return;
	/*
	} else if (ewol::clipBoard::CLIPBOARD_STD == clipboardID) {
		guiAbstraction::ClipBoardSet(data, guiAbstraction::CLIPBOARD_MODE_STD);
		return;
	} else if (ewol::clipBoard::CLIPBOARD_SELECTION == clipboardID) {
		guiAbstraction::ClipBoardSet(data, guiAbstraction::CLIPBOARD_MODE_PRIMARY);
		return;
	*/
	}else if(clipboardID >= ewol::clipBoard::TOTAL_OF_CLICKBOARD) {
		EWOL_WARNING("request ClickBoard id error");
		return;
	}
	// Copy datas ...
	mesCopy[clipboardID] = data;
}

void ewol::clipBoard::Get(uint8_t clipboardID, etk::UString &data)
{
	/*
	if (ewol::clipBoard::CLIPBOARD_STD == clipboardID) {
		guiAbstraction::ClipBoardGet(data, guiAbstraction::CLIPBOARD_MODE_STD);
		return;
	} else if (ewol::clipBoard::CLIPBOARD_SELECTION == clipboardID) {
		guiAbstraction::ClipBoardGet(data, guiAbstraction::CLIPBOARD_MODE_PRIMARY);
		return;
	} else */ if(clipboardID >= ewol::clipBoard::TOTAL_OF_CLICKBOARD) {
		EWOL_WARNING("request ClickBoard id error");
		return;
	}
	// Copy datas ...
	data = mesCopy[clipboardID];
}





