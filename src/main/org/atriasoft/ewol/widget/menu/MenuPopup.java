package org.atriasoft.ewol.widget.menu;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.DimensionInsets;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.event.EventEntry;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.event.InputSystem;
import org.atriasoft.ewol.widget.Box;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.ewol.widget.Windows;
import org.atriasoft.gale.key.KeyStatus;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Menu popup overlay widget.
 *
 * <p>Follows the SelectPopup pattern: fills the entire window, but only
 * renders the popup box at the anchor position. Clicking outside closes
 * the menu.</p>
 *
 * <p>Usage:</p>
 * <pre>
 * MenuPopup.create()
 *     .item("Cut", "cut", "ctrl+x", () -> cut())
 *     .item("Copy", "copy", "ctrl+c", () -> copy())
 *     .separator()
 *     .item("Select All", null, "ctrl+a", () -> selectAll())
 *     .anchorAt(mousePosition)
 *     .show(getWindows());
 * </pre>
 */
public class MenuPopup extends Widget {
	private static final Logger LOGGER = LoggerFactory.getLogger(MenuPopup.class);

	private static final float POPUP_MAX_HEIGHT = 400.0f;
	private static final float POPUP_MIN_WIDTH = 150.0f;
	private static final Color POPUP_BG_COLOR = Color.WHITE;
	private static final Color POPUP_BORDER_COLOR = new Color(0xC0, 0xC0, 0xC0, 0xFF);

	// Signal
	public final SignalEmpty signalClosed = new SignalEmpty();

	// Anchor
	private Vector2f anchorOrigin = Vector2f.ZERO;
	private Vector2f anchorSize = Vector2f.ZERO;

	// Internal UI
	private final Box popupBox;
	private final Sizer itemsContainer;

	// Items (kept for event routing)
	private final List<Widget> menuWidgets = new ArrayList<>();

	// Signal connections
	private final List<Connection> connections = new ArrayList<>();

	// Computed popup position
	private Vector2f popupOrigin = Vector2f.ZERO;
	private Vector2f popupSize = Vector2f.ZERO;

	// Optional parent menu bar for walk-through hover
	private MenuBar menuBar = null;

	public MenuPopup() {
		this.propertyCanFocus = true;
		setMouseLimit(1);
		setPropertyExpand(Vector2b.TRUE);
		setPropertyFill(Vector2b.TRUE);

		this.popupBox = new Box();
		this.popupBox.setPropertyExpand(Vector2b.FALSE);
		this.popupBox.setPropertyFill(Vector2b.FALSE);
		this.popupBox.setPropertyBorderWidth(new DimensionInsets(1));
		this.popupBox.setPropertyBorderColor(POPUP_BORDER_COLOR);
		this.popupBox.setPropertyColor(POPUP_BG_COLOR);
		this.popupBox.setPropertyPadding(new DimensionInsets(4));
		this.popupBox.setPropertyMargin(new DimensionInsets(0));

		this.itemsContainer = Sizer.vertical();
		this.itemsContainer.setPropertyExpand(Vector2b.TRUE);
		this.itemsContainer.setPropertyFill(Vector2b.TRUE);

		this.popupBox.setSubWidget(this.itemsContainer);
	}

	// ========================================================================
	// Factory and fluent API
	// ========================================================================

	public static MenuPopup create() {
		return new MenuPopup();
	}

	/**
	 * Add a menu item with text and action.
	 * @param text the label
	 * @param action callback on click
	 * @return this popup for chaining
	 */
	public MenuPopup item(final String text, final Runnable action) {
		return item(text, null, null, action);
	}

	/**
	 * Add a menu item with text, icon, and action.
	 * @param text the label
	 * @param iconName icon name (nullable)
	 * @param action callback on click
	 * @return this popup for chaining
	 */
	public MenuPopup item(final String text, final String iconName, final Runnable action) {
		return item(text, iconName, null, action);
	}

	/**
	 * Add a menu item with text, icon, shortcut, and action.
	 * @param text the label
	 * @param iconName icon name (nullable)
	 * @param shortcut shortcut string (nullable, e.g., "ctrl+s")
	 * @param action callback on click
	 * @return this popup for chaining
	 */
	public MenuPopup item(final String text, final String iconName, final String shortcut, final Runnable action) {
		final MenuItem menuItem = MenuItem.create(text);
		if (iconName != null) {
			menuItem.icon(iconName);
		}
		if (shortcut != null) {
			menuItem.shortcut(shortcut);
		}
		this.connections.add(menuItem.signalClick.connect(() -> {
			if (action != null) {
				action.run();
			}
			close();
		}));
		return addWidget(menuItem);
	}

	/**
	 * Add a pre-built MenuItem.
	 * @param menuItem the item
	 * @return this popup for chaining
	 */
	public MenuPopup item(final MenuItem menuItem) {
		this.connections.add(menuItem.signalClick.connect(this::close));
		return addWidget(menuItem);
	}

