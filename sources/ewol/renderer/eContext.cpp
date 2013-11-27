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
 * @brief get the main ewol mutex (event or periodic call mutex).
 * @note due ti the fact that the system can be called for multiple instance, for naw we just limit the acces to one process at a time.
 * @return the main inteface Mutex
 */
static etk::Mutex& mutexInterface(void) {
	static etk::Mutex s_interfaceMutex;
	return s_interfaceMutex;
}


static ewol::eContext* l_curentInterface=NULL;
ewol::eContext& ewol::getContext(void) {
	#if DEBUG_LEVEL > 2
		if(NULL == l_curentInterface){
			EWOL_CRITICAL("[CRITICAL] try acces at an empty interface");
		}
	#endif
	return *l_curentInterface;
}

/**
 * @brief set the curent interface.
 * @note this lock the main mutex
 */
void ewol::eContext::lockContext(void) {
	mutexInterface().lock();
	l_curentInterface = this;
}

/**
 * @brief set the curent interface at NULL.
 * @note this un-lock the main mutex
 */
void ewol::eContext::unLockContext(void) {
	l_curentInterface = NULL;
	mutexInterface().unLock();
}

void ewol::eContext::inputEventTransfertWidget(ewol::Widget* _source,
                                               ewol::Widget* _destination) {
	m_input.transfertEvent(_source, _destination);
}


void ewol::eContext::inputEventGrabPointer(ewol::Widget* _widget) {
	m_input.grabPointer(_widget);
}

void ewol::eContext::inputEventUnGrabPointer(void) {
	m_input.unGrabPointer();
}

void ewol::eContext::processEvents(void) {
	int32_t nbEvent = 0;
	//EWOL_DEBUG(" ********  Event");
	eSystemMessage data;
	while (m_msgSystem.count()>0) {
		nbEvent++;
		m_msgSystem.wait(data);
		//EWOL_DEBUG("EVENT");
		switch (data.TypeMessage) {
			case eSystemMessage::msgInit:
				// this is due to the openGL context
				/*bool returnVal = */APP_Init(*this);
				break;
			case eSystemMessage::msgRecalculateSize:
				forceRedrawAll();
				break;
			case eSystemMessage::msgResize:
				//EWOL_DEBUG("Receive MSG : THREAD_RESIZE");
				m_windowsSize = data.dimention;
				ewol::dimension::setPixelWindowsSize(m_windowsSize);
				forceRedrawAll();
				break;
			case eSystemMessage::msgInputMotion:
				//EWOL_DEBUG("Receive MSG : THREAD_INPUT_MOTION");
				m_input.motion(data.inputType, data.inputId, data.dimention);
				break;
			case eSystemMessage::msgInputState:
				//EWOL_DEBUG("Receive MSG : THREAD_INPUT_STATE");
				m_input.state(data.inputType, data.inputId, data.stateIsDown, data.dimention);
				break;
			case eSystemMessage::msgKeyboardKey:
			case eSystemMessage::msgKeyboardMove:
				//EWOL_DEBUG("Receive MSG : THREAD_KEYBORAD_KEY");
				if (NULL != m_windowsCurrent) {
					if (false == m_windowsCurrent->onEventShortCut(data.keyboardSpecial,
					                                             data.keyboardChar,
					                                             data.keyboardMove,
					                                             data.stateIsDown) ) {
						// get the current focused Widget :
						ewol::Widget * tmpWidget = m_widgetManager.focusGet();
						if (NULL != tmpWidget) {
							// check if the widget allow repeating key events.
							//EWOL_DEBUG("repeating test :" << data.repeateKey << " widget=" << tmpWidget->getKeyboardRepeate() << " state=" << data.stateIsDown);
							if(    false == data.repeateKey
							    || (    true == data.repeateKey
							         && true == tmpWidget->getKeyboardRepeate()) ) {
								// check Widget shortcut
								if (false == tmpWidget->onEventShortCut(data.keyboardSpecial,
								                                      data.keyboardChar,
								                                      data.keyboardMove,
								                                      data.stateIsDown) ) {
									// generate the direct event ...
									if (data.TypeMessage == eSystemMessage::msgKeyboardKey) {
										ewol::EventEntrySystem tmpEntryEvent(ewol::keyEvent::keyboardChar,
										                                     ewol::keyEvent::statusUp,
										                                     data.keyboardSpecial,
										                                     data.keyboardChar);
										if(true == data.stateIsDown) {
											tmpEntryEvent.m_event.setStatus(ewol::keyEvent::statusDown);
										}
										tmpWidget->systemEventEntry(tmpEntryEvent);
									} else { // THREAD_KEYBORAD_MOVE
										EWOL_DEBUG("THREAD_KEYBORAD_MOVE" << data.keyboardMove << " " << data.stateIsDown);
										ewol::EventEntrySystem tmpEntryEvent(data.keyboardMove,
										                                     ewol::keyEvent::statusUp,
										                                     data.keyboardSpecial,
										                                     0);
										if(true == data.stateIsDown) {
											tmpEntryEvent.m_event.setStatus(ewol::keyEvent::statusDown);
										}
										tmpWidget->systemEventEntry(tmpEntryEvent);
									}
								} else {
									EWOL_DEBUG("remove Repeate key ...");
								}
							}
						}
					}
				}
				break;
			case eSystemMessage::msgClipboardArrive:
				{
					ewol::Widget * tmpWidget = m_widgetManager.focusGet();
					if (tmpWidget != NULL) {
						tmpWidget->onEventClipboard(data.clipboardID);
					}
				}
				break;
			case eSystemMessage::msgHide:
				EWOL_DEBUG("Receive MSG : msgHide");
				//guiAbstraction::SendKeyboardEventMove(tmpData->isDown, tmpData->move);
				//gui_uniqueWindows->SysOnHide();
				break;
			case eSystemMessage::msgShow:
				EWOL_DEBUG("Receive MSG : msgShow");
				//guiAbstraction::SendKeyboardEventMove(tmpData->isDown, tmpData->move);
				//gui_uniqueWindows->SysOnShow();
				break;
			default:
				EWOL_DEBUG("Receive MSG : UNKNOW");
				break;
		}
	}
}

