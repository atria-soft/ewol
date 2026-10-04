package test.atriasoft.ewol.widget;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;

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
		// X11 or Windows: AltGr comes with Control and Alt held.
		type(keys(true, false, false, true), '@');
		type(keys(true, false, true, false), '€');
		type(keys(false, false, false, true), '#');
		type(keys(true, true, false, true), '[');
		assertEquals("abc@€#[", this.entry.getPropertyValue());
	}

	@Test
	void controlCharactersAreNotInserted() {
		// Escape, and the control codes AWT typed for Ctrl+T to Ctrl+Z before gale handed the letters.
		type(keys(false, false, false, false), '\u001b');
		type(keys(true, false, false, false), '\u0014');
		type(keys(true, false, false, false), '\u001a');
		assertEquals("abc", this.entry.getPropertyValue());
	}

	@Test
	void backspaceStillErasesWithControl() {
		type(keys(true, false, false, false), '\b');
		assertEquals("ab", this.entry.getPropertyValue());
	}
}
