package org.atriasoft.ewol.widget.menu;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Utility class to parse shortcut strings into display tokens.
 *
 * <p>Examples:</p>
 * <ul>
 *   <li>{@code "ctrl+shift+s"} → {@code ["Ctrl", "Shift", "S"]}</li>
 *   <li>{@code "F5"} → {@code ["F5"]}</li>
 *   <li>{@code "ctrl+alt+delete"} → {@code ["Ctrl", "Alt", "Delete"]}</li>
 * </ul>
 */
public final class ShortcutFormatter {

	private ShortcutFormatter() {
		// Utility class
	}

	/**
	 * Parse a shortcut string into a list of display tokens.
	 * @param shortcut the shortcut string (e.g., "ctrl+shift+s")
	 * @return list of formatted tokens (e.g., ["Ctrl", "Shift", "S"]), empty if null/blank
	 */
	public static List<String> parse(final String shortcut) {
		if (shortcut == null || shortcut.isBlank()) {
			return Collections.emptyList();
		}
		final String[] parts = shortcut.split("\\+");
		final List<String> tokens = new ArrayList<>(parts.length);
		for (final String part : parts) {
			final String trimmed = part.trim();
			if (!trimmed.isEmpty()) {
				tokens.add(capitalize(trimmed));
			}
		}
		return tokens;
	}

	/**
	 * Capitalize first letter, rest as-is for multi-char tokens,
	 * uppercase everything for single-char tokens.
	 */
	private static String capitalize(final String token) {
		if (token.length() == 1) {
			return token.toUpperCase();
		}
		return Character.toUpperCase(token.charAt(0)) + token.substring(1).toLowerCase();
	}
}
