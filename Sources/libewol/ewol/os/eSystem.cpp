/**
 *******************************************************************************
 * @file os/eSystem.cpp
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
#include <ewol/os/eSystem.h>
#include <ewol/os/gui.h>
#include <ewol/texture/Texture.h>
#include <ewol/eObject/EObject.h>
#include <ewol/eObject/EObjectManager.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/ShortCutManager.h>
#include <ewol/os/eSystemInput.h>
#include <ewol/openGl.h>
#include <ewol/os/Fps.h>

static ewol::Windows*         windowsCurrent = NULL;
static Vector2D<int32_t> windowsSize(320, 480);

static ewol::threadMsg::threadMsg_ts    androidJniMsg;

static ewol::eSystemInput l_managementInput;

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

extern eSystem::specialKey_ts specialCurrentKey;

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
					eSystem::ForceRedrawAll();
					break;
				case THREAD_RESIZE:
					//EWOL_DEBUG("Receive MSG : THREAD_RESIZE");
					{
						eventResize_ts * tmpData = (eventResize_ts*)data.data;
						windowsSize.x = tmpData->w;
						windowsSize.y = tmpData->h;
						eSystem::ForceRedrawAll();
					}
					break;
				case THREAD_INPUT_MOTION:
					//EWOL_DEBUG("Receive MSG : THREAD_INPUT_MOTION");
					{
						eventInputMotion_ts * tmpData = (eventInputMotion_ts*)data.data;
						Vector2D<float> pos;
						pos.x = tmpData->x;
						pos.y = tmpData->y;
						l_managementInput.Motion(tmpData->type, tmpData->pointerID, pos);
					}
					break;
				case THREAD_INPUT_STATE:
					//EWOL_DEBUG("Receive MSG : THREAD_INPUT_STATE");
					{
						eventInputState_ts * tmpData = (eventInputState_ts*)data.data;
						Vector2D<float> pos;
						pos.x = tmpData->x;
						pos.y = tmpData->y;
						l_managementInput.State(tmpData->type, tmpData->pointerID, tmpData->state, pos);
					}
					break;
				case THREAD_KEYBORAD_KEY:
					//EWOL_DEBUG("Receive MSG : THREAD_KEYBORAD_KEY");
					{
						eSystem::keyboardKey_ts * tmpData = (eSystem::keyboardKey_ts*)data.data;
						specialCurrentKey = tmpData->special;
						if (false==ewol::shortCut::Process(tmpData->special.shift, tmpData->special.ctrl, tmpData->special.alt, tmpData->special.meta, tmpData->myChar, tmpData->isDown)) {
							// Get the current Focused Widget :
							ewol::Widget * tmpWidget = ewol::widgetManager::FocusGet();
							if (NULL != tmpWidget) {
								if(true == tmpData->isDown) {
									EWOL_VERBOSE("GUI PRESSED : \"" << tmpData->myChar << "\"");
									tmpWidget->OnEventKb(ewol::EVENT_KB_TYPE_DOWN, tmpData->myChar);
								} else {
									EWOL_VERBOSE("GUI Release : \"" << tmpData->myChar << "\"");
									tmpWidget->OnEventKb(ewol::EVENT_KB_TYPE_UP, tmpData->myChar);
								}
							}
						}
					}
					break;
				case THREAD_KEYBORAD_MOVE:
					//EWOL_DEBUG("Receive MSG : THREAD_KEYBORAD_MOVE");
					{
						eSystem::keyboardMove_ts * tmpData = (eSystem::keyboardMove_ts*)data.data;
						specialCurrentKey = tmpData->special;
						// Get the current Focused Widget :
						ewol::Widget * tmpWidget = ewol::widgetManager::FocusGet();
						if (NULL != tmpWidget) {
							if(true == tmpData->isDown) {
								tmpWidget->OnEventKbMove(ewol::EVENT_KB_TYPE_DOWN, tmpData->move);
							} else {
								tmpWidget->OnEventKbMove(ewol::EVENT_KB_TYPE_UP, tmpData->move);
							}
						}
					}
					break;
				case THREAD_CLIPBOARD_ARRIVE:
					{
						ewol::clipBoard::clipboardListe_te * tmpdata = (ewol::clipBoard::clipboardListe_te*)data.data;
						ewol::Widget * tmpWidget = ewol::widgetManager::FocusGet();
						if (tmpWidget != NULL) {
							tmpWidget->OnEventClipboard(*tmpdata);
						}
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



void eSystem::SetArchiveDir(int mode, const char* str)
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

void eSystem::Init(void)
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
		l_managementInput.Reset();
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

void eSystem::UnInit(void)
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
		l_managementInput.Reset();
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



void eSystem::Resize(int w, int h )
{
	if (true == isGlobalSystemInit) {
		eventResize_ts tmpData;
		tmpData.w = w;
		tmpData.h = h;
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_RESIZE, ewol::threadMsg::MSG_PRIO_MEDIUM, &tmpData, sizeof(eventResize_ts) );
	}
}
void eSystem::Move(int w, int h )
{
	if (true == isGlobalSystemInit) {
		/*
		eventResize_ts tmpData;
		tmpData.w = w;
		tmpData.h = h;
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_RESIZE, ewol::threadMsg::MSG_PRIO_MEDIUM, &tmpData, sizeof(eventResize_ts) );
		*/
	}
}

