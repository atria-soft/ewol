package sample.atriasoft.ewol;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.widget.Icon;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;
import org.atriasoft.ewol.widget.Widget;

/**
 * Test widget for Icon - demonstrates various icon colors and sizes.
 */
public class TestWidgetIcon implements TestWidgetInterface {

	@Override
	public Widget getWidget() {
		// Main vertical sizer
		final var mainSizer = new Sizer(DisplayMode.VERTICAL);
		mainSizer.setPropertyExpand(Vector2b.TRUE);
		mainSizer.setPropertyFill(Vector2b.TRUE);

		// Row 1: Basic icons with different colors
		final var row1 = new Sizer(DisplayMode.HORIZONTAL);
		row1.setPropertyExpand(Vector2b.TRUE_FALSE);
		row1.setPropertyFill(Vector2b.TRUE);
		mainSizer.subWidgetAdd(row1);

		// Home icon - white
		row1.subWidgetAdd(Icon.create("Home")
				.fill(Color.WHITE)
				.size(new Dimension2f(new Vector2f(48, 48), Distance.PIXEL)));

		// Search icon - blue
		row1.subWidgetAdd(Icon.create("Search")
				.fill(new Color(0.2f, 0.6f, 1.0f))
				.size(new Dimension2f(new Vector2f(48, 48), Distance.PIXEL)));

		// Add icon - green
		row1.subWidgetAdd(Icon.create("Add")
				.fill(new Color(0.2f, 0.8f, 0.2f))
				.size(new Dimension2f(new Vector2f(48, 48), Distance.PIXEL)));

		// Close icon - red
		row1.subWidgetAdd(Icon.create("Close")
				.fill(new Color(1.0f, 0.2f, 0.2f))
				.size(new Dimension2f(new Vector2f(48, 48), Distance.PIXEL)));

		// Menu icon - yellow
		row1.subWidgetAdd(Icon.create("Menu")
				.fill(new Color(1.0f, 0.8f, 0.0f))
				.size(new Dimension2f(new Vector2f(48, 48), Distance.PIXEL)));

		// Row 2: Different sizes
		final var row2 = new Sizer(DisplayMode.HORIZONTAL);
		row2.setPropertyExpand(Vector2b.TRUE_FALSE);
		row2.setPropertyFill(Vector2b.TRUE);
		mainSizer.subWidgetAdd(row2);

		// Small (24px)
		row2.subWidgetAdd(Icon.create("Settings")
				.fill(Color.WHITE)
				.size(new Dimension2f(new Vector2f(24, 24), Distance.PIXEL)));

		// Medium (32px)
		row2.subWidgetAdd(Icon.create("Settings")
				.fill(Color.WHITE)
				.size(new Dimension2f(new Vector2f(32, 32), Distance.PIXEL)));

		// Large (48px)
		row2.subWidgetAdd(Icon.create("Settings")
				.fill(Color.WHITE)
				.size(new Dimension2f(new Vector2f(48, 48), Distance.PIXEL)));

		// Extra large (64px)
		row2.subWidgetAdd(Icon.create("Settings")
				.fill(Color.WHITE)
				.size(new Dimension2f(new Vector2f(64, 64), Distance.PIXEL)));

		// Row 3: Navigation arrows
		final var row3 = new Sizer(DisplayMode.HORIZONTAL);
		row3.setPropertyExpand(Vector2b.TRUE_FALSE);
		row3.setPropertyFill(Vector2b.TRUE);
		mainSizer.subWidgetAdd(row3);

		row3.subWidgetAdd(Icon.create("ArrowLeft")
				.fill(new Color(0.5f, 0.5f, 1.0f))
				.stroke(new Color(0.3f, 0.3f, 0.8f))
				.size(new Dimension2f(new Vector2f(48, 48), Distance.PIXEL)));

		row3.subWidgetAdd(Icon.create("ArrowUp")
				.fill(new Color(0.5f, 0.5f, 1.0f))
				.stroke(new Color(0.3f, 0.3f, 0.8f))
				.size(new Dimension2f(new Vector2f(48, 48), Distance.PIXEL)));

		row3.subWidgetAdd(Icon.create("ArrowDown")
				.fill(new Color(0.5f, 0.5f, 1.0f))
				.stroke(new Color(0.3f, 0.3f, 0.8f))
				.size(new Dimension2f(new Vector2f(48, 48), Distance.PIXEL)));

		row3.subWidgetAdd(Icon.create("ArrowRight")
				.fill(new Color(0.5f, 0.5f, 1.0f))
				.stroke(new Color(0.3f, 0.3f, 0.8f))
				.size(new Dimension2f(new Vector2f(48, 48), Distance.PIXEL)));

		// Row 4: Action icons
		final var row4 = new Sizer(DisplayMode.HORIZONTAL);
		row4.setPropertyExpand(Vector2b.TRUE_FALSE);
		row4.setPropertyFill(Vector2b.TRUE);
		mainSizer.subWidgetAdd(row4);

		row4.subWidgetAdd(Icon.create("Edit")
				.fill(new Color(0.9f, 0.7f, 0.2f))
				.size(new Dimension2f(new Vector2f(48, 48), Distance.PIXEL)));

		row4.subWidgetAdd(Icon.create("Delete")
				.fill(new Color(0.9f, 0.3f, 0.3f))
				.size(new Dimension2f(new Vector2f(48, 48), Distance.PIXEL)));

		row4.subWidgetAdd(Icon.create("Save")
				.fill(new Color(0.3f, 0.7f, 0.3f))
				.size(new Dimension2f(new Vector2f(48, 48), Distance.PIXEL)));

		row4.subWidgetAdd(Icon.create("Check")
				.fill(new Color(0.2f, 0.9f, 0.2f))
				.stroke(new Color(0.1f, 0.6f, 0.1f))
				.size(new Dimension2f(new Vector2f(48, 48), Distance.PIXEL)));

		// Row 5: File icons
		final var row5 = new Sizer(DisplayMode.HORIZONTAL);
		row5.setPropertyExpand(Vector2b.TRUE_FALSE);
		row5.setPropertyFill(Vector2b.TRUE);
		mainSizer.subWidgetAdd(row5);

		row5.subWidgetAdd(Icon.create("Folder")
				.fill(new Color(1.0f, 0.8f, 0.3f))
				.size(new Dimension2f(new Vector2f(48, 48), Distance.PIXEL)));

		row5.subWidgetAdd(Icon.create("File")
				.fill(Color.WHITE)
				.stroke(Color.GRAY)
				.size(new Dimension2f(new Vector2f(48, 48), Distance.PIXEL)));

		return mainSizer;
	}

	@Override
	public String getTitle() {
		return "Icon Widget";
	}
}
