/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.etk.Color;

/**
 * Colours of a {@link Dialog} (and of every dialog built on it: loading, pause, file chooser...).
 * {@link Dialog#setDefaultSkin(DialogSkin)} makes every new dialog follow the active theme.
 *
 * @param overlay the veil over the window behind the dialog
 * @param content the background of the content area
 * @param headerFooter the background of the title bar and of the footer
 * @param border the border of the content area
 * @param icon the close icon
 * @param title the colour of the title text
 */
public record DialogSkin(Color overlay, Color content, Color headerFooter, Color border, Color icon, Color title) {
	/** The light skin, the historical look of the dialogs. */
	public static final DialogSkin LIGHT = new DialogSkin(new Color(0x00, 0x00, 0x00, 0x60),
			new Color(0xF5, 0xF5, 0xF5, 0xFF), new Color(0xE0, 0xE0, 0xE0, 0xFF), new Color(0xC0, 0xC0, 0xC0, 0xFF),
			new Color(0x40, 0x40, 0x40, 0xFF), new Color(0x20, 0x20, 0x20, 0xFF));

	/** A dark skin: light texts on dark panels. */
	public static final DialogSkin DARK = new DialogSkin(new Color(0x00, 0x00, 0x00, 0xA0),
			new Color(0x1E, 0x1B, 0x19, 0xFF), new Color(0x2C, 0x28, 0x25, 0xFF), new Color(0x4A, 0x42, 0x3C, 0xFF),
			new Color(0xD8, 0xD0, 0xC8, 0xFF), new Color(0xEE, 0xE6, 0xDC, 0xFF));
}
