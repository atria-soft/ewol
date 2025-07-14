package org.atriasoft.ewol.compositing;

/** @file
 * @author Edouard DUPIN
 * @copyright 2025, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

public abstract class CompositingDraw extends Compositing implements CompositingDrawInterface {
	
	public static final CompositingDraw createGC() {
		return new CompositingGC();
	}

}
