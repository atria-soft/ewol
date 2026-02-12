package org.atriasoft.ewol.widget.notification;

import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.widget.Widget;

/**
 * Abstract base class for notification management.
 * Subclass this to provide a custom notification rendering system.
 */
public abstract class NotificationManager {

	protected final ToastConfig config;

	protected NotificationManager(final ToastConfig config) {
		this.config = config != null ? config : new ToastConfig();
	}

	protected NotificationManager() {
		this(new ToastConfig());
	}

	/**
	 * Get the current configuration.
	 */
	public ToastConfig getConfig() {
		return this.config;
	}

	// -- Convenience API --

	/**
	 * Show a WARNING toast.
	 */
	public abstract void warning(String title, String description);

	/**
	 * Show an INFO toast.
	 */
	public abstract void info(String title, String description);

	/**
	 * Show an ERROR toast.
	 */
	public abstract void error(String title, String description);

	/**
	 * Show a toast with explicit type.
	 */
	public abstract void show(ToastType type, String title, String description);

	/**
	 * Show a toast with explicit type and return it for per-toast overrides.
	 */
	public abstract Toast showToast(ToastType type, String title, String description);

	/**
	 * Dismiss all active toasts.
	 */
	public abstract void dismissAll();

	/**
	 * Dismiss a specific toast.
	 */
	public abstract void dismiss(Toast toast);

	// -- Rendering interface (called by Windows) --

	/**
	 * Called during onChangeSize to layout toasts relative to window size.
	 */
	public abstract void onWindowChangeSize(Vector2f windowSize);

	/**
	 * Called during systemRegenerateDisplay.
	 */
	public abstract void onRegenerateDisplay();

	/**
	 * Called during systemDraw after popups.
	 */
	public abstract void onDraw(DrawProperty displayProp);

	/**
	 * Called during getWidgetAtPos to check if a toast is at the given position.
	 * @return the widget at pos inside a toast, or null
	 */
	public abstract Widget getWidgetAtPos(Vector2f pos);

	/**
	 * Check if there are active toasts.
	 */
	public abstract boolean hasActiveToasts();
}
