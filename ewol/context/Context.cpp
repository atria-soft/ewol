/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <unistd.h>

#include <etk/types.h>
#include <etk/etk.h>

#include <etk/tool.h>
#include <etk/os/FSNode.h>
#include <etk/thread/tools.h>
#include <mutex>

#include <date/date.h>

#include <ewol/ewol.h>
#include <ewol/debug.h>

#include <gale/renderer/openGL/openGL.h>
#include <gale/Dimension.h>

#include <ewol/translate.h>
#include <ewol/object/Object.h>
#include <ewol/object/Manager.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Windows.h>
#include <ewol/widget/Manager.h>

#include <ewol/context/Context.h>

static ewol::Context* l_curentInterface=nullptr;
ewol::Context& ewol::getContext() {
	gale::Context& context = gale::getContext();
	std::shared_ptr<gale::Application> appl = context.getApplication();
	if (appl == nullptr) {
		EWOL_CRITICAL("[CRITICAL] try acces at an empty GALE application (can not get Context)");
		// ???
	}
	return *(std::static_pointer_cast<ewol::Context>(appl));
}


void ewol::Context::setInitImage(const std::string& _fileName) {
	//m_initDisplayImageName = _fileName;
}



void ewol::Context::inputEventTransfertWidget(std::shared_ptr<ewol::Widget> _source,
                                              std::shared_ptr<ewol::Widget> _destination) {
	m_input.transfertEvent(_source, _destination);
}


void ewol::Context::inputEventGrabPointer(std::shared_ptr<ewol::Widget> _widget) {
	m_input.grabPointer(_widget);
}

void ewol::Context::inputEventUnGrabPointer() {
	m_input.unGrabPointer();
}


void ewol::Context::onCreate(gale::Context& _context) {
	EWOL_INFO(" == > Ewol system init (BEGIN)");
	// Add basic ewol translation:
	ewol::translate::addPath("ewol", "DATA:translate/ewol/");
	ewol::translate::autoDetectLanguage();
	// By default we set 2 themes (1 color and 1 shape ...) :
	etk::theme::setNameDefault("GUI", "shape/square/");
	etk::theme::setNameDefault("COLOR", "color/black/");
	// parse for help:
	for(int32_t iii = 0; iii < _context.getCmd().size() ; ++iii) {
		if (    _context.getCmd().get(iii) == "-h"
		     || _context.getCmd().get(iii) == "--help") {
			EWOL_PRINT("ewol - help : ");
			EWOL_PRINT("    " << etk::getApplicationName() << " [options]");
			EWOL_PRINT("        -h/--help:    Display this help");
			EWOL_PRINT("    example:");
			EWOL_PRINT("        " << etk::getApplicationName() << " --help");
			// this is a global help system does not remove it
			continue;
		} else {
			continue;
		}
		_context.getCmd().remove(iii);
		--iii;
	}
	
	EWOL_INFO("EWOL v:" << ewol::getVersion());
	EWOL_INFO("Build Date: " << date::getYear() << "/" << date::getMonth() << "/" << date::getDay() << " " << date::getHour() << "h" << date::getMinute());
	// force a recalculation
	/*
	requestUpdateSize();
	#if defined(__EWOL_ANDROID_ORIENTATION_LANDSCAPE__)
		forceOrientation(ewol::screenLandscape);
	#elif defined(__EWOL_ANDROID_ORIENTATION_PORTRAIT__)
		forceOrientation(ewol::screenPortrait);
	#else
		forceOrientation(ewol::screenAuto);
	#endif
	*/
	EWOL_INFO(" == > Ewol system init (END)");
	std::shared_ptr<ewol::context::Application> appl = m_application;
	if (appl == nullptr) {
		return;
	}
	appl->onCreate(*this);
}

void ewol::Context::onStart(gale::Context& _context) {
	std::shared_ptr<ewol::context::Application> appl = m_application;
	if (appl == nullptr) {
		// TODO : Request exit of the application .... with error ...
		return;
	}
	appl->onStart(*this);
}

