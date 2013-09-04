/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <unistd.h>

#include <etk/types.h>

#include <ewol/ewol.h>
#include <ewol/Dimension.h>
#include <ewol/debug.h>

#include <ewol/renderer/EObject.h>
#include <ewol/renderer/EObjectManager.h>

#include <ewol/renderer/eContext.h>
#include <ewol/resources/ResourceManager.h>
#include <ewol/renderer/openGL.h>

#include <ewol/widget/WidgetManager.h>

#include <etk/os/FSNode.h>
#include <etk/os/Mutex.h>

#include <date/date.h>

/**
 * @brief Get the main ewol mutex (event or periodic call mutex).
 * @note due ti the fact that the system can be called for multiple instance, for naw we just limit the acces to one process at a time.
 * @return the main inteface Mutex
 */
static etk::Mutex& MutexInterface(void)
{
	static etk::Mutex s_interfaceMutex;
	return s_interfaceMutex;
}


static ewol::eContext* l_curentInterface=NULL;
ewol::eContext& ewol::GetContext(void)
{
	#if DEBUG_LEVEL > 2
		if(NULL == l_curentInterface){
			EWOL_CRITICAL("[CRITICAL] try acces at an empty interface");
		}
	#endif
	return *l_curentInterface;
}

/**
 * @brief Set the curent interface.
 * @note this lock the main mutex
 */
void ewol::eContext::LockContext(void)
{
	MutexInterface().Lock();
	l_curentInterface = this;
}

/**
 * @brief Set the curent interface at NULL.
 * @note this un-lock the main mutex
 */
void ewol::eContext::UnLockContext(void)
{
	l_curentInterface = NULL;
	MutexInterface().UnLock();
}

void ewol::eContext::InputEventTransfertWidget(ewol::Widget* source, ewol::Widget* destination)
{
	m_input.TransfertEvent(source, destination);
}


void ewol::eContext::InputEventGrabPointer(ewol::Widget* widget)
{
	m_input.GrabPointer(widget);
}

void ewol::eContext::InputEventUnGrabPointer(void)
{
	m_input.UnGrabPointer();
}

