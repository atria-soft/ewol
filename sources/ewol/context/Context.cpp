/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <unistd.h>

#include <etk/types.h>

#include <etk/tool.h>
#include <etk/os/FSNode.h>
#include <etk/os/Mutex.h>

#include <date/date.h>

#include <ewol/ewol.h>
#include <ewol/Dimension.h>
#include <ewol/debug.h>

#include <ewol/object/Object.h>
#include <ewol/object/Manager.h>

#include <ewol/context/Context.h>
#include <ewol/resource/Manager.h>
#include <ewol/openGL/openGL.h>

#include <ewol/widget/Manager.h>
#include <ewol/translate.h>



/**
 * @brief get the main ewol mutex (event or periodic call mutex).
 * @note due ti the fact that the system can be called for multiple instance, for naw we just limit the acces to one process at a time.
 * @return the main inteface Mutex
 */
static etk::Mutex& mutexInterface(void) {
	static etk::Mutex s_interfaceMutex;
	return s_interfaceMutex;
}


static ewol::Context* l_curentInterface=NULL;
ewol::Context& ewol::getContext(void) {
	#if DEBUG_LEVEL > 2
		if(NULL == l_curentInterface){
			EWOL_CRITICAL("[CRITICAL] try acces at an empty interface");
		}
	#endif
	return *l_curentInterface;
}



void ewol::Context::setInitImage(const std::string& _fileName) {
	//m_initDisplayImageName = _fileName;
}



/**
 * @brief set the curent interface.
 * @note this lock the main mutex
 */
void ewol::Context::lockContext(void) {
	mutexInterface().lock();
	l_curentInterface = this;
}

/**
 * @brief set the curent interface at NULL.
 * @note this un-lock the main mutex
 */
void ewol::Context::unLockContext(void) {
	l_curentInterface = NULL;
	mutexInterface().unLock();
}


namespace ewol {
	class eSystemMessage {
		public:
			enum theadMessage {
				msgNone,
				msgInit,
				msgRecalculateSize,
				msgResize,
				msgHide,
				msgShow,
				
				msgInputMotion,
				msgInputState,
				
				msgKeyboardKey,
				msgKeyboardMove,
				
				msgClipboardArrive
			};
		public :
			// specify the message type
			enum theadMessage TypeMessage;
			// can not set a union ...
			enum ewol::context::clipBoard::clipboardListe clipboardID;
			// InputId
			enum ewol::key::type inputType;
			int32_t                 inputId;
			// generic dimentions
			vec2 dimention;
			// keyboard events :
			bool                        repeateKey;  //!< special flag for the repeating key on the PC interface
			bool                        stateIsDown;
			char32_t                   keyboardChar;
			enum ewol::key::keyboard keyboardMove;
			ewol::key::Special            keyboardSpecial;
			
			eSystemMessage(void) :
				TypeMessage(msgNone),
				clipboardID(ewol::context::clipBoard::clipboardStd),
				inputType(ewol::key::typeUnknow),
				inputId(-1),
				dimention(0,0),
				repeateKey(false),
				stateIsDown(false),
				keyboardChar(0),
				keyboardMove(ewol::key::keyboardUnknow)
			{
				
			}
	};
};


void ewol::Context::inputEventTransfertWidget(ewol::Widget* _source,
                                               ewol::Widget* _destination) {
	m_input.transfertEvent(_source, _destination);
}


void ewol::Context::inputEventGrabPointer(ewol::Widget* _widget) {
	m_input.grabPointer(_widget);
}

void ewol::Context::inputEventUnGrabPointer(void) {
	m_input.unGrabPointer();
}

