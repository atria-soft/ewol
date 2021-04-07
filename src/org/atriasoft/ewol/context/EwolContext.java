/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.context;

import org.atriasoft.echrono.Clock;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.etranslate.ETranslate;
import org.atriasoft.ewol.event.EntrySystem;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.object.ObjectManager;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.ewol.widget.WidgetManager;
import org.atriasoft.ewol.widget.Windows;
import org.atriasoft.gale.Application;
import org.atriasoft.gale.Gale;
import org.atriasoft.gale.context.ClipboardList;
import org.atriasoft.gale.context.CommandLine;
import org.atriasoft.gale.context.Context;
import org.atriasoft.gale.key.KeyKeyboard;
import org.atriasoft.gale.key.KeySpecial;
import org.atriasoft.gale.key.KeyStatus;
import org.atriasoft.gale.key.KeyType;
import org.atriasoft.gale.resource.ResourceManager;

// Here we hereted from the gale application to be agnostic of the OW where we work ...
public class EwolContext extends Application {
	
	/**
	 * From everyware in the program, we can get the context inteface.
	 * @return current reference on the instance.
	 */
	@SuppressWarnings("preview")
	public static EwolContext getContext() {
		Application appl = Gale.getContext().getApplication();
		if (appl instanceof EwolContext elem) {
			return elem;
		}
		return null;
	}
	
	private EwolApplication application; // !< Application handle
	
	private final ConfigFont configFont = new ConfigFont(); // !< global font configuration
	
	private final int initStepId = 0;
	
	private final int initTotalStep = 0;
	
	private final InputManager input;
	
	private final ObjectManager objectManager; // !< Object Manager main instance
	
	private final WidgetManager widgetManager = new WidgetManager(); // !< global widget manager
	
	private Windows windowsCurrent = null; // !< current displayed windows
	
	public EwolContext(final EwolApplication application) {
		this.application = application;
		this.objectManager = new ObjectManager(this);
		this.input = new InputManager(this);
		if (this.application == null) {
			Log.critical("Can not start context with no Application ==> rtfm ...");
		}
	}
	
	/**
	 * Redraw all the windows
	 */
	public void forceRedrawAll() {
		if (this.windowsCurrent == null) {
			return;
		}
		final Vector2f size = getSize();
		this.windowsCurrent.setSize(new Vector2f((int) size.x(), (int) size.y()));
		this.windowsCurrent.onChangeSize();
	}
	
	public EwolApplication getApplication() {
		return this.application;
	}
	
	public CommandLine getCmd() {
		return Gale.getContext().getCmd();
	}
	
	public ObjectManager getEObjectManager() {
		return this.objectManager;
	}
	
	public ConfigFont getFontDefault() {
		return this.configFont;
	}
	
	public ResourceManager getResourcesManager() {
		return Gale.getContext().getResourcesManager();
	}
	
	public WidgetManager getWidgetManager() {
		return this.widgetManager;
	}
	
	/**
	 * get the current windows that is displayed
	 * @return the current handle on the windows (can be null)
	 */
	public Windows getWindows() {
		return this.windowsCurrent;
	}
	
	/**
	 * This fonction lock the pointer properties to move in relative instead
	 *        of absolute
	 * @param widget The widget that lock the pointer events
	 */
	public void inputEventGrabPointer(final Widget widget) {
		this.input.grabPointer(widget);
	}
	
	/**
	 * This is to transfert the event from one widget to another one
	 * @param source      the widget where the event came from
	 * @param destination the widget where the event mitgh be generated now
	 */
	public void inputEventTransfertWidget(final Widget source, final Widget destination) {
		this.input.transfertEvent(source, destination);
	}
	
	/**
	 * This fonction un-lock the pointer properties to move in relative
	 *        instead of absolute
	 */
	public void inputEventUnGrabPointer() {
		this.input.unGrabPointer();
	}
	
	@Override
	public void onClipboardEvent(final ClipboardList clipboardId) {
		final Widget tmpWidget = this.widgetManager.focusGet();
		if (tmpWidget != null) {
			tmpWidget.onEventClipboard(clipboardId);
		}
	}
	
