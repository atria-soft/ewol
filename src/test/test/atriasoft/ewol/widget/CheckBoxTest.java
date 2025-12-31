package test.atriasoft.ewol.widget;

import static org.junit.jupiter.api.Assertions.*;

import org.atriasoft.ewol.widget.CheckBox;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import test.atriasoft.ewol.EwolTestContext;

class CheckBoxTest {

	private CheckBox checkBox;

	@BeforeAll
	static void setUpClass() {
		EwolTestContext.init();
	}

	@BeforeEach
	void setUp() {
		checkBox = new CheckBox("Test Label");
	}

	@Test
	void testDefaultState() {
		assertFalse(checkBox.isChecked(), "CheckBox should be unchecked by default");
		assertEquals("Test Label", checkBox.getPropertyLabel(), "Label should match constructor argument");
	}

	@Test
	void testDefaultConstructor() {
		final CheckBox defaultCheckBox = new CheckBox();
		assertEquals("No Label", defaultCheckBox.getPropertyLabel(), "Default label should be 'No Label'");
	}

	@Test
	void testSetValue() {
		checkBox.setPropertyValue(true);
		assertTrue(checkBox.isChecked(), "CheckBox should be checked after setting value to true");

		checkBox.setPropertyValue(false);
		assertFalse(checkBox.isChecked(), "CheckBox should be unchecked after setting value to false");
	}

	@Test
	void testToggle() {
		assertFalse(checkBox.isChecked(), "Initial state should be unchecked");

		checkBox.toggle();
		assertTrue(checkBox.isChecked(), "After toggle, should be checked");

		checkBox.toggle();
		assertFalse(checkBox.isChecked(), "After second toggle, should be unchecked");
	}

	@Test
	void testSetLabel() {
		checkBox.setPropertyLabel("New Label");
		assertEquals("New Label", checkBox.getPropertyLabel(), "Label should be updated");
	}

	@Test
	void testSignalsNotNull() {
		assertNotNull(checkBox.signalClick, "signalClick should not be null");
		assertNotNull(checkBox.signalDown, "signalDown should not be null");
		assertNotNull(checkBox.signalUp, "signalUp should not be null");
		assertNotNull(checkBox.signalValue, "signalValue should not be null");
	}
}
