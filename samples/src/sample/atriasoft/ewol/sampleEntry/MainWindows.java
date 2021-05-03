package sample.atriasoft.ewol.sampleEntry;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.Entry;
import org.atriasoft.ewol.widget.Windows;

public class MainWindows extends Windows {
	
	public MainWindows() {
		setPropertyTitle("Simple sample test");
		//EwolObject.getContext().getFontDefault().setName("FreeSans");
		Entry simpleEntry = new Entry();
		simpleEntry.setPropertyExpand(new Vector2b(true, true));
		simpleEntry.setPropertyFill(new Vector2b(true, false));
		setSubWidget(simpleEntry);
	}
}
