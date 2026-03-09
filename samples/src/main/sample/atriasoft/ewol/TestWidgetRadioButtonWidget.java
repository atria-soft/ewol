package sample.atriasoft.ewol;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.widget.Icon;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.RadioButton;
import org.atriasoft.ewol.widget.RadioGroup;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Widget;

public class TestWidgetRadioButtonWidget implements TestWidgetInterface {

	@Override
	public Widget getWidget() {
		final RadioGroup group = RadioGroup.vertical();
		group.setPropertyExpand(Vector2b.TRUE);
		group.setPropertyFill(Vector2b.TRUE);

		// Option 1: Label with colored text
		final Label coloredLabel = new Label("<font color='#E91E63'>Pink option</font> with <b>formatted</b> text");
		coloredLabel.setPropertyGravity(Gravity.LEFT);
		group.addRadio(new RadioButton(coloredLabel));

		// Option 2: Horizontal sizer with icon + label
		final Sizer iconRow = Sizer.horizontal();
		final Icon icon = Icon.create("home");
		icon.setPropertyExpand(Vector2b.FALSE);
		icon.setPropertyFill(Vector2b.FALSE);
		icon.setPropertyGravity(Gravity.CENTER);
		iconRow.subWidgetAdd(icon);
		final Label iconLabel = new Label("Home option");
		iconLabel.setPropertyExpand(Vector2b.TRUE);
		iconLabel.setPropertyFill(Vector2b.FALSE);
		iconLabel.setPropertyGravity(Gravity.LEFT);
		iconRow.subWidgetAdd(iconLabel);
		group.addRadio(new RadioButton(iconRow));

		// Option 3: Multi-line label
		final Label multiLine = new Label("Multi-line option<br/><font color='#808080'>with a description below</font>");
		multiLine.setPropertyGravity(Gravity.LEFT);
		group.addRadio(new RadioButton(multiLine));

		group.selectedIndex(0);
		return group;
	}

	@Override
	public String getTitle() {
		return "Radio Button (Widget)";
	}

	@Override
	public String getDescription() {
		return "Radio buttons with custom widget content";
	}

	@Override
	public String getCategory() {
		return "Basic";
	}
}
