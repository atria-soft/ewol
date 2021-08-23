package sample.atriasoft.ewol.simpleWindowsWithImage;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.ImageDisplay;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;
import org.atriasoft.ewol.widget.Spacer;
import org.atriasoft.ewol.widget.Windows;

public class MainWindows extends Windows {
	ImageDisplay testWidget;
	Button buttonExpandX;
	Button buttonExpandY;
	Button buttonFillX;
	Button buttonFillY;
	Button buttonGravity;
	Button buttonAspectRatio;
	public MainWindows() {
		//! [ewol_sample_HW_windows_title]
		setPropertyTitle("Simple Image");

		Sizer sizerMain = new Sizer(DisplayMode.modeVert);
		sizerMain.setPropertyExpand(new Vector2b(true, true));
		sizerMain.setPropertyFill(new Vector2b(true, true));
		setSubWidget(sizerMain);
		
		Sizer sizerHori1 = new Sizer(DisplayMode.modeHori);
		sizerHori1.setPropertyExpand(new Vector2b(true, false));
		sizerHori1.setPropertyLockExpand(new Vector2b(true, true));
		sizerHori1.setPropertyFill(new Vector2b(true, true));
		sizerMain.subWidgetAdd(sizerHori1);

		{
			Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyMinSize(new Dimension(new Vector2f(100, 100), Distance.PIXEL));
			simpleSpacer.setPropertyColor(Color.ALICE_BLUE);
			simpleSpacer.setPropertyExpand(new Vector2b(true, false));
			simpleSpacer.setPropertyFill(new Vector2b(true, true));
			simpleSpacer.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
			sizerMain.subWidgetAdd(simpleSpacer);
		}
		
		Sizer sizerHori2 = new Sizer(DisplayMode.modeHori);
		sizerHori2.setPropertyExpand(new Vector2b(true, true));
		sizerHori2.setPropertyFill(new Vector2b(true, true));
		sizerMain.subWidgetAdd(sizerHori2);
		
		{
			Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.DARK_GREEN);
			simpleSpacer.setPropertyExpand(new Vector2b(true, false));
			simpleSpacer.setPropertyFill(new Vector2b(true, true));
			simpleSpacer.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
			sizerMain.subWidgetAdd(simpleSpacer);
		}
		{
			Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.CHOCOLATE);
			simpleSpacer.setPropertyExpand(new Vector2b(false, true));
			simpleSpacer.setPropertyFill(new Vector2b(true, true));
			simpleSpacer.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
			sizerHori2.subWidgetAdd(simpleSpacer);
		}
		this.testWidget = new ImageDisplay();
		this.testWidget.setPropertySource(new Uri("DATA", "mireA.png"));
		this.testWidget.setPropertyExpand(new Vector2b(true, true));
		this.testWidget.setPropertyFill(new Vector2b(true, true));
		sizerHori2.subWidgetAdd(this.testWidget);
		{
			Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.GREEN_YELLOW);
			simpleSpacer.setPropertyExpand(new Vector2b(false, true));
			simpleSpacer.setPropertyFill(new Vector2b(true, true));
			simpleSpacer.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
			sizerHori2.subWidgetAdd(simpleSpacer);
		}
		
		{
			Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.PINK);
			simpleSpacer.setPropertyExpand(new Vector2b(true, true));
			simpleSpacer.setPropertyFill(new Vector2b(true, true));
			simpleSpacer.setPropertyMinSize(new Dimension(new Vector2f(30, 30), Distance.PIXEL));
			sizerHori1.subWidgetAdd(simpleSpacer);
		}
		{
			this.buttonExpandX = new Button();
			this.buttonExpandX.setPropertyValue("un-expand X");
			this.buttonExpandX.setPropertyExpand(new Vector2b(false, false));
			this.buttonExpandX.setPropertyFill(new Vector2b(false, false));
			this.buttonExpandX.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
			sizerHori1.subWidgetAdd(this.buttonExpandX);
			this.buttonExpandX.signalClick.connectAuto(this, MainWindows::eventButtonExpandX);
		}
		{
			this.buttonExpandY = new Button();
			this.buttonExpandY.setPropertyValue("un-expand Y");
			this.buttonExpandY.setPropertyExpand(new Vector2b(false, false));
			this.buttonExpandY.setPropertyFill(new Vector2b(false, false));
			this.buttonExpandY.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
			sizerHori1.subWidgetAdd(this.buttonExpandY);
			this.buttonExpandY.signalClick.connectAuto(this, MainWindows::eventButtonExpandY);
		}
		{
			this.buttonFillX = new Button();
			this.buttonFillX.setPropertyValue("un-fill X");
			this.buttonFillX.setPropertyExpand(new Vector2b(false, false));
			this.buttonFillX.setPropertyFill(new Vector2b(false, false));
			this.buttonFillX.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
			sizerHori1.subWidgetAdd(this.buttonFillX);
			this.buttonFillX.signalClick.connectAuto(this, MainWindows::eventButtonFillX);
		}
		{
			this.buttonFillY = new Button();
			this.buttonFillY.setPropertyValue("un-fill Y");
			this.buttonFillY.setPropertyExpand(new Vector2b(false, false));
			this.buttonFillY.setPropertyFill(new Vector2b(false, false));
			this.buttonFillY.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
			sizerHori1.subWidgetAdd(this.buttonFillY);
			this.buttonFillY.signalClick.connectAuto(this, MainWindows::eventButtonFillY);
		}
		{
			Button button = new Button();
			button.setPropertyValue("Change image");
			button.setPropertyExpand(new Vector2b(false, false));
			button.setPropertyFill(new Vector2b(false, false));
			button.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
			sizerHori1.subWidgetAdd(button);
			button.signalClick.connectAuto(this, MainWindows::eventButtonChangeImage);
		}
		{
			this.buttonGravity = new Button();
			this.buttonGravity.setPropertyValue("gravity");
			this.buttonGravity.setPropertyExpand(new Vector2b(false, false));
			this.buttonGravity.setPropertyFill(new Vector2b(false, false));
			this.buttonGravity.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
			sizerHori1.subWidgetAdd(this.buttonGravity);
			this.buttonGravity.signalClick.connectAuto(this, MainWindows::eventButtonChangeGravity);
		}
		{
			this.buttonAspectRatio = new Button();
			this.buttonAspectRatio.setPropertyValue("keep aspect ratio");
			this.buttonAspectRatio.setPropertyExpand(new Vector2b(false, false));
			this.buttonAspectRatio.setPropertyFill(new Vector2b(false, false));
			this.buttonAspectRatio.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
			sizerHori1.subWidgetAdd(this.buttonAspectRatio);
			this.buttonAspectRatio.signalClick.connectAuto(this, MainWindows::eventButtonChangeKeepRatio);
		}
		
		
	}
	public static void eventButtonExpandX(final MainWindows self) {
		Vector2b state = self.testWidget.getPropertyExpand();
		self.testWidget.setPropertyExpand(state.withX(!state.x()));
		self.buttonExpandX.setPropertyValue(state.x()?"expand X":"un-expand X");
	}
	public static void eventButtonExpandY(final MainWindows self) {
		Vector2b state = self.testWidget.getPropertyExpand();
		self.testWidget.setPropertyExpand(state.withY(!state.y()));
		self.buttonExpandY.setPropertyValue(state.y()?"expand Y":"un-expand Y");
	}
	public static void eventButtonFillX(final MainWindows self) {
		Vector2b state = self.testWidget.getPropertyFill();
		self.testWidget.setPropertyFill(state.withX(!state.x()));
		self.buttonFillX.setPropertyValue(state.x()?"fill X":"un-fill X");
	}
	public static void eventButtonFillY(final MainWindows self) {
		Vector2b state = self.testWidget.getPropertyFill();
		self.testWidget.setPropertyFill(state.withY(!state.y()));
		self.buttonFillY.setPropertyValue(state.y()?"fill Y":"un-fill Y");
	}
	public static void eventButtonChangeKeepRatio(final MainWindows self) {
		boolean state = self.testWidget.isPropertyKeepRatio();
		self.testWidget.setPropertyKeepRatio(!state);
		self.buttonAspectRatio.setPropertyValue(state?"fkeep aspect ratio":"un-keep aspect ratio");
	}
	public static void eventButtonChangeGravity(final MainWindows self) {
		Gravity state = self.testWidget.getPropertyGravity();
		switch(state) {
			case BUTTOM:
				state = Gravity.BUTTOM_LEFT;
				break;
			case BUTTOM_LEFT:
				state = Gravity.BUTTOM_RIGHT;
				break;
			case BUTTOM_RIGHT:
				state = Gravity.CENTER;
				break;
			case CENTER:
				state = Gravity.LEFT;
				break;
			case LEFT:
				state = Gravity.RIGHT;
				break;
			case RIGHT:
				state = Gravity.TOP;
				break;
			case TOP:
				state = Gravity.TOP_LEFT;
				break;
			case TOP_LEFT:
				state = Gravity.TOP_RIGHT;
				break;
			case TOP_RIGHT:
				state = Gravity.BUTTOM;
				break;
		}
		self.testWidget.setPropertyGravity(state);
		self.buttonGravity.setPropertyValue("gravity: " + state);
	}
	public static void eventButtonChangeImage(final MainWindows self) {
		self.testWidget.setPropertySource(new Uri("DATA", "mireC.png"));
	}
}
