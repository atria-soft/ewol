package test.atriasoft.ewol.widget;

import static org.junit.jupiter.api.Assertions.*;

import org.atriasoft.ewol.widget.NumberInput;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import test.atriasoft.ewol.EwolTestContext;

class NumberInputTest {

	private NumberInput numberInput;

	@BeforeAll
	static void setUpClass() {
		EwolTestContext.init();
	}

	@BeforeEach
	void setUp() {
		numberInput = new NumberInput();
	}

	@Test
	void testDefaultValues() {
		assertEquals(0, numberInput.getPropertyValue(), "Default value should be 0");
		assertEquals(Long.MIN_VALUE, numberInput.getPropertyMin(), "Default min should be Long.MIN_VALUE");
		assertEquals(Long.MAX_VALUE, numberInput.getPropertyMax(), "Default max should be Long.MAX_VALUE");
		assertEquals(1, numberInput.getPropertyIncrement(), "Default increment should be 1");
		assertEquals(0, numberInput.getPropertyMantis(), "Default mantis should be 0");
	}

	@Test
	void testSetValue() {
		numberInput.setPropertyValue(42);
		assertEquals(42, numberInput.getPropertyValue(), "Value should be set to 42");
	}

	@Test
	void testValueClamping() {
		numberInput.setPropertyMin(0);
		numberInput.setPropertyMax(100);

		numberInput.setPropertyValue(150);
		assertEquals(100, numberInput.getPropertyValue(), "Value should be clamped to max");

		numberInput.setPropertyValue(-10);
		assertEquals(0, numberInput.getPropertyValue(), "Value should be clamped to min");
	}

	@Test
	void testIncrement() {
		numberInput.setPropertyMin(0);
		numberInput.setPropertyMax(100);
		numberInput.setPropertyValue(50);
		numberInput.setPropertyIncrement(5);

		assertEquals(5, numberInput.getPropertyIncrement(), "Increment should be 5");
	}

	@Test
	void testMantisConversion() {
		numberInput.setPropertyMantis(2);
		numberInput.setPropertyValue(1234);

		assertEquals(12.34, numberInput.getValueAsDouble(), 0.001, "Value as double with mantis 2 should be 12.34");
	}

	@Test
	void testMantisZero() {
		numberInput.setPropertyMantis(0);
		numberInput.setPropertyValue(100);

		assertEquals(100.0, numberInput.getValueAsDouble(), 0.001, "Value as double with mantis 0 should be 100.0");
	}

	@Test
	void testSignalsNotNull() {
		assertNotNull(numberInput.signalValue, "signalValue should not be null");
		assertNotNull(numberInput.signalValueDouble, "signalValueDouble should not be null");
	}
}
