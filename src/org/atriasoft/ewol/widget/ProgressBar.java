/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.annotation.EwolDescription;
import org.atriasoft.ewol.compositing.CompositingDrawing;
import org.atriasoft.exml.annotation.XmlManaged;
import org.atriasoft.exml.annotation.XmlName;
import org.atriasoft.exml.annotation.XmlProperty;

class ProgressBar extends Widget {
	private static final int DOT_RADIUS = 6;
	private final CompositingDrawing draw = new CompositingDrawing(); // basic drawing element
	@XmlManaged
	@XmlProperty
	@XmlName(value = "color-off")
	@EwolDescription(value = "Color of the false value")
	protected Color propertyTextColorBgOff = Color.NONE;
	@XmlManaged
	@XmlProperty
	@XmlName(value = "color-on")
	@EwolDescription(value = "Color of the true value")
	protected Color propertyTextColorBgOn = Color.GREEN;
	@XmlManaged
	@XmlProperty
	@XmlName(value = "color-bg")
	@EwolDescription(value = "ackground color")
	protected Color propertyTextColorFg = Color.BLACK;
	@XmlManaged
	@XmlProperty
	@XmlName(value = "value")
	@EwolDescription(value = "Value of the progress bar [0..1]")
	protected float propertyValue = 0;
	
	public ProgressBar() {
		setPropertyCanFocus(true);
	}
	
	@Override
	public void calculateMinMaxSize() {
		Vector2f tmpMin = this.propertyMinSize.getPixel();
		this.minSize = new Vector2f(Math.max(tmpMin.x(), 40.0f), Math.max(tmpMin.y(), ProgressBar.DOT_RADIUS * 2.0f));
		markToRedraw();
	}
	
	public Color getPropertyTextColorBgOff() {
		return this.propertyTextColorBgOff;
	}
	
	public Color getPropertyTextColorBgOn() {
		return this.propertyTextColorBgOn;
	}
	
	public Color getPropertyTextColorFg() {
		return this.propertyTextColorFg;
	}
	
	public float getPropertyValue() {
		return this.propertyValue;
	}
	
	@Override
	protected void onDraw() {
		this.draw.draw();
	}
	
	@Override
	public void onRegenerateDisplay() {
		if (!needRedraw()) {
			return;
		}
		// clean the object list ...
		this.draw.clear();
		
		this.draw.setColor(this.propertyTextColorFg);
		
		int tmpSizeX = (int) (this.size.x() - 10);
		int tmpSizeY = (int) (this.size.y() - 10);
		int tmpOriginX = 5;
		int tmpOriginY = 5;
		this.draw.setColor(this.propertyTextColorBgOn);
		this.draw.setPos(new Vector3f(tmpOriginX, tmpOriginY, 0));
		this.draw.rectangleWidth(new Vector3f(tmpSizeX * this.propertyValue, tmpSizeY, 0));
		this.draw.setColor(this.propertyTextColorBgOff);
		this.draw.setPos(new Vector3f(tmpOriginX + tmpSizeX * this.propertyValue, tmpOriginY, 0));
		this.draw.rectangleWidth(new Vector3f(tmpSizeX * (1.0f - this.propertyValue), tmpSizeY, 0));
		
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
