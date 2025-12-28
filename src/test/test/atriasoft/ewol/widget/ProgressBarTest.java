package test.atriasoft.ewol.widget;

import static org.junit.jupiter.api.Assertions.*;

import org.atriasoft.etk.Color;
import org.atriasoft.ewol.widget.ProgressBar;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class ProgressBarTest {

	private ProgressBar progressBar;

	@BeforeEach
	void setUp() {
		progressBar = new ProgressBar();
	}

	@Test
	void testDefaultValues() {
		assertEquals(0.0f, progressBar.getPropertyValue(), 0.001f, "Default value should be 0");
		assertEquals(0.0f, progressBar.getPercent(), 0.001f, "Default percent should be 0");
	}

	@Test
	void testSetValue() {
		progressBar.setPropertyValue(0.5f);
		assertEquals(0.5f, progressBar.getPropertyValue(), 0.001f, "Value should be 0.5");
		assertEquals(50.0f, progressBar.getPercent(), 0.001f, "Percent should be 50");
	}

	@Test
	void testValueClamping() {
		progressBar.setPropertyValue(1.5f);
		assertEquals(1.0f, progressBar.getPropertyValue(), 0.001f, "Value should be clamped to 1.0");

		progressBar.setPropertyValue(-0.5f);
		assertEquals(0.0f, progressBar.getPropertyValue(), 0.001f, "Value should be clamped to 0.0");
	}

	@Test
	void testSetPercent() {
		progressBar.setPercent(75.0f);
		assertEquals(0.75f, progressBar.getPropertyValue(), 0.001f, "Value should be 0.75");
		assertEquals(75.0f, progressBar.getPercent(), 0.001f, "Percent should be 75");
	}

	@Test
	void testPercentClamping() {
		progressBar.setPercent(150.0f);
		assertEquals(1.0f, progressBar.getPropertyValue(), 0.001f, "Value should be clamped to 1.0");
		assertEquals(100.0f, progressBar.getPercent(), 0.001f, "Percent should be 100");
	}

	@Test
	void testColorProperties() {
		progressBar.setPropertyColorOn(Color.BLUE);
		assertEquals(Color.BLUE, progressBar.getPropertyColorOn(), "Color on should be BLUE");

		progressBar.setPropertyColorOff(Color.RED);
		assertEquals(Color.RED, progressBar.getPropertyColorOff(), "Color off should be RED");
	}

	@Test
	void testSignalNotNull() {
		assertNotNull(progressBar.signalValue, "signalValue should not be null");
	}
}
