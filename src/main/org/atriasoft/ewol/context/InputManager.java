/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.context;

import java.lang.ref.WeakReference;

import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.event.InputSystem;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.ewol.widget.Windows;
import org.atriasoft.gale.key.KeySpecial;
import org.atriasoft.gale.key.KeyStatus;
import org.atriasoft.gale.key.KeyType;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * internal structure
 */
class InputLimit {
	public int dpiOffset;
	public long sepatateTime; // in nanosecond System.nanoTime()
}

class InputManager {
	private static final Logger LOGGER = LoggerFactory.getLogger(InputManager.class);
	private static final long MILLI_TO_DURATION = 1000000;
	private static final long SECONDS_TO_DURATION = 1000000000;
	private static final int MAX_MANAGE_INPUT = 15;
	private final EwolContext context;
	private int dpi;
	private final InputLimit eventInputLimit = new InputLimit();
	private final InputPoperty[] eventInputSaved = new InputPoperty[InputManager.MAX_MANAGE_INPUT];
	private final InputLimit eventMouseLimit = new InputLimit();
	private final InputPoperty[] eventMouseSaved = new InputPoperty[InputManager.MAX_MANAGE_INPUT];

	// special grab pointer mode :
	private WeakReference<Widget> grabWidget = null; //!< widget that grab the curent pointer.
	private KeySpecial specialKey;

	public InputManager(final EwolContext context) {
		this.context = context;

		setDpi(200);
		LOGGER.info("Init (start)");
		for (int iii = 0; iii < InputManager.MAX_MANAGE_INPUT; iii++) {
			// remove the property of this input ...
			this.eventInputSaved[iii] = new InputPoperty();
			this.eventMouseSaved[iii] = new InputPoperty();
		}

		LOGGER.info("Init (end)");
	}

	public void abortElement(final InputPoperty[] eventTable, final int idInput, final KeyType type) {
		if (eventTable == null) {
			return;
		}
		if (eventTable[idInput].isUsed) {
			localEventInput(type, eventTable[idInput].curentWidgetEvent.get(), eventTable[idInput].destinationInputId,
					KeyStatus.abort, eventTable[idInput].posEvent);
		}
	}

	private void calculateLimit() {
		this.eventInputLimit.sepatateTime = 300 * InputManager.MILLI_TO_DURATION;
		this.eventInputLimit.dpiOffset = this.dpi * 100;
		this.eventMouseLimit.sepatateTime = 300 * InputManager.MILLI_TO_DURATION;
		this.eventMouseLimit.dpiOffset = (int) (this.dpi * 0.1f);
	}

	public void cleanElement(final InputPoperty[] eventMouseSaved2, final int idInput) {
		if (eventMouseSaved2 == null) {
			return;
		}
		//LOGGER.info("CleanElement[" + idInput + "] = @" + (long)eventTable);
		eventMouseSaved2[idInput].clear();
	}

	/**
	 * This fonction lock the pointer properties to move in relative instead of absolute
	 * @param widget The widget that lock the pointer events
	 */
	public void grabPointer(final Widget widget) {
		if (widget == null) {
			return;
		}
		this.grabWidget = new WeakReference<>(widget);
		/* TODO :
		this.context.grabPointerEvents(true,   widget.getOrigin()
		                                  + Vector2i(widget.getSize().x/2.0f,
		                                          widget.getSize().y/2.0f) );
		*/
	}

	/**
	 * generate the event on the destinated widget.
	 * @param type Type of the event that might be sended.
	 * @param destWidget Pointer on the requested widget that element might be sended
	 * @param idInput Id of the event (PC : [0..9] and touch : [1..9])
	 * @param typeEvent type of the eventg generated
	 * @param pos position of the event
	 * @return true if event has been greped
	 */
	public boolean localEventInput(
			final KeyType type,
			final Widget destWidget,
			final int idInput,
			final KeyStatus status,
			final Vector2f pos) {
		if (destWidget != null) {
			if (type == KeyType.mouse || type == KeyType.finger) {
				// create the system Event :
				// TODO : set the real ID ...
				final InputSystem tmpEventSystem = new InputSystem(type, status, idInput, pos, destWidget, 0,
						this.specialKey);
				// generate the event :
				return destWidget.systemEventInput(tmpEventSystem);
			}
			return false;
		}
		return false;
	}

