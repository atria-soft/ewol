package sample.atriasoft.ewol;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.DimensionBorderRadius;
import org.atriasoft.etk.DimensionInsets;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.widget.Box;
import org.atriasoft.ewol.widget.Widget;

public class TestWidgetBox implements TestWidgetInterface {
	@Override
	public Widget getWidget() {
		final var innerWidget = new Box();
		// this.testWidget.setPropertySource(new Uri("DATA", "mireA.png"));
		innerWidget.setPropertyExpand(Vector2b.FALSE);
		innerWidget.setPropertyExpandIfFree(Vector2b.TRUE);
		innerWidget.setPropertyFill(Vector2b.TRUE);
		innerWidget.setPropertyColor(Color.PINK);
		innerWidget.setPropertyMinSize(new Dimension2f(new Vector2f(50, 80)));

		final var testWidget = new Box(innerWidget);
		testWidget.setPropertyExpand(Vector2b.FALSE);
		testWidget.setPropertyFill(Vector2b.FALSE);
		testWidget.setPropertyBorderWidth(new DimensionInsets(10));
		testWidget.setPropertyBorderRadius(new DimensionBorderRadius(25));
		testWidget.setPropertyBorderColor(Color.BLACK);
		testWidget.setPropertyColor(Color.GREEN_YELLOW);
		testWidget.setPropertyPadding(new Dimension2f(new Vector2f(15, 15)));
		testWidget.setPropertyMargin(new Dimension2f(new Vector2f(25, 25)));

		return testWidget;
	}

	@Override
	public String getTitle() {
		return "Simple CheckBox";
	}
}
