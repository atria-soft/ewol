package test.atriasoft.ewol.widget;

import static org.junit.jupiter.api.Assertions.*;

import org.atriasoft.ewol.widget.Slider;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import test.atriasoft.ewol.EwolTestContext;

class SliderTest {

	private Slider slider;

	@BeforeAll
	static void setUpClass() {
		EwolTestContext.init();
	}

	@BeforeEach
	void setUp() {
		slider = new Slider();
	}

	@Test
	void testDefaultValues() {
		assertEquals(0.0f, slider.getPropertyValue(), 0.001f, "Default value should be 0");
		assertEquals(0.0f, slider.getPropertyMinimum(), 0.001f, "Default minimum should be 0");
		assertEquals(10.0f, slider.getPropertyMaximum(), 0.001f, "Default maximum should be 10");
		assertEquals(0.1f, slider.getPropertyStep(), 0.001f, "Default step should be 0.1");
	}

	@Test
	void testSetValue() {
		slider.setPropertyValue(5.0f);
		assertEquals(5.0f, slider.getPropertyValue(), 0.001f, "Value should be set to 5");
	}

	@Test
	void testValueClamping() {
		slider.setPropertyValue(15.0f);
		assertEquals(10.0f, slider.getPropertyValue(), 0.001f, "Value should be clamped to maximum");

		slider.setPropertyValue(-5.0f);
		assertEquals(0.0f, slider.getPropertyValue(), 0.001f, "Value should be clamped to minimum");
	}

	@Test
	void testStepRounding() {
		slider.setPropertyStep(1.0f);
		slider.setPropertyValue(5.4f);
		assertEquals(5.0f, slider.getPropertyValue(), 0.001f, "Value should be rounded to nearest step");

		slider.setPropertyValue(5.6f);
		assertEquals(6.0f, slider.getPropertyValue(), 0.001f, "Value should be rounded to nearest step");
	}

	@Test
	void testMinMaxChange() {
		slider.setPropertyValue(5.0f);
		slider.setPropertyMaximum(3.0f);
		assertEquals(3.0f, slider.getPropertyValue(), 0.001f, "Value should be clamped when max changes");

		slider.setPropertyMinimum(2.0f);
		slider.setPropertyValue(1.0f);
		assertEquals(2.0f, slider.getPropertyValue(), 0.001f, "Value should be clamped when below new min");
	}

	@Test
	void testSignalNotNull() {
		assertNotNull(slider.signalValue, "signalValue should not be null");
	}
}
