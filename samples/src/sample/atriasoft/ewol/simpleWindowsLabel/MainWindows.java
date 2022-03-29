package sample.atriasoft.ewol.simpleWindowsLabel;

import org.atriasoft.etk.math.Vector3b;
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
			Label simpleLabel = new Label();
			simpleLabel.setPropertyValue(
					"He<b>llo.</b> <font color='blue'>World</font><br/><br/>  - Coucou comment ca vas ???<br/>  - Pas trop bien, je me suis cassé la jambe.<br/><br/><center>The end</center>");
			simpleLabel.setPropertyExpand(Vector3b.TRUE);
			simpleLabel.setPropertyFill(Vector3b.TRUE);
			this.setTestWidget(simpleLabel);
			//! [ewol_sample_HW_windows_label]
		} else {
			Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyExpand(Vector3b.TRUE);
			simpleSpacer.setPropertyFill(Vector3b.TRUE);
			this.setTestWidget(simpleSpacer);
		}
	}
}
