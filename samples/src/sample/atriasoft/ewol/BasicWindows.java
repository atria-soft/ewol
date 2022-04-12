package sample.atriasoft.ewol;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension3f;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.GravityDepth;
import org.atriasoft.ewol.GravityHorizontal;
import org.atriasoft.ewol.GravityVertical;
import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;
import org.atriasoft.ewol.widget.Spacer;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.ewol.widget.Windows;

public class BasicWindows extends Windows {
	private static final String LABEL_GRAVITY = "gravity<br/>";
	
	public static void eventButtonChangeGravity(final BasicWindows self) {
		Gravity state = self.testWidget.getPropertyGravity();
		// TODO: I change the gravity model to integrate the 3rd rank...
		if (state.x() == GravityHorizontal.LEFT && state.y() == GravityVertical.BOTTOM) {
			state = new Gravity(GravityHorizontal.CENTER, GravityVertical.BOTTOM, GravityDepth.CENTER);
		} else if (state.x() == GravityHorizontal.CENTER && state.y() == GravityVertical.BOTTOM) {
			state = new Gravity(GravityHorizontal.RIGHT, GravityVertical.BOTTOM, GravityDepth.CENTER);
		} else if (state.x() == GravityHorizontal.RIGHT && state.y() == GravityVertical.BOTTOM) {
			state = new Gravity(GravityHorizontal.LEFT, GravityVertical.CENTER, GravityDepth.CENTER);
			
		} else if (state.x() == GravityHorizontal.LEFT && state.y() == GravityVertical.CENTER) {
			state = new Gravity(GravityHorizontal.CENTER, GravityVertical.CENTER, GravityDepth.CENTER);
		} else if (state.x() == GravityHorizontal.CENTER && state.y() == GravityVertical.CENTER) {
			state = new Gravity(GravityHorizontal.RIGHT, GravityVertical.CENTER, GravityDepth.CENTER);
		} else if (state.x() == GravityHorizontal.RIGHT && state.y() == GravityVertical.CENTER) {
			state = new Gravity(GravityHorizontal.LEFT, GravityVertical.TOP, GravityDepth.CENTER);
			
		} else if (state.x() == GravityHorizontal.LEFT && state.y() == GravityVertical.TOP) {
			state = new Gravity(GravityHorizontal.CENTER, GravityVertical.TOP, GravityDepth.CENTER);
		} else if (state.x() == GravityHorizontal.CENTER && state.y() == GravityVertical.TOP) {
			state = new Gravity(GravityHorizontal.RIGHT, GravityVertical.TOP, GravityDepth.CENTER);
		} else if (state.x() == GravityHorizontal.RIGHT && state.y() == GravityVertical.TOP) {
			state = new Gravity(GravityHorizontal.LEFT, GravityVertical.BOTTOM, GravityDepth.CENTER);
		}
		final Label gravLabel = (Label) (self.buttonGravity.getSubWidgets()[0]);
		gravLabel.setPropertyValue(LABEL_GRAVITY + state.toString());
		self.testWidget.setPropertyGravity(state);
	}
	
	public static void eventButtonExpandX(final BasicWindows self, final Boolean value) {
		final Vector3b state = self.testWidget.getPropertyExpand();
		self.testWidget.setPropertyExpand(state.withX(value));
		Log.info("set expand X: {}", state.x() ? "un-expand X" : "expand X");
	}
	
	public static void eventButtonExpandY(final BasicWindows self, final Boolean value) {
		final Vector3b state = self.testWidget.getPropertyExpand();
		self.testWidget.setPropertyExpand(state.withY(value));
		Log.info("set expand Y: {}", state.y() ? "un-expand Y" : "expand Y");
	}
	
	public static void eventButtonFillX(final BasicWindows self, final Boolean value) {
		final Vector3b state = self.testWidget.getPropertyFill();
		self.testWidget.setPropertyFill(state.withX(value));
		Log.info("set fill X: {}", state.x() ? "un-fill X" : "fill X");
	}
	
	public static void eventButtonFillY(final BasicWindows self, final Boolean value) {
		final Vector3b state = self.testWidget.getPropertyFill();
		self.testWidget.setPropertyFill(state.withY(value));
		Log.info("set fill Y: {}", state.y() ? "un-fill Y" : "fill Y");
	}
	
	Widget testWidget;
	Button buttonExpandX;
	Button buttonExpandY;
	Button buttonFillX;
	Button buttonFillY;
	Button buttonGravity;
	Sizer sizerTestAreaHori;
	Sizer sizerMenuHori;
	Gravity basicGravity = Gravity.BOTTOM_LEFT;
	
	public BasicWindows() {
		//! [ewol_sample_HW_windows_title]
		setPropertyTitle("No title set !!! for this test");
		
		final Sizer sizerVertMain = new Sizer(DisplayMode.modeVert);
		sizerVertMain.setPropertyExpand(Vector3b.TRUE);
		sizerVertMain.setPropertyFill(Vector3b.TRUE);
		setSubWidget(sizerVertMain);
		
		this.sizerMenuHori = new Sizer(DisplayMode.modeHori);
		this.sizerMenuHori.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
		this.sizerMenuHori.setPropertyLockExpand(Vector3b.TRUE);
		this.sizerMenuHori.setPropertyFill(Vector3b.TRUE);
		sizerVertMain.subWidgetAdd(this.sizerMenuHori);
		
		{
			final Spacer simpleSpacer = new Spacer();
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
			final Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.DARK_GREEN);
			simpleSpacer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
			simpleSpacer.setPropertyExpandIfFree(Vector3b.TRUE);
			simpleSpacer.setPropertyFill(Vector3b.TRUE);
			simpleSpacer.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			sizerVertMain.subWidgetAdd(simpleSpacer);
		}
		
