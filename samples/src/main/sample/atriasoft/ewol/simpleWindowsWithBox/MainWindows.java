package sample.atriasoft.ewol.simpleWindowsWithBox;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension1f;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.ewol.widget.Box;

import sample.atriasoft.ewol.BasicWindows;

public class MainWindows extends BasicWindows {

	Box testWidget;

	public MainWindows() {
		//! [ewol_sample_HW_windows_title]
		setPropertyTitle("Simple CheckBox");
		
		final Box innerWidget = new Box();
		//this.testWidget.setPropertySource(new Uri("DATA", "mireA.png"));
		innerWidget.setPropertyExpand(Vector3b.FALSE);
		innerWidget.setPropertyFill(Vector3b.TRUE);
		innerWidget.setPropertyColor(Color.PINK);

		this.testWidget = new Box(innerWidget);
		//this.testWidget.setPropertySource(new Uri("DATA", "mireA.png"));
		this.testWidget.setPropertyExpand(Vector3b.TRUE);
		this.testWidget.setPropertyFill(Vector3b.TRUE);
		this.testWidget.setPropertyBorderWidth(new Dimension1f(10));
		this.testWidget.setPropertyBorderRadius(new Dimension1f(25));
		this.testWidget.setPropertyBorderColor(Color.BLACK);
		this.testWidget.setPropertyColor(Color.GREEN_YELLOW);
		this.testWidget.setPropertyPadding(new Dimension2f(new Vector2f(15, 15)));
		this.testWidget.setPropertyMargin(new Dimension2f(new Vector2f(25, 25)));
		setTestWidget(this.testWidget);
	}
}
