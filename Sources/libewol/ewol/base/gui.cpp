/**
 *******************************************************************************
 * @file gui.cpp
 * @brief Gui abstraction layer common part (Sources)
 * @author Edouard DUPIN
 * @date 01/02/2012
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
#include <etk/UString.h>
#include <ewol/EObject.h>
#include <ewol/EObjectManager.h>
#include <ewol/WidgetManager.h>
#include <ewol/base/gui.h>
#include <ewol/ewol.h>

#include <ewol/Texture.h>
#include <ewol/base/MainThread.h>

ewol::Windows* gui_uniqueWindows = NULL;
float     gui_width = 320;
float     gui_height = 480;


int32_t ewol::GetCurrentWidth(void)
{
	return gui_width;
}

int32_t ewol::GetCurrentHeight(void)
{
	return gui_height;
}


void ewol::PopUpWidgetPush(ewol::Widget * tmpWidget)
{
	if (NULL != gui_uniqueWindows && NULL != tmpWidget) {
		gui_uniqueWindows->PopUpWidgetPush(tmpWidget);
	}
}

void UpdateGuiSize(void)
{
	if (NULL != gui_uniqueWindows) {
		gui_uniqueWindows->CalculateSize((float)gui_width, (float)gui_height);
		gui_uniqueWindows->SetOrigin(0.0, 0.0);
	}
}
void EWOL_NativeResize(int w, int h )
{
	gui_width = w;
	gui_height = h;
	//EWOL_INFO("Resize w=" << w << " h=" << h);
	UpdateGuiSize();
}

void EWOL_NativeRegenerateDisplay(void)
{
	//EWOL_INFO("Resize w=" << w << " h=" << h);
	if (NULL != gui_uniqueWindows) {
		// Redraw all needed elements
		gui_uniqueWindows->OnRegenerateDisplay();
		// Keep Inter-thread-lock-mutex
		ewol::widgetManager::DoubleBufferLock();
		// Inform the main thread of openGl draw that somthing to display
		ewol::widgetManager::SetDoubleBufferNeedDraw();
		// Release Inter-thread-lock-mutex
		ewol::widgetManager::DoubleBufferUnLock();
		// Remove deprecated widget (which have no more reference in the system)
		ewol::EObjectManager::RemoveAllMark();
	}
}

void guiAbstraction::SetDisplayOnWindows(ewol::Windows * newWindows)
{
	gui_uniqueWindows = newWindows;
	if (NULL != gui_uniqueWindows) {
		gui_uniqueWindows->CalculateSize((float)gui_width, (float)gui_height);
	}
}

void guiAbstraction::ForceRedrawAll(void)
{
	if (NULL != gui_uniqueWindows) {
		gui_uniqueWindows->CalculateSize((float)gui_width, (float)gui_height);
	}
}


void guiAbstraction::SendKeyboardEvent(bool isDown, uniChar_t keyInput)
{
	// Get the current Focused Widget :
	ewol::Widget * tmpWidget = ewol::widgetManager::FocusGet();
	if (NULL != tmpWidget) {
		if(true == isDown) {
			EWOL_VERBOSE("GUI PRESSED : \"" << keyInput << "\"");
			tmpWidget->OnEventKb(ewol::EVENT_KB_TYPE_DOWN, keyInput);
		} else {
			EWOL_VERBOSE("GUI Release : \"" << keyInput << "\"");
			tmpWidget->OnEventKb(ewol::EVENT_KB_TYPE_UP, keyInput);
		}
	}
}

void guiAbstraction::SendKeyboardEventMove(bool isDown, ewol::eventKbMoveType_te &keyInput)
{
	// Get the current Focused Widget :
	ewol::Widget * tmpWidget = ewol::widgetManager::FocusGet();
	if (NULL != tmpWidget) {
		if(true == isDown) {
			tmpWidget->OnEventKbMove(ewol::EVENT_KB_TYPE_DOWN, keyInput);
		} else {
			tmpWidget->OnEventKbMove(ewol::EVENT_KB_TYPE_UP, keyInput);
		}
	}
}


/*
void guiAbstraction::KeyboardShow(ewol::keyboardMode_te mode)
{
	if (NULL != gui_uniqueWindows) {
		gui_uniqueWindows->KeyboardShow(mode);
	}
}

void guiAbstraction::KeyboardHide(void)
{
	if (NULL != gui_uniqueWindows) {
		gui_uniqueWindows->KeyboardHide();
	}
	ForceRedrawAll();
}
*/






