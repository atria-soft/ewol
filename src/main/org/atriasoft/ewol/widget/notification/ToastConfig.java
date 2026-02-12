package org.atriasoft.ewol.widget.notification;

import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.Gravity;

/**
 * Global toast notification configuration.
 * All values are defaults that can be overridden per-toast.
 */
public class ToastConfig {
	private Gravity position = Gravity.BOTTOM_RIGHT;
	private float width = 350.0f;
	private float timeoutSeconds = 2.0f;
	private Vector2f edgeMargin = new Vector2f(16.0f, 16.0f);
	private float stackSpacing = 15.0f;
	private int maxVisibleToasts = 10;
	private int titleFontSize = 14;
	private int descriptionFontSize = 11;
	private float borderRadius = 8.0f;
	private float borderWidth = 2.0f;
	private float padding = 10.0f;

	public Gravity getPosition() {
		return this.position;
	}

	public float getWidth() {
		return this.width;
	}

	public float getTimeoutSeconds() {
		return this.timeoutSeconds;
	}

	public Vector2f getEdgeMargin() {
		return this.edgeMargin;
	}

	public float getStackSpacing() {
		return this.stackSpacing;
	}

	public int getMaxVisibleToasts() {
		return this.maxVisibleToasts;
	}

	public int getTitleFontSize() {
		return this.titleFontSize;
	}

	public int getDescriptionFontSize() {
		return this.descriptionFontSize;
	}

	public float getBorderRadius() {
		return this.borderRadius;
	}

	public float getBorderWidth() {
		return this.borderWidth;
	}

	public float getPadding() {
		return this.padding;
	}

	// Fluent setters

	public ToastConfig position(final Gravity position) {
		this.position = position;
		return this;
	}

	public ToastConfig width(final float width) {
		this.width = width;
		return this;
	}

	public ToastConfig timeoutSeconds(final float timeout) {
		this.timeoutSeconds = timeout;
		return this;
	}

	public ToastConfig edgeMargin(final Vector2f margin) {
		this.edgeMargin = margin;
		return this;
	}

	public ToastConfig stackSpacing(final float spacing) {
		this.stackSpacing = spacing;
		return this;
	}

	public ToastConfig maxVisibleToasts(final int max) {
		this.maxVisibleToasts = max;
		return this;
	}

	public ToastConfig titleFontSize(final int size) {
		this.titleFontSize = size;
		return this;
	}

	public ToastConfig descriptionFontSize(final int size) {
		this.descriptionFontSize = size;
		return this;
	}

	public ToastConfig borderRadius(final float radius) {
		this.borderRadius = radius;
		return this;
	}

	public ToastConfig borderWidth(final float width) {
		this.borderWidth = width;
		return this;
	}

	public ToastConfig padding(final float padding) {
		this.padding = padding;
		return this;
	}
}
