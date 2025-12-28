package sample.atriasoft.ewol;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.widget.Box;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Scroll;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Widget;

public class TestWidgetScroll implements TestWidgetInterface {
	
	@Override
	public Widget getWidget() {
		// Create a large content that exceeds the viewport
		final Sizer content = Sizer.vertical();
		content.expand(true, false).fill(true, false);
		
		// Add many items to make content scrollable
		for (int i = 1; i <= 100; i++) {
			final var itemBox = new Box();
			// Alternate colors for visibility
			if (i % 2 == 0) {
				itemBox.setPropertyColor(new Color(0x40, 0x80, 0xC0, 0xFF));
			} else {
				itemBox.setPropertyColor(new Color(0x30, 0x60, 0x90, 0xFF));
			}
			itemBox.setPropertyMinSize(new Dimension2f(new Vector2f(200, 40), Distance.PIXEL));
			itemBox.expand(true, false).fill(true, false);
			
			final Sizer itemSizer = Sizer.horizontal();
			itemSizer.expand(true, false).fill(true, false);
			itemSizer.subWidgetAdd(Label.create("Item " + i + " - Scroll to see more items"));
			itemSizer.subWidgetAdd(itemBox);
			
			content.subWidgetAdd(itemSizer);
		}
		
		// Wrap content in a Scroll widget
		final var scroll = Scroll.create().content(content).hover(true).expand(true, true).fill(true, true);
		
		return scroll;
	}
	
	@Override
	public String getTitle() {
		return "Scroll";
	}
}
