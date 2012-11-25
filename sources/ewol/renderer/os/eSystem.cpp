/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <etk/types.h>
#include <etk/MessageFifo.h>

#include <ewol/ewol.h>
#include <ewol/debug.h>

#include <ewol/config.h>

#include <ewol/eObject/EObject.h>
#include <ewol/eObject/EObjectManager.h>

#include <ewol/renderer/os/eSystem.h>
#include <ewol/renderer/os/gui.h>
#include <ewol/renderer/ResourceManager.h>
#include <ewol/renderer/os/eSystemInput.h>
#include <ewol/renderer/openGL.h>
#include <ewol/renderer/os/Fps.h>

#include <ewol/widget/WidgetManager.h>


static ewol::Windows*          windowsCurrent = NULL;
static etk::Vector2D<int32_t>  windowsSize(320, 480);
static ewol::eSystemInput      l_managementInput;



void eSystem::InputEventTransfertWidget(ewol::Widget* source, ewol::Widget* destination)
{
	l_managementInput.TransfertEvent(source, destination);
}


enum {
	THREAD_INIT,
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
	ewol::keyEvent::type_te type;
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
			case THREAD_INIT:
				// this is due to the openGL context
				APP_Init();
				break;
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
					etk::Vector2D<float> pos;
					pos.x = data.input.x;
					pos.y = data.input.y;
					l_managementInput.Motion(data.input.type, data.input.pointerID, pos);
				}
				break;
			case THREAD_INPUT_STATE:
				//EWOL_DEBUG("Receive MSG : THREAD_INPUT_STATE");
				{
					etk::Vector2D<float> pos;
					pos.x = data.input.x;
					pos.y = data.input.y;
					l_managementInput.State(data.input.type, data.input.pointerID, data.input.state, pos);
				}
				break;
			case THREAD_KEYBORAD_KEY:
				//EWOL_DEBUG("Receive MSG : THREAD_KEYBORAD_KEY");
				{
					ewol::SpecialKey& specialCurrentKey = ewol::GetCurrentSpecialKeyStatus();
					specialCurrentKey = data.keyboardKey.special;
					if (NULL != windowsCurrent) {
						if (false==windowsCurrent->OnEventShortCut(data.keyboardKey.special,
						                                           data.keyboardKey.myChar,
						                                           ewol::keyEvent::keyboardUnknow,
						                                           data.keyboardKey.isDown)) {
							// Get the current Focused Widget :
							ewol::Widget * tmpWidget = ewol::widgetManager::FocusGet();
							if (NULL != tmpWidget) {
								// check Widget shortcut
								if (false==tmpWidget->OnEventShortCut(data.keyboardKey.special,
								                                      data.keyboardKey.myChar,
								                                      ewol::keyEvent::keyboardUnknow,
								                                      data.keyboardKey.isDown)) {
									// generate the direct event ...
									if(true == data.keyboardKey.isDown) {
										EWOL_VERBOSE("GUI PRESSED : \"" << data.keyboardKey.myChar << "\"");
										tmpWidget->OnEventKb(ewol::keyEvent::statusDown, data.keyboardKey.myChar);
									} else {
										EWOL_VERBOSE("GUI Release : \"" << data.keyboardKey.myChar << "\"");
										tmpWidget->OnEventKb(ewol::keyEvent::statusUp, data.keyboardKey.myChar);
									}
								}
							}
						}
					}
				}
				break;
			case THREAD_KEYBORAD_MOVE:
				//EWOL_DEBUG("Receive MSG : THREAD_KEYBORAD_MOVE");
				// check main shortcut
				if (NULL != windowsCurrent) {
					if (false==windowsCurrent->OnEventShortCut(data.keyboardKey.special,
					                                           data.keyboardKey.myChar,
					                                           ewol::keyEvent::keyboardUnknow,
					                                           data.keyboardKey.isDown)) {
						ewol::SpecialKey& specialCurrentKey = ewol::GetCurrentSpecialKeyStatus();
						specialCurrentKey = data.keyboardMove.special;
						// Get the current Focused Widget :
						ewol::Widget * tmpWidget = ewol::widgetManager::FocusGet();
						if (NULL != tmpWidget) {
							// check Widget shortcut
							if (false==tmpWidget->OnEventShortCut(data.keyboardKey.special,
							                                      data.keyboardKey.myChar,
							                                      ewol::keyEvent::keyboardUnknow,
							                                      data.keyboardKey.isDown)) {
								// generate the direct event ...
								if(true == data.keyboardMove.isDown) {
									tmpWidget->OnEventKbMove(ewol::keyEvent::statusDown, data.keyboardMove.move);
								} else {
									tmpWidget->OnEventKbMove(ewol::keyEvent::statusUp, data.keyboardMove.move);
								}
							}
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

void RequestInit(void)
{
	if (true == isGlobalSystemInit) {
		eSystemMessage_ts data;
		data.TypeMessage = THREAD_INIT;
		l_msgSystem.Post(data);
	}
}


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
		ewol::config::Init();
		isGlobalSystemInit = true;
		// request the init of the application in the main context of openGL ...
		RequestInit();
		// force a recalculation
		ewol::RequestUpdateSize();
		#if defined(__EWOL_ANDROID_ORIENTATION_LANDSCAPE__)
			ewol::ForceOrientation(ewol::SCREEN_ORIENTATION_LANDSCAPE);
		#elif defined(__EWOL_ANDROID_ORIENTATION_PORTRAIT__)
			ewol::ForceOrientation(ewol::SCREEN_ORIENTATION_PORTRAIT);
		#else
			ewol::ForceOrientation(ewol::SCREEN_ORIENTATION_AUTO);
		#endif
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
		ewol::WindowsSet(NULL);
		// call application to uninit
		APP_UnInit();
		ewol::widgetManager::UnInit();
		ewol::config::UnInit();
		ewol::EObjectMessageMultiCast::UnInit();
		ewol::EObjectManager::UnInit();
		ewol::resource::UnInit();
		ewol::openGL::UnInit();
		l_managementInput.Reset();
		l_msgSystem.Clean();
	}
	EWOL_INFO("==> Ewol System Un-Init (END)");
}


void eSystem::RequestUpdateSize(void)
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
		data.input.type = ewol::keyEvent::typeFinger;
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
		data.input.type = ewol::keyEvent::typeFinger;
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
		data.input.type = ewol::keyEvent::typeMouse;
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
		data.input.type = ewol::keyEvent::typeMouse;
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


static ewol::Fps l_FpsSystemEvent(     "Event     ", false);
static ewol::Fps l_FpsSystemContext(   "Context   ", false);
static ewol::Fps l_FpsSystem(          "Draw      ", true);

bool eSystem::Draw(bool displayEveryTime)
{
	int64_t currentTime = ewol::GetTime();
	if (true == isGlobalSystemInit) {
		// process the events
		l_FpsSystemEvent.Tic();
		ewolProcessEvents();
		// call all the widget that neded to do something periodicly
		ewol::widgetManager::PeriodicCall(currentTime);
		// Remove all widget that they are no more usefull (these who decided to destroy themself)
		ewol::EObjectManager::RemoveAllAutoRemove();
		ewol::Windows* tmpWindows = eSystem::GetCurrentWindows();
		// check if the user selected a windows
		if (NULL != tmpWindows) {
			// Redraw all needed elements
			tmpWindows->OnRegenerateDisplay();
		}
		l_FpsSystemEvent.IncrementCounter();
		l_FpsSystemEvent.Toc();
		bool needRedraw = ewol::widgetManager::IsDrawingNeeded();
		
		l_FpsSystemContext.Tic();
		if (NULL != tmpWindows) {
			if(    true == needRedraw
			    || true == displayEveryTime) {
				ewol::resource::UpdateContext();
				l_FpsSystemContext.IncrementCounter();
			}
		}
		l_FpsSystemContext.Toc();
		
		l_FpsSystem.Tic();
		if (NULL != tmpWindows) {
			if(    true == needRedraw
			    || true == displayEveryTime) {
				l_FpsSystem.IncrementCounter();
				tmpWindows->SysDraw();
			}
		}
		l_FpsSystem.Toc();
		glFlush();
	}
	return true;
}


void eSystem::OnObjectRemove(ewol::EObject * removeObject)
{
	l_managementInput.OnObjectRemove(removeObject);
}


void eSystem::ResetIOEvent(void)
{
	l_managementInput.NewLayerSet();
}


void eSystem::OpenGlContextDestroy(void)
{
	ewol::resource::ContextHasBeenDestroyed();
}


void eSystem::SetCurrentWindows(ewol::Windows * windows)
{
	// set the new pointer as windows system
	windowsCurrent = windows;
	// request all the widget redrawing
	eSystem::ForceRedrawAll();
}


ewol::Windows* eSystem::GetCurrentWindows(void)
{
	return windowsCurrent;
}


etk::Vector2D<int32_t> eSystem::GetSize(void)
{
	return windowsSize;
}


void eSystem::ForceRedrawAll(void)
{
	ewol::Windows* tmpWindows = eSystem::GetCurrentWindows();
	if (NULL != tmpWindows) {
		etk::Vector2D<int32_t> tmpSize = eSystem::GetSize();
		tmpWindows->CalculateSize(tmpSize.x, tmpSize.y);
	}
}


void eSystem::OnKill(void)
{
	ewol::Windows* tmpWindows = eSystem::GetCurrentWindows();
	if (NULL != tmpWindows) {
		tmpWindows->SysOnKill();
	}
}

