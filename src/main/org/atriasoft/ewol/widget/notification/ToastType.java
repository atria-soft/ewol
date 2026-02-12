package org.atriasoft.ewol.widget.notification;

import org.atriasoft.etk.Color;

/**
 * Type of toast notification with default colors.
 */
public enum ToastType {
	INFO(
		new Color(0x21 / 255f, 0x96 / 255f, 0xF3 / 255f, 1f),
		Color.WHITE,
		new Color(0x19 / 255f, 0x76 / 255f, 0xD2 / 255f, 1f)
	),
	WARNING(
		new Color(0xFF / 255f, 0x98 / 255f, 0x00 / 255f, 1f),
		Color.WHITE,
		new Color(0xF5 / 255f, 0x7C / 255f, 0x00 / 255f, 1f)
	),
	ERROR(
		new Color(0xF4 / 255f, 0x43 / 255f, 0x36 / 255f, 1f),
		Color.WHITE,
		new Color(0xD3 / 255f, 0x2F / 255f, 0x2F / 255f, 1f)
	);

	private final Color backgroundColor;
	private final Color textColor;
	private final Color borderColor;

	ToastType(final Color backgroundColor, final Color textColor, final Color borderColor) {
		this.backgroundColor = backgroundColor;
		this.textColor = textColor;
		this.borderColor = borderColor;
	}

	public Color getBackgroundColor() {
		return this.backgroundColor;
	}

	public Color getTextColor() {
		return this.textColor;
	}

	public Color getBorderColor() {
		return this.borderColor;
	}
}
