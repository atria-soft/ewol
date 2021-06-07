package sample.atriasoft.ewol.sampleButton;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;
import org.atriasoft.ewol.widget.Windows;

public class MainWindows extends Windows {
	
	public MainWindows() {
		setPropertyTitle("Simple Button test");
		//EwolObject.getContext().getFontDefault().setName("FreeSans");

		Sizer sizerMain = new Sizer(DisplayMode.modeVert);
		sizerMain.setPropertyExpand(new Vector2b(true, true));
		sizerMain.setPropertyFill(new Vector2b(true, true));
		setSubWidget(sizerMain);

		Button simpleButton = new Button();
		simpleButton.setPropertyValue("Top Button");
		simpleButton.setPropertyExpand(new Vector2b(true, true));
		simpleButton.setPropertyFill(new Vector2b(true, false));
		sizerMain.subWidgetAdd(simpleButton);
		
		Button simpleButton2 = new Button();
		simpleButton2.setPropertyValue("Botom Button");
		simpleButton2.setPropertyExpand(new Vector2b(true, true));
		simpleButton2.setPropertyFill(new Vector2b(false, true));
		sizerMain.subWidgetAdd(simpleButton2);
	}
}
