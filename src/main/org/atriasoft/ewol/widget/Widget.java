
/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.aknot.annotation.AknotSignal;
import org.atriasoft.esignal.Signal;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension3f;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Matrix4f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.compositing.CompositingDrawing;
import org.atriasoft.ewol.event.EntrySystem;
import org.atriasoft.ewol.event.EventEntry;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.event.EventShortCut;
import org.atriasoft.ewol.event.InputSystem;
import org.atriasoft.ewol.object.EwolObject;
import org.atriasoft.gale.backend3d.OpenGL;
import org.atriasoft.gale.context.ClipboardList;
import org.atriasoft.gale.context.Cursor;
import org.atriasoft.gale.key.KeyKeyboard;
import org.atriasoft.gale.key.KeySpecial;
import org.lwjgl.opengl.GL11;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Widget class is the main widget interface, it has so me generic properties:
 * :** known his parent
 * :** Can be display at a special position with a special scale
 * :** Can get focus
 * :** Receive Event (keyboard / mouse / ...)
 *
 */
public class Widget extends EwolObject {
	private static final Logger LOGGER = LoggerFactory.getLogger(Widget.class);
	// ----------------------------------------------------------------------------------------------------------------
	// -- keyboard event properties Area
	// ----------------------------------------------------------------------------------------------------------------
	private boolean allowRepeatKeyboardEvent = true; //!< This remove the repeating keybord event due to the ant pressing key.
	private Cursor cursorDisplay = Cursor.arrow;

	private final CompositingDrawing drawDebugBorder = null;//new CompositingDrawing(); //!< Compositing drawing element

	// grab cursor mode
	private boolean grabCursor = false;

	// ----------------------------------------------------------------------------------------------------------------
	// -- focus Area
	// ----------------------------------------------------------------------------------------------------------------
	private boolean hasFocus = false; //!< set the focus on this widget

	// ----------------------------------------------------------------------------------------------------------------
	// -- Mouse event properties Area
	// ----------------------------------------------------------------------------------------------------------------
	private int limitMouseEvent = 3; //!< this is to limit the number of mouse event that the widget can supported

	private final List<EventShortCut> localShortcut = new ArrayList<>(); //!< list of all shortcut in the widget

	protected Vector3f maxSize = Vector3f.MAX_VALUE; //!< internal: maximum size of the widget

	protected Vector3f minSize = Vector3f.ZERO; //!< internal: minimum size of the widget
	// ----------------------------------------------------------------------------------------------------------------
	// -- drawing : All drawing must be done in 2 separate buffer 1 for the current display and 1 for the working...
	// ----------------------------------------------------------------------------------------------------------------
	protected boolean needRegenerateDisplay = true; //!< the display might be done the next regeneration
	protected Vector3f offset = Vector3f.ZERO; //!< Offset of the display in the view-port
	protected Vector3f origin = Vector3f.ZERO; //!< internal ... I do not really known how if can use it ...
	protected boolean propertyCanFocus = false; //!< the focus can be done on this widget
	protected Vector3b propertyExpand = Vector3b.FALSE; //!< the widget will expand if possible
	protected Vector3b propertyExpandIfFree = Vector3b.FALSE; //!< the widget will expand if possible
	protected Vector3b propertyFill = Vector3b.FALSE; //!< the widget will fill all the space provided by the parent.
	protected Gravity propertyGravity = Gravity.CENTER; //!< Gravity of the widget
	protected boolean propertyHide = false; //!< hide a widget on the display
	protected Dimension3f propertyMaxSize = new Dimension3f(Vector3f.MAX_VALUE, Distance.PIXEL); //!< user define the maximum size of the widget
	protected Dimension3f propertyMinSize = new Dimension3f(Vector3f.ZERO, Distance.PIXEL); //!< user define the minimum size of the widget

	// ----------------------------------------------------------------------------------------------------------------
	// -- Shortcut : management of the shortcut
	// ----------------------------------------------------------------------------------------------------------------
	@AknotSignal
	@AknotName("shortcut")
	public Signal<String> signalShortcut; //!< signal handle of the message

	// ----------------------------------------------------------------------------------------------------------------
	// -- Widget size:
	// ----------------------------------------------------------------------------------------------------------------
	protected Vector3f size = Vector3f.VALUE_16; //!< internal: current size of the widget

	// internal element calculated by the system
	protected float zoom = 1.0f; //!< generic widget zoom

