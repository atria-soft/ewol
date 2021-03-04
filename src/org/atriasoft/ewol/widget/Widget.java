
/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.esignal.Signal;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.annotation.EwolDescription;
import org.atriasoft.ewol.annotation.EwolSignal;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.event.InputSystem;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.object.EwolObject;
import org.atriasoft.exml.annotation.XmlDefaultManaged;
import org.atriasoft.exml.annotation.XmlManaged;
import org.atriasoft.exml.annotation.XmlName;
import org.atriasoft.exml.annotation.XmlProperty;
import org.atriasoft.gale.Dimension;
import org.atriasoft.gale.Distance;
import org.atriasoft.gale.key.KeyKeyboard;
import org.atriasoft.gale.key.KeySpecial;
import org.atriasoft.gale.context.ClipboardList;
import org.atriasoft.gale.context.Cursor;
import org.atriasoft.gale.event.EventEntry;

// TODO: change position of this ...
class EventShortCut {
	public final String message; //!< data link with the event
	public final KeySpecial specialKey; //!< special board key
	public final Character unicodeValue; //!< 0 if not used
	public final KeyKeyboard keyboardMoveValue; //!< ewol::EVENT_KB_MOVE_TYPE_NONE if not used
	public final boolean isActive; //!< If true, we need to filter the up key of ascii element (not control)
	
	public EventShortCut(final String message, final KeySpecial specialKey, final Character unicodeValue, final KeyKeyboard keyboardMoveValue, final boolean isActive) {
		super();
		this.message = message;
		this.specialKey = specialKey;
		this.unicodeValue = unicodeValue;
		this.keyboardMoveValue = keyboardMoveValue;
		this.isActive = isActive;
	}
};

/**
 * @brief Widget class is the main widget interface, it has so me generic properties: 
 * :** known his parent
 * :** Can be display at a special position with a special scale
 * :** Can get focus
 * :** Receive Event (keyboard / mouse / ...)
 * 
 */
@XmlDefaultManaged(value = false)
class Widget extends EwolObject {
	@XmlManaged()
	@XmlProperty()
	@XmlName(value = "min-size")
	@EwolDescription(value = "User minimum size")
	protected Dimension propertyMinSize = new Dimension(new Vector2f(0, 0), Distance.PIXEL); //!< user define the minimum size of the widget
	
	@XmlManaged()
	@XmlProperty()
	@XmlName(value = "max-size")
	@EwolDescription(value = "User maximum size")
	protected Dimension propertyMaxSize = new Dimension(new Vector2f(999999, 999999), Distance.PIXEL); //!< user define the maximum size of the widget
	
	@XmlManaged()
	@XmlProperty()
	@XmlName(value = "gravity")
	@EwolDescription(value = "Gravity orientation")
	protected Gravity propertyGravity = Gravity.buttomLeft; //!< Gravity of the widget
	
	@XmlManaged()
	@XmlProperty()
	@XmlName(value = "focus")
	@EwolDescription(value = "enable the widget to have the focus capacity")
	protected boolean propertyCanFocus = false; //!< the focus can be done on this widget
	
	@XmlManaged()
	@XmlProperty()
	@XmlName(value = "expand")
	@EwolDescription(value = "Request the widget Expand size while space is available")
	Vector2b propertyExpand = new Vector2b(false, false); //!< the widget will expand if possible
	
	@XmlManaged()
	@XmlProperty()
	@XmlName(value = "fill")
	@EwolDescription(value = "Fill the widget available size")
	Vector2b propertyFill = new Vector2b(true, true); //!< the widget will fill all the space provided by the parent.
	
	@XmlManaged()
	@XmlProperty()
	@XmlName(value = "hide")
	@EwolDescription(value = "The widget start hided")
	boolean propertyHide = false; //!< hide a widget on the display
	
	// ----------------------------------------------------------------------------------------------------------------
	// -- Widget size:
	// ----------------------------------------------------------------------------------------------------------------
	protected Vector2f size = new Vector2f(10, 10); //!< internal: current size of the widget
	protected Vector2f minSize = new Vector2f(0, 0); //!< internal: minimum size of the widget
	protected Vector2f maxSize = new Vector2f(999999, 999999); //!< internal: maximum size of the widget
	
