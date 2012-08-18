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

enum {
	THREAD_INIT,
	THREAD_UN_INIT,
	THREAD_RECALCULATE_SIZE,
	THREAD_RESIZE,
	THREAD_HIDE,
	THREAD_SHOW,
	
	THREAD_INPUT_MOTION,
	THREAD_INPUT_STATE,
	
	THREAD_KEYBORAD_KEY,
	THREAD_KEYBORAD_MOVE,
	THREAD_JUST_DISPLAY,
	
	THREAD_CLIPBOARD_ARRIVE,
};


#include <unistd.h>

typedef struct {
	int w;
	int h;
} eventResize_ts;

typedef struct {
	ewol::inputType_te type;
	int pointerID;
	float x;
	float y;
} eventInputMotion_ts;

typedef struct {
	ewol::inputType_te type;
	int pointerID;
	bool state;
	float x;
	float y;
} eventInputState_ts;

void EWOL_NativeResize(int w, int h );
void EWOL_NativeRegenerateDisplay(void);

extern guiSystem::event::specialKey_ts specialCurrentKey;

static bool requestEndProcessing = false;

void ewolProcessEvents(void)
{
	int32_t nbEvent = 0;
	//EWOL_DEBUG(" ********  Event");
	ewol::threadMsg::threadMsgContent_ts data;
	data.type = THREAD_JUST_DISPLAY;
	while (ewol::threadMsg::WaitingMessage(androidJniMsg)>0) 
	{
		nbEvent++;
		ewol::threadMsg::WaitMessage(androidJniMsg, data);
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
				case THREAD_RECALCULATE_SIZE:
					UpdateGuiSize();
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
						Vector2D<float> pos;
						pos.x = tmpData->x;
						pos.y = tmpData->y;
						ewol::eventInput::Motion(tmpData->type, tmpData->pointerID, pos);
					}
					break;
				case THREAD_INPUT_STATE:
					//EWOL_DEBUG("Receive MSG : THREAD_INPUT_STATE");
					{
						eventInputState_ts * tmpData = (eventInputState_ts*)data.data;
						Vector2D<float> pos;
						pos.x = tmpData->x;
						pos.y = tmpData->y;
						ewol::eventInput::State(tmpData->type, tmpData->pointerID, tmpData->state, pos);
					}
					break;
				case THREAD_KEYBORAD_KEY:
					//EWOL_DEBUG("Receive MSG : THREAD_KEYBORAD_KEY");
					{
						guiSystem::event::keyboardKey_ts * tmpData = (guiSystem::event::keyboardKey_ts*)data.data;
						specialCurrentKey = tmpData->special;
						if (false==ewol::shortCut::Process(tmpData->special.shift, tmpData->special.ctrl, tmpData->special.alt, tmpData->special.meta, tmpData->myChar, tmpData->isDown)) {
							guiAbstraction::SendKeyboardEvent(tmpData->isDown, tmpData->myChar);
						}
					}
					break;
				case THREAD_KEYBORAD_MOVE:
					//EWOL_DEBUG("Receive MSG : THREAD_KEYBORAD_MOVE");
					{
						guiSystem::event::keyboardMove_ts * tmpData = (guiSystem::event::keyboardMove_ts*)data.data;
						specialCurrentKey = tmpData->special;
						guiAbstraction::SendKeyboardEventMove(tmpData->isDown, tmpData->move);
					}
					break;
				case THREAD_CLIPBOARD_ARRIVE:
					{
						ewol::clipBoard::clipboardListe_te * tmpdata = (ewol::clipBoard::clipboardListe_te*)data.data;
						guiAbstraction::SendClipboard(*tmpdata);
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
	}
}



void guiSystem::SetArchiveDir(int mode, const char* str)
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

void guiSystem::Init(void)
{
	EWOL_INFO("==> Ewol System Init (BEGIN)");
	if (false == isGlobalSystemInit) {
		// create message system ...
		EWOL_DEBUG("Init thread message system");
		ewol::threadMsg::Init(androidJniMsg);
		requestEndProcessing = false;
		EWOL_INFO("v" EWOL_VERSION_TAG_NAME);
		EWOL_INFO("Build Date: " BUILD_TIME);
		etk::InitDefaultFolder("ewolApplNoName");
		ewol::EObjectManager::Init();
		ewol::EObjectMessageMultiCast::Init();
		ewol::eventInput::Init();
		ewol::widgetManager::Init();
		ewol::texture::Init();
		ewol::InitFont();
		ewol::shortCut::Init();
		APP_Init();
		isGlobalSystemInit = true;
		EWOL_DEBUG("Send Init message to the thread");
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_INIT, ewol::threadMsg::MSG_PRIO_REAL_TIME);
		EWOL_DEBUG("end basic init");
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_RECALCULATE_SIZE, ewol::threadMsg::MSG_PRIO_MEDIUM);
	}
	EWOL_INFO("==> Ewol System Init (END)");
}