void ewol::Context::processEvents(void) {
	int32_t nbEvent = 0;
	//EWOL_DEBUG(" ********  Event");
	ewol::eSystemMessage* data = NULL;
	while (m_msgSystem.count()>0) {
		nbEvent++;
		if (data != NULL) {
			delete(data);
			data = NULL;
		}
		m_msgSystem.wait(data);
		//EWOL_DEBUG("EVENT");
		switch (data->TypeMessage) {
			case eSystemMessage::msgInit:
				// this is due to the openGL context
				/*bool returnVal = */
				APP_Init(*this, m_initStepId, m_initTotalStep);
				m_initStepId++;
				break;
			case eSystemMessage::msgRecalculateSize:
				forceRedrawAll();
				break;
			case eSystemMessage::msgResize:
				//EWOL_DEBUG("Receive MSG : THREAD_RESIZE");
				m_windowsSize = data->dimention;
				ewol::Dimension::setPixelWindowsSize(m_windowsSize);
				forceRedrawAll();
				break;
			case eSystemMessage::msgInputMotion:
				//EWOL_DEBUG("Receive MSG : THREAD_INPUT_MOTION");
				m_input.motion(data->inputType, data->inputId, data->dimention);
				break;
			case eSystemMessage::msgInputState:
				//EWOL_DEBUG("Receive MSG : THREAD_INPUT_STATE");
				m_input.state(data->inputType, data->inputId, data->stateIsDown, data->dimention);
				break;
			case eSystemMessage::msgKeyboardKey:
			case eSystemMessage::msgKeyboardMove:
				//EWOL_DEBUG("Receive MSG : THREAD_KEYBORAD_KEY");
				// store the keyboard special key status for mouse event...
				m_input.setLastKeyboardSpecial(data->keyboardSpecial);
				if (NULL != m_windowsCurrent) {
					if (false == m_windowsCurrent->onEventShortCut(data->keyboardSpecial,
					                                             data->keyboardChar,
					                                             data->keyboardMove,
					                                             data->stateIsDown) ) {
						// get the current focused Widget :
						ewol::Widget * tmpWidget = m_widgetManager.focusGet();
						if (NULL != tmpWidget) {
							// check if the widget allow repeating key events.
							//EWOL_DEBUG("repeating test :" << data->repeateKey << " widget=" << tmpWidget->getKeyboardRepeate() << " state=" << data->stateIsDown);
							if(    false == data->repeateKey
							    || (    true == data->repeateKey
							         && true == tmpWidget->getKeyboardRepeate()) ) {
								// check Widget shortcut
								if (false == tmpWidget->onEventShortCut(data->keyboardSpecial,
								                                      data->keyboardChar,
								                                      data->keyboardMove,
								                                      data->stateIsDown) ) {
									// generate the direct event ...
									if (data->TypeMessage == eSystemMessage::msgKeyboardKey) {
										ewol::event::EntrySystem tmpEntryEvent(ewol::key::keyboardChar,
										                                     ewol::key::statusUp,
										                                     data->keyboardSpecial,
										                                     data->keyboardChar);
										if(true == data->stateIsDown) {
											tmpEntryEvent.m_event.setStatus(ewol::key::statusDown);
										}
										tmpWidget->systemEventEntry(tmpEntryEvent);
									} else { // THREAD_KEYBORAD_MOVE
										EWOL_DEBUG("THREAD_KEYBORAD_MOVE" << data->keyboardMove << " " << data->stateIsDown);
										ewol::event::EntrySystem tmpEntryEvent(data->keyboardMove,
										                                     ewol::key::statusUp,
										                                     data->keyboardSpecial,
										                                     0);
										if(true == data->stateIsDown) {
											tmpEntryEvent.m_event.setStatus(ewol::key::statusDown);
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
						tmpWidget->onEventClipboard(data->clipboardID);
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

void ewol::Context::setArchiveDir(int _mode, const char* _str) {
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



ewol::Context::Context(int32_t _argc, const char* _argv[]) :
  m_previousDisplayTime(0),
  m_input(*this),
#if (defined(__TARGET_OS__Android) || defined(__TARGET_OS__IOs))
  m_displayFps(true),
#else
  m_displayFps(false),
#endif
  m_FpsSystemEvent(  "Event     ", false),
  m_FpsSystemContext("Context   ", false),
  m_FpsSystem(       "Draw      ", true),
  m_FpsFlush(        "Flush     ", false),
  m_windowsCurrent(NULL),
  m_windowsSize(320,480),
  m_initStepId(0),
  m_initTotalStep(1) {
	m_commandLine.parse(_argc, _argv);
	EWOL_INFO(" == > Ewol system init (BEGIN)");
	// Add basic ewol translation:
	ewol::translate::addPath("ewol", "DATA:translate/ewol/");
	ewol::translate::autoDetectLanguage();
	// Reset the random system to be sure have real random values...
	etk::tool::resetRandom();
	// set the curent interface :
	lockContext();
	// By default we set 2 themes (1 color and 1 shape ...) :
	etk::theme::setNameDefault("GUI", "shape/square/");
	etk::theme::setNameDefault("COLOR", "color/black/");
	// parse the debug level:
	for(int32_t iii = 0; iii < m_commandLine.size() ; ++iii) {
		if (m_commandLine.get(iii) == "-l0") {
			etk::log::setLevel(etk::log::logLevelNone);
		} else if (m_commandLine.get(iii) == "-l1") {
			etk::log::setLevel(etk::log::logLevelCritical);
		} else if (m_commandLine.get(iii) == "-l2") {
			etk::log::setLevel(etk::log::logLevelError);
		} else if (m_commandLine.get(iii) == "-l3") {
			etk::log::setLevel(etk::log::logLevelWarning);
		} else if (m_commandLine.get(iii) == "-l4") {
			etk::log::setLevel(etk::log::logLevelInfo);
		} else if (m_commandLine.get(iii) == "-l5") {
			etk::log::setLevel(etk::log::logLevelDebug);
		} else if(    m_commandLine.get(iii) == "-l6"
		           || m_commandLine.get(iii) == "-l7"
		           || m_commandLine.get(iii) == "-l8"
		           || m_commandLine.get(iii) == "-l9") {
			etk::log::setLevel(etk::log::logLevelVerbose);
		} else if (m_commandLine.get(iii) == "-fps") {
			m_displayFps=true;
		} else if (m_commandLine.get(iii) == "--dbg-file") {
			// TODO : Set it back ...
			//etk::cout.setOutputFile(true);
		} else {
			continue;
		}
		m_commandLine.remove(iii);
		--iii;
	}
	//etk::cout.setOutputFile(true);
	
	EWOL_INFO("EWOL v:" << ewol::getVersion());
	EWOL_INFO("Build Date: " << date::getYear() << "/" << date::getMonth() << "/" << date::getDay() << " " << date::getHour() << "h" << date::getMinute());
	// TODO : remove this ...
	etk::initDefaultFolder("ewolApplNoName");
	// request the init of the application in the main context of openGL ...
	{
		ewol::eSystemMessage *data = new ewol::eSystemMessage();
		if (data == NULL) {
			EWOL_ERROR("allocationerror of message");
		} else {
			data->TypeMessage = eSystemMessage::msgInit;
			m_msgSystem.post(data);
		}
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

ewol::Context::~Context(void) {
	EWOL_INFO(" == > Ewol system Un-Init (BEGIN)");
	// TODO : Clean the message list ...
	// set the curent interface :
	lockContext();
	// call application to uninit
	APP_UnInit(*this);
	if (m_windowsCurrent != NULL) {
		EWOL_DEBUG("Main windows has not been auto-removed...");
		m_windowsCurrent->removeObject();
		m_windowsCurrent = NULL;
	}
	// unset all windows
	m_msgSystem.clean();
	
	m_objectManager.unInit();
	m_resourceManager.unInit();
	// release the curent interface :
	unLockContext();
	EWOL_INFO(" == > Ewol system Un-Init (END)");
}

void ewol::Context::requestUpdateSize(void) {
	ewol::eSystemMessage *data = new ewol::eSystemMessage();
	if (data == NULL) {
		EWOL_ERROR("allocationerror of message");
		return;
	}
	data->TypeMessage = eSystemMessage::msgRecalculateSize;
	m_msgSystem.post(data);
}

void ewol::Context::OS_Resize(const vec2& _size) {
	// TODO : Better in the thread ...  == > but generate some init error ...
	ewol::Dimension::setPixelWindowsSize(_size);
	ewol::eSystemMessage *data = new ewol::eSystemMessage();
	if (data == NULL) {
		EWOL_ERROR("allocationerror of message");
		return;
	}
	data->TypeMessage = eSystemMessage::msgResize;
	data->dimention = _size;
	m_msgSystem.post(data);
}
void ewol::Context::OS_Move(const vec2& _pos) {
	/*
	ewol::eSystemMessage *data = new ewol::eSystemMessage();
	data->TypeMessage = eSystemMessage::msgResize;
	data->resize.w = w;
	data->resize.h = h;
	m_msgSystem.Post(data);
	*/
}

void ewol::Context::OS_SetInputMotion(int _pointerID, const vec2& _pos ) {
	ewol::eSystemMessage *data = new ewol::eSystemMessage();
	if (data == NULL) {
		EWOL_ERROR("allocationerror of message");
		return;
	}
	data->TypeMessage = eSystemMessage::msgInputMotion;
	data->inputType = ewol::key::typeFinger;
	data->inputId = _pointerID;
	data->dimention = _pos;
	m_msgSystem.post(data);
}

void ewol::Context::OS_SetInputState(int _pointerID, bool _isDown, const vec2& _pos ) {
	ewol::eSystemMessage *data = new ewol::eSystemMessage();
	if (data == NULL) {
		EWOL_ERROR("allocationerror of message");
		return;
	}
	data->TypeMessage = eSystemMessage::msgInputState;
	data->inputType = ewol::key::typeFinger;
	data->inputId = _pointerID;
	data->stateIsDown = _isDown;
	data->dimention = _pos;
	m_msgSystem.post(data);
}

void ewol::Context::OS_SetMouseMotion(int _pointerID, const vec2& _pos ) {
	ewol::eSystemMessage *data = new ewol::eSystemMessage();
	if (data == NULL) {
		EWOL_ERROR("allocationerror of message");
		return;
	}
	data->TypeMessage = eSystemMessage::msgInputMotion;
	data->inputType = ewol::key::typeMouse;
	data->inputId = _pointerID;
	data->dimention = _pos;
	m_msgSystem.post(data);
}

void ewol::Context::OS_SetMouseState(int _pointerID, bool _isDown, const vec2& _pos ) {
	ewol::eSystemMessage *data = new ewol::eSystemMessage();
	if (data == NULL) {
		EWOL_ERROR("allocationerror of message");
		return;
	}
	data->TypeMessage = eSystemMessage::msgInputState;
	data->inputType = ewol::key::typeMouse;
	data->inputId = _pointerID;
	data->stateIsDown = _isDown;
	data->dimention = _pos;
	m_msgSystem.post(data);
}

void ewol::Context::OS_SetKeyboard(ewol::key::Special& _special,
                                    char32_t _myChar,
                                    bool _isDown,
                                    bool _isARepeateKey) {
	ewol::eSystemMessage *data = new ewol::eSystemMessage();
	if (data == NULL) {
		EWOL_ERROR("allocationerror of message");
		return;
	}
	data->TypeMessage = eSystemMessage::msgKeyboardKey;
	data->stateIsDown = _isDown;
	data->keyboardChar = _myChar;
	data->keyboardSpecial = _special;
	data->repeateKey = _isARepeateKey;
	m_msgSystem.post(data);
}

void ewol::Context::OS_SetKeyboardMove(ewol::key::Special& _special,
                                        enum ewol::key::keyboard _move,
                                        bool _isDown,
                                        bool _isARepeateKey) {
	ewol::eSystemMessage *data = new ewol::eSystemMessage();
	if (data == NULL) {
		EWOL_ERROR("allocationerror of message");
		return;
	}
	data->TypeMessage = eSystemMessage::msgKeyboardMove;
	data->stateIsDown = _isDown;
	data->keyboardMove = _move;
	data->keyboardSpecial = _special;
	data->repeateKey = _isARepeateKey;
	m_msgSystem.post(data);
}

void ewol::Context::OS_Hide(void) {
	ewol::eSystemMessage *data = new ewol::eSystemMessage();
	if (data == NULL) {
		EWOL_ERROR("allocationerror of message");
		return;
	}
	data->TypeMessage = eSystemMessage::msgHide;
	m_msgSystem.post(data);
}

void ewol::Context::OS_Show(void) {
	ewol::eSystemMessage *data = new ewol::eSystemMessage();
	if (data == NULL) {
		EWOL_ERROR("allocationerror of message");
		return;
	}
	data->TypeMessage = eSystemMessage::msgShow;
	m_msgSystem.post(data);
}


void ewol::Context::OS_ClipBoardArrive(enum ewol::context::clipBoard::clipboardListe _clipboardID) {
	ewol::eSystemMessage *data = new ewol::eSystemMessage();
	if (data == NULL) {
		EWOL_ERROR("allocationerror of message");
		return;
	}
	data->TypeMessage = eSystemMessage::msgClipboardArrive;
	data->clipboardID = _clipboardID;
	m_msgSystem.post(data);
}

bool ewol::Context::OS_Draw(bool _displayEveryTime) {
	int64_t currentTime = ewol::getTime();
	// this is to prevent the multiple display at the a high frequency ...
	#if (!defined(__TARGET_OS__Android) && !defined(__TARGET_OS__Windows))
	if(currentTime - m_previousDisplayTime < 1000000/120) {
		usleep(1000);
		return false;
	}
	#endif
	//EWOL_DEBUG("DRAW");
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
		if (m_initStepId < m_initTotalStep) {
			ewol::eSystemMessage *data = new ewol::eSystemMessage();
			if (data == NULL) {
				EWOL_ERROR("allocation error of message");
			} else {
				data->TypeMessage = eSystemMessage::msgInit;
				m_msgSystem.post(data);
			}
		}
		// call all the widget that neded to do something periodicly
		//! ewol::widgetManager::periodicCall(currentTime);
		m_widgetManager.periodicCall(currentTime);
		// remove all widget that they are no more usefull (these who decided to destroy themself)
		//! ewol::ObjectManager::removeAllAutoRemove();
		m_objectManager.removeAllAutoRemove();
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

void ewol::Context::onObjectRemove(ewol::Object * _removeObject) {
	//EWOL_CRITICAL("element removed");
	m_input.onObjectRemove(_removeObject);
}

void ewol::Context::resetIOEvent(void) {
	m_input.newLayerSet();
}

void ewol::Context::OS_OpenGlContextDestroy(void) {
	m_resourceManager.contextHasBeenDestroyed();
}

void ewol::Context::setWindows(ewol::widget::Windows* _windows) {
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

void ewol::Context::forceRedrawAll(void) {
	if (NULL != m_windowsCurrent) {
		m_windowsCurrent->calculateSize(vec2(m_windowsSize.x(), m_windowsSize.y()));
	}
}

void ewol::Context::OS_Stop(void) {
	if (NULL != m_windowsCurrent) {
		m_windowsCurrent->sysOnKill();
	}
}

void ewol::Context::OS_Suspend(void) {
	m_previousDisplayTime = -1;
}

void ewol::Context::OS_Resume(void) {
	m_previousDisplayTime = ewol::getTime();
	m_widgetManager.periodicCallResume(m_previousDisplayTime);
}


void ewol::Context::stop(void) {
	
}

void ewol::Context::setSize(const vec2& _size) {
	EWOL_INFO("setSize: NOT implemented ...");
};

void ewol::Context::setPos(const vec2& _pos) {
	EWOL_INFO("setPos: NOT implemented ...");
}

void ewol::Context::hide(void) {
	EWOL_INFO("hide: NOT implemented ...");
};

void ewol::Context::show(void) {
	EWOL_INFO("show: NOT implemented ...");
}

void ewol::Context::setTitle(const std::string& _title) {
	EWOL_INFO("setTitle: NOT implemented ...");
}

void ewol::Context::keyboardShow(void) {
	EWOL_INFO("keyboardShow: NOT implemented ...");
}


void ewol::Context::keyboardHide(void) {
	EWOL_INFO("keyboardHide: NOT implemented ...");
}


bool ewol::Context::systemKeyboradEvent(enum ewol::key::keyboardSystem _key, bool _down) {
	if (m_windowsCurrent == NULL) {
		return false;
	}
	lockContext();
	bool ret = m_windowsCurrent->onEventHardwareInput(_key, _down);
	unLockContext();
	return ret;
}