void ewol::eContext::setArchiveDir(int _mode, const char* _str) {
	switch(_mode) {
		case 0:
			EWOL_DEBUG("Directory APK : path=" << _str);
			etk::setBaseFolderData(_str);
			break;
		case 1:
			EWOL_DEBUG("Directory mode=FILE path=" << _str);
			etk::setBaseFolderDataUser(_str);
			break;
		case 2:
			EWOL_DEBUG("Directory mode=CACHE path=" << _str);
			etk::setBaseFolderCache(_str);
			break;
		case 3:
			EWOL_DEBUG("Directory mode=EXTERNAL_CACHE path=" << _str);
			break;
		default:
			EWOL_DEBUG("Directory mode=???? path=" << _str);
			break;
	}
}



ewol::eContext::eContext(int32_t _argc, const char* _argv[]) :
  m_previousDisplayTime(0),
  m_input(*this),
#if defined(__TARGET_OS__Android)
  m_displayFps(true),
#else
  m_displayFps(false),
#endif
  m_FpsSystemEvent(  "Event     ", false),
  m_FpsSystemContext("Context   ", false),
  m_FpsSystem(       "Draw      ", true),
  m_FpsFlush(        "Flush     ", false),
  m_windowsCurrent(NULL),
  m_windowsSize(320,480) {
	m_commandLine.parse(_argc, _argv);
	EWOL_INFO(" == > Ewol system init (BEGIN)");
	// set the curent interface :
	lockContext();
	// parse the debug level:
	for(int32_t iii=m_commandLine.size()-1 ; iii >= 0 ; --iii) {
		if (m_commandLine.get(iii) == "-l0") {
			debug::setGeneralLevel(etk::logLevelNone);
		} else if (m_commandLine.get(iii) == "-l1") {
			debug::setGeneralLevel(etk::logLevelCritical);
		} else if (m_commandLine.get(iii) == "-l2") {
			debug::setGeneralLevel(etk::logLevelError);
		} else if (m_commandLine.get(iii) == "-l3") {
			debug::setGeneralLevel(etk::logLevelWarning);
		} else if (m_commandLine.get(iii) == "-l4") {
			debug::setGeneralLevel(etk::logLevelInfo);
		} else if (m_commandLine.get(iii) == "-l5") {
			debug::setGeneralLevel(etk::logLevelDebug);
		} else if(    m_commandLine.get(iii) == "-l6"
		           || m_commandLine.get(iii) == "-l7"
		           || m_commandLine.get(iii) == "-l8"
		           || m_commandLine.get(iii) == "-l9") {
			debug::setGeneralLevel(etk::logLevelVerbose);
		} else if (m_commandLine.get(iii) == "-fps") {
			m_displayFps=true;
		} else {
			continue;
		}
		m_commandLine.remove(iii);
	}
	
	EWOL_INFO("v:" << ewol::getVersion());
	EWOL_INFO("Build Date: " << date::getYear() << "/" << date::getMonth() << "/" << date::getDay() << " " << date::getHour() << "h" << date::getMinute());
	// TODO : remove this ...
	etk::initDefaultFolder("ewolApplNoName");
	// request the init of the application in the main context of openGL ...
	{
		eSystemMessage data;
		data.TypeMessage = eSystemMessage::msgInit;
		m_msgSystem.post(data);
	}
	// force a recalculation
	requestUpdateSize();
	#if defined(__EWOL_ANDROID_ORIENTATION_LANDSCAPE__)
		forceOrientation(ewol::screenLandscape);
	#elif defined(__EWOL_ANDROID_ORIENTATION_PORTRAIT__)
		forceOrientation(ewol::screenPortrait);
	#else
		forceOrientation(ewol::screenAuto);
	#endif
	// release the curent interface :
	unLockContext();
	EWOL_INFO(" == > Ewol system init (END)");
}

