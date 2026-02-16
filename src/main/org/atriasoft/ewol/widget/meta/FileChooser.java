/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget.meta;

import java.io.File;

import org.atriasoft.esignal.Signal;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.DimensionBorderRadius;
import org.atriasoft.etk.DimensionInsets;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.widget.Box;
import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.CheckBox;
import org.atriasoft.ewol.widget.Entry;
import org.atriasoft.ewol.widget.Icon;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.ListFileSystem;
import org.atriasoft.ewol.widget.PopUp;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;
import org.atriasoft.ewol.widget.Spacer;
import org.atriasoft.ewol.widget.SplitPane;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * FileChooser is a simple file selector widget for opening or saving files.
 *
 * <p>Example usage:</p>
 * <pre>{@code
 * FileChooser fileChooser = FileChooser.create()
 *     .title("Open file...")
 *     .validateLabel("Open")
 *     .path("/home/user");
 *
 * fileChooser.signalValidate.connectAuto(this, MyClass::onFileSelected);
 * fileChooser.signalCancel.connectAuto(this, MyClass::onFileCanceled);
 *
 * Windows windows = getWindows();
 * if (windows != null) {
 *     windows.popUpWidgetPush(fileChooser);
 * }
 * }</pre>
 */
public class FileChooser extends PopUp {
	private static final Logger LOGGER = LoggerFactory.getLogger(FileChooser.class);

	private static final Color COLOR_OVERLAY = new Color(0x00, 0x00, 0x00, 0xA0);
	private static final Color COLOR_CONTENT = new Color(0x35, 0x35, 0x35, 0xFF);
	private static final Color COLOR_HEADER_FOOTER = new Color(0xB0, 0xB0, 0xB0, 0xFF);
	private static final Color COLOR_BORDER = new Color(0xB0, 0xB0, 0xB0, 0xFF);
	private static final Color COLOR_ICON_FILL = new Color(0xFF, 0xFF, 0xFF, 0xFF);
	private static final Color COLOR_ICON_FILL_DARK = new Color(0x00, 0x00, 0x00, 0xFF);

	// ========================================================================
	// Signals
	// ========================================================================

	public final SignalEmpty signalCancel = new SignalEmpty();
	public final Signal<String> signalValidate = new Signal<>();

	// ========================================================================
	// Properties
	// ========================================================================

	private String propertyPath = System.getProperty("user.home");
	private String propertyFile = "";
	private String propertyLabelTitle = "FileChooser";
	private String propertyLabelValidate = "Validate";
	private String propertyLabelCancel = "Cancel";

	// ========================================================================
	// Internal widgets
	// ========================================================================

	private final Label titleLabel;
	private final Entry entryFolder;
	private final Entry entryFile;
	private final Icon iconHome;
	private final ListFileSystem listFolder;
	private final ListFileSystem listFiles;
	private final CheckBox showHiddenCheckBox;
	private final Button validateButton;
	private final Button cancelButton;
	private final Label validateLabelWidget;
	private final Label cancelLabelWidget;

	// ========================================================================
	// Constructor
	// ========================================================================

