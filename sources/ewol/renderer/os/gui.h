/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __GUI_INTERFACE_H__
#define __GUI_INTERFACE_H__

#include <etk/types.h>
#include <etk/UString.h>
#include <ewol/widget/Windows.h>
#include <ewol/ewol.h>
#include <ewol/clipBoard.h>
#include <etk/os/FSNode.h>


namespace guiInterface
{
	int main(int argc, const char *argv[]);
	/**
	 * @brief Get the curent time in micro-second
	 */
	int64_t GetTime(void);
	/**
	 * @brief Stop the current program
	 */
	void Stop(void);
	/**
	 * @brief Change the current Windows size
	 * @param size The requested size
	 */
	void ChangeSize(ivec2 size);
	/**
	 * @brief Change the current Windows position
	 * @param pos The position where the winsdows might be placed.
	 */
	void ChangePos(ivec2 pos);
	/**
	 * @brief Get the current Windows position
	 * @param pos The position where the winsdows is.
	 */
	void GetAbsPos(ivec2& pos);
	/**
	 * @brief Display the virtal keyboard (for touch system only)
	 */
	void KeyboardShow(void);
	/**
	 * @brief Hide the virtal keyboard (for touch system only)
	 */
	void KeyboardHide(void);
	/**
	 * @brief Inform the Gui that we want to have a copy of the clipboard
	 * @param ID of the clipboard (STD/SELECTION) only apear here
	 */
	void ClipBoardGet(ewol::clipBoard::clipboardListe_te clipboardID);
	/**
	 * @brief Inform the Gui that we are the new owner of the clipboard
	 * @param ID of the clipboard (STD/SELECTION) only apear here
	 */
	void ClipBoardSet(ewol::clipBoard::clipboardListe_te clipboardID);
	/**
	 * @brief Set the new title of the windows
	 * @param title New desired title
	 */
	void SetTitle(etk::UString& title);
	/**
	 * @brief Force the screen orientation (availlable on portable elements ...
	 * @param orientation Selected orientation.
	 */
	void ForceOrientation(ewol::orientation_te orientation);
	/**
	 * @brief Get all the event from the X system
	 * @param[in] isGrabbed "true" if all the event will be get, false if we want only ours.
	 * @param[in] forcedPosition the position where the mouse might be reset at  every events ...
	 */
	void GrabPointerEvents(bool isGrabbed, vec2 forcedPosition);
	/**
	 * @brief Set the cursor display type.
	 * @param[in] newCursor selected new cursor.
	 */
	void SetCursor(ewol::cursorDisplay_te newCursor);
};


//!< must be define in CPP by the application ... this are the main init and unInit of the Application
void         APP_Init(void);
void         APP_UnInit(void);
etk::UString APP_Icon(void);

#define NB_MAX_INPUT                  (20)

#endif
