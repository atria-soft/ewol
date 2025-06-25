/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.etk.Dimension3f;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.gale.key.KeyStatus;

/**
 * Pop-up Display a sub element in a field inside the whole size (id set in pup-up windows)
 */
public class PopUp extends Box {
	// properties
	public boolean propertyCloseOutEvent = false; //!< ratio progression of a sliding

	/**
	 * Constructor
	 */
	public PopUp() {
		//super(new Uri("THEME", "shape/PopUp.json", "ewol"));
		this.propertyMinSize = new Dimension3f(new Vector3f(80, 80, 20), Distance.POURCENT);
		this.propertyExpand = Vector3b.FALSE;
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

		final Vector3f pos = relativePosition(new Vector3f(event.pos().x(), event.pos().y(), 0));
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

}
