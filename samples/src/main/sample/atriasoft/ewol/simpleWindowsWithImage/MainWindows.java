package sample.atriasoft.ewol.simpleWindowsWithImage;

import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.ImageDisplay;

import sample.atriasoft.ewol.BasicWindows;

public class MainWindows extends BasicWindows {

	public static void eventButtonChangeImage(final MainWindows self, final Boolean value) {
		if (value) {
			self.testWidget.setPropertySource(new Uri("DATA", "mireC.png"));
		} else {
			self.testWidget.setPropertySource(new Uri("DATA", "mireA.png"));
		}
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
		this.testWidget.setPropertyExpand(Vector2b.TRUE);
		this.testWidget.setPropertyFill(Vector2b.TRUE);
		this.testWidget.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
		setTestWidget(this.testWidget);
		{
			//			final Button button = Button.createToggleLabelButton("mireA.png", "mireC.png");
			//			button.setPropertyExpand(Vector2b.FALSE);
			//			button.setPropertyFill(Vector2b.FALSE);
			//			button.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
			//			this.addButton(button);
			//			button.signalValue.connectAuto(this, MainWindows::eventButtonChangeImage);
		}
		this.buttonAspectRatio = Button.createLabelButton("keep aspect ratio");
		this.buttonAspectRatio.setPropertyExpand(Vector2b.FALSE);
		this.buttonAspectRatio.setPropertyFill(Vector2b.FALSE);
		this.buttonAspectRatio.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
		addButton(this.buttonAspectRatio);
		this.buttonAspectRatio.signalClick.connectAuto(this, MainWindows::eventButtonChangeKeepRatio);
	}
}
