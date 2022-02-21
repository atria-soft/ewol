package sample.atriasoft.ewol.ComplexWindiows1;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;
import org.atriasoft.ewol.widget.Spacer;
import org.atriasoft.ewol.widget.Windows;

public class MainWindows extends Windows {
	
	public MainWindows() {
		//! [ewol_sample_HW_windows_title]
		setPropertyTitle("Simple sample test");
		//EwolObject.getContext().getFontDefault().setName("FreeSans");
		Sizer sizerMain = new Sizer(DisplayMode.modeVert);
		sizerMain.setPropertyExpand(Vector2b.TRUE_TRUE);
		sizerMain.setPropertyFill(Vector2b.TRUE_TRUE);
		setSubWidget(sizerMain);
		
		Sizer sizerHori1 = new Sizer(DisplayMode.modeHori);
		sizerHori1.setPropertyExpand(Vector2b.TRUE_TRUE);
		sizerHori1.setPropertyFill(Vector2b.TRUE_TRUE);
		sizerMain.subWidgetAdd(sizerHori1);
		
		Sizer sizerHori2 = new Sizer(DisplayMode.modeHori);
		sizerHori2.setPropertyExpand(Vector2b.TRUE_TRUE);
		sizerHori2.setPropertyFill(Vector2b.TRUE_TRUE);
		sizerMain.subWidgetAdd(sizerHori2);
		
		{
			Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyMinSize(new Dimension(new Vector2f(100, 100), Distance.PIXEL));
			simpleSpacer.setPropertyColor(Color.ALICE_BLUE);
			simpleSpacer.setPropertyExpand(Vector2b.TRUE_TRUE);
			simpleSpacer.setPropertyFill(Vector2b.TRUE_TRUE);
			sizerHori1.subWidgetAdd(simpleSpacer);
		}
		{
			Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.DARK_GREEN);
			simpleSpacer.setPropertyExpand(Vector2b.TRUE_TRUE);
			simpleSpacer.setPropertyFill(Vector2b.TRUE_TRUE);
			sizerHori1.subWidgetAdd(simpleSpacer);
		}
		{
			Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.CHOCOLATE);
			simpleSpacer.setPropertyExpand(Vector2b.TRUE_TRUE);
			simpleSpacer.setPropertyFill(Vector2b.TRUE_TRUE);
			sizerHori1.subWidgetAdd(simpleSpacer);
		}
		{
			Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.GREEN_YELLOW);
			simpleSpacer.setPropertyExpand(Vector2b.TRUE_TRUE);
			simpleSpacer.setPropertyFill(Vector2b.TRUE_TRUE);
			sizerHori2.subWidgetAdd(simpleSpacer);
		}
		{
			Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.PINK);
			simpleSpacer.setPropertyExpand(Vector2b.TRUE_TRUE);
			simpleSpacer.setPropertyFill(Vector2b.TRUE_TRUE);
			sizerHori2.subWidgetAdd(simpleSpacer);
		}
	}
}
