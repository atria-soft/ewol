package sample.atriasoft.ewol;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.Container;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.ScrollView;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;
import org.atriasoft.ewol.widget.SplitPane;
import org.atriasoft.ewol.widget.Windows;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class BasicWindows extends Windows {
	private static final Logger LOGGER = LoggerFactory.getLogger(BasicWindows.class);
	private static final int MAX_LOG_LINES = 100;

	private int index = -1;
	private final List<TestWidgetInterface> testedElement = new ArrayList<>();
	private Container container = null;
	private Label title = null;
	private ModelWidget currentModelWidget = null;

	// Log panel components
	private Sizer logContent;
	private ScrollView logScrollView;
	private final List<String> logLines = new ArrayList<>();
	
	public static void staticRequestNext(final BasicWindows self) {
		self.requestNext();
	}
	
	public static void staticRequestPrevious(final BasicWindows self) {
		self.requestPrevious();
	}
	
	public void requestNext() {
		LOGGER.info("Request Next");
		this.index++;
		if (this.index >= this.testedElement.size()) {
			this.index = 0;
		}
		updateDisplay();
	}
	
	public void requestPrevious() {
		LOGGER.info("Request Previous");
		this.index--;
		if (this.index < 0) {
			this.index = this.testedElement.size() - 1;
		}
		updateDisplay();
	}
	
	public void updateDisplay() {
		final var test = this.testedElement.get(this.index);
		final var titlegenerated = "<b>[" + (this.index + 1) + "/" + this.testedElement.size() + "] " + test.getTitle()
				+ "</b>";
		this.title.setPropertyValue(titlegenerated);
		setPropertyTitle(titlegenerated);
		// Clear log on test change
		clearLog();
		// Create new ModelWidget and set log callback
		this.currentModelWidget = new ModelWidget(test, this::addLogEntry);
		this.container.setSubWidget(this.currentModelWidget);
	}
	
	public BasicWindows() {

		final var sizerMain = new Sizer(DisplayMode.VERTICAL);
		sizerMain.setPropertyExpand(Vector2b.TRUE);
		sizerMain.setPropertyFill(Vector2b.TRUE);
		setSubWidget(sizerMain);

		// Navigation menu at top
		final var menu = new Sizer(DisplayMode.HORIZONTAL);
		menu.setPropertyExpand(Vector2b.TRUE_FALSE);
		menu.setPropertyExpandIfFree(Vector2b.TRUE_FALSE);
		menu.setPropertyFill(Vector2b.TRUE);
		menu.setPropertyLockExpand(Vector2b.TRUE);
		menu.setPropertyMaxSize(new Dimension2f(new Vector2f(9999, 3), Distance.CENTIMETER));
		sizerMain.subWidgetAdd(menu);

		final var next = Button.createLabelButton("&lt;&lt; Previous");
		next.setPropertyMaxSize(new Dimension2f(new Vector2f(9999, 2), Distance.CENTIMETER));
		menu.subWidgetAdd(next);
		next.signalClick.connectAuto(this, BasicWindows::staticRequestNext);

		this.title = new Label("unknown");
		this.title.setPropertyFill(Vector2b.FALSE);
		this.title.setPropertyExpand(Vector2b.TRUE);
		menu.subWidgetAdd(this.title);

		final var previous = Button.createLabelButton("Next &gt;&gt;");
		previous.setPropertyMaxSize(new Dimension2f(new Vector2f(9999, 2), Distance.CENTIMETER));
		menu.subWidgetAdd(previous);
		previous.signalClick.connectAuto(this, BasicWindows::staticRequestPrevious);

		// SplitPane to separate test widget area from log panel
		final var splitPane = SplitPane.vertical().splitPosition(0.25f) // 75% for test widget, 25% for log
				.minSizes(100.0f, 50.0f);
		splitPane.setPropertyExpand(Vector2b.TRUE);
		splitPane.setPropertyFill(Vector2b.TRUE);
		sizerMain.subWidgetAdd(splitPane);

		// Test widget container (first part of split pane)
		this.container = new Container();
		this.container.setPropertyExpand(Vector2b.TRUE);
		this.container.setPropertyFill(Vector2b.TRUE);
		this.container.setPropertyExpandIfFree(Vector2b.TRUE);
		splitPane.second(this.container);

		// Log panel (second part of split pane)
		final var logPanel = createLogPanel();
		splitPane.first(logPanel);

		this.testedElement.add(new TestWidgetIcon());
		this.testedElement.add(new TestWidgetSelect());
		this.testedElement.add(new TestWidgetSlider());
		this.testedElement.add(new TestWidgetSliderShowcase());
		this.testedElement.add(new TestWidgetFileChooser());
		this.testedElement.add(new TestWidgetColorPicker());
		this.testedElement.add(new TestWidgetListFileSystem());
		this.testedElement.add(new TestWidgetScrollView());
		this.testedElement.add(new TestWidgetSplitPane());
		this.testedElement.add(new TestWidgetEntry());
		this.testedElement.add(new TestWidgetBox());
		this.testedElement.add(new TestWidgetButton());
		this.testedElement.add(new TestWidgetButtonToggle());
		this.testedElement.add(new TestWidgetCheckBox());
		this.testedElement.add(new TestWidgetImage());
		this.testedElement.add(new TestWidgetLabel());
		this.testedElement.add(new TestWidgetToast());
		requestNext();
	}

	/**
	 * Creates the log panel for the split pane.
	 * @return the log panel widget
	 */
	private Sizer createLogPanel() {
		final var logPanel = new Sizer(DisplayMode.VERTICAL);
		logPanel.setPropertyExpand(Vector2b.TRUE);
		logPanel.setPropertyFill(Vector2b.TRUE);

		// Header with title and clear button
		final var header = new Sizer(DisplayMode.HORIZONTAL);
		header.setPropertyExpand(Vector2b.TRUE_FALSE);
		header.setPropertyFill(Vector2b.TRUE);
		logPanel.subWidgetAdd(header);

		final var titleLabel = new Label("<b>Signal Events Log:</b>");
		titleLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
		titleLabel.setPropertyFill(Vector2b.TRUE);
		titleLabel.setPropertyGravity(Gravity.LEFT);
		header.subWidgetAdd(titleLabel);

		final var clearButton = Button.createLabelButton("Clear");
		clearButton.setPropertyExpand(Vector2b.FALSE);
		header.subWidgetAdd(clearButton);
		clearButton.signalClick.connectAuto(this, BasicWindows::staticClearLog);

		// Log content area
		this.logContent = new Sizer(DisplayMode.VERTICAL);
		this.logContent.setPropertyExpand(Vector2b.TRUE_FALSE);
		this.logContent.setPropertyFill(Vector2b.TRUE);
		this.logContent.setPropertyGravity(Gravity.TOP_LEFT);

		this.logScrollView = ScrollView.create().content(this.logContent).showVertical(true).showHorizontal(false);
		this.logScrollView.setPropertyExpand(Vector2b.TRUE);
		this.logScrollView.setPropertyFill(Vector2b.TRUE);
		logPanel.subWidgetAdd(this.logScrollView);

		return logPanel;
	}

	public static void staticClearLog(final BasicWindows self) {
		self.clearLog();
	}

	/**
	 * Clears all log entries.
	 */
	public void clearLog() {
		this.logLines.clear();
		if (this.logContent != null) {
			this.logContent.subWidgetRemoveAll();
		}
	}

	/**
	 * Adds a log entry to the log panel.
	 */
	public void addLogEntry(final String message) {
		// Check if log panel is initialized
		if (this.logContent == null || this.logScrollView == null) {
			return;
		}

		// Add to list
		this.logLines.add(message);

		// Limit the number of lines
		while (this.logLines.size() > MAX_LOG_LINES) {
			this.logLines.remove(0);
		}

		// Add label widget
		final var logLabel = new Label(message);
		logLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
		logLabel.setPropertyFill(Vector2b.TRUE);
		logLabel.setPropertyGravity(Gravity.LEFT);
		this.logContent.subWidgetAdd(logLabel);

		// Remove oldest widget if exceeding limit
		final var widgets = this.logContent.getSubWidgets();
		if (widgets != null && widgets.size() > MAX_LOG_LINES) {
			this.logContent.subWidgetRemove(widgets.get(0));
		}

		// Scroll to bottom to show newest entry
		this.logScrollView.scrollToBottom();
	}
}
