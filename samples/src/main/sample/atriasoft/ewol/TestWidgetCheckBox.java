package sample.atriasoft.ewol;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.CheckBox;
import org.atriasoft.ewol.widget.Widget;

public class TestWidgetCheckBox implements TestWidgetInterface {

	@Override
	public Widget getWidget() {
		final var testWidget = new CheckBox("<b>Hello, how Are</b> You?<br/>second-life?");
		// this.testWidget.setPropertySource(new Uri("DATA", "mireA.png"));
		testWidget.setPropertyExpand(Vector2b.TRUE);
		testWidget.setPropertyFill(Vector2b.TRUE);
		return testWidget;
	}

	@Override
	public String getTitle() {
		return "Simple CheckBox";
	}

	@Override
	public String getDescription() {
		return "Boolean checkbox with label";
	}

	@Override
	public String getCategory() {
		return "Basic";
	}
}
