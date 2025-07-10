package sample.atriasoft.ewol.simpleWindowsWithBox;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension1f;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.Box;

import sample.atriasoft.ewol.BasicWindows;

public class MainWindows extends BasicWindows {

	Box testWidget;

	public MainWindows() {
		//! [ewol_sample_HW_windows_title]
		setPropertyTitle("Simple CheckBox");
		
		final Box innerWidget = new Box();
		//this.testWidget.setPropertySource(new Uri("DATA", "mireA.png"));
		innerWidget.setPropertyExpand(Vector2b.FALSE);
		innerWidget.setPropertyExpandIfFree(Vector2b.TRUE);
		innerWidget.setPropertyFill(Vector2b.TRUE);
		innerWidget.setPropertyColor(Color.PINK);
		innerWidget.setPropertyMinSize(new Dimension2f(new Vector2f(50, 80)));

		this.testWidget = new Box(innerWidget);
		this.testWidget.setPropertyExpand(Vector2b.FALSE);
		this.testWidget.setPropertyFill(Vector2b.FALSE);
		this.testWidget.setPropertyBorderWidth(new Dimension1f(10));
		this.testWidget.setPropertyBorderRadius(new Dimension1f(25));
		this.testWidget.setPropertyBorderColor(Color.BLACK);
		this.testWidget.setPropertyColor(Color.GREEN_YELLOW);
		this.testWidget.setPropertyPadding(new Dimension2f(new Vector2f(15, 15)));
		this.testWidget.setPropertyMargin(new Dimension2f(new Vector2f(25, 25)));
		setTestWidget(this.testWidget);
	}
}
