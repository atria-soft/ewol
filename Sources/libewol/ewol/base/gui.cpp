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
#include <etk/String.h>
#include <ewol/WidgetManager.h>
#include <ewol/base/gui.h>

#include <ewol/Texture.h>
#include <ewol/base/MainThread.h>

ewol::Windows* gui_uniqueWindows = NULL;
etkFloat_t     gui_width = 320;
etkFloat_t     gui_height = 480;

void EWOL_NativeResize(int w, int h )
{
	gui_width = w;
	gui_height = h;
	//EWOL_INFO("Resize w=" << w << " h=" << h);
	if (NULL != gui_uniqueWindows) {
		gui_uniqueWindows->CalculateSize((etkFloat_t)gui_width, (etkFloat_t)gui_height);
		gui_uniqueWindows->SetOrigin(0.0, 0.0);
	}
}

void EWOL_NativeRegenerateDisplay(void)
{
	//EWOL_INFO("Resize w=" << w << " h=" << h);
	if (NULL != gui_uniqueWindows) {
		gui_uniqueWindows->OnRegenerateDisplay();
	}
}

void guiAbstraction::SetDisplayOnWindows(ewol::Windows * newWindows)
{
	gui_uniqueWindows = newWindows;
	if (NULL != gui_uniqueWindows) {
		gui_uniqueWindows->CalculateSize((etkFloat_t)gui_width, (etkFloat_t)gui_height);
	}
}

void guiAbstraction::ForceRedrawAll(void)
{
	if (NULL != gui_uniqueWindows) {
		gui_uniqueWindows->CalculateSize((etkFloat_t)gui_width, (etkFloat_t)gui_height);
	}
}


