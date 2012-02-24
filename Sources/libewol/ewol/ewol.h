/**
 *******************************************************************************
 * @file ewol/ewol.h
 * @brief Main include of ewol (header)
 * @author Edouard DUPIN
 * @date 17/10/2011
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


#ifndef __EWOL_H__
#define __EWOL_H__

#include <etk/Types.h>
#include <etk/UString.h>
#include <ewol/Widget.h>
#include <ewol/Windows.h>


namespace ewol {
	void Stop(void);
	void DisplayWindows(ewol::Windows * windows);
	void ChangeSize(int32_t w, int32_t h);
	void ChangePos(int32_t x, int32_t y);
	void GetAbsPos(int32_t & x, int32_t & y);
	bool IsPressedInput(int32_t inputID);
	void KeyboardShow(ewol::keyboardMode_te mode);
	void KeyboardHide(void);
	void ForceRedrawAll(void);
	void PopUpWidgetPush(ewol::Widget * tmpWidget);
	void RmPopUp(int32_t widgetID);
	int32_t      CmdLineNb(void);
	etk::UString CmdLineGet(int32_t id);
	
	bool IsSetCapsLock(void);
	bool IsSetShift(void);
	bool IsSetCtrl(void);
	bool IsSetMeta(void);
	bool IsSetAlt(void);
	bool IsSetAltGr(void);
	bool IsSetVerNum(void);
	bool IsSetInsert(void);
	namespace shortCut {
		void Add(bool shift, bool control, bool alt, bool meta, uniChar_t unicodeValue, const char * generateEventId, etk::UString data);
		void Add(const char * descriptiveString, const char * generateEventId, etk::UString data);
	};
	
	int32_t GetCurrentWidth(void);
	int32_t GetCurrentHeight(void);
};

int64_t GetCurrentTime(void);


#endif
