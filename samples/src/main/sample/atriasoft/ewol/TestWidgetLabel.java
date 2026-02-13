package sample.atriasoft.ewol;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Widget;

public class TestWidgetLabel implements TestWidgetInterface {

	@Override
	public Widget getWidget() {
		final var testWidget = new Label();
		testWidget.setPropertyValue(
				// Basic text + bold
				"He<b>llo.</b> "
				// Font color (foreground)
				+ "<font color='blue'>World</font> "
				// Font color (background)
				+ "<font colorBg='#FFFF00'>highlighted</font>"
				+ "<br/><br/>"
				// Italic
				+ "<i>Italic text</i> "
				// Bold + Italic combined
				+ "<b><i>Bold Italic</i></b> "
				// Underline
				+ "<u>Underlined</u>"
				+ "<br/><br/>"
				// Paragraph
				+ "<p>A paragraph block with automatic line return.</p>"
				// Left alignment
				+ "<left>Left aligned text</left>"
				+ "<br/>"
				// Center alignment
				+ "<center>Centered text</center>"
				+ "<br/>"
				// Right alignment
				+ "<right>Right aligned text</right>"
				+ "<br/>"
				// Justify alignment
				+ "<justify>Justified text that should spread across the full width of the label widget area.</justify>"
				+ "<br/><br/>"
				// Nested font colors
				+ "<font color='red'>Red <font color='green'>Green <b>Bold Green</b></font> back to Red</font>"
				+ "<br/>"
				// Alternative tag names
				+ "<bold>Alt bold</bold> <italic>Alt italic</italic>"
		);
		testWidget.setPropertyExpand(Vector2b.TRUE);
		testWidget.setPropertyFill(Vector2b.TRUE);
		return testWidget;
	}

	@Override
	public String getTitle() {
		return "Simple Label test";
	}

}
