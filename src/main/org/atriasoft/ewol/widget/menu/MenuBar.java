package org.atriasoft.ewol.widget.menu;

import java.util.ArrayList;
import java.util.List;
import java.util.function.Supplier;

import org.atriasoft.esignal.Connection;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.DimensionInsets;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Windows;
import org.atriasoft.ewol.Gravity;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Horizontal menu bar that contains labelled buttons, each opening a {@link MenuPopup} dropdown.
 *
 * <p>When a menu is open and the user hovers over another button, the open menu
 * switches to the new dropdown (walk-through behavior).</p>
 *
 * <p>Usage:</p>
 * <pre>
 * MenuBar.create()
 *     .menu("File", () -> MenuPopup.create()
 *         .item("New", "file-new", "ctrl+n", () -> newFile())
 *         .item("Open", "folder-open", "ctrl+o", () -> openFile())
 *         .separator()
 *         .item("Quit", null, "alt+F4", () -> quit())
 *     )
 *     .menu("Edit", () -> MenuPopup.create()
 *         .item("Cut", "cut", "ctrl+x", () -> cut())
 *         .item("Copy", "copy", "ctrl+c", () -> copy())
 *         .item("Paste", "paste", "ctrl+v", () -> paste())
 *     );
 * </pre>
 */
public class MenuBar extends Sizer {
	private static final Logger LOGGER = LoggerFactory.getLogger(MenuBar.class);

	private static final Color BAR_BG_COLOR = new Color(0xF5, 0xF5, 0xF5, 0xFF);
	private static final Color BAR_BORDER_COLOR = new Color(0xD0, 0xD0, 0xD0, 0xFF);
	private static final Color BUTTON_COLOR = new Color(0xF5, 0xF5, 0xF5, 0xFF);
	private static final Color BUTTON_HOVER_COLOR = new Color(0xE3, 0xF2, 0xFD, 0xFF);
	private static final Color BUTTON_ACTIVE_COLOR = new Color(0xBB, 0xDE, 0xFB, 0xFF);
	private static final Color BUTTON_BORDER_COLOR = Color.NONE;

	private MenuPopup activeMenu = null;
	private Button activeButton = null;

	private final List<MenuEntry> entries = new ArrayList<>();
	private final List<Connection> connections = new ArrayList<>();

	private static class MenuEntry {
		final String label;
		final Supplier<MenuPopup> menuFactory;
		Button button;

		MenuEntry(final String label, final Supplier<MenuPopup> menuFactory) {
			this.label = label;
			this.menuFactory = menuFactory;
		}
	}

	public MenuBar() {
		super(DisplayMode.HORIZONTAL);
		setPropertyExpand(Vector2b.TRUE_FALSE);
		setPropertyFill(Vector2b.TRUE);
	}

	// ========================================================================
	// Factory and fluent API
	// ========================================================================

	public static MenuBar create() {
		return new MenuBar();
	}

	/**
	 * Add a menu entry to the bar.
	 * @param label the button label
	 * @param menuFactory factory creating the MenuPopup (called each time the menu opens)
	 * @return this bar for chaining
	 */
	public MenuBar menu(final String label, final Supplier<MenuPopup> menuFactory) {
		final MenuEntry entry = new MenuEntry(label, menuFactory);

		final Button btn = new Button();
		btn.setPropertyExpand(Vector2b.FALSE);
		btn.setPropertyFill(Vector2b.FALSE);
		btn.setPropertyBorderWidth(new DimensionInsets(0));
		btn.setPropertyBorderColor(BUTTON_BORDER_COLOR);
		btn.setPropertyColor(BUTTON_COLOR);
		btn.setPropertyPadding(new DimensionInsets(4, 8, 4, 8));
		btn.setPropertyMargin(new DimensionInsets(0));

		final Label labelWidget = new Label(label);
		labelWidget.setPropertyFontSize(12);
		labelWidget.setPropertyFill(Vector2b.FALSE);
		labelWidget.setPropertyExpand(Vector2b.FALSE);
		labelWidget.setPropertyGravity(Gravity.CENTER);
		btn.setSubWidget(labelWidget);

		entry.button = btn;

		this.connections.add(btn.signalClick.connect(() -> onButtonClicked(entry)));
		this.connections.add(btn.signalEnter.connect(() -> {
			onButtonHover(entry);
			if (this.activeButton != btn) {
				btn.setPropertyColor(BUTTON_HOVER_COLOR);
			}
		}));
		this.connections.add(btn.signalLeave.connect(() -> {
			if (this.activeButton != btn) {
				btn.setPropertyColor(BUTTON_COLOR);
			}
		}));

		this.entries.add(entry);
		subWidgetAdd(btn);
		return this;
	}

	// ========================================================================
	// Menu logic
	// ========================================================================

	private void onButtonClicked(final MenuEntry entry) {
		if (this.activeMenu != null && this.activeButton == entry.button) {
			// Toggle: close the active menu if clicking the same button
			this.activeMenu.close();
			this.activeMenu = null;
			this.activeButton = null;
			return;
		}
		openMenu(entry);
	}

	private void onButtonHover(final MenuEntry entry) {
		// Walk-through: switch menus when hovering with one already open
		if (this.activeMenu != null && this.activeButton != entry.button) {
			this.activeMenu.close();
			openMenu(entry);
		}
	}

	/**
	 * Called by MenuPopup when the mouse moves outside the popup area.
	 * Checks if the position is over one of the menu bar buttons and
	 * triggers walk-through if so.
	 * @param pos absolute position
	 * @return true if the position is over the menu bar area
	 */
	boolean handleHoverAt(final Vector2f pos) {
		for (final MenuEntry entry : this.entries) {
			final Button btn = entry.button;
			final Vector2f btnOrigin = btn.getOrigin();
			final Vector2f btnSize = btn.getSize();
			final boolean over = pos.x() >= btnOrigin.x()
					&& pos.x() <= btnOrigin.x() + btnSize.x()
					&& pos.y() >= btnOrigin.y()
					&& pos.y() <= btnOrigin.y() + btnSize.y();
			if (over && this.activeMenu != null && this.activeButton != btn) {
				this.activeMenu.close();
				openMenu(entry);
				return true;
			}
		}
		return false;
	}

	private void openMenu(final MenuEntry entry) {
		final Windows windows = getWindows();
		if (windows == null) {
			LOGGER.error("Cannot open menu: no windows available");
			return;
		}

		final MenuPopup menu = entry.menuFactory.get();
		menu.anchorBelow(entry.button);
		menu.setMenuBar(this);
		final Button btn = entry.button;
		btn.setPropertyColor(BUTTON_ACTIVE_COLOR);
		this.connections.add(menu.signalClosed.connect(() -> {
			if (this.activeButton == btn) {
				btn.setPropertyColor(BUTTON_COLOR);
			}
			this.activeMenu = null;
			this.activeButton = null;
		}));
		menu.show(windows);
		this.activeMenu = menu;
		this.activeButton = btn;
	}
}
