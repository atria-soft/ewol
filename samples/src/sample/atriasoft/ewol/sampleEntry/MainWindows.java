package sample.atriasoft.ewol.sampleEntry;

import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.ewol.widget.Entry;

import sample.atriasoft.ewol.BasicWindows;

public class MainWindows extends BasicWindows {
	
	public MainWindows() {
		setPropertyTitle("Simple Entry test");
		
		Entry simpleEntry = new Entry();
		simpleEntry.setPropertyExpand(Vector3b.TRUE);
		simpleEntry.setPropertyFill(Vector3b.TRUE_FALSE_FALSE);
		this.setTestWidget(simpleEntry);
		
	}
}
