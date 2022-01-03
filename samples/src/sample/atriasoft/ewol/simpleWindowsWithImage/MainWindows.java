package sample.atriasoft.ewol.simpleWindowsWithImage;

import org.atriasoft.etk.Dimension;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.ImageDisplay;

import sample.atriasoft.ewol.BasicWindows;

public class MainWindows extends BasicWindows {
	
	public static void eventButtonChangeImage(final MainWindows self) {
		self.testWidget.setPropertySource(new Uri("DATA", "mireC.png"));
	}
	
	public static void eventButtonChangeKeepRatio(final MainWindows self) {
		boolean state = self.testWidget.isPropertyKeepRatio();
		self.testWidget.setPropertyKeepRatio(!state);
		self.buttonAspectRatio.setPropertyValue(state ? "fkeep aspect ratio" : "un-keep aspect ratio");
	}
	
	ImageDisplay testWidget;
	Button buttonAspectRatio;
	
	public MainWindows() {
		//! [ewol_sample_HW_windows_title]
		setPropertyTitle("Simple Image");
		
		this.testWidget = new ImageDisplay();
		this.testWidget.setPropertySource(new Uri("DATA", "mireA.png"));
		this.testWidget.setPropertyExpand(new Vector2b(true, true));
		this.testWidget.setPropertyFill(new Vector2b(true, true));
		this.testWidget.setPropertyMinSize(new Dimension(Vector2f.VALUE_16, Distance.PIXEL));
		this.setTestWidget(this.testWidget);
		{
			Button button = new Button();
			button.setPropertyValue("Change image");
			button.setPropertyExpand(new Vector2b(false, false));
			button.setPropertyFill(new Vector2b(false, false));
			button.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
			this.addButton(button);
			button.signalClick.connectAuto(this, MainWindows::eventButtonChangeImage);
		}
		this.buttonAspectRatio = new Button();
		this.buttonAspectRatio.setPropertyValue("keep aspect ratio");
		this.buttonAspectRatio.setPropertyExpand(new Vector2b(false, false));
		this.buttonAspectRatio.setPropertyFill(new Vector2b(false, false));
		this.buttonAspectRatio.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
		this.addButton(this.buttonAspectRatio);
		this.buttonAspectRatio.signalClick.connectAuto(this, MainWindows::eventButtonChangeKeepRatio);
	}
}
