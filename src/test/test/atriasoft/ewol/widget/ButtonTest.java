package test.atriasoft.ewol.widget;

import static org.junit.jupiter.api.Assertions.*;

import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.Label;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class ButtonTest {

	private Button button;

	@BeforeEach
	void setUp() {
		button = new Button();
	}

	@Test
	void testDefaultState() {
		assertFalse(button.isPressed(), "Button should not be pressed by default");
		assertFalse(button.isHovered(), "Button should not be hovered by default");
	}

	@Test
	void testCreateLabelButton() {
		final Button labelButton = Button.createLabelButton("Test Label");
		assertNotNull(labelButton, "Label button should not be null");
		assertNotNull(labelButton.getSubWidget(), "Label button should have a sub widget");
		assertTrue(labelButton.getSubWidget() instanceof Label, "Sub widget should be a Label");
	}

	@Test
	void testSignalsNotNull() {
		assertNotNull(button.signalClick, "signalClick should not be null");
		assertNotNull(button.signalDown, "signalDown should not be null");
		assertNotNull(button.signalUp, "signalUp should not be null");
		assertNotNull(button.signalEnter, "signalEnter should not be null");
		assertNotNull(button.signalLeave, "signalLeave should not be null");
	}
}