	/**
	 * @brief Constructor of the widget classes
	 * @return (no exception generated (not managed in embedded platform))
	 */
	public Widget() {}
	
	/**
	 * @brief Convert the absolute position in the local Position (Relative)
	 * @param[in] _pos Absolute position that you request conversion.
	 * @return The relative position.
	 */
	public Vector2f relativePosition(final Vector2f _pos) {
		return _pos.lessNew(this.origin);
	}
	
	/**
	 * @brief Parent have set the size and the origin. The container need to update the child widget property
	 * @note INTERNAL EWOL SYSTEM
	 */
	public void onChangeSize() {
		Log.verbose("[" + getId() + "] {" + getObjectType() + "} update size : " + this.size);
		markToRedraw();
	}
	
	public void calculateSize() {};
	
	/**
	 * @brief get the widget size
	 * @return Requested size
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public Vector2f getSize() {
		if (this.propertyHide == false) {
			return this.size;
		}
		return new Vector2f(0, 0);
	}
	
	/**
	 * @brief set the widget size
	 * @return Requested size
	 * @note : INTERNAL EWOL SYSTEM Do not modify the size yourself: calculation is complex and need knowledge of around widget
	 */
	public void setSize(final Vector2f _value) {
		this.size = _value;
	}
	
	/**
	 * @brief calculate the minimum and maximum size (need to estimate expend properties of the widget)
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public void calculateMinMaxSize() {
		this.minSize = this.propertyMinSize.getPixel();
		//Log.error("[" + getId() + "] convert in min size : " + propertyMinSize + " out=" + this.minSize);
		this.maxSize = this.propertyMaxSize.getPixel();
		markToRedraw();
	}
	
	/**
	 * @brief get the widget minimum size calculated
	 * @return Requested size
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public Vector2f getCalculateMinSize() {
		if (this.propertyHide == false) {
			return this.minSize;
		}
		return new Vector2f(0, 0);
	}
	
	/**
	 * @brief get the widget maximum size calculated
	 * @return Requested size
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public Vector2f getCalculateMaxSize() {
		if (this.propertyHide == false) {
			return this.maxSize;
		}
		return new Vector2f(999999, 999999);
	}
	
	protected Vector2f offset = new Vector2f(0, 0); //!< Offset of the display in the view-port
	
	/**
	 * @brief set the zoom property of the widget.
	 * @param[in] _newVal offset value.
	 */
	public void setOffset(final Vector2f _newVal) {
		Log.info("Set offset: " + _newVal);
		if (this.offset != _newVal) {
			this.offset = _newVal;
			markToRedraw();
		}
	}
	
	/**
	 * @brief get the offset property of the widget.
	 * @return The current offset value.
	 */
	Vector2f getOffset() {
		return this.offset;
	};
	
	// internal element calculated by the system
	protected float zoom = 1.0f; //!< generic widget zoom
	
	/**
	 * @brief set the zoom property of the widget
	 * @param[in] _newVal newZoom value
	 */
	public void setZoom(final float _newVal) {
		if (this.zoom == _newVal) {
			return;
		}
		this.zoom = FMath.avg(0.0000001f, _newVal, 1000000.0f);
		markToRedraw();
	}
	
	/**
	 * @brief get the zoom property of the widget
	 * @return the current zoom value
	 */
	public float getZoom() {
		return this.zoom;
	}
	
	/**
	 * @brief Change Zoom property.
	 * @param[in] _range Range of the zoom change.
	 */
	void changeZoom(final float _range) {
		
	};
	
	protected Vector2f origin = new Vector2f(0, 0); //!< internal ... I do not really known how if can use it ...
	
	/**
	 * @brief Set origin at the widget (must be an parent widget that set this parameter).
	 * This represent the absolute origin in the program windows.
	 * @param[in] _pos Position of the origin.
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public void setOrigin(final Vector2f _pos) {
		this.origin = _pos;
	}
	
	/**
	 * @brief Get the origin (absolute position in the windows).
	 * @return Coordinate of the origin requested.
	 */
	public Vector2f getOrigin() {
		return this.origin;
	}
	
