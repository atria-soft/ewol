package sample.atriasoft.ewol.sampleEntry;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.Entry;

import sample.atriasoft.ewol.BasicWindows;

public class MainWindows extends BasicWindows {
	
	public MainWindows() {
		setPropertyTitle("Simple Entry test");
		
		Entry simpleEntry = new Entry();
		simpleEntry.setPropertyExpand(Vector2b.TRUE_TRUE);
		simpleEntry.setPropertyFill(Vector2b.TRUE_FALSE);
		this.setTestWidget(simpleEntry);
		
	}
}
