package sample.atriasoft.ewol.sampleEntry;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.Entry;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;
import org.atriasoft.ewol.widget.Windows;

public class MainWindows extends Windows {
	
	public MainWindows() {
		setPropertyTitle("Simple Entry test");
		//EwolObject.getContext().getFontDefault().setName("FreeSans");

		Sizer sizerMain = new Sizer(DisplayMode.modeVert);
		sizerMain.setPropertyExpand(new Vector2b(true, true));
		sizerMain.setPropertyFill(new Vector2b(true, true));
		setSubWidget(sizerMain);

		Entry simpleEntry = new Entry();
		simpleEntry.setPropertyExpand(new Vector2b(true, true));
		simpleEntry.setPropertyFill(new Vector2b(true, false));
		sizerMain.subWidgetAdd(simpleEntry);
		
		Entry simpleEntry2 = new Entry();
		simpleEntry2.setPropertyExpand(new Vector2b(true, true));
		simpleEntry2.setPropertyFill(new Vector2b(true, false));
		sizerMain.subWidgetAdd(simpleEntry2);
	}
}
