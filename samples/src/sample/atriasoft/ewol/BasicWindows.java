package sample.atriasoft.ewol;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension3f;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;
import org.atriasoft.ewol.widget.Spacer;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.ewol.widget.Windows;

public class BasicWindows extends Windows {
	public static void eventButtonChangeGravity(final BasicWindows self) {
		Gravity state = self.testWidget.getPropertyGravity();
		// TODO: I change the gravity model to integrate the 3rd rank...
		/*state = switch (state) {
			case BUTTOM -> Gravity.BOTTOM_LEFT;
			case BUTTOM_LEFT -> Gravity.BOTTOM_RIGHT;
			case BUTTOM_RIGHT -> Gravity.CENTER;
			case CENTER -> Gravity.LEFT;
			case LEFT -> Gravity.RIGHT;
			case RIGHT -> Gravity.TOP;
			case TOP -> Gravity.TOP_LEFT;
			case TOP_LEFT -> Gravity.TOP_RIGHT;
			case TOP_RIGHT -> Gravity.BOTTOM;
		};
		*/
		self.testWidget.setPropertyGravity(state);
		//self.buttonGravity.setPropertyValue("gravity: " + state);
	}
	
	public static void eventButtonExpandX(final BasicWindows self) {
		Vector3b state = self.testWidget.getPropertyExpand();
		self.testWidget.setPropertyExpand(state.withX(!state.x()));
		//self.buttonExpandX.setPropertyValue(state.x() ? "expand X" : "un-expand X");
	}
	
	public static void eventButtonExpandY(final BasicWindows self) {
		Vector3b state = self.testWidget.getPropertyExpand();
		self.testWidget.setPropertyExpand(state.withY(!state.y()));
		//self.buttonExpandY.setPropertyValue(state.y() ? "expand Y" : "un-expand Y");
	}
	
	public static void eventButtonFillX(final BasicWindows self) {
		Vector3b state = self.testWidget.getPropertyFill();
		self.testWidget.setPropertyFill(state.withX(!state.x()));
		//self.buttonFillX.setPropertyValue(state.x() ? "fill X" : "un-fill X");
	}
	
	public static void eventButtonFillY(final BasicWindows self) {
		Vector3b state = self.testWidget.getPropertyFill();
		self.testWidget.setPropertyFill(state.withY(!state.y()));
		//self.buttonFillY.setPropertyValue(state.y() ? "fill Y" : "un-fill Y");
	}
	
	Widget testWidget;
	Button buttonExpandX;
	Button buttonExpandY;
	Button buttonFillX;
	Button buttonFillY;
	Button buttonGravity;
	Sizer sizerTestAreaHori;
	Sizer sizerMenuHori;
	