void ewol::Context::onResume(gale::Context& _context) {
	std::shared_ptr<ewol::context::Application> appl = m_application;
	if (appl == nullptr) {
		return;
	}
	appl->onResume(*this);
}

void ewol::Context::onRegenerateDisplay(gale::Context& _context) {
	//EWOL_INFO("REGENERATE_DISPLAY");
	// check if the user selected a windows
	std::shared_ptr<ewol::widget::Windows> window = m_windowsCurrent;
	if (window == nullptr) {
		EWOL_INFO("No windows ...");
		return;
	}
	// Redraw all needed elements
	window->onRegenerateDisplay();
	if (m_widgetManager.isDrawingNeeded() == true) {
		markDrawingIsNeeded();
	}
	//markDrawingIsNeeded();
}

void ewol::Context::onDraw(gale::Context& _context) {
	//EWOL_INFO("DRAW");
	// clean internal data...
	m_objectManager.cleanInternalRemoved();
	// real draw...
	std::shared_ptr<ewol::widget::Windows> window = m_windowsCurrent;
	if (window == nullptr) {
		return;
	}
	window->sysDraw();
}

void ewol::Context::onPause(gale::Context& _context) {
	std::shared_ptr<ewol::context::Application> appl = m_application;
	if (appl == nullptr) {
		return;
	}
	appl->onPause(*this);
}

void ewol::Context::onStop(gale::Context& _context) {
	std::shared_ptr<ewol::context::Application> appl = m_application;
	if (appl == nullptr) {
		return;
	}
	appl->onStop(*this);
}

void ewol::Context::onDestroy(gale::Context& _context) {
	EWOL_INFO(" == > Ewol system Un-Init (BEGIN)");
	// Remove current windows
	m_windowsCurrent.reset();
	// clean all widget and sub widget with their resources:
	m_objectManager.cleanInternalRemoved();
	std::shared_ptr<ewol::context::Application> appl = m_application;
	if (appl != nullptr) {
		// call application to uninit
		appl->onDestroy(*this);
		m_application.reset();
	}
	// internal clean elements
	m_objectManager.cleanInternalRemoved();
	EWOL_INFO("List of all widget of this context must be equal at 0 ==> otherwise some remove is missing");
	m_objectManager.displayListObject();
	// now All must be removed !!!
	m_objectManager.unInit();
	EWOL_INFO(" == > Ewol system Un-Init (END)");
}

