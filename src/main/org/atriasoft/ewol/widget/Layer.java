/*
 * @author Edouard DUPIN
 * @copyright 2020, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.etk.math.Vector2f;

/**
 * Layer container that stacks widgets on top of each other.
 */
public class Layer extends ContainerN {
	/**
	 * Constructor
	 */
	public Layer() {
		// nothing to do.
	}
	
	@Override
	public Widget getWidgetAtPos(final Vector2f pos) {
		if (this.propertyHide) {
			return null;
		}
		// for all element in the sizer ...
		for (final Widget it : this.subWidget) {
			if (it == null) {
				continue;
			}
			final Vector2f tmpSize = it.getSize();
			final Vector2f tmpOrigin = it.getOrigin();
			if ((tmpOrigin.x() <= pos.x() && tmpOrigin.x() + tmpSize.x() >= pos.x()) && (tmpOrigin.y() <= pos.y() && tmpOrigin.y() + tmpSize.y() >= pos.y())) {
				final Widget tmpWidget = it.getWidgetAtPos(pos);
				if (tmpWidget != null) {
					return tmpWidget;
				}
				// parse the next layer ...
			}
		}
		return null;
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	/**
	 * Create a new Layer.
	 * @return a new Layer
	 */
	public static Layer create() {
		return new Layer();
	}

	/**
	 * Fluent method to add widgets to the layer.
	 * @param widgets the widgets to add
	 * @return this layer for chaining
	 */
	public Layer add(final Widget... widgets) {
		for (final Widget w : widgets) {
			subWidgetAdd(w);
		}
		return this;
	}
}