	/**
	 * @brief User set No minimum size.
	 */
	public void setNoMinSize() {
		this.propertyMinSize.set(new Dimension(new Vector2f(0, 0), Distance.PIXEL));
	}
	
	/**
	 * @brief Check if the current min size is compatible with the user minimum size
	 *        If it is not the user minimum size will overWrite the minimum size set.
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public void checkMinSize() {
		final Vector2f pixelSize = this.propertyMinSize.getPixel();
		this.minSize.setX(FMath.max(this.minSize.x, pixelSize.x));
		this.minSize.setY(FMath.max(this.minSize.y, pixelSize.y));
	}
	
	/**
	 * @brief User set No maximum size.
	 */
	public void setNoMaxSize() {
		this.propertyMaxSize.set(new Dimension(new Vector2f(999999, 999999), Distance.PIXEL));
	}
	
	/**
	 * @brief Check if the current max size is compatible with the user maximum size
	 *        If it is not the user maximum size will overWrite the maximum size set.
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public void checkMaxSize() {
		final Vector2f pixelSize = this.propertyMaxSize.getPixel();
		this.maxSize.setX(FMath.min(this.maxSize.x, pixelSize.x));
		this.maxSize.setY(FMath.min(this.maxSize.y, pixelSize.y));
	}
	
	/**
	 * @brief get the expend capabilities (xy)
	 * @return 2D boolean represents the capacity to expend
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public Vector2b canExpand() {
		if (this.propertyHide == false) {
			return this.propertyExpand;
		}
		return new Vector2b(false, false);
	}
	
	/**
	 * @brief get the filling capabilities xy
	 * @return Vector2b repensent the capacity to xy filling
	 * @note : INTERNAL EWOL SYSTEM
	 */
	public Vector2b canFill() {
		return this.propertyFill;
	}
	
	// ----------------------------------------------------------------------------------------------------------------
	// -- focus Area
	// ----------------------------------------------------------------------------------------------------------------
	private final boolean hasFocus = false; //!< set the focus on this widget
	
	/**
	 * @brief get the focus state of the widget
	 * @return focus state
	 */
	public boolean getFocus() {
		return this.hasFocus;
	};
	
	/**
	 * @brief set focus on this widget
	 * @return return true if the widget keep the focus
	 */
	public boolean setFocus() {
		Log.verbose("set focus (start) *propertyCanFocus=" + this.propertyCanFocus + " this.hasFocus=" + this.hasFocus);
		if (this.propertyCanFocus == true) {
			if (this.hasFocus == false) {
				this.hasFocus = true;
				onGetFocus();
			}
			Log.verbose("set focus (stop) ret true");
			return true;
		}
		Log.verbose("set focus (stop) ret false");
		return false;
	}
	
	/**
	 * @brief remove the focus on this widget
	 * @return return true if the widget have release his focus (if he has it)
	 */
	public boolean rmFocus() {
		if (this.propertyCanFocus == true) {
			if (this.hasFocus == true) {
				this.hasFocus = false;
				onLostFocus();
			}
			return true;
		}
		return false;
	}
	
	/**
	 * @brief keep the focus on this widget  == > this remove the previous focus on all other widget
	 */
	public void keepFocus() {
		getWidgetManager().focusKeep(this);
	}
	
	/**
	 * @brief Event of the focus has been grabed by the current widget
	 */
	protected void onGetFocus() {};
	
	/**
	 * @brief Event of the focus has been lost by the current widget
	 */
	protected void onLostFocus() {};
	
	// ----------------------------------------------------------------------------------------------------------------
	// -- Mouse event properties Area
	// ----------------------------------------------------------------------------------------------------------------
	private int limitMouseEvent = 3; //!< this is to limit the number of mouse event that the widget can supported
	
