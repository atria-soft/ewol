package sample.atriasoft.ewol.simpleWindowsWithImage;

import org.atriasoft.etk.Dimension3f;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.ImageDisplay;

import sample.atriasoft.ewol.BasicWindows;

public class MainWindows extends BasicWindows {
	
	public static void eventButtonChangeImage(final MainWindows self) {
		self.testWidget.setPropertySource(new Uri("DATA", "mireC.png"));
	}
	
	public static void eventButtonChangeKeepRatio(final MainWindows self) {
		final boolean state = self.testWidget.isPropertyKeepRatio();
		self.testWidget.setPropertyKeepRatio(!state);
		//self.buttonAspectRatio.setPropertyValue(state ? "fkeep aspect ratio" : "un-keep aspect ratio");
	}
	
	ImageDisplay testWidget;
	Button buttonAspectRatio;
	
	public MainWindows() {
		//! [ewol_sample_HW_windows_title]
		setPropertyTitle("Simple Image");
		
		this.testWidget = new ImageDisplay();
		this.testWidget.setPropertySource(new Uri("DATA", "mireA.png"));
		this.testWidget.setPropertyExpand(Vector3b.TRUE);
		this.testWidget.setPropertyFill(Vector3b.TRUE);
		this.testWidget.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
		this.setTestWidget(this.testWidget);
		{
			final Button button = Button.createLabelButton("Change image");
			button.setPropertyExpand(Vector3b.FALSE);
			button.setPropertyFill(Vector3b.FALSE);
			button.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			this.addButton(button);
			button.signalClick.connectAuto(this, MainWindows::eventButtonChangeImage);
		}
		this.buttonAspectRatio = Button.createLabelButton("keep aspect ratio");
		this.buttonAspectRatio.setPropertyExpand(Vector3b.FALSE);
		this.buttonAspectRatio.setPropertyFill(Vector3b.FALSE);
		this.buttonAspectRatio.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
		this.addButton(this.buttonAspectRatio);
		this.buttonAspectRatio.signalClick.connectAuto(this, MainWindows::eventButtonChangeKeepRatio);
	}
}
