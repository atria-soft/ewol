package sample.atriasoft.ewol;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.Entry;
import org.atriasoft.ewol.widget.Widget;

public class TestWidgetEntry implements TestWidgetInterface {
	@Override
	public Widget getWidget() {
		final var testWidget = new Entry();
		testWidget.setPropertyExpand(Vector2b.FALSE);
		testWidget.setPropertyFill(Vector2b.FALSE);

		return testWidget;
	}

	@Override
	public String getTitle() {
		return "Test Entry";
	}
}
