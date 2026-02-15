package test.atriasoft.ewol.widget.menu;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.concurrent.atomic.AtomicBoolean;

import org.atriasoft.ewol.widget.menu.MenuItem;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import test.atriasoft.ewol.EwolTestContext;

class MenuItemTest {

	@BeforeAll
	static void setUpClass() {
		EwolTestContext.init();
	}

	@Test
	void testFactoryCreate() {
		final MenuItem item = MenuItem.create("Copy");
		assertNotNull(item);
		assertEquals("Copy", item.getText());
	}

	@Test
	void testDefaultEnabled() {
		final MenuItem item = MenuItem.create("Test");
		assertTrue(item.isItemEnabled(), "item should be enabled by default");
	}

	@Test
	void testDisable() {
		final MenuItem item = MenuItem.create("Test");
		final MenuItem result = item.enabled(false);
		assertEquals(item, result, "fluent enabled() should return same instance");
		assertFalse(item.isItemEnabled());
	}

	@Test
	void testReEnable() {
		final MenuItem item = MenuItem.create("Test");
		item.enabled(false);
		assertFalse(item.isItemEnabled());
		item.enabled(true);
		assertTrue(item.isItemEnabled());
	}

	@Test
	void testDefaultHovered() {
		final MenuItem item = MenuItem.create("Test");
		assertFalse(item.isHovered(), "item should not be hovered by default");
	}

	@Test
	void testSetHovered() {
		final MenuItem item = MenuItem.create("Test");
		item.setHovered(true);
		assertTrue(item.isHovered());
		item.setHovered(false);
		assertFalse(item.isHovered());
	}

	@Test
	void testSignalClickNotNull() {
		final MenuItem item = MenuItem.create("Test");
		assertNotNull(item.signalClick, "signalClick should not be null");
	}

	@Test
	void testOnSelectCallback() {
		final MenuItem item = MenuItem.create("Test");
		final AtomicBoolean called = new AtomicBoolean(false);
		item.onSelect(() -> called.set(true));
		item.signalClick.emit();
		assertTrue(called.get(), "onSelect callback should be invoked on signalClick emit");
	}

	@Test
	void testFluentChaining() {
		final MenuItem item = MenuItem.create("Save")
				.shortcut("ctrl+s")
				.enabled(true)
				.onSelect(() -> {});
		assertNotNull(item);
		assertEquals("Save", item.getText());
		assertTrue(item.isItemEnabled());
	}
}
