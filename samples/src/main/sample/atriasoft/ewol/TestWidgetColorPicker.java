package sample.atriasoft.ewol;

import org.atriasoft.etk.Color;
import org.atriasoft.ewol.widget.ColorPicker;
import org.atriasoft.ewol.widget.Widget;

public class TestWidgetColorPicker implements TestWidgetInterface {

	@Override
	public Widget getWidget() {
		// ColorPicker - color selection widget with popup
		// Signal (color-changed) is automatically connected by ModelWidget
		// and displayed in the central log
		final ColorPicker colorPicker = ColorPicker.create(Color.CORAL);
		colorPicker.expand(false, false).fill(false, false);
		return colorPicker;
	}

	@Override
	public String getTitle() {
		return "ColorPicker";
	}
}
