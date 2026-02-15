package org.atriasoft.ewol.widget.menu;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.compositing.CompositingGC;
import org.atriasoft.ewol.widget.Widget;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * A simple horizontal separator line for use in menus.
 * Height is fixed at 9px (4px top margin + 1px line + 4px bottom margin).
 */
public class MenuSeparator extends Widget {
	private static final Logger LOGGER = LoggerFactory.getLogger(MenuSeparator.class);

	private static final float SEPARATOR_HEIGHT = 9.0f;
	private static final float LINE_THICKNESS = 1.0f;
	private static final Color LINE_COLOR = new Color(0xD0, 0xD0, 0xD0, 0xFF);

	private final CompositingGC lineDraw = new CompositingGC();

	public MenuSeparator() {
		// No focus, no mouse interaction
	}

	/**
	 * Factory method.
	 * @return a new MenuSeparator
	 */
	public static MenuSeparator create() {
		return new MenuSeparator();
	}

	@Override
	public void calculateMinMaxSize() {
		this.minSize = new Vector2f(0, SEPARATOR_HEIGHT);
		this.maxSize = new Vector2f(Float.MAX_VALUE, SEPARATOR_HEIGHT);
	}

	@Override
	public void onRegenerateDisplay() {
		if (!needRedraw()) {
			return;
		}
		this.lineDraw.clear();

		// Draw a horizontal line centered vertically
		final float lineY = (this.size.y() - LINE_THICKNESS) * 0.5f;
		final float marginH = 8.0f;
		final Vector2f linePos = new Vector2f(marginH, lineY);
		final Vector2f lineSize = new Vector2f(this.size.x() - marginH * 2, LINE_THICKNESS);

		this.lineDraw.setPaintFillColor(LINE_COLOR);
		this.lineDraw.setPaintStrokeWidth(0);
		this.lineDraw.addRectangle(linePos, lineSize);
		this.lineDraw.flush();
	}

	@Override
	protected void onDraw() {
		this.lineDraw.draw(true);
	}
}
