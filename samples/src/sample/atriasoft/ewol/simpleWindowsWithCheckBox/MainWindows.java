package sample.atriasoft.ewol.simpleWindowsWithCheckBox;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.CheckBox;

import sample.atriasoft.ewol.BasicWindows;

public class MainWindows extends BasicWindows {
	
	CheckBox testWidget;
	
	public MainWindows() {
		//! [ewol_sample_HW_windows_title]
		setPropertyTitle("Simple CheckBox");
		
		this.testWidget = new CheckBox();
		//this.testWidget.setPropertySource(new Uri("DATA", "mireA.png"));
		this.testWidget.setPropertyExpand(Vector2b.TRUE_TRUE);
		this.testWidget.setPropertyFill(Vector2b.TRUE_TRUE);
		this.setTestWidget(this.testWidget);
	}
}
