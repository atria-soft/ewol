/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.context;

import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.object.ObjectManager;
import org.atriasoft.ewol.widget.WidgetManager;
import org.atriasoft.gale.Application;
import org.atriasoft.gale.Gale;
import org.atriasoft.gale.context.ClipboardList;
import org.atriasoft.gale.context.CommandLine;
import org.atriasoft.gale.key.KeyKeyboard;
import org.atriasoft.gale.key.KeySpecial;
import org.atriasoft.gale.key.KeyStatus;
import org.atriasoft.gale.key.KeyType;
import org.atriasoft.gale.resource.ResourceManager;

// Here we hereted from the gale application to be agnostic of the OW where we work ...
public abstract class EwolContext extends Application {
	private static EwolContext curentInterface = null;
	
	/**
	 * @brief From everyware in the program, we can get the context inteface.
	 * @return current reference on the instance.
	 */
	static EwolContext getContext() {
		return curentInterface;
	}
	
	private final EwolApplication application; //!< Application handle
	
	public EwolApplication getApplication() {
		return this.application;
	}
	
	public CommandLine getCmd() {
		return Gale.getContext().getCmd();
	}
	
	private ConfigFont configFont; //!< global font configuration
	
	public ConfigFont getFontDefault() {
		return this.configFont;
	}
	
	private final ObjectManager objectManager; //!< Object Manager main instance
	
	public ObjectManager getEObjectManager() {
		return this.objectManager;
	}
	
	private WidgetManager widgetManager; //!< global widget manager
	
	public WidgetManager getWidgetManager() {
		return this.widgetManager;
	}
	
	public ResourceManager getResourcesManager() {
		return Gale.getContext().getResourcesManager();
	}
	
	public EwolContext(final EwolApplication _application) {
		this.application = _application;
		this.objectManager = new ObjectManager(this);
		this.input = new InputManager(this);
		if (this.application == null) {
			Log.critical("Can not start context with no Application ==> rtfm ...");
		}
	}
	
	private final InputManager input;
	
	public void onCreate(final Context _context) {
		Log.info(" == > Ewol system create (BEGIN)");
		// Add basic ewol translation:
		etranslate::addPath("ewol", "DATA:///translate/ewol/?lib=ewol");
		etranslate::autoDetectLanguage();
		// By default we set 2 themes (1 color and 1 shape ...) :
		etk::theme::setNameDefault("GUI", "shape/square/");
		etk::theme::setNameDefault("COLOR", "color/black/");
		// parse for help:
		for(int iii = 0; iii < _context.getCmd().size() ; ++iii) {
			if (    _context.getCmd().get(iii) == "-h"
			     || _context.getCmd().get(iii) == "--help") {
				Log.print("ewol - help : ");
				Log.print("    " + etk::getApplicationName() + " [options]");
				Log.print("        -h/--help:    Display this help");
				Log.print("    example:");
				Log.print("        " + etk::getApplicationName() + " --help");
				// this is a global help system does not remove it
				continue;
			} else {
				continue;
			}
			_context.getCmd().remove(iii);
			--iii;
		}
		
		Log.info("EWOL v:" + ewol::getVersion());
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
		EwolApplication appl = this.application;
		if (appl == null) {
			Log.error(" == > Create without application");
			return;
		}
		appl.onCreate(*this);
		Log.info(" == > Ewol system create (END)");
	}
	
	public abstract void onStart(final Context _context) {
		Log.info(" == > Ewol system start (BEGIN)");
		EwolApplication appl = this.application;
		if (appl == null) {
			// TODO : Request exit of the application .... with error ...
			return;
		}
		appl.onStart(*this);
		Log.info(" == > Ewol system start (END)");
	}
	
	public abstract void onResume(final Context _context){
		Log.info(" == > Ewol system resume (BEGIN)");
		EwolApplication appl = this.application;
		if (appl == null) {
			return;
		}
		appl.onResume(*this);
		Log.info(" == > Ewol system resume (END)");
	}
	
	public abstract void onRegenerateDisplay(final Context _context) {
		//Log.info("REGENERATE_DISPLAY");
		// check if the user selected a windows
		ewol::widget::WindowsShared window = this.windowsCurrent;
		if (window == null) {
			Log.debug("No windows ...");
			return;
		}
		// Redraw all needed elements
		window.onRegenerateDisplay();
		if (this.widgetManager.isDrawingNeeded() == true) {
			markDrawingIsNeeded();
		}
		//markDrawingIsNeeded();
	}
	
