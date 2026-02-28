package sample.atriasoft.ewol;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.CheckBox;
import org.atriasoft.ewol.widget.FieldSet;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Slider;
import org.atriasoft.ewol.widget.Widget;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * FieldSet with checkbox to enable/disable the section.
 */
public class TestWidgetFieldSetCheckable implements TestWidgetInterface {
	private static final Logger LOGGER = LoggerFactory.getLogger(TestWidgetFieldSetCheckable.class);

	@Override
	public Widget getWidget() {
		final FieldSet fieldSet = FieldSet.create("Rendering Options")
				.checkable(true)
				.add(CheckBox.create("Wireframe").expand(true, false))
				.add(Label.create("Normal Length:").expand(true, false))
				.add(Slider.create().range(0.1f, 2.0f).value(0.5f).expand(true, false).fill(true, false))
				.onActivate(() -> LOGGER.info("Section activated"))
				.onDeactivate(() -> LOGGER.info("Section deactivated"));
		fieldSet.setPropertyExpand(Vector2b.TRUE_FALSE);
		fieldSet.setPropertyFill(Vector2b.TRUE_FALSE);
		return fieldSet;
	}

	@Override
	public String getTitle() {
		return "FieldSet Checkable";
	}

	@Override
	public String getDescription() {
		return "FieldSet with checkbox to show/hide content";
	}

	@Override
	public String getCategory() {
		return "Basic";
	}
}
