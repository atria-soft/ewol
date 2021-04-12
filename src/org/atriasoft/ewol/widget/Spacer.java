package org.atriasoft.ewol.widget;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

import org.atriasoft.etk.Color;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.annotation.EwolDescription;
import org.atriasoft.ewol.compositing.CompositingDrawing;
import org.atriasoft.exml.annotation.XmlManaged;
import org.atriasoft.exml.annotation.XmlName;
import org.atriasoft.exml.annotation.XmlProperty;

/**
 * @ingroup ewolWidgetGroup
 */
public class Spacer extends Widget {
	private final CompositingDrawing draw = new CompositingDrawing(); //!< Compositing drawing element
	@XmlManaged
	@XmlProperty
	@XmlName(value = "color")
	@EwolDescription(value = "background of the spacer")
	protected Color propertyColor = Color.GREEN; //!< Background color
	
	/**
	 * Main ructer
	 */
	public Spacer() {
		
	}
	
	public Color getPropertyColor() {
		return this.propertyColor;
	}
	
	@Override
	public Widget getWidgetAtPos(final Vector2f pos) {
		return null;
	}
	
	@Override
	public void onDraw() {
		this.draw.draw();
		this.draw.flush();
	}
	
	@Override
	public void onRegenerateDisplay() {
		if (!needRedraw()) {
			return;
		}
		this.draw.clear();
		
		if (this.propertyColor.a() == 0) {
			return;
		}
		this.draw.setColor(this.propertyColor);
		this.draw.setPos(Vector3f.ZERO);
		this.draw.setPos(new Vector3f(this.size.x() * 0.1f, this.size.y() * 0.1f, 0));
		this.draw.rectangleWidth(new Vector3f(this.size.x() * 0.8f, this.size.y() * 0.8f, 0));
		
		//		this.draw.setColor(Color.RED);
		//		this.draw.setPos(new Vector3f(-1024, -1024, 0));
		//		this.draw.rectangleWidth(new Vector3f(2048, 2048, 0));
		this.draw.flush();
	}
	
	public void setPropertyTextColorBgOn(final Color propertyColor) {
		if (propertyColor.equals(this.propertyColor)) {
			return;
		}
		this.propertyColor = propertyColor;
		markToRedraw();
	}
}
