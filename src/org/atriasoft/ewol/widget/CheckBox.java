package org.atriasoft.ewol.widget;

import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;

public class CheckBox extends Container {
	
	public CheckBox() {
		final Sizer subs = new Sizer(DisplayMode.modeHori);
		subs.setPropertyLockExpand(Vector3b.TRUE);
		subs.setPropertyGravity(Gravity.CENTER);
		setSubWidget(subs);
		
		final Tick tick = new Tick();
		tick.setPropertyExpand(new Vector3b(false, true, true));
		tick.setPropertyFill(Vector3b.FALSE);
		tick.setPropertyGravity(Gravity.CENTER);
		subs.subWidgetAdd(tick);
		
		final Label label = new Label("No Label");
		label.setPropertyExpand(Vector3b.TRUE);
		label.setPropertyFill(Vector3b.FALSE);
		label.setPropertyGravity(Gravity.LEFT);
		subs.subWidgetAdd(label);
		
	}
	
}
