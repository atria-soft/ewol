package org.atriasoft.ewol.widget.notification;

import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

import org.atriasoft.esignal.Connection;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.GravityHorizontal;
import org.atriasoft.ewol.GravityVertical;
import org.atriasoft.ewol.event.EventTime;
import org.atriasoft.ewol.Ewol;
import org.atriasoft.ewol.object.EwolObject;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.ewol.widget.WidgetManager;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Default notification manager implementation.
 * Manages toast lifecycle, stacking, timeouts, and rendering.
 */
public class DefaultNotificationManager extends NotificationManager {
	private static final Logger LOGGER = LoggerFactory.getLogger(DefaultNotificationManager.class);

	private final List<ToastEntry> activeToasts = new CopyOnWriteArrayList<>();
	private Vector2f windowSize = Vector2f.ZERO;
	private Connection periodicConnection;

	private static class ToastEntry {
		final Toast toast;
		final Connection dismissConnection;
		Vector2f origin = Vector2f.ZERO;
		Vector2f size = Vector2f.ZERO;

		ToastEntry(final Toast toast, final Connection dismissConnection) {
			this.toast = toast;
			this.dismissConnection = dismissConnection;
		}
	}

	public DefaultNotificationManager() {
		super();
	}

	public DefaultNotificationManager(final ToastConfig config) {
		super(config);
	}

	// -- Convenience API --

	@Override
	public void warning(final String title, final String description) {
		show(ToastType.WARNING, title, description);
	}

	@Override
	public void info(final String title, final String description) {
		show(ToastType.INFO, title, description);
	}

	@Override
	public void error(final String title, final String description) {
		show(ToastType.ERROR, title, description);
	}

	@Override
	public void show(final ToastType type, final String title, final String description) {
		showToast(type, title, description);
	}

	@Override
	public Toast showToast(final ToastType type, final String title, final String description) {
		final Toast toast = new Toast(type, title, description, this.config);
		final Connection dismissConn = toast.signalDismiss.connect(() -> dismiss(toast));
		final ToastEntry entry = new ToastEntry(toast, dismissConn);
		this.activeToasts.add(entry);

		ensurePeriodicCallActive();
		relayout();
		markGlobalRedraw();

		LOGGER.debug("Toast shown: [{}] {} (activeToasts={})", type, title,
				this.activeToasts.size());
		return toast;
	}

	@Override
	public void dismiss(final Toast toast) {
		for (final ToastEntry entry : this.activeToasts) {
			if (entry.toast == toast) {
				entry.dismissConnection.close();
				this.activeToasts.remove(entry);
				break;
			}
		}
		relayout();
		markGlobalRedraw();
		if (this.activeToasts.isEmpty()) {
			stopPeriodicCall();
		}
	}

	@Override
	public void dismissAll() {
		for (final ToastEntry entry : this.activeToasts) {
			entry.dismissConnection.close();
		}
		this.activeToasts.clear();
		stopPeriodicCall();
		markGlobalRedraw();
	}

	// -- Periodic call for timeout tracking --

	private void ensurePeriodicCallActive() {
		if (this.periodicConnection != null && this.periodicConnection.isConnected()) {
			return;
		}
		this.periodicConnection = EwolObject.getObjectManager().periodicCall
				.connect(this::onPeriodicCall);
	}

	private void stopPeriodicCall() {
		if (this.periodicConnection != null) {
			this.periodicConnection.close();
			this.periodicConnection = null;
		}
	}

	private void onPeriodicCall(final EventTime event) {
		if (this.activeToasts.isEmpty()) {
			return;
		}
		// Only the front toast (index 0, oldest) counts down its timeout.
		// Others wait until they become the front toast.
		final float delta = event.getTimeDeltaCallSecond();
		final ToastEntry front = this.activeToasts.get(0);
		if (front.toast.advanceTime(delta)) {
			dismiss(front.toast);
		}
	}

	private void removeEntry(final ToastEntry entry) {
		entry.dismissConnection.close();
		this.activeToasts.remove(entry);
	}

	/**
	 * Notify the widget manager that a redraw is needed.
	 * Since toast widgets are not in the normal widget tree, we must
	 * explicitly tell the framework to schedule a redraw cycle.
	 */
	private void markGlobalRedraw() {
		final WidgetManager wm = Ewol.getContext().getWidgetManager();
		if (wm != null) {
			wm.markDrawingIsNeeded();
		}
	}

	// -- Layout --

	@Override
	public void onWindowChangeSize(final Vector2f windowSize) {
		this.windowSize = windowSize;
		relayout();
	}