static int64_t startTime = -1;
static int64_t nbCallTime = 0;
static int64_t nbDisplayTime = 0;
static int64_t min = 99999999999999;
static int64_t avg = 0;
static int64_t max = 0;
static int64_t min2 = 99999999999999;
static int64_t avg2 = 0;
static int64_t max2 = 0;
// display every second ...
#define DISPLAY_PERIODE_US       (1000000)


void EWOL_GenericDraw(bool everyTime)
{
	bool display = false;
	nbCallTime++;
	if (startTime<0) {
		startTime = GetCurrentTime();
	}
	int64_t currentTime = GetCurrentTime();
	//EWOL_DEBUG("current : " << currentTime << "time    diff : " << (currentTime - startTime));
	if ( (currentTime - startTime) > DISPLAY_PERIODE_US) {
		display = true;
	}
	// TODO : Remove this ...
	if (ewol::widgetManager::PeriodicCallHave()) {
		everyTime = true;
	}
	ewol::widgetManager::DoubleBufferLock();
	int64_t currentTime3 = GetCurrentTime();
	if(    true == ewol::widgetManager::GetDoubleBufferNeedDraw()
	    || true == everyTime)
	{
		ewol::texture::UpdateContext();
		nbDisplayTime++;
		gui_uniqueWindows->SysDraw();
	}
	ewol::widgetManager::DoubleBufferUnLock();
	// send Message that we just finished a display ...
	EWOL_ThreadEventHasJustDisplay();
	int64_t currentTime2 = GetCurrentTime();
	int64_t processTimeLocal = (currentTime2 - currentTime);
	min = etk_min(min, processTimeLocal);
	max = etk_max(max, processTimeLocal);
	avg += processTimeLocal;
	processTimeLocal = (currentTime2 - currentTime3);
	min2 = etk_min(min2, processTimeLocal);
	max2 = etk_max(max2, processTimeLocal);
	avg2 += processTimeLocal;
	if (true == display) {
		EWOL_DEBUG("display property : " << nbDisplayTime << "/" << nbCallTime << "fps");
		EWOL_DEBUG("timeToProcess1 : " << (float)((float)min / 1000.0) << "ms "
		                              << (float)((float)avg/(float)nbDisplayTime / 1000.0) << "ms "
		                              << (float)((float)max / 1000.0) << "ms ");
		EWOL_DEBUG("timeToProcess2 : " << (float)((float)min2 / 1000.0) << "ms "
		                              << (float)((float)avg2/(float)nbDisplayTime / 1000.0) << "ms "
		                              << (float)((float)max2 / 1000.0) << "ms ");
		max2 = 0;
		max = 0;
		min = 99999999999999;
		min2 = 99999999999999;
		avg=0;
		avg2=0;
		
		nbCallTime = 0;
		nbDisplayTime = 0;
		startTime = -1;
	}
}

void EWOL_NativeGLDestroy(void)
{
	ewol::texture::UpdateContextIsDestroy();
}


#include <ewol/importgl.h>


void glOrthoEwol(GLfloat left,
             GLfloat right,
             GLfloat bottom,
             GLfloat top,
             GLfloat nearVal,
             GLfloat farVal)
{
	GLfloat myMatrix[4*4];
	int iii;
	for(iii=0; iii<4*4 ; iii++) {
		myMatrix[iii] = 0;
	}
	myMatrix[0] = 2.0 / (right - left);
	myMatrix[5] = 2.0 / (top - bottom);
	myMatrix[10] = -2.0 / (farVal - nearVal);
#if 1
	myMatrix[3]  = -1*(right + left) / (right - left);
	myMatrix[7]  = -1*(top + bottom) / (top - bottom);
	myMatrix[11] = -1*(farVal + nearVal) / (farVal - nearVal);
#else
	// test if matrix is not corectly instanciate ...
	myMatrix[12]  = -1*(right + left) / (right - left);
	myMatrix[13]  = -1*(top + bottom) / (top - bottom);
	myMatrix[14] = -1*(farVal + nearVal) / (farVal - nearVal);
#endif
	myMatrix[15] = 1;

	glLoadMatrixf(myMatrix);


}
