package org.atriasoft.ewol.widget;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

import org.atriasoft.etk.Color;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.compositing.CompositingDrawing;

public class Spacer extends Widget {
	private final CompositingDrawing draw = new CompositingDrawing(); //!< Compositing drawing element
	@AknotManaged
	@AknotAttribute
	@AknotName("color")
	@AknotDescription("background of the spacer")
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
	public Widget getWidgetAtPos(final Vector2f pos) {
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
		this.draw.setPos(Vector2f.ZERO);
		this.draw.rectangleWidth(new Vector2f(this.size.x(), this.size.y()));
		//this.draw.setPos(new Vector2f(this.size.x() * 0.1f, this.size.y() * 0.1f, 0));
		//this.draw.rectangleWidth(new Vector2f(this.size.x() * 0.8f, this.size.y() * 0.8f, 0));
		
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
