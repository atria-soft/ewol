package org.atriasoft.ewol.widget;

import org.atriasoft.etk.Dimension3f;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.etk.math.Vector3f;

import sample.atriasoft.ewol.BasicWindows;

public class MainWindows extends BasicWindows {
	
	public MainWindows() {
		setPropertyTitle("Simple Entry test");
		
		final Entry simpleEntry = new Entry();
		simpleEntry.setPropertyExpand(Vector3b.TRUE);
		simpleEntry.setPropertyFill(Vector3b.TRUE_FALSE_FALSE);
		simpleEntry.setPropertyMinSize(new Dimension3f(new Vector3f(200, 15, 10), Distance.PIXEL));
		this.setTestWidget(simpleEntry);
		
	}
}
