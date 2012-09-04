/**
 *******************************************************************************
 * @file os/eSystem.h
 * @brief Main Ewol system of management of the input events and drawing request (Header)
 * @author Edouard DUPIN
 * @date 27/01/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *O
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */



#ifndef __EWOL_SYSTEM_H__
#define __EWOL_SYSTEM_H__

#include <ewol/ewol.h>

namespace eSystem
{
	void Init(void);
	void UnInit(void);
	
	void SetArchiveDir(int mode, const char* str);
	
	void Resize(int w, int h );
	void Move(int x, int y );
	void Hide(void);
	void Show(void);
	
	void SetInputMotion(int pointerID, float x, float y);
	void SetInputState(int pointerID, bool isUp, float x, float y);
	
	void SetMouseMotion(int pointerID, float x, float y);
	void SetMouseState(int pointerID, bool isUp, float x, float y);
	
	typedef struct {
		bool capLock;
		bool shift;
		bool ctrl;
		bool meta;
		bool alt;
		bool altGr;
		bool verNum;
		bool insert;
	} specialKey_ts;
	
	typedef struct {
		bool                      isDown;
		uniChar_t                 myChar;
		eSystem::specialKey_ts    special;
	} keyboardKey_ts;
	
	typedef struct {
		bool                      isDown;
		ewol::eventKbMoveType_te  move;
		eSystem::specialKey_ts    special;
	} keyboardMove_ts;
	
	void SetKeyboard(eSystem::keyboardKey_ts& keyInput);
	void SetKeyboardMove(eSystem::keyboardMove_ts& keyInput);
	
	void ClipBoardArrive(ewol::clipBoard::clipboardListe_te clipboardID);

	// return true if a flush is needed
	bool Draw(bool displayEveryTime);
	
	/**
	 * @brief Inform object that an other object is removed ...
	 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
	 * @note : Sub classes must call this class
	 * @return ---
	 */
	void OnObjectRemove(ewol::EObject * removeObject);
	
	/**
	 * @brief reset event management for the IO like Input ou Mouse or keyborad
	 * @param ---
	 * @return ---
	 */
	void ResetIOEvent(void);
	
	/**
	 * @brief Inform the system that the OpenGL constext has been destroy ==> use to automaticly reload the texture and other thinks ...
	 * @param ---
	 * @return ---
	 */
	void OpenGlContextDestroy(void);
	
	/**
	 * @brief Inform the system that the Application has been killed
	 * @param ---
	 * @return ---
	 */
	void OnKill(void);
	
	/**
	 * @brief set the current windows to display :
	 * @param windows windows that might be displayed
	 * @return ---
	 */
	void SetCurrentWindows(ewol::Windows * windows);
	
	/**
	 * @brief Get the current windows that is displayed
	 * @param ---
	 * @return the current handle on the windows (can be null)
	 */
	ewol::Windows* GetCurrentWindows(void);
	
	/**
	 * @brief Get the current windows size
	 * @param ---
	 * @return the current size ...
	 */
	Vector2D<int32_t> GetSize(void);
	
	/**
	 * @brief Redraw all the windows
	 * @param ---
	 * @return ---
	 */
	void ForceRedrawAll(void);
	
	/**
	 * @brief This is to transfert the event from one widget to another one
	 * @param source the widget where the event came from
	 * @param destination the widget where the event mitgh be generated now
	 * @return ---
	 */
	void InputEventTransfertWidget(ewol::Widget* source, ewol::Widget* destination);
};


#endif

