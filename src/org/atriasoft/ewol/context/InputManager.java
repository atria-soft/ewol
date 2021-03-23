/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.context;

import java.lang.ref.WeakReference;

import org.atriasoft.echrono.Clock;
import org.atriasoft.echrono.Duration;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.event.InputSystem;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.ewol.widget.Windows;
import org.atriasoft.gale.key.KeySpecial;
import org.atriasoft.gale.key.KeyStatus;
import org.atriasoft.gale.key.KeyType;

/**
 * internal structure
 */
class InputLimit {
	public Duration sepatateTime;
	public int DpiOffset;
};

class InputManager {
	private final static int MAX_MANAGE_INPUT = 15;
	
	// special grab pointer mode : 
	private WeakReference<Widget> grabWidget = null; //!< widget that grab the curent pointer.
	private int dpi;
	private InputLimit eventInputLimit;
	private InputLimit eventMouseLimit;
	private final InputPoperty[] eventInputSaved = new InputPoperty[MAX_MANAGE_INPUT];
	private final InputPoperty[] eventMouseSaved = new InputPoperty[MAX_MANAGE_INPUT];
	
	private final EwolContext context;
	private KeySpecial specialKey;
	
	public InputManager(final EwolContext _context) {
		this.context = _context;
		setDpi(200);
		Log.info("Init (start)");
		for (int iii = 0; iii < MAX_MANAGE_INPUT; iii++) {
			// remove the property of this input ...
			cleanElement(this.eventInputSaved, iii);
			cleanElement(this.eventMouseSaved, iii);
		}
		Log.info("Init (end)");
	}
	
	public void abortElement(final InputPoperty[] _eventTable, final int _idInput, final KeyType _type) {
		if (_eventTable == null) {
			return;
		}
		if (_eventTable[_idInput].isUsed == true) {
			localEventInput(_type, _eventTable[_idInput].curentWidgetEvent.get(), _eventTable[_idInput].destinationInputId, KeyStatus.abort, _eventTable[_idInput].posEvent);
		}
	}
	
	private void calculateLimit() {
		this.eventInputLimit.sepatateTime = Duration.milliseconds(300);
		this.eventInputLimit.DpiOffset = this.dpi * 100;
		this.eventMouseLimit.sepatateTime = Duration.milliseconds(300);
		this.eventMouseLimit.DpiOffset = (int) (this.dpi * 0.1f);
	}
	
	public void cleanElement(final InputPoperty[] eventMouseSaved2, final int _idInput) {
		if (eventMouseSaved2 == null) {
			return;
		}
		//Log.info("CleanElement[" + idInput + "] = @" + (long)eventTable);
		eventMouseSaved2[_idInput].isUsed = false;
		eventMouseSaved2[_idInput].destinationInputId = 0;
		eventMouseSaved2[_idInput].lastTimeEvent = new Clock();
		eventMouseSaved2[_idInput].curentWidgetEvent = null;
		eventMouseSaved2[_idInput].origin = new Vector2f(0, 0);
		eventMouseSaved2[_idInput].size = new Vector2f(99999999, 99999999);
		eventMouseSaved2[_idInput].downStart = new Vector2f(0, 0);
		eventMouseSaved2[_idInput].isDown = false;
		eventMouseSaved2[_idInput].isInside = false;
		eventMouseSaved2[_idInput].nbClickEvent = 0;
		eventMouseSaved2[_idInput].posEvent = new Vector2f(0, 0);
	}
	
	/**
	 * This fonction lock the pointer properties to move in relative instead of absolute
	 * @param _widget The widget that lock the pointer events
	 */
	public void grabPointer(final Widget _widget) {
		if (_widget == null) {
			return;
		}
		this.grabWidget = new WeakReference<>(_widget);
		/* TODO : 
		this.context.grabPointerEvents(true,   _widget.getOrigin()
		                                  + Vector2i(_widget.getSize().x/2.0f,
		                                          _widget.getSize().y/2.0f) );
		*/
	}
	
