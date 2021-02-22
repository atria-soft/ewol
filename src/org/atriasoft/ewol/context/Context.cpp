/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */



#include <etk/types.hpp>
#include <etk/etk.hpp>
#include <etk/tool.hpp>
#include <etk/theme/theme.hpp>

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

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::Context);

static ewol::Context* l_curentInterface=null;
ewol::Context& ewol::getContext() {
	gale::Context& context = gale::getContext();
	ememory::SharedPtr<gale::Application> appl = context.getApplication();
	if (appl == null) {
		Log.critical("[CRITICAL] try acces at an empty GALE application (can not get Context)");
		// ???
	}
	return *(ememory::staticPointerCast<ewol::Context>(appl));
}


void ewol::Context::setInitImage(const etk::Uri& _fileName) {
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
	Log.info(" == > Ewol system create (BEGIN)");
	// Add basic ewol translation:
	etranslate::addPath("ewol", "DATA:///translate/ewol/?lib=ewol");
	etranslate::autoDetectLanguage();
	// By default we set 2 themes (1 color and 1 shape ...) :
	etk::theme::setNameDefault("GUI", "shape/square/");
	etk::theme::setNameDefault("COLOR", "color/black/");
	// parse for help:
	for(int32_t iii = 0; iii < _context.getCmd().size() ; ++iii) {
		if (    _context.getCmd().get(iii) == "-h"
		     || _context.getCmd().get(iii) == "--help") {
			Log.print("ewol - help : ");
			Log.print("    " << etk::getApplicationName() << " [options]");
			Log.print("        -h/--help:    Display this help");
			Log.print("    example:");
			Log.print("        " << etk::getApplicationName() << " --help");
			// this is a global help system does not remove it
			continue;
		} else {
			continue;
		}
		_context.getCmd().remove(iii);
		--iii;
	}
	
	Log.info("EWOL v:" << ewol::getVersion());
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
	if (appl == null) {
		Log.error(" == > Create without application");
		return;
	}
	appl->onCreate(*this);
	Log.info(" == > Ewol system create (END)");
}

void ewol::Context::onStart(gale::Context& _context) {
	Log.info(" == > Ewol system start (BEGIN)");
	ememory::SharedPtr<ewol::context::Application> appl = m_application;
	if (appl == null) {
		// TODO : Request exit of the application .... with error ...
		return;
	}
	appl->onStart(*this);
	Log.info(" == > Ewol system start (END)");
}

void ewol::Context::onResume(gale::Context& _context) {
	Log.info(" == > Ewol system resume (BEGIN)");
	ememory::SharedPtr<ewol::context::Application> appl = m_application;
	if (appl == null) {
		return;
	}
	appl->onResume(*this);
	Log.info(" == > Ewol system resume (END)");
}

