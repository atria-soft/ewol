package sample.atriasoft.ewol;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.widget.Box;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.ScrollView;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Widget;

public class TestWidgetScrollView implements TestWidgetInterface {

	@Override
	public Widget getWidget() {
		// Create a large content that exceeds the viewport in both directions
		final Sizer content = Sizer.vertical();
		content.expand(false, false).fill(false, false);

		// Add many rows to make content scrollable vertically
		for (int row = 1; row <= 50; row++) {
			final Sizer rowSizer = Sizer.horizontal();
			rowSizer.expand(false, false).fill(false, false);

			// Add many columns to make content scrollable horizontally
			for (int col = 1; col <= 25; col++) {
				final Box itemBox = new Box();
				// Create a checkered pattern with different colors
				if ((row + col) % 2 == 0) {
					itemBox.setPropertyColor(new Color(0x40, 0x80, 0xC0, 0xFF));
				} else {
					itemBox.setPropertyColor(new Color(0x80, 0xC0, 0x40, 0xFF));
				}
				itemBox.setPropertyMinSize(new Dimension2f(new Vector2f(120, 50), Distance.PIXEL));
				itemBox.expand(false, false).fill(false, false);

				final Sizer cellSizer = Sizer.vertical();
				cellSizer.expand(false, false).fill(false, false);
				cellSizer.subWidgetAdd(Label.create("R" + row + " C" + col));
				cellSizer.subWidgetAdd(itemBox);

				rowSizer.subWidgetAdd(cellSizer);
			}

			content.subWidgetAdd(rowSizer);
		}

		// Wrap content in a ScrollView widget with both scrollbars
		final ScrollView scrollView = ScrollView.create().content(content).showHorizontal(true).showVertical(true)
				.expand(true, true).fill(true, true);

		return scrollView;
	}

	@Override
	public String getTitle() {
		return "ScrollView";
	}
}