	/**
	 * generate the event on the destinated widget.
	 * @param _type Type of the event that might be sended.
	 * @param _destWidget Pointer on the requested widget that element might be sended
	 * @param _IdInput Id of the event (PC : [0..9] and touch : [1..9])
	 * @param _typeEvent type of the eventg generated
	 * @param _pos position of the event
	 * @return true if event has been greped
	 */
	public boolean localEventInput(final KeyType _type, final Widget _destWidget, final int _IdInput, final KeyStatus _status, final Vector2f _pos) {
		if (_destWidget != null) {
			if (_type == KeyType.mouse || _type == KeyType.finger) {
				// create the system Event :
				// TODO : set the real ID ...
				final InputSystem tmpEventSystem = new InputSystem(_type, _status, _IdInput, _pos, _destWidget, 0, this.specialKey);
				// generate the event :
				return _destWidget.systemEventInput(tmpEventSystem);
			} else {
				return false;
			}
		}
		return false;
	}
	
	/**
	 * convert the system event id in the correct EWOL id depending of the system management mode
	 *        This function find the next input id unused on the specifiic widget
	 *             == > on PC, the ID does not change (GUI is not the same)
	 * @param _type Type of the kay event.
	 * @param _destWidget Pointer of the widget destination
	 * @param _realInputId system Id
	 * @return the ewol input id
	 */
	public int localGetDestinationId(final KeyType _type, final Widget _destWidget, final int _realInputId) {
		if (_type == KeyType.finger) {
			int lastMinimum = 0;
			for (int iii = 0; iii < MAX_MANAGE_INPUT; iii++) {
				if (true == this.eventInputSaved[iii].isUsed) {
					final Widget tmpWidget = this.eventInputSaved[iii].curentWidgetEvent.get();
					if (tmpWidget == _destWidget) {
						if (iii != _realInputId) {
							lastMinimum = FMath.max(lastMinimum, this.eventInputSaved[iii].destinationInputId);
						}
					}
				}
			}
			return lastMinimum + 1;
		}
		return _realInputId;
	}
	