	/**
	 * Constructor of the widget classes
	 * @return (no exception generated (not managed in embedded platform))
	 */
	public Widget() {}

	/**
	 * calculate the minimum and maximum size (need to estimate expend properties of the widget)
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public void calculateMinMaxSize() {
		calculateMinMaxSizeWidget();
	}

	protected void calculateMinMaxSizeWidget() {
		this.minSize = this.propertyMinSize.getPixel();
		//LOGGER.error("[" + getId() + "] convert in min size : " + propertyMinSize + " out=" + this.minSize);
		this.maxSize = this.propertyMaxSize.getPixel();
		markToRedraw();
	}

	public void calculateSize() {}

	/**
	 * get the expend capabilities (xy)
	 * @return 2D boolean represents the capacity to expend
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public Vector3b canExpand() {
		if (!this.propertyHide) {
			return this.propertyExpand;
		}
		return Vector3b.FALSE;
	}

	/**
	 * get the expend if free capabilities (xy)
	 * @return 2D boolean represents the capacity to expend (if some free space is available)
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public Vector3b canExpandIfFree() {
		if (!this.propertyHide) {
			return this.propertyExpandIfFree;
		}
		return Vector3b.FALSE;
	}

	/**
	 * get the filling capabilities xy
	 * @return Vector3b repensent the capacity to xy filling
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public Vector3b canFill() {
		return this.propertyFill;
	}

	/**
	 * Change Zoom property.
	 * @param range Range of the zoom change.
	 */
	void changeZoom(final float range) {

	}

	/**
	 * Check if the current max size is compatible with the user maximum size
	 *        If it is not the user maximum size will overWrite the maximum size set.
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public void checkMaxSize() {
		final Vector3f pixelSize = this.propertyMaxSize.getPixel();
		this.maxSize = Vector3f.min(this.maxSize, pixelSize);
	}

	/**
	 * Check if the current min size is compatible with the user minimum size
	 *        If it is not the user minimum size will overWrite the minimum size set.
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public void checkMinSize() {
		final Vector3f pixelSize = this.propertyMinSize.getPixel();
		this.minSize = Vector3f.max(this.minSize, pixelSize);
	}

	public void drawWidgetTree(final int level) {
		final StringBuilder space = new StringBuilder();
		for (int iii = 0; iii < level; ++iii) {
			space.append("    ");
		}
		LOGGER.info(space.append("[").append(getId()).append("] name='").append(this.name).append("' type=")
				.append(getClass().getCanonicalName()).append(" o=").append(this.origin).append("  s=")
				.append(this.size).append(" hide=").append(this.propertyHide).toString());
	}

	/**
	 * get the widget maximum size calculated
	 * @return Requested size
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public Vector3f getCalculateMaxSize() {
		if (!this.propertyHide) {
			return this.maxSize;
		}
		return Vector3f.MAX_VALUE;
	}

	/**
	 * get the widget minimum size calculated
	 * @return Requested size
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public Vector3f getCalculateMinSize() {
		if (!this.propertyHide) {
			return this.minSize;
		}
		return Vector3f.ZERO;
	}

	/**
	 * get the current cursor.
	 * @return the type of the cursor.
	 */
	public Cursor getCursor() {
		return this.cursorDisplay;
	}

	/**
	 * get the grabbing status of the cursor.
	 * @return true if the cursor is currently grabbed
	 */
	public boolean getGrabStatus() {
		return this.grabCursor;
	}

	/**
	 * get the keyboard repeating event supporting.
	 * @return true : the event can be repeated.
	 * @return false : the event must not be repeated.
	 */
	public boolean getKeyboardRepeat() {
		return this.allowRepeatKeyboardEvent;
	}

	/**
	 * get the number of mouse event supported
	 * @return return the number of event that the mouse supported [0..3]
	 */
	public int getMouseLimit() {
		return this.limitMouseEvent;
	}

	/**
	 * get the offset property of the widget.
	 * @return The current offset value.
	 */
	Vector3f getOffset() {
		return this.offset;
	}