	/**
	 * @brief get the number of mouse event supported
	 * @return return the number of event that the mouse supported [0..3]
	 */
	public int getMouseLimit() {
		return this.limitMouseEvent;
	};
	
	/**
	 * @brief get the number of mouse event supported
	 * @param[in] _numberState The number of event that the mouse supported [0..3]
	 */
	public void setMouseLimit(final int _numberState) {
		this.limitMouseEvent = _numberState;
	};
	
	// ----------------------------------------------------------------------------------------------------------------
	// -- keyboard event properties Area
	// ----------------------------------------------------------------------------------------------------------------
	private boolean allowRepeatKeyboardEvent = true; //!< This remove the repeating keybord event due to the ant pressing key.
	
	/**
	 * @brief get the keyboard repeating event supporting.
	 * @return true : the event can be repeated.
	 * @return false : the event must not be repeated.
	 */
	public boolean getKeyboardRepeat() {
		return this.allowRepeatKeyboardEvent;
	};
	
	/**
	 * @brief set the keyboard repeating event supporting.
	 * @param[in] _state The repeating status (true: enable, false disable).
	 */
	protected void setKeyboardRepeat(final boolean _state) {
		this.allowRepeatKeyboardEvent = _state;
	};
	
	/**
	 * @brief display the  keyboard (if needed)
	 */
	protected void showKeyboard() {
		getContext().keyboardShow();
	}
	
	/**
	 * @brief Hide the  keyboard (if needed)
	 */
	protected void hideKeyboard() {
		getContext().keyboardHide();
	}
	
	/**
			 * @brief get the widget at the specific windows absolute position
			 * @param[in] _pos gAbsolute position of the requested widget knowledge
			 * @return null No widget found
			 * @return pointer on the widget found
			 * @note : INTERNAL EWOL SYSTEM
			 */
	public Widget getWidgetAtPos(final Vector2f _pos) {
		if (this.propertyHide == false) {
			return this;
		}
		return null;
	}
	
	// event section:
	/**
			 * @brief {SYSTEM} system event input (only meta widget might overwrite this function).
			 * @param[in] _event Event properties
			 * @return true the event is used
			 * @return false the event is not used
			 */
	public boolean systemEventInput(final InputSystem _event) {
		final Widget up = (Widget) this.parent.get();
		if (up != null) {
			if (up.systemEventInput(_event) == true) {
				return true;
			}
		}
		return onEventInput(_event.event);
	}
	
	/**
	 * @brief Event on an input of this Widget (finger, mouse, stylet)
	 * @param[in] _event Event properties
	 * @return true the event is used
	 * @return false the event is not used
	 */
	protected boolean onEventInput(final EventInput _event) {
		return false;
	};
	
	/**
	 * @brief {SYSTEM} Entry event (only meta widget might overwrite this function).
	 * @param[in] _event Event properties
	 * @return true if the event has been used
	 * @return false if the event has not been used
	 */
	public boolean systemEventEntry(final EntrySystem _event) {
		final Widget up = (Widget) this.parent.get();
		if (up != null) {
			if (up.systemEventEntry(_event) == true) {
				return true;
			}
		}
		return onEventEntry(_event.event);
	}
	
	/**
	 * @brief Entry event.
	 *        represent the physical event :
	 *            - Keyboard (key event and move event)
	 *            - Accelerometer
	 *            - Joystick
	 * @param[in] _event Event properties
	 * @return true if the event has been used
	 * @return false if the event has not been used
	 */
	protected boolean onEventEntry(final EventEntry _event) {
		return false;
	};
	
	/**
	 * @brief Event on a past event  == > this event is asynchronous due to all system does not support direct getting data.
	 * @note : need to have focus ...
	 * @param[in] mode Mode of data requested
	 */
	public void onEventClipboard(final ClipboardList _clipboardID) {};
	
	// ----------------------------------------------------------------------------------------------------------------
	// -- Shortcut : management of the shortcut
	// ----------------------------------------------------------------------------------------------------------------
	@EwolSignal(name = "shortcut")
	public Signal<String> signalShortcut; //!< signal handle of the message
	private List<EventShortCut> localShortcut; //!< list of all shortcut in the widget
	