void ewol::Context::onPointer(enum gale::key::type _type,
                              int32_t _pointerID,
                              const vec2& _pos,
                              gale::key::status _state) {
	switch (_state) {
		case gale::key::status_move:
			//EWOL_DEBUG("Receive MSG : THREAD_INPUT_MOTION");
			m_input.motion(_type, _pointerID, _pos);
			break;
		case gale::key::status_down:
		case gale::key::status_downRepeate:
			//EWOL_DEBUG("Receive MSG : THREAD_INPUT_STATE");
			m_input.state(_type, _pointerID, true, _pos);
			break;
		case gale::key::status_up:
			//EWOL_DEBUG("Receive MSG : THREAD_INPUT_STATE");
			m_input.state(_type, _pointerID, false, _pos);
			break;
		default:
			EWOL_DEBUG("Unknow state : " << _state);
			break;
	}
}
void ewol::Context::onKeyboard(gale::key::Special& _special,
                               enum gale::key::keyboard _type,
                               char32_t _value,
                               gale::key::status _state) {
	EWOL_VERBOSE("event {" << _special << "} " << _type << " " << _value << " " << _state);
	// store the keyboard special key status for mouse event...
	m_input.setLastKeyboardSpecial(_special);
	if (m_windowsCurrent == nullptr) {
		// No windows ...
		return;
	}
	bool repeate = (_state == gale::key::status_downRepeate);
	bool isDown =    (_state == gale::key::status_downRepeate)
	              || (_state == gale::key::status_down);
	if (m_windowsCurrent->onEventShortCut(_special,
	                                      _value,
	                                      _type,
	                                      isDown) == true) {
		// Keep a shortcut ...
		return;
	}
	// get the current focused Widget :
	std::shared_ptr<ewol::Widget> tmpWidget = m_widgetManager.focusGet();
	if (tmpWidget == nullptr) {
		// no Widget ...
		return;
	}
	// check if the widget allow repeating key events.
	//EWOL_INFO("repeating test :" << repeate << " widget=" << tmpWidget->getKeyboardRepeate() << " state=" << isDown);
	if(    repeate == false
	    || (    repeate == true
	         && tmpWidget->getKeyboardRepeate() == true) ) {
		// check Widget shortcut
		if (tmpWidget->onEventShortCut(_special,
		                               _value,
		                               _type,
		                               isDown) == false) {
			// generate the direct event ...
			if (_type == gale::key::keyboard_char) {
				ewol::event::EntrySystem tmpEntryEvent(gale::key::keyboard_char,
				                                       gale::key::status_up,
				                                       _special,
				                                       _value);
				if(isDown == true) {
					tmpEntryEvent.m_event.setStatus(gale::key::status_down);
				}
				tmpWidget->systemEventEntry(tmpEntryEvent);
			} else { // THREAD_KEYBORAD_MOVE
				ewol::event::EntrySystem tmpEntryEvent(_type,
				                                       gale::key::status_up,
				                                       _special,
				                                       0);
				if(isDown == true) {
					tmpEntryEvent.m_event.setStatus(gale::key::status_down);
				}
				tmpWidget->systemEventEntry(tmpEntryEvent);
			}
		} else {
			EWOL_DEBUG("remove Repeate key ...");
		}
	}
}

/*
void ewol::Context::processEvents() {
			case eSystemMessage::msgResize:
				//EWOL_DEBUG("Receive MSG : THREAD_RESIZE");
				m_windowsSize = data->dimention;
				ewol::Dimension::setPixelWindowsSize(m_windowsSize);
				forceRedrawAll();
				break;
*/

void ewol::Context::onClipboardEvent(enum gale::context::clipBoard::clipboardListe _clipboardId) {
	std::shared_ptr<ewol::Widget> tmpWidget = m_widgetManager.focusGet();
	if (tmpWidget != nullptr) {
		tmpWidget->onEventClipboard(_clipboardId);
	}
}


ewol::Context::Context(ewol::context::Application* _application) :
  //m_application(std::make_shared<ewol::context::Application>(_application)),
  m_application(_application),
  m_objectManager(*this),
  m_input(*this),
  m_windowsCurrent(nullptr),
  m_initStepId(0) {
	if (m_application == nullptr) {
		EWOL_CRITICAL("Can not start context with no Application ==> rtfm ...");
	}
}

ewol::Context::~Context() {
	// nothing to do ...
}

void ewol::Context::requestUpdateSize() {
	gale::Context& context = gale::getContext();
	context.requestUpdateSize();
}

void ewol::Context::onPeriod(int64_t _time) {
	m_objectManager.timeCall(_time);
}

