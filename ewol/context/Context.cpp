/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */



#include <etk/types.hpp>
#include <etk/etk.hpp>

#include <etk/tool.hpp>
#include <etk/os/FSNode.hpp>
#include <ethread/tools.hpp>
#include <ethread/Mutex.hpp>

#include <ewol/ewol.hpp>
#include <ewol/debug.hpp>

#include <gale/renderer/openGL/openGL.hpp>
#include <gale/Dimension.hpp>

#include <etranslate/etranslate.hpp>
#include <ewol/object/Object.hpp>
#include <ewol/object/Manager.hpp>
#include <ewol/widget/Widget.hpp>
#include <ewol/widget/Windows.hpp>
#include <ewol/widget/Manager.hpp>

#include <ewol/context/Context.hpp>

static ewol::Context* l_curentInterface=nullptr;
ewol::Context& ewol::getContext() {
	gale::Context& context = gale::getContext();
	ememory::SharedPtr<gale::Application> appl = context.getApplication();
	if (appl == nullptr) {
		EWOL_CRITICAL("[CRITICAL] try acces at an empty GALE application (can not get Context)");
		// ???
	}
	return *(ememory::staticPointerCast<ewol::Context>(appl));
}


void ewol::Context::setInitImage(const etk::String& _fileName) {
	//m_initDisplayImageName = _fileName;
}



void ewol::Context::inputEventTransfertWidget(ewol::WidgetShared _source,
                                              ewol::WidgetShared _destination) {
	m_input.transfertEvent(_source, _destination);
}


void ewol::Context::inputEventGrabPointer(ewol::WidgetShared _widget) {
	m_input.grabPointer(_widget);
}

void ewol::Context::inputEventUnGrabPointer() {
	m_input.unGrabPointer();
}


void ewol::Context::onCreate(gale::Context& _context) {
	EWOL_INFO(" == > Ewol system create (BEGIN)");
	// Add basic ewol translation:
	etranslate::addPath("ewol", "{ewol}DATA:translate/ewol/");
	etranslate::autoDetectLanguage();
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
	ememory::SharedPtr<ewol::context::Application> appl = m_application;
	if (appl == nullptr) {
		EWOL_ERROR(" == > Create without application");
		return;
	}
	appl->onCreate(*this);
	EWOL_INFO(" == > Ewol system create (END)");
}

void ewol::Context::onStart(gale::Context& _context) {
	EWOL_INFO(" == > Ewol system start (BEGIN)");
	ememory::SharedPtr<ewol::context::Application> appl = m_application;
	if (appl == nullptr) {
		// TODO : Request exit of the application .... with error ...
		return;
	}
	appl->onStart(*this);
	EWOL_INFO(" == > Ewol system start (END)");
}

void ewol::Context::onResume(gale::Context& _context) {
	EWOL_INFO(" == > Ewol system resume (BEGIN)");
	ememory::SharedPtr<ewol::context::Application> appl = m_application;
	if (appl == nullptr) {
		return;
	}
	appl->onResume(*this);
	EWOL_INFO(" == > Ewol system resume (END)");
}

