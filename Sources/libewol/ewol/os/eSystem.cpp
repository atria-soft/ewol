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
#include <etk/MessageFifo.h>
#include <ewol/os/eSystem.h>
#include <ewol/os/gui.h>
#include <ewol/ResourceManager.h>
#include <ewol/eObject/EObject.h>
#include <ewol/eObject/EObjectManager.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/ShortCutManager.h>
#include <ewol/os/eSystemInput.h>
#include <ewol/openGL/openGL.h>
#include <ewol/os/Fps.h>
#include <ewol/font/FontManager.h>

static ewol::Windows*     windowsCurrent = NULL;
static Vector2D<int32_t>  windowsSize(320, 480);
static ewol::eSystemInput l_managementInput;

enum {
	THREAD_RECALCULATE_SIZE,
	THREAD_RESIZE,
	THREAD_HIDE,
	THREAD_SHOW,
	
	THREAD_INPUT_MOTION,
	THREAD_INPUT_STATE,
	
	THREAD_KEYBORAD_KEY,
	THREAD_KEYBORAD_MOVE,
	
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
	bool state;
	float x;
	float y;
} eventInput_ts;

typedef struct {
	int32_t TypeMessage;
	union {
		ewol::clipBoard::clipboardListe_te clipboardID;
		eventResize_ts resize;
		eventInput_ts input;
		eSystem::keyboardMove_ts keyboardMove;
		eSystem::keyboardKey_ts  keyboardKey;
	};
} eSystemMessage_ts;

// deblare the message system
static etk::MessageFifo<eSystemMessage_ts> l_msgSystem;


extern eSystem::specialKey_ts specialCurrentKey;

static bool requestEndProcessing = false;

