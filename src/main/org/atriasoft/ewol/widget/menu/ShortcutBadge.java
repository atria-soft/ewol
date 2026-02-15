package org.atriasoft.ewol.widget.menu;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.compositing.CompositingGC;
import org.atriasoft.ewol.compositing.CompositingText;
import org.atriasoft.ewol.widget.Widget;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Small badge widget that displays a keyboard key label (e.g., "Ctrl", "S", "F5")
 * inside a rounded rectangle, styled like a physical key cap.
 */
public class ShortcutBadge extends Widget {
	private static final Logger LOGGER = LoggerFactory.getLogger(ShortcutBadge.class);

	private static final Color BG_COLOR = new Color(0xE0, 0xE0, 0xE0, 0xFF);
	private static final Color BORDER_COLOR = new Color(0xB0, 0xB0, 0xB0, 0xFF);
	private static final Color TEXT_COLOR = new Color(0x40, 0x40, 0x40, 0xFF);
	private static final float PADDING_H = 4.0f;
	private static final float PADDING_V = 2.0f;
	private static final float CORNER_RADIUS = 3.0f;
	private static final int FONT_SIZE = 11;
	private static final float MIN_WIDTH = 18.0f;

	private String text = "";
	private final CompositingGC backgroundDraw = new CompositingGC();
	private final CompositingText textDraw = new CompositingText();

	public ShortcutBadge() {
		// No focus, no mouse interaction
	}

	/**
	 * Factory method.
	 * @param text the key label to display
	 * @return a new ShortcutBadge
	 */
	public static ShortcutBadge create(final String text) {
		final ShortcutBadge badge = new ShortcutBadge();
		badge.text = text;
		return badge;
	}

	/**
	 * Fluent setter for the key label text.
	 * @param text the key label
	 * @return this badge for chaining
	 */
	public ShortcutBadge text(final String text) {
		this.text = text;
		markToRedraw();
		requestUpdateSize();
		return this;
	}

	public String getText() {
		return this.text;
	}

	@Override
	public void calculateMinMaxSize() {
		this.textDraw.setFontSize(FONT_SIZE);
		final Vector2f textSize = this.textDraw.calculateSize(this.text);
		final float width = Math.max(MIN_WIDTH, textSize.x() + PADDING_H * 2);
		final float height = textSize.y() + PADDING_V * 2;
		this.minSize = new Vector2f(width, height);
		this.maxSize = this.minSize;
	}

	@Override
	public void onRegenerateDisplay() {
		if (!needRedraw()) {
			return;
		}
		this.backgroundDraw.clear();
		this.textDraw.clear();
		this.textDraw.reset();

		this.textDraw.setFontSize(FONT_SIZE);
		final Vector2f textSize = this.textDraw.calculateSize(this.text);
		final float badgeWidth = Math.max(MIN_WIDTH, textSize.x() + PADDING_H * 2);
		final float badgeHeight = textSize.y() + PADDING_V * 2;

		// Center badge in widget
		final float xOffset = (this.size.x() - badgeWidth) * 0.5f;
		final float yOffset = (this.size.y() - badgeHeight) * 0.5f;
		final Vector2f pos = new Vector2f(xOffset, yOffset);
		final Vector2f badgeSize = new Vector2f(badgeWidth, badgeHeight);

		// Draw rounded rectangle background
		this.backgroundDraw.setPaintFillColor(BG_COLOR);
		this.backgroundDraw.setPaintStrokeColor(BORDER_COLOR);
		this.backgroundDraw.setPaintStrokeWidth(1.0f);
		this.backgroundDraw.addRectangle(pos, badgeSize, new Vector2f(CORNER_RADIUS, CORNER_RADIUS));
		this.backgroundDraw.flush();

		// Draw centered text
		final float textX = xOffset + (badgeWidth - textSize.x()) * 0.5f;
		final float textY = yOffset + PADDING_V;
		this.textDraw.setColor(TEXT_COLOR);
		this.textDraw.setPos(new Vector2f(textX, textY));
		this.textDraw.print(this.text);
		this.textDraw.flush();
	}

	@Override
	protected void onDraw() {
		this.backgroundDraw.draw(true);
		this.textDraw.draw(true);
	}
}
