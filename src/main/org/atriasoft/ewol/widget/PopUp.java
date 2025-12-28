/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.gale.key.KeyStatus;

/**
 * Pop-up Display a sub element in a field inside the whole size (id set in pup-up windows)
 */
public class PopUp extends Box {
	protected boolean propertyCloseOutEvent = false;

	/**
	 * Constructor
	 */
	public PopUp() {
		//super(new Uri("THEME", "shape/PopUp.json", "ewol"));
		this.propertyMinSize = new Dimension2f(new Vector2f(80, 80), Distance.POURCENT);
		this.propertyExpand = Vector2b.FALSE;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "out-click-remove")
	@AknotDescription(value = "Remove the widget if the use click outside")
	public boolean isPropertyCloseOutEvent() {
		return this.propertyCloseOutEvent;
	}

	@Override
	public boolean onEventInput(final EventInput event) {
		if (this.propertyCloseOutEvent) {
			return false;
		}
		if (event.inputId() == 0) {
			return false;
		}
		if (event.status() == KeyStatus.move) {
			return false;
		}

		final Vector2f pos = relativePosition(event.pos());
		//		if (!this.shapeProperty.isInside(pos)) {
		//			autoDestroy();
		//			return true;
		//		}
		return false;
	}

	public void setPropertyCloseOutEvent(final boolean propertyCloseOutEvent) {
		if (this.propertyCloseOutEvent == propertyCloseOutEvent) {
			return;
		}
		this.propertyCloseOutEvent = propertyCloseOutEvent;
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	/**
	 * Create a new PopUp.
	 * @return a new PopUp
	 */
	public static PopUp create() {
		return new PopUp();
	}

	/**
	 * Fluent method to set close on outside click.
	 * @param closeOnOutside true to close when clicking outside
	 * @return this popup for chaining
	 */
	public PopUp closeOnOutside(final boolean closeOnOutside) {
		setPropertyCloseOutEvent(closeOnOutside);
		return this;
	}

	/**
	 * Fluent method to set the content widget.
	 * @param widget the widget to display in popup
	 * @return this popup for chaining
	 */
	public PopUp content(final Widget widget) {
		setSubWidget(widget);
		return this;
	}
}