	/**
	 * Recalculate positions of all active toasts based on Gravity and stacking.
	 * The front toast (index 0, oldest) is fully visible at the anchor position.
	 * Newer toasts are stacked behind it with a small visible offset, so many
	 * notifications can be shown without covering the entire screen.
	 */
	private void relayout() {
		if (this.windowSize.equals(Vector2f.ZERO)) {
			LOGGER.warn("relayout: windowSize is ZERO, skipping");
			return;
		}
		if (this.activeToasts.isEmpty()) {
			return;
		}

		final Gravity pos = this.config.getPosition();
		final Vector2f margin = this.config.getEdgeMargin();
		final float stackOffset = this.config.getStackSpacing();
		final float toastWidth = this.config.getWidth();

		// Determine starting X based on horizontal gravity
		float startX;
		if (pos.x() == GravityHorizontal.LEFT) {
			startX = margin.x();
		} else if (pos.x() == GravityHorizontal.RIGHT) {
			startX = this.windowSize.x() - toastWidth - margin.x();
		} else {
			startX = (this.windowSize.x() - toastWidth) / 2.0f;
		}

		// Stack direction: BOTTOM = stack upward, TOP = stack downward
		final boolean stackUpward = (pos.y() != GravityVertical.TOP);

		// X offset direction: LEFT = offset toward left (negative), RIGHT = offset toward right (positive)
		// CENTER = no X offset
		final float xOffsetDirection;
		if (pos.x() == GravityHorizontal.LEFT) {
			xOffsetDirection = -1.0f; // newer toasts shift left (toward exterior)
		} else if (pos.x() == GravityHorizontal.RIGHT) {
			xOffsetDirection = 1.0f; // newer toasts shift right (toward exterior)
		} else {
			xOffsetDirection = 0.0f; // center: no horizontal offset
		}

		// Index 0 = oldest = front toast (fully visible at base margin).
		// Index N = newest = behind, with N * stackOffset further from anchor.
		// Only layout the first maxVisibleToasts; the rest wait in queue.
		final List<ToastEntry> snapshot = List.copyOf(this.activeToasts);
		final int count = Math.min(snapshot.size(), this.config.getMaxVisibleToasts());

		for (int i = 0; i < count; i++) {
			final ToastEntry entry = snapshot.get(i);
			final Vector2f minSize = entry.toast.getCalculatedMinSize();
			final float toastHeight = minSize.y();

			final float offsetFromAnchor = i * stackOffset;

			float originY;
			if (stackUpward) {
				originY = margin.y() + offsetFromAnchor;
			} else {
				originY = this.windowSize.y() - margin.y() - toastHeight - offsetFromAnchor;
			}

			// Each stacked toast also shifts in X toward the exterior
			final float originX = startX + i * stackOffset * xOffsetDirection;

			entry.origin = new Vector2f(originX, originY);
			entry.size = new Vector2f(toastWidth, toastHeight);
			entry.toast.layoutAt(entry.origin, entry.size);
		}
	}

	// -- Rendering --

	@Override
	public void onRegenerateDisplay() {
		final List<ToastEntry> snapshot = List.copyOf(this.activeToasts);
		final int count = Math.min(snapshot.size(), this.config.getMaxVisibleToasts());
		for (int i = 0; i < count; i++) {
			snapshot.get(i).toast.regenerate();
		}
	}

	@Override
	public void onDraw(final DrawProperty displayProp) {
		// Draw in reverse order: newest (back) first, oldest (front) last
		// so the front toast is rendered on top.
		final List<ToastEntry> snapshot = List.copyOf(this.activeToasts);
		final int count = Math.min(snapshot.size(), this.config.getMaxVisibleToasts());
		for (int i = count - 1; i >= 0; i--) {
			snapshot.get(i).toast.draw(displayProp);
		}
	}

	@Override
	public Widget getWidgetAtPos(final Vector2f pos) {
		// Check front toast first (index 0 = oldest, visually on top)
		final List<ToastEntry> snapshot = List.copyOf(this.activeToasts);
		final int count = Math.min(snapshot.size(), this.config.getMaxVisibleToasts());
		for (int i = 0; i < count; i++) {
			final ToastEntry entry = snapshot.get(i);
			if (pos.x() >= entry.origin.x()
					&& pos.x() <= entry.origin.x() + entry.size.x()
					&& pos.y() >= entry.origin.y()
					&& pos.y() <= entry.origin.y() + entry.size.y()) {
				return entry.toast.getRootBox().getWidgetAtPos(pos);
			}
		}
		return null;
	}

	@Override
	public boolean hasActiveToasts() {
		return !this.activeToasts.isEmpty();
	}
}
