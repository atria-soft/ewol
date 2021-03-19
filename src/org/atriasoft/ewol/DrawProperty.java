package org.atriasoft.ewol;

import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

public class DrawProperty {
	/*
	                                                          /-. windowsSize
	      *--------------------------------------------------*
	      |                                                  |
	      |                                                  |
	      |                                    size          |
	      |                                   /              |
	      |              o-------------------o               |
	      |              |                   |               |
	      |              |                   |               |
	      |              |                   |               |
	      |              |                   |               |
	      |              |                   |               |
	      |              |                   |               |
	      |              |                   |               |
	      |              |                   |               |
	      |              o-------------------o               |
	      |             /                                    |
	      |       origin                                     |
	      |                                                  |
	      *--------------------------------------------------*
	     /
	   (0,0)
	 */
	public Vector2i windowsSize = new Vector2i(0, 0); //!< Windows complete size
	public Vector2i origin = new Vector2i(0, 0); //!< Windows clipping upper widget (can not be <0)
	public Vector2i size = new Vector2i(0, 0); //!< Windows clipping upper widget (can not be <0 and >this.windowsSize)
	
	public DrawProperty() {
		
	}
	
	public DrawProperty(final Vector2i windowsSize, final Vector2i origin, final Vector2i size) {
		super();
		this.windowsSize = windowsSize;
		this.origin = origin;
		this.size = size;
	}
	
	@Override
	public DrawProperty clone() {
		return new DrawProperty(this.windowsSize, this.origin, this.size);
	}
	
	public void limit(final Vector2f _origin, final Vector2f _size) {
		this.size.add(this.origin);
		this.origin.setMax((int) _origin.x, (int) _origin.y);
		this.size.setMin((int) (_origin.x + _size.x), (int) (_origin.y + _size.y));
		this.size.less(this.origin);
	}
	
	@Override
	public String toString() {
		return "DrawProperty [windowsSize=" + this.windowsSize + ", start=" + this.origin + ", stop=" + this.origin.add(this.size) + "]";
	}
}