void ewol::Context::onRegenerateDisplay(gale::Context& _context) {
	//Log.info("REGENERATE_DISPLAY");
	// check if the user selected a windows
	ewol::widget::WindowsShared window = m_windowsCurrent;
	if (window == null) {
		Log.debug("No windows ...");
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
	//Log.info("DRAW");
	// clean internal data...
	m_objectManager.cleanInternalRemoved();
	// real draw...
	ewol::widget::WindowsShared window = m_windowsCurrent;
	if (window == null) {
		return;
	}
	window->sysDraw();
}

void ewol::Context::onPause(gale::Context& _context) {
	Log.info(" == > Ewol system pause (BEGIN)");
	ememory::SharedPtr<ewol::context::Application> appl = m_application;
	if (appl == null) {
		return;
	}
	appl->onPause(*this);
	Log.info(" == > Ewol system pause (END)");
}

void ewol::Context::onStop(gale::Context& _context) {
	Log.info(" == > Ewol system stop (BEGIN)");
	ememory::SharedPtr<ewol::context::Application> appl = m_application;
	if (appl == null) {
		return;
	}
	appl->onStop(*this);
	Log.info(" == > Ewol system stop (END)");
}

void ewol::Context::onDestroy(gale::Context& _context) {
	Log.info(" == > Ewol system destroy (BEGIN)");
	// Remove current windows
	m_windowsCurrent.reset();
	// clean all widget and sub widget with their resources:
	m_objectManager.cleanInternalRemoved();
	ememory::SharedPtr<ewol::context::Application> appl = m_application;
	if (appl != null) {
		// call application to uninit
		appl->onDestroy(*this);
		m_application.reset();
	}
	// internal clean elements
	m_objectManager.cleanInternalRemoved();
	Log.info("List of all widget of this context must be equal at 0 ==> otherwise some remove is missing");
	m_objectManager.displayListObject();
	// now All must be removed !!!
	m_objectManager.unInit();
	Log.info(" == > Ewol system destroy (END)");
}

void ewol::Context::onKillDemand(gale::Context& _context) {
	Log.info(" == > User demand a destroy (BEGIN)");
	ememory::SharedPtr<ewol::context::Application> appl = m_application;
	if (appl == null) {
		exit(0);
		return;
	}
	appl->onKillDemand(*this);
	Log.info(" == > User demand a destroy (END)");
}

void ewol::Context::onPointer(enum gale::key::type _type,
                              int32_t _pointerID,
                              const Vector2f& _pos,
                              gale::key::status _state) {
	switch (_state) {
		case gale::key::status::move:
			//Log.debug("Receive MSG : THREAD_INPUT_MOTION");
			m_input.motion(_type, _pointerID, _pos);
			break;
		case gale::key::status::down:
		case gale::key::status::downRepeate:
			//Log.debug("Receive MSG : THREAD_INPUT_STATE");
			m_input.state(_type, _pointerID, true, _pos);
			break;
		case gale::key::status::up:
			//Log.debug("Receive MSG : THREAD_INPUT_STATE");
			m_input.state(_type, _pointerID, false, _pos);
			break;
		default:
			Log.debug("Unknow state : " << _state);
			break;
	}
}
void ewol::Context::onKeyboard(const gale::key::Special& _special,
                               enum gale::key::keyboard _type,
                               char32_t _value,
                               gale::key::status _state) {
	Log.verbose("event {" << _special << "} " << _type << " " << _value << " " << _state);
	// store the keyboard special key status for mouse event...
	m_input.setLastKeyboardSpecial(_special);
	if (m_windowsCurrent == null) {
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
	if (tmpWidget == null) {
		// no Widget ...
		return;
	}
	// check if the widget allow repeating key events.
	//Log.info("repeating test :" << repeate << " widget=" << tmpWidget->getKeyboardRepeate() << " state=" << isDown);
	if(    repeate == false
	    || (    repeate == true
	         && tmpWidget->getKeyboardRepeat() == true) ) {
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
			Log.debug("remove Repeate key ...");
		}
	}
}

/*
void ewol::Context::processEvents() {
			case eSystemMessage::msgResize:
				//Log.debug("Receive MSG : THREAD_RESIZE");
				m_windowsSize = data->dimention;
				ewol::Dimension::setPixelWindowsSize(m_windowsSize);
				forceRedrawAll();
				break;
*/

void ewol::Context::onClipboardEvent(enum gale::context::clipBoard::clipboardListe _clipboardId) {
	ewol::WidgetShared tmpWidget = m_widgetManager.focusGet();
	if (tmpWidget != null) {
		tmpWidget->onEventClipboard(_clipboardId);
	}
}


ewol::Context::Context(ewol::context::Application* _application) :
  //m_application(ememory::makeShared<ewol::context::Application>(_application)),
  m_application(_application),
  m_objectManager(*this),
  m_input(*this),
  m_windowsCurrent(null),
  m_initStepId(0) {
	if (m_application == null) {
		Log.critical("Can not start context with no Application ==> rtfm ...");
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
	Log.info("set New windows");
	// remove current focus :
	m_widgetManager.focusSetDefault(null);
	m_widgetManager.focusRelease();
	// set the new pointer as windows system
	m_windowsCurrent = _windows;
	// set the new default focus:
	m_widgetManager.focusSetDefault(_windows);
	// display the title of the Windows:
	if (m_windowsCurrent != null) {
		setTitle(m_windowsCurrent->propertyTitle.get());
	}
	// request all the widget redrawing
	forceRedrawAll();
}

ewol::widget::WindowsShared ewol::Context::getWindows() {
	return m_windowsCurrent;
};
void ewol::Context::onResize(const Vector2i& _size) {
	Log.verbose("Resize: " << _size);
	forceRedrawAll();
}

void ewol::Context::forceRedrawAll() {
	if (m_windowsCurrent == null) {
		return;
	}
	Vector2i size = getSize();
	m_windowsCurrent->setSize(Vector2f(size.x(), size.y()));
	m_windowsCurrent->onChangeSize();
}

