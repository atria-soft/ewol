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

#include <pthread.h>

void EWOL_SystemStart(void);
void EWOL_SystemStop(void);


void EWOL_ThreadSetArchiveDir(int mode, const char* str);
void EWOL_ThreadResize(int w, int h );
void EWOL_ThreadEventInputMotion(int pointerID, float x, float y);
void EWOL_ThreadEventInputState(int pointerID, bool isUp, float x, float y);
void EWOL_ThreadEventMouseMotion(int pointerID, float x, float y);
void EWOL_ThreadEventMouseState(int pointerID, bool isUp, float x, float y);



typedef struct {
	bool capLock;
	bool shift;
	bool ctrl;
	bool meta;
	bool alt;
	bool altGr;
	bool verNum;
	bool insert;
} eventSpecialKey_ts;

typedef struct {
	bool               isDown;
	uniChar_t          myChar;
	eventSpecialKey_ts special;
} eventKeyboardKey_ts;

typedef struct {
	bool                     isDown;
	ewol::eventKbMoveType_te move;
	eventSpecialKey_ts       special;
} eventKeyboardMove_ts;

void EWOL_ThreadKeyboardEvent(eventKeyboardKey_ts& keyInput);
void EWOL_ThreadKeyboardEventMove(eventKeyboardMove_ts& keyInput);


void EWOL_ThreadEventHide(void);
void EWOL_ThreadEventShow(void);
void EWOL_ThreadEventHasJustDisplay(void);

#endif

