package sample.atriasoft.ewol;

import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import org.atriasoft.esignal.Connection;
import org.atriasoft.etk.Color;
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
import org.atriasoft.ewol.widget.Spacer;
import org.atriasoft.ewol.widget.SplitPane;
import org.atriasoft.ewol.widget.Windows;
import org.atriasoft.ewol.widget.menu.MenuBar;
import org.atriasoft.ewol.widget.menu.MenuPopup;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class BasicWindows extends Windows {
	private static final Logger LOGGER = LoggerFactory.getLogger(BasicWindows.class);
	private static final int MAX_LOG_LINES = 100;

	// Category display order
	private static final List<String> CATEGORY_ORDER = List.of("Basic", "Layout", "Data", "Dialog", "Overlay");

	private final List<TestWidgetInterface> testedElement = new ArrayList<>();
	private Container container = null;
	private ModelWidget currentModelWidget = null;

	// Log panel components
	private Sizer logContent;
	private ScrollView logScrollView;
	private final List<String> logLines = new ArrayList<>();

	// Grouped tests by category
	private final Map<String, List<TestWidgetInterface>> categorizedTests = new LinkedHashMap<>();

	// Keep signal connections alive (esignal uses weak references)
	private final List<Connection> connections = new ArrayList<>();

	public BasicWindows() {
		// Register all test widgets
		this.testedElement.add(new TestWidgetIcon());
		this.testedElement.add(new TestWidgetSelect());
		this.testedElement.add(new TestWidgetSlider());
		this.testedElement.add(new TestWidgetSliderShowcase());
		this.testedElement.add(new TestWidgetFileChooser());
		this.testedElement.add(new TestWidgetColorPicker());
		this.testedElement.add(new TestWidgetDialog());
		this.testedElement.add(new TestWidgetListFileSystem());
		this.testedElement.add(new TestWidgetTreeView());
		this.testedElement.add(new TestWidgetTreeFileSystem());
		this.testedElement.add(new TestWidgetScrollView());
		this.testedElement.add(new TestWidgetSplitPane());
		this.testedElement.add(new TestWidgetEntry());
		this.testedElement.add(new TestWidgetBox());
		this.testedElement.add(new TestWidgetButton());
		this.testedElement.add(new TestWidgetButtonToggle());
		this.testedElement.add(new TestWidgetCheckBox());
		this.testedElement.add(new TestWidgetFieldSet());
		this.testedElement.add(new TestWidgetFieldSetCheckable());
		this.testedElement.add(new TestWidgetFieldSetCollapsed());
		this.testedElement.add(new TestWidgetImage());
		this.testedElement.add(new TestWidgetLabel());
		this.testedElement.add(new TestWidgetTextRendering());
		this.testedElement.add(new TestWidgetToast());
		this.testedElement.add(new TestWidgetPopover());
		this.testedElement.add(new TestWidgetMenuBar());
		this.testedElement.add(new TestWidgetContextMenu());

		// Group by category (preserve order)
		for (final String category : CATEGORY_ORDER) {
			this.categorizedTests.put(category, new ArrayList<>());
		}
		for (final TestWidgetInterface test : this.testedElement) {
			final String category = test.getCategory();
			this.categorizedTests.computeIfAbsent(category, k -> new ArrayList<>()).add(test);
		}

		// Build UI
		final Sizer sizerMain = new Sizer(DisplayMode.VERTICAL);
		sizerMain.setPropertyExpand(Vector2b.TRUE);
		sizerMain.setPropertyFill(Vector2b.TRUE);
		setSubWidget(sizerMain);

		// MenuBar
		sizerMain.subWidgetAdd(buildMenuBar());

		// Main content container
		this.container = new Container();
		this.container.setPropertyExpand(Vector2b.TRUE);
		this.container.setPropertyFill(Vector2b.TRUE);
		this.container.setPropertyExpandIfFree(Vector2b.TRUE);
		sizerMain.subWidgetAdd(this.container);

		// Show landing page
		showLandingPage();
	}

	// ========================================================================
	// MenuBar
	// ========================================================================

	private MenuBar buildMenuBar() {
		final MenuBar menuBar = MenuBar.create();
		// File menu with Quit
		menuBar.menu("File", () -> MenuPopup.create()
				.item("Home", "home", () -> showLandingPage())
				.separator()
				.item("Quit", "close", "alt+F4", () -> System.exit(0)));

		// One menu per category
		for (final Map.Entry<String, List<TestWidgetInterface>> entry : this.categorizedTests.entrySet()) {
			final String category = entry.getKey();
			final List<TestWidgetInterface> tests = entry.getValue();
			if (tests.isEmpty()) {
				continue;
			}
			menuBar.menu(category, () -> {
				final MenuPopup popup = MenuPopup.create();
				for (final TestWidgetInterface test : tests) {
					final TestWidgetInterface capturedTest = test;
					popup.item(test.getTitle(), () -> showTest(capturedTest));
				}
				return popup;
			});
		}

		return menuBar;
	}

	// ========================================================================
	// Landing Page
	// ========================================================================

	public void showLandingPage() {
		setPropertyTitle("ewol Widget Gallery");
		clearLog();
		this.connections.clear();

		final Sizer landingContent = new Sizer(DisplayMode.VERTICAL);
		landingContent.setPropertyExpand(Vector2b.TRUE);
		landingContent.setPropertyFill(Vector2b.TRUE);

		// Title
		final Label titleLabel = new Label("<b>ewol Widget Gallery</b>");
		titleLabel.setPropertyFontSize(20);
		titleLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
		titleLabel.setPropertyFill(Vector2b.FALSE);
		titleLabel.setPropertyGravity(Gravity.CENTER);
		landingContent.subWidgetAdd(titleLabel);

		// Spacer after title
		final Spacer titleSpacer = new Spacer();
		titleSpacer.setPropertyMinSize(new Dimension2f(new Vector2f(0, 16), Distance.PIXEL));
		titleSpacer.setPropertyExpand(Vector2b.TRUE_FALSE);
		landingContent.subWidgetAdd(titleSpacer);

		// For each category, add a section
		for (final Map.Entry<String, List<TestWidgetInterface>> entry : this.categorizedTests.entrySet()) {
			final String category = entry.getKey();
			final List<TestWidgetInterface> tests = entry.getValue();
			if (tests.isEmpty()) {
				continue;
			}

			// Category title
			final Label categoryLabel = new Label("<b>" + category + "</b>");
			categoryLabel.setPropertyFontSize(16);
			categoryLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
			categoryLabel.setPropertyFill(Vector2b.FALSE);
			categoryLabel.setPropertyGravity(Gravity.LEFT);
			landingContent.subWidgetAdd(categoryLabel);

			// Separator line
			final Spacer separator = new Spacer();
			separator.setPropertyMinSize(new Dimension2f(new Vector2f(0, 1), Distance.PIXEL));
			separator.setPropertyExpand(Vector2b.TRUE_FALSE);
			separator.setPropertyFill(Vector2b.TRUE);
			separator.setPropertyColor(new Color(0xC0, 0xC0, 0xC0, 0xFF));
			landingContent.subWidgetAdd(separator);

			// Spacer after separator
			final Spacer sepSpacer = new Spacer();
			sepSpacer.setPropertyMinSize(new Dimension2f(new Vector2f(0, 4), Distance.PIXEL));
			sepSpacer.setPropertyExpand(Vector2b.TRUE_FALSE);
			landingContent.subWidgetAdd(sepSpacer);

			// One row per test widget: button + description
			for (final TestWidgetInterface test : tests) {
				final TestWidgetInterface capturedTest = test;

				final Sizer row = new Sizer(DisplayMode.HORIZONTAL);
				row.setPropertyExpand(Vector2b.TRUE_FALSE);
				row.setPropertyFill(Vector2b.TRUE);
				row.setPropertyGravity(Gravity.LEFT);
				landingContent.subWidgetAdd(row);

				final Button btn = Button.createLabelButton(test.getTitle());
				btn.setPropertyExpand(Vector2b.FALSE);
				btn.setPropertyMinSize(new Dimension2f(new Vector2f(150, 0), Distance.PIXEL));
				this.connections.add(btn.signalClick.connect(() -> showTest(capturedTest)));
				row.subWidgetAdd(btn);

				final String description = test.getDescription();
				if (!description.isEmpty()) {
					final Label descLabel = new Label("<font color=\"#808080\">" + description + "</font>");
					descLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
					descLabel.setPropertyFill(Vector2b.FALSE);
					descLabel.setPropertyGravity(Gravity.LEFT);
					row.subWidgetAdd(descLabel);
				}
			}

			// Spacer between categories
			final Spacer catSpacer = new Spacer();
			catSpacer.setPropertyMinSize(new Dimension2f(new Vector2f(0, 16), Distance.PIXEL));
			catSpacer.setPropertyExpand(Vector2b.TRUE_FALSE);
			landingContent.subWidgetAdd(catSpacer);
		}

		// Wrap in ScrollView (vertical only)
		final ScrollView scrollView = ScrollView.create()
				.content(landingContent)
				.showVertical(true)
				.showHorizontal(false);
		scrollView.setPropertyExpand(Vector2b.TRUE);
		scrollView.setPropertyFill(Vector2b.TRUE);

		this.container.setSubWidget(scrollView);
	}

	// ========================================================================
	// Test Widget Display
	// ========================================================================

	public void showTest(final TestWidgetInterface test) {
		LOGGER.info("Show test: {}", test.getTitle());
		setPropertyTitle(test.getTitle());
		clearLog();
		this.connections.clear();

		// SplitPane: test widget (top) + log panel (bottom)
		final SplitPane splitPane = SplitPane.vertical()
				.splitPosition(0.25f)
				.minSizes(100.0f, 50.0f);
		splitPane.setPropertyExpand(Vector2b.TRUE);
		splitPane.setPropertyFill(Vector2b.TRUE);

		// Test widget area
		final Container testContainer = new Container();
		testContainer.setPropertyExpand(Vector2b.TRUE);
		testContainer.setPropertyFill(Vector2b.TRUE);
		testContainer.setPropertyExpandIfFree(Vector2b.TRUE);
		splitPane.second(testContainer);

		// Log panel
		final Sizer logPanel = createLogPanel();
		splitPane.first(logPanel);

		// Create ModelWidget
		this.currentModelWidget = new ModelWidget(test, this::addLogEntry);
		testContainer.setSubWidget(this.currentModelWidget);

		this.container.setSubWidget(splitPane);
	}

	// ========================================================================
	// Log Panel
	// ========================================================================

	/**
	 * Creates the log panel for the split pane.
	 * @return the log panel widget
	 */
	private Sizer createLogPanel() {
		final Sizer logPanel = new Sizer(DisplayMode.VERTICAL);
		logPanel.setPropertyExpand(Vector2b.TRUE);
		logPanel.setPropertyFill(Vector2b.TRUE);

		// Header with title and clear button
		final Sizer header = new Sizer(DisplayMode.HORIZONTAL);
		header.setPropertyExpand(Vector2b.TRUE_FALSE);
		header.setPropertyFill(Vector2b.TRUE);
		logPanel.subWidgetAdd(header);

		final Label titleLabel = new Label("<b>Signal Events Log:</b>");
		titleLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
		titleLabel.setPropertyFill(Vector2b.TRUE);
		titleLabel.setPropertyGravity(Gravity.LEFT);
		header.subWidgetAdd(titleLabel);

		final Button clearButton = Button.createLabelButton("Clear");
		clearButton.setPropertyExpand(Vector2b.FALSE);
		header.subWidgetAdd(clearButton);
		this.connections.add(clearButton.signalClick.connect(this::clearLog));

		// Log content area
		this.logContent = new Sizer(DisplayMode.VERTICAL);
		this.logContent.setPropertyExpand(Vector2b.TRUE_FALSE);
		this.logContent.setPropertyFill(Vector2b.TRUE);
		this.logContent.setPropertyGravity(Gravity.TOP_LEFT);

		this.logScrollView = ScrollView.create()
				.content(this.logContent)
				.showVertical(true)
				.showHorizontal(false);
		this.logScrollView.setPropertyExpand(Vector2b.TRUE);
		this.logScrollView.setPropertyFill(Vector2b.TRUE);
		logPanel.subWidgetAdd(this.logScrollView);

		return logPanel;
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
		if (this.logContent == null || this.logScrollView == null) {
			return;
		}

		this.logLines.add(message);

		while (this.logLines.size() > MAX_LOG_LINES) {
			this.logLines.remove(0);
		}

		final Label logLabel = new Label(message);
		logLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
		logLabel.setPropertyFill(Vector2b.TRUE);
		logLabel.setPropertyGravity(Gravity.LEFT);
		this.logContent.subWidgetAdd(logLabel);

		final List<?> widgets = this.logContent.getSubWidgets();
		if (widgets != null && widgets.size() > MAX_LOG_LINES) {
			this.logContent.subWidgetRemove(this.logContent.getSubWidgets().get(0));
		}

		this.logScrollView.scrollToBottom();
	}
}
