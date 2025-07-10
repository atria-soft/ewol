package sample.atriasoft.ewol;

import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.Widget;

public class TestWidgetButton implements TestWidgetInterface {

	@Override
	public Widget getWidget() {
		return Button.createLabelButton("A simple Label");
	}

	@Override
	public String getTitle() {
		return "Simple CheckBox";
	}
}