	/**
	 * convert the system event id in the correct EWOL id depending of the system management mode
	 *        This function find the next input id unused on the specifiic widget
	 *             == > on PC, the ID does not change (GUI is not the same)
	 * @param type Type of the kay event.
	 * @param destWidget Pointer of the widget destination
	 * @param realInputId system Id
	 * @return the ewol input id
	 */
	public int localGetDestinationId(final KeyType type, final Widget destWidget, final int realInputId) {
		if (type == KeyType.finger) {
			int lastMinimum = 0;
			for (int iii = 0; iii < InputManager.MAX_MANAGE_INPUT; iii++) {
				if (this.eventInputSaved[iii].isUsed) {
					final Widget tmpWidget = this.eventInputSaved[iii].curentWidgetEvent.get();
					if (tmpWidget == destWidget) {
						if (iii != realInputId) {
							lastMinimum = FMath.max(lastMinimum, this.eventInputSaved[iii].destinationInputId);
						}
					}
				}
			}
			return lastMinimum + 1;
		}
		return realInputId;
	}

	// note if id<0  == > the it was finger event ...
	public void motion(final KeyType type, final int pointerID, final Vector2f pos) {
		//LOGGER.debug("motion event : " + type + " " + pointerID + " " + pos);
		if (InputManager.MAX_MANAGE_INPUT <= pointerID) {
			// reject pointer  == > out of IDs...
			return;
		}
		InputPoperty[] eventTable = null;
		if (type == KeyType.mouse) {
			eventTable = this.eventMouseSaved;
		} else if (type == KeyType.finger) {
			eventTable = this.eventInputSaved;
		} else {
			LOGGER.error("Unknown type of event");
			return;
		}
		if (pointerID > InputManager.MAX_MANAGE_INPUT || pointerID < 0) {
			// not manage input
			return;
		}
		final Windows tmpWindows = this.context.getWindows();
		// special case for the mouse event 0 that represent the hover event of the system :
		if (type == KeyType.mouse && pointerID == 0) {
			// this event is all time on the good widget ... and manage the enter and leave ...
			// NOTE : the "layer widget" force us to get the widget at the specific position all the time :
			Widget tmpWidget = null;
			if (this.grabWidget != null && this.grabWidget.get() != null) {
				// grab all events ...
				tmpWidget = this.grabWidget.get();
			} else if (tmpWindows != null) {
				tmpWidget = tmpWindows.getWidgetAtPos(pos);
			}
			if (eventTable[pointerID].curentWidgetEvent != null
					&& tmpWidget != eventTable[pointerID].curentWidgetEvent.get()
					|| (eventTable[pointerID].isInside && (eventTable[pointerID].origin.x() > pos.x()
							|| eventTable[pointerID].origin.y() > pos.y()
							|| (eventTable[pointerID].origin.x() + eventTable[pointerID].size.x()) < pos.x()
							|| (eventTable[pointerID].origin.y() + eventTable[pointerID].size.y()) < pos.y()))) {
				eventTable[pointerID].isInside = false;
				//LOGGER.debug("GUI : Input ID=" + pointerID + " == >" + eventTable[pointerID].destinationInputId + " [LEAVE] " + pos);
				eventTable[pointerID].posEvent = pos;
				localEventInput(type, eventTable[pointerID].curentWidgetEvent.get(),
						eventTable[pointerID].destinationInputId, KeyStatus.leave, pos);
			}
			if (!eventTable[pointerID].isInside) {
				// set the element inside ...
				eventTable[pointerID].isInside = true;
				// get destination widget :
				eventTable[pointerID].curentWidgetEvent = new WeakReference<>(tmpWidget);
				if (tmpWidget == null) {
					eventTable[pointerID].isInside = false;
				} else {
					eventTable[pointerID].origin = tmpWidget.getOrigin();
					eventTable[pointerID].size = tmpWidget.getSize();
				}
				eventTable[pointerID].destinationInputId = 0;
				//LOGGER.debug("GUI : Input ID=" + pointerID + " == >" + eventTable[pointerID].destinationInputId + " [ENTER] " + pos);
				eventTable[pointerID].posEvent = pos;
				localEventInput(type, tmpWidget, eventTable[pointerID].destinationInputId, KeyStatus.enter, pos);
			}
			//LOGGER.debug("GUI : Input ID=" + pointerID + " == >" + eventTable[pointerID].destinationInputId + " [MOVE]  " + pos);
			eventTable[pointerID].posEvent = pos;
			localEventInput(type, tmpWidget, eventTable[pointerID].destinationInputId, KeyStatus.move, pos);
		} else if (eventTable[pointerID].isUsed) {
			if (eventTable[pointerID].isInside) {
				if (eventTable[pointerID].origin.x() > pos.x() || eventTable[pointerID].origin.y() > pos.y()
						|| (eventTable[pointerID].origin.x() + eventTable[pointerID].size.x()) < pos.x()
						|| (eventTable[pointerID].origin.y() + eventTable[pointerID].size.y()) < pos.y()) {
					eventTable[pointerID].isInside = false;
					//LOGGER.debug("GUI : Input ID=" + pointerID + " == >" + eventTable[pointerID].destinationInputId + " [LEAVE] " + pos);
					eventTable[pointerID].posEvent = pos;
					localEventInput(type, eventTable[pointerID].curentWidgetEvent.get(),
							eventTable[pointerID].destinationInputId, KeyStatus.leave, pos);
				}
			} else if ((eventTable[pointerID].origin.x() <= pos.x()
					&& (eventTable[pointerID].origin.x() + eventTable[pointerID].size.x()) >= pos.x())
					&& (eventTable[pointerID].origin.y() <= pos.y()
							&& (eventTable[pointerID].origin.y() + eventTable[pointerID].size.y()) >= pos.y())) {
				eventTable[pointerID].isInside = true;
				//LOGGER.debug("GUI : Input ID=" + pointerID + " == >" + eventTable[pointerID].destinationInputId + " [ENTER] " + pos);
				eventTable[pointerID].posEvent = pos;
				localEventInput(type, eventTable[pointerID].curentWidgetEvent.get(),
						eventTable[pointerID].destinationInputId, KeyStatus.enter, pos);
			}
			//LOGGER.debug("GUI : Input ID=" + pointerID + " == >" + eventTable[pointerID].destinationInputId + " [MOVE]  " + pos);
			eventTable[pointerID].posEvent = pos;
			localEventInput(type, eventTable[pointerID].curentWidgetEvent.get(),
					eventTable[pointerID].destinationInputId, KeyStatus.move, pos);
		}
	}

