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

#include <ewol/renderer/eSystem.h>
#include <ewol/renderer/ResourceManager.h>
#include <ewol/renderer/eSystemInput.h>
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


static ewol::eSystem* l_curentInterface=NULL;
ewol::eSystem& ewol::eSystem::GetSystem(void)
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
void ewol::eSystem::SetSystem(void)
{
	MutexInterface().Lock();
	l_curentInterface = this;
}

/**
 * @brief Set the curent interface at NULL.
 * @note this un-lock the main mutex
 */
void ewol::eSystem::ReleaseSystem(void)
{
	l_curentInterface = NULL;
	MutexInterface().UnLock();
}

void ewol::eSystem::InputEventTransfertWidget(ewol::Widget* source, ewol::Widget* destination)
{
	m_managementInput.TransfertEvent(source, destination);
}


void ewol::eSystem::InputEventGrabPointer(ewol::Widget* widget)
{
	m_managementInput.GrabPointer(widget);
}

void ewol::eSystem::InputEventUnGrabPointer(void)
{
	m_managementInput.UnGrabPointer();
}

void ewol::eSystem::ProcessEvents(void)
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
				eSystem::ForceRedrawAll();
				break;
			case THREAD_RESIZE:
				//EWOL_DEBUG("Receive MSG : THREAD_RESIZE");
				m_windowsSize = data.dimention;
				ewol::dimension::SetPixelWindowsSize(m_windowsSize);
				eSystem::ForceRedrawAll();
				break;
			case THREAD_INPUT_MOTION:
				//EWOL_DEBUG("Receive MSG : THREAD_INPUT_MOTION");
				m_managementInput.Motion(data.inputType, data.inputId, data.dimention);
				break;
			case THREAD_INPUT_STATE:
				//EWOL_DEBUG("Receive MSG : THREAD_INPUT_STATE");
				m_managementInput.State(data.inputType, data.inputId, data.stateIsDown, data.dimention);
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

void ewol::eSystem::SetArchiveDir(int mode, const char* str)
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



ewol::eSystem::eSystem(void) :
	m_previousDisplayTime(0),
	m_managementInput(*this),
	m_FpsSystemEvent(  "Event     ", false),
	m_FpsSystemContext("Context   ", false),
	m_FpsSystem(       "Draw      ", true),
	m_FpsFlush(        "Flush     ", false),
	m_windowsCurrent(NULL),
	m_windowsSize(320,480)
{
	EWOL_INFO("==> Ewol System Init (BEGIN)");
	// set the curent interface :
	SetSystem();
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
	ReleaseSystem();
	EWOL_INFO("==> Ewol System Init (END)");
}

ewol::eSystem::~eSystem(void)
{
	EWOL_INFO("==> Ewol System Un-Init (BEGIN)");
	// set the curent interface :
	SetSystem();
	// call application to uninit
	APP_UnInit(*this);
	if (NULL!=m_windowsCurrent) {
		EWOL_ERROR("Main windows has not been removed... ==> memory leek");
	}
	// unset all windows
	m_windowsCurrent = NULL;
	m_msgSystem.Clean();
	// release the curent interface :
	ReleaseSystem();
	EWOL_INFO("==> Ewol System Un-Init (END)");
}


void ewol::eSystem::RequestUpdateSize(void)
{
	eSystemMessage data;
	data.TypeMessage = THREAD_RECALCULATE_SIZE;
	m_msgSystem.Post(data);
}

void ewol::eSystem::OS_Resize(const vec2& _size)
{
	// TODO : Better in the thread ... ==> but generate some init error ...
	ewol::dimension::SetPixelWindowsSize(_size);
	eSystemMessage data;
	data.TypeMessage = THREAD_RESIZE;
	data.dimention = _size;
	m_msgSystem.Post(data);
}
void ewol::eSystem::OS_Move(const vec2& _pos)
{
	/*
	eSystemMessage data;
	data.TypeMessage = THREAD_RESIZE;
	data.resize.w = w;
	data.resize.h = h;
	m_msgSystem.Post(data);
	*/
}

void ewol::eSystem::OS_SetInputMotion(int _pointerID, const vec2& _pos )
{
	eSystemMessage data;
	data.TypeMessage = THREAD_INPUT_MOTION;
	data.inputType = ewol::keyEvent::typeFinger;
	data.inputId = _pointerID;
	data.dimention = _pos;
	m_msgSystem.Post(data);
}


void ewol::eSystem::OS_SetInputState(int _pointerID, bool _isDown, const vec2& _pos )
{
	eSystemMessage data;
	data.TypeMessage = THREAD_INPUT_STATE;
	data.inputType = ewol::keyEvent::typeFinger;
	data.inputId = _pointerID;
	data.stateIsDown = _isDown;
	data.dimention = _pos;
	m_msgSystem.Post(data);
}


void ewol::eSystem::OS_SetMouseMotion(int _pointerID, const vec2& _pos )
{
	eSystemMessage data;
	data.TypeMessage = THREAD_INPUT_MOTION;
	data.inputType = ewol::keyEvent::typeMouse;
	data.inputId = _pointerID;
	data.dimention = _pos;
	m_msgSystem.Post(data);
}


void ewol::eSystem::OS_SetMouseState(int _pointerID, bool _isDown, const vec2& _pos )
{
	eSystemMessage data;
	data.TypeMessage = THREAD_INPUT_STATE;
	data.inputType = ewol::keyEvent::typeMouse;
	data.inputId = _pointerID;
	data.stateIsDown = _isDown;
	data.dimention = _pos;
	m_msgSystem.Post(data);
}


void ewol::eSystem::OS_SetKeyboard(ewol::SpecialKey& _special,
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

void ewol::eSystem::OS_SetKeyboardMove(ewol::SpecialKey& _special,
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


void ewol::eSystem::OS_Hide(void)
{
	eSystemMessage data;
	data.TypeMessage = THREAD_HIDE;
	m_msgSystem.Post(data);
}

void ewol::eSystem::OS_Show(void)
{
	eSystemMessage data;
	data.TypeMessage = THREAD_SHOW;
	m_msgSystem.Post(data);
}


void ewol::eSystem::OS_ClipBoardArrive(ewol::clipBoard::clipboardListe_te _clipboardID)
{
	eSystemMessage data;
	data.TypeMessage = THREAD_CLIPBOARD_ARRIVE;
	data.clipboardID = _clipboardID;
	m_msgSystem.Post(data);
}

bool ewol::eSystem::OS_Draw(bool _displayEveryTime)
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
		SetSystem();
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
		ReleaseSystem();
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

/*
void ewol::eSystem::OnObjectRemove(ewol::EObject * removeObject)
{
	m_managementInput.OnObjectRemove(removeObject);
}
*/

void ewol::eSystem::ResetIOEvent(void)
{
	m_managementInput.NewLayerSet();
}


void ewol::eSystem::OS_OpenGlContextDestroy(void)
{
	m_resourceManager.ContextHasBeenDestroyed();
}


void ewol::eSystem::SetWindows(ewol::Windows* _windows)
{
	// set the new pointer as windows system
	m_windowsCurrent = _windows;
	// request all the widget redrawing
	ForceRedrawAll();
}


void ewol::eSystem::ForceRedrawAll(void)
{
	if (NULL != m_windowsCurrent) {
		m_windowsCurrent->CalculateSize(vec2(m_windowsSize.x(), m_windowsSize.y()));
	}
}


void ewol::eSystem::OS_Stop(void)
{
	if (NULL != m_windowsCurrent) {
		m_windowsCurrent->SysOnKill();
	}
}

