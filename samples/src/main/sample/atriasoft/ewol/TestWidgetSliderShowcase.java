package sample.atriasoft.ewol;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Slider;
import org.atriasoft.ewol.widget.Widget;

public class TestWidgetSliderShowcase implements TestWidgetInterface {
	@Override
	public Widget getWidget() {
		// Label to show current value
		final var valueLabel = new Label("Value: 50.0");

		// Default slider (blue theme)
		final var defaultSlider = Slider.create()
				.range(0, 100)
				.value(50)
				.step(1)
				.onValueChange(v -> valueLabel.setPropertyValue("Value: " + String.format("%.1f", v)));
		defaultSlider.setPropertyExpand(new Vector2b(true, false));
		defaultSlider.setPropertyFill(new Vector2b(true, false));

		// Green themed slider
		final var greenSlider = Slider.create()
				.range(0, 100)
				.value(30)
				.fillColor(new Color(0x4C, 0xAF, 0x50, 0xFF))
				.cursorColor(new Color(0x38, 0x8E, 0x3C, 0xFF));
		greenSlider.setPropertyExpand(new Vector2b(true, false));
		greenSlider.setPropertyFill(new Vector2b(true, false));

		// Orange themed slider
		final var orangeSlider = Slider.create()
				.range(0, 100)
				.value(70)
				.fillColor(new Color(0xFF, 0x98, 0x00, 0xFF))
				.cursorColor(new Color(0xF5, 0x7C, 0x00, 0xFF));
		orangeSlider.setPropertyExpand(new Vector2b(true, false));
		orangeSlider.setPropertyFill(new Vector2b(true, false));

		// Music player style slider with chapter markers (simulating MP3 chapters)
		final var musicSlider = Slider.create()
				.range(0, 300) // 5 minutes song (in seconds)
				.value(0)
				.step(1)
				.fillColor(new Color(0xE9, 0x1E, 0x63, 0xFF)) // Pink
				.cursorColor(new Color(0xC2, 0x18, 0x5B, 0xFF))
				.markerColor(Color.WHITE)
				.markerRadius(4.0f)
				.markers(0, 20, 160, 180, 240); // Chapter markers at 0s, 45s, 2min, 3min, 4min
		musicSlider.setPropertyExpand(new Vector2b(true, false));
		musicSlider.setPropertyFill(new Vector2b(true, false));

		final var container = Sizer.vertical();
		container.setPropertyExpand(Vector2b.TRUE);
		container.setPropertyFill(Vector2b.TRUE);
		container.add(valueLabel, defaultSlider, greenSlider, orangeSlider, musicSlider);

		return container;
	}

	@Override
	public String getTitle() {
		return "Slider Showcase";
	}

	@Override
	public String getDescription() {
		return "Slider variants and configurations";
	}

	@Override
	public String getCategory() {
		return "Data";
	}

	@Override
	public boolean isMetaWidget() {
		return true;
	}
}