	public FileChooser() {
		// PopUp background
		setPropertyColor(COLOR_OVERLAY);
		setPropertyExpand(Vector2b.TRUE);
		setPropertyFill(Vector2b.TRUE);

		// Create all internal widgets
		this.titleLabel = new Label(this.propertyLabelTitle);
		this.titleLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
		this.titleLabel.setPropertyFill(Vector2b.TRUE_FALSE);

		this.entryFolder = Entry.create();
		this.entryFolder.setPropertyExpand(Vector2b.TRUE_FALSE);
		this.entryFolder.setPropertyFill(Vector2b.TRUE_FALSE);

		this.entryFile = Entry.create();
		this.entryFile.setPropertyExpand(Vector2b.TRUE_FALSE);
		this.entryFile.setPropertyFill(Vector2b.TRUE_FALSE);

		this.iconHome = Icon.create("home");
		this.iconHome.setPropertyIconSize(new Dimension2f(new Vector2f(32, 32), Distance.PIXEL));
		this.iconHome.setPropertyFillColor(COLOR_ICON_FILL);
		this.iconHome.setPropertyExpand(Vector2b.FALSE);

		this.listFolder = ListFileSystem.create()
				.showFiles(false)
				.showFolders(true)
				.showHidden(false);
		this.listFolder.setPropertyExpand(Vector2b.TRUE);
		this.listFolder.setPropertyFill(Vector2b.TRUE);

		this.listFiles = ListFileSystem.create()
				.showFiles(true)
				.showFolders(false)
				.showHidden(false);
		this.listFiles.setPropertyExpand(Vector2b.TRUE);
		this.listFiles.setPropertyFill(Vector2b.TRUE);

		this.showHiddenCheckBox = CheckBox.create("Show Hidden Files");

		this.validateLabelWidget = new Label(this.propertyLabelValidate);
		this.cancelLabelWidget = new Label(this.propertyLabelCancel);

		this.validateButton = new Button();
		this.validateButton.setSubWidget(buildButtonContent("open-in-app", this.validateLabelWidget));

		this.cancelButton = new Button();
		this.cancelButton.setSubWidget(buildButtonContent("cancel", this.cancelLabelWidget));

		// Connect signals
		this.showHiddenCheckBox.signalValue.connectAuto(this, FileChooser::onCallbackHidenFileChangeChangeValue);
		this.validateButton.signalClick.connectAuto(this, FileChooser::onCallbackListValidate);
		this.cancelButton.signalClick.connectAuto(this, FileChooser::onCallbackButtonCancelPressed);
		this.listFolder.signalFolderValidate.connectAuto(this, FileChooser::onCallbackListFolderSelectChange);
		this.listFiles.signalFileSelect.connectAuto(this, FileChooser::onCallbackListFileSelectChange);
		this.listFiles.signalFileValidate.connectAuto(this, FileChooser::onCallbackListFileValidate);
		this.entryFile.signalModify.connectAuto(this, FileChooser::onCallbackEntryFileChangeValue);
		this.entryFile.signalEnter.connectAuto(this, FileChooser::onCallbackEntryFileChangeValidate);
		this.entryFolder.signalModify.connectAuto(this, FileChooser::onCallbackEntryFolderChangeValue);
		this.iconHome.signalPressed.connectAuto(this, FileChooser::onCallbackHomePressed);

		// Build the widget tree
		setSubWidget(buildLayout());

		// Initialize folder view
		updateCurrentFolder();
		setPropertyCanFocus(true);
	}

	// ========================================================================
	// Layout construction
	// ========================================================================

	private Box buildLayout() {
		// Dialog box (centered, 80%x80%)
		final Box dialogBox = new Box();
		dialogBox.setPropertyColor(new Color(0x00, 0x00, 0x00, 0x00));
		dialogBox.setPropertyPadding(new DimensionInsets(0));
		dialogBox.setPropertyMargin(new DimensionInsets(0));
		dialogBox.setPropertyMinSize(new Dimension2f(new Vector2f(80, 80), Distance.POURCENT));
		dialogBox.setPropertyMaxSize(new Dimension2f(new Vector2f(80, 80), Distance.POURCENT));
		dialogBox.setPropertyExpand(Vector2b.FALSE);
		dialogBox.setPropertyFill(Vector2b.FALSE);

		final Sizer mainSizer = new Sizer(DisplayMode.VERTICAL);
		mainSizer.setPropertyExpand(Vector2b.TRUE);
		mainSizer.setPropertyFill(Vector2b.TRUE);
		mainSizer.setPropertyLockExpand(Vector2b.TRUE);
		dialogBox.setSubWidget(mainSizer);

		// Title bar
		mainSizer.subWidgetAdd(buildTitleBar());

		// Content area
		mainSizer.subWidgetAdd(buildContentArea());

		// Footer
		mainSizer.subWidgetAdd(buildFooter());

		return dialogBox;
	}

