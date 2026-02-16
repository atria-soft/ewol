package sample.atriasoft.ewol;

import org.atriasoft.ewol.widget.Select;
import org.atriasoft.ewol.widget.Widget;

public class TestWidgetSelect implements TestWidgetInterface {

	@Override
	public Widget getWidget() {
		// Simple Select with a few options
		final Select select = Select.create()
				.items("Option 1", "Option 2", "Option 3", "Option 4", "Option 5", "Option 6", "Option 7", "Option 8", "Option 9")
				.placeholder("Select an option...")
				.selectedIndex(0);
		select.expand(false, false).fill(false, false);
		return select;
	}

	@Override
	public String getTitle() {
		return "Select";
	}

	@Override
	public String getDescription() {
		return "Dropdown selection list";
	}

	@Override
	public String getCategory() {
		return "Data";
	}
}