void eSystem::SetInputMotion(int pointerID, float x, float y )
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


void eSystem::SetInputState(int pointerID, bool isUp, float x, float y )
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

void eSystem::SetMouseMotion(int pointerID, float x, float y )
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


void eSystem::SetMouseState(int pointerID, bool isUp, float x, float y )
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

void eSystem::SetKeyboard(eSystem::keyboardKey_ts& keyInput)
{
	if (true == isGlobalSystemInit) {
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_KEYBORAD_KEY, ewol::threadMsg::MSG_PRIO_LOW, &keyInput, sizeof(eSystem::keyboardKey_ts) );
	}
}

void eSystem::SetKeyboardMove(eSystem::keyboardMove_ts& keyInput)
{
	if (true == isGlobalSystemInit) {
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_KEYBORAD_MOVE, ewol::threadMsg::MSG_PRIO_LOW, &keyInput, sizeof(eSystem::keyboardMove_ts) );
	}
}

void eSystem::Hide(void)
{
	if (true == isGlobalSystemInit) {
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_HIDE, ewol::threadMsg::MSG_PRIO_LOW);
	}
}

void eSystem::Show(void)
{
	if (true == isGlobalSystemInit) {
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_SHOW, ewol::threadMsg::MSG_PRIO_LOW);
	}
}


void eSystem::ClipBoardArrive(ewol::clipBoard::clipboardListe_te clipboardID)
{
	if (true == isGlobalSystemInit) {
		ewol::threadMsg::SendMessage(androidJniMsg, THREAD_CLIPBOARD_ARRIVE, ewol::threadMsg::MSG_PRIO_LOW, &clipboardID, sizeof(uint8_t));
	}
}




static float gTriangleVertices[] = { 0.0f, 0.0f, 200.0f, 0.0f, 0.0f, 200.0f };
static float gTriangleVertices5[] = { 200.0f, 200.0f, 100.0f, 200.0f, 200.0f, 100.0f,
                                           200.0f, 200.0f, 300.0f, 200.0f, 200.0f, 300.0f };

