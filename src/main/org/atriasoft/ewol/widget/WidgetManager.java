package org.atriasoft.ewol.widget;

import java.lang.ref.WeakReference;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

public class WidgetManager {
	private static final Logger LOGGER = LoggerFactory.getLogger(WidgetManager.class);

	// ---------------------------------------------
	// --  Factory area
	// ---------------------------------------------
	private WeakReference<Widget> focusWidgetCurrent; //!< Current focus selected
	// ---------------------------------------------
	// --  Focus area
	// ---------------------------------------------
	private WeakReference<Widget> focusWidgetDefault; //!< default focus when no current focus is set

	private Runnable funcRedrawNeeded = null;

	// ---------------------------------------------
	// --  Something change area (TODO maybe set it in the windows)
	// ---------------------------------------------
	private boolean haveRedraw = true; //!< something request a redraw

	public WidgetManager() {
	}

	/**
	 * Get the current Focused widget.
	 * @return The pointer on the current focused element.
	 */
	public Widget focusGet() {
		return this.focusWidgetCurrent.get();
	}

	/**
	 * Request a focus on a specify widget.
	 * @param newWidget Widget that might get the focus.
	 */
	public void focusKeep(final Widget newWidget) {
		if (newWidget == null) {
			// nothing to do ...
			return;
		}
		LOGGER.debug("focusKeep={}", newWidget.getId());
		//elog::displayBacktrace();
		Widget focusWidgetCurrent = this.focusWidgetCurrent.get();
		if (newWidget == focusWidgetCurrent) {
			// nothing to do ...
			return;
		}
		if (focusWidgetCurrent != null) {
			LOGGER.debug("Rm focus on WidgetID={}", focusWidgetCurrent.getId());
			focusWidgetCurrent.rmFocus();
			focusWidgetCurrent = null;
		}
		if (!newWidget.propertyCanFocus) {
			LOGGER.debug("Widget can not have focus, id={}", newWidget.getId());
			return;
		}
		this.focusWidgetCurrent = new WeakReference<>(newWidget);
		if (newWidget != null) {
			LOGGER.debug("Set focus on WidgetID={}", newWidget.getId());
			newWidget.setFocus();
		}
	}

	/**
	 * Release the current focus (back on default if possible).
	 */
	public void focusRelease() {
		final Widget focusWidgetDefault = this.focusWidgetDefault.get();
		Widget focusWidgetCurrent = this.focusWidgetCurrent.get();
		if (focusWidgetDefault == focusWidgetCurrent) {
			// nothink to do ...
			return;
		}
		if (focusWidgetCurrent != null) {
			LOGGER.debug("Rm focus on WidgetID={}", focusWidgetCurrent.getId());
			focusWidgetCurrent.rmFocus();
		}
		this.focusWidgetCurrent = this.focusWidgetDefault;
		focusWidgetCurrent = this.focusWidgetCurrent.get();
		if (focusWidgetCurrent != null) {
			LOGGER.debug("Set focus on WidgetID={}", focusWidgetCurrent.getId());
			focusWidgetCurrent.setFocus();
		}
	}

	/**
	 * Set the default focus when none selected.
	 * @param newWidget Widget that might get the focus (when nothing else).
	 */
	public void focusSetDefault(final Widget newWidget) {
		if ((newWidget != null) && (!newWidget.propertyCanFocus)) {
			LOGGER.trace("Widget can not have focus, id={}", newWidget.getId());
			return;
		}
		Widget focusWidgetDefault = null;
		if (this.focusWidgetDefault != null) {
			focusWidgetDefault = this.focusWidgetDefault.get();
		}
		Widget focusWidgetCurrent = null;
		if (this.focusWidgetCurrent != null) {
			focusWidgetCurrent = this.focusWidgetCurrent.get();
		}
		if (focusWidgetDefault == focusWidgetCurrent) {
			if (focusWidgetCurrent != null) {
				LOGGER.debug("Rm focus on WidgetID={}", focusWidgetCurrent.getId());
				focusWidgetCurrent.rmFocus();
			}
			this.focusWidgetCurrent = new WeakReference<>(newWidget);
			if (newWidget != null) {
				LOGGER.debug("Set focus on WidgetID={}", newWidget.getId());
				newWidget.setFocus();
			}
		}
		this.focusWidgetDefault = new WeakReference<>(newWidget);
	}

	/**
	 * Check if a redraw has been requested (set the local value back at false)
	 * @return true if something to be redraw
	 */
	public boolean isDrawingNeeded() {
		final boolean tmp = this.haveRedraw;
		this.haveRedraw = false;
		return tmp;
	}

	/**
	 * Get the list of all Widget that can be created.
	 * @return Separate with ',' string list.
	 */
	public String list() {
		//return this.creatorList.keySet().toString();
		return "";
	}

	/**
	 * Mark the display to redraw
	 */
	public void markDrawingIsNeeded() {
		if (this.haveRedraw) {
			return;
		}
		this.haveRedraw = true;
		if (this.funcRedrawNeeded != null) {
			this.funcRedrawNeeded.run();
		}
	}

	/**
	 * Set a callback when we need redraw the display (need by MacOs)
	 * @param func function to call
	 */
	public void setCallbackonRedrawNeeded(final Runnable func) {
		this.funcRedrawNeeded = func;
	}

}
