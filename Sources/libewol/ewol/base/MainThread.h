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
 *
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
void EWOL_ThreadEventInputMotion(int pointerID, float x, float y );
void EWOL_ThreadEventInputState(int pointerID, bool isUp, float x, float y );



#endif