	/**
	 * a new layer on the windows is set  == > might remove all the property of the current element ...
	 */
	public void newLayerSet() {
		for (int iii = 0; iii < InputManager.MAX_MANAGE_INPUT; iii++) {
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

	public void setLastKeyboardSpecial(final KeySpecial specialKey) {
		this.specialKey = specialKey;
	}

	public void state(final KeyType type, final int pointerID, final boolean isDown, final Vector2f pos) {
		if (pointerID >= InputManager.MAX_MANAGE_INPUT) {
			// reject pointer  == > out of IDs...
			return;
		}
		//LOGGER.debug("event pointerId=" + pointerID);
		// convert position in open-GL coordonates ...
		InputPoperty[] eventTable = null;
		InputLimit localLimit;
		if (type == KeyType.mouse) {
			eventTable = this.eventMouseSaved;
			localLimit = this.eventMouseLimit;
		} else if (type == KeyType.finger) {
			eventTable = this.eventInputSaved;
			localLimit = this.eventInputLimit;
		} else {
			LOGGER.error("Unknown type of event");
			return;
		}
		if (pointerID > InputManager.MAX_MANAGE_INPUT || pointerID <= 0) {
			// not manage input
			return;
		}
		// get the curent time ...
		final long currentTime = System.nanoTime();
		final Windows tmpWindows = this.context.getWindows();

		if (isDown) {
			if (eventTable[pointerID].isUsed) {
				// we have an event previously ... check delay between click and offset position
				if (currentTime - eventTable[pointerID].lastTimeEvent > localLimit.sepatateTime) {
					cleanElement(eventTable, pointerID);
				} else if (FMath.abs(eventTable[pointerID].downStart.x() - pos.x()) >= localLimit.dpiOffset
						|| FMath.abs(eventTable[pointerID].downStart.y() - pos.y()) >= localLimit.dpiOffset) {
					cleanElement(eventTable, pointerID);
				}
			}
			if (eventTable[pointerID].isUsed) {
				// save start time
				eventTable[pointerID].lastTimeEvent = currentTime;
				// generate DOWN Event
				//LOGGER.debug("GUI : Input ID=" + pointerID + " == >" + eventTable[pointerID].destinationInputId + " [DOWN]   " + pos);
				eventTable[pointerID].posEvent = pos;
				localEventInput(type, eventTable[pointerID].curentWidgetEvent.get(),
						eventTable[pointerID].destinationInputId, KeyStatus.down, pos);
			} else {
				// Mark it used :
				eventTable[pointerID].isUsed = true;
				// Save current position :
				eventTable[pointerID].downStart = pos;
				// save start time
				eventTable[pointerID].lastTimeEvent = currentTime;
				// set the element inside ...
				eventTable[pointerID].isInside = true;
				Widget tmpWidget = this.grabWidget == null ? null : this.grabWidget.get();
				LOGGER.info("InputManager: DOWN event grabWidget={}", tmpWidget != null ? tmpWidget.getClass().getSimpleName() : "null");
				// get destination widget :
				if (tmpWindows != null) {
					if (tmpWidget != null && type == KeyType.mouse) {
						LOGGER.info("InputManager: using grabWidget for mouse");
						eventTable[pointerID].curentWidgetEvent = new WeakReference<>(tmpWidget);
					} else {
						tmpWidget = tmpWindows.getWidgetAtPos(pos);
						LOGGER.info("InputManager: getWidgetAtPos returned: {}", tmpWidget != null ? tmpWidget.getClass().getSimpleName() : "null");
						eventTable[pointerID].curentWidgetEvent = new WeakReference<>(tmpWidget);
						/*
						if (tmpWidget != null) {
							LOGGER.debug("Get widget at pos=" + pos + " type: " + tmpWidget.getObjectType());
						} else {
							LOGGER.debug("Get widget at pos=" + pos + " NO WIDGET");
						}
						*/
					}
				} else {
					eventTable[pointerID].curentWidgetEvent = null;
				}
				tmpWidget = eventTable[pointerID].curentWidgetEvent.get();
				if (tmpWidget != null) {
					eventTable[pointerID].origin = tmpWidget.getOrigin();
					eventTable[pointerID].size = tmpWidget.getSize();
					eventTable[pointerID].destinationInputId = localGetDestinationId(type, tmpWidget, pointerID);
				} else {
					eventTable[pointerID].destinationInputId = -1;
				}
				// generate DOWN Event
				//LOGGER.debug("GUI : Input ID=" + pointerID + " == >" + eventTable[pointerID].destinationInputId + " [DOWN]   " + pos);
				eventTable[pointerID].posEvent = pos;
				localEventInput(type, tmpWidget, eventTable[pointerID].destinationInputId, KeyStatus.down, pos);
			}
		} else {
			//LOGGER.debug("GUI : Input ID=" + pointerID + " == >" + eventTable[pointerID].destinationInputId + " [UP]     " + pos);
			final Widget tmpWidget = eventTable[pointerID].curentWidgetEvent.get();
			if (!eventTable[pointerID].isUsed) {
				// bad case ... ???
				LOGGER.debug("Up event without previous down ... ");
				// Mark it un-used :
				eventTable[pointerID].isUsed = false;
				// revove the widget ...
				eventTable[pointerID].curentWidgetEvent = null;
			} else if (tmpWidget == null) {
				// The widget has been removed:
				//LOGGER.debug("    Object Removed ...");
				// Mark it un-used :
				eventTable[pointerID].isUsed = false;
				// revove the widget ...
				eventTable[pointerID].curentWidgetEvent = null;
			} else {
				// generate UP Event
				//LOGGER.debug("GUI : Input ID=" + pointerID + " == >" + eventTable[pointerID].destinationInputId + " [UP]     " + pos);
				eventTable[pointerID].posEvent = pos;
				// send up event after the single event to prevent multiple widget getting elements
				localEventInput(type, tmpWidget, pointerID, KeyStatus.up, pos);
				// generate event (single)
				if (FMath.abs(eventTable[pointerID].downStart.x() - pos.x()) < localLimit.dpiOffset
						&& FMath.abs(eventTable[pointerID].downStart.y() - pos.y()) < localLimit.dpiOffset) {
					// Save current position :
					eventTable[pointerID].downStart = pos;
					// save start time
					eventTable[pointerID].lastTimeEvent = currentTime;
					int nbClickMax = 0;
					nbClickMax = tmpWidget.getMouseLimit();
					if (nbClickMax > 5) {
						nbClickMax = 5;
					}
					// in grab mode the single to quinte event are not generated ....
					if ((this.grabWidget == null || this.grabWidget.get() == null || type != KeyType.mouse)
							&& eventTable[pointerID].nbClickEvent < nbClickMax) {
						// generate event SINGLE :
						eventTable[pointerID].nbClickEvent++;
						//LOGGER.debug("GUI : Input ID=" + pointerID + " == >" + eventTable[pointerID].destinationInputId + " [" + eventTable[pointerID].nbClickEvent + "] " + pos);
						eventTable[pointerID].posEvent = pos;
						localEventInput(type, tmpWidget, eventTable[pointerID].destinationInputId,
								KeyStatus.pressCount(eventTable[pointerID].nbClickEvent), pos);
						if (eventTable[pointerID].nbClickEvent >= nbClickMax) {
							eventTable[pointerID].nbClickEvent = 0;
						}
					} else {
						eventTable[pointerID].nbClickEvent = 0;
					}
				}
				// send up event after the single event to prevent multiple widget getting elements
				localEventInput(type, tmpWidget, pointerID, KeyStatus.upAfter, pos);
				// specific for tuch event
				if (type == KeyType.finger) {
					cleanElement(eventTable, pointerID);
				}
			}
		}
	}

	/**
	 * This is to transfert the event from one widget to another one
	 * @param source the widget where the event came from
	 * @param destination the widget where the event mitgh be generated now
	 */
	public void transfertEvent(final Widget source, final Widget destination) {
		if (source == null || destination == null) {
			// prevent errors ...
			return;
		}
		for (int iii = 0; iii < InputManager.MAX_MANAGE_INPUT; iii++) {
			Widget tmpWidget = this.eventInputSaved[iii].curentWidgetEvent.get();
			if (tmpWidget == source) {
				// inform the widget that it does not receive the event now
				//LOGGER.debug("GUI : Input ID=" + iii + " == >" + this.eventInputSaved[iii].destinationInputId + " [EVENTINPUTTYPEABORT] " + this.eventInputSaved[iii].posEvent);
				localEventInput(KeyType.finger, tmpWidget, this.eventInputSaved[iii].destinationInputId,
						KeyStatus.abort, this.eventInputSaved[iii].posEvent);
				// set the new widget ...
				this.eventInputSaved[iii].curentWidgetEvent = new WeakReference<>(destination);
				// inform the widget that he receive the event property now...
				//LOGGER.debug("GUI : Input ID=" + iii + " == >" + this.eventInputSaved[iii].destinationInputId + " [EVENTINPUTTYPETRANSFERT] " + this.eventInputSaved[iii].posEvent);
				localEventInput(KeyType.finger, destination, this.eventInputSaved[iii].destinationInputId,
						KeyStatus.transfer, this.eventInputSaved[iii].posEvent);
			}
			tmpWidget = this.eventMouseSaved[iii].curentWidgetEvent.get();
			if (tmpWidget == source) {
				// inform the widget that it does not receive the event now
				//LOGGER.debug("GUI : Input ID=" + iii + " == >" + this.eventMouseSaved[iii].destinationInputId + " [EVENTINPUTTYPEABORT] " + this.eventMouseSaved[iii].posEvent);
				localEventInput(KeyType.mouse, tmpWidget, this.eventMouseSaved[iii].destinationInputId, KeyStatus.abort,
						this.eventMouseSaved[iii].posEvent);
				// set the new widget ...
				this.eventMouseSaved[iii].curentWidgetEvent = new WeakReference<>(destination);
				// inform the widget that he receive the event property now...
				//LOGGER.debug("GUI : Input ID=" + iii + " == >" + this.eventMouseSaved[iii].destinationInputId + " [EVENTINPUTTYPETRANSFERT] " + this.eventMouseSaved[iii].posEvent);
				localEventInput(KeyType.mouse, destination, this.eventMouseSaved[iii].destinationInputId,
						KeyStatus.transfer, this.eventMouseSaved[iii].posEvent);
			}
		}
	}

	/**
	 * This function un-lock the pointer properties to move in relative instead of absolute
	 */
	public void unGrabPointer() {
		this.grabWidget = null;
		// TODO this.context.grabPointerEvents(false, Vector2f(0,0));
	}

}

/**
 * internal structure
 */
class InputPoperty {
	public WeakReference<Widget> curentWidgetEvent;
	public int destinationInputId = 0;
	public Vector2f downStart = Vector2f.ZERO;
	public boolean isDown = false;
	public boolean isInside = false;
	public boolean isUsed = false;
	public long lastTimeEvent = 0; // in ns
	public int nbClickEvent = 0; // 0 .. 1 .. 2 .. 3
	public Vector2f origin = Vector2f.ZERO;
	public Vector2f posEvent = Vector2f.ZERO;
	public Vector2f size = Vector2f.MAX_VALUE;

	public void clear() {
		this.isUsed = false;
		this.destinationInputId = 0;
		this.lastTimeEvent = System.nanoTime();
		this.curentWidgetEvent = null;
		this.origin = Vector2f.ZERO;
		this.size = Vector2f.MAX_VALUE;
		this.downStart = Vector2f.ZERO;
		this.isDown = false;
		this.isInside = false;
		this.nbClickEvent = 0;
		this.posEvent = Vector2f.ZERO;

	}
}
