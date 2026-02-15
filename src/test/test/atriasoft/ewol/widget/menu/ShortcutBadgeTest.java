package test.atriasoft.ewol.widget.menu;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;

import org.atriasoft.ewol.widget.menu.ShortcutBadge;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import test.atriasoft.ewol.EwolTestContext;

class ShortcutBadgeTest {

	@BeforeAll
	static void setUpClass() {
		EwolTestContext.init();
	}

	@Test
	void testFactoryCreate() {
		final ShortcutBadge badge = ShortcutBadge.create("Ctrl");
		assertNotNull(badge);
		assertEquals("Ctrl", badge.getText());
	}

	@Test
	void testFluentText() {
		final ShortcutBadge badge = new ShortcutBadge();
		final ShortcutBadge result = badge.text("S");
		assertEquals(badge, result, "fluent text() should return same instance");
		assertEquals("S", badge.getText());
	}

	@Test
	void testDefaultText() {
		final ShortcutBadge badge = new ShortcutBadge();
		assertEquals("", badge.getText(), "default text should be empty");
	}

	@Test
	void testTextChange() {
		final ShortcutBadge badge = ShortcutBadge.create("F5");
		assertEquals("F5", badge.getText());
		badge.text("F12");
		assertEquals("F12", badge.getText());
	}
}
