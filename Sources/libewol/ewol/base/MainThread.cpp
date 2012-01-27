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



static ewol::threadMsg::threadMsg_ts    androidJniMsg;
static pthread_t                        androidJniThread;
static pthread_attr_t                   androidJniThreadAttr;

enum {
	JNI_NONE,
	JNI_INIT,
	JNI_UN_INIT,
	JNI_DONE,
	JNI_RESIZE,
	JNI_INPUT_MOTION,
	JNI_INPUT_STATE,
	JNI_DATA_ARCHIVE_DIR,
	JNI_APP_INIT,
	JNI_APP_UN_INIT,
	JNI_APP_RENDERER,
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



static void* BaseAppEntry(void* param)
{
	bool requestEndProcessing = false;
	EDN_DEBUG("start Ewol Basic thread ...");
	guiAbstraction::Init(0, NULL);
	ewol::Init(0, NULL);
	APP_Init(0, NULL);
	while(false == requestEndProcessing) {
		ewol::threadMsg::threadMsgContent_ts data;
		ewol::threadMsg::WaitMessage(androidJniMsg, data);
		switch (data.type) {
			case JNI_NONE:
				EDN_DEBUG("Receive MSG : JNI_NONE");
				break;
			case JNI_INIT:
				EDN_DEBUG("Receive MSG : JNI_INIT");
				EWOL_NativeApplicationInit();
				break;
			case JNI_UN_INIT:
				EDN_DEBUG("Receive MSG : JNI_UN_INIT");
				EWOL_NativeApplicationUnInit();
				requestEndProcessing = true;
				break;
			case JNI_DONE:
				EDN_DEBUG("Receive MSG : JNI_DONE");
				break;
			case JNI_RESIZE:
				EDN_DEBUG("Receive MSG : JNI_RESIZE");
				{
					eventResize_ts * tmpData = (eventResize_ts*)data.data;
					EWOL_NativeResize(tmpData->w, tmpData->h);
					EWOL_NativeInit();
				}
				break;
			case JNI_INPUT_MOTION:
				EDN_DEBUG("Receive MSG : JNI_INPUT_MOTION");
				{
					eventInputMotion_ts * tmpData = (eventInputMotion_ts*)data.data;
					EWOL_NativeEventInputMotion(tmpData->pointerID, tmpData->x, tmpData->y);
				}
				break;
			case JNI_INPUT_STATE:
				EDN_DEBUG("Receive MSG : JNI_INPUT_STATE");
				{
					eventInputState_ts * tmpData = (eventInputState_ts*)data.data;
					EWOL_NativeEventInputState(tmpData->pointerID, tmpData->state, tmpData->x, tmpData->y);
				}
				break;
			case JNI_DATA_ARCHIVE_DIR:
				EDN_DEBUG("Receive MSG : JNI_DATA_ARCHIVE_DIR");
				break;
			case JNI_APP_INIT:
				EDN_DEBUG("Receive MSG : JNI_APP_INIT");
				break;
			case JNI_APP_UN_INIT:
				EDN_DEBUG("Receive MSG : JNI_APP_UN_INIT");
				break;
			case JNI_APP_RENDERER:
				EDN_DEBUG("Receive MSG : JNI_APP_RENDERER");
				break;
			default:
				EDN_DEBUG("Receive MSG : UNKNOW");
				break;
		}
	}
	EDN_DEBUG("End Ewol Basic thread ...");
	
	// unset all windows
	ewol::DisplayWindows(NULL);
	// call application to uninit
	APP_UnInit();
	// basic abstraction un-init
	guiAbstraction::UnInit();
	// uninit Ewol
	ewol::UnInit();
	
	pthread_exit(NULL);
}


void EWOL_ThreadSetArchiveDir(int mode, const char* str)
{
	switch(mode)
	{
		case 0:
			EWOL_DEBUG("Directory APK : path=" << str);
			//if (firstInitDone == false)
			{
				etk::SetBaseFolderData(str);
			}
			break;
		case 1:
			EWOL_DEBUG("Directory mode=FILE path=" << str);
			//if (firstInitDone == false)
			{
				etk::SetBaseFolderDataUser(str);
			}
			break;
		case 2:
			EWOL_DEBUG("Directory mode=CACHE path=" << str);
			//if (firstInitDone == false)
			{
				etk::SetBaseFolderCache(str);
			}
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
		ewol::threadMsg::SendMessage(androidJniMsg, JNI_INIT, ewol::threadMsg::MSG_PRIO_REAL_TIME);
	}
}

void EWOL_SystemStop(void)
{
	if (true == isGlobalSystemInit) {
		isGlobalSystemInit = false;
		ewol::threadMsg::SendMessage(androidJniMsg, JNI_UN_INIT, ewol::threadMsg::MSG_PRIO_REAL_TIME);
		
		EDN_DEBUG("Wait end of the thread ...");
		// Wait end of the thread
		pthread_join(androidJniThread, NULL);
		ewol::threadMsg::UnInit(androidJniMsg);
	}
}



void EWOL_ThreadResize(int w, int h )
{
	
}


void EWOL_ThreadEventInputMotion(int pointerID, float x, float y )
{
	
}


void EWOL_ThreadEventInputState(int pointerID, bool isUp, float x, float y )
{
	
}