	private Box buildTitleBar() {
		final Box titleBar = new Box();
		titleBar.setPropertyColor(COLOR_HEADER_FOOTER);
		titleBar.setPropertyBorderRadius(new DimensionBorderRadius(8, 8, 0, 0));
		titleBar.setPropertyPadding(new DimensionInsets(10));
		titleBar.setPropertyExpand(Vector2b.TRUE_FALSE);
		titleBar.setPropertyFill(Vector2b.TRUE_FALSE);
		titleBar.setSubWidget(this.titleLabel);
		return titleBar;
	}

	private Box buildContentArea() {
		final Box contentBox = new Box();
		contentBox.setPropertyColor(COLOR_CONTENT);
		contentBox.setPropertyBorderColor(COLOR_BORDER);
		contentBox.setPropertyBorderWidth(new DimensionInsets(0, 2, 0, 2));
		contentBox.setPropertyPadding(new DimensionInsets(10));
		contentBox.setPropertyExpand(Vector2b.TRUE);
		contentBox.setPropertyFill(Vector2b.TRUE);

		final Sizer contentSizer = new Sizer(DisplayMode.VERTICAL);
		contentSizer.setPropertyExpand(Vector2b.TRUE);
		contentSizer.setPropertyFill(Vector2b.TRUE);
		contentBox.setSubWidget(contentSizer);

		// Folder path entry row
		contentSizer.subWidgetAdd(buildFolderRow());

		// Spacer
		contentSizer.subWidgetAdd(createVerticalSpacer(5));

		// File name entry row
		contentSizer.subWidgetAdd(buildFileRow());

		// Spacer
		contentSizer.subWidgetAdd(createVerticalSpacer(10));

		// Split pane: folders | files
		final SplitPane splitPane = SplitPane.horizontal()
				.splitPosition(0.25f)
				.minSizes(100.0f, 150.0f);
		splitPane.setPropertyExpand(Vector2b.TRUE);
		splitPane.setPropertyFill(Vector2b.TRUE);
		splitPane.first(this.listFolder);
		splitPane.second(this.listFiles);
		contentSizer.subWidgetAdd(splitPane);

		return contentBox;
	}

	private Sizer buildFolderRow() {
		final Sizer row = new Sizer(DisplayMode.HORIZONTAL);
		row.setPropertyExpand(Vector2b.TRUE_FALSE);
		row.setPropertyFill(Vector2b.TRUE_FALSE);

		final Icon folderIcon = Icon.create("folder");
		folderIcon.setPropertyIconSize(new Dimension2f(new Vector2f(32, 32), Distance.PIXEL));
		folderIcon.setPropertyFillColor(COLOR_ICON_FILL);
		folderIcon.setPropertyExpand(Vector2b.FALSE);
		row.subWidgetAdd(folderIcon);

		row.subWidgetAdd(createHorizontalSpacer(5));
		row.subWidgetAdd(this.entryFolder);
		row.subWidgetAdd(createHorizontalSpacer(5));
		row.subWidgetAdd(this.iconHome);

		return row;
	}

	private Sizer buildFileRow() {
		final Sizer row = new Sizer(DisplayMode.HORIZONTAL);
		row.setPropertyExpand(Vector2b.TRUE_FALSE);
		row.setPropertyFill(Vector2b.TRUE_FALSE);

		final Icon fileIcon = Icon.create("file");
		fileIcon.setPropertyIconSize(new Dimension2f(new Vector2f(32, 32), Distance.PIXEL));
		fileIcon.setPropertyFillColor(COLOR_ICON_FILL);
		fileIcon.setPropertyExpand(Vector2b.FALSE);
		row.subWidgetAdd(fileIcon);

		row.subWidgetAdd(createHorizontalSpacer(5));
		row.subWidgetAdd(this.entryFile);

		return row;
	}

