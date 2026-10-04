package org.atriasoft.ewol.widget.menu;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.compositing.CompositingGC;
import org.atriasoft.ewol.compositing.CompositingSVG;
import org.atriasoft.ewol.compositing.CompositingText;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.gale.key.KeyStatus;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * A menu item widget with optional icon, text label, and shortcut key badges.
 *
 * <p>Layout: {@code [icon 16x16 | text (expand) | shortcut badges]}</p>
 *
 * <p>Supports enabled/disabled state and hover highlighting.</p>
 */
public class MenuItem extends Widget {
	private static final Logger LOGGER = LoggerFactory.getLogger(MenuItem.class);

	// Dimensions
	private static final float ITEM_HEIGHT = 28.0f;
	private static final float ICON_SIZE = 16.0f;
	private static final float ICON_MARGIN_RIGHT = 8.0f;
	private static final float BADGE_MARGIN_LEFT = 16.0f;
	private static final float BADGE_GAP = 3.0f;
	private static final float BADGE_PADDING_H = 4.0f;
	private static final float BADGE_PADDING_V = 2.0f;
	private static final float BADGE_CORNER_RADIUS = 3.0f;
	private static final float HORIZONTAL_PADDING = 10.0f;
	private static final int FONT_SIZE = 12;
	private static final int BADGE_FONT_SIZE = 11;

	// Colors
	private static final Color BG_HOVER = new Color(0xE3, 0xF2, 0xFD, 0xFF);
	private static final Color TEXT_COLOR = new Color(0x20, 0x20, 0x20, 0xFF);
	private static final Color TEXT_DISABLED = new Color(0xA0, 0xA0, 0xA0, 0xFF);
	private static final Color BADGE_BG = new Color(0xE0, 0xE0, 0xE0, 0xFF);
	private static final Color BADGE_BORDER = new Color(0xB0, 0xB0, 0xB0, 0xFF);
	private static final Color BADGE_TEXT = new Color(0x40, 0x40, 0x40, 0xFF);
	private static final Color BADGE_TEXT_DISABLED = new Color(0xA0, 0xA0, 0xA0, 0xFF);
	private static final Color BADGE_BG_DISABLED = new Color(0xEC, 0xEC, 0xEC, 0xFF);

	// Signal
	public final SignalEmpty signalClick = new SignalEmpty();

	// State
	private boolean itemEnabled = true;
	private boolean hovered = false;
	private boolean reserveIconSpace = false;
	private String text = "";
	private String iconName = null;
	private String shortcut = null;
	private List<String> shortcutTokens = Collections.emptyList();

	// Drawing
	private final CompositingGC backgroundDraw = new CompositingGC();
	private final CompositingText textDraw = new CompositingText();
	private final CompositingGC badgeDraw = new CompositingGC();
	private final CompositingText badgeTextDraw = new CompositingText();
	private CompositingSVG iconDraw = null;

	// Stored connections
	private final List<Connection> fluentConnections = new ArrayList<>();

	public MenuItem() {
		setMouseLimit(1);
	}

	// ========================================================================
	// Factory and fluent API
	// ========================================================================

	/**
	 * Create a menu item with text.
	 * @param text the label text
	 * @return a new MenuItem
	 */
	public static MenuItem create(final String text) {
		final MenuItem item = new MenuItem();
		item.text = text;
		return item;
	}

	/**
	 * Set the icon name (loads from THEME/icon/).
	 * @param iconName icon name without .svg extension, or null
	 * @return this item for chaining
	 */
	public MenuItem icon(final String iconName) {
		this.iconName = iconName;
		// The previous icon is drawn by this item only: its texture goes now.
		if (this.iconDraw != null) {
			this.iconDraw.release();
		}
		if (iconName != null && !iconName.isEmpty()) {
			this.iconDraw = new CompositingSVG();
			final String svgData = Uri.getAllDataString(new Uri("THEME", "icon/" + iconName + ".svg", "ewol"));
			if (svgData != null) {
				this.iconDraw.setSource(svgData, new Vector2i((int) ICON_SIZE, (int) ICON_SIZE));
			}
		} else {
			this.iconDraw = null;
		}
		markToRedraw();
		requestUpdateSize();
		return this;
	}

