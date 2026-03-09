package sample.atriasoft.ewol;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.RadioGroup;
import org.atriasoft.ewol.widget.Widget;

public class TestWidgetRadioButton implements TestWidgetInterface {

	@Override
	public Widget getWidget() {
		final RadioGroup group = RadioGroup.vertical()
				.addRadio("Option A")
				.addRadio("Option <b>B</b> (bold)")
				.addRadio("Option C")
				.selectedIndex(0);
		group.setPropertyExpand(Vector2b.TRUE);
		group.setPropertyFill(Vector2b.TRUE);
		return group;
	}

	@Override
	public String getTitle() {
		return "Radio Button";
	}

	@Override
	public String getDescription() {
		return "Exclusive selection radio buttons with text labels";
	}

	@Override
	public String getCategory() {
		return "Basic";
	}
}