	@Override
	public void onCreate(final Context context) {
		Log.info(" == > Ewol system create (BEGIN)");
		// Add basic ewol translation:
		ETranslate.addPath("ewol", new Uri("TRANSLATE", "", "ewol"));
		ETranslate.autoDetectLanguage();
		// parse for help:
		for (int iii = 0; iii < context.getCmd().size(); ++iii) {
			if (context.getCmd().get(iii).equals("-h") || context.getCmd().get(iii).equals("--help")) {
				Log.print("ewol - help : ");
				Log.print("    xxxxxxxxxxxxx [options]");
				Log.print("        -h/--help:    Display this help");
				Log.print("    example:");
				Log.print("        xxxxxxxxxxxxx --help");
				// this is a global help system does not remove it
				continue;
			}
			//context.getCmd().remove(iii);
			//--iii;
		}
		
		// Log.info("EWOL v:" + ewol::getVersion());
		// force a recalculation
		/*
		 * requestUpdateSize(){ Context context = gale::getContext();
		 * context.requestUpdateSize(); } #if
		 * defined(EWOLANDROIDORIENTATIONLANDSCAPE)
		 * forceOrientation(ewol::screenLandscape); #elif
		 * defined(EWOLANDROIDORIENTATIONPORTRAIT)
		 * forceOrientation(ewol::screenPortrait); #else
		 * forceOrientation(ewol::screenAuto); #endif
		 */
		final EwolApplication appl = this.application;
		if (appl == null) {
			Log.error(" == > Create without application");
			return;
		}
		appl.onCreate(this);
		Log.info(" == > Ewol system create (END)");
	}
	
	@Override
	public void onDestroy(final Context context) {
		Log.info(" == > Ewol system destroy (BEGIN)");
		// Remove current windows
		this.windowsCurrent = null;
		// clean all widget and sub widget with their resources:
		this.objectManager.cleanInternalRemoved();
		final EwolApplication appl = this.application;
		if (appl != null) {
			// call application to uninit
			appl.onDestroy(this);
			this.application = null;
		}
		// internal clean elements
		this.objectManager.cleanInternalRemoved();
		Log.info("List of all widget of this context must be equal at 0 ==> otherwise some remove is missing");
		this.objectManager.displayListObject();
		// now All must be removed !!!
		this.objectManager.unInit();
		Log.info(" == > Ewol system destroy (END)");
	}
	
	@Override
	public void onDraw(final Context context) {
		// Log.info("DRAW");
		// clean internal data...
		this.objectManager.cleanInternalRemoved();
		// real draw...
		final Windows window = this.windowsCurrent;
		if (window == null) {
			return;
		}
		window.sysDraw();
	}
	
	@Override
	public void onKeyboard(final KeySpecial special, final KeyKeyboard type, final Character value, final KeyStatus state) {
		Log.verbose("event {" + special + "} " + type + " " + value + " " + state);
		// store the keyboard special key status for mouse event...
		this.input.setLastKeyboardSpecial(special);
		if (this.windowsCurrent == null) {
			// No windows ...
			return;
		}
		final boolean repeate = (state == KeyStatus.downRepeat);
		final boolean isDown = (state == KeyStatus.downRepeat) || (state == KeyStatus.down);
		if (this.windowsCurrent.onEventShortCut(special, value, type, isDown)) {
			// Keep a shortcut ...
			return;
		}
		// get the current focused Widget :
		final Widget tmpWidget = this.widgetManager.focusGet();
		if (tmpWidget == null) {
			// no Widget ...
			return;
		}
		// check if the widget allow repeating key events.
		// Log.info("repeating test :" + repeate + " widget=" +
		// tmpWidget.getKeyboardRepeate() + " state=" + isDown);
		if (!repeate || (repeate && tmpWidget.getKeyboardRepeat())) {
			// check Widget shortcut
			if (!tmpWidget.onEventShortCut(special, value, type, isDown)) {
				// generate the direct event ...
				if (type == KeyKeyboard.character) {
					final EntrySystem tmpEntryEvent;
					if (isDown) {
						tmpEntryEvent = new EntrySystem(KeyKeyboard.character, KeyStatus.down, special, value);
					} else {
						tmpEntryEvent = new EntrySystem(KeyKeyboard.character, KeyStatus.up, special, value);
					}
					tmpWidget.systemEventEntry(tmpEntryEvent);
				} else { // THREADKEYBORADMOVE
					final EntrySystem tmpEntryEvent;
					if (isDown) {
						tmpEntryEvent = new EntrySystem(KeyKeyboard.character, KeyStatus.down, special, null);
					} else {
						tmpEntryEvent = new EntrySystem(KeyKeyboard.character, KeyStatus.up, special, null);
					}
					tmpWidget.systemEventEntry(tmpEntryEvent);
				}
			} else {
				Log.debug("remove Repeate key ...");
			}
		}
	}
	
