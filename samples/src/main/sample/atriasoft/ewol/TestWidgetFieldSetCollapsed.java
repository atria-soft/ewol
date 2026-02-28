package sample.atriasoft.ewol;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.FieldSet;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Slider;
import org.atriasoft.ewol.widget.Widget;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * FieldSet with checkbox initially unchecked (content hidden by default).
 */
public class TestWidgetFieldSetCollapsed implements TestWidgetInterface {
	private static final Logger LOGGER = LoggerFactory.getLogger(TestWidgetFieldSetCollapsed.class);

	@Override
	public Widget getWidget() {
		final FieldSet fieldSet = FieldSet.create("Advanced Options")
				.checkable(true)
				.checked(false)
				.add(Label.create("These options are hidden by default.").expand(true, false))
				.add(Slider.create().range(0, 100).value(50).expand(true, false).fill(true, false))
				.onActivate(() -> LOGGER.info("Advanced section opened"))
				.onDeactivate(() -> LOGGER.info("Advanced section closed"));
		fieldSet.setPropertyExpand(Vector2b.TRUE_FALSE);
		fieldSet.setPropertyFill(Vector2b.TRUE_FALSE);
		return fieldSet;
	}

	@Override
	public String getTitle() {
		return "FieldSet Collapsed";
	}

	@Override
	public String getDescription() {
		return "FieldSet initially collapsed (unchecked)";
	}

	@Override
	public String getCategory() {
		return "Basic";
	}
}
