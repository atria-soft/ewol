package sample.atriasoft.ewol;

import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.ImageDisplay;
import org.atriasoft.ewol.widget.Widget;

public class TestWidgetImage implements TestWidgetInterface {

	ImageDisplay testWidget;
	Button buttonAspectRatio;

	@Override
	public Widget getWidget() {
		this.testWidget = new ImageDisplay();
		this.testWidget.setPropertySource(new Uri("DATA", "mireA.png"));
		this.testWidget.setPropertyExpand(Vector2b.TRUE);
		this.testWidget.setPropertyFill(Vector2b.TRUE);
		this.testWidget.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));

		this.buttonAspectRatio = Button.createLabelButton("keep aspect ratio");
		this.buttonAspectRatio.setPropertyExpand(Vector2b.FALSE);
		this.buttonAspectRatio.setPropertyFill(Vector2b.FALSE);
		this.buttonAspectRatio.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
		////////////// addButton(this.buttonAspectRatio);
		this.buttonAspectRatio.signalClick.connectAuto(this, TestWidgetImage::eventButtonChangeKeepRatio);
		return this.testWidget;
	}

	@Override
	public String getTitle() {
		return "Simple Image";
	}

	@Override
	public String getDescription() {
		return "Bitmap image display";
	}

	@Override
	public String getCategory() {
		return "Basic";
	}

	public static void eventButtonChangeImage(final TestWidgetImage self, final Boolean value) {
		if (value) {
			self.testWidget.setPropertySource(new Uri("DATA", "mireC.png"));
		} else {
			self.testWidget.setPropertySource(new Uri("DATA", "mireA.png"));
		}
	}

	public static void eventButtonChangeKeepRatio(final TestWidgetImage self) {
		final var state = self.testWidget.isPropertyKeepRatio();
		self.testWidget.setPropertyKeepRatio(!state);
		// self.buttonAspectRatio.setPropertyValue(state ? "fkeep aspect ratio" :
		// "un-keep aspect ratio");
	}

}