	/**
	 * @brief add a specific shortcut with his description
	 * @param[in] _descriptiveString Description string of the shortcut
	 * @param[in] _message massage to generate (or shortcut name)
	 */
	protected void shortCutAdd(final String _descriptiveString) {
		shortCutAdd(_descriptiveString, "");
	}
	
	protected void shortCutAdd( final String _descriptiveString,
			                          final String _message){
			if (_descriptiveString.size() == 0) {
				Log.error("try to add shortcut with no descriptive string ...");
				return;
			}
			final EventShortCut tmpElement;
			if (_message.size() == 0) {
				tmpElement.message = _descriptiveString;
			} else {
				tmpElement.message = _message;
			}
			// parsing of the string:
			//"ctrl+shift+alt+meta+s"
			if(_descriptiveString.find("ctrl") != String::npos) {
				tmpElement.specialKey.setCtrlLeft(true);
			}
			if(_descriptiveString.find("shift") != String::npos) {
				tmpElement.specialKey.setShiftLeft(true);
			}
			if(_descriptiveString.find("alt") != String::npos) {
				tmpElement.specialKey.setAltLeft(true);
			}
			if(_descriptiveString.find("meta") != String::npos) {
				tmpElement.specialKey.setMetaLeft(true);
			}
			if(_descriptiveString.find("F12") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::f12;
			} else if(_descriptiveString.find("F11") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::f11;
			} else if(_descriptiveString.find("F10") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::f10;
			} else if(_descriptiveString.find("F9") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::f9;
			} else if(_descriptiveString.find("F8") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::f8;
			} else if(_descriptiveString.find("F7") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::f7;
			} else if(_descriptiveString.find("F6") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::f6;
			} else if(_descriptiveString.find("F5") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::f5;
			} else if(_descriptiveString.find("F4") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::f4;
			} else if(_descriptiveString.find("F3") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::f3;
			} else if(_descriptiveString.find("F2") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::f2;
			} else if(_descriptiveString.find("F1") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::f1;
			} else if(_descriptiveString.find("LEFT") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::left;
			} else if(_descriptiveString.find("RIGHT") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::right;
			} else if(_descriptiveString.find("UP") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::up;
			} else if(_descriptiveString.find("DOWN") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::down;
			} else if(_descriptiveString.find("PAGE_UP") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::pageUp;
			} else if(_descriptiveString.find("PAGE_DOWN") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::pageDown;
			} else if(_descriptiveString.find("START") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::start;
			} else if(_descriptiveString.find("END") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::end;
			} else if(_descriptiveString.find("PRINT") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::print;
			} else if(_descriptiveString.find("ARRET_DEFIL") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::stopDefil;
			} else if(_descriptiveString.find("WAIT") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::wait;
			} else if(_descriptiveString.find("INSERT") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::insert;
			} else if(_descriptiveString.find("CAPLOCK") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::capLock;
			} else if(_descriptiveString.find("CONTEXT_MENU") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::contextMenu;
			} else if(_descriptiveString.find("NUM_LOCK") != String::npos) {
				tmpElement.keyboardMoveValue = KeyKeyboard::numLock;
			} else {
				tmpElement.unicodeValue = _descriptiveString[_descriptiveString.size() -1];
			}
			// add it on the List ...
			this.localShortcut.pushBack(etk::move(tmpElement));
		}
	
	/**
	 * @brief remove all current shortCut
	 */
	protected void shortCutClean() {
		this.localShortcut.clear();
	}
	
	/**
			 * @brief remove a specific shortCut with his event name
			 * @param[in] _message generated event name
			 */
		protected void shortCutRemove( final String _message){
	
	auto it(this.localShortcut.begin());
			while(it != this.localShortcut.end()) {
				if (it.message != _message) {
					++it;
					continue;
				}
				this.localShortcut.erase(it);
				it = this.localShortcut.begin();
			}
		}
	
