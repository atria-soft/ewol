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


#include <etk/Types.h>
#include <etk/File.h>
#include <ewol/ewol.h>
#include <ewol/Debug.h>
#include <ewol/threadMsg.h>
#include <ewol/base/MainThread.h>
#include <ewol/base/gui.h>
#include <ewol/Texture.h>
#include <ewol/EObject.h>
#include <ewol/EObjectManager.h>
#include <ewol/WidgetManager.h>
#include <ewol/ShortCutManager.h>
#include <ewol/base/eventInputManagement.h>



static ewol::threadMsg::threadMsg_ts    androidJniMsg;
static pthread_t                        androidJniThread;
static pthread_attr_t                   androidJniThreadAttr;

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

void EWOL_NativeResize(int w, int h );
void EWOL_NativeRegenerateDisplay(void);

extern eventSpecialKey_ts specialCurrentKey;

static void* BaseAppEntry(void* param)
{
	bool requestEndProcessing = false;
	EWOL_DEBUG("==> Init BThread (START)");
	
	EWOL_INFO("v" EWOL_VERSION_TAG_NAME);
	EWOL_INFO("Build Date: " VERSION_BUILD_TIME);
	
	etk::InitDefaultFolder("ewolApplNoName");

	/*
		struct sched_param pr;
		int ret = 9;
		int policy;
		pthread_getschedparam(pthread_self(), &policy, &pr);
		EWOL_INFO("Child Thread Up PL" << policy << " PRI" << pr.sched_priority); //The result here
		policy = SCHED_RR;
		pr.sched_priority = 19;
		pthread_setschedparam(pthread_self(), policy, &pr);
		sleep(1);
		pthread_getschedparam(pthread_self(), &policy, &pr);
		EWOL_INFO("Child Thread Up PL" << policy << " PRI" << pr.sched_priority); //The result Set
	*/
	
	ewol::EObjectManager::Init();
	ewol::EObjectMessageMultiCast::Init();
	ewol::eventInput::Init();
	ewol::widgetManager::Init();
	ewol::texture::Init();
	ewol::InitFont();
	ewol::shortCut::Init();
	APP_Init();
	int32_t countNbEvent = 0;
	EWOL_DEBUG("==> Init BThread (END)");
	while(false == requestEndProcessing) {
		ewol::threadMsg::threadMsgContent_ts data;
		data.type = THREAD_JUST_DISPLAY;
		ewol::threadMsg::WaitMessage(androidJniMsg, data);
		countNbEvent++;
		if (data.type != THREAD_JUST_DISPLAY) {
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
						coord2D_ts pos;
						pos.x = tmpData->x;
						pos.y = tmpData->y;
						ewol::eventInput::Motion(tmpData->pointerID, pos);
					}
					break;
				case THREAD_INPUT_STATE:
					//EWOL_DEBUG("Receive MSG : THREAD_INPUT_STATE");
					{
						eventInputState_ts * tmpData = (eventInputState_ts*)data.data;
						coord2D_ts pos;
						pos.x = tmpData->x;
						pos.y = tmpData->y;
						ewol::eventInput::State(tmpData->pointerID, tmpData->state, pos);
					}
					break;
				case THREAD_KEYBORAD_KEY:
					//EWOL_DEBUG("Receive MSG : THREAD_KEYBORAD_KEY");
					{
						eventKeyboardKey_ts * tmpData = (eventKeyboardKey_ts*)data.data;
						specialCurrentKey = tmpData->special;
						if (false==ewol::shortCut::Process(tmpData->special.shift, tmpData->special.ctrl, tmpData->special.alt, tmpData->special.meta, tmpData->myChar, tmpData->isDown)) {
							guiAbstraction::SendKeyboardEvent(tmpData->isDown, tmpData->myChar);
						}
					}
					break;
				case THREAD_KEYBORAD_MOVE:
					//EWOL_DEBUG("Receive MSG : THREAD_KEYBORAD_MOVE");
					{
						eventKeyboardMove_ts * tmpData = (eventKeyboardMove_ts*)data.data;
						specialCurrentKey = tmpData->special;
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
				if(true == ewol::threadMsg::HasDisplayDone(androidJniMsg)) {
					int64_t localTime = GetCurrentTime();
					ewol::widgetManager::PeriodicCall(localTime);
				}
				EWOL_NativeRegenerateDisplay();
				countNbEvent = 0;
			}
		}
	}
	EWOL_DEBUG("==> Un-Init BThread (START)");
	
	// unset all windows
	ewol::DisplayWindows(NULL);
	// call application to uninit
	APP_UnInit();
	
	ewol::shortCut::UnInit();
	ewol::texture::UnInit();
	ewol::UnInitFont();
	ewol::widgetManager::UnInit();
	ewol::EObjectMessageMultiCast::UnInit();
	ewol::EObjectManager::UnInit();
	ewol::eventInput::UnInit();
	EWOL_DEBUG("==> Un-Init BThread (END)");
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
		EWOL_DEBUG("Init thread message system");
		ewol::threadMsg::Init(androidJniMsg);
		// init the thread :
		EWOL_DEBUG("Create the thread");
		pthread_attr_init(&androidJniThreadAttr);
		pthread_attr_setdetachstate(&androidJniThreadAttr, PTHREAD_CREATE_JOINABLE);
		//pthread_attr_setdetachstate(&androidJniThreadAttr, PTHREAD_CREATE_DETACHED);
		//pthread_attr_setscope(      &androidJniThreadAttr, PTHREAD_SCOPE_SYSTEM);
		/* // note android does not permit to change the thread priority ...
		// try to set prio : 
		struct sched_param pr;
		pr.sched_priority = 10;
		pthread_attr_setschedpolicy(&androidJniThreadAttr, SCHED_RR);
		pthread_attr_setschedparam(&androidJniThreadAttr, &pr);
		*/
		pthread_setname_np(androidJniThread, "ewol_basic_thread");
		pthread_create(&androidJniThread, &androidJniThreadAttr, BaseAppEntry, NULL);
		
		//pthread_create(&androidJniThread, NULL,                  BaseAppEntry, NULL);
		isGlobalSystemInit = true;
		EWOL_DEBUG("Send Init message to the thread");
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_INIT, ewol::threadMsg::MSG_PRIO_REAL_TIME);
		EWOL_DEBUG("end basic init");
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

void EWOL_ThreadKeyboardEvent(eventKeyboardKey_ts& keyInput)
{
	ewol::threadMsg::SendMessage(androidJniMsg, THREAD_KEYBORAD_KEY, ewol::threadMsg::MSG_PRIO_LOW, &keyInput, sizeof(eventKeyboardKey_ts) );
}

void EWOL_ThreadKeyboardEventMove(eventKeyboardMove_ts& keyInput)
{
	ewol::threadMsg::SendMessage(androidJniMsg, THREAD_KEYBORAD_MOVE, ewol::threadMsg::MSG_PRIO_LOW, &keyInput, sizeof(eventKeyboardMove_ts) );
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
	ewol::threadMsg::SendDisplayDone(androidJniMsg);
	//ewol::threadMsg::SendMessage(androidJniMsg, THREAD_JUST_DISPLAY, ewol::threadMsg::MSG_PRIO_REAL_TIME);
}

