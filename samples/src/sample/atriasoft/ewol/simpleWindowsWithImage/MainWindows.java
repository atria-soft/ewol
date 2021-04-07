package sample.atriasoft.ewol.simpleWindowsWithImage;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Windows;

public class MainWindows extends Windows {
	
	public MainWindows() {
		//! [ewol_sample_HW_windows_title]
		setPropertyTitle("Simple sample test");
		//! [ewol_sample_HW_windows_label]
		//! [ewol_sample_HW_windows_title]
		Label simpleLabel = new Label();
		simpleLabel.setPropertyValue("Hello <font color='blue'>World</font>");
		simpleLabel.setPropertyExpand(new Vector2b(true, true));
		simpleLabel.setPropertyFill(new Vector2b(true, true));
		setSubWidget(simpleLabel);
		//! [ewol_sample_HW_windows_label]
	}
}
