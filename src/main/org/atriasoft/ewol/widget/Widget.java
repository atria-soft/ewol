/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.esignal.Signal;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.databind.annotation.JsonDeserialize;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlProperty;
import org.atriasoft.ewol.internal.WidgetDeserializer;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Matrix4f;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
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
@JsonDeserialize(using = WidgetDeserializer.class)
public class Widget extends EwolObject {
	private static final Logger LOGGER = LoggerFactory.getLogger(Widget.class);
	/** Keyboard repeat events enabled */
	private boolean allowRepeatKeyboardEvent = true;
	private Cursor cursorDisplay = Cursor.arrow;
	private final CompositingDrawing drawDebugBorder = null;
	private boolean grabCursor = false;
	private boolean hasFocus = false;
	/** Maximum number of mouse events supported [0..3] */
	private int limitMouseEvent = 3;
	private final List<EventShortCut> localShortcut = new ArrayList<>();
	
	protected Vector2f maxSize = Vector2f.MAX_VALUE;
	protected Vector2f minSize = Vector2f.ZERO;
	protected boolean needRegenerateDisplay = true;
	protected Vector2f offset = Vector2f.ZERO;
	protected Vector2f origin = Vector2f.ZERO;
	protected boolean propertyCanFocus = false;
	protected Vector2b propertyExpand = Vector2b.FALSE;
	protected Vector2b propertyExpandIfFree = Vector2b.FALSE;
	protected Vector2b propertyFill = Vector2b.FALSE;
	protected Gravity propertyGravity = Gravity.CENTER;
	protected boolean propertyHide = false;
	protected Dimension2f propertyMaxSize = new Dimension2f(Vector2f.MAX_VALUE, Distance.PIXEL);
	protected Dimension2f propertyMinSize = new Dimension2f(Vector2f.ZERO, Distance.PIXEL);
	
	public Signal<String> signalShortcut;
	
	protected Vector2f size = Vector2f.VALUE_16;
	protected float zoom = 1.0f;

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
	public Vector2b canExpand() {
		if (!this.propertyHide) {
			return this.propertyExpand;
		}
		return Vector2b.FALSE;
	}

	/**
	 * get the expend if free capabilities (xy)
	 * @return 2D boolean represents the capacity to expend (if some free space is available)
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public Vector2b canExpandIfFree() {
		if (!this.propertyHide) {
			return this.propertyExpandIfFree;
		}
		return Vector2b.FALSE;
	}

	/**
	 * get the filling capabilities xy
	 * @return Vector2b repensent the capacity to xy filling
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public Vector2b canFill() {
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
		final Vector2f pixelSize = this.propertyMaxSize.getPixel();
		this.maxSize = Vector2f.min(this.maxSize, pixelSize);
	}

	/**
	 * Check if the current min size is compatible with the user minimum size
	 *        If it is not the user minimum size will overWrite the minimum size set.
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public void checkMinSize() {
		final Vector2f pixelSize = this.propertyMinSize.getPixel();
		this.minSize = Vector2f.max(this.minSize, pixelSize);
	}

	public void drawWidgetTree(final int level) {
		final StringBuilder space = new StringBuilder();
		for (int iii = 0; iii < level; ++iii) {
			space.append("    ");
		}
		LOGGER.info("{}[{}] name='{}' type={} o={}  s={} hide={}", space, getId(), this.name,
				getClass().getCanonicalName(), this.origin, this.size, this.propertyHide);
	}

	/**
	 * get the widget maximum size calculated
	 * @return Requested size
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public Vector2f getCalculateMaxSize() {
		if (!this.propertyHide) {
			return this.maxSize;
		}
		return Vector2f.MAX_VALUE;
	}

	/**
	 * get the widget minimum size calculated
	 * @return Requested size
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public Vector2f getCalculateMinSize() {
		if (!this.propertyHide) {
			return this.minSize;
		}
		return Vector2f.ZERO;
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
	Vector2f getOffset() {
		return this.offset;
	}

	/**
	 * Get the origin (absolute position in the windows).
	 * @return Coordinate of the origin requested.
	 */
	public Vector2f getOrigin() {
		return this.origin;
	}

	@JsonProperty("focus")
	@JacksonXmlProperty(isAttribute = true, localName = "focus")
	public boolean getPropertyCanFocus() {
		return this.propertyCanFocus;
	}

	@JsonProperty("expand")
	@JacksonXmlProperty(isAttribute = true, localName = "expand")
	public Vector2b getPropertyExpand() {
		return this.propertyExpand;
	}

