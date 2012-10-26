/**
 *******************************************************************************
 * @file os/gui.h
 * @brief Gui abstraction layer (header)
 * @author Edouard DUPIN
 * @date 20/10/2011
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


#ifndef __GUI_INTERFACE_H__
#define __GUI_INTERFACE_H__

#include <etk/Types.h>
#include <etk/UString.h>
#include <ewol/widget/Windows.h>
#include <ewol/ewol.h>
#include <ewol/ClipBoard.h>


namespace guiInterface
{
	/**
	 * @brief Get the curent time in micro-second
	 * @param ---
	 * @return ---
	 */
	int64_t GetTime(void);
	/**
	 * @brief Stop the current program
	 * @param ---
	 * @return ---
	 */
	void Stop(void);
	/**
	 * @brief Change the current Windows size
	 * @param size The requested size
	 * @return ---
	 */
	void ChangeSize(etk::Vector2D<int32_t> size);
	/**
	 * @brief Change the current Windows position
	 * @param pos The position where the winsdows might be placed.
	 * @return ---
	 */
	void ChangePos(etk::Vector2D<int32_t> pos);
	/**
	 * @brief Get the current Windows position
	 * @param pos The position where the winsdows is.
	 * @return ---
	 */
	void GetAbsPos(etk::Vector2D<int32_t>& pos);
	/**
	 * @brief Display the virtal keyboard (for touch system only)
	 * @param ---
	 * @return ---
	 */
	void KeyboardShow(void);
	/**
	 * @brief Hide the virtal keyboard (for touch system only)
	 * @param ---
	 * @return ---
	 */
	void KeyboardHide(void);
	/**
	 * @brief Inform the Gui that we want to have a copy of the clipboard
	 * @param ID of the clipboard (STD/SELECTION) only apear here
	 * @return ---
	 */
	void ClipBoardGet(ewol::clipBoard::clipboardListe_te clipboardID);
	/**
	 * @brief Inform the Gui that we are the new owner of the clipboard
	 * @param ID of the clipboard (STD/SELECTION) only apear here
	 * @return ---
	 */
	void ClipBoardSet(ewol::clipBoard::clipboardListe_te clipboardID);
	/**
	 * @brief Set the new title of the windows
	 * @param title New desired title
	 * @return ---
	 */
	void SetTitle(etk::UString& title);
	/**
	 * @brief Force the screen orientation (availlable on portable elements ...
	 * @param orientation Selected orientation.
	 * @return ---
	 */
	void ForceOrientation(ewol::orientation_te orientation);
};


//!< must be define in CPP by the application ... this are the main init and unInit of the Application
void      APP_Init(void);
void      APP_UnInit(void);
etk::File APP_Icon(void);

#define NB_MAX_INPUT                  (20)

#endif