	private Box buildFooter() {
		final Box footerBox = new Box();
		footerBox.setPropertyColor(COLOR_HEADER_FOOTER);
		footerBox.setPropertyBorderRadius(new DimensionBorderRadius(0, 0, 8, 8));
		footerBox.setPropertyPadding(new DimensionInsets(8));
		footerBox.setPropertyExpand(Vector2b.TRUE_FALSE);
		footerBox.setPropertyFill(Vector2b.TRUE_FALSE);

		final Sizer footerSizer = new Sizer(DisplayMode.HORIZONTAL);
		footerSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
		footerSizer.setPropertyFill(Vector2b.TRUE_FALSE);
		footerBox.setSubWidget(footerSizer);

		footerSizer.subWidgetAdd(this.showHiddenCheckBox);

		// Expanding spacer to push buttons right
		final Spacer expandSpacer = new Spacer();
		expandSpacer.setPropertyExpand(Vector2b.TRUE_FALSE);
		footerSizer.subWidgetAdd(expandSpacer);

		footerSizer.subWidgetAdd(this.validateButton);
		footerSizer.subWidgetAdd(createHorizontalSpacer(10));
		footerSizer.subWidgetAdd(this.cancelButton);

		return footerBox;
	}

	private Sizer buildButtonContent(final String iconName, final Label label) {
		final Sizer sizer = new Sizer(DisplayMode.HORIZONTAL);

		final Icon icon = Icon.create(iconName);
		icon.setPropertyFillColor(COLOR_ICON_FILL_DARK);
		icon.setPropertyIconSize(new Dimension2f(new Vector2f(24, 24), Distance.PIXEL));
		sizer.subWidgetAdd(icon);

		sizer.subWidgetAdd(createHorizontalSpacer(5));
		sizer.subWidgetAdd(label);

		return sizer;
	}

	// ========================================================================
	// Helper methods
	// ========================================================================

	private static Spacer createVerticalSpacer(final float height) {
		final Spacer spacer = new Spacer();
		spacer.setPropertyMinSize(new Dimension2f(new Vector2f(0, height), Distance.PIXEL));
		return spacer;
	}

	private static Spacer createHorizontalSpacer(final float width) {
		final Spacer spacer = new Spacer();
		spacer.setPropertyMinSize(new Dimension2f(new Vector2f(width, 0), Distance.PIXEL));
		return spacer;
	}

	// ========================================================================
	// Static callbacks
	// ========================================================================

	static void onCallbackButtonCancelPressed(final FileChooser self) {
		self.signalCancel.emit();
		self.autoDestroy();
	}

	protected static void onCallbackEntryFileChangeValidate(final FileChooser self, final String value) {
		onCallbackListFileValidate(self, value);
	}

	protected static void onCallbackEntryFileChangeValue(final FileChooser self, final String value) {
		self.propertyFile = value;
		self.listFiles.setPropertyFile(new File(self.propertyFile));
	}

	protected static void onCallbackEntryFolderChangeValue(final FileChooser self, final String value) {
		final File folder = new File(value);
		if (folder.exists() && folder.isDirectory()) {
			self.propertyPath = value;
			self.propertyFile = "";
			self.updateCurrentFolder();
		}
	}

	protected static void onCallbackHidenFileChangeChangeValue(final FileChooser self, final Boolean value) {
		self.listFiles.setPropertyShowHidden(value);
		self.listFolder.setPropertyShowHidden(value);
	}

	protected static void onCallbackHomePressed(final FileChooser self) {
		final String tmpUserFolder = System.getProperty("user.home");
		LOGGER.debug("new PATH: '{}'", tmpUserFolder);
		self.propertyPath = tmpUserFolder;
		self.propertyFile = "";
		self.updateCurrentFolder();
	}

	protected static void onCallbackListFileSelectChange(final FileChooser self, final String value) {
		self.setPropertyFile(value);
	}

