package org.atriasoft.ewol;

import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;

/**
 * @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

//@formatter:off
	/*
                                                            /- windowsSize
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
	//@formatter:on
public record DrawProperty(
		Vector2i windowsSize, // !< Windows complete size
		Vector2i origin, // !< Windows clipping upper widget (can not be <0)
		Vector2i size// !< Windows clipping upper widget (can not be <0 and >this.windowsSize)
) {
	public DrawProperty() {
		this(Vector2i.ZERO, Vector2i.ZERO, Vector2i.ZERO);
	}
	
	public DrawProperty(final Vector2i windowsSize, final Vector2i origin, final Vector2i size) {
		this.windowsSize = windowsSize;
		this.origin = origin;
		this.size = size;
	}
	
	public DrawProperty withLimit(final Vector2f originIn, final Vector2f size) {
		Vector2i tmpSize = this.size.add(this.origin);
		Vector2i origin = this.origin.max((int) originIn.x(), (int) originIn.y());
		tmpSize = tmpSize.min((int) (originIn.x() + size.x()), (int) (originIn.y() + size.y()));
		tmpSize = tmpSize.less(origin);
		return new DrawProperty(this.windowsSize, origin, tmpSize);
	}
	
	@Override
	public String toString() {
		return "DrawProperty [windowsSize=" + this.windowsSize + ", start=" + this.origin + ", stop=" + this.origin.add(this.size) + "]";
	}
}
