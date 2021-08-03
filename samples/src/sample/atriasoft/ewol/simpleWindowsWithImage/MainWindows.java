package sample.atriasoft.ewol.simpleWindowsWithImage;

import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.ImageDisplay;
import org.atriasoft.ewol.widget.Windows;

public class MainWindows extends Windows {
	
	public MainWindows() {
		//! [ewol_sample_HW_windows_title]
		setPropertyTitle("Simple label");
		//EwolObject.getContext().getFontDefault().setName("FreeSans");
		//! [ewol_sample_HW_windows_label]
		//! [ewol_sample_HW_windows_title]
		ImageDisplay testWidget = new ImageDisplay();
		testWidget.setPropertySource(new Uri("DATA", "mireA.png"));
		testWidget.setPropertyExpand(new Vector2b(true, true));
		testWidget.setPropertyFill(new Vector2b(true, true));
		setSubWidget(testWidget);
		//! [ewol_sample_HW_windows_label]
		
	}
}