	/**
	 * Add a separator line.
	 * @return this popup for chaining
	 */
	public MenuPopup separator() {
		return addWidget(MenuSeparator.create());
	}

	/**
	 * Add a disabled (grayed-out) menu item.
	 * @param text the label
	 * @return this popup for chaining
	 */
	public MenuPopup disabledItem(final String text) {
		return disabledItem(text, null);
	}

	/**
	 * Add a disabled (grayed-out) menu item with icon.
	 * @param text the label
	 * @param iconName icon name (nullable)
	 * @return this popup for chaining
	 */
	public MenuPopup disabledItem(final String text, final String iconName) {
		final MenuItem menuItem = MenuItem.create(text).enabled(false);
		if (iconName != null) {
			menuItem.icon(iconName);
		}
		return addWidget(menuItem);
	}

	private MenuPopup addWidget(final Widget widget) {
		widget.setPropertyExpand(Vector2b.TRUE_FALSE);
		widget.setPropertyFill(Vector2b.TRUE);
		this.menuWidgets.add(widget);
		this.itemsContainer.subWidgetAdd(widget);
		return this;
	}

	// ========================================================================
	// Positioning
	// ========================================================================

	/**
	 * Set the parent menu bar for walk-through hover support.
	 * When set, hover events outside the popup are forwarded to the menu bar
	 * so the user can glide between menus without clicking.
	 * @param menuBar the parent menu bar
	 */
	void setMenuBar(final MenuBar menuBar) {
		this.menuBar = menuBar;
	}

	/**
	 * Anchor the popup at a specific position (e.g., mouse position for context menu).
	 * @param position the anchor point
	 * @return this popup for chaining
	 */
	public MenuPopup anchorAt(final Vector2f position) {
		this.anchorOrigin = position;
		this.anchorSize = Vector2f.ZERO;
		return this;
	}

	/**
	 * Anchor the popup below a widget (e.g., for menu bar dropdown).
	 * @param widget the widget to anchor below
	 * @return this popup for chaining
	 */
	public MenuPopup anchorBelow(final Widget widget) {
		this.anchorOrigin = widget.getOrigin();
		this.anchorSize = widget.getSize();
		return this;
	}

	// ========================================================================
	// Show / Close
	// ========================================================================

	/**
	 * Show the menu popup by pushing it onto the window's popup stack.
	 * @param windows the windows to push onto
	 */
	public void show(final Windows windows) {
		if (windows == null) {
			LOGGER.error("Cannot show MenuPopup: windows is null");
			return;
		}
		// Check if any MenuItem has an icon — if so, reserve icon space on all items for alignment
		boolean hasAnyIcon = false;
		for (final Widget widget : this.menuWidgets) {
			if (widget instanceof MenuItem && ((MenuItem) widget).hasIcon()) {
				hasAnyIcon = true;
				break;
			}
		}
		if (hasAnyIcon) {
			for (final Widget widget : this.menuWidgets) {
				if (widget instanceof MenuItem) {
					((MenuItem) widget).setReserveIconSpace(true);
				}
			}
		}
		windows.popUpWidgetPush(this);
	}

	/**
	 * Close the menu popup.
	 */
	public void close() {
		this.signalClosed.emit();
		autoDestroy();
	}

	// ========================================================================
	// Layout (SelectPopup pattern)
	// ========================================================================

	@Override
	public void calculateMinMaxSize() {
		this.minSize = Vector2f.ZERO;
		this.maxSize = new Vector2f(Float.MAX_VALUE, Float.MAX_VALUE);
		this.popupBox.calculateMinMaxSize();
	}

	@Override
	public void onChangeSize() {
		markToRedraw();

		this.popupBox.calculateMinMaxSize();
		final Vector2f popupMinSize = this.popupBox.getCalculateMinSize();

		float popupWidth = Math.max(POPUP_MIN_WIDTH, popupMinSize.x());
		float popupHeight = Math.min(popupMinSize.y(), POPUP_MAX_HEIGHT);

		// Determine position relative to anchor
		// In ewol, Y=0 is bottom, Y increases upward
		final float spaceBelow = this.anchorOrigin.y();
		final float spaceAbove = this.size.y() - (this.anchorOrigin.y() + this.anchorSize.y());

		if (this.anchorSize.equals(Vector2f.ZERO)) {
			// Context menu mode: position at anchor point
			// Try below-right of the anchor point
			this.popupOrigin = new Vector2f(this.anchorOrigin.x(), this.anchorOrigin.y() - popupHeight);
			if (this.popupOrigin.y() < 0) {
				// Not enough space below, position above
				this.popupOrigin = new Vector2f(this.anchorOrigin.x(), this.anchorOrigin.y());
				if (this.popupOrigin.y() + popupHeight > this.size.y()) {
					popupHeight = this.size.y() - this.popupOrigin.y();
				}
			}
		} else {
			// Dropdown mode: position below the anchor widget
			if (spaceBelow >= popupHeight || spaceBelow >= spaceAbove) {
				this.popupOrigin = new Vector2f(this.anchorOrigin.x(), this.anchorOrigin.y() - popupHeight);
				if (this.popupOrigin.y() < 0) {
					popupHeight = this.anchorOrigin.y();
					this.popupOrigin = new Vector2f(this.anchorOrigin.x(), 0);
				}
			} else {
				this.popupOrigin = new Vector2f(this.anchorOrigin.x(), this.anchorOrigin.y() + this.anchorSize.y());
				if (this.popupOrigin.y() + popupHeight > this.size.y()) {
					popupHeight = this.size.y() - this.popupOrigin.y();
				}
			}
		}

		// Clamp X
		if (this.popupOrigin.x() + popupWidth > this.size.x()) {
			this.popupOrigin = this.popupOrigin.withX(Math.max(0, this.size.x() - popupWidth));
		}
		if (this.popupOrigin.x() < 0) {
			this.popupOrigin = this.popupOrigin.withX(0);
		}

		this.popupSize = new Vector2f(popupWidth, popupHeight);

		this.popupBox.setOrigin(this.popupOrigin);
		this.popupBox.setSize(this.popupSize);
		this.popupBox.onChangeSize();
	}