	/**
	 * Set the shortcut display string.
	 * @param shortcut e.g., "ctrl+s"
	 * @return this item for chaining
	 */
	public MenuItem shortcut(final String shortcut) {
		this.shortcut = shortcut;
		this.shortcutTokens = ShortcutFormatter.parse(shortcut);
		markToRedraw();
		requestUpdateSize();
		return this;
	}

	/**
	 * Set enabled/disabled state.
	 * @param enabled true to enable
	 * @return this item for chaining
	 */
	public MenuItem enabled(final boolean enabled) {
		this.itemEnabled = enabled;
		markToRedraw();
		return this;
	}

	/**
	 * Connect a callback for when this item is selected.
	 * @param action the callback
	 * @return this item for chaining
	 */
	public MenuItem onSelect(final Runnable action) {
		this.fluentConnections.add(this.signalClick.connect(action));
		return this;
	}

	public boolean isItemEnabled() {
		return this.itemEnabled;
	}

	public String getText() {
		return this.text;
	}

	public boolean hasIcon() {
		return this.iconDraw != null;
	}

	/**
	 * Set whether this item should reserve space for an icon column,
	 * even if it has no icon itself. Used by MenuPopup to align all items
	 * when at least one item in the menu has an icon.
	 * @param reserve true to reserve icon space
	 */
	public void setReserveIconSpace(final boolean reserve) {
		if (this.reserveIconSpace != reserve) {
			this.reserveIconSpace = reserve;
			markToRedraw();
			requestUpdateSize();
		}
	}

	// ========================================================================
	// Hover state (used by MenuPopup for manual hover routing)
	// ========================================================================

	public void setHovered(final boolean hovered) {
		if (this.hovered != hovered) {
			this.hovered = hovered;
			markToRedraw();
		}
	}

	public boolean isHovered() {
		return this.hovered;
	}

	// ========================================================================
	// Layout
	// ========================================================================

	@Override
	public void calculateMinMaxSize() {
		this.textDraw.setFontSize(FONT_SIZE);
		final Vector2f textSize = this.textDraw.calculateSize(this.text);

		float width = HORIZONTAL_PADDING;
		// Icon area (reserve space if this item has an icon, or if aligned with other items that do)
		if (this.iconDraw != null || this.reserveIconSpace) {
			width += ICON_SIZE + ICON_MARGIN_RIGHT;
		}
		// Text
		width += textSize.x();
		// Badges
		if (!this.shortcutTokens.isEmpty()) {
			width += BADGE_MARGIN_LEFT;
			this.badgeTextDraw.setFontSize(BADGE_FONT_SIZE);
			for (final String token : this.shortcutTokens) {
				final Vector2f tokenSize = this.badgeTextDraw.calculateSize(token);
				final float badgeWidth = Math.max(18.0f, tokenSize.x() + BADGE_PADDING_H * 2);
				width += badgeWidth + BADGE_GAP;
			}
			width -= BADGE_GAP;
		}
		width += HORIZONTAL_PADDING;

		this.minSize = new Vector2f(width, ITEM_HEIGHT);
		this.maxSize = new Vector2f(Float.MAX_VALUE, ITEM_HEIGHT);
	}

	// ========================================================================
	// Event handling
	// ========================================================================

	@Override
	public boolean onEventInput(final EventInput event) {
		final Vector2f relPos = relativePosition(event.pos());
		final boolean over = isInsideBounds(relPos);

		if (event.status() == KeyStatus.leave) {
			if (this.hovered) {
				this.hovered = false;
				markToRedraw();
			}
			return true;
		}

		if (event.inputId() == 0) {
			if (over && !this.hovered) {
				this.hovered = true;
				markToRedraw();
			} else if (!over && this.hovered) {
				this.hovered = false;
				markToRedraw();
			}
			return over;
		}

		if (event.inputId() == 1 && event.status() == KeyStatus.pressSingle && over) {
			if (this.itemEnabled) {
				this.signalClick.emit();
				return true;
			}
		}
		return false;
	}

	private boolean isInsideBounds(final Vector2f relPos) {
		return relPos.x() >= 0 && relPos.y() >= 0
				&& relPos.x() < this.size.x() && relPos.y() < this.size.y();
	}

	// ========================================================================
	// Rendering
	// ========================================================================

