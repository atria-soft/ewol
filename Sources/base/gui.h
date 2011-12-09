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
#include <ewol/Windows.h>

namespace guiAbstraction
{
	void Init(int32_t argc, char *argv[]);
	void Run(void);
	void Stop(void);
	void UnInit(void);
	void SetDisplayOnWindows(ewol::Windows * newOne);
	void ChangeSize(int32_t w, int32_t h);
	void ChangePos(int32_t x, int32_t y);
	void GetAbsPos(int32_t & x, int32_t & y);
	void StartResizeSystem(void);
	void StartMoveSystem(void);
	bool IsPressedInput(int32_t inputID);
	void ForceRedrawAll(void);
};

//!< must be define in CPP by the application ...
void APP_Init(int argc, char *argv[]);
void APP_UnInit(void);

#endif
