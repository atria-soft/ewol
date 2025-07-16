/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.compositing;

import org.atriasoft.etk.BorderRadius;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Insets;
import org.atriasoft.etk.math.Vector2f;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class CompositingGC extends CompositingDrawing {
	private static final Logger LOGGER = LoggerFactory.getLogger(CompositingGC.class);
	
	@Override
	public void setPaintFillColor(final Color color) {
		setColor(color);
	}
	
	@Override
	public void setPaintStrokeColor(final Color color) {
		setColorBg(color);
	}

	float strokeSize = 0;

	@Override
	public void setPaintStrokeWidth(final float width) {
		this.strokeSize = width;
	}
	
	@Override
	public void addLine(final Vector2f startPos, final Vector2f stopPos) {
		setPos(startPos);
		lineTo(stopPos);
	}
	
	@Override
	public void addRectangle(final Vector2f position, final Vector2f size) {
		setPos(position);
		rectangle(position.add(size));
		if (this.strokeSize > 0) {
			rectangleBorder(position.add(size), this.strokeSize);
		}
	}
	
	@Override
	public void addRectangle(final Vector2f position, final Vector2f size, final Vector2f roundedCorner) {
		if (roundedCorner == null || roundedCorner.x() <= 0) {
			addRectangle(position, size);
		} else {
			setPos(position);
			rectangleRadius(position.add(size), roundedCorner.x());
			if (this.strokeSize > 0) {
				rectangleBorderRadius(position.add(size), this.strokeSize, roundedCorner.x());
			}
		}
	}
	
	@Override
	public void addRectangle(
			final Vector2f position,
			final Vector2f size,
			final Insets thickness,
			final BorderRadius radius) {
		if (radius == null || radius.isZero()) {
			//addRectangle(position, size);
			//throw new RuntimeException("Not implemented ...");
		} else {
			setPos(position);
			rectangleRadius(position.add(size), thickness, radius);
			if (!thickness.isZero()) {
				rectangleBorderRadius(position.add(size), thickness, radius);
			}
		}
	}
	
	@Override
	public void addCircle(final Vector2f position, final float radius) {
		setPos(position);
		circle(radius);

	}
	
	@Override
	public void addEllipse(final Vector2f center, final Vector2f radius) {
		// TODO Auto-generated method stub

	}
}