	/**
	 * @brief Event on a short-cut of this Widget (in case of return false, the event on the keyevent will arrive in the function @ref onEventKb).
	 * @param[in] _special All the special kay pressed at this time.
	 * @param[in] _unicodeValue Key pressed by the user not used if the kbMove!=ewol::EVENT_KB_MOVE_TYPE_NONE.
	 * @param[in] _kbMove Special key of the keyboard.
	 * @return true if the event has been used.
	 * @return false if the event has not been used.
	 * @note To prevent some error when you get an event get it if it is down and Up ...  ==> like this it could not generate some mistake in the error.
	 */
	public boolean onEventShortCut(final KeySpecial _special, Character _unicodeValue, final KeyKeyboard _kbMove, final boolean _isDown) {
		if (_unicodeValue >= 'A' && _unicodeValue <= 'Z') {
			_unicodeValue += 'a' - 'A';
		}
		Log.verbose("check shortcut...." + _special + " " + _unicodeValue + " " + _kbMove + " " + (_isDown ? "DOWN" : "UP") + " nb shortcut:" << this.localShortcut.size());
		// Remove the up event of the shortcut...
		if (_isDown == false) {
			for (int iii = this.localShortcut.size() - 1; iii >= 0; iii--) {
				if (this.localShortcut[iii].isActive == false) {
					continue;
				}
				if ((this.localShortcut[iii].keyboardMoveValue == KeyKeyboard::unknow && this.localShortcut[iii].unicodeValue == _unicodeValue)
						|| (this.localShortcut[iii].keyboardMoveValue == _kbMove && this.localShortcut[iii].unicodeValue == 0)) {
					// In this case we grap the event in case of an error can occured ...
					this.localShortcut[iii].isActive = false;
					Log.verbose("detect up of a shortcut");
					return true;
				}
			}
		}
		//Log.info("Try to find generic shortcut ...");
		for (int iii = this.localShortcut.size() - 1; iii >= 0; iii--) {
			if (this.localShortcut[iii].specialKey.getShift() == _special.getShift() && this.localShortcut[iii].specialKey.getCtrl() == _special.getCtrl()
					&& this.localShortcut[iii].specialKey.getAlt() == _special.getAlt() && this.localShortcut[iii].specialKey.getMeta() == _special.getMeta()
					&& ((this.localShortcut[iii].keyboardMoveValue == KeyKeyboard::unknow && this.localShortcut[iii].unicodeValue == _unicodeValue)
							|| (this.localShortcut[iii].keyboardMoveValue == _kbMove && this.localShortcut[iii].unicodeValue == 0))) {
				if (_isDown == true) {
					this.localShortcut[iii].isActive = true;
					Log.verbose("Generate shortCut: " + this.localShortcut[iii].message);
					this.signalShortcut.emit(this.localShortcut[iii].message);
				}
				return true;
			}
		}
		return false;
	}
	
	// ----------------------------------------------------------------------------------------------------------------
	// -- drawing : All drawing must be done in 2 separate buffer 1 for the current display and 1 for the working...
	// ----------------------------------------------------------------------------------------------------------------
	protected boolean needRegenerateDisplay = true; //!< the display might be done the next regeneration
	
	/**
	 * @brief The widget mark itself that it need to regenerate the nest time.
	 */
	public void markToRedraw() {
		if (this.needRegenerateDisplay == true) {
			return;
		}
		this.needRegenerateDisplay = true;
		getWidgetManager().markDrawingIsNeeded();
	}
	
	/**
	 * @brief get the need of the redrawing of the widget and reset it to false
	 * @return true if we need to redraw
	 * @return false if we have no need to redraw
	 */
	protected boolean needRedraw() {
		final boolean tmpData = this.needRegenerateDisplay;
		this.needRegenerateDisplay = false;
		return tmpData;
	};
	
