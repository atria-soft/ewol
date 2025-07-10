/** @file
 * @author Edouard DUPIN
 * @copyright 2020, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.etk.math.Vector2f;

/**
 * @ingroup ewolWidgetGroup
 */
class Layer extends ContainerN {
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
	
}