package sample.atriasoft.ewol.simpleWindowsWithCheckBox;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.CheckBox;

import sample.atriasoft.ewol.BasicWindows;

public class MainWindows extends BasicWindows {

	CheckBox testWidget;

	public MainWindows() {
		//! [ewol_sample_HW_windows_title]
		setPropertyTitle("Simple CheckBox");

		this.testWidget = new CheckBox("<b>Hello, how Are</b> You?<br/>second-life?");
		//this.testWidget.setPropertySource(new Uri("DATA", "mireA.png"));
		this.testWidget.setPropertyExpand(Vector2b.TRUE);
		this.testWidget.setPropertyFill(Vector2b.TRUE);
		setTestWidget(this.testWidget);
		/*
		Button simpleButton = new Button();
		simpleButton.setPropertyValue("Top Button");
		simpleButton.setPropertyExpand(Vector2b.TRUE);
		simpleButton.setPropertyFill(Vector2b.TRUE);
		this.setTestWidget(simpleButton);
		*/
	}
}
