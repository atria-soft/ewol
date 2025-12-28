package test.atriasoft.ewol.widget;

import static org.junit.jupiter.api.Assertions.*;

import org.atriasoft.ewol.widget.Tick;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class TickTest {

	private Tick tick;

	@BeforeEach
	void setUp() {
		tick = new Tick();
	}

	@Test
	void testDefaultState() {
		assertFalse(tick.isChecked(), "Tick should be unchecked by default");
		assertFalse(tick.isPressed(), "Tick should not be pressed by default");
	}

	@Test
	void testSetValue() {
		tick.setPropertyValue(true);
		assertTrue(tick.isChecked(), "Tick should be checked after setting value to true");

		tick.setPropertyValue(false);
		assertFalse(tick.isChecked(), "Tick should be unchecked after setting value to false");
	}

	@Test
	void testToggle() {
		assertFalse(tick.isChecked(), "Initial state should be unchecked");

		tick.toggle();
		assertTrue(tick.isChecked(), "After toggle, should be checked");

		tick.toggle();
		assertFalse(tick.isChecked(), "After second toggle, should be unchecked");
	}

	@Test
	void testNullValueHandling() {
		tick.setPropertyValue(true);
		assertTrue(tick.isChecked(), "Should be checked");

		tick.setPropertyValue(null);
		assertFalse(tick.isChecked(), "Null should be treated as false");
	}

	@Test
	void testSignalsNotNull() {
		assertNotNull(tick.signalClick, "signalClick should not be null");
		assertNotNull(tick.signalDown, "signalDown should not be null");
		assertNotNull(tick.signalUp, "signalUp should not be null");
		assertNotNull(tick.signalValue, "signalValue should not be null");
	}

	@Test
	void testGetPropertyValue() {
		tick.setPropertyValue(true);
		assertEquals(Boolean.TRUE, tick.getPropertyValue(), "getPropertyValue should return true");

		tick.setPropertyValue(false);
		assertEquals(Boolean.FALSE, tick.getPropertyValue(), "getPropertyValue should return false");
	}
}
