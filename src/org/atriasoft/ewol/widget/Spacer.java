package org.atriasoft.ewol.widget;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

import org.atriasoft.etk.Color;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.annotation.EwolDescription;
import org.atriasoft.ewol.compositing.CompositingDrawing;
import org.atriasoft.exml.annotation.XmlAttribute;
import org.atriasoft.exml.annotation.XmlManaged;
import org.atriasoft.exml.annotation.XmlName;

public class Spacer extends Widget {
	private final CompositingDrawing draw = new CompositingDrawing(); //!< Compositing drawing element
	@XmlManaged
	@XmlAttribute
	@XmlName(value = "color")
	@EwolDescription(value = "background of the spacer")
	protected Color propertyColor = Color.NONE; //!< Background color
	
	/**
	 * Main ructer
	 */
	public Spacer() {
		
	}
	
	public Color getPropertyColor() {
		return this.propertyColor;
	}
	
	@Override
	public Widget getWidgetAtPos(final Vector3f pos) {
		return null;
	}
	
	@Override
	public void onDraw() {
		this.draw.draw();
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
		this.draw.rectangleWidth(new Vector3f(this.size.x(), this.size.y(), 0));
		//this.draw.setPos(new Vector3f(this.size.x() * 0.1f, this.size.y() * 0.1f, 0));
		//this.draw.rectangleWidth(new Vector3f(this.size.x() * 0.8f, this.size.y() * 0.8f, 0));
		
		this.draw.flush();
	}
	
	public void setPropertyColor(final Color propertyColor) {
		if (propertyColor.equals(this.propertyColor)) {
			return;
		}
		this.propertyColor = propertyColor;
		markToRedraw();
	}
}
