package sample.atriasoft.ewol;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Widget;

public class TestWidgetLabel implements TestWidgetInterface {

	@Override
	public Widget getWidget() {
		final var testWidget = new Label();
		testWidget.setPropertyValue(
				"He<b>llo.</b> <font color='blue'>World</font><br/><br/>  - How are You ???<br/>  - Not so Well, I break my leg.<br/><br/><center>The end</center>");
		testWidget.setPropertyExpand(Vector2b.TRUE);
		testWidget.setPropertyFill(Vector2b.TRUE);
		return testWidget;
	}

	@Override
	public String getTitle() {
		return "Simple Label test";
	}

}
