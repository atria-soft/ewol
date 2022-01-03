package sample.atriasoft.ewol;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
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
		state = switch (state) {
			case BUTTOM -> Gravity.BUTTOM_LEFT;
			case BUTTOM_LEFT -> Gravity.BUTTOM_RIGHT;
			case BUTTOM_RIGHT -> Gravity.CENTER;
			case CENTER -> Gravity.LEFT;
			case LEFT -> Gravity.RIGHT;
			case RIGHT -> Gravity.TOP;
			case TOP -> Gravity.TOP_LEFT;
			case TOP_LEFT -> Gravity.TOP_RIGHT;
			case TOP_RIGHT -> Gravity.BUTTOM;
		};
		self.testWidget.setPropertyGravity(state);
		self.buttonGravity.setPropertyValue("gravity: " + state);
	}
	
	public static void eventButtonExpandX(final BasicWindows self) {
		Vector2b state = self.testWidget.getPropertyExpand();
		self.testWidget.setPropertyExpand(state.withX(!state.x()));
		self.buttonExpandX.setPropertyValue(state.x() ? "expand X" : "un-expand X");
	}
	
	public static void eventButtonExpandY(final BasicWindows self) {
		Vector2b state = self.testWidget.getPropertyExpand();
		self.testWidget.setPropertyExpand(state.withY(!state.y()));
		self.buttonExpandY.setPropertyValue(state.y() ? "expand Y" : "un-expand Y");
	}
	
	public static void eventButtonFillX(final BasicWindows self) {
		Vector2b state = self.testWidget.getPropertyFill();
		self.testWidget.setPropertyFill(state.withX(!state.x()));
		self.buttonFillX.setPropertyValue(state.x() ? "fill X" : "un-fill X");
	}
	
	public static void eventButtonFillY(final BasicWindows self) {
		Vector2b state = self.testWidget.getPropertyFill();
		self.testWidget.setPropertyFill(state.withY(!state.y()));
		self.buttonFillY.setPropertyValue(state.y() ? "fill Y" : "un-fill Y");
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
		sizerVertMain.setPropertyExpand(Vector2b.TRUE_TRUE);
		sizerVertMain.setPropertyFill(Vector2b.TRUE_TRUE);
		setSubWidget(sizerVertMain);
		
		this.sizerMenuHori = new Sizer(DisplayMode.modeHori);
		this.sizerMenuHori.setPropertyExpand(Vector2b.TRUE_FALSE);
		this.sizerMenuHori.setPropertyLockExpand(Vector2b.TRUE_TRUE);
		this.sizerMenuHori.setPropertyFill(Vector2b.TRUE_TRUE);
		sizerVertMain.subWidgetAdd(this.sizerMenuHori);
		
		{
			Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyMinSize(new Dimension(new Vector2f(100, 100), Distance.PIXEL));
			simpleSpacer.setPropertyColor(Color.ALICE_BLUE);
			simpleSpacer.setPropertyExpand(Vector2b.TRUE_FALSE);
			simpleSpacer.setPropertyExpandIfFree(Vector2b.TRUE_TRUE);
			simpleSpacer.setPropertyFill(Vector2b.TRUE_TRUE);
			simpleSpacer.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
			sizerVertMain.subWidgetAdd(simpleSpacer);
		}
		
		this.sizerTestAreaHori = new Sizer(DisplayMode.modeHori);
		this.sizerTestAreaHori.setPropertyExpand(Vector2b.TRUE_FALSE);
		this.sizerTestAreaHori.setPropertyExpandIfFree(Vector2b.TRUE_TRUE);
		this.sizerTestAreaHori.setPropertyFill(Vector2b.TRUE_FALSE);
		sizerVertMain.subWidgetAdd(this.sizerTestAreaHori);
		
		{
			Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.DARK_GREEN);
			simpleSpacer.setPropertyExpand(Vector2b.TRUE_FALSE);
			simpleSpacer.setPropertyExpandIfFree(Vector2b.TRUE_TRUE);
			simpleSpacer.setPropertyFill(Vector2b.TRUE_TRUE);
			simpleSpacer.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
			sizerVertMain.subWidgetAdd(simpleSpacer);
		}
		
		{
			Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.PINK);
			simpleSpacer.setPropertyExpand(Vector2b.TRUE_TRUE);
			simpleSpacer.setPropertyFill(Vector2b.TRUE_TRUE);
			simpleSpacer.setPropertyMinSize(new Dimension(new Vector2f(30, 30), Distance.PIXEL));
			this.sizerMenuHori.subWidgetAdd(simpleSpacer);
		}
		{
			this.buttonExpandX = new Button();
			this.buttonExpandX.setPropertyValue("un-expand X");
			this.buttonExpandX.setPropertyExpand(Vector2b.FALSE_FALSE);
			this.buttonExpandX.setPropertyFill(Vector2b.FALSE_FALSE);
			this.buttonExpandX.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
			this.sizerMenuHori.subWidgetAdd(this.buttonExpandX);
			this.buttonExpandX.signalClick.connectAuto(this, BasicWindows::eventButtonExpandX);
		}
		{
			this.buttonExpandY = new Button();
			this.buttonExpandY.setPropertyValue("un-expand Y");
			this.buttonExpandY.setPropertyExpand(Vector2b.FALSE_FALSE);
			this.buttonExpandY.setPropertyFill(Vector2b.FALSE_FALSE);
			this.buttonExpandY.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
			this.sizerMenuHori.subWidgetAdd(this.buttonExpandY);
			this.buttonExpandY.signalClick.connectAuto(this, BasicWindows::eventButtonExpandY);
		}
		{
			this.buttonFillX = new Button();
			this.buttonFillX.setPropertyValue("un-fill X");
			this.buttonFillX.setPropertyExpand(Vector2b.FALSE_FALSE);
			this.buttonFillX.setPropertyFill(Vector2b.FALSE_FALSE);
			this.buttonFillX.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
			this.sizerMenuHori.subWidgetAdd(this.buttonFillX);
			this.buttonFillX.signalClick.connectAuto(this, BasicWindows::eventButtonFillX);
		}
		{
			this.buttonFillY = new Button();
			this.buttonFillY.setPropertyValue("un-fill Y");
			this.buttonFillY.setPropertyExpand(Vector2b.FALSE_FALSE);
			this.buttonFillY.setPropertyFill(Vector2b.FALSE_FALSE);
			this.buttonFillY.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
			this.sizerMenuHori.subWidgetAdd(this.buttonFillY);
			this.buttonFillY.signalClick.connectAuto(this, BasicWindows::eventButtonFillY);
		}
		{
			this.buttonGravity = new Button();
			this.buttonGravity.setPropertyValue("gravity");
			this.buttonGravity.setPropertyExpand(Vector2b.FALSE_FALSE);
			this.buttonGravity.setPropertyFill(Vector2b.FALSE_FALSE);
			this.buttonGravity.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
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
			simpleSpacer.setPropertyExpand(Vector2b.FALSE_FALSE);
			simpleSpacer.setPropertyExpandIfFree(Vector2b.TRUE_TRUE);
			simpleSpacer.setPropertyFill(Vector2b.TRUE_TRUE);
			simpleSpacer.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
			this.sizerTestAreaHori.subWidgetAdd(simpleSpacer);
		}
		this.testWidget = widget;
		this.sizerTestAreaHori.subWidgetAdd(this.testWidget);
		{
			Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.GREEN_YELLOW);
			simpleSpacer.setPropertyExpand(Vector2b.FALSE_FALSE);
			simpleSpacer.setPropertyExpandIfFree(Vector2b.TRUE_TRUE);
			simpleSpacer.setPropertyFill(Vector2b.TRUE_TRUE);
			simpleSpacer.setPropertyMinSize(new Dimension(new Vector2f(10, 10), Distance.PIXEL));
			this.sizerTestAreaHori.subWidgetAdd(simpleSpacer);
		}
	}
}