	@JsonProperty("expand-free")
	@JacksonXmlProperty(isAttribute = true, localName = "expand-free")
	public Vector2b getPropertyExpandIfFree() {
		return this.propertyExpandIfFree;
	}

	@JsonProperty("fill")
	@JacksonXmlProperty(isAttribute = true, localName = "fill")
	public Vector2b getPropertyFill() {
		return this.propertyFill;
	}

	@JsonProperty("gravity")
	@JacksonXmlProperty(isAttribute = true, localName = "gravity")
	public Gravity getPropertyGravity() {
		return this.propertyGravity;
	}

	@JsonProperty("hide")
	@JacksonXmlProperty(isAttribute = true, localName = "hide")
	public boolean getPropertyHide() {
		return this.propertyHide;
	}

	@JsonProperty("max-size")
	@JacksonXmlProperty(isAttribute = true, localName = "max-size")
	public Dimension2f getPropertyMaxSize() {
		return this.propertyMaxSize;
	}

	@JsonProperty("min-size")
	@JacksonXmlProperty(isAttribute = true, localName = "min-size")
	public Dimension2f getPropertyMinSize() {
		return this.propertyMinSize;
	}

	/**
	 * get the widget size
	 * @return Requested size
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public Vector2f getSize() {
		if (!this.propertyHide) {
			return this.size;
		}
		return Vector2f.ZERO;
	}

	/**
			 * get the widget at the specific windows absolute position
			 * @param pos gAbsolute position of the requested widget knowledge
			 * @return null No widget found
			 * @return pointer on the widget found
			 * @note : INTERNAL EWOL SYSTEM
			 */
	public Widget getWidgetAtPos(final Vector2f pos) {
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
		LOGGER.trace("[{}] {{}} update size: {}", getId(), getClass().getCanonicalName(), this.size);
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
					LOGGER.trace("Generate shortCut: {}", this.localShortcut.get(iii).message());
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
		final Vector2f pixelMin = this.propertyMinSize.getPixel();
		final Vector2f pixelMax = this.propertyMaxSize.getPixel();
		// check minimum  maximum compatibility :
		if (pixelMin.x() > pixelMax.x() || pixelMin.y() > pixelMax.y()) {
			LOGGER.error("Can not set a 'min size' > 'max size' reset to maximum ...");
			this.propertyMaxSize = new Dimension2f(Vector2f.MAX_VALUE, Distance.PIXEL);
		}
		requestUpdateSize();
	}

	/**
	 * Convert the absolute position in the local Position (Relative)
	 * @param pos Absolute position that you request conversion.
	 * @return The relative position.
	 */
	public Vector2f relativePosition(final Vector2f pos) {
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
		LOGGER.debug("Change Cursor in {}", newCursor);
		this.cursorDisplay = newCursor;
		EwolObject.getContext().setCursor(this.cursorDisplay);
	}

	/**
	 * set focus on this widget
	 * @return return true if the widget keep the focus
	 */
	public boolean setFocus() {
		LOGGER.trace("set focus (start) *propertyCanFocus={} this.hasFocus={}", this.propertyCanFocus, this.hasFocus);
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
		setPropertyMaxSize(new Dimension2f(Vector2f.MAX_VALUE, Distance.PIXEL));
	}

	/**
	 * User set No minimum size.
	 */
	public void setNoMinSize() {
		setPropertyMinSize(new Dimension2f(Vector2f.ZERO, Distance.PIXEL));
	}

