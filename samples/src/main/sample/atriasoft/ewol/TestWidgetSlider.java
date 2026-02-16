package sample.atriasoft.ewol;

import org.atriasoft.ewol.widget.Slider;
import org.atriasoft.ewol.widget.Widget;

public class TestWidgetSlider implements TestWidgetInterface {

	@Override
	public Widget getWidget() {
		// Single slider with markers for property testing
		final var slider = Slider.create()
				.range(0, 100)
				.value(50)
				.step(1)
				.markers(10, 25, 50, 75, 90);
		slider.expand(true, false).fill(true, false);
		return slider;
	}

	@Override
	public String getTitle() {
		return "Slider";
	}

	@Override
	public String getDescription() {
		return "Numeric value slider control";
	}

	@Override
	public String getCategory() {
		return "Data";
	}
}