	public abstract void onDraw(final Context _context) {
		//Log.info("DRAW");
		// clean internal data...
		this.objectManager.cleanInternalRemoved();
		// real draw...
		ewol::widget::WindowsShared window = this.windowsCurrent;
		if (window == null) {
			return;
		}
		window.sysDraw();
	}
	
	public abstract void onPause(final Context _context){
		Log.info(" == > Ewol system pause (BEGIN)");
		EwolApplication appl = this.application;
		if (appl == null) {
			return;
		}
		appl.onPause(*this);
		Log.info(" == > Ewol system pause (END)");
	}
	
	public abstract void onStop(final Context _context){
		Log.info(" == > Ewol system stop (BEGIN)");
		EwolApplication appl = this.application;
		if (appl == null) {
			return;
		}
		appl.onStop(*this);
		Log.info(" == > Ewol system stop (END)");
	}
	
	public abstract void onDestroy(final Context _context){
		Log.info(" == > Ewol system destroy (BEGIN)");
		// Remove current windows
		this.windowsCurrent.reset();
		// clean all widget and sub widget with their resources:
		this.objectManager.cleanInternalRemoved();
		EwolApplication appl = this.application;
		if (appl != null) {
			// call application to uninit
			appl.onDestroy(*this);
			this.application.reset();
		}
		// internal clean elements
		this.objectManager.cleanInternalRemoved();
		Log.info("List of all widget of this context must be equal at 0 ==> otherwise some remove is missing");
		this.objectManager.displayListObject();
		// now All must be removed !!!
		this.objectManager.unInit();
		Log.info(" == > Ewol system destroy (END)");
	}
	
	public abstract void onKillDemand(final Context _context){
		Log.info(" == > User demand a destroy (BEGIN)");
		EwolApplication appl = this.application;
		if (appl == null) {
			exit(0);
			return;
		}
		appl.onKillDemand(*this);
		Log.info(" == > User demand a destroy (END)");
	}
	
	public abstract void onPointer(final KeyType _type, final int _pointerID, final Vector2f _pos, final KeyStatus _state) {
		switch (_state) {
			case KeyStatus::move:
				//Log.debug("Receive MSG : THREAD_INPUT_MOTION");
				this.input.motion(_type, _pointerID, _pos);
				break;
			case KeyStatus::down:
			case KeyStatus::downRepeate:
				//Log.debug("Receive MSG : THREAD_INPUT_STATE");
				this.input.state(_type, _pointerID, true, _pos);
				break;
			case KeyStatus::up:
				//Log.debug("Receive MSG : THREAD_INPUT_STATE");
				this.input.state(_type, _pointerID, false, _pos);
				break;
			default:
				Log.debug("Unknow state : " + _state);
				break;
		}
	}
	
	@Override
	public abstract void onKeyboard(final KeySpecial _special, final KeyKeyboard _type, final Character _value, final KeyStatus _state) {
		Log.verbose("event {" + _special + "} " + _type + " " + _value + " " + _state);
		// store the keyboard special key status for mouse event...
		this.input.setLastKeyboardSpecial(_special);
		if (this.windowsCurrent == null) {
			// No windows ...
			return;
		}
		boolean repeate = (_state == KeyStatus::downRepeate);
		boolean isDown =    (_state == KeyStatus::downRepeate)
		              || (_state == KeyStatus::down);
		if (this.windowsCurrent.onEventShortCut(_special,
		                                      _value,
		                                      _type,
		                                      isDown) == true) {
			// Keep a shortcut ...
			return;
		}
		// get the current focused Widget :
		Widget tmpWidget = this.widgetManager.focusGet();
		if (tmpWidget == null) {
			// no Widget ...
			return;
		}
		// check if the widget allow repeating key events.
		//Log.info("repeating test :" + repeate + " widget=" + tmpWidget.getKeyboardRepeate() + " state=" + isDown);
		if(    repeate == false
		    || (    repeate == true
		         LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM tmpWidget.getKeyboardRepeat() == true) ) {
			// check Widget shortcut
			if (tmpWidget.onEventShortCut(_special,
			                               _value,
			                               _type,
			                               isDown) == false) {
				// generate the direct event ...
				if (_type == KeyKeyboard::character) {
					ewol::event::EntrySystem tmpEntryEvent(KeyKeyboard::character,
					                                       KeyStatus::up,
					                                       _special,
					                                       _value);
					if(isDown == true) {
						tmpEntryEvent.this.event.setStatus(KeyStatus::down);
					}
					tmpWidget.systemEventEntry(tmpEntryEvent);
				} else { // THREAD_KEYBORAD_MOVE
					ewol::event::EntrySystem tmpEntryEvent(_type,
					                                       KeyStatus::up,
					                                       _special,
					                                       0);
					if(isDown == true) {
						tmpEntryEvent.this.event.setStatus(KeyStatus::down);
					}
					tmpWidget.systemEventEntry(tmpEntryEvent);
				}
			} else {
				Log.debug("remove Repeate key ...");
			}
		}
	}
	