static void BasicOpenGlDraw(void)
{
	Vector2D<int32_t> tmpSize = eSystem::GetSize();
	// set the size of the open GL system
	glViewport(0, 0, tmpSize.x, tmpSize.y);
	
	// Clear the screen with transparency ...
	glClearColor(0.0,0.0,0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(0., width, 0., -height, 1., 20.);
	glOrthoEwol(-tmpSize.x/2, tmpSize.x/2, tmpSize.y/2, -tmpSize.y/2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//glTranslatef(0, -height/2, -5);
	glTranslatef(-tmpSize.x/2, -tmpSize.y/2, -1.0);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnableClientState( GL_VERTEX_ARRAY );
	
	//LOGI("engine_draw_frame (%d,%d)",width,height);
	
	glColor4f(0.0, 1.0, 1.0, 1.0);
	glVertexPointer(2, GL_FLOAT, 0, gTriangleVertices5 );
	glDrawArrays( GL_TRIANGLES, 0, 6);
	static int vallllll = 0;
	static float transparency = 0.0;
	if (vallllll <= 1) {
		transparency +=0.025;
		if (transparency >= 1.0) {
			vallllll++;
			transparency = 0.0;
			glColor4f(1.0, 0.0, 0.0, 1.0);
		} else {
			glColor4f(1.0, 0.0, 0.0, transparency);
		}
	} else if (vallllll <= 2) {
		transparency +=0.025;
		if (transparency >= 1.0) {
			vallllll++;
			transparency = 0.0;
			glColor4f(1.0, 1.0, 0.0, 1.0);
		} else {
			glColor4f(1.0, 1.0, 0.0, transparency);
		}
	} else if (vallllll <= 3) {
		transparency +=0.025;
		if (transparency >= 1.0) {
			vallllll++;
			transparency = 0.0;
			glColor4f(0.0, 1.0, 0.0, 1.0);
		} else {
			glColor4f(0.0, 1.0, 0.0, transparency);
		}
	} else if (vallllll <= 4) {
		transparency +=0.025;
		if (transparency >= 1.0) {
			vallllll++;
			transparency = 0.0;
			glColor4f(0.0, 1.0, 1.0, 1.0);
		} else {
			glColor4f(0.0, 1.0, 1.0, transparency);
		}
	} else if (vallllll <= 5) {
		transparency +=0.025;
		if (transparency >= 1.0) {
			vallllll++;
			transparency = 0.0;
			glColor4f(0.0, 0.0, 1.0, 1.0);
		} else {
			glColor4f(0.0, 0.0, 1.0, transparency);
		}
	} else {
		transparency +=0.025;
		if (transparency >= 1.0) {
			vallllll = 0;
			transparency = 0.0;
			glColor4f(1.0, 0.0, 1.0, 1.0);
		} else {
			glColor4f(1.0, 0.0, 1.0, transparency);
		}
	}
	glVertexPointer(2, GL_FLOAT, 0, gTriangleVertices );
	glDrawArrays( GL_TRIANGLES, 0, 3);
	
	glDisableClientState( GL_VERTEX_ARRAY );

	glDisable(GL_BLEND);
}

static ewol::Fps l_FpsSystem;

bool eSystem::Draw(bool displayEveryTime)
{
	int64_t currentTime = ewol::GetTime();
	// FPS display system
	l_FpsSystem.Tic();
	if (true == isGlobalSystemInit) {
		// process the events
		ewolProcessEvents();
		// call all the widget that neded to do something periodicly
		ewol::widgetManager::PeriodicCall(currentTime);
		
		// Remove all widget that they are no more usefull (these who decided to destroy themself)
		ewol::EObjectManager::RemoveAllAutoRemove();
		ewol::Windows* tmpWindows = eSystem::GetCurrentWindows();
		// check if the user selected a windows
		if (NULL == tmpWindows) {
			BasicOpenGlDraw();
		} else {
			// Redraw all needed elements
			tmpWindows->OnRegenerateDisplay();
			// check if the regenerate is needed ...
			if(    true == ewol::widgetManager::IsDrawingNeeded()
			    || true == displayEveryTime) {
				ewol::texture::UpdateContext();
				l_FpsSystem.IncrementCounter();
				tmpWindows->SysDraw();
			}
		}
		glFlush();
	}
	// FPS display system
	l_FpsSystem.Toc();
	return true;
}

/**
 * @brief Inform object that an other object is removed ...
 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
 * @note : Sub classes must call this class
 * @return ---
 */
void eSystem::OnObjectRemove(ewol::EObject * removeObject)
{
	l_managementInput.OnObjectRemove(removeObject);
}

/**
 * @brief reset event management for the IO like Input ou Mouse or keyborad
 * @param ---
 * @return ---
 */
void eSystem::ResetIOEvent(void)
{
	l_managementInput.NewLayerSet();
}


/**
 * @brief Inform the system that the OpenGL constext has been destroy ==> use to automaticly reload the texture and other thinks ...
 * @param ---
 * @return ---
 */
void eSystem::OpenGlContextDestroy(void)
{
	ewol::texture::UpdateContextIsDestroy();
}


/**
 * @brief set the current windows to display :
 * @param windows windows that might be displayed
 * @return ---
 */
void eSystem::SetCurrentWindows(ewol::Windows * windows)
{
	// set the new pointer as windows system
	windowsCurrent = windows;
	// request all the widget redrawing
	eSystem::ForceRedrawAll();
}


/**
 * @brief Get the current windows that is displayed
 * @param ---
 * @return the current handle on the windows (can be null)
 */
ewol::Windows* eSystem::GetCurrentWindows(void)
{
	return windowsCurrent;
}


/**
 * @brief Get the current windows size
 * @param ---
 * @return the current size ...
 */
Vector2D<int32_t> eSystem::GetSize(void)
{
	return windowsSize;
}


/**
 * @brief Redraw all the windows
 * @param ---
 * @return ---
 */
void eSystem::ForceRedrawAll(void)
{
	ewol::Windows* tmpWindows = eSystem::GetCurrentWindows();
	if (NULL != tmpWindows) {
		Vector2D<int32_t> tmpSize = eSystem::GetSize();
		tmpWindows->CalculateSize(tmpSize.x, tmpSize.y);
	}
}

/**
 * @brief Inform the system that the Application has been killed
 * @param ---
 * @return ---
 */
void eSystem::OnKill(void)
{
	ewol::Windows* tmpWindows = eSystem::GetCurrentWindows();
	if (NULL != tmpWindows) {
		tmpWindows->SysOnKill();
	}
}

