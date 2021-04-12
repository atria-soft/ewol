package sample.atriasoft.ewol.simpleWindowsWithImage;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.Spacer;
import org.atriasoft.ewol.widget.Windows;

public class MainWindows extends Windows {
	
	public MainWindows() {
		//! [ewol_sample_HW_windows_title]
		setPropertyTitle("Simple sample test");
		//! [ewol_sample_HW_windows_label]
		//! [ewol_sample_HW_windows_title]
		/*
		Label simpleLabel = new Label();
		simpleLabel.setPropertyValue("He<b>llo</b> <font color='blue'>World</font><br/><br/> coucou comment ca vas ???<br/>sdsdfgsdfgdsfgsZESRTZAERÉ");
		simpleLabel.setPropertyExpand(new Vector2b(true, true));
		simpleLabel.setPropertyFill(new Vector2b(true, true));
		setSubWidget(simpleLabel);
		*/
		//! [ewol_sample_HW_windows_label]
		Spacer simpleSpacer = new Spacer();
		simpleSpacer.setPropertyExpand(new Vector2b(true, true));
		simpleSpacer.setPropertyFill(new Vector2b(true, true));
		setSubWidget(simpleSpacer);
	}
}