	// note if id<0  == > the it was finger event ...
	public void motion(final KeyType _type, final int _pointerID, final Vector2f _pos) {
		//Log.debug("motion event : " + _type + " " + _pointerID + " " + _pos);
		if (MAX_MANAGE_INPUT <= _pointerID) {
			// reject pointer  == > out of IDs...
			return;
		}
		InputPoperty[] eventTable = null;
		if (_type == KeyType.mouse) {
			eventTable = this.eventMouseSaved;
		} else if (_type == KeyType.finger) {
			eventTable = this.eventInputSaved;
		} else {
			Log.error("Unknown type of event");
			return;
		}
		if (_pointerID > MAX_MANAGE_INPUT || _pointerID < 0) {
			// not manage input
			return;
		}
		final Windows tmpWindows = this.context.getWindows();
		// special case for the mouse event 0 that represent the hover event of the system :
		if (_type == KeyType.mouse && _pointerID == 0) {
			// this event is all time on the good widget ... and manage the enter and leave ...
			// NOTE : the "layer widget" force us to get the widget at the specific position all the time :
			Widget tmpWidget = null;
			if (this.grabWidget.get() != null) {
				// grab all events ...
				tmpWidget = this.grabWidget.get();
			} else if (tmpWindows != null) {
				tmpWidget = tmpWindows.getWidgetAtPos(_pos);
			}
			if (tmpWidget != eventTable[_pointerID].curentWidgetEvent.get()
					|| (eventTable[_pointerID].isInside == true && (eventTable[_pointerID].origin.x() > _pos.x() || eventTable[_pointerID].origin.y() > _pos.y()
							|| (eventTable[_pointerID].origin.x() + eventTable[_pointerID].size.x()) < _pos.x() || (eventTable[_pointerID].origin.y() + eventTable[_pointerID].size.y()) < _pos.y()))) {
				eventTable[_pointerID].isInside = false;
				//Log.debug("GUI : Input ID=" + _pointerID + " == >" + eventTable[_pointerID].destinationInputId + " [LEAVE] " + _pos);
				eventTable[_pointerID].posEvent = _pos;
				localEventInput(_type, eventTable[_pointerID].curentWidgetEvent.get(), eventTable[_pointerID].destinationInputId, KeyStatus.leave, _pos);
			}
			if (eventTable[_pointerID].isInside == false) {
				// set the element inside ...
				eventTable[_pointerID].isInside = true;
				// get destination widget :
				eventTable[_pointerID].curentWidgetEvent = new WeakReference<>(tmpWidget);
				if (tmpWidget == null) {
					eventTable[_pointerID].isInside = false;
				} else {
					eventTable[_pointerID].origin = tmpWidget.getOrigin();
					eventTable[_pointerID].size = tmpWidget.getSize();
				}
				eventTable[_pointerID].destinationInputId = 0;
				//Log.debug("GUI : Input ID=" + _pointerID + " == >" + eventTable[_pointerID].destinationInputId + " [ENTER] " + _pos);
				eventTable[_pointerID].posEvent = _pos;
				localEventInput(_type, tmpWidget, eventTable[_pointerID].destinationInputId, KeyStatus.enter, _pos);
			}
			//Log.debug("GUI : Input ID=" + _pointerID + " == >" + eventTable[_pointerID].destinationInputId + " [MOVE]  " + _pos);
			eventTable[_pointerID].posEvent = _pos;
			localEventInput(_type, tmpWidget, eventTable[_pointerID].destinationInputId, KeyStatus.move, _pos);
		} else if (eventTable[_pointerID].isUsed == true) {
			if (eventTable[_pointerID].isInside == true) {
				if (eventTable[_pointerID].origin.x() > _pos.x() || eventTable[_pointerID].origin.y() > _pos.y() || (eventTable[_pointerID].origin.x() + eventTable[_pointerID].size.x()) < _pos.x()
						|| (eventTable[_pointerID].origin.y() + eventTable[_pointerID].size.y()) < _pos.y()) {
					eventTable[_pointerID].isInside = false;
					//Log.debug("GUI : Input ID=" + _pointerID + " == >" + eventTable[_pointerID].destinationInputId + " [LEAVE] " + _pos);
					eventTable[_pointerID].posEvent = _pos;
					localEventInput(_type, eventTable[_pointerID].curentWidgetEvent.get(), eventTable[_pointerID].destinationInputId, KeyStatus.leave, _pos);
				}
			} else if ((eventTable[_pointerID].origin.x() <= _pos.x() && (eventTable[_pointerID].origin.x() + eventTable[_pointerID].size.x()) >= _pos.x())
					&& (eventTable[_pointerID].origin.y() <= _pos.y() && (eventTable[_pointerID].origin.y() + eventTable[_pointerID].size.y()) >= _pos.y())) {
				eventTable[_pointerID].isInside = true;
				//Log.debug("GUI : Input ID=" + _pointerID + " == >" + eventTable[_pointerID].destinationInputId + " [ENTER] " + _pos);
				eventTable[_pointerID].posEvent = _pos;
				localEventInput(_type, eventTable[_pointerID].curentWidgetEvent.get(), eventTable[_pointerID].destinationInputId, KeyStatus.enter, _pos);
			}
			//Log.debug("GUI : Input ID=" + _pointerID + " == >" + eventTable[_pointerID].destinationInputId + " [MOVE]  " + _pos);
			eventTable[_pointerID].posEvent = _pos;
			localEventInput(_type, eventTable[_pointerID].curentWidgetEvent.get(), eventTable[_pointerID].destinationInputId, KeyStatus.move, _pos);
		}
	}
	
