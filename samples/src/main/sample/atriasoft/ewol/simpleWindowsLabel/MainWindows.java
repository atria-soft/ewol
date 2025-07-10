package sample.atriasoft.ewol.simpleWindowsLabel;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Spacer;

import sample.atriasoft.ewol.BasicWindows;

public class MainWindows extends BasicWindows {
	
	public MainWindows() {
		//! [ewol_sample_HW_windows_title]
		setPropertyTitle("Simple sample test");
		//EwolObject.getContext().getFontDefault().setName("FreeSans");
		//! [ewol_sample_HW_windows_label]
		if (true) {
			//! [ewol_sample_HW_windows_title]
			final Label simpleLabel = new Label();
			simpleLabel.setPropertyValue("He<b>llo.</b> <font color='blue'>World</font><br/><br/>  - How are You ???<br/>  - Not so Well, I break my leg.<br/><br/><center>The end</center>");
			simpleLabel.setPropertyExpand(Vector2b.TRUE);
			simpleLabel.setPropertyFill(Vector2b.TRUE);
			this.setTestWidget(simpleLabel);
			//! [ewol_sample_HW_windows_label]
		} else {
			final Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyExpand(Vector2b.TRUE);
			simpleSpacer.setPropertyFill(Vector2b.TRUE);
			this.setTestWidget(simpleSpacer);
		}
	}
}