	public BasicWindows() {
		//! [ewol_sample_HW_windows_title]
		setPropertyTitle("No title set !!! for this test");
		
		Sizer sizerVertMain = new Sizer(DisplayMode.modeVert);
		sizerVertMain.setPropertyExpand(Vector3b.TRUE);
		sizerVertMain.setPropertyFill(Vector3b.TRUE);
		setSubWidget(sizerVertMain);
		
		this.sizerMenuHori = new Sizer(DisplayMode.modeHori);
		this.sizerMenuHori.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
		this.sizerMenuHori.setPropertyLockExpand(Vector3b.TRUE);
		this.sizerMenuHori.setPropertyFill(Vector3b.TRUE);
		sizerVertMain.subWidgetAdd(this.sizerMenuHori);
		
		{
			Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_128, Distance.PIXEL));
			simpleSpacer.setPropertyColor(Color.ALICE_BLUE);
			simpleSpacer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
			simpleSpacer.setPropertyExpandIfFree(Vector3b.TRUE);
			simpleSpacer.setPropertyFill(Vector3b.TRUE);
			simpleSpacer.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			sizerVertMain.subWidgetAdd(simpleSpacer);
		}
		
		this.sizerTestAreaHori = new Sizer(DisplayMode.modeHori);
		this.sizerTestAreaHori.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
		this.sizerTestAreaHori.setPropertyExpandIfFree(Vector3b.TRUE);
		this.sizerTestAreaHori.setPropertyFill(Vector3b.TRUE_FALSE_FALSE);
		sizerVertMain.subWidgetAdd(this.sizerTestAreaHori);
		
		{
			Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.DARK_GREEN);
			simpleSpacer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
			simpleSpacer.setPropertyExpandIfFree(Vector3b.TRUE);
			simpleSpacer.setPropertyFill(Vector3b.TRUE);
			simpleSpacer.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			sizerVertMain.subWidgetAdd(simpleSpacer);
		}
		
		{
			Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.PINK);
			simpleSpacer.setPropertyExpand(Vector3b.TRUE);
			simpleSpacer.setPropertyFill(Vector3b.TRUE);
			simpleSpacer.setPropertyMinSize(new Dimension3f(new Vector3f(30, 30, 30), Distance.PIXEL));
			this.sizerMenuHori.subWidgetAdd(simpleSpacer);
		}
		{
			this.buttonExpandX = Button.createToggleLabelButton("un-expand X", "expand X");
			this.buttonExpandX.setPropertyExpand(Vector3b.FALSE);
			this.buttonExpandX.setPropertyFill(Vector3b.FALSE);
			this.buttonExpandX.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			this.sizerMenuHori.subWidgetAdd(this.buttonExpandX);
			this.buttonExpandX.signalClick.connectAuto(this, BasicWindows::eventButtonExpandX);
		}
		{
			this.buttonExpandY = Button.createToggleLabelButton("un-expand Y", "expand Y");
			this.buttonExpandY.setPropertyExpand(Vector3b.FALSE);
			this.buttonExpandY.setPropertyFill(Vector3b.FALSE);
			this.buttonExpandY.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			this.sizerMenuHori.subWidgetAdd(this.buttonExpandY);
			this.buttonExpandY.signalClick.connectAuto(this, BasicWindows::eventButtonExpandY);
		}
		{
			this.buttonFillX = Button.createToggleLabelButton("un-fill X", "fill X");
			this.buttonFillX.setPropertyExpand(Vector3b.FALSE);
			this.buttonFillX.setPropertyFill(Vector3b.FALSE);
			this.buttonFillX.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			this.sizerMenuHori.subWidgetAdd(this.buttonFillX);
			this.buttonFillX.signalClick.connectAuto(this, BasicWindows::eventButtonFillX);
		}
		{
			this.buttonFillY = Button.createToggleLabelButton("un-fill Y", "fill Y");
			this.buttonFillY.setPropertyExpand(Vector3b.FALSE);
			this.buttonFillY.setPropertyFill(Vector3b.FALSE);
			this.buttonFillY.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			this.sizerMenuHori.subWidgetAdd(this.buttonFillY);
			this.buttonFillY.signalClick.connectAuto(this, BasicWindows::eventButtonFillY);
		}
		{
			this.buttonGravity = Button.createLabelButton("gravity");
			this.buttonGravity.setPropertyExpand(Vector3b.FALSE);
			this.buttonGravity.setPropertyFill(Vector3b.FALSE);
			this.buttonGravity.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			this.sizerMenuHori.subWidgetAdd(this.buttonGravity);
			this.buttonGravity.signalClick.connectAuto(this, BasicWindows::eventButtonChangeGravity);
		}
	}
	
	public void addButton(Widget widget) {
		this.sizerMenuHori.subWidgetAdd(widget);
	}
	
	public void setTestWidget(Widget widget) {
		this.sizerTestAreaHori.subWidgetRemoveAll();
		{
			Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.CHOCOLATE);
			simpleSpacer.setPropertyExpand(Vector3b.FALSE);
			simpleSpacer.setPropertyExpandIfFree(Vector3b.TRUE);
			simpleSpacer.setPropertyFill(Vector3b.TRUE);
			simpleSpacer.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			this.sizerTestAreaHori.subWidgetAdd(simpleSpacer);
		}
		this.testWidget = widget;
		this.sizerTestAreaHori.subWidgetAdd(this.testWidget);
		{
			Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.GREEN_YELLOW);
			simpleSpacer.setPropertyExpand(Vector3b.FALSE);
			simpleSpacer.setPropertyExpandIfFree(Vector3b.TRUE);
			simpleSpacer.setPropertyFill(Vector3b.TRUE);
			simpleSpacer.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			this.sizerTestAreaHori.subWidgetAdd(simpleSpacer);
		}
	}
}