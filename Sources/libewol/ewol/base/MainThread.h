/**
 *******************************************************************************
 * @file MainThread.h
 * @brief Main Ewol thread for the abstraction of the OS problematics (Header)
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



#ifndef __EWOL_MAIN_TREAD_H__
#define __EWOL_MAIN_TREAD_H__

#include <ewol/ewol.h>

namespace guiSystem
{
	
	void Init(void);
	void UnInit(void);
	
	void SetArchiveDir(int mode, const char* str);
	
	namespace event {
		void Resize(int w, int h );
		
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
			guiSystem::event::specialKey_ts special;
		} keyboardKey_ts;
		
		typedef struct {
			bool                      isDown;
			ewol::eventKbMoveType_te  move;
			guiSystem::event::specialKey_ts special;
		} keyboardMove_ts;
		
		void SetKeyboard(guiSystem::event::keyboardKey_ts& keyInput);
		void SetKeyboardMove(guiSystem::event::keyboardMove_ts& keyInput);
		
		void Hide(void);
		void Show(void);
		
		void ClipBoardArrive(ewol::clipBoard::clipboardListe_te clipboardID);
	};
	// return true if a flush is needed
	bool Draw(bool displayEveryTime);
};

#endif