	/**
	 * Get the origin (absolute position in the windows).
	 * @return Coordinate of the origin requested.
	 */
	public Vector3f getOrigin() {
		return this.origin;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName("focus")
	@AknotDescription("enable the widget to have the focus capacity")
	public boolean getPropertyCanFocus() {
		return this.propertyCanFocus;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName("expand")
	@AknotDescription("Request the widget Expand size while space is available")
	public Vector3b getPropertyExpand() {
		return this.propertyExpand;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName("expand-free")
	@AknotDescription("Request the widget Expand size while free space is detected (does not generate expand in upper wideget)")
	public Vector3b getPropertyExpandIfFree() {
		return this.propertyExpandIfFree;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName("fill")
	@AknotDescription("Fill the widget available size")
	public Vector3b getPropertyFill() {
		return this.propertyFill;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName("gravity")
	@AknotDescription("Gravity orientation")
	public Gravity getPropertyGravity() {
		return this.propertyGravity;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName("hide")
	@AknotDescription("The widget start hided")
	public boolean getPropertyHide() {
		return this.propertyHide;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName("max-size")
	@AknotDescription("User maximum size")
	public Dimension3f getPropertyMaxSize() {
		return this.propertyMaxSize;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName("min-size")
	@AknotDescription("User minimum size")
	public Dimension3f getPropertyMinSize() {
		return this.propertyMinSize;
	}

	/**
	 * get the widget size
	 * @return Requested size
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public Vector3f getSize() {
		if (!this.propertyHide) {
			return this.size;
		}
		return Vector3f.ZERO;
	}

	/**
			 * get the widget at the specific windows absolute position
			 * @param pos gAbsolute position of the requested widget knowledge
			 * @return null No widget found
			 * @return pointer on the widget found
			 * @note : INTERNAL EWOL SYSTEM
			 */
	public Widget getWidgetAtPos(final Vector3f pos) {
		if (!this.propertyHide) {
			return this;
		}
		return null;
	}

	/**
	 * Get the current Widget Manager.
	 */
	public WidgetManager getWidgetManager() {
		return EwolObject.getContext().getWidgetManager();
	}

	/**
	 * Get the current Windows.
	 */
	public Windows getWindows() {
		return EwolObject.getContext().getWindows();
	}

	/**
	 * get the zoom property of the widget
	 * @return the current zoom value
	 */
	public float getZoom() {
		return this.zoom;
	}

	/**
			 * Grab the cursor : This get all the movement of the mouse in PC mode, and generate an offset instead of a position.
			 * @note : the generation of the offset is due to the fact the cursor position is forced at the center of the widget.
			 * @note This done nothing in "Finger" or "Stylet" mode.
			 */
	public void grabCursor() {
		if (!this.grabCursor) {
			EwolObject.getContext().inputEventGrabPointer(this);
			this.grabCursor = true;
		}
	}

	/**
	 * Hide the  keyboard (if needed)
	 */
	protected void hideKeyboard() {
		EwolObject.getContext().keyboardHide();
	}

	/**
	 * get the focus state of the widget
	 * @return focus state
	 */
	public boolean isFocused() {
		return this.hasFocus;
	}

	/**
	 * keep the focus on this widget  == > this remove the previous focus on all other widget
	 */
	public void keepFocus() {
		getWidgetManager().focusKeep(this);
	}

	/**
	 * The widget mark itself that it need to regenerate the nest time.
	 */
	public void markToRedraw() {
		if (this.needRegenerateDisplay) {
			return;
		}
		this.needRegenerateDisplay = true;
		getWidgetManager().markDrawingIsNeeded();
	}

	/**
	 * get the need of the redrawing of the widget and reset it to false
	 * @return true if we need to redraw
	 * @return false if we have no need to redraw
	 */
	protected boolean needRedraw() {
		final boolean tmpData = this.needRegenerateDisplay;
		this.needRegenerateDisplay = false;
		return tmpData;
	}

	/**
	 * Parent have set the size and the origin. The container need to update the child widget property
	 * @note INTERNAL EWOL SYSTEM
	 */
	public void onChangeSize() {
		LOGGER.trace("[" + getId() + "] {" + getClass().getCanonicalName() + "} update size : " + this.size);
		markToRedraw();
	}

	/**
	 * Common widget drawing function (called by the drawing thread [Android, X11, ...])
	 */
	protected void onDraw() {}

	/**
	 * Event on a past event  == > this event is asynchronous due to all system does not support direct getting data.
	 * @note : need to have focus ...
	 * @param clipboardID Mode of data requested
	 */
	public void onEventClipboard(final ClipboardList clipboardID) {}

	/**
	 * Entry event.
	 *        represent the physical event :
	 *            - Keyboard (key event and move event)
	 *            - Accelerometer
	 *            - Joystick
	 * @param event Event properties
	 * @return true if the event has been used
	 * @return false if the event has not been used
	 */
	protected boolean onEventEntry(final EventEntry event) {
		return false;
	}

	/**
	 * Event on an input of this Widget (finger, mouse, stylet)
	 * @param event Event properties
	 * @return true the event is used
	 * @return false the event is not used
	 */
	protected boolean onEventInput(final EventInput event) {
		return false;
	}

	/**
	 * Event on a short-cut of this Widget (in case of return false, the event on the keyevent will arrive in the function @ref onEventKb).
	 * @param special All the special kay pressed at this time.
	 * @param unicodeValue Key pressed by the user not used if the kbMove!=ewol::EVENTKBMOVETYPENONE.
	 * @param kbMove Special key of the keyboard.
	 * @return true if the event has been used.
	 * @return false if the event has not been used.
	 * @note To prevent some error when you get an event get it if it is down and Up ...  ==> like this it could not generate some mistake in the error.
	 */
	public boolean onEventShortCut(
			final KeySpecial special,
			Character unicodeValue,
			final KeyKeyboard kbMove,
			final boolean isDown) {
		unicodeValue = Character.toLowerCase(unicodeValue);
		//LOGGER.trace("check shortcut...." + special + " " + unicodeValue + " " + kbMove + " " + (isDown ? "DOWN" : "UP") + " nb shortcut:" + this.localShortcut.size());
		// Remove the up event of the shortcut...
		if (!isDown) {
			for (int iii = this.localShortcut.size() - 1; iii >= 0; iii--) {
				if (!this.localShortcut.get(iii).isActive) {
					continue;
				}
				if ((this.localShortcut.get(iii).keyboardMoveValue() == KeyKeyboard.UNKNOWN
						&& this.localShortcut.get(iii).unicodeValue() == unicodeValue)
						|| (this.localShortcut.get(iii).keyboardMoveValue() == kbMove
								&& this.localShortcut.get(iii).unicodeValue() == 0)) {
					// In this case we grap the event in case of an error can occured ...
					this.localShortcut.get(iii).isActive = false;
					LOGGER.trace("detect up of a shortcut");
					return true;
				}
			}
		}
		//LOGGER.info("Try to indexOf generic shortcut ...");
		for (int iii = this.localShortcut.size() - 1; iii >= 0; iii--) {
			if (this.localShortcut.get(iii).specialKey().getShift() == special.getShift()
					&& this.localShortcut.get(iii).specialKey().getCtrl() == special.getCtrl()
					&& this.localShortcut.get(iii).specialKey().getAlt() == special.getAlt()
					&& this.localShortcut.get(iii).specialKey().getMeta() == special.getMeta()
					&& ((this.localShortcut.get(iii).keyboardMoveValue() == KeyKeyboard.UNKNOWN
							&& this.localShortcut.get(iii).unicodeValue() == unicodeValue)
							|| (this.localShortcut.get(iii).keyboardMoveValue() == kbMove
									&& this.localShortcut.get(iii).unicodeValue() == 0))) {
				if (isDown) {
					this.localShortcut.get(iii).isActive = true;
					LOGGER.trace("Generate shortCut: " + this.localShortcut.get(iii).message());
					this.signalShortcut.emit(this.localShortcut.get(iii).message());
				}
				return true;
			}
		}
		return false;
	}

	/**
	 * Event of the focus has been grabed by the current widget
	 */
	protected void onGetFocus() {}

	/**
	 * Event of the focus has been lost by the current widget
	 */
	protected void onLostFocus() {}

	protected void onRegenerateDisplay() {}

	protected void onUpdateMinMaxSize() {
		final Vector3f pixelMin = this.propertyMinSize.getPixel();
		final Vector3f pixelMax = this.propertyMaxSize.getPixel();
		// check minimum  maximum compatibility :
		if (pixelMin.x() > pixelMax.x() || pixelMin.y() > pixelMax.y()) {
			LOGGER.error("Can not set a 'min size' > 'max size' reset to maximum ...");
			this.propertyMaxSize = new Dimension3f(Vector3f.MAX_VALUE, Distance.PIXEL);
		}
		requestUpdateSize();
	}

	/**
	 * Convert the absolute position in the local Position (Relative)
	 * @param pos Absolute position that you request conversion.
	 * @return The relative position.
	 */
	public Vector3f relativePosition(final Vector3f pos) {
		return pos.less(this.origin);
	}

	/**
	 * Need to be call When the size of the current widget have change  ==> this force the system to recalculate all the widget positions.
	 */
	public void requestUpdateSize() {
		EwolObject.getContext().requestUpdateSize();
	}

	/**
	 * remove the focus on this widget
	 * @return return true if the widget have release his focus (if he has it)
	 */
	public boolean rmFocus() {
		if (this.propertyCanFocus) {
			if (this.hasFocus) {
				this.hasFocus = false;
				onLostFocus();
			}
			return true;
		}
		return false;
	}

	/**
	 * set the cursor display type.
	 * @param newCursor selected new cursor.
	 */
	public void setCursor(final Cursor newCursor) {
		LOGGER.debug("Change Cursor in " + newCursor);
		this.cursorDisplay = newCursor;
		EwolObject.getContext().setCursor(this.cursorDisplay);
	}

	/**
	 * set focus on this widget
	 * @return return true if the widget keep the focus
	 */
	public boolean setFocus() {
		LOGGER.trace(
				"set focus (start) *propertyCanFocus=" + this.propertyCanFocus + " this.hasFocus=" + this.hasFocus);
		if (this.propertyCanFocus) {
			if (!this.hasFocus) {
				this.hasFocus = true;
				onGetFocus();
			}
			LOGGER.trace("set focus (stop) ret true");
			return true;
		}
		LOGGER.trace("set focus (stop) ret false");
		return false;
	}

	/**
	 * set the keyboard repeating event supporting.
	 * @param state The repeating status (true: enable, false disable).
	 */
	protected void setKeyboardRepeat(final boolean state) {
		this.allowRepeatKeyboardEvent = state;
	}

	/**
	 * get the number of mouse event supported
	 * @param numberState The number of event that the mouse supported [0..3]
	 */
	public void setMouseLimit(final int numberState) {
		this.limitMouseEvent = numberState;
	}

	/**
	 * User set No maximum size.
	 */
	public void setNoMaxSize() {
		setPropertyMaxSize(new Dimension3f(Vector3f.MAX_VALUE, Distance.PIXEL));
	}

	/**
	 * User set No minimum size.
	 */
	public void setNoMinSize() {
		setPropertyMinSize(new Dimension3f(Vector3f.ZERO, Distance.PIXEL));
	}

	/**
	 * set the zoom property of the widget.
	 * @param newVal offset value.
	 */
	public void setOffset(final Vector3f newVal) {
		LOGGER.info("Set offset: " + newVal);
		if (this.offset != newVal) {
			this.offset = newVal;
			markToRedraw();
		}
	}

	/**
	 * Set origin at the widget (must be an parent widget that set this parameter).
	 * This represent the absolute origin in the program windows.
	 * @param pos Position of the origin.
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public void setOrigin(final Vector3f pos) {
		this.origin = pos;
	}

	public void setPropertyCanFocus(final boolean canFocus) {
		if (this.propertyCanFocus == canFocus) {
			return;
		}
		this.propertyCanFocus = canFocus;
		if (this.propertyCanFocus) {
			return;
		}
		if (this.hasFocus) {
			rmFocus();
		}
	}

	public void setPropertyExpand(final Vector3b value) {
		if (this.propertyExpand.equals(value)) {
			return;
		}
		this.propertyExpand = value;
		markToRedraw();
		requestUpdateSize();
	}

	public void setPropertyExpandIfFree(final Vector3b value) {
		if (this.propertyExpandIfFree.equals(value)) {
			return;
		}
		this.propertyExpandIfFree = value;
		markToRedraw();
		requestUpdateSize();
	}

	public void setPropertyFill(final Vector3b value) {
		if (this.propertyFill.equals(value)) {
			return;
		}
		this.propertyFill = value;
		markToRedraw();
		requestUpdateSize();
	}

	public void setPropertyGravity(final Gravity gravity) {
		if (this.propertyGravity.equals(gravity)) {
			return;
		}
		this.propertyGravity = gravity;
		markToRedraw();
		requestUpdateSize();
	}

	public void setPropertyHide(final boolean value) {
		if (this.propertyHide == value) {
			return;
		}
		this.propertyHide = value;
		markToRedraw();
		requestUpdateSize();
	}

	public void setPropertyMaxSize(final Dimension3f value) {
		if (this.propertyMaxSize.equals(value)) {
			return;
		}
		this.propertyMaxSize = value;
		onUpdateMinMaxSize();
	}

	public void setPropertyMinSize(final Dimension3f value) {
		if (this.propertyMinSize.equals(value)) {
			return;
		}
		this.propertyMinSize = value;
		onUpdateMinMaxSize();
	}

	/**
	 * set the widget size
	 * @return Requested size
	 * @note : INTERNAL EWOL SYSTEM Do not modify the size yourself: calculation is complex and need knowledge of around widget
	 */
	public void setSize(final Vector3f value) {
		this.size = value;
		if (this.size.x() > 15000) {
			return;
		}
	}

	/**
	 * set the zoom property of the widget
	 * @param newVal newZoom value
	 */
	public void setZoom(final float newVal) {
		if (this.zoom == newVal) {
			return;
		}
		this.zoom = FMath.avg(0.0000001f, newVal, 1000000.0f);
		markToRedraw();
	}

	/**
	 * add a specific shortcut with his description
	 * @param descriptiveString Description string of the shortcut
	 */
	protected void shortCutAdd(final String descriptiveString) {
		shortCutAdd(descriptiveString, "");
	}

	/**
	 * add a specific shortcut with his description
	 * @param descriptiveString Description string of the shortcut
	 * @param message massage to generate (or shortcut name)
	 */
	protected void shortCutAdd(final String descriptiveString, final String sendMessage) {
		if (descriptiveString.length() == 0) {
			LOGGER.error("try to add shortcut with no descriptive string ...");
			return;
		}
		String message; //!< data link with the event
		final KeySpecial specialKey = new KeySpecial(); //!< special board key
		Character unicodeValue = null; //!< 0 if not used
		KeyKeyboard keyboardMoveValue = KeyKeyboard.UNKNOWN; //!< ewol::EVENTKBMOVETYPENONE if not used
		if (sendMessage.length() == 0) {
			message = descriptiveString;
		} else {
			message = sendMessage;
		}
		// parsing of the string:
		//"ctrl+shift+alt+metatmpElement.+s"
		if (descriptiveString.contains("ctrl")) {
			specialKey.setCtrlLeft(true);
		}
		if (descriptiveString.contains("shift")) {
			specialKey.setShiftLeft(true);
		}
		if (descriptiveString.contains("alt")) {
			specialKey.setAltLeft(true);
		}
		if (descriptiveString.contains("meta")) {
			specialKey.setMetaLeft(true);
		}
		if (descriptiveString.contains("F12")) {
			keyboardMoveValue = KeyKeyboard.F12;
		} else if (descriptiveString.contains("F11")) {
			keyboardMoveValue = KeyKeyboard.F11;
		} else if (descriptiveString.contains("F10")) {
			keyboardMoveValue = KeyKeyboard.F10;
		} else if (descriptiveString.contains("F9")) {
			keyboardMoveValue = KeyKeyboard.F9;
		} else if (descriptiveString.contains("F8")) {
			keyboardMoveValue = KeyKeyboard.F8;
		} else if (descriptiveString.contains("F7")) {
			keyboardMoveValue = KeyKeyboard.F7;
		} else if (descriptiveString.contains("F6")) {
			keyboardMoveValue = KeyKeyboard.F6;
		} else if (descriptiveString.contains("F5")) {
			keyboardMoveValue = KeyKeyboard.F5;
		} else if (descriptiveString.contains("F4")) {
			keyboardMoveValue = KeyKeyboard.F4;
		} else if (descriptiveString.contains("F3")) {
			keyboardMoveValue = KeyKeyboard.F3;
		} else if (descriptiveString.contains("F2")) {
			keyboardMoveValue = KeyKeyboard.F2;
		} else if (descriptiveString.contains("F1")) {
			keyboardMoveValue = KeyKeyboard.F1;
		} else if (descriptiveString.contains("LEFT")) {
			keyboardMoveValue = KeyKeyboard.LEFT;
		} else if (descriptiveString.contains("RIGHT")) {
			keyboardMoveValue = KeyKeyboard.RIGHT;
		} else if (descriptiveString.contains("UP")) {
			keyboardMoveValue = KeyKeyboard.UP;
		} else if (descriptiveString.contains("DOWN")) {
			keyboardMoveValue = KeyKeyboard.DOWN;
		} else if (descriptiveString.contains("PAGEUP")) {
			keyboardMoveValue = KeyKeyboard.PAGE_UP;
		} else if (descriptiveString.contains("PAGEDOWN")) {
			keyboardMoveValue = KeyKeyboard.PAGE_DOWN;
		} else if (descriptiveString.contains("START")) {
			keyboardMoveValue = KeyKeyboard.START;
		} else if (descriptiveString.contains("END")) {
			keyboardMoveValue = KeyKeyboard.END;
		} else if (descriptiveString.contains("PRINT")) {
			keyboardMoveValue = KeyKeyboard.PRINT;
		} else if (descriptiveString.contains("ARRETDEFIL")) {
			keyboardMoveValue = KeyKeyboard.STOP_DEFIL;
		} else if (descriptiveString.contains("WAIT")) {
			keyboardMoveValue = KeyKeyboard.WAIT;
		} else if (descriptiveString.contains("INSERT")) {
			keyboardMoveValue = KeyKeyboard.INSERT;
		} else if (descriptiveString.contains("CAPLOCK")) {
			keyboardMoveValue = KeyKeyboard.CAP_LOCK;
		} else if (descriptiveString.contains("CONTEXTMENU")) {
			keyboardMoveValue = KeyKeyboard.CONTEXT_MENU;
		} else if (descriptiveString.contains("NUMLOCK")) {
			keyboardMoveValue = KeyKeyboard.NUM_LOCK;
		} else {
			unicodeValue = descriptiveString.charAt(descriptiveString.length() - 1);
		}
		// add it on the List ...
		this.localShortcut.add(new EventShortCut(message, specialKey, unicodeValue, keyboardMoveValue, true));
	}

	/**
	 * remove all current shortCut
	 */
	protected void shortCutClean() {
		this.localShortcut.clear();
	}

	/**
			 * remove a specific shortCut with his event name
			 * @param message generated event name
			 */
	protected void shortCutRemove(final String message) {
		this.localShortcut.removeIf(eventShortCut -> eventShortCut.message().contentEquals(message));
	}

	/**
	 * display the  keyboard (if needed)
	 */
	protected void showKeyboard() {
		EwolObject.getContext().keyboardShow();
	}

	/**
	 * {SYSTEM} extern interface to request a draw ...  (called by the drawing thread [Android, X11, ...])
	 * This function generate a clipping with the view-port openGL system. Like this a widget draw can not draw over an other widget
	 * @note This function is  for the scrolled widget, and the more complicated openGL widget
	 * @param displayProp properties of the current display
	 * @note : INTERNAL EWOL SYSTEM
	                                                              /-. displayProp.this.windowsSize
	      *------------------------------------------------------*
	      |                                                      |
	      |                                           this.size  |
	      |                                                 /    |
	      |                        *-----------------------*     |
	      |                        '                       '     |
	      |                        ' displayProp.this.size '     |
	      |              Viewport  '          /            '     |
	      |              o---------'---------o             '     |
	      |              |         '         |             '     |
	      |              |         '         |             '     |
	      |              |         '         |             '     |
	      |              |         '         |             '     |
	      |              |         *-----------------------*     |
	      |              |        /          |                   |
	      |              |this.offset        |                   |
	      |              |                   |                   |
	      |              o-------------------o                   |
	      |             /                                        |
	      |displayProp.this.origin                               |
	      |                                                      |
	      *------------------------------------------------------*
	     /
	   (0,0)
	 */
	public void systemDraw(final DrawProperty displayProp) {
		systemDrawWidget(displayProp);
	}

	protected void systemDrawWidget(final DrawProperty displayProp) {
		//LOGGER.info("[" + getId() + "] Draw : [" + propertyName + "] t=" + getObjectType() + " o=" + this.origin + "  s=" << this.size << " hide=" << propertyHide);
		if (this.propertyHide) {
			// widget is hidden ...
			return;
		}
		final Vector3f displayOrigin = this.origin.add(this.offset);

		// check if the element is displayable in the windows :
		if (displayProp.windowsSize().x() < this.origin.x() || displayProp.windowsSize().y() < this.origin.y()) {
			// out of the windows  == > nothing to display ...
			return;
		}

		final DrawProperty tmpSize = displayProp.withLimit(this.origin, this.size);
		if (tmpSize.size().x() <= 0 || tmpSize.size().y() <= 0) {
			return;
		}
		//LOGGER.info("setViewport(" + tmpSize.origin() + ", " + tmpSize.size() + ")");
		OpenGL.setViewPort(tmpSize.origin(), tmpSize.size());
		// special case, when origin < display origin, we need to cut the display :
		Vector2i downOffset = new Vector2i((int) (this.origin.x() - tmpSize.origin().x()),
				(int) (this.origin.y() - tmpSize.origin().y()));
		downOffset = Vector2i.min(downOffset, Vector2i.ZERO);
		//LOGGER.info("translate : (" + (new Vector3f(-tmpSize.size().x() / 2 + this.offset.x() + downOffset.x(), -tmpSize.size().y() / 2 + this.offset.y() + downOffset.y(), -1.0f)).clipInteger());
		// translate the display to have a Gui 0,0 position on the Left button angle
		final Matrix4f tmpTranslate = Matrix4f
				.createMatrixTranslate((new Vector3f(-tmpSize.size().x() / 2 + this.offset.x() + downOffset.x(),
						-tmpSize.size().y() / 2 + this.offset.y() + downOffset.y(), -1.0f)).clipInteger());
		//final Matrix4f tmpTranslate = Matrix4f.createMatrixTranslate(new Vector3f(0, 0, 1.0f));
		// Scale if needed (feature not validate)
		final Matrix4f tmpScale = Matrix4f.createMatrixScale(this.zoom, this.zoom, 1.0f);
		// create orthogonal projection for GUI ==> simple to manage staking
		final Matrix4f tmpProjection = Matrix4f.createMatrixOrtho(-tmpSize.size().x() / 2, tmpSize.size().x() / 2,
				-tmpSize.size().y() / 2, tmpSize.size().y() / 2, -500, 500);
		//Matrix4f tmpMat = tmpProjection.multiply(tmpScale).multiply(tmpTranslate);

		OpenGL.push();
		// set internal matrix system :
		//OpenGL.setMatrix(tmpMat);
		OpenGL.setMatrix(tmpProjection);
		OpenGL.setCameraMatrix(tmpScale.multiply(tmpTranslate));
		if (this.drawDebugBorder != null) {
			this.drawDebugBorder.draw();
		}
		//long startTime = ewol::getTime();
		onDraw();
		OpenGL.pop();
		GL11.glFinish();
	}

	/**
	 * {SYSTEM} Entry event (only meta widget might overwrite this function).
	 * @param event Event properties
	 * @return true if the event has been used
	 * @return false if the event has not been used
	 */
	public boolean systemEventEntry(final EntrySystem event) {
		if (this.parent != null && this.parent.get() != null && this.parent.get() instanceof final Widget up) {
			if (up.systemEventEntry(event)) {
				return true;
			}
		}
		return onEventEntry(event.event());
	}

	/**
	 * {SYSTEM} system event input (only meta widget might overwrite this function).
	 * @param event Event properties
	 * @return true the event is used
	 * @return false the event is not used
	 */
	public boolean systemEventInput(final InputSystem event) {
		if (this.parent != null) {
			final Widget up = (Widget) this.parent.get();
			if (up != null) {
				if (up.systemEventInput(event)) {
					return true;
				}
			}
		}
		return onEventInput(event.event());
	}

	/**
	 * Event generated when a redraw is needed
	 */
	public void systemRegenerateDisplay() {
		if (this.drawDebugBorder != null) {
			this.drawDebugBorder.clear();
			this.drawDebugBorder.setColor(Color.RED);
			this.drawDebugBorder.setPos(1, 1);
			this.drawDebugBorder.setThickness(1);
			this.drawDebugBorder.lineRel(this.size.x() - 2, 0); // TODO PB with the thickness when draw rectangle ...
			this.drawDebugBorder.setPos(this.size.x() - 1, 1);
			this.drawDebugBorder.lineRel(0, this.size.y() - 2);
			this.drawDebugBorder.setPos(this.size.x() - 1, this.size.y() - 1);
			this.drawDebugBorder.lineRel(-this.size.x() - 2, 0);
			this.drawDebugBorder.setPos(1, this.size.y() - 1);
			this.drawDebugBorder.lineRel(0, -this.size.y() - 2);
			/*
			this.drawDebugBorder.setColor(Color.BLUE);
			this.drawDebugBorder.setPos(3, 3);
			this.drawDebugBorder.rectangleWidth(new Vector3f(this.size.x() - 6, this.size.y() - 6, 0));
			*/
			this.drawDebugBorder.flush();
		}
		onRegenerateDisplay();
	}

	/**
	 * Un-Grab the cursor (default mode cursor offset)
	 */
	public void unGrabCursor() {
		if (this.grabCursor) {
			EwolObject.getContext().inputEventUnGrabPointer();
			this.grabCursor = false;
		}
	}
}