	/**
	 * a new layer on the windows is set  == > might remove all the property of the current element ...
	 */
	public void newLayerSet() {
		for (int iii = 0; iii < MAX_MANAGE_INPUT; iii++) {
			// remove the property of this input ...
			abortElement(this.eventInputSaved, iii, KeyType.finger);
			cleanElement(this.eventInputSaved, iii);
			abortElement(this.eventMouseSaved, iii, KeyType.mouse);
			cleanElement(this.eventMouseSaved, iii);
		}
	}
	
	public void setDpi(final int newDPI) {
		this.dpi = newDPI;
		// recalculate the DPI system ...
		calculateLimit();
	}
	
	public void setLastKeyboardSpecial(final KeySpecial _specialKey) {
		this.specialKey = _specialKey;
	}
	
	public void state(final KeyType _type, final int _pointerID, final boolean _isDown, final Vector2f _pos) {
		if (_pointerID >= MAX_MANAGE_INPUT) {
			// reject pointer  == > out of IDs...
			return;
		}
		//Log.debug("event pointerId=" + _pointerID);
		// convert position in open-GL coordonates ...
		InputPoperty[] eventTable = null;
		InputLimit localLimit;
		if (_type == KeyType.mouse) {
			eventTable = this.eventMouseSaved;
			localLimit = this.eventMouseLimit;
		} else if (_type == KeyType.finger) {
			eventTable = this.eventInputSaved;
			localLimit = this.eventInputLimit;
		} else {
			Log.error("Unknown type of event");
			return;
		}
		if (_pointerID > MAX_MANAGE_INPUT || _pointerID <= 0) {
			// not manage input
			return;
		}
		// get the curent time ...
		final Clock currentTime = Clock.now();
		final Windows tmpWindows = this.context.getWindows();
		
		if (_isDown == true) {
			//Log.debug("GUI : Input ID=" + _pointerID + " == >" + eventTable[_pointerID].destinationInputId + " [DOWN] " + _pos);
			if (eventTable[_pointerID].isUsed == true) {
				// we have an event previously ... check delay between click and offset position
				if (currentTime.less(eventTable[_pointerID].lastTimeEvent).isGreaterThan(localLimit.sepatateTime)) {
					cleanElement(eventTable, _pointerID);
				} else if (FMath.abs(eventTable[_pointerID].downStart.x() - _pos.x()) >= localLimit.DpiOffset || FMath.abs(eventTable[_pointerID].downStart.y() - _pos.y()) >= localLimit.DpiOffset) {
					cleanElement(eventTable, _pointerID);
				}
			}
			if (eventTable[_pointerID].isUsed == true) {
				// save start time
				eventTable[_pointerID].lastTimeEvent = currentTime;
				// generate DOWN Event
				//Log.debug("GUI : Input ID=" + _pointerID + " == >" + eventTable[_pointerID].destinationInputId + " [DOWN]   " + _pos);
				eventTable[_pointerID].posEvent = _pos;
				localEventInput(_type, eventTable[_pointerID].curentWidgetEvent.get(), eventTable[_pointerID].destinationInputId, KeyStatus.down, _pos);
			} else {
				// Mark it used :
				eventTable[_pointerID].isUsed = true;
				// Save current position :
				eventTable[_pointerID].downStart = _pos;
				// save start time
				eventTable[_pointerID].lastTimeEvent = currentTime;
				// set the element inside ...
				eventTable[_pointerID].isInside = true;
				Widget tmpWidget = this.grabWidget.get();
				// get destination widget :
				if (tmpWindows != null) {
					if (tmpWidget != null && _type == KeyType.mouse) {
						eventTable[_pointerID].curentWidgetEvent = new WeakReference<>(tmpWidget);
					} else {
						tmpWidget = tmpWindows.getWidgetAtPos(_pos);
						eventTable[_pointerID].curentWidgetEvent = new WeakReference<>(tmpWidget);
						/*
						if (tmpWidget != null) {
							Log.debug("Get widget at pos=" + _pos + " type: " + tmpWidget.getObjectType());
						} else {
							Log.debug("Get widget at pos=" + _pos + " NO WIDGET");
						}
						*/
					}
				} else {
					eventTable[_pointerID].curentWidgetEvent = null;
				}
				tmpWidget = eventTable[_pointerID].curentWidgetEvent.get();
				if (tmpWidget != null) {
					eventTable[_pointerID].origin = tmpWidget.getOrigin();
					eventTable[_pointerID].size = tmpWidget.getSize();
					eventTable[_pointerID].destinationInputId = localGetDestinationId(_type, tmpWidget, _pointerID);
				} else {
					eventTable[_pointerID].destinationInputId = -1;
				}
				// generate DOWN Event
				//Log.debug("GUI : Input ID=" + _pointerID + " == >" + eventTable[_pointerID].destinationInputId + " [DOWN]   " + _pos);
				eventTable[_pointerID].posEvent = _pos;
				localEventInput(_type, tmpWidget, eventTable[_pointerID].destinationInputId, KeyStatus.down, _pos);
			}
		} else {
			//Log.debug("GUI : Input ID=" + _pointerID + " == >" + eventTable[_pointerID].destinationInputId + " [UP]     " + _pos);
			final Widget tmpWidget = eventTable[_pointerID].curentWidgetEvent.get();
			if (eventTable[_pointerID].isUsed == false) {
				// bad case ... ???
				Log.debug("Up event without previous down ... ");
				// Mark it un-used :
				eventTable[_pointerID].isUsed = false;
				// revove the widget ...
				eventTable[_pointerID].curentWidgetEvent = null;
			} else if (tmpWidget == null) {
				// The widget has been removed:
				//Log.debug("    Object Removed ...");
				// Mark it un-used :
				eventTable[_pointerID].isUsed = false;
				// revove the widget ...
				eventTable[_pointerID].curentWidgetEvent = null;
			} else {
				// generate UP Event
				//Log.debug("GUI : Input ID=" + _pointerID + " == >" + eventTable[_pointerID].destinationInputId + " [UP]     " + _pos);
				eventTable[_pointerID].posEvent = _pos;
				// send up event after the single event to prevent multiple widget getting elements
				localEventInput(_type, tmpWidget, _pointerID, KeyStatus.up, _pos);
				// generate event (single)
				if (FMath.abs(eventTable[_pointerID].downStart.x() - _pos.x()) < localLimit.DpiOffset && FMath.abs(eventTable[_pointerID].downStart.y() - _pos.y()) < localLimit.DpiOffset) {
					// Save current position :
					eventTable[_pointerID].downStart = _pos;
					// save start time
					eventTable[_pointerID].lastTimeEvent = currentTime;
					int nbClickMax = 0;
					if (tmpWidget != null) {
						nbClickMax = tmpWidget.getMouseLimit();
						if (nbClickMax > 5) {
							nbClickMax = 5;
						}
					}
					// in grab mode the single to quinte event are not generated ....
					if ((this.grabWidget.get() == null || _type != KeyType.mouse) && eventTable[_pointerID].nbClickEvent < nbClickMax) {
						// generate event SINGLE :
						eventTable[_pointerID].nbClickEvent++;
						//Log.debug("GUI : Input ID=" + _pointerID + " == >" + eventTable[_pointerID].destinationInputId + " [" + eventTable[_pointerID].nbClickEvent + "] " + _pos);
						eventTable[_pointerID].posEvent = _pos;
						localEventInput(_type, tmpWidget, eventTable[_pointerID].destinationInputId, KeyStatus.pressCount(eventTable[_pointerID].nbClickEvent), _pos);
						if (eventTable[_pointerID].nbClickEvent >= nbClickMax) {
							eventTable[_pointerID].nbClickEvent = 0;
						}
					} else {
						eventTable[_pointerID].nbClickEvent = 0;
					}
				}
				// send up event after the single event to prevent multiple widget getting elements
				localEventInput(_type, tmpWidget, _pointerID, KeyStatus.upAfter, _pos);
				// specific for tuch event
				if (_type == KeyType.finger) {
					cleanElement(eventTable, _pointerID);
				}
			}
		}
	}
	
