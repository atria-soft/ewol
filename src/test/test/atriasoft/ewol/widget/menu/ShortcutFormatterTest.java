package test.atriasoft.ewol.widget.menu;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.List;

import org.atriasoft.ewol.widget.menu.ShortcutFormatter;
import org.junit.jupiter.api.Test;

class ShortcutFormatterTest {

	@Test
	void testParseNull() {
		final List<String> result = ShortcutFormatter.parse(null);
		assertTrue(result.isEmpty(), "null should return empty list");
	}

	@Test
	void testParseEmpty() {
		final List<String> result = ShortcutFormatter.parse("");
		assertTrue(result.isEmpty(), "empty string should return empty list");
	}

	@Test
	void testParseBlank() {
		final List<String> result = ShortcutFormatter.parse("   ");
		assertTrue(result.isEmpty(), "blank string should return empty list");
	}

	@Test
	void testParseSingleKey() {
		final List<String> result = ShortcutFormatter.parse("s");
		assertEquals(1, result.size());
		assertEquals("S", result.get(0), "single char should be uppercased");
	}

	@Test
	void testParseFunctionKey() {
		final List<String> result = ShortcutFormatter.parse("F5");
		assertEquals(1, result.size());
		assertEquals("F5", result.get(0), "F5 should be capitalized as F5");
	}

	@Test
	void testParseCtrlS() {
		final List<String> result = ShortcutFormatter.parse("ctrl+s");
		assertEquals(2, result.size());
		assertEquals("Ctrl", result.get(0));
		assertEquals("S", result.get(1));
	}

	@Test
	void testParseCtrlShiftS() {
		final List<String> result = ShortcutFormatter.parse("ctrl+shift+s");
		assertEquals(3, result.size());
		assertEquals("Ctrl", result.get(0));
		assertEquals("Shift", result.get(1));
		assertEquals("S", result.get(2));
	}

	@Test
	void testParseAltF4() {
		final List<String> result = ShortcutFormatter.parse("alt+F4");
		assertEquals(2, result.size());
		assertEquals("Alt", result.get(0));
		assertEquals("F4", result.get(1));
	}

	@Test
	void testParseCtrlAltDelete() {
		final List<String> result = ShortcutFormatter.parse("ctrl+alt+delete");
		assertEquals(3, result.size());
		assertEquals("Ctrl", result.get(0));
		assertEquals("Alt", result.get(1));
		assertEquals("Delete", result.get(2));
	}

	@Test
	void testParsePreservesMultiCharCase() {
		final List<String> result = ShortcutFormatter.parse("CTRL+SHIFT+A");
		assertEquals(3, result.size());
		assertEquals("Ctrl", result.get(0), "CTRL should be Ctrl");
		assertEquals("Shift", result.get(1), "SHIFT should be Shift");
		assertEquals("A", result.get(2), "single char A stays A");
	}

	@Test
	void testParseTrimsWhitespace() {
		final List<String> result = ShortcutFormatter.parse("ctrl + s");
		assertEquals(2, result.size());
		assertEquals("Ctrl", result.get(0));
		assertEquals("S", result.get(1));
	}

	@Test
	void testParseF1() {
		final List<String> result = ShortcutFormatter.parse("F1");
		assertEquals(1, result.size());
		assertEquals("F1", result.get(0));
	}

	@Test
	void testParseF12() {
		final List<String> result = ShortcutFormatter.parse("F12");
		assertEquals(1, result.size());
		assertEquals("F12", result.get(0));
	}
}