	// ========================================================================
	// Rendering
	// ========================================================================

	@Override
	public void onRegenerateDisplay() {
		// Always propagate to children — a MenuItem may need redraw (hover state)
		// even when the popup itself is not marked for redraw.
		this.popupBox.systemRegenerateDisplay();
	}

	@Override
	public void systemDraw(final DrawProperty displayProp) {
		if (this.propertyHide) {
			return;
		}
		final DrawProperty prop = displayProp.withLimit(this.popupOrigin, this.popupSize);
		this.popupBox.systemDraw(prop);
	}

	// ========================================================================
	// Event routing (SelectPopup pattern)
	// ========================================================================

	@Override
	public Widget getWidgetAtPos(final Vector2f pos) {
		if (this.propertyHide) {
			return null;
		}
		return this;
	}

	@Override
	public boolean systemEventInput(final InputSystem event) {
		final Vector2f pos = event.event().pos();
		final boolean inside = isInsidePopup(pos);

		if (onEventInput(event.event())) {
			return true;
		}

		// Handle hover: manage hover state centrally to ensure proper enter/leave across items
		if (event.event().inputId() == 0) {
			updateHover(inside ? pos : null);
			// When outside popup, forward hover to parent MenuBar for walk-through
			if (!inside && this.menuBar != null) {
				this.menuBar.handleHoverAt(pos);
			}
			return inside;
		}

		if (inside) {
			final Widget target = this.popupBox.getWidgetAtPos(pos);
			if (target != null && target != this && target != this.popupBox) {
				return target.systemEventInput(event);
			}
		}
		return false;
	}

	/**
	 * Update hover state on all MenuItems. The item under the given position
	 * gets hovered, all others get un-hovered.
	 * @param pos the mouse position, or null to clear all hover
	 */
	private void updateHover(final Vector2f pos) {
		for (final Widget widget : this.menuWidgets) {
			if (widget instanceof MenuItem) {
				final MenuItem menuItem = (MenuItem) widget;
				if (pos != null) {
					final Vector2f itemOrigin = menuItem.getOrigin();
					final Vector2f itemSize = menuItem.getSize();
					final boolean over = pos.x() >= itemOrigin.x()
							&& pos.x() <= itemOrigin.x() + itemSize.x()
							&& pos.y() >= itemOrigin.y()
							&& pos.y() <= itemOrigin.y() + itemSize.y();
						menuItem.setHovered(over);
				} else {
					menuItem.setHovered(false);
				}
			}
		}
	}

	@Override
	protected boolean onEventInput(final EventInput event) {
		final boolean inside = isInsidePopup(event.pos());
		// Close on click outside
		if (!inside && event.inputId() == 1 && event.status() == KeyStatus.pressSingle) {
			LOGGER.debug("Click outside menu popup - closing");
			close();
			return true;
		}
		// Close on right-click outside (for context menus)
		if (!inside && event.inputId() == 3 && event.status() == KeyStatus.pressSingle) {
			close();
			return true;
		}
		return false;
	}

	@Override
	protected boolean onEventEntry(final EventEntry event) {
		// Close on Escape key (Unicode 0x1B)
		if (event.status() == KeyStatus.down
				&& event.unicodeData() != null
				&& event.unicodeData() == 0x1B) {
			LOGGER.debug("Escape pressed - closing menu popup");
			close();
			return true;
		}
		return false;
	}

	private boolean isInsidePopup(final Vector2f pos) {
		return pos.x() >= this.popupOrigin.x()
				&& pos.x() <= this.popupOrigin.x() + this.popupSize.x()
				&& pos.y() >= this.popupOrigin.y()
				&& pos.y() <= this.popupOrigin.y() + this.popupSize.y();
	}
}