void ewol::Context::onRegenerateDisplay(gale::Context& _context) {
	//EWOL_INFO("REGENERATE_DISPLAY");
	// check if the user selected a windows
	ewol::widget::WindowsShared window = m_windowsCurrent;
	if (window == nullptr) {
		EWOL_DEBUG("No windows ...");
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
	ewol::widget::WindowsShared window = m_windowsCurrent;
	if (window == nullptr) {
		return;
	}
	window->sysDraw();
}

void ewol::Context::onPause(gale::Context& _context) {
	EWOL_INFO(" == > Ewol system pause (BEGIN)");
	ememory::SharedPtr<ewol::context::Application> appl = m_application;
	if (appl == nullptr) {
		return;
	}
	appl->onPause(*this);
	EWOL_INFO(" == > Ewol system pause (END)");
}

void ewol::Context::onStop(gale::Context& _context) {
	EWOL_INFO(" == > Ewol system stop (BEGIN)");
	ememory::SharedPtr<ewol::context::Application> appl = m_application;
	if (appl == nullptr) {
		return;
	}
	appl->onStop(*this);
	EWOL_INFO(" == > Ewol system stop (END)");
}

void ewol::Context::onDestroy(gale::Context& _context) {
	EWOL_INFO(" == > Ewol system destroy (BEGIN)");
	// Remove current windows
	m_windowsCurrent.reset();
	// clean all widget and sub widget with their resources:
	m_objectManager.cleanInternalRemoved();
	ememory::SharedPtr<ewol::context::Application> appl = m_application;
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
	EWOL_INFO(" == > Ewol system destroy (END)");
}

void ewol::Context::onKillDemand(gale::Context& _context) {
	EWOL_INFO(" == > User demand a destroy (BEGIN)");
	ememory::SharedPtr<ewol::context::Application> appl = m_application;
	if (appl == nullptr) {
		exit(0);
		return;
	}
	appl->onKillDemand(*this);
	EWOL_INFO(" == > User demand a destroy (END)");
}

void ewol::Context::onPointer(enum gale::key::type _type,
                              int32_t _pointerID,
                              const vec2& _pos,
                              gale::key::status _state) {
	switch (_state) {
		case gale::key::status::move:
			//EWOL_DEBUG("Receive MSG : THREAD_INPUT_MOTION");
			m_input.motion(_type, _pointerID, _pos);
			break;
		case gale::key::status::down:
		case gale::key::status::downRepeate:
			//EWOL_DEBUG("Receive MSG : THREAD_INPUT_STATE");
			m_input.state(_type, _pointerID, true, _pos);
			break;
		case gale::key::status::up:
			//EWOL_DEBUG("Receive MSG : THREAD_INPUT_STATE");
			m_input.state(_type, _pointerID, false, _pos);
			break;
		default:
			EWOL_DEBUG("Unknow state : " << _state);
			break;
	}
}
void ewol::Context::onKeyboard(const gale::key::Special& _special,
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
	bool repeate = (_state == gale::key::status::downRepeate);
	bool isDown =    (_state == gale::key::status::downRepeate)
	              || (_state == gale::key::status::down);
	if (m_windowsCurrent->onEventShortCut(_special,
	                                      _value,
	                                      _type,
	                                      isDown) == true) {
		// Keep a shortcut ...
		return;
	}
	// get the current focused Widget :
	ewol::WidgetShared tmpWidget = m_widgetManager.focusGet();
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
			if (_type == gale::key::keyboard::character) {
				ewol::event::EntrySystem tmpEntryEvent(gale::key::keyboard::character,
				                                       gale::key::status::up,
				                                       _special,
				                                       _value);
				if(isDown == true) {
					tmpEntryEvent.m_event.setStatus(gale::key::status::down);
				}
				tmpWidget->systemEventEntry(tmpEntryEvent);
			} else { // THREAD_KEYBORAD_MOVE
				ewol::event::EntrySystem tmpEntryEvent(_type,
				                                       gale::key::status::up,
				                                       _special,
				                                       0);
				if(isDown == true) {
					tmpEntryEvent.m_event.setStatus(gale::key::status::down);
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
	ewol::WidgetShared tmpWidget = m_widgetManager.focusGet();
	if (tmpWidget != nullptr) {
		tmpWidget->onEventClipboard(_clipboardId);
	}
}


ewol::Context::Context(ewol::context::Application* _application) :
  //m_application(ememory::makeShared<ewol::context::Application>(_application)),
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

void ewol::Context::onPeriod(const echrono::Clock& _time) {
	m_objectManager.timeCall(_time);
}

void ewol::Context::resetIOEvent() {
	m_input.newLayerSet();
}

void ewol::Context::setWindows(const ewol::widget::WindowsShared& _windows) {
	EWOL_INFO("set New windows");
	// remove current focus :
	m_widgetManager.focusSetDefault(nullptr);
	m_widgetManager.focusRelease();
	// set the new pointer as windows system
	m_windowsCurrent = _windows;
	// set the new default focus:
	m_widgetManager.focusSetDefault(_windows);
	// display the title of the Windows:
	if (m_windowsCurrent != nullptr) {
		setTitle(m_windowsCurrent->propertyTitle.get());
	}
	// request all the widget redrawing
	forceRedrawAll();
}

ewol::widget::WindowsShared ewol::Context::getWindows() {
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
	m_windowsCurrent->setSize(vec2(size.x(), size.y()));
	m_windowsCurrent->onChangeSize();
}

