package sample.atriasoft.ewol.simpleWindowsWithCheckBox;

import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.ewol.widget.CheckBox;

import sample.atriasoft.ewol.BasicWindows;

public class MainWindows extends BasicWindows {
	
	CheckBox testWidget;
	
	public MainWindows() {
		//! [ewol_sample_HW_windows_title]
		setPropertyTitle("Simple CheckBox");
		
		this.testWidget = new CheckBox();
		//this.testWidget.setPropertySource(new Uri("DATA", "mireA.png"));
		this.testWidget.setPropertyExpand(Vector3b.TRUE);
		this.testWidget.setPropertyFill(Vector3b.TRUE);
		this.setTestWidget(this.testWidget);
		/*
		Button simpleButton = new Button();
		simpleButton.setPropertyValue("Top Button");
		simpleButton.setPropertyExpand(Vector3b.TRUE);
		simpleButton.setPropertyFill(Vector3b.TRUE);
		this.setTestWidget(simpleButton);
		*/
	}
}
