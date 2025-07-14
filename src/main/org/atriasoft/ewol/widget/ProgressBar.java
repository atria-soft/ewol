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
import org.atriasoft.etk.Color;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.compositing.CompositingDrawing;
import org.atriasoft.ewol.compositing.CompositingGC;

class ProgressBar extends Widget {
	private static final int DOT_RADIUS = 6;
	private final CompositingDrawing vectorialDraw = new CompositingGC(); // basic drawing element

	protected Color propertyTextColorBgOff = Color.NONE;
	protected Color propertyTextColorBgOn = Color.GREEN;
	protected Color propertyTextColorFg = Color.BLACK;
	protected float propertyValue = 0;

	public ProgressBar() {
		setPropertyCanFocus(true);
	}

	@Override
	public void calculateMinMaxSize() {
		final Vector2f tmpMin = this.propertyMinSize.getPixel();
		this.minSize = new Vector2f(Math.max(tmpMin.x(), 40.0f), Math.max(tmpMin.y(), ProgressBar.DOT_RADIUS * 2.0f));
		markToRedraw();
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "color-off")
	@AknotDescription(value = "Color of the false value")
	public Color getPropertyTextColorBgOff() {
		return this.propertyTextColorBgOff;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "color-on")
	@AknotDescription(value = "Color of the true value")
	public Color getPropertyTextColorBgOn() {
		return this.propertyTextColorBgOn;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "color-bg")
	@AknotDescription(value = "ackground color")
	public Color getPropertyTextColorFg() {
		return this.propertyTextColorFg;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "value")
	@AknotDescription(value = "Value of the progress bar [0..1]")
	public float getPropertyValue() {
		return this.propertyValue;
	}

	@Override
	protected void onDraw() {
		this.vectorialDraw.draw();
	}

	@Override
	public void onRegenerateDisplay() {
		if (!needRedraw()) {
			return;
		}
		// clean the object list ...
		this.vectorialDraw.clear();

		this.vectorialDraw.setColor(this.propertyTextColorFg);

		final int tmpSizeX = (int) (this.size.x() - 10);
		final int tmpSizeY = (int) (this.size.y() - 10);
		final int tmpOriginX = 5;
		final int tmpOriginY = 5;
		this.vectorialDraw.setColor(this.propertyTextColorBgOn);
		this.vectorialDraw.setPos(new Vector2f(tmpOriginX, tmpOriginY));
		this.vectorialDraw.rectangleWidth(new Vector2f(tmpSizeX * this.propertyValue, tmpSizeY));
		this.vectorialDraw.setColor(this.propertyTextColorBgOff);
		this.vectorialDraw.setPos(new Vector2f(tmpOriginX + tmpSizeX * this.propertyValue, tmpOriginY));
		this.vectorialDraw.rectangleWidth(new Vector2f(tmpSizeX * (1.0f - this.propertyValue), tmpSizeY));

		// TODO : Create a better progress Bar ...
		//this.draw.setColor(propertyTextColorFg);
		//this.draw.rectangleBorder( tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY, 1);
	}

	public void setPropertyTextColorBgOff(final Color propertyTextColorBgOff) {
		if (propertyTextColorBgOff.equals(this.propertyTextColorBgOff)) {
			return;
		}
		this.propertyTextColorBgOff = propertyTextColorBgOff;
		markToRedraw();
	}

	public void setPropertyTextColorBgOn(final Color propertyTextColorBgOn) {
		if (propertyTextColorBgOn.equals(this.propertyTextColorBgOn)) {
			return;
		}
		this.propertyTextColorBgOn = propertyTextColorBgOn;
		markToRedraw();
	}

	public void setPropertyTextColorFg(final Color propertyTextColorFg) {
		if (propertyTextColorFg.equals(this.propertyTextColorFg)) {
			return;
		}
		this.propertyTextColorFg = propertyTextColorFg;
		markToRedraw();
	}

	public void setPropertyValue(final float propertyValue) {
		if (propertyValue == this.propertyValue) {
			return;
		}
		this.propertyValue = propertyValue;
		markToRedraw();
	}
}
