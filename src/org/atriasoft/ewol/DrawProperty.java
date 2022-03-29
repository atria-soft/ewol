package org.atriasoft.ewol;

import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.etk.math.Vector3i;

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
		Vector3i windowsSize, // !< Windows complete size
		Vector3i origin, // !< Windows clipping upper widget (can not be <0)
		Vector3i size// !< Windows clipping upper widget (can not be <0 and >this.windowsSize)
) {
	public DrawProperty() {
		this(Vector3i.ZERO, Vector3i.ZERO, Vector3i.ZERO);
	}
	
	public DrawProperty(final Vector3i windowsSize, final Vector3i origin, final Vector3i size) {
		this.windowsSize = windowsSize;
		this.origin = origin;
		this.size = size;
	}
	
	public DrawProperty withLimit(final Vector3f originIn, final Vector3f size) {
		Vector3i tmpSize = this.size.add(this.origin);
		Vector3i origin = this.origin.max((int) originIn.x(), (int) originIn.y(), (int) originIn.z());
		tmpSize = tmpSize.min((int) (originIn.x() + size.x()), (int) (originIn.y() + size.y()), (int) (originIn.z() + size.z()));
		tmpSize = tmpSize.less(origin);
		return new DrawProperty(this.windowsSize, origin, tmpSize);
	}
	
	@Override
	public String toString() {
		return "DrawProperty [windowsSize=" + this.windowsSize + ", start=" + this.origin + ", stop=" + this.origin.add(this.size) + "]";
	}
}