	protected static void onCallbackListFileValidate(final FileChooser self, final String value) {
		self.setPropertyFile(value);
		LOGGER.trace("Generate a file opening: '{}'", self.propertyFile);
		self.signalValidate.emit(value);
		self.autoDestroy();
	}

	protected static void onCallbackListFolderSelectChange(final FileChooser self, final String value) {
		LOGGER.debug("Path change: '{}' ==> '{}'", self.propertyPath, value);
		self.propertyPath = value;
		self.propertyFile = "";
		self.updateCurrentFolder();
	}

	protected static void onCallbackListValidate(final FileChooser self) {
		if (self.propertyFile.isEmpty()) {
			LOGGER.warn("Validate with empty file name");
			return;
		}
		LOGGER.debug("Generate file opening: '{}'", self.propertyFile);
		self.signalValidate.emit(self.propertyFile);
		self.autoDestroy();
	}

	// ========================================================================
	// Property accessors
	// ========================================================================

	public String getPropertyFile() {
		return this.propertyFile;
	}

	public String getPropertyLabelCancel() {
		return this.propertyLabelCancel;
	}

	public String getPropertyLabelTitle() {
		return this.propertyLabelTitle;
	}

	public String getPropertyLabelValidate() {
		return this.propertyLabelValidate;
	}

	public String getPropertyPath() {
		return this.propertyPath;
	}

	public void setPropertyFile(final String propertyFile) {
		if (this.propertyFile.equals(propertyFile)) {
			return;
		}
		this.propertyFile = propertyFile;
		this.entryFile.setPropertyValue(this.propertyFile);
	}

	public void setPropertyLabelCancel(final String propertyLabelCancel) {
		if (this.propertyLabelCancel.equals(propertyLabelCancel)) {
			return;
		}
		this.propertyLabelCancel = propertyLabelCancel;
		this.cancelLabelWidget.setPropertyValue(this.propertyLabelCancel);
	}

	public void setPropertyLabelTitle(final String propertyLabelTitle) {
		if (this.propertyLabelTitle.equals(propertyLabelTitle)) {
			return;
		}
		this.propertyLabelTitle = propertyLabelTitle;
		this.titleLabel.setPropertyValue(this.propertyLabelTitle);
	}

	public void setPropertyLabelValidate(final String propertyLabelValidate) {
		if (this.propertyLabelValidate.equals(propertyLabelValidate)) {
			return;
		}
		this.propertyLabelValidate = propertyLabelValidate;
		this.validateLabelWidget.setPropertyValue(this.propertyLabelValidate);
	}

	public void setPropertyPath(final String propertyPath) {
		if (this.propertyPath.equals(propertyPath)) {
			return;
		}
		this.propertyPath = propertyPath;
		updateCurrentFolder();
	}

	@Override
	public void onGetFocus() {
		this.entryFolder.keepFocus();
	}

	private void updateCurrentFolder() {
		this.listFiles.setPropertyPath(this.propertyPath);
		this.listFolder.setPropertyPath(this.propertyPath);
		this.entryFolder.setPropertyValue(this.propertyPath);
		markToRedraw();
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	public static FileChooser create() {
		return new FileChooser();
	}

	public FileChooser title(final String title) {
		setPropertyLabelTitle(title);
		return this;
	}

	public FileChooser validateLabel(final String label) {
		setPropertyLabelValidate(label);
		return this;
	}

	public FileChooser cancelLabel(final String label) {
		setPropertyLabelCancel(label);
		return this;
	}

	public FileChooser path(final String path) {
		setPropertyPath(path);
		return this;
	}

	public FileChooser file(final String file) {
		setPropertyFile(file);
		return this;
	}

	public String getFullPath() {
		if (this.propertyFile.isEmpty()) {
			return this.propertyPath;
		}
		final String separator = this.propertyPath.endsWith("/") ? "" : "/";
		return this.propertyPath + separator + this.propertyFile;
	}
}