void guiSystem::UnInit(void)
{
	EWOL_INFO("==> Ewol System Un-Init (BEGIN)");
	if (true == isGlobalSystemInit) {
		isGlobalSystemInit = false;
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
		ewol::threadMsg::UnInit(androidJniMsg);
	}
	EWOL_INFO("==> Ewol System Un-Init (END)");
}

void ewol::RequestUpdateSize(void)
{
	if (true == isGlobalSystemInit) {
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_RECALCULATE_SIZE, ewol::threadMsg::MSG_PRIO_MEDIUM);
	}
}



void guiSystem::event::Resize(int w, int h )
{
	if (true == isGlobalSystemInit) {
		eventResize_ts tmpData;
		tmpData.w = w;
		tmpData.h = h;
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_RESIZE, ewol::threadMsg::MSG_PRIO_MEDIUM, &tmpData, sizeof(eventResize_ts) );
	}
}

void guiSystem::event::SetInputMotion(int pointerID, float x, float y )
{
	if (true == isGlobalSystemInit) {
		eventInputMotion_ts tmpData;
		tmpData.type = ewol::INPUT_TYPE_FINGER;
		tmpData.pointerID = pointerID;
		tmpData.x = x;
		tmpData.y = y;
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_INPUT_MOTION, ewol::threadMsg::MSG_PRIO_LOW, &tmpData, sizeof(eventInputMotion_ts) );
	}
}


void guiSystem::event::SetInputState(int pointerID, bool isUp, float x, float y )
{
	if (true == isGlobalSystemInit) {
		eventInputState_ts tmpData;
		tmpData.type = ewol::INPUT_TYPE_FINGER;
		tmpData.pointerID = pointerID;
		tmpData.state = isUp;
		tmpData.x = x;
		tmpData.y = y;
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_INPUT_STATE, ewol::threadMsg::MSG_PRIO_LOW, &tmpData, sizeof(eventInputState_ts) );
	}
}

void guiSystem::event::SetMouseMotion(int pointerID, float x, float y )
{
	if (true == isGlobalSystemInit) {
		eventInputMotion_ts tmpData;
		tmpData.type = ewol::INPUT_TYPE_MOUSE;
		tmpData.pointerID = pointerID;
		tmpData.x = x;
		tmpData.y = y;
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_INPUT_MOTION, ewol::threadMsg::MSG_PRIO_LOW, &tmpData, sizeof(eventInputMotion_ts) );
	}
}


void guiSystem::event::SetMouseState(int pointerID, bool isUp, float x, float y )
{
	if (true == isGlobalSystemInit) {
		eventInputState_ts tmpData;
		tmpData.type = ewol::INPUT_TYPE_MOUSE;
		tmpData.pointerID = pointerID;
		tmpData.state = isUp;
		tmpData.x = x;
		tmpData.y = y;
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_INPUT_STATE, ewol::threadMsg::MSG_PRIO_LOW, &tmpData, sizeof(eventInputState_ts) );
	}
}

void guiSystem::event::SetKeyboard(guiSystem::event::keyboardKey_ts& keyInput)
{
	if (true == isGlobalSystemInit) {
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_KEYBORAD_KEY, ewol::threadMsg::MSG_PRIO_LOW, &keyInput, sizeof(guiSystem::event::keyboardKey_ts) );
	}
}

void guiSystem::event::SetKeyboardMove(guiSystem::event::keyboardMove_ts& keyInput)
{
	if (true == isGlobalSystemInit) {
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_KEYBORAD_MOVE, ewol::threadMsg::MSG_PRIO_LOW, &keyInput, sizeof(guiSystem::event::keyboardMove_ts) );
	}
}

void guiSystem::event::Hide(void)
{
	if (true == isGlobalSystemInit) {
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_HIDE, ewol::threadMsg::MSG_PRIO_LOW);
	}
}

void guiSystem::event::Show(void)
{
	if (true == isGlobalSystemInit) {
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_SHOW, ewol::threadMsg::MSG_PRIO_LOW);
	}
}


void guiSystem::event::ClipBoardArrive(ewol::clipBoard::clipboardListe_te clipboardID)
{
	if (true == isGlobalSystemInit) {
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_CLIPBOARD_ARRIVE, ewol::threadMsg::MSG_PRIO_LOW, &clipboardID, sizeof(uint8_t));
	}
}


bool guiSystem::Draw(bool displayEveryTime)
{
	if (true == isGlobalSystemInit) {
		ewolProcessEvents();
		
		int64_t localTime = ewol::GetTime();
		ewol::widgetManager::PeriodicCall(localTime);
		
		EWOL_NativeRegenerateDisplay();
		
		ewol::texture::UpdateContext();
		EWOL_GenericDraw(true);
	}
	return true;
}