	@Override
	public void onKillDemand(final Context context) {
		Log.info(" == > User demand a destroy (BEGIN)");
		final EwolApplication appl = this.application;
		if (appl == null) {
			exit(0);
			return;
		}
		appl.onKillDemand(this);
		Log.info(" == > User demand a destroy (END)");
	}
	
	@Override
	public void onPause(final Context context) {
		Log.info(" == > Ewol system pause (BEGIN)");
		final EwolApplication appl = this.application;
		if (appl == null) {
			return;
		}
		appl.onPause(this);
		Log.info(" == > Ewol system pause (END)");
	}
	
	public void onPeriod(final Clock time) {
		this.objectManager.timeCall(time);
	}
	
	public void onPointer(final KeyType type, final int pointerID, final Vector2f pos, final KeyStatus state) {
		switch (state) {
			case move:
				// Log.debug("Receive MSG : THREAD_INPUT_MOTION");
				this.input.motion(type, pointerID, pos);
				break;
			case down:
			case downRepeat:
				// Log.debug("Receive MSG : THREAD_INPUT_STATE");
				this.input.state(type, pointerID, true, pos);
				break;
			case up:
				// Log.debug("Receive MSG : THREAD_INPUT_STATE");
				this.input.state(type, pointerID, false, pos);
				break;
			default:
				Log.debug("Unknow state : " + state);
				break;
		}
	}
	
	@Override
	public void onRegenerateDisplay(final Context context) {
		// Log.info("REGENERATEDISPLAY");
		// check if the user selected a windows
		final Windows window = this.windowsCurrent;
		if (window == null) {
			Log.debug("No windows ...");
			return;
		}
		// Redraw all needed elements
		window.onRegenerateDisplay();
		if (this.widgetManager.isDrawingNeeded()) {
			markDrawingIsNeeded();
		}
		// markDrawingIsNeeded();
	}
	
	public void onResize(final Vector2i size) {
		Log.verbose("Resize: " + size);
		forceRedrawAll();
	}
	
	@Override
	public void onResume(final Context context) {
		Log.info(" == > Ewol system resume (BEGIN)");
		final EwolApplication appl = this.application;
		if (appl == null) {
			return;
		}
		appl.onResume(this);
		Log.info(" == > Ewol system resume (END)");
	}
	
	@Override
	public void onStart(final Context context) {
		Log.info(" == > Ewol system start (BEGIN)");
		final EwolApplication appl = this.application;
		if (appl == null) {
			// TODO : Request exit of the application .... with error ...
			return;
		}
		appl.onStart(this);
		Log.info(" == > Ewol system start (END)");
	}
	
	@Override
	public void onStop(final Context context) {
		Log.info(" == > Ewol system stop (BEGIN)");
		final EwolApplication appl = this.application;
		if (appl == null) {
			return;
		}
		appl.onStop(this);
		Log.info(" == > Ewol system stop (END)");
	}
	
	/**
	 * Request a display after call a resize
	 */
	public void requestUpdateSize() {
		final Context context = Gale.getContext();
		context.requestUpdateSize();
	}
	
	/**
	 * reset event management for the IO like Input ou Mouse or keyborad
	 */
	public void resetIOEvent() {
		this.input.newLayerSet();
	}
	
	/**
	 * Special for init (main) set the start image when loading data
	 * @param fileName Name of the image to load
	 */
	public void setInitImage(final Uri fileName) {
		// this.initDisplayImageName = fileName;
	}
	
	/**
	 * set the current windows to display :
	 * @param windows Windows that might be displayed
	 */
	public void setWindows(final Windows windows) {
		Log.info("set New windows");
		// remove current focus :
		this.widgetManager.focusSetDefault(null);
		this.widgetManager.focusRelease();
		// set the new pointer as windows system
		this.windowsCurrent = windows;
		// set the new default focus:
		this.widgetManager.focusSetDefault(windows);
		// display the title of the Windows:
		if (this.windowsCurrent != null) {
			setTitle(this.windowsCurrent.propertyTitle);
		}
		// request all the widget redrawing
		forceRedrawAll();
	}
}