ewol::eContext::~eContext(void) {
	EWOL_INFO(" == > Ewol system Un-Init (BEGIN)");
	// set the curent interface :
	lockContext();
	// call application to uninit
	APP_UnInit(*this);
	if (NULL!=m_windowsCurrent) {
		EWOL_ERROR("Main windows has not been removed...  == > memory leek");
	}
	// unset all windows
	m_windowsCurrent = NULL;
	m_msgSystem.clean();
	
	m_EObjectManager.unInit();
	m_resourceManager.unInit();
	// release the curent interface :
	unLockContext();
	EWOL_INFO(" == > Ewol system Un-Init (END)");
}

void ewol::eContext::requestUpdateSize(void) {
	eSystemMessage data;
	data.TypeMessage = eSystemMessage::msgRecalculateSize;
	m_msgSystem.post(data);
}

void ewol::eContext::OS_Resize(const vec2& _size) {
	// TODO : Better in the thread ...  == > but generate some init error ...
	ewol::dimension::setPixelWindowsSize(_size);
	eSystemMessage data;
	data.TypeMessage = eSystemMessage::msgResize;
	data.dimention = _size;
	m_msgSystem.post(data);
}
void ewol::eContext::OS_Move(const vec2& _pos) {
	/*
	eSystemMessage data;
	data.TypeMessage = eSystemMessage::msgResize;
	data.resize.w = w;
	data.resize.h = h;
	m_msgSystem.Post(data);
	*/
}

void ewol::eContext::OS_SetInputMotion(int _pointerID, const vec2& _pos ) {
	eSystemMessage data;
	data.TypeMessage = eSystemMessage::msgInputMotion;
	data.inputType = ewol::keyEvent::typeFinger;
	data.inputId = _pointerID;
	data.dimention = _pos;
	m_msgSystem.post(data);
}

