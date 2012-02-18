/**
 *******************************************************************************
 * @file gui.h
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


#ifndef __GUI_ABSTRACTION_H__
#define __GUI_ABSTRACTION_H__

#include <etk/Types.h>
#include <etk/UString.h>
#include <ewol/Windows.h>

void EWOL_NativeResize(int w, int h );
void EWOL_GenericDraw(bool everyTime);
void EWOL_NativeGLDestroy(void);

namespace guiAbstraction
{
	void Stop(void);
	void SetDisplayOnWindows(ewol::Windows * newOne);
	void ChangeSize(int32_t w, int32_t h);
	void ChangePos(int32_t x, int32_t y);
	void GetAbsPos(int32_t & x, int32_t & y);
	bool IsPressedInput(int32_t inputID);
	void KeyboardShow(ewol::keyboardMode_te mode);
	void KeyboardHide(void);
	void ForceRedrawAll(void);
	void SendKeyboardEvent(bool isDown, uniChar_t keyInput);
	void SendKeyboardEventMove(bool isDown, ewol::eventKbMoveType_te &keyInput);
};


//!< must be define in CPP by the application ...
void APP_Init(void);
void APP_UnInit(void);

#define NB_MAX_INPUT                  (20)

#endif