#if 0
bool ewol::Context::OS_Draw(bool _displayEveryTime) {
	int64_t currentTime = ewol::getTime();
	// this is to prevent the multiple display at the a high frequency ...
	m_previousDisplayTime = currentTime;
	
	// process the events
	if (m_displayFps == true) {
		m_FpsSystemEvent.tic();
	}
	bool needRedraw = false;
	//! Event management section ...
	{
		// set the curent interface :
		processEvents();
		if (m_initStepId < m_application->getNbStepInit()) {
			ewol::eSystemMessage *data = new ewol::eSystemMessage();
			if (data == nullptr) {
				EWOL_ERROR("allocation error of message");
			} else {
				data->TypeMessage = eSystemMessage::msgInit;
				m_msgSystem.post(data);
			}
		}
		// call all the widget that neded to do something periodicly
		m_objectManager.timeCall(currentTime);
		
		if (m_displayFps == true) {
			m_FpsSystemEvent.incrementCounter();
			m_FpsSystemEvent.toc();
		}
		//! bool needRedraw = ewol::widgetManager::isDrawingNeeded();
		needRedraw = m_widgetManager.isDrawingNeeded();
		// release the curent interface :
	}
	bool hasDisplayDone = false;
	//! drawing section :
	{
		// Lock openGl context:
		gale::openGL::lock();
		if (m_displayFps == true) {
			m_FpsSystemContext.tic();
		}
		if (nullptr != m_windowsCurrent) {
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
		if (nullptr != m_windowsCurrent) {
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
		}
		if (hasDisplayDone == true) {
			if (m_displayFps == true) {
				m_FpsFlush.incrementCounter();
			}
			gale::openGL::flush();
		}
		if (m_displayFps == true) {
			m_FpsFlush.toc();
		}
		// release open GL Context
		gale::openGL::unLock();
	}
	{
		// set the curent interface :
		// release open GL Context
		gale::openGL::lock();
		// while The Gui is drawing in OpenGl, we do some not realTime things
		m_resourceManager.updateContext();
		// release open GL Context
		gale::openGL::unLock();
		m_objectManager.cleanInternalRemoved();
		m_resourceManager.cleanInternalRemoved();
		// release the curent interface :
	}
	return hasDisplayDone;
}
#endif

void ewol::Context::resetIOEvent() {
	m_input.newLayerSet();
}


void ewol::Context::setWindows(const std::shared_ptr<ewol::widget::Windows>& _windows) {
	// remove current focus :
	m_widgetManager.focusSetDefault(nullptr);
	m_widgetManager.focusRelease();
	// set the new pointer as windows system
	m_windowsCurrent = _windows;
	// set the new default focus :
	m_widgetManager.focusSetDefault(_windows);
	// request all the widget redrawing
	forceRedrawAll();
}

std::shared_ptr<ewol::widget::Windows> ewol::Context::getWindows() {
	return m_windowsCurrent;
};
void ewol::Context::onResize(const ivec2& _size) {
	EWOL_VERBOSE("Resize: " << _size);
	forceRedrawAll();
}

void ewol::Context::forceRedrawAll() {
	if (m_windowsCurrent == nullptr) {
		return;
	}
	ivec2 size = getSize();
	m_windowsCurrent->calculateSize(vec2(size.x(), size.y()));
}
/*
void ewol::Context::OS_Stop() {
	// set the curent interface :
	EWOL_INFO("OS_Stop...");
	if (m_windowsCurrent != nullptr) {
		m_windowsCurrent->sysOnKill();
	}
	// release the curent interface :
}

void ewol::Context::OS_Suspend() {
	// set the curent interface :
	EWOL_INFO("OS_Suspend...");
	m_previousDisplayTime = -1;
	if (m_windowsCurrent != nullptr) {
		m_windowsCurrent->onStateSuspend();
	}
	// release the curent interface :
}

void ewol::Context::OS_Resume() {
	// set the curent interface :
	EWOL_INFO("OS_Resume...");
	m_previousDisplayTime = ewol::getTime();
	m_objectManager.timeCallResume(m_previousDisplayTime);
	if (m_windowsCurrent != nullptr) {
		m_windowsCurrent->onStateResume();
	}
	// release the curent interface :
}
void ewol::Context::OS_Foreground() {
	// set the curent interface :
	EWOL_INFO("OS_Foreground...");
	if (m_windowsCurrent != nullptr) {
		m_windowsCurrent->onStateForeground();
	}
	// release the curent interface :
}

void ewol::Context::OS_Background() {
	// set the curent interface :
	EWOL_INFO("OS_Background...");
	if (m_windowsCurrent != nullptr) {
		m_windowsCurrent->onStateBackground();
	}
	// release the curent interface :
}
*/

void ewol::Context::stop() {
	
}