void ewol::eContext::OS_SetInputState(int _pointerID, bool _isDown, const vec2& _pos ) {
	eSystemMessage data;
	data.TypeMessage = eSystemMessage::msgInputState;
	data.inputType = ewol::keyEvent::typeFinger;
	data.inputId = _pointerID;
	data.stateIsDown = _isDown;
	data.dimention = _pos;
	m_msgSystem.post(data);
}

void ewol::eContext::OS_SetMouseMotion(int _pointerID, const vec2& _pos ) {
	eSystemMessage data;
	data.TypeMessage = eSystemMessage::msgInputMotion;
	data.inputType = ewol::keyEvent::typeMouse;
	data.inputId = _pointerID;
	data.dimention = _pos;
	m_msgSystem.post(data);
}

void ewol::eContext::OS_SetMouseState(int _pointerID, bool _isDown, const vec2& _pos ) {
	eSystemMessage data;
	data.TypeMessage = eSystemMessage::msgInputState;
	data.inputType = ewol::keyEvent::typeMouse;
	data.inputId = _pointerID;
	data.stateIsDown = _isDown;
	data.dimention = _pos;
	m_msgSystem.post(data);
}

void ewol::eContext::OS_SetKeyboard(ewol::SpecialKey& _special,
                                    char32_t _myChar,
                                    bool _isDown,
                                    bool _isARepeateKey) {
	eSystemMessage data;
	data.TypeMessage = eSystemMessage::msgKeyboardKey;
	data.stateIsDown = _isDown;
	data.keyboardChar = _myChar;
	data.keyboardSpecial = _special;
	data.repeateKey = _isARepeateKey;
	m_msgSystem.post(data);
}

void ewol::eContext::OS_SetKeyboardMove(ewol::SpecialKey& _special,
                                        enum ewol::keyEvent::keyboard _move,
                                        bool _isDown,
                                        bool _isARepeateKey) {
	eSystemMessage data;
	data.TypeMessage = eSystemMessage::msgKeyboardMove;
	data.stateIsDown = _isDown;
	data.keyboardMove = _move;
	data.keyboardSpecial = _special;
	data.repeateKey = _isARepeateKey;
	m_msgSystem.post(data);
}

void ewol::eContext::OS_Hide(void) {
	eSystemMessage data;
	data.TypeMessage = eSystemMessage::msgHide;
	m_msgSystem.post(data);
}

void ewol::eContext::OS_Show(void) {
	eSystemMessage data;
	data.TypeMessage = eSystemMessage::msgShow;
	m_msgSystem.post(data);
}


void ewol::eContext::OS_ClipBoardArrive(enum ewol::clipBoard::clipboardListe _clipboardID) {
	eSystemMessage data;
	data.TypeMessage = eSystemMessage::msgClipboardArrive;
	data.clipboardID = _clipboardID;
	m_msgSystem.post(data);
}

