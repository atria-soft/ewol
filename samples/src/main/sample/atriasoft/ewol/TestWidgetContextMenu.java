package sample.atriasoft.ewol;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.Ewol;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.ewol.widget.menu.MenuPopup;
import org.atriasoft.gale.key.KeyStatus;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Test widget demonstrating context menus:
 * - Programmatic context menu via button
 * - Right-click context menu
 * - Mixed enabled/disabled items
 */
public class TestWidgetContextMenu implements TestWidgetInterface {
	private static final Logger LOGGER = LoggerFactory.getLogger(TestWidgetContextMenu.class);

	@Override
	public Widget getWidget() {
		final Sizer mainSizer = new Sizer(DisplayMode.VERTICAL);
		mainSizer.setPropertyExpand(Vector2b.TRUE);
		mainSizer.setPropertyFill(Vector2b.TRUE);

		// Title
		final Label titleLabel = new Label("<b>Context Menu Demo</b>");
		titleLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
		titleLabel.setPropertyFill(Vector2b.TRUE_FALSE);
		mainSizer.subWidgetAdd(titleLabel);

		// ============================================================
		// Section 1: Context Menu via button
		// ============================================================
		final Label contextLabel = new Label("Context Menu (via button click):");
		contextLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
		mainSizer.subWidgetAdd(contextLabel);

		final Sizer row1 = new Sizer(DisplayMode.HORIZONTAL);
		row1.setPropertyExpand(Vector2b.TRUE_FALSE);
		row1.setPropertyFill(Vector2b.TRUE);
		mainSizer.subWidgetAdd(row1);

		row1.subWidgetAdd(Button.create("Show Context Menu").onClick(() -> {
			LOGGER.info(">>> Context Menu button clicked");
			final org.atriasoft.ewol.widget.Windows windows = Ewol.getContext().getWindows();
			if (windows != null) {
				MenuPopup.create()
						.item("Cut", null, "ctrl+x", () -> LOGGER.info(">>> Context > Cut"))
						.item("Copy", null, "ctrl+c", () -> LOGGER.info(">>> Context > Copy"))
						.item("Paste", null, "ctrl+v", () -> LOGGER.info(">>> Context > Paste"))
						.separator()
						.disabledItem("Undo")
						.item("Select All", null, "ctrl+a", () -> LOGGER.info(">>> Context > Select All"))
						.anchorAt(new Vector2f(300, 300))
						.show(windows);
			}
		}));

		// ============================================================
		// Section 2: Right-click area
		// ============================================================
		final Label rightClickLabel = new Label("Right-click area (right-click on the area below):");
		rightClickLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
		mainSizer.subWidgetAdd(rightClickLabel);

		final Widget rightClickArea = new Widget() {
			@Override
			public boolean onEventInput(final EventInput event) {
				// Right-click = inputId 3 on ewol
				if (event.inputId() == 3 && event.status() == KeyStatus.pressSingle) {
					LOGGER.info(">>> Right-click detected at {}", event.pos());
					final org.atriasoft.ewol.widget.Windows windows = Ewol.getContext().getWindows();
					if (windows != null) {
						MenuPopup.create()
								.item("Inspect Element", "search", null, () -> LOGGER.info(">>> Inspect"))
								.item("View Source", "file", null, () -> LOGGER.info(">>> View Source"))
								.separator()
								.item("Refresh", null, "F5", () -> LOGGER.info(">>> Refresh"))
								.item("Reload", null, "ctrl+shift+r", () -> LOGGER.info(">>> Reload"))
								.separator()
								.disabledItem("Save As...", "save")
								.item("Print", null, "ctrl+p", () -> LOGGER.info(">>> Print"))
								.anchorAt(event.pos())
								.show(windows);
					}
					return true;
				}
				return false;
			}
		};
		rightClickArea.setMouseLimit(3);
		rightClickArea.setPropertyExpand(Vector2b.TRUE);
		rightClickArea.setPropertyFill(Vector2b.TRUE);
		mainSizer.subWidgetAdd(rightClickArea);

		// ============================================================
		// Section 3: Disabled items demo
		// ============================================================
		final Label disabledLabel = new Label("Disabled items demo:");
		disabledLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
		mainSizer.subWidgetAdd(disabledLabel);

		final Sizer row2 = new Sizer(DisplayMode.HORIZONTAL);
		row2.setPropertyExpand(Vector2b.TRUE_FALSE);
		row2.setPropertyFill(Vector2b.TRUE);
		mainSizer.subWidgetAdd(row2);

		row2.subWidgetAdd(Button.create("Mixed Enabled/Disabled").onClick(() -> {
			final org.atriasoft.ewol.widget.Windows windows = Ewol.getContext().getWindows();
			if (windows != null) {
				MenuPopup.create()
						.item("Enabled Item 1", "edit", () -> LOGGER.info(">>> Item 1"))
						.disabledItem("Disabled Item", "settings")
						.item("Enabled Item 2", "home", () -> LOGGER.info(">>> Item 2"))
						.separator()
						.disabledItem("Cannot Delete", "delete")
						.disabledItem("Cannot Rename", "pencil")
						.separator()
						.item("OK", "check", () -> LOGGER.info(">>> OK"))
						.anchorAt(new Vector2f(400, 250))
						.show(windows);
			}
		}));

		return mainSizer;
	}

	@Override
	public String getTitle() {
		return "Context Menu";
	}

	@Override
	public boolean isMetaWidget() {
		return true;
	}
}