void ewolProcessEvents(void)
{
	int32_t nbEvent = 0;
	//EWOL_DEBUG(" ********  Event");
	eSystemMessage_ts data;
	while (l_msgSystem.Count()>0) 
	{
		nbEvent++;
		l_msgSystem.Wait(data);
		//EWOL_DEBUG("EVENT");
		switch (data.TypeMessage) {
			case THREAD_RECALCULATE_SIZE:
				eSystem::ForceRedrawAll();
				break;
			case THREAD_RESIZE:
				//EWOL_DEBUG("Receive MSG : THREAD_RESIZE");
				windowsSize.x = data.resize.w;
				windowsSize.y = data.resize.h;
				eSystem::ForceRedrawAll();
				break;
			case THREAD_INPUT_MOTION:
				//EWOL_DEBUG("Receive MSG : THREAD_INPUT_MOTION");
				{
					Vector2D<float> pos;
					pos.x = data.input.x;
					pos.y = data.input.y;
					l_managementInput.Motion(data.input.type, data.input.pointerID, pos);
				}
				break;
			case THREAD_INPUT_STATE:
				//EWOL_DEBUG("Receive MSG : THREAD_INPUT_STATE");
				{
					Vector2D<float> pos;
					pos.x = data.input.x;
					pos.y = data.input.y;
					l_managementInput.State(data.input.type, data.input.pointerID, data.input.state, pos);
				}
				break;
			case THREAD_KEYBORAD_KEY:
				//EWOL_DEBUG("Receive MSG : THREAD_KEYBORAD_KEY");
				{
					specialCurrentKey = data.keyboardKey.special;
					if (false==ewol::shortCut::Process(data.keyboardKey.special.shift,
					                                   data.keyboardKey.special.ctrl,
					                                   data.keyboardKey.special.alt,
					                                   data.keyboardKey.special.meta,
					                                   data.keyboardKey.myChar,
					                                   data.keyboardKey.isDown)) {
						// Get the current Focused Widget :
						ewol::Widget * tmpWidget = ewol::widgetManager::FocusGet();
						if (NULL != tmpWidget) {
							if(true == data.keyboardKey.isDown) {
								EWOL_VERBOSE("GUI PRESSED : \"" << data.keyboardKey.myChar << "\"");
								tmpWidget->OnEventKb(ewol::EVENT_KB_TYPE_DOWN, data.keyboardKey.myChar);
							} else {
								EWOL_VERBOSE("GUI Release : \"" << data.keyboardKey.myChar << "\"");
								tmpWidget->OnEventKb(ewol::EVENT_KB_TYPE_UP, data.keyboardKey.myChar);
							}
						}
					}
				}
				break;
			case THREAD_KEYBORAD_MOVE:
				//EWOL_DEBUG("Receive MSG : THREAD_KEYBORAD_MOVE");
				{
					specialCurrentKey = data.keyboardMove.special;
					// Get the current Focused Widget :
					ewol::Widget * tmpWidget = ewol::widgetManager::FocusGet();
					if (NULL != tmpWidget) {
						if(true == data.keyboardMove.isDown) {
							tmpWidget->OnEventKbMove(ewol::EVENT_KB_TYPE_DOWN, data.keyboardMove.move);
						} else {
							tmpWidget->OnEventKbMove(ewol::EVENT_KB_TYPE_UP, data.keyboardMove.move);
						}
					}
				}
				break;
			case THREAD_CLIPBOARD_ARRIVE:
				{
					ewol::Widget * tmpWidget = ewol::widgetManager::FocusGet();
					if (tmpWidget != NULL) {
						tmpWidget->OnEventClipboard(data.clipboardID);
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
		l_msgSystem.Clean();
		requestEndProcessing = false;
		EWOL_INFO("v" EWOL_VERSION_TAG_NAME);
		EWOL_INFO("Build Date: " BUILD_TIME);
		etk::InitDefaultFolder("ewolApplNoName");
		ewol::openGL::Init();
		ewol::EObjectManager::Init();
		ewol::EObjectMessageMultiCast::Init();
		l_managementInput.Reset();
		ewol::resource::Init();
		ewol::widgetManager::Init();
		ewol::font::Init();
		ewol::shortCut::Init();
		APP_Init();
		isGlobalSystemInit = true;
		// force a recalculation
		ewol::RequestUpdateSize();
	}
	EWOL_INFO("==> Ewol System Init (END)");
}

void eSystem::UnInit(void)
{
	EWOL_INFO("==> Ewol System Un-Init (BEGIN)");
	if (true == isGlobalSystemInit) {
		isGlobalSystemInit = false;
		requestEndProcessing = true;
		// unset all windows
		ewol::DisplayWindows(NULL);
		// call application to uninit
		APP_UnInit();
		ewol::shortCut::UnInit();
		ewol::widgetManager::UnInit();
		ewol::font::UnInit();
		ewol::EObjectMessageMultiCast::UnInit();
		ewol::EObjectManager::UnInit();
		ewol::resource::UnInit();
		ewol::openGL::UnInit();
		l_managementInput.Reset();
		l_msgSystem.Clean();
	}
	EWOL_INFO("==> Ewol System Un-Init (END)");
}

void ewol::RequestUpdateSize(void)
{
	if (true == isGlobalSystemInit) {
		eSystemMessage_ts data;
		data.TypeMessage = THREAD_RECALCULATE_SIZE;
		l_msgSystem.Post(data);
	}
}

void eSystem::Resize(int w, int h )
{
	if (true == isGlobalSystemInit) {
		eSystemMessage_ts data;
		data.TypeMessage = THREAD_RESIZE;
		data.resize.w = w;
		data.resize.h = h;
		l_msgSystem.Post(data);
	}
}
void eSystem::Move(int w, int h )
{
	if (true == isGlobalSystemInit) {
		/*
		eSystemMessage_ts data;
		data.TypeMessage = THREAD_RESIZE;
		data.resize.w = w;
		data.resize.h = h;
		l_msgSystem.Post(data);
		*/
	}
}

void eSystem::SetInputMotion(int pointerID, float x, float y )
{
	if (true == isGlobalSystemInit) {
		eSystemMessage_ts data;
		data.TypeMessage = THREAD_INPUT_MOTION;
		data.input.type = ewol::INPUT_TYPE_FINGER;
		data.input.pointerID = pointerID;
		data.input.x = x;
		data.input.y = y;
		l_msgSystem.Post(data);
	}
}


void eSystem::SetInputState(int pointerID, bool isUp, float x, float y )
{
	if (true == isGlobalSystemInit) {
		eSystemMessage_ts data;
		data.TypeMessage = THREAD_INPUT_STATE;
		data.input.type = ewol::INPUT_TYPE_FINGER;
		data.input.pointerID = pointerID;
		data.input.state = isUp;
		data.input.x = x;
		data.input.y = y;
		l_msgSystem.Post(data);
	}
}


void eSystem::SetMouseMotion(int pointerID, float x, float y )
{
	if (true == isGlobalSystemInit) {
		eSystemMessage_ts data;
		data.TypeMessage = THREAD_INPUT_MOTION;
		data.input.type = ewol::INPUT_TYPE_MOUSE;
		data.input.pointerID = pointerID;
		data.input.x = x;
		data.input.y = y;
		l_msgSystem.Post(data);
	}
}


void eSystem::SetMouseState(int pointerID, bool isUp, float x, float y )
{
	if (true == isGlobalSystemInit) {
		eSystemMessage_ts data;
		data.TypeMessage = THREAD_INPUT_STATE;
		data.input.type = ewol::INPUT_TYPE_MOUSE;
		data.input.pointerID = pointerID;
		data.input.state = isUp;
		data.input.x = x;
		data.input.y = y;
		l_msgSystem.Post(data);
	}
}

void eSystem::SetKeyboard(eSystem::keyboardKey_ts& keyInput)
{
	if (true == isGlobalSystemInit) {
		eSystemMessage_ts data;
		data.TypeMessage = THREAD_KEYBORAD_KEY;
		data.keyboardKey = keyInput;
		l_msgSystem.Post(data);
	}
}

void eSystem::SetKeyboardMove(eSystem::keyboardMove_ts& keyInput)
{
	if (true == isGlobalSystemInit) {
		eSystemMessage_ts data;
		data.TypeMessage = THREAD_KEYBORAD_MOVE;
		data.keyboardMove = keyInput;
		l_msgSystem.Post(data);
	}
}


void eSystem::Hide(void)
{
	if (true == isGlobalSystemInit) {
		eSystemMessage_ts data;
		data.TypeMessage = THREAD_HIDE;
		l_msgSystem.Post(data);
	}
}

void eSystem::Show(void)
{
	if (true == isGlobalSystemInit) {
		eSystemMessage_ts data;
		data.TypeMessage = THREAD_SHOW;
		l_msgSystem.Post(data);
	}
}


void eSystem::ClipBoardArrive(ewol::clipBoard::clipboardListe_te clipboardID)
{
	if (true == isGlobalSystemInit) {
		eSystemMessage_ts data;
		data.TypeMessage = THREAD_CLIPBOARD_ARRIVE;
		data.clipboardID = clipboardID;
		l_msgSystem.Post(data);
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
				ewol::resource::UpdateContext();
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
	ewol::resource::ContextHasBeenDestroyed();
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

