package sample.atriasoft.ewol;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.Ewol;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.ewol.widget.notification.ToastType;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Test widget for Toast notifications.
 * Demonstrates the three toast types, stacking, timeout, and configuration.
 */
public class TestWidgetToast implements TestWidgetInterface {
	private static final Logger LOGGER = LoggerFactory.getLogger(TestWidgetToast.class);

	@Override
	public Widget getWidget() {
		final var mainSizer = new Sizer(DisplayMode.VERTICAL);
		mainSizer.setPropertyExpand(Vector2b.TRUE);
		mainSizer.setPropertyFill(Vector2b.TRUE);

		// Title
		final var titleLabel = new Label("<b>Toast Notification Demo</b>");
		titleLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
		titleLabel.setPropertyFill(Vector2b.TRUE_FALSE);
		mainSizer.subWidgetAdd(titleLabel);

		// Row 1: Basic toast types
		final var row1Label = new Label("Basic types:");
		row1Label.setPropertyExpand(Vector2b.TRUE_FALSE);
		mainSizer.subWidgetAdd(row1Label);

		final var row1 = new Sizer(DisplayMode.HORIZONTAL);
		row1.setPropertyExpand(Vector2b.TRUE_FALSE);
		row1.setPropertyFill(Vector2b.TRUE);
		mainSizer.subWidgetAdd(row1);

		row1.subWidgetAdd(Button.create("Info Toast").onClick(() -> {
			LOGGER.info(">>> Info Toast button CLICKED");
			final var windows = Ewol.getContext().getWindows();
			if (windows != null) {
				windows.getNotification().info("Information", "This is an info notification.");
			}
		}));

		row1.subWidgetAdd(Button.create("Warning Toast").onClick(() -> {
			LOGGER.info(">>> Warning Toast button CLICKED");
			final var windows = Ewol.getContext().getWindows();
			if (windows != null) {
				windows.getNotification().warning("Warning", "Something needs your attention.");
			}
		}));

		row1.subWidgetAdd(Button.create("Error Toast").onClick(() -> {
			LOGGER.info(">>> Error Toast button CLICKED");
			final var windows = Ewol.getContext().getWindows();
			if (windows != null) {
				windows.getNotification().error("Error", "Something went wrong!");
			}
		}));

		// Row 2: Stacking demo
		final var row2Label = new Label("Stacking (multiple toasts):");
		row2Label.setPropertyExpand(Vector2b.TRUE_FALSE);
		mainSizer.subWidgetAdd(row2Label);

		final var row2 = new Sizer(DisplayMode.HORIZONTAL);
		row2.setPropertyExpand(Vector2b.TRUE_FALSE);
		row2.setPropertyFill(Vector2b.TRUE);
		mainSizer.subWidgetAdd(row2);

		row2.subWidgetAdd(Button.create("3 Toasts").onClick(() -> {
			final var windows = Ewol.getContext().getWindows();
			if (windows != null) {
				final var nm = windows.getNotification();
				nm.info("Step 1", "First notification in the stack.");
				nm.warning("Step 2", "Second notification, stacked above.");
				nm.error("Step 3", "Third notification, on top of all.");
			}
		}));

		row2.subWidgetAdd(Button.create("Dismiss All").onClick(() -> {
			final var windows = Ewol.getContext().getWindows();
			if (windows != null) {
				windows.getNotification().dismissAll();
			}
		}));

		// Row 3: Position demo
		final var row3Label = new Label("Positions:");
		row3Label.setPropertyExpand(Vector2b.TRUE_FALSE);
		mainSizer.subWidgetAdd(row3Label);

		final var row3 = new Sizer(DisplayMode.HORIZONTAL);
		row3.setPropertyExpand(Vector2b.TRUE_FALSE);
		row3.setPropertyFill(Vector2b.TRUE);
		mainSizer.subWidgetAdd(row3);

		row3.subWidgetAdd(Button.create("Bottom-Left").onClick(() -> {
			final var windows = Ewol.getContext().getWindows();
			if (windows != null) {
				windows.getNotification().getConfig().position(Gravity.BOTTOM_LEFT);
				windows.getNotification().info("Bottom-Left", "Position changed to bottom-left.");
			}
		}));

		row3.subWidgetAdd(Button.create("Bottom-Center").onClick(() -> {
			final var windows = Ewol.getContext().getWindows();
			if (windows != null) {
				windows.getNotification().getConfig().position(Gravity.BOTTOM);
				windows.getNotification().info("Bottom-Center", "Position changed to bottom-center.");
			}
		}));

		row3.subWidgetAdd(Button.create("Bottom-Right").onClick(() -> {
			final var windows = Ewol.getContext().getWindows();
			if (windows != null) {
				windows.getNotification().getConfig().position(Gravity.BOTTOM_RIGHT);
				windows.getNotification().info("Bottom-Right", "Position changed to bottom-right.");
			}
		}));

		final var row3b = new Sizer(DisplayMode.HORIZONTAL);
		row3b.setPropertyExpand(Vector2b.TRUE_FALSE);
		row3b.setPropertyFill(Vector2b.TRUE);
		mainSizer.subWidgetAdd(row3b);

		row3b.subWidgetAdd(Button.create("Top-Left").onClick(() -> {
			final var windows = Ewol.getContext().getWindows();
			if (windows != null) {
				windows.getNotification().getConfig().position(Gravity.TOP_LEFT);
				windows.getNotification().info("Top-Left", "Position changed to top-left.");
			}
		}));

		row3b.subWidgetAdd(Button.create("Top-Center").onClick(() -> {
			final var windows = Ewol.getContext().getWindows();
			if (windows != null) {
				windows.getNotification().getConfig().position(Gravity.TOP);
				windows.getNotification().info("Top-Center", "Position changed to top-center.");
			}
		}));

		row3b.subWidgetAdd(Button.create("Top-Right").onClick(() -> {
			final var windows = Ewol.getContext().getWindows();
			if (windows != null) {
				windows.getNotification().getConfig().position(Gravity.TOP_RIGHT);
				windows.getNotification().info("Top-Right", "Position changed to top-right.");
			}
		}));

		// Row 4: Custom timeout demo
		final var row4Label = new Label("Timeout override:");
		row4Label.setPropertyExpand(Vector2b.TRUE_FALSE);
		mainSizer.subWidgetAdd(row4Label);

		final var row4 = new Sizer(DisplayMode.HORIZONTAL);
		row4.setPropertyExpand(Vector2b.TRUE_FALSE);
		row4.setPropertyFill(Vector2b.TRUE);
		mainSizer.subWidgetAdd(row4);

		row4.subWidgetAdd(Button.create("1s Timeout").onClick(() -> {
			final var windows = Ewol.getContext().getWindows();
			if (windows != null) {
				windows.getNotification()
						.showToast(ToastType.INFO, "Quick", "Disappears in 1 second.")
						.overrideTimeout(1.0f);
			}
		}));

		row4.subWidgetAdd(Button.create("10s Timeout").onClick(() -> {
			final var windows = Ewol.getContext().getWindows();
			if (windows != null) {
				windows.getNotification()
						.showToast(ToastType.WARNING, "Persistent", "Stays for 10 seconds.")
						.overrideTimeout(10.0f);
			}
		}));

		row4.subWidgetAdd(Button.create("No Timeout").onClick(() -> {
			final var windows = Ewol.getContext().getWindows();
			if (windows != null) {
				windows.getNotification()
						.showToast(ToastType.ERROR, "Manual Close", "Only closes via the X button.")
						.overrideTimeout(0f);
			}
		}));

		// Row 5: Config demo
		final var row5Label = new Label("Width config:");
		row5Label.setPropertyExpand(Vector2b.TRUE_FALSE);
		mainSizer.subWidgetAdd(row5Label);

		final var row5 = new Sizer(DisplayMode.HORIZONTAL);
		row5.setPropertyExpand(Vector2b.TRUE_FALSE);
		row5.setPropertyFill(Vector2b.TRUE);
		mainSizer.subWidgetAdd(row5);

		row5.subWidgetAdd(Button.create("Narrow (250px)").onClick(() -> {
			final var windows = Ewol.getContext().getWindows();
			if (windows != null) {
				windows.getNotification().getConfig().width(250f);
				windows.getNotification().info("Narrow", "Width set to 250px.");
			}
		}));

		row5.subWidgetAdd(Button.create("Default (350px)").onClick(() -> {
			final var windows = Ewol.getContext().getWindows();
			if (windows != null) {
				windows.getNotification().getConfig().width(350f);
				windows.getNotification().info("Default", "Width set to 350px.");
			}
		}));

		row5.subWidgetAdd(Button.create("Wide (500px)").onClick(() -> {
			final var windows = Ewol.getContext().getWindows();
			if (windows != null) {
				windows.getNotification().getConfig().width(500f);
				windows.getNotification().info("Wide", "Width set to 500px. This toast has more room for longer descriptions.");
			}
		}));

		return mainSizer;
	}

	@Override
	public String getTitle() {
		return "Toast Notifications";
	}

	@Override
	public String getDescription() {
		return "Temporary notification messages";
	}

	@Override
	public String getCategory() {
		return "Overlay";
	}

	@Override
	public boolean isMetaWidget() {
		return true;
	}
}
