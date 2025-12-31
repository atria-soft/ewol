package sample.atriasoft.ewol;

import org.atriasoft.ewol.widget.Widget;

public interface TestWidgetInterface {
	Widget getWidget();

	String getTitle();

	/**
	 * Returns true if this test contains a meta widget (composite of multiple widgets).
	 * Meta widgets don't have editable properties in the property panel.
	 * @return true if this is a meta widget test
	 */
	default boolean isMetaWidget() {
		return false;
	}
}
