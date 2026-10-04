package test.atriasoft.ewol.widget;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

import org.atriasoft.ewol.event.EventEntry;
import org.atriasoft.ewol.widget.Entry;
import org.atriasoft.gale.key.KeyKeyboard;
import org.atriasoft.gale.key.KeySpecial;
import org.atriasoft.gale.key.KeyStatus;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import test.atriasoft.ewol.EwolTestContext;

/** The characters an entry inserts: text, not the shortcuts typed with Control (gale hands Ctrl+Z as a z). */
class EntryTest {

	private Entry entry;

	@BeforeAll
	static void setUpClass() {
		EwolTestContext.init();
	}

	@BeforeEach
	void setUp() {
		this.entry = new Entry();
		this.entry.setPropertyValue("abc");
	}

	private static KeySpecial keys(final boolean ctrlLeft, final boolean shift, final boolean altLeft,
			final boolean altGr) {
		final KeySpecial special = new KeySpecial();
		special.setCtrlLeft(ctrlLeft);
		special.setShiftLeft(shift);
		special.setAltLeft(altLeft);
		special.setAltRight(altGr);
		return special;
	}

	private boolean type(final KeySpecial special, final char typed) {
		return this.entry.onEventEntry(new EventEntry(special, KeyKeyboard.CHARACTER, KeyStatus.down, typed));
	}

	@Test
	void aLetterIsInserted() {
		type(keys(false, false, false, false), 'd');
		type(keys(false, true, false, false), 'E');
		assertEquals("abcdE", this.entry.getPropertyValue());
	}

	@Test
	void aLetterTypedWithControlIsAShortcutNotText() {
		for (final char letter : new char[] { 'z', 's', 'b', 'f', 'm', 'h', 'q' }) {
			assertFalse(type(keys(true, false, false, false), letter), "ctrl+" + letter);
		}
		type(keys(true, true, false, false), 'S');
		final KeySpecial ctrlRight = new KeySpecial();
		ctrlRight.setCtrlRight(true);
		type(ctrlRight, 'z');
		assertEquals("abc", this.entry.getPropertyValue());
	}

	@Test
	void altGrTypesItsSymbols() {
		// X11: AltGr comes alone (VK_ALT_GRAPH), without Control.
		type(keys(false, false, false, true), '#');
		type(keys(false, true, false, true), '€');
		// Windows: AltGr comes with Control.
		type(keys(true, false, false, true), '@');
		type(keys(true, true, false, true), '[');
		assertEquals("abc#€@[", this.entry.getPropertyValue());
	}

	@Test
	void controlWithTheLeftAltIsAShortcut() {
		// X11: a real Control+Alt chord; gale hands what AWT typed (the 1 of the key, the control code of T).
		assertFalse(type(keys(true, false, true, false), '1'));
		assertFalse(type(keys(true, false, true, false), '\u0014'));
		assertEquals("abc", this.entry.getPropertyValue());
	}

	@Test
	void controlCharactersAreNotInserted() {
		// Escape, Tab, the control codes AWT types for Ctrl+T to Ctrl+Z, and the U+FFFF of a key that types
		// nothing (a dead key, AltGr on a key without third symbol).
		type(keys(false, false, false, false), '\u001b');
		type(keys(false, false, false, false), '\t');
		type(keys(true, false, false, false), '\u0014');
		type(keys(true, false, false, false), '\u001a');
		type(keys(false, false, false, false), '\uffff');
		type(keys(true, false, false, true), '\uffff');
		assertEquals("abc", this.entry.getPropertyValue());
	}

	@Test
	void backspaceStillErasesWithControl() {
		type(keys(true, false, false, false), '\b');
		assertEquals("ab", this.entry.getPropertyValue());
	}

	@Test
	void theShortcutsOfTheEntryAreCaught() {
		// ctrl+w/x/c/v/a reach the entry as letters now: caught as shortcuts, without any error, press and release.
		for (final char letter : new char[] { 'w', 'x', 'c', 'v', 'a' }) {
			assertTrue(this.entry.onEventShortCut(keys(true, false, false, false), letter, KeyKeyboard.CHARACTER, true),
					"ctrl+" + letter);
			assertTrue(this.entry.onEventShortCut(keys(true, false, false, false), letter, KeyKeyboard.CHARACTER, false),
					"release of ctrl+" + letter);
		}
		assertTrue(this.entry.onEventShortCut(keys(true, true, false, false), 'A', KeyKeyboard.CHARACTER, true));
		// A letter alone is no shortcut.
		assertFalse(this.entry.onEventShortCut(keys(false, false, false, false), 'c', KeyKeyboard.CHARACTER, true));
	}

	@Test
	void theShortcutsOfTheEntryDoNothingUntilTheClipboardWorks() {
		// To wire once gale's ClipBoard exists (a stub today): a cut must never erase a selection it did not copy.
		final KeySpecial ctrl = keys(true, false, false, false);
		this.entry.onEventShortCut(ctrl, 'a', KeyKeyboard.CHARACTER, true);
		this.entry.onEventShortCut(ctrl, 'x', KeyKeyboard.CHARACTER, true);
		this.entry.onEventShortCut(ctrl, 'w', KeyKeyboard.CHARACTER, true);
		assertEquals("abc", this.entry.getPropertyValue(), "nothing cut nor cleaned");
		// Nothing selected by ctrl+a: a letter goes after the text.
		type(keys(false, false, false, false), 'd');
		assertEquals("abcd", this.entry.getPropertyValue());
	}
}