	/**
	 * This is to transfert the event from one widget to another one
	 * @param _source the widget where the event came from
	 * @param _destination the widget where the event mitgh be generated now
	 */
	public void transfertEvent(final Widget _source, final Widget _destination) {
		if (_source == null || _destination == null) {
			// prevent errors ...
			return;
		}
		for (int iii = 0; iii < MAX_MANAGE_INPUT; iii++) {
			Widget tmpWidget = this.eventInputSaved[iii].curentWidgetEvent.get();
			if (tmpWidget == _source) {
				// inform the widget that it does not receive the event now
				//Log.debug("GUI : Input ID=" + iii + " == >" + this.eventInputSaved[iii].destinationInputId + " [EVENT_INPUT_TYPE_ABORT] " + this.eventInputSaved[iii].posEvent);
				localEventInput(KeyType.finger, tmpWidget, this.eventInputSaved[iii].destinationInputId, KeyStatus.abort, this.eventInputSaved[iii].posEvent);
				// set the new widget ...
				this.eventInputSaved[iii].curentWidgetEvent = new WeakReference<>(_destination);
				// inform the widget that he receive the event property now...
				//Log.debug("GUI : Input ID=" + iii + " == >" + this.eventInputSaved[iii].destinationInputId + " [EVENT_INPUT_TYPE_TRANSFERT] " + this.eventInputSaved[iii].posEvent);
				localEventInput(KeyType.finger, _destination, this.eventInputSaved[iii].destinationInputId, KeyStatus.transfert, this.eventInputSaved[iii].posEvent);
			}
			tmpWidget = this.eventMouseSaved[iii].curentWidgetEvent.get();
			if (tmpWidget == _source) {
				// inform the widget that it does not receive the event now
				//Log.debug("GUI : Input ID=" + iii + " == >" + this.eventMouseSaved[iii].destinationInputId + " [EVENT_INPUT_TYPE_ABORT] " + this.eventMouseSaved[iii].posEvent);
				localEventInput(KeyType.mouse, tmpWidget, this.eventMouseSaved[iii].destinationInputId, KeyStatus.abort, this.eventMouseSaved[iii].posEvent);
				// set the new widget ...
				this.eventMouseSaved[iii].curentWidgetEvent = new WeakReference<>(_destination);
				// inform the widget that he receive the event property now...
				//Log.debug("GUI : Input ID=" + iii + " == >" + this.eventMouseSaved[iii].destinationInputId + " [EVENT_INPUT_TYPE_TRANSFERT] " + this.eventMouseSaved[iii].posEvent);
				localEventInput(KeyType.mouse, _destination, this.eventMouseSaved[iii].destinationInputId, KeyStatus.transfert, this.eventMouseSaved[iii].posEvent);
			}
		}
	}
	
	/**
	 * This function un-lock the pointer properties to move in relative instead of absolute
	 */
	public void unGrabPointer() {
		this.grabWidget = null;
		// TODO: this.context.grabPointerEvents(false, Vector2f(0,0));
	}
	
};

/**
 * internal structure
 */
class InputPoperty {
	public boolean isUsed;
	public int destinationInputId;
	public Clock lastTimeEvent;
	public WeakReference<Widget> curentWidgetEvent;
	public Vector2f origin;
	public Vector2f size;
	public Vector2f downStart;
	public Vector2f posEvent;
	public boolean isDown;
	public boolean isInside;
	public int nbClickEvent; // 0 .. 1 .. 2 .. 3
}
