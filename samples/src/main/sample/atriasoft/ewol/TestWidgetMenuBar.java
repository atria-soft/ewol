package sample.atriasoft.ewol;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.ewol.widget.menu.MenuBar;
import org.atriasoft.ewol.widget.menu.MenuPopup;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Test widget demonstrating the MenuBar with dropdown menus.
 */
public class TestWidgetMenuBar implements TestWidgetInterface {
	private static final Logger LOGGER = LoggerFactory.getLogger(TestWidgetMenuBar.class);

	@Override
	public Widget getWidget() {
		final Sizer mainSizer = new Sizer(DisplayMode.VERTICAL);
		mainSizer.setPropertyExpand(Vector2b.TRUE);
		mainSizer.setPropertyFill(Vector2b.TRUE);

		// Title
		final Label titleLabel = new Label("<b>MenuBar Demo</b>");
		titleLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
		titleLabel.setPropertyFill(Vector2b.TRUE_FALSE);
		mainSizer.subWidgetAdd(titleLabel);

		// Description
		final Label descLabel = new Label("Click menu entries to open dropdowns.<br/>Hover between entries while a menu is open to switch.<br/>Press Escape to close.");
		descLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
		mainSizer.subWidgetAdd(descLabel);

		// MenuBar
		final MenuBar menuBar = MenuBar.create()
				.menu("File", () -> MenuPopup.create()
						.item("New", "add", "ctrl+n", () -> LOGGER.info(">>> File > New"))
						.item("Open", "folder", "ctrl+o", () -> LOGGER.info(">>> File > Open"))
						.item("Save", "save", "ctrl+s", () -> LOGGER.info(">>> File > Save"))
						.item("Save As...", null, "ctrl+shift+s", () -> LOGGER.info(">>> File > Save As"))
						.separator()
						.item("Export PDF", null, null, () -> LOGGER.info(">>> File > Export PDF"))
						.separator()
						.disabledItem("Recent Files")
						.separator()
						.item("Quit", "close", "alt+F4", () -> LOGGER.info(">>> File > Quit")))
				.menu("Edit", () -> MenuPopup.create()
						.item("Undo", null, "ctrl+z", () -> LOGGER.info(">>> Edit > Undo"))
						.item("Redo", null, "ctrl+shift+z", () -> LOGGER.info(">>> Edit > Redo"))
						.separator()
						.item("Cut", null, "ctrl+x", () -> LOGGER.info(">>> Edit > Cut"))
						.item("Copy", null, "ctrl+c", () -> LOGGER.info(">>> Edit > Copy"))
						.item("Paste", null, "ctrl+v", () -> LOGGER.info(">>> Edit > Paste"))
						.separator()
						.item("Select All", "check", "ctrl+a", () -> LOGGER.info(">>> Edit > Select All")))
				.menu("View", () -> MenuPopup.create()
						.item("Zoom In", null, "ctrl++", () -> LOGGER.info(">>> View > Zoom In"))
						.item("Zoom Out", null, "ctrl+-", () -> LOGGER.info(">>> View > Zoom Out"))
						.item("Reset Zoom", null, "ctrl+0", () -> LOGGER.info(">>> View > Reset Zoom"))
						.separator()
						.disabledItem("Full Screen")
						.disabledItem("Status Bar"))
				.menu("Help", () -> MenuPopup.create()
						.item("About", null, null, () -> LOGGER.info(">>> Help > About"))
						.item("Documentation", null, "F1", () -> LOGGER.info(">>> Help > Documentation")));
		mainSizer.subWidgetAdd(menuBar);

		// Spacer to fill remaining area
		final Widget spacer = new Widget();
		spacer.setPropertyExpand(Vector2b.TRUE);
		spacer.setPropertyFill(Vector2b.TRUE);
		mainSizer.subWidgetAdd(spacer);

		return mainSizer;
	}

	@Override
	public String getTitle() {
		return "MenuBar";
	}

	@Override
	public boolean isMetaWidget() {
		return true;
	}
}
