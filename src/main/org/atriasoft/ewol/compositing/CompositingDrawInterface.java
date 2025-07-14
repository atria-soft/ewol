/** @file
 * @author Edouard DUPIN
 * @copyright 2025, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.compositing;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.math.Vector2f;

public interface CompositingDrawInterface {
	void clear();
	
	void flush();
	
	void setPaintFillColor(final Color color);
	
	void setPaintStrokeColor(final Color color);
	
	void setPaintStrokeWidth(final float width);
	
	void addLine(final Vector2f startPos, final Vector2f stopPos);
	
	void addRectangle(final Vector2f position, final Vector2f size);
	
	void addRectangle(final Vector2f position, final Vector2f size, final Vector2f roundedCorner);
	
	void addCircle(final Vector2f position, final float radius);
	
	void addEllipse(final Vector2f center, final Vector2f radius);
	
}
