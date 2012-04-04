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
	if(clipboardID >= ewol::clipBoard::TOTAL_OF_CLICKBOARD) {
		EWOL_WARNING("request ClickBoard id error");
	} else if(0 == data.Size()) {
		EWOL_INFO("request a copy of nothing");
	} else if (ewol::clipBoard::CLIPBOARD_STD == clipboardID) {
		//GtkClipboard * clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
		//gtk_clipboard_set_text(clipboard, (const gchar*)&data[0], data.Size() );
	} else if (ewol::clipBoard::CLIPBOARD_SELECTION == clipboardID) {
		//GtkClipboard * clipboard = gtk_clipboard_get(GDK_SELECTION_PRIMARY);
		//gtk_clipboard_set_text(clipboard, (const gchar*)&data[0], data.Size() );
	}
	// Copy datas ...
	mesCopy[clipboardID] = data;
}


void ewol::clipBoard::Get(uint8_t clipboardID, etk::UString &data)
{
	if(clipboardID >= ewol::clipBoard::TOTAL_OF_CLICKBOARD) {
		EWOL_WARNING("request ClickBoard id error");
	} else if (ewol::clipBoard::CLIPBOARD_STD == clipboardID) {
		/*
		GtkClipboard * clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD );
		gchar *text = gtk_clipboard_wait_for_text(clipboard);
		if (text != NULL) {
			mesCopy[COPY_STD].Clear();
			mesCopy[COPY_STD].PushBack((int8_t*)text, strlen(text) );
		}
		*/
	} else if (ewol::clipBoard::CLIPBOARD_SELECTION == clipboardID) {
		/*
		GtkClipboard * clipboard = gtk_clipboard_get(GDK_SELECTION_PRIMARY );
		gchar *text = gtk_clipboard_wait_for_text(clipboard);
		if (text != NULL) {
			mesCopy[COPY_MIDDLE_BUTTON].Clear();
			mesCopy[COPY_MIDDLE_BUTTON].PushBack((int8_t*)text, strlen(text) );
		}
		*/
	}
	// Copy datas ...
	data = mesCopy[clipboardID];
}





