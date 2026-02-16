package sample.atriasoft.ewol;

import org.atriasoft.ewol.widget.Widget;

public interface TestWidgetInterface {
	Widget getWidget();

	String getTitle();

	/**
	 * Returns the category for grouping in the landing page and menu bar.
	 * @return the category name
	 */
	default String getCategory() {
		return "Other";
	}

	/**
	 * Returns a short one-line description of the widget.
	 * @return the description
	 */
	default String getDescription() {
		return "";
	}

	/**
	 * Returns true if this test contains a meta widget (composite of multiple widgets).
	 * Meta widgets don't have editable properties in the property panel.
	 * @return true if this is a meta widget test
	 */
	default boolean isMetaWidget() {
		return false;
	}
}