bool ewol::eContext::OS_Draw(bool _displayEveryTime) {
	int64_t currentTime = ewol::getTime();
	// this is to prevent the multiple display at the a high frequency ...
	#if (!defined(__TARGET_OS__Android) && !defined(__TARGET_OS__Windows))
	if(currentTime - m_previousDisplayTime < 1000000/120) {
		usleep(1000);
		return false;
	}
	#endif
	
	m_previousDisplayTime = currentTime;
	
	// process the events
	if (m_displayFps == true) {
		m_FpsSystemEvent.tic();
	}
	bool needRedraw = false;
	//! Event management section ...
	{
		// set the curent interface :
		lockContext();
		processEvents();
		// call all the widget that neded to do something periodicly
		//! ewol::widgetManager::periodicCall(currentTime);
		m_widgetManager.periodicCall(currentTime);
		// remove all widget that they are no more usefull (these who decided to destroy themself)
		//! ewol::EObjectManager::removeAllAutoRemove();
		m_EObjectManager.removeAllAutoRemove();
		// check if the user selected a windows
		if (NULL != m_windowsCurrent) {
			// Redraw all needed elements
			m_windowsCurrent->onRegenerateDisplay();
		}
		if (m_displayFps == true) {
			m_FpsSystemEvent.incrementCounter();
			m_FpsSystemEvent.toc();
		}
		//! bool needRedraw = ewol::widgetManager::isDrawingNeeded();
		needRedraw = m_widgetManager.isDrawingNeeded();
		// release the curent interface :
		unLockContext();
	}
	bool hasDisplayDone = false;
	//! drawing section :
	{
		// Lock openGl context:
		ewol::openGL::lock();
		if (m_displayFps == true) {
			m_FpsSystemContext.tic();
		}
		if (NULL != m_windowsCurrent) {
			if(    true == needRedraw
			    || true == _displayEveryTime) {
				m_resourceManager.updateContext();
				if (m_displayFps == true) {
					m_FpsSystemContext.incrementCounter();
				}
			}
		}
		if (m_displayFps == true) {
			m_FpsSystemContext.toc();
			m_FpsSystem.tic();
		}
		if (NULL != m_windowsCurrent) {
			if(    true == needRedraw
			    || true == _displayEveryTime) {
				m_FpsSystem.incrementCounter();
				m_windowsCurrent->sysDraw();
				hasDisplayDone = true;
			}
		}
		if (m_displayFps == true) {
			m_FpsSystem.toc();
			m_FpsFlush.tic();
			m_FpsFlush.incrementCounter();
		}
		glFlush();
		//glFinish();
		if (m_displayFps == true) {
			m_FpsFlush.toc();
		}
		// release open GL Context
		ewol::openGL::unLock();
	}
	if (m_displayFps == true) {
		m_FpsSystemEvent.draw();
		m_FpsSystemContext.draw();
		m_FpsSystem.draw();
		m_FpsFlush.draw();
	}
	return hasDisplayDone;
}

void ewol::eContext::onObjectRemove(ewol::EObject * _removeObject) {
	//EWOL_CRITICAL("element removed");
	m_input.onObjectRemove(_removeObject);
}

void ewol::eContext::resetIOEvent(void) {
	m_input.newLayerSet();
}

void ewol::eContext::OS_OpenGlContextDestroy(void) {
	m_resourceManager.contextHasBeenDestroyed();
}

void ewol::eContext::setWindows(ewol::Windows* _windows) {
	// remove current focus :
	m_widgetManager.focusSetDefault(NULL);
	m_widgetManager.focusRelease();
	// set the new pointer as windows system
	m_windowsCurrent = _windows;
	// set the new default focus :
	m_widgetManager.focusSetDefault(_windows);
	// request all the widget redrawing
	forceRedrawAll();
}

void ewol::eContext::forceRedrawAll(void) {
	if (NULL != m_windowsCurrent) {
		m_windowsCurrent->calculateSize(vec2(m_windowsSize.x(), m_windowsSize.y()));
	}
}

void ewol::eContext::OS_Stop(void) {
	if (NULL != m_windowsCurrent) {
		m_windowsCurrent->sysOnKill();
	}
}

void ewol::eContext::OS_Suspend(void) {
	m_previousDisplayTime = -1;
}

void ewol::eContext::OS_Resume(void) {
	m_previousDisplayTime = ewol::getTime();
	m_widgetManager.periodicCallResume(m_previousDisplayTime);
}


void ewol::eContext::stop(void) {
	
}

void ewol::eContext::setSize(const vec2& _size) {
	EWOL_INFO("setSize: NOT implemented ...");
};

void ewol::eContext::setPos(const vec2& _pos) {
	EWOL_INFO("setPos: NOT implemented ...");
}

void ewol::eContext::hide(void) {
	EWOL_INFO("hide: NOT implemented ...");
};

void ewol::eContext::show(void) {
	EWOL_INFO("show: NOT implemented ...");
}

void ewol::eContext::setTitle(const std::string& _title) {
	EWOL_INFO("setTitle: NOT implemented ...");
}

void ewol::eContext::keyboardShow(void) {
	EWOL_INFO("keyboardShow: NOT implemented ...");
}


void ewol::eContext::keyboardHide(void) {
	EWOL_INFO("keyboardHide: NOT implemented ...");
}