	@Override
	public void onRegenerateDisplay() {
		if (!needRedraw()) {
			return;
		}
		this.backgroundDraw.clear();
		this.textDraw.clear();
		this.textDraw.reset();
		this.badgeDraw.clear();
		this.badgeTextDraw.clear();
		this.badgeTextDraw.reset();

		final Color fgColor = this.itemEnabled ? TEXT_COLOR : TEXT_DISABLED;

		// 1. Hover background
		if (this.hovered && this.itemEnabled) {
			this.backgroundDraw.setPaintFillColor(BG_HOVER);
			this.backgroundDraw.setPaintStrokeWidth(0);
			this.backgroundDraw.addRectangle(Vector2f.ZERO, this.size);
		}
		this.backgroundDraw.flush();

		float xCursor = HORIZONTAL_PADDING;

		// 2. Icon (advance cursor if icon is present or space is reserved for alignment)
		if (this.iconDraw != null) {
			this.iconDraw.clear();
			final float iconY = (this.size.y() - ICON_SIZE) * 0.5f;
			this.iconDraw.setPos(new Vector2f(xCursor, iconY));
			this.iconDraw.print(new Vector2f(ICON_SIZE, ICON_SIZE));
			this.iconDraw.flush();
		}
		if (this.iconDraw != null || this.reserveIconSpace) {
			xCursor += ICON_SIZE + ICON_MARGIN_RIGHT;
		}

		// 3. Text
		this.textDraw.setFontSize(FONT_SIZE);
		this.textDraw.setColor(fgColor);
		final float textHeight = this.textDraw.getHeight();
		final float textY = (this.size.y() - textHeight) * 0.5f;
		this.textDraw.setPos(new Vector2f(xCursor, textY));
		this.textDraw.print(this.text);
		this.textDraw.flush();

		// 4. Shortcut badges (right-aligned)
		if (!this.shortcutTokens.isEmpty()) {
			this.badgeTextDraw.setFontSize(BADGE_FONT_SIZE);
			final Color badgeFg = this.itemEnabled ? BADGE_TEXT : BADGE_TEXT_DISABLED;
			final Color badgeBg = this.itemEnabled ? BADGE_BG : BADGE_BG_DISABLED;

			// Calculate total width of all badges
			float totalBadgesWidth = 0;
			final List<Float> badgeWidths = new ArrayList<>();
			for (final String token : this.shortcutTokens) {
				final Vector2f tokenSize = this.badgeTextDraw.calculateSize(token);
				final float badgeWidth = Math.max(18.0f, tokenSize.x() + BADGE_PADDING_H * 2);
				badgeWidths.add(badgeWidth);
				totalBadgesWidth += badgeWidth + BADGE_GAP;
			}
			totalBadgesWidth -= BADGE_GAP;

			float badgeX = this.size.x() - HORIZONTAL_PADDING - totalBadgesWidth;
			for (int i = 0; i < this.shortcutTokens.size(); i++) {
				final String token = this.shortcutTokens.get(i);
				final float badgeWidth = badgeWidths.get(i);
				final Vector2f tokenSize = this.badgeTextDraw.calculateSize(token);
				final float badgeHeight = tokenSize.y() + BADGE_PADDING_V * 2;
				final float badgeY = (this.size.y() - badgeHeight) * 0.5f;

				// Badge background
				this.badgeDraw.setPaintFillColor(badgeBg);
				this.badgeDraw.setPaintStrokeColor(BADGE_BORDER);
				this.badgeDraw.setPaintStrokeWidth(1.0f);
				this.badgeDraw.addRectangle(
						new Vector2f(badgeX, badgeY),
						new Vector2f(badgeWidth, badgeHeight),
						new Vector2f(BADGE_CORNER_RADIUS, BADGE_CORNER_RADIUS));

				// Badge text (centered in badge)
				final float textXInBadge = badgeX + (badgeWidth - tokenSize.x()) * 0.5f;
				final float textYInBadge = badgeY + BADGE_PADDING_V;
				this.badgeTextDraw.setColor(badgeFg);
				this.badgeTextDraw.setPos(new Vector2f(textXInBadge, textYInBadge));
				this.badgeTextDraw.print(token);

				badgeX += badgeWidth + BADGE_GAP;
			}
			this.badgeDraw.flush();
			this.badgeTextDraw.flush();
		}
	}

	@Override
	protected void onDraw() {
		this.backgroundDraw.draw(true);
		if (this.iconDraw != null) {
			this.iconDraw.draw(true);
		}
		this.textDraw.draw(true);
		this.badgeDraw.draw(true);
		this.badgeTextDraw.draw(true);
	}
}
