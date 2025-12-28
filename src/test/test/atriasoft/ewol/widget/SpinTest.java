package test.atriasoft.ewol.widget;

import static org.junit.jupiter.api.Assertions.*;

import org.atriasoft.ewol.widget.Spin;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class SpinTest {

	private Spin spin;

	@BeforeEach
	void setUp() {
		spin = new Spin();
	}

	@Test
	void testDefaultValues() {
		assertEquals(0, spin.getPropertyValue(), "Default value should be 0");
		assertEquals(Long.MIN_VALUE, spin.getPropertyMin(), "Default min should be Long.MIN_VALUE");
		assertEquals(Long.MAX_VALUE, spin.getPropertyMax(), "Default max should be Long.MAX_VALUE");
		assertEquals(1, spin.getPropertyIncrement(), "Default increment should be 1");
		assertEquals(0, spin.getPropertyMantis(), "Default mantis should be 0");
	}

	@Test
	void testSetValue() {
		spin.setPropertyValue(42);
		assertEquals(42, spin.getPropertyValue(), "Value should be set to 42");
	}

	@Test
	void testValueClamping() {
		spin.setPropertyMin(0);
		spin.setPropertyMax(100);

		spin.setPropertyValue(150);
		assertEquals(100, spin.getPropertyValue(), "Value should be clamped to max");

		spin.setPropertyValue(-10);
		assertEquals(0, spin.getPropertyValue(), "Value should be clamped to min");
	}

	@Test
	void testIncrement() {
		spin.setPropertyMin(0);
		spin.setPropertyMax(100);
		spin.setPropertyValue(50);
		spin.setPropertyIncrement(5);

		assertEquals(5, spin.getPropertyIncrement(), "Increment should be 5");
	}

	@Test
	void testMantisConversion() {
		spin.setPropertyMantis(2);
		spin.setPropertyValue(1234);

		assertEquals(12.34, spin.getValueAsDouble(), 0.001, "Value as double with mantis 2 should be 12.34");
	}

	@Test
	void testMantisZero() {
		spin.setPropertyMantis(0);
		spin.setPropertyValue(100);

		assertEquals(100.0, spin.getValueAsDouble(), 0.001, "Value as double with mantis 0 should be 100.0");
	}

	@Test
	void testSignalsNotNull() {
		assertNotNull(spin.signalValue, "signalValue should not be null");
		assertNotNull(spin.signalValueDouble, "signalValueDouble should not be null");
	}
}