void ewol::eContext::ProcessEvents(void)
{
	int32_t nbEvent = 0;
	//EWOL_DEBUG(" ********  Event");
	eSystemMessage data;
	while (m_msgSystem.Count()>0) 
	{
		nbEvent++;
		m_msgSystem.Wait(data);
		//EWOL_DEBUG("EVENT");
		switch (data.TypeMessage) {
			case THREAD_INIT:
				// this is due to the openGL context
				/*bool returnVal = */APP_Init(*this);
				break;
			case THREAD_RECALCULATE_SIZE:
				ForceRedrawAll();
				break;
			case THREAD_RESIZE:
				//EWOL_DEBUG("Receive MSG : THREAD_RESIZE");
				m_windowsSize = data.dimention;
				ewol::dimension::SetPixelWindowsSize(m_windowsSize);
				ForceRedrawAll();
				break;
			case THREAD_INPUT_MOTION:
				//EWOL_DEBUG("Receive MSG : THREAD_INPUT_MOTION");
				m_input.Motion(data.inputType, data.inputId, data.dimention);
				break;
			case THREAD_INPUT_STATE:
				//EWOL_DEBUG("Receive MSG : THREAD_INPUT_STATE");
				m_input.State(data.inputType, data.inputId, data.stateIsDown, data.dimention);
				break;
			case THREAD_KEYBORAD_KEY:
			case THREAD_KEYBORAD_MOVE:
				//EWOL_DEBUG("Receive MSG : THREAD_KEYBORAD_KEY");
				{
					ewol::SpecialKey& specialCurrentKey = ewol::GetCurrentSpecialKeyStatus();
					specialCurrentKey = data.keyboardSpecial;
					//EWOL_DEBUG("newStatus Key" << specialCurrentKey);
				}
				if (NULL != m_windowsCurrent) {
					if (false==m_windowsCurrent->OnEventShortCut(data.keyboardSpecial,
					                                             data.keyboardChar,
					                                             data.keyboardMove,
					                                             data.stateIsDown) ) {
						// Get the current Focused Widget :
						ewol::Widget * tmpWidget = m_widgetManager.FocusGet();
						if (NULL != tmpWidget) {
							// check if the widget allow repeating key events.
							//EWOL_DEBUG("repeating test :" << data.repeateKey << " widget=" << tmpWidget->GetKeyboardRepeate() << " state=" << data.stateIsDown);
							if(    false==data.repeateKey
							    || (    true==data.repeateKey
							         && true==tmpWidget->GetKeyboardRepeate()) ) {
								// check Widget shortcut
								if (false==tmpWidget->OnEventShortCut(data.keyboardSpecial,
								                                      data.keyboardChar,
								                                      data.keyboardMove,
								                                      data.stateIsDown) ) {
									// generate the direct event ...
									if (data.TypeMessage == THREAD_KEYBORAD_KEY) {
										ewol::EventEntrySystem tmpEntryEvent(ewol::keyEvent::keyboardChar, ewol::keyEvent::statusUp, data.keyboardChar);
										if(true == data.stateIsDown) {
											tmpEntryEvent.m_event.SetStatus(ewol::keyEvent::statusDown);
										}
										tmpWidget->SystemEventEntry(tmpEntryEvent);
									} else { // THREAD_KEYBORAD_MOVE
										EWOL_DEBUG("THREAD_KEYBORAD_MOVE" << data.keyboardMove << " " << data.stateIsDown);
										ewol::EventEntrySystem tmpEntryEvent(data.keyboardMove, ewol::keyEvent::statusUp, 0);
										if(true == data.stateIsDown) {
											tmpEntryEvent.m_event.SetStatus(ewol::keyEvent::statusDown);
										}
										tmpWidget->SystemEventEntry(tmpEntryEvent);
									}
								} else {
									EWOL_DEBUG("remove Repeate key ...");
								}
							}
						}
					}
				}
				break;
			case THREAD_CLIPBOARD_ARRIVE:
				{
					ewol::Widget * tmpWidget = m_widgetManager.FocusGet();
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

void ewol::eContext::SetArchiveDir(int mode, const char* str)
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



ewol::eContext::eContext(int32_t _argc, const char* _argv[]) :
	m_previousDisplayTime(0),
	m_input(*this),
	m_FpsSystemEvent(  "Event     ", false),
	m_FpsSystemContext("Context   ", false),
	m_FpsSystem(       "Draw      ", true),
	m_FpsFlush(        "Flush     ", false),
	m_windowsCurrent(NULL),
	m_windowsSize(320,480)
{
	m_commandLine.Parse(_argc, _argv);
	EWOL_INFO("==> Ewol System Init (BEGIN)");
	// set the curent interface :
	LockContext();
	// parse the debug level:
	for(esize_t iii=m_commandLine.Size()-1 ; iii>=0 ; --iii) {
		if (m_commandLine.Get(iii) == "-l0") {
			GeneralDebugSetLevel(etk::LOG_LEVEL_NONE);
		} else if (m_commandLine.Get(iii) == "-l1") {
			GeneralDebugSetLevel(etk::LOG_LEVEL_CRITICAL);
		} else if (m_commandLine.Get(iii) == "-l2") {
			GeneralDebugSetLevel(etk::LOG_LEVEL_ERROR);
		} else if (m_commandLine.Get(iii) == "-l3") {
			GeneralDebugSetLevel(etk::LOG_LEVEL_WARNING);
		} else if (m_commandLine.Get(iii) == "-l4") {
			GeneralDebugSetLevel(etk::LOG_LEVEL_INFO);
		} else if (m_commandLine.Get(iii) == "-l5") {
			GeneralDebugSetLevel(etk::LOG_LEVEL_DEBUG);
		} else if(    m_commandLine.Get(iii) == "-l6"
		           || m_commandLine.Get(iii) == "-l7"
		           || m_commandLine.Get(iii) == "-l8"
		           || m_commandLine.Get(iii) == "-l9") {
			GeneralDebugSetLevel(etk::LOG_LEVEL_VERBOSE);
		} else {
			continue;
		}
		m_commandLine.Remove(iii);
	}
	
	EWOL_INFO("v:" << ewol::GetVersion());
	EWOL_INFO("Build Date: " << date::GetYear() << "/" << date::GetMonth() << "/" << date::GetDay() << " " << date::GetHour() << "h" << date::GetMinute());
	// TODO : Remove this ...
	etk::InitDefaultFolder("ewolApplNoName");
	// request the init of the application in the main context of openGL ...
	{
		eSystemMessage data;
		data.TypeMessage = THREAD_INIT;
		m_msgSystem.Post(data);
	}
	// force a recalculation
	RequestUpdateSize();
	#if defined(__EWOL_ANDROID_ORIENTATION_LANDSCAPE__)
		ForceOrientation(ewol::SCREEN_ORIENTATION_LANDSCAPE);
	#elif defined(__EWOL_ANDROID_ORIENTATION_PORTRAIT__)
		ForceOrientation(ewol::SCREEN_ORIENTATION_PORTRAIT);
	#else
		ForceOrientation(ewol::SCREEN_ORIENTATION_AUTO);
	#endif
	// release the curent interface :
	UnLockContext();
	EWOL_INFO("==> Ewol System Init (END)");
}

ewol::eContext::~eContext(void)
{
	EWOL_INFO("==> Ewol System Un-Init (BEGIN)");
	// set the curent interface :
	LockContext();
	// call application to uninit
	APP_UnInit(*this);
	if (NULL!=m_windowsCurrent) {
		EWOL_ERROR("Main windows has not been removed... ==> memory leek");
	}
	// unset all windows
	m_windowsCurrent = NULL;
	m_msgSystem.Clean();
	
	m_EObjectManager.UnInit();
	m_resourceManager.UnInit();
	// release the curent interface :
	UnLockContext();
	EWOL_INFO("==> Ewol System Un-Init (END)");
}


void ewol::eContext::RequestUpdateSize(void)
{
	eSystemMessage data;
	data.TypeMessage = THREAD_RECALCULATE_SIZE;
	m_msgSystem.Post(data);
}

void ewol::eContext::OS_Resize(const vec2& _size)
{
	// TODO : Better in the thread ... ==> but generate some init error ...
	ewol::dimension::SetPixelWindowsSize(_size);
	eSystemMessage data;
	data.TypeMessage = THREAD_RESIZE;
	data.dimention = _size;
	m_msgSystem.Post(data);
}
void ewol::eContext::OS_Move(const vec2& _pos)
{
	/*
	eSystemMessage data;
	data.TypeMessage = THREAD_RESIZE;
	data.resize.w = w;
	data.resize.h = h;
	m_msgSystem.Post(data);
	*/
}

void ewol::eContext::OS_SetInputMotion(int _pointerID, const vec2& _pos )
{
	eSystemMessage data;
	data.TypeMessage = THREAD_INPUT_MOTION;
	data.inputType = ewol::keyEvent::typeFinger;
	data.inputId = _pointerID;
	data.dimention = _pos;
	m_msgSystem.Post(data);
}


void ewol::eContext::OS_SetInputState(int _pointerID, bool _isDown, const vec2& _pos )
{
	eSystemMessage data;
	data.TypeMessage = THREAD_INPUT_STATE;
	data.inputType = ewol::keyEvent::typeFinger;
	data.inputId = _pointerID;
	data.stateIsDown = _isDown;
	data.dimention = _pos;
	m_msgSystem.Post(data);
}


void ewol::eContext::OS_SetMouseMotion(int _pointerID, const vec2& _pos )
{
	eSystemMessage data;
	data.TypeMessage = THREAD_INPUT_MOTION;
	data.inputType = ewol::keyEvent::typeMouse;
	data.inputId = _pointerID;
	data.dimention = _pos;
	m_msgSystem.Post(data);
}


void ewol::eContext::OS_SetMouseState(int _pointerID, bool _isDown, const vec2& _pos )
{
	eSystemMessage data;
	data.TypeMessage = THREAD_INPUT_STATE;
	data.inputType = ewol::keyEvent::typeMouse;
	data.inputId = _pointerID;
	data.stateIsDown = _isDown;
	data.dimention = _pos;
	m_msgSystem.Post(data);
}


void ewol::eContext::OS_SetKeyboard(ewol::SpecialKey& _special,
                                    uniChar_t _myChar,
                                    bool _isDown,
                                    bool _isARepeateKey)
{
	eSystemMessage data;
	data.TypeMessage = THREAD_KEYBORAD_KEY;
	data.stateIsDown = _isDown;
	data.keyboardChar = _myChar;
	data.keyboardSpecial = _special;
	data.repeateKey = _isARepeateKey;
	m_msgSystem.Post(data);
}

void ewol::eContext::OS_SetKeyboardMove(ewol::SpecialKey& _special,
                                        ewol::keyEvent::keyboard_te _move,
                                        bool _isDown,
                                        bool _isARepeateKey)
{
	eSystemMessage data;
	data.TypeMessage = THREAD_KEYBORAD_MOVE;
	data.stateIsDown = _isDown;
	data.keyboardMove = _move;
	data.keyboardSpecial = _special;
	data.repeateKey = _isARepeateKey;
	m_msgSystem.Post(data);
}


void ewol::eContext::OS_Hide(void)
{
	eSystemMessage data;
	data.TypeMessage = THREAD_HIDE;
	m_msgSystem.Post(data);
}

void ewol::eContext::OS_Show(void)
{
	eSystemMessage data;
	data.TypeMessage = THREAD_SHOW;
	m_msgSystem.Post(data);
}


void ewol::eContext::OS_ClipBoardArrive(ewol::clipBoard::clipboardListe_te _clipboardID)
{
	eSystemMessage data;
	data.TypeMessage = THREAD_CLIPBOARD_ARRIVE;
	data.clipboardID = _clipboardID;
	m_msgSystem.Post(data);
}

bool ewol::eContext::OS_Draw(bool _displayEveryTime)
{
	int64_t currentTime = ewol::GetTime();
	// this is to prevent the multiple display at the a high frequency ...
	#if (!defined(__TARGET_OS__Android) && !defined(__TARGET_OS__Windows))
	if(currentTime - m_previousDisplayTime < 1000000/120) {
		usleep(1000);
		return false;
	}
	#endif
	
	m_previousDisplayTime = currentTime;
	
	// process the events
	m_FpsSystemEvent.Tic();
	bool needRedraw = false;
	//! Event management section ...
	{
		// set the curent interface :
		LockContext();
		ProcessEvents();
		// call all the widget that neded to do something periodicly
		//! ewol::widgetManager::PeriodicCall(currentTime);
		m_widgetManager.PeriodicCall(currentTime);
		// Remove all widget that they are no more usefull (these who decided to destroy themself)
		//! ewol::EObjectManager::RemoveAllAutoRemove();
		m_EObjectManager.RemoveAllAutoRemove();
		// check if the user selected a windows
		if (NULL != m_windowsCurrent) {
			// Redraw all needed elements
			m_windowsCurrent->OnRegenerateDisplay();
		}
		m_FpsSystemEvent.IncrementCounter();
		m_FpsSystemEvent.Toc();
		//! bool needRedraw = ewol::widgetManager::IsDrawingNeeded();
		needRedraw = m_widgetManager.IsDrawingNeeded();
		// release the curent interface :
		UnLockContext();
	}
	bool hasDisplayDone = false;
	//! Drawing section :
	{
		// Lock OpenGl context:
		ewol::openGL::Lock();
		m_FpsSystemContext.Tic();
		if (NULL != m_windowsCurrent) {
			if(    true == needRedraw
			    || true == _displayEveryTime) {
				m_resourceManager.UpdateContext();
				m_FpsSystemContext.IncrementCounter();
			}
		}
		m_FpsSystemContext.Toc();
		m_FpsSystem.Tic();
		if (NULL != m_windowsCurrent) {
			if(    true == needRedraw
			    || true == _displayEveryTime) {
				m_FpsSystem.IncrementCounter();
				m_windowsCurrent->SysDraw();
				hasDisplayDone = true;
			}
		}
		m_FpsSystem.Toc();
		
		m_FpsFlush.Tic();
		m_FpsFlush.IncrementCounter();
		glFlush();
		//glFinish();
		m_FpsFlush.Toc();
		// Release Open GL Context
		ewol::openGL::UnLock();
	}
	m_FpsSystemEvent.Draw();
	m_FpsSystemContext.Draw();
	m_FpsSystem.Draw();
	m_FpsFlush.Draw();
	return hasDisplayDone;
}


void ewol::eContext::OnObjectRemove(ewol::EObject * _removeObject)
{
	//EWOL_CRITICAL("element removed");
	m_input.OnObjectRemove(_removeObject);
}


void ewol::eContext::ResetIOEvent(void)
{
	m_input.NewLayerSet();
}


void ewol::eContext::OS_OpenGlContextDestroy(void)
{
	m_resourceManager.ContextHasBeenDestroyed();
}


void ewol::eContext::SetWindows(ewol::Windows* _windows)
{
	// Remove current Focus :
	m_widgetManager.FocusSetDefault(NULL);
	m_widgetManager.FocusRelease();
	// set the new pointer as windows system
	m_windowsCurrent = _windows;
	// Set the new default Focus :
	m_widgetManager.FocusSetDefault(_windows);
	// request all the widget redrawing
	ForceRedrawAll();
}


void ewol::eContext::ForceRedrawAll(void)
{
	if (NULL != m_windowsCurrent) {
		m_windowsCurrent->CalculateSize(vec2(m_windowsSize.x(), m_windowsSize.y()));
	}
}


void ewol::eContext::OS_Stop(void)
{
	if (NULL != m_windowsCurrent) {
		m_windowsCurrent->SysOnKill();
	}
}

void ewol::eContext::OS_Suspend(void)
{
	m_previousDisplayTime = -1;
}

void ewol::eContext::OS_Resume(void)
{
	m_previousDisplayTime = ewol::GetTime();
	m_widgetManager.PeriodicCallResume(m_previousDisplayTime);
}