	/**
	 * set the zoom property of the widget.
	 * @param newVal offset value.
	 */
	public void setOffset(final Vector2f newVal) {
		LOGGER.trace("Set offset: {}", newVal);
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
	public void setOrigin(final Vector2f pos) {
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

	public void setPropertyExpand(final Vector2b value) {
		if (this.propertyExpand.equals(value)) {
			return;
		}
		this.propertyExpand = value;
		markToRedraw();
		requestUpdateSize();
	}

	public void setPropertyExpandIfFree(final Vector2b value) {
		if (this.propertyExpandIfFree.equals(value)) {
			return;
		}
		this.propertyExpandIfFree = value;
		markToRedraw();
		requestUpdateSize();
	}

	public void setPropertyFill(final Vector2b value) {
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

	public void setPropertyMaxSize(final Dimension2f value) {
		if (this.propertyMaxSize.equals(value)) {
			return;
		}
		this.propertyMaxSize = value;
		onUpdateMinMaxSize();
	}

	public void setPropertyMinSize(final Dimension2f value) {
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
	public void setSize(final Vector2f value) {
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
			LOGGER.error("try to add shortcut with no descriptive string");
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
		final Vector2f displayOrigin = this.origin.add(this.offset);

		// check if the element is displayable in the windows :
		if (displayProp.windowsSize().x() < this.origin.x() || displayProp.windowsSize().y() < this.origin.y()) {
			// out of the windows  == > nothing to display ...
			return;
		}

		final DrawProperty tmpSize = displayProp.withLimit(this.origin, this.size);
		if (tmpSize.size().x() <= 0 || tmpSize.size().y() <= 0) {
			return;
		}
		OpenGL.setViewPort(tmpSize.origin(), tmpSize.size());
		// special case, when origin < display origin, we need to cut the display
		Vector2i downOffset = new Vector2i((int) (this.origin.x() - tmpSize.origin().x()),
				(int) (this.origin.y() - tmpSize.origin().y()));
		downOffset = Vector2i.min(downOffset, Vector2i.ZERO);
		// translate the display to have a GUI 0,0 position on the left bottom corner
		final Matrix4f tmpTranslate = Matrix4f
				.createMatrixTranslate((new Vector3f(-tmpSize.size().x() / 2 + this.offset.x() + downOffset.x(),
						-tmpSize.size().y() / 2 + this.offset.y() + downOffset.y(), -1.0f)).clipInteger());
		final Matrix4f tmpScale = Matrix4f.createMatrixScale(this.zoom, this.zoom, 1.0f);
		final Matrix4f tmpProjection = Matrix4f.createMatrixOrtho(-tmpSize.size().x() / 2, tmpSize.size().x() / 2,
				-tmpSize.size().y() / 2, tmpSize.size().y() / 2, -500, 500);
		
		OpenGL.push();
		OpenGL.setMatrix(tmpProjection);
		OpenGL.setCameraMatrix(tmpScale.multiply(tmpTranslate));
		if (this.drawDebugBorder != null) {
			this.drawDebugBorder.draw();
		}
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
			this.drawDebugBorder.lineRel(this.size.x() - 2, 0);
			this.drawDebugBorder.setPos(this.size.x() - 1, 1);
			this.drawDebugBorder.lineRel(0, this.size.y() - 2);
			this.drawDebugBorder.setPos(this.size.x() - 1, this.size.y() - 1);
			this.drawDebugBorder.lineRel(-this.size.x() - 2, 0);
			this.drawDebugBorder.setPos(1, this.size.y() - 1);
			this.drawDebugBorder.lineRel(0, -this.size.y() - 2);
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

	/**
	 * Grab events for drag operations. While grabbed, leave/enter events
	 * are suppressed for this widget during active drag. The cursor still
	 * moves freely. The grab is automatically released on mouse button UP.
	 *
	 * Unlike {@link #grabCursor()} which locks the cursor for FPS-style
	 * relative movement, this only suppresses leave/enter events to allow
	 * smooth drag operations when the cursor exits the widget bounds.
	 */
	public void grabEvents() {
		EwolObject.getContext().inputEventGrabEvents(this);
	}

	/**
	 * Release the event grab. Leave/enter events resume normal behavior.
	 */
	public void unGrabEvents() {
		EwolObject.getContext().inputEventUnGrabEvents();
	}

	// ========================================================================
	// Fluent API methods
	// ========================================================================
	
	/**
	 * Fluent method to set expand property.
	 * @param x expand horizontally
	 * @param y expand vertically
	 * @return this widget for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Widget> T expand(final boolean x, final boolean y) {
		setPropertyExpand(new Vector2b(x, y));
		return (T) this;
	}
	
	/**
	 * Fluent method to set horizontal expand only.
	 * @param expand true to expand horizontally
	 * @return this widget for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Widget> T expandX(final boolean expand) {
		setPropertyExpand(new Vector2b(expand, this.propertyExpand.y()));
		return (T) this;
	}
	
	/**
	 * Fluent method to set vertical expand only.
	 * @param expand true to expand vertically
	 * @return this widget for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Widget> T expandY(final boolean expand) {
		setPropertyExpand(new Vector2b(this.propertyExpand.x(), expand));
		return (T) this;
	}
	
	/**
	 * Fluent method to set fill property.
	 * @param x fill horizontally
	 * @param y fill vertically
	 * @return this widget for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Widget> T fill(final boolean x, final boolean y) {
		setPropertyFill(new Vector2b(x, y));
		return (T) this;
	}
	
	/**
	 * Fluent method to set horizontal fill only.
	 * @param fill true to fill horizontally
	 * @return this widget for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Widget> T fillX(final boolean fill) {
		setPropertyFill(new Vector2b(fill, this.propertyFill.y()));
		return (T) this;
	}
	
	/**
	 * Fluent method to set vertical fill only.
	 * @param fill true to fill vertically
	 * @return this widget for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Widget> T fillY(final boolean fill) {
		setPropertyFill(new Vector2b(this.propertyFill.x(), fill));
		return (T) this;
	}
	
	/**
	 * Fluent method to set expand-if-free property.
	 * @param x expand horizontally if free space
	 * @param y expand vertically if free space
	 * @return this widget for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Widget> T expandIfFree(final boolean x, final boolean y) {
		setPropertyExpandIfFree(new Vector2b(x, y));
		return (T) this;
	}
	
	/**
	 * Fluent method to set horizontal expand-if-free only.
	 * @param expand true to expand horizontally if free space
	 * @return this widget for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Widget> T expandIfFreeX(final boolean expand) {
		setPropertyExpandIfFree(new Vector2b(expand, this.propertyExpandIfFree.y()));
		return (T) this;
	}
	
	/**
	 * Fluent method to set vertical expand-if-free only.
	 * @param expand true to expand vertically if free space
	 * @return this widget for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Widget> T expandIfFreeY(final boolean expand) {
		setPropertyExpandIfFree(new Vector2b(this.propertyExpandIfFree.x(), expand));
		return (T) this;
	}
	
	/**
	 * Fluent method to set gravity.
	 * @param gravity the gravity
	 * @return this widget for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Widget> T gravity(final Gravity gravity) {
		setPropertyGravity(gravity);
		return (T) this;
	}
	
	/**
	 * Fluent method to hide the widget.
	 * @param hidden true to hide
	 * @return this widget for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Widget> T hide(final boolean hidden) {
		setPropertyHide(hidden);
		return (T) this;
	}
	
	/**
	 * Fluent method to set minimum size.
	 * @param size the minimum size
	 * @return this widget for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Widget> T minSize(final Dimension2f size) {
		setPropertyMinSize(size);
		return (T) this;
	}
	
	/**
	 * Fluent method to set minimum size in pixels.
	 * @param x minimum width in pixels
	 * @param y minimum height in pixels
	 * @return this widget for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Widget> T minSizePixel(final float x, final float y) {
		setPropertyMinSize(new Dimension2f(new Vector2f(x, y), Distance.PIXEL));
		return (T) this;
	}
	
	/**
	 * Fluent method to set minimum width in pixels only.
	 * @param x minimum width in pixels
	 * @return this widget for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Widget> T minSizePixelX(final float x) {
		final Vector2f current = this.propertyMinSize.getPixel();
		setPropertyMinSize(new Dimension2f(new Vector2f(x, current.y()), Distance.PIXEL));
		return (T) this;
	}
	
	/**
	 * Fluent method to set minimum height in pixels only.
	 * @param y minimum height in pixels
	 * @return this widget for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Widget> T minSizePixelY(final float y) {
		final Vector2f current = this.propertyMinSize.getPixel();
		setPropertyMinSize(new Dimension2f(new Vector2f(current.x(), y), Distance.PIXEL));
		return (T) this;
	}
	
	/**
	 * Fluent method to set maximum size.
	 * @param size the maximum size
	 * @return this widget for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Widget> T maxSize(final Dimension2f size) {
		setPropertyMaxSize(size);
		return (T) this;
	}
	
	/**
	 * Fluent method to set maximum size in pixels.
	 * @param x maximum width in pixels
	 * @param y maximum height in pixels
	 * @return this widget for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Widget> T maxSizePixel(final float x, final float y) {
		setPropertyMaxSize(new Dimension2f(new Vector2f(x, y), Distance.PIXEL));
		return (T) this;
	}
	
	/**
	 * Fluent method to set maximum width in pixels only.
	 * @param x maximum width in pixels
	 * @return this widget for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Widget> T maxSizePixelX(final float x) {
		final Vector2f current = this.propertyMaxSize.getPixel();
		setPropertyMaxSize(new Dimension2f(new Vector2f(x, current.y()), Distance.PIXEL));
		return (T) this;
	}
	
	/**
	 * Fluent method to set maximum height in pixels only.
	 * @param y maximum height in pixels
	 * @return this widget for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Widget> T maxSizePixelY(final float y) {
		final Vector2f current = this.propertyMaxSize.getPixel();
		setPropertyMaxSize(new Dimension2f(new Vector2f(current.x(), y), Distance.PIXEL));
		return (T) this;
	}
	
	/**
	 * Fluent method to enable focus.
	 * @param canFocus true to enable focus
	 * @return this widget for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Widget> T canFocus(final boolean canFocus) {
		setPropertyCanFocus(canFocus);
		return (T) this;
	}
}