		{
			final Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.PINK);
			simpleSpacer.setPropertyExpand(Vector3b.TRUE);
			simpleSpacer.setPropertyFill(Vector3b.TRUE);
			simpleSpacer.setPropertyMinSize(new Dimension3f(new Vector3f(30, 30, 30), Distance.PIXEL));
			this.sizerMenuHori.subWidgetAdd(simpleSpacer);
		}
		{
			this.buttonExpandX = Button.createToggleLabelButton("Expand X", "Un-expand X");
			this.buttonExpandX.setPropertyExpand(Vector3b.FALSE_TRUE_FALSE);
			this.buttonExpandX.setPropertyFill(Vector3b.FALSE);
			this.buttonExpandX.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			this.buttonExpandX.setPropertyGravity(Gravity.CENTER);
			this.sizerMenuHori.subWidgetAdd(this.buttonExpandX);
			this.buttonExpandX.signalValue.connectAuto(this, BasicWindows::eventButtonExpandX);
		}
		{
			this.buttonExpandY = Button.createToggleLabelButton("Expand Y", "Un-expand Y");
			this.buttonExpandY.setPropertyExpand(Vector3b.FALSE_TRUE_FALSE);
			this.buttonExpandY.setPropertyFill(Vector3b.FALSE);
			this.buttonExpandY.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			this.buttonExpandY.setPropertyGravity(Gravity.CENTER);
			this.sizerMenuHori.subWidgetAdd(this.buttonExpandY);
			this.buttonExpandY.signalValue.connectAuto(this, BasicWindows::eventButtonExpandY);
		}
		{
			this.buttonFillX = Button.createToggleLabelButton("Fill X", "Un-fill X");
			this.buttonFillX.setPropertyExpand(Vector3b.FALSE_TRUE_FALSE);
			this.buttonFillX.setPropertyFill(Vector3b.FALSE);
			this.buttonFillX.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			this.buttonFillX.setPropertyGravity(Gravity.CENTER);
			this.sizerMenuHori.subWidgetAdd(this.buttonFillX);
			this.buttonFillX.signalValue.connectAuto(this, BasicWindows::eventButtonFillX);
		}
		{
			this.buttonFillY = Button.createToggleLabelButton("Fill Y", "Un-fill Y");
			this.buttonFillY.setPropertyExpand(Vector3b.FALSE_TRUE_FALSE);
			this.buttonFillY.setPropertyFill(Vector3b.FALSE);
			this.buttonFillY.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			this.buttonFillY.setPropertyGravity(Gravity.CENTER);
			this.sizerMenuHori.subWidgetAdd(this.buttonFillY);
			this.buttonFillY.signalValue.connectAuto(this, BasicWindows::eventButtonFillY);
		}
		{
			this.buttonGravity = Button.createLabelButton("Gravity");
			this.buttonGravity.setPropertyExpand(Vector3b.FALSE_TRUE_FALSE);
			this.buttonGravity.setPropertyFill(Vector3b.FALSE);
			this.buttonGravity.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			this.buttonGravity.setPropertyGravity(Gravity.CENTER);
			this.sizerMenuHori.subWidgetAdd(this.buttonGravity);
			this.buttonGravity.signalClick.connectAuto(this, BasicWindows::eventButtonChangeGravity);
			final Label gravLabel = (Label) (this.buttonGravity.getSubWidgets()[0]);
			gravLabel.setPropertyValue(LABEL_GRAVITY + Gravity.BOTTOM_LEFT);
		}
	}
	
	public void addButton(final Widget widget) {
		this.sizerMenuHori.subWidgetAdd(widget);
	}
	
	public void setTestWidget(final Widget widget) {
		this.sizerTestAreaHori.subWidgetRemoveAll();
		{
			final Spacer simpleSpacer = new Spacer();
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
			final Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.GREEN_YELLOW);
			simpleSpacer.setPropertyExpand(Vector3b.FALSE);
			simpleSpacer.setPropertyExpandIfFree(Vector3b.TRUE);
			simpleSpacer.setPropertyFill(Vector3b.TRUE);
			simpleSpacer.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			this.sizerTestAreaHori.subWidgetAdd(simpleSpacer);
		}
		// update properties...
		final Vector3b stateExpand = this.testWidget.getPropertyExpand();
		this.buttonExpandX.setPropertyValue(stateExpand.x());
		this.buttonExpandY.setPropertyValue(stateExpand.y());
		final Vector3b stateFill = this.testWidget.getPropertyFill();
		this.buttonFillX.setPropertyValue(stateFill.x());
		this.buttonFillY.setPropertyValue(stateFill.y());
		
		final Gravity gravity = this.testWidget.getPropertyGravity();
		final Label gravLabel = (Label) (this.buttonGravity.getSubWidgets()[0]);
		gravLabel.setPropertyValue(LABEL_GRAVITY + gravity.toString());
	}
}