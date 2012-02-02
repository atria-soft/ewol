/**
 *******************************************************************************
 * @file MainThread.cpp
 * @brief Main Ewol thread for the abstraction of the OS problematics (Sources)
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


#include <ewol/ewol.h>
#include <ewol/Debug.h>
#include <ewol/threadMsg.h>
#include <ewol/base/MainThread.h>
#include <ewol/base/gui.h>
#include <ewol/Texture.h>
#include <ewol/WidgetManager.h>
#include <ewol/themeManager.h>



static ewol::threadMsg::threadMsg_ts    androidJniMsg;
static pthread_t                        androidJniThread;
//static pthread_attr_t                   androidJniThreadAttr;

enum {
	THREAD_INIT,
	THREAD_UN_INIT,
	THREAD_RESIZE,
	THREAD_HIDE,
	THREAD_SHOW,
	
	THREAD_INPUT_MOTION,
	THREAD_INPUT_STATE,
	
	THREAD_KEYBORAD_KEY,
	THREAD_KEYBORAD_MOVE,
	THREAD_JUST_DISPLAY,
};


#include <unistd.h>

typedef struct {
	int w;
	int h;
} eventResize_ts;

typedef struct {
	int pointerID;
	float x;
	float y;
} eventInputMotion_ts;

typedef struct {
	int pointerID;
	bool state;
	float x;
	float y;
} eventInputState_ts;

typedef struct {
	bool      isDown;
	uniChar_t myChar;
} eventKeyboardKey_ts;

typedef struct {
	bool                     isDown;
	ewol::eventKbMoveType_te move;
} eventKeyboardMove_ts;

void EWOL_NativeEventInputMotion(int pointerID, float x, float y );
void EWOL_NativeEventInputState(int pointerID, bool isUp, float x, float y );
void EWOL_NativeResize(int w, int h );



static void* BaseAppEntry(void* param)
{
	bool requestEndProcessing = false;
	EWOL_DEBUG("BThread Init (START)");
	
	EWOL_INFO("v" EWOL_VERSION_TAG_NAME);
	EWOL_INFO("Build Date: " VERSION_BUILD_TIME);
	ewol::widgetManager::Init();
	ewol::texture::Init();
	ewol::theme::Init();
	ewol::InitFont();
	APP_Init();
	int32_t countNbEvent = 0;
	EWOL_DEBUG("BThread Init (END)");
	while(false == requestEndProcessing) {
		ewol::threadMsg::threadMsgContent_ts data;
		ewol::threadMsg::WaitMessage(androidJniMsg, data);
		if (data.type != THREAD_JUST_DISPLAY) {
			countNbEvent++;
			//EWOL_DEBUG("EVENT");
			switch (data.type) {
				case THREAD_INIT:
					EWOL_DEBUG("Receive MSG : THREAD_INIT");
					break;
				case THREAD_UN_INIT:
					EWOL_DEBUG("Receive MSG : THREAD_UN_INIT");
					requestEndProcessing = true;
					break;
				case THREAD_RESIZE:
					//EWOL_DEBUG("Receive MSG : THREAD_RESIZE");
					{
						eventResize_ts * tmpData = (eventResize_ts*)data.data;
						EWOL_NativeResize(tmpData->w, tmpData->h);
					}
					break;
				case THREAD_INPUT_MOTION:
					//EWOL_DEBUG("Receive MSG : THREAD_INPUT_MOTION");
					{
						eventInputMotion_ts * tmpData = (eventInputMotion_ts*)data.data;
						EWOL_NativeEventInputMotion(tmpData->pointerID, tmpData->x, tmpData->y);
					}
					break;
				case THREAD_INPUT_STATE:
					//EWOL_DEBUG("Receive MSG : THREAD_INPUT_STATE");
					{
						eventInputState_ts * tmpData = (eventInputState_ts*)data.data;
						EWOL_NativeEventInputState(tmpData->pointerID, tmpData->state, tmpData->x, tmpData->y);
					}
					break;
				case THREAD_KEYBORAD_KEY:
					EWOL_DEBUG("Receive MSG : THREAD_KEYBORAD_KEY");
					{
						eventKeyboardKey_ts * tmpData = (eventKeyboardKey_ts*)data.data;
						etk::String keyInput = "a";
						keyInput.c_str()[0] = tmpData->myChar;
						guiAbstraction::SendKeyboardEvent(tmpData->isDown, keyInput);
					}
					break;
				case THREAD_KEYBORAD_MOVE:
					EWOL_DEBUG("Receive MSG : THREAD_KEYBORAD_MOVE");
					{
						eventKeyboardMove_ts * tmpData = (eventKeyboardMove_ts*)data.data;
						guiAbstraction::SendKeyboardEventMove(tmpData->isDown, tmpData->move);
					}
					break;
				case THREAD_HIDE:
					EWOL_DEBUG("Receive MSG : THREAD_HIDE");
					//guiAbstraction::SendKeyboardEventMove(tmpData->isDown, tmpData->move);
					//gui_uniqueWindows->SysOnHide();
					break;
				case THREAD_SHOW:
					EWOL_DEBUG("Receive MSG : THREAD_SHOW");
					//guiAbstraction::SendKeyboardEventMove(tmpData->isDown, tmpData->move);
					//gui_uniqueWindows->SysOnShow();
					break;
				default:
					EWOL_DEBUG("Receive MSG : UNKNOW");
					break;
			}
		}
		if (0 == ewol::threadMsg::WaitingMessage(androidJniMsg)) {
			if (countNbEvent > 0) {
				// TODO : Generate the display here ... Instead of every time we call the sub-Widget ...
				ewol::widgetManager::GetDoubleBufferFlipFlop();
				countNbEvent = 0;
			}
		}
	}
	EWOL_DEBUG("BThread Un-Init (START)");
	
	// unset all windows
	ewol::DisplayWindows(NULL);
	// call application to uninit
	APP_UnInit();
	
	ewol::texture::UnInit();
	ewol::UnInitFont();
	ewol::widgetManager::UnInit();
	ewol::theme::UnInit();
	EWOL_DEBUG("BThread Un-Init (END)");
	pthread_exit(NULL);
}


void EWOL_ThreadSetArchiveDir(int mode, const char* str)
{
	switch(mode)
	{
		case 0:
			EWOL_DEBUG("Directory APK : path=" << str);
			etk::SetBaseFolderData(str);
			break;
		case 1:
			EWOL_DEBUG("Directory mode=FILE path=" << str);
			etk::SetBaseFolderDataUser(str);
			break;
		case 2:
			EWOL_DEBUG("Directory mode=CACHE path=" << str);
			etk::SetBaseFolderCache(str);
			break;
		case 3:
			EWOL_DEBUG("Directory mode=EXTERNAL_CACHE path=" << str);
			break;
		default:
			EWOL_DEBUG("Directory mode=???? path=" << str);
			break;
	}
}


bool isGlobalSystemInit = false;

void EWOL_SystemStart(void)
{
	if (false == isGlobalSystemInit) {
		// create interface mutex :
		ewol::threadMsg::Init(androidJniMsg);
		// init the thread :
		pthread_create(&androidJniThread, NULL, BaseAppEntry, NULL);
		isGlobalSystemInit = true;
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_INIT, ewol::threadMsg::MSG_PRIO_REAL_TIME);
	}
}

void EWOL_SystemStop(void)
{
	if (true == isGlobalSystemInit) {
		isGlobalSystemInit = false;
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_UN_INIT, ewol::threadMsg::MSG_PRIO_REAL_TIME);
		
		EWOL_DEBUG("Wait end of the thread ...");
		// Wait end of the thread
		pthread_join(androidJniThread, NULL);
		ewol::threadMsg::UnInit(androidJniMsg);
	}
}



void EWOL_ThreadResize(int w, int h )
{
	eventResize_ts tmpData;
	tmpData.w = w;
	tmpData.h = h;
	ewol::threadMsg::SendMessage(androidJniMsg, THREAD_RESIZE, ewol::threadMsg::MSG_PRIO_MEDIUM, &tmpData, sizeof(eventResize_ts) );
}

void EWOL_ThreadEventInputMotion(int pointerID, float x, float y )
{
	eventInputMotion_ts tmpData;
	tmpData.pointerID = pointerID;
	tmpData.x = x;
	tmpData.y = y;
	ewol::threadMsg::SendMessage(androidJniMsg, THREAD_INPUT_MOTION, ewol::threadMsg::MSG_PRIO_LOW, &tmpData, sizeof(eventInputMotion_ts) );
}


void EWOL_ThreadEventInputState(int pointerID, bool isUp, float x, float y )
{
	eventInputState_ts tmpData;
	tmpData.pointerID = pointerID;
	tmpData.state = isUp;
	tmpData.x = x;
	tmpData.y = y;
	ewol::threadMsg::SendMessage(androidJniMsg, THREAD_INPUT_STATE, ewol::threadMsg::MSG_PRIO_LOW, &tmpData, sizeof(eventInputState_ts) );
}

void EWOL_ThreadKeyboardEvent(bool isDown, etk::String &keyInput)
{
	eventKeyboardKey_ts tmpData;
	tmpData.isDown = isDown;
	tmpData.myChar = keyInput.c_str()[0];
	ewol::threadMsg::SendMessage(androidJniMsg, THREAD_KEYBORAD_KEY, ewol::threadMsg::MSG_PRIO_LOW, &tmpData, sizeof(eventKeyboardKey_ts) );
}

void EWOL_ThreadKeyboardEventMove(bool isDown, ewol::eventKbMoveType_te &keyInput)
{
	eventKeyboardMove_ts tmpData;
	tmpData.isDown = isDown;
	tmpData.move = keyInput;
	ewol::threadMsg::SendMessage(androidJniMsg, THREAD_KEYBORAD_MOVE, ewol::threadMsg::MSG_PRIO_LOW, &tmpData, sizeof(eventKeyboardMove_ts) );
}

void EWOL_ThreadEventHide(void)
{
	ewol::threadMsg::SendMessage(androidJniMsg, THREAD_HIDE, ewol::threadMsg::MSG_PRIO_LOW);
}

void EWOL_ThreadEventShow(void)
{
	ewol::threadMsg::SendMessage(androidJniMsg, THREAD_SHOW, ewol::threadMsg::MSG_PRIO_LOW);
}

void EWOL_ThreadEventHasJustDisplay(void)
{
	ewol::threadMsg::SendMessage(androidJniMsg, THREAD_JUST_DISPLAY, ewol::threadMsg::MSG_PRIO_REAL_TIME);
}