	/**
			 * @brief {SYSTEM} extern interface to request a draw ...  (called by the drawing thread [Android, X11, ...])
			 * This function generate a clipping with the view-port openGL system. Like this a widget draw can not draw over an other widget
			 * @note This function is  for the scrolled widget, and the more complicated openGL widget
			 * @param[in] _displayProp properties of the current display
			 * @note : INTERNAL EWOL SYSTEM
			                                                              /-. _displayProp.this.windowsSize
			      *------------------------------------------------------*
			      |                                                      |
			      |                                              this.size  |
			      |                                                 /    |
			      |                        *-----------------------*     |
			      |                        '                       '     |
			      |                        '   _displayProp.this.size '     |
			      |              Viewport  '          /            '     |
			      |              o---------'---------o             '     |
			      |              |         '         |             '     |
			      |              |         '         |             '     |
			      |              |         '         |             '     |
			      |              |         '         |             '     |
			      |              |         *-----------------------*     |
			      |              |        /          |                   |
			      |              |   this.offset        |                   |
			      |              |                   |                   |
			      |              o-------------------o                   |
			      |             /                                        |
			      |  _displayProp.this.origin                               |
			      |                                                      |
			      *------------------------------------------------------*
			     /
			   (0,0)
			 */
			public			 void systemDraw( final DrawProperty _displayProp){
				//Log.info("[" + getId() + "] Draw : [" + propertyName + "] t=" + getObjectType() + " o=" + this.origin + "  s=" << this.size << " hide=" << propertyHide);
				if (this.propertyHide == true){
					// widget is hidden ...
					return;
				}
				final Vector2f displayOrigin = this.origin + this.offset;
				
				// check if the element is displayable in the windows : 
				if(    _displayProp.this.windowsSize.x() < this.origin.x()
				    || _displayProp.this.windowsSize.y() < this.origin.y() ) {
					// out of the windows  == > nothing to display ...
					return;
				}
				
				final DrawProperty tmpSize = _displayProp.clone();
				tmpSize.limit(this.origin, this.size);
				if (tmpSize.this.size.x() <= 0 || tmpSize.this.size.y() <= 0) {
					return;
				}
				glViewport( (int)tmpSize.this.origin.x(),
				            (int)tmpSize.this.origin.y(),
				            (int)tmpSize.this.size.x(),
				            (int)tmpSize.this.size.y());
				// special case, when origin < display origin, we need to cut the display :
				final Vector2i downOffset = this.origin - tmpSize.this.origin;
				downOffset.setMin(Vector2i(0,0));
				
				final mat4 tmpTranslate = etk::matTranslate(Vector3fClipInt32(Vector3f(-tmpSize.this.size.x()/2+this.offset.x() + downOffset.x(),
				                                                         -tmpSize.this.size.y()/2+this.offset.y() + downOffset.y(),
				                                                         -1.0f)));
				final mat4 tmpScale = etk::matScale(Vector3f(this.zoom, this.zoom, 1.0f));
				final mat4 tmpProjection = etk::matOrtho((int)(-tmpSize.this.size.x())>>1,
				                                   (int)( tmpSize.this.size.x())>>1,
				                                   (int)(-tmpSize.this.size.y())>>1,
				                                   (int)( tmpSize.this.size.y())>>1,
				                                   (int)(-1),
				                                   (int)( 1));
				mat4 tmpMat = tmpProjection * tmpScale * tmpTranslate;
				
				gale::openGL::push();
				// set internal matrix system :
				gale::openGL::setMatrix(tmpMat);
				//long ___startTime = ewol::getTime();
				onDraw();
				gale::openGL::pop();
				return;
			}
	
	/**
	 * @brief Common widget drawing function (called by the drawing thread [Android, X11, ...])
	 */
	protected void onDraw() {};
	
	/**
	 * @brief Event generated when a redraw is needed
	 */
	public void onRegenerateDisplay() {};
	
	// grab cursor mode
	private boolean grabCursor = false;
	