void guiAbstraction::SendKeyboardEvent(bool isDown, etk::String &keyInput)
{
	// Get the current Focused Widget :
	ewol::Widget * tmpWidget = ewol::widgetManager::FocusGet();
	if (NULL != tmpWidget) {
		if(true == isDown) {
			EWOL_VERBOSE("X11 PRESSED : \"" << keyInput << "\" size=" << keyInput.Size());
			tmpWidget->OnEventKb(ewol::EVENT_KB_TYPE_DOWN, keyInput.c_str());
		} else {
			EWOL_VERBOSE("X11 Release : \"" << keyInput << "\" size=" << keyInput.Size());
			tmpWidget->OnEventKb(ewol::EVENT_KB_TYPE_UP, keyInput.c_str());
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


// defined by the platform specific file : 
extern int32_t separateClickTime;
extern int32_t offsetMoveClicked;
extern int32_t offsetMoveClickedDouble;


static int32_t m_previousBouttonId = -1;
static int32_t m_previousDown_x = -1;
static int32_t m_previousDown_y = -1;
static int32_t m_previous_x = -1;
static int32_t m_previous_y = -1;
static int64_t m_previousTime = 0;
static bool    m_previousDouble = false;

void EWOL_NativeEventInputMotion(int pointerID, float x, float y )
{
	//EWOL_INFO("Event : Input Motion ID=" << pointerID << " x=" << x << " y=" << y);
	if(0<=pointerID && pointerID < NB_MAX_INPUT ) {
		if(NULL != gui_uniqueWindows) {
			//EWOL_DEBUG("Event: bt=" << pointerID+1 << " ** = \"MotionNotify\" (" << (etkFloat_t)x << "," << (etkFloat_t)y << ")");
			gui_uniqueWindows->GenEventInput(pointerID, ewol::EVENT_INPUT_TYPE_MOVE, (etkFloat_t)x, (etkFloat_t)y);
		}
	}
}

void EWOL_NativeEventInputState(int pointerID, bool isUp, float x, float y )
{
	//EWOL_INFO("GUI : Input ID=" << pointerID << " [" << isUp << "] x=" << x << " y=" << y);
	if (isUp) {
		//EWOL_DEBUG("GUI : Input ID=" << pointerID << " [DOWN] x=" << x << " y=" << y);
		if(0<=pointerID && pointerID < NB_MAX_INPUT ) {
			// Send Down message
			if (NULL != gui_uniqueWindows) {
				EWOL_VERBOSE("GUI : Input ID=" << pointerID << " [DOWN]   (" << (etkFloat_t)x << "," << (etkFloat_t)y << ")");
				gui_uniqueWindows->GenEventInput(pointerID, ewol::EVENT_INPUT_TYPE_DOWN, (etkFloat_t)x, (etkFloat_t)y);
			}
			// Check double or triple click event ...
			m_previousDown_x = x;
			m_previousDown_y = y;
			if (m_previousBouttonId != pointerID) {
				m_previousBouttonId = pointerID;
				m_previous_x = -1;
				m_previous_y = -1;
				m_previousTime = 0;
				m_previousDouble = false;
			} else {
				if(    abs(m_previous_x - x) < offsetMoveClicked
				    && abs(m_previous_y - y) < offsetMoveClicked )
				{
					// nothink to do ... wait up ...
				} else {
					m_previous_x = -1;
					m_previous_y = -1;
					m_previousTime = 0;
					m_previousDouble = false;
				}
			}
		}
	} else {
		//EWOL_DEBUG("Event : Input ID=" << pointerID << " [UP]   x=" << x << " y=" << y);
		if(0<=pointerID && pointerID < NB_MAX_INPUT ) {
			// Send Down message
			if (NULL != gui_uniqueWindows) {
				EWOL_VERBOSE("GUI : Input ID=" << pointerID << " [UP]     (" << (etkFloat_t)x << "," << (etkFloat_t)y << ")");
				gui_uniqueWindows->GenEventInput(pointerID, ewol::EVENT_INPUT_TYPE_UP, (etkFloat_t)x, (etkFloat_t)y);
			}
			if (m_previousBouttonId != pointerID) {
				m_previousDown_x = -1;
				m_previousDown_y = -1;
				m_previousBouttonId = 0;
				m_previous_x = -1;
				m_previous_y = -1;
				m_previousTime = 0;
				m_previousDouble = false;
			} else {
				int64_t currentTime = GetCurrentTime(); // return the tic in 1ms
				EWOL_VERBOSE("time is : " << (int)currentTime << "    "<< (int)(currentTime/1000) <<"s " << (int)((currentTime%100)*10) << "ms    delta : " << (currentTime - m_previousTime) << "<" << separateClickTime );
				if (currentTime - m_previousTime >= separateClickTime) {
					//check if the same area click : 
					if(    abs(m_previousDown_x - x) < offsetMoveClicked
					    && abs(m_previousDown_y - y) < offsetMoveClicked )
					{
						// might generate an sigle event :
						EWOL_VERBOSE("GUI : Input ID=" << pointerID << " [SINGLE] (" << (etkFloat_t)x << "," << (etkFloat_t)y << ")");
						gui_uniqueWindows->GenEventInput(pointerID, ewol::EVENT_INPUT_TYPE_SINGLE, (etkFloat_t)x, (etkFloat_t)y);
						m_previous_x = m_previousDown_x;
						m_previous_y = m_previousDown_y;
						m_previousTime = currentTime;
					} else {
						// reset values ...
						m_previousDown_x = -1;
						m_previousDown_y = -1;
						m_previousBouttonId = 0;
						m_previous_x = -1;
						m_previous_y = -1;
						m_previousTime = 0;
					}
					m_previousDouble = false;
				} else {
					// TODO : the double ckick does not work, I need to check this later ... if needed
					//check if the same area click : 
					if(    abs(m_previous_x - x) < offsetMoveClickedDouble
					    && abs(m_previous_y - y) < offsetMoveClickedDouble )
					{
						// might generate an sigle event :
						if (false == m_previousDouble) {
							EWOL_VERBOSE("GUI : Input ID=" << pointerID << " [DOUBLE] (" << (etkFloat_t)x << "," << (etkFloat_t)y << ")");
							gui_uniqueWindows->GenEventInput(pointerID, ewol::EVENT_INPUT_TYPE_DOUBLE, (etkFloat_t)x, (etkFloat_t)y);
							m_previousTime = currentTime;
							m_previousDouble = true;
						} else {
							EWOL_VERBOSE("GUI : Input ID=" << pointerID << " [TRIPLE] (" << (etkFloat_t)x << "," << (etkFloat_t)y << ")");
							gui_uniqueWindows->GenEventInput(pointerID, ewol::EVENT_INPUT_TYPE_TRIPLE, (etkFloat_t)x, (etkFloat_t)y);
							// reset values ...
							m_previousDown_x = -1;
							m_previousDown_y = -1;
							m_previousBouttonId = 0;
							m_previous_x = -1;
							m_previous_y = -1;
							m_previousTime = 0;
							m_previousDouble = false;
						}
					} else {
						// reset values ...
						m_previousDown_x = -1;
						m_previousDown_y = -1;
						m_previousBouttonId = 0;
						m_previous_x = -1;
						m_previous_y = -1;
						m_previousTime = 0;
						m_previousDouble = false;
					}
				}
			}
		}
	}
}

static int64_t startTime = -1;
static int64_t nbCallTime = 0;
static int64_t nbDisplayTime = 0;
#define DISPLAY_PERIODE_MS       (1000)


void EWOL_GenericDraw(bool everyTime)
{
	bool display = false;
	nbCallTime++;
	if (startTime<0) {
		startTime = GetCurrentTime();
	}
	int64_t currentTime = GetCurrentTime();
	//EWOL_DEBUG("current : " << currentTime << "time    diff : " << (currentTime - startTime));
	if ( (currentTime - startTime) > DISPLAY_PERIODE_MS) {
		display = true;
	}
	ewol::widgetManager::GetDoubleBufferStartDraw();
	if(    true == ewol::widgetManager::GetDoubleBufferNeedDraw()
	    || true == everyTime)
	{
		nbDisplayTime++;
		gui_uniqueWindows->SysDraw();
	}
	ewol::widgetManager::GetDoubleBufferStopDraw();
	// send Message that we just finished a display ...
	EWOL_ThreadEventHasJustDisplay();
	
	if (true == display) {
		EWOL_DEBUG("display property : " << (int32_t)((double)nbDisplayTime/(double)DISPLAY_PERIODE_MS*(double)1000) << "/" << (int32_t)((double)nbCallTime/(double)DISPLAY_PERIODE_MS*(double)1000) << "fps");
		nbCallTime = 0;
		nbDisplayTime = 0;
		startTime = -1;
	}
}

void EWOL_NativeGLDestroy(void)
{
	ewol::UpdateTextureContextIsDestroy();
}

