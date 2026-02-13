package org.atriasoft.ewol.widget.meta;

import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

import org.atriasoft.esignal.Connection;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.Ewol;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.ewol.widget.WidgetManager;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Manages popovers in a dedicated layer, separate from the popup stack.
 *
 * <p>This avoids the GUI shift caused by pushing popovers onto the popup stack
 * (which triggers a full layout recalculation). Popovers manage their own
 * layout and rendering, similar to how the notification manager handles toasts.</p>
 *
 * <p>Layer order (bottom to top):</p>
 * <ol>
 *   <li>Main widget tree</li>
 *   <li>Popup stack (SelectPopup, FileChooser, etc.)</li>
 *   <li><b>Popover layer (this manager)</b></li>
 *   <li>Toast notifications</li>
 * </ol>
 */
public class PopoverManager {
	private static final Logger LOGGER = LoggerFactory.getLogger(PopoverManager.class);

	private final List<Popover> activePopovers = new CopyOnWriteArrayList<>();
	private Vector2f windowSize = Vector2f.ZERO;

	/**
	 * Add a popover to the layer and lay it out.
	 */
	public void show(final Popover popover) {
		if (popover == null) {
			return;
		}
		this.activePopovers.add(popover);
		layoutPopover(popover);
		markGlobalRedraw();
		LOGGER.debug("Popover shown (active={})", this.activePopovers.size());
	}

	/**
	 * Remove a popover from the layer.
	 */
	public void remove(final Popover popover) {
		if (this.activePopovers.remove(popover)) {
			markGlobalRedraw();
			LOGGER.debug("Popover removed (active={})", this.activePopovers.size());
		}
	}

	/**
	 * Remove all active popovers.
	 */
	public void removeAll() {
		if (!this.activePopovers.isEmpty()) {
			this.activePopovers.clear();
			markGlobalRedraw();
		}
	}

	public boolean hasActivePopovers() {
		return !this.activePopovers.isEmpty();
	}

	// -- Layout --

	/**
	 * Called by Windows when the window size changes.
	 */
	public void onWindowChangeSize(final Vector2f windowSize) {
		this.windowSize = windowSize;
		for (final Popover popover : this.activePopovers) {
			layoutPopover(popover);
		}
	}

	private void layoutPopover(final Popover popover) {
		if (this.windowSize.equals(Vector2f.ZERO)) {
			return;
		}
		popover.calculateMinMaxSize();
		popover.setSize(this.windowSize);
		popover.setOrigin(Vector2f.ZERO);
		popover.onChangeSize();
	}

	// -- Rendering --

	public void onRegenerateDisplay() {
		for (final Popover popover : this.activePopovers) {
			popover.systemRegenerateDisplay();
		}
	}

	public void onDraw(final DrawProperty displayProp) {
		for (final Popover popover : this.activePopovers) {
			popover.systemDraw(displayProp);
		}
	}

	// -- Event routing --

	/**
	 * Check if a popover is at the given position. Returns the topmost
	 * popover's widget (or the popover itself for event handling).
	 */
	public Widget getWidgetAtPos(final Vector2f pos) {
		// Check in reverse order (last = topmost)
		final List<Popover> snapshot = List.copyOf(this.activePopovers);
		for (int i = snapshot.size() - 1; i >= 0; i--) {
			final Popover popover = snapshot.get(i);
			final Widget w = popover.getWidgetAtPos(pos);
			if (w != null) {
				return w;
			}
		}
		return null;
	}

	private void markGlobalRedraw() {
		final WidgetManager wm = Ewol.getContext().getWidgetManager();
		if (wm != null) {
			wm.markDrawingIsNeeded();
		}
	}
}