	/**
			 * @brief Grab the cursor : This get all the movement of the mouse in PC mode, and generate an offset instead of a position.
			 * @note : the generation of the offset is due to the fact the cursor position is forced at the center of the widget.
			 * @note This done nothing in "Finger" or "Stylet" mode.
			 */
public			 void grabCursor()
if (this.grabCursor == false) {
	getContext().inputEventGrabPointer(ememory::dynamicPointerCast<Widget>(sharedFromThis()));
	this.grabCursor = true;
}
}
	
	/**
	 * @brief Un-Grab the cursor (default mode cursor offset)
	 */
	public void unGrabCursor() {
		if (this.grabCursor == true) {
			getContext().inputEventUnGrabPointer();
			this.grabCursor = false;
		}
	}
	
	/**
	 * @brief get the grabbing status of the cursor.
	 * @return true if the cursor is currently grabbed
	 */
	public boolean getGrabStatus() {
		return this.grabCursor;
	}
	
	private final Cursor cursorDisplay = Cursor.arrow;
	
	/**
			 * @brief set the cursor display type.
			 * @param[in] _newCursor selected new cursor.
			 */
		public			 void setCursor(enum gale::context::cursor _newCursor) {
			Log.debug("Change Cursor in " + _newCursor);
			this.cursorDisplay = _newCursor;
			getContext().setCursor(this.cursorDisplay);
		}
	
	/**
	 * @brief get the current cursor.
	 * @return the type of the cursor.
	 */
	public Cursor getCursor() {
		return this.cursorDisplay;
	}
	
	/*
	public boolean loadXML( XmlElement _node){
		EwolObject::loadXML(_node);
		markToRedraw();
		return true;
	}
	
	*/
	/**
	 * @brief Need to be call When the size of the current widget have change  ==> this force the system to recalculate all the widget positions.
	 */
	public void requestUpdateSize() {
		getContext().requestUpdateSize();
	}
	
	/**
	 * @brief Get the current Widget Manager.
	 */
	public WidgetManager getWidgetManager() {
		return getContext().getWidgetManager();
	}
	
	/**
	 * @brief Get the current Windows.
	 */
	public Windows getWindows() {
		return getContext().getWindows();
	}
	
	protected void onChangePropertyCanFocus() {
		if (this.hasFocus == true) {
			rmFocus();
		}
	}
	
	protected void onChangePropertyGravity() {
		markToRedraw();
		requestUpdateSize();
	}
	
	protected void onChangePropertyHide() {
		markToRedraw();
		requestUpdateSize();
	}
	
	protected void onChangePropertyFill() {
		markToRedraw();
		requestUpdateSize();
	}
	
	protected void onChangePropertyExpand() {
		requestUpdateSize();
		markToRedraw();
	}
	
	protected 	 void onChangePropertyMaxSize() {
					final Vector2f pixelMin = this.propertyMinSize.getPixel();
					final Vector2f pixelMax = this.propertyMaxSize.getPixel();
					// check minimum  maximum compatibility :
					boolean error=false;
					if (pixelMin.x()>pixelMax.x()) {
						error=true;
					}
					if (pixelMin.y()>pixelMax.y()) {
						error=true;
					}
					if (error == true) {
						Log.error("Can not set a 'min size' > 'max size' reset to maximum ...");
						this.propertyMaxSize.setDirect(gale::Dimension(Vector2f(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE),gale::distance::pixel));
					}
					requestUpdateSize();
				}
	
	protected 	 void onChangePropertyMinSize() {
					final Vector2f pixelMin = this.propertyMinSize.getPixel();
					final Vector2f pixelMax = this.propertyMaxSize.getPixel();
					// check minimum  maximum compatibility :
					boolean error=false;
					if (pixelMin.x()>pixelMax.x()) {
						error=true;
					}
					if (pixelMin.y()>pixelMax.y()) {
						error=true;
					}
					if (error == true) {
						Log.error("Can not set a 'min size' > 'max size' set nothing ...");
						this.propertyMinSize.setDirect(gale::Dimension(Vector2f(0,0),gale::distance::pixel));
					}
					requestUpdateSize();
				}
	
	public void drawWidgetTree(final int _level) {
		String space;
		for (int iii = 0; iii < _level; ++iii) {
			space += "    ";
		}
		Log.print(space + "[" + getId() + "] name='" + propertyName + "' type=" + getObjectType() + " o=" + this.origin << "  s=" << this.size << " hide=" << this.propertyHide);
	}
};};