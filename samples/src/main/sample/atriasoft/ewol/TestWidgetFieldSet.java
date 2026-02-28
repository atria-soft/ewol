package sample.atriasoft.ewol;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.CheckBox;
import org.atriasoft.ewol.widget.FieldSet;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Widget;

/**
 * Simple FieldSet with title only (no checkbox, no icon).
 */
public class TestWidgetFieldSet implements TestWidgetInterface {

	@Override
	public Widget getWidget() {
		final FieldSet fieldSet = FieldSet.create("General Settings")
				.add(Label.create("Option A").expand(true, false))
				.add(CheckBox.create("Enable feature").expand(true, false));
		fieldSet.setPropertyExpand(Vector2b.TRUE_FALSE);
		fieldSet.setPropertyFill(Vector2b.TRUE_FALSE);
		return fieldSet;
	}

	@Override
	public String getTitle() {
		return "FieldSet";
	}

	@Override
	public String getDescription() {
		return "Grouping container with border and legend";
	}

	@Override
	public String getCategory() {
		return "Basic";
	}
}
