package sample.atriasoft.ewol.simpleWindowsWithButton;

import org.atriasoft.ewol.widget.Box;
import org.atriasoft.ewol.widget.Button;

import sample.atriasoft.ewol.BasicWindows;

public class MainWindows extends BasicWindows {

	Box testWidget;

	public MainWindows() {
		//! [ewol_sample_HW_windows_title]
		setPropertyTitle("Simple Button");
		this.testWidget = Button.createLabelButton("A simple Label");
		//this.testWidget.setPropertySource(new Uri("DATA", "mireA.png"));
		//		this.testWidget.setPropertyExpand(Vector2b.TRUE);
		//		this.testWidget.setPropertyFill(Vector2b.TRUE);
		//		this.testWidget.setPropertyBorderWidth(new Dimension1f(10));
		//		this.testWidget.setPropertyBorderRadius(new Dimension1f(25));
		//		this.testWidget.setPropertyBorderColor(Color.BLACK);
		//		this.testWidget.setPropertyColor(Color.GREEN_YELLOW);
		//		this.testWidget.setPropertyPadding(new Dimension2f(new Vector2f(15, 15)));
		//		this.testWidget.setPropertyMargin(new Dimension2f(new Vector2f(25, 25)));
		setTestWidget(this.testWidget);
	}
}