	@Override
	public void onClipboardEvent(final ClipboardList _clipboardId) {
		final Widget tmpWidget = this.widgetManager.focusGet();
		if (tmpWidget != null) {
			tmpWidget.onEventClipboard(_clipboardId);
		}
	}
	
	/**
	 * @brief reset event management for the IO like Input ou Mouse or keyborad
	 */
	public void resetIOEvent() {
		this.input.newLayerSet();
	}
	
	private final Windows windowsCurrent = null; //!< current displayed windows
	
	/**
	 * @brief set the current windows to display :
	 * @param _windows Windows that might be displayed
	 */
	public void setWindows(final Windows _windows) {
		Log.info("set New windows");
		// remove current focus :
		this.widgetManager.focusSetDefault(null);
		this.widgetManager.focusRelease();
		// set the new pointer as windows system
		this.windowsCurrent = _windows;
		// set the new default focus:
		this.widgetManager.focusSetDefault(_windows);
		// display the title of the Windows:
		if (this.windowsCurrent != null) {
			setTitle(this.windowsCurrent.propertyTitle.get());
		}
		// request all the widget redrawing
		forceRedrawAll();
	}
	
	/**
	 * @brief get the current windows that is displayed
	 * @return the current handle on the windows (can be null)
	 */
	public Windows getWindows() {
		return this.windowsCurrent;
	};
	
	/**
	 * @brief Redraw all the windows
	 */
	public void forceRedrawAll() {
		if (this.windowsCurrent == null) {
			return;
		}
		final Vector2i size = getSize();
		this.windowsCurrent.setSize(Vector2f(size.x(), size.y()));
		this.windowsCurrent.onChangeSize();
	}
	
	/**
	 * @brief This is to transfert the event from one widget to another one
	 * @param source the widget where the event came from
	 * @param destination the widget where the event mitgh be generated now
	 */
	public void inputEventTransfertWidget(final Widget _source, final Widget _destination) {
		this.input.transfertEvent(_source, _destination);
	}
	
	/**
	 * @brief This fonction lock the pointer properties to move in relative instead of absolute
	 * @param[in] widget The widget that lock the pointer events
	 */
	public void inputEventGrabPointer(final Widget _widget) {
		this.input.grabPointer(_widget);
	}
	
	/**
	 * @brief This fonction un-lock the pointer properties to move in relative instead of absolute
	 */
	public void inputEventUnGrabPointer() {
		this.input.unGrabPointer();
	}
	
	public void onResize(final Vector2i _size) {
		Log.verbose("Resize: " + _size);
		forceRedrawAll();
	}
	
	/**
	 * @brief This is the only one things the User might done in his main();
	 * @note : must be implemented in all system OPS implementation
	 * @note To answare you before you ask the question, this is really simple:
	 *       Due to the fect that the current system is multiple-platform, you "main"
	 *       Does not exist in the android platform, then ewol call other start 
	 *       and stop function, to permit to have only one code
	 * @note The main can not be in the ewol, due to the fact thet is an librairy
	 * @param[in] _argc Standard argc
	 * @param[in] _argv Standard argv
	 * @return normal error int for the application error management
	 */
	public static int main(String[] _args);
	
	private final int initStepId = 0;
	private final int initTotalStep = 0;
	
	/**
	 * @brief Special for init (main) set the start image when loading data
	 * @param[in] _fileName Name of the image to load
	 */
	public void setInitImage(final Uri _fileName) {
		//this.initDisplayImageName = _fileName;
	}
	
	/**
	 * @brief Request a display after call a resize
	 */
	public void requestUpdateSize() {
		final Context context = gale::getContext();
		context.requestUpdateSize();
	}
	
	public void onPeriod(final Clock _time) {
		this.objectManager.timeCall(_time);
	}
}
