/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.DimensionBorderRadius;
import org.atriasoft.etk.DimensionInsets;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.Insets;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;

/**
 * Dialog is a structured popup with a title bar (including a close button),
 * a content area, and a footer button bar.
 *
 * <p>Structure:</p>
 * <pre>
 * Dialog (overlay)
 *   └── dialogBox (centered, 80%x80% by default)
 *         └── mainSizer (vertical)
 *               ├── titleBar (title label + close icon)
 *               ├── contentBox (user content)
 *               └── footerBox (user buttons)
 * </pre>
 *
 * <p>Example usage:</p>
 * <pre>{@code
 * Dialog dialog = Dialog.create()
 *     .title("My Dialog")
 *     .content(myContentWidget)
 *     .footer(Button.create("OK").onClick(() -> dialog.autoDestroy()))
 *     .footer(Button.create("Cancel").onClick(() -> dialog.autoDestroy()));
 *
 * windows.popUpWidgetPush(dialog);
 * }</pre>
 */
public class Dialog extends PopUp {

	// ========================================================================
	// Color constants (shared with subclasses)
	// ========================================================================

	protected static final Color COLOR_OVERLAY = new Color(0x00, 0x00, 0x00, 0x60);
	protected static final Color COLOR_CONTENT = new Color(0xF5, 0xF5, 0xF5, 0xFF);
	protected static final Color COLOR_HEADER_FOOTER = new Color(0xE0, 0xE0, 0xE0, 0xFF);
	protected static final Color COLOR_BORDER = new Color(0xC0, 0xC0, 0xC0, 0xFF);

	// ========================================================================
	// Signal
	// ========================================================================

	public final SignalEmpty signalClose = new SignalEmpty();

	// ========================================================================
	// Internal widgets
	// ========================================================================

	private final Label titleLabel;
	private final Button closeButton;
	private final Box contentBox;
	private final Sizer footerSizer;
	private final Box dialogBox;

	// ========================================================================
	// Properties
	// ========================================================================

	private String propertyTitle = "Dialog";

	// ========================================================================
	// Constructor
	// ========================================================================

	public Dialog() {
		// Override PopUp defaults: PopUp sets minSize=80% and expand=FALSE,
		// but Dialog needs to fill the entire window for the overlay effect.
		this.propertyMinSize = new Dimension2f(new Vector2f(100, 100), Distance.POURCENT);
		setPropertyExpand(Vector2b.TRUE);
		setPropertyFill(Vector2b.TRUE);

		// Overlay background
		setPropertyColor(COLOR_OVERLAY);

		// Title label
		this.titleLabel = new Label(this.propertyTitle);
		this.titleLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
		this.titleLabel.setPropertyFill(Vector2b.TRUE_FALSE);

		// Close button (X)
		final Icon closeIcon = Icon.create("close");
		closeIcon.setPropertyFillColor(new Color(0x40, 0x40, 0x40, 0xFF));
		closeIcon.setPropertyIconSize(new Dimension2f(new Vector2f(16, 16), Distance.PIXEL));
		closeIcon.setPropertyExpand(Vector2b.FALSE);
		this.closeButton = new Button();
		this.closeButton.setPropertyExpand(Vector2b.FALSE);
		this.closeButton.setPropertyFill(Vector2b.FALSE);
		this.closeButton.setPropertyColor(COLOR_HEADER_FOOTER);
		this.closeButton.setPropertyBorderColor(Color.NONE);
		this.closeButton.setPropertyBorderWidth(new DimensionInsets(2));
		this.closeButton.setPropertyPadding(new DimensionInsets(2));
		this.closeButton.setPropertyMargin(new DimensionInsets(0));
		this.closeButton.setSubWidget(closeIcon);
		this.closeButton.signalClick.connectAuto(this, Dialog::onCloseClicked);

		// Content area
		this.contentBox = new Box();
		this.contentBox.setPropertyColor(COLOR_CONTENT);
		this.contentBox.setPropertyBorderColor(COLOR_BORDER);
		this.contentBox.setPropertyBorderWidth(new DimensionInsets(0, 2, 0, 2));
		this.contentBox.setPropertyPadding(new DimensionInsets(10));
		this.contentBox.setPropertyExpand(Vector2b.TRUE);
		this.contentBox.setPropertyFill(Vector2b.TRUE);

		// Footer sizer
		this.footerSizer = new Sizer(Sizer.DisplayMode.HORIZONTAL);
		this.footerSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
		this.footerSizer.setPropertyFill(Vector2b.TRUE_FALSE);

		// Build the dialog structure
		this.dialogBox = buildDialogBox();

		// Set as the PopUp's content
		setSubWidget(this.dialogBox);

		setPropertyCanFocus(true);
	}

	// ========================================================================
	// Layout
	// ========================================================================

	@Override
	public void onChangeSize() {
		markToRedraw();
		if (this.propertyHide) {
			return;
		}
		if (this.subWidget == null) {
			return;
		}
		// The dialogBox has expand=FALSE so Box.onChangeSize() would give it
		// this.minSize (= 100% of window), making it fill the entire overlay.
		// Instead, use the child's own calculated size, clamped by its maxSize.
		final Insets offsetSubWidget = getBorderAggregation();
		final Vector2f availableSize = this.size.less(offsetSubWidget.toVector2f());
		final Vector2f childMin = this.subWidget.getCalculateMinSize();
		final Vector2f childMax = this.subWidget.getCalculateMaxSize();
		// Clamp child size between its min and max, within available space
		final float childW = Math.min(Math.min(childMax.x(), availableSize.x()), Math.max(childMin.x(), availableSize.x()));
		final float childH = Math.min(Math.min(childMax.y(), availableSize.y()), Math.max(childMin.y(), availableSize.y()));
		final Vector2f subWidgetSize = new Vector2f(childW, childH).clipInteger();
		// Center the dialogBox within the overlay
		final Vector2f freeSpace = availableSize.less(subWidgetSize);
		final Vector2f childOrigin = this.origin.add(offsetSubWidget.getOrigin())
				.add(new Vector2f(freeSpace.x() / 2.0f, freeSpace.y() / 2.0f)).clipInteger();
		this.subWidget.setOrigin(childOrigin);
		this.subWidget.setSize(subWidgetSize);
		this.subWidget.onChangeSize();
	}

	// ========================================================================
	// Layout construction
	// ========================================================================

	private Box buildTitleBar() {
		final Box bar = new Box();
		bar.setPropertyColor(COLOR_HEADER_FOOTER);
		bar.setPropertyBorderRadius(new DimensionBorderRadius(8, 8, 0, 0));
		bar.setPropertyPadding(new DimensionInsets(10));
		bar.setPropertyExpand(Vector2b.TRUE_FALSE);
		bar.setPropertyFill(Vector2b.TRUE_FALSE);

		final Sizer titleSizer = new Sizer(Sizer.DisplayMode.HORIZONTAL);
		titleSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
		titleSizer.setPropertyFill(Vector2b.TRUE_FALSE);
		titleSizer.subWidgetAdd(this.titleLabel);

		// Expanding spacer pushes close icon to the right
		final Spacer expandSpacer = new Spacer();
		expandSpacer.setPropertyExpand(Vector2b.TRUE_FALSE);
		titleSizer.subWidgetAdd(expandSpacer);

		titleSizer.subWidgetAdd(this.closeButton);

		bar.setSubWidget(titleSizer);
		return bar;
	}

	private Box buildFooterBox() {
		final Box footer = new Box();
		footer.setPropertyColor(COLOR_HEADER_FOOTER);
		footer.setPropertyBorderRadius(new DimensionBorderRadius(0, 0, 8, 8));
		footer.setPropertyPadding(new DimensionInsets(10));
		footer.setPropertyExpand(Vector2b.TRUE_FALSE);
		footer.setPropertyFill(Vector2b.TRUE_FALSE);
		footer.setSubWidget(this.footerSizer);
		return footer;
	}

	private Box buildDialogBox() {
		final Box dialog = new Box();
		dialog.setPropertyColor(new Color(0x00, 0x00, 0x00, 0x00));
		dialog.setPropertyPadding(new DimensionInsets(0));
		dialog.setPropertyMargin(new DimensionInsets(0));
		dialog.setPropertyMinSize(new Dimension2f(new Vector2f(80, 80), Distance.POURCENT));
		dialog.setPropertyMaxSize(new Dimension2f(new Vector2f(80, 80), Distance.POURCENT));
		dialog.setPropertyExpand(Vector2b.FALSE);
		dialog.setPropertyFill(Vector2b.FALSE);

		final Sizer mainSizer = new Sizer(Sizer.DisplayMode.VERTICAL);
		mainSizer.setPropertyExpand(Vector2b.TRUE);
		mainSizer.setPropertyFill(Vector2b.TRUE);
		mainSizer.setPropertyLockExpand(Vector2b.TRUE);
		dialog.setSubWidget(mainSizer);

		mainSizer.subWidgetAdd(buildTitleBar());
		mainSizer.subWidgetAdd(this.contentBox);
		mainSizer.subWidgetAdd(buildFooterBox());

		return dialog;
	}

	// ========================================================================
	// Close behavior
	// ========================================================================

	static void onCloseClicked(final Dialog self) {
		self.handleClose();
	}

	/**
	 * Called when the close button is clicked.
	 * Default: emit signalClose and destroy.
	 * Subclasses can override to add pre-close logic.
	 */
	protected void handleClose() {
		this.signalClose.emit();
		autoDestroy();
	}

	// ========================================================================
	// Content and Footer API
	// ========================================================================

	/**
	 * Set the content widget displayed in the dialog body.
	 * @param widget the content widget
	 */
	public void setContentWidget(final Widget widget) {
		this.contentBox.setSubWidget(widget);
	}

	/**
	 * Get the content area Box (for advanced customization).
	 * @return the content Box
	 */
	protected Box getContentBox() {
		return this.contentBox;
	}

	/**
	 * Add a widget to the footer button bar.
	 * @param widget the widget to add (typically a Button or Spacer)
	 */
	public void addFooterWidget(final Widget widget) {
		this.footerSizer.subWidgetAdd(widget);
	}

	/**
	 * Get the footer Sizer directly (for advanced customization).
	 * @return the footer Sizer
	 */
	protected Sizer getFooterSizer() {
		return this.footerSizer;
	}

	// ========================================================================
	// Property accessors
	// ========================================================================

	public String getPropertyTitle() {
		return this.propertyTitle;
	}

	public void setPropertyTitle(final String title) {
		if (this.propertyTitle.equals(title)) {
			return;
		}
		this.propertyTitle = title;
		this.titleLabel.setPropertyValue(this.propertyTitle);
	}

	/**
	 * Set the dialog box size using pixel dimensions.
	 * @param minWidth minimum width in pixels
	 * @param minHeight minimum height in pixels
	 * @param maxWidth maximum width in pixels
	 * @param maxHeight maximum height in pixels
	 */
	public void setDialogSize(final float minWidth, final float minHeight,
	                           final float maxWidth, final float maxHeight) {
		this.dialogBox.setPropertyMinSize(
				new Dimension2f(new Vector2f(minWidth, minHeight), Distance.PIXEL));
		this.dialogBox.setPropertyMaxSize(
				new Dimension2f(new Vector2f(maxWidth, maxHeight), Distance.PIXEL));
	}

	/**
	 * Set the dialog box size using percentage dimensions.
	 * @param widthPercent width as percentage (0-100)
	 * @param heightPercent height as percentage (0-100)
	 */
	public void setDialogSizePercent(final float widthPercent, final float heightPercent) {
		this.dialogBox.setPropertyMinSize(
				new Dimension2f(new Vector2f(widthPercent, heightPercent), Distance.POURCENT));
		this.dialogBox.setPropertyMaxSize(
				new Dimension2f(new Vector2f(widthPercent, heightPercent), Distance.POURCENT));
	}

	// ========================================================================
	// Helper methods
	// ========================================================================

	/**
	 * Create a horizontal spacer with fixed width.
	 * @param width width in pixels
	 * @return a new Spacer widget
	 */
	protected static Spacer createHorizontalSpacer(final float width) {
		final Spacer spacer = new Spacer();
		spacer.setPropertyMinSize(new Dimension2f(new Vector2f(width, 0), Distance.PIXEL));
		return spacer;
	}

	/**
	 * Create a vertical spacer with fixed height.
	 * @param height height in pixels
	 * @return a new Spacer widget
	 */
	protected static Spacer createVerticalSpacer(final float height) {
		final Spacer spacer = new Spacer();
		spacer.setPropertyMinSize(new Dimension2f(new Vector2f(0, height), Distance.PIXEL));
		return spacer;
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	/**
	 * Create a new Dialog.
	 * @return a new Dialog
	 */
	public static Dialog create() {
		return new Dialog();
	}

	/**
	 * Fluent method to set the title.
	 * @param title the dialog title
	 * @return this dialog for chaining
	 */
	public Dialog title(final String title) {
		setPropertyTitle(title);
		return this;
	}

	/**
	 * Fluent method to set the content widget.
	 * Overrides PopUp.content() to set the dialog body content
	 * instead of the raw PopUp child.
	 * @param widget the content widget
	 * @return this dialog for chaining
	 */
	@Override
	public Dialog content(final Widget widget) {
		setContentWidget(widget);
		return this;
	}

	/**
	 * Fluent method to add a footer widget.
	 * @param widget the widget to add to the footer
	 * @return this dialog for chaining
	 */
	public Dialog footer(final Widget widget) {
		addFooterWidget(widget);
		return this;
	}

	/**
	 * Fluent method to set dialog size in pixels.
	 * @param minW minimum width
	 * @param minH minimum height
	 * @param maxW maximum width
	 * @param maxH maximum height
	 * @return this dialog for chaining
	 */
	public Dialog dialogSize(final float minW, final float minH,
	                          final float maxW, final float maxH) {
		setDialogSize(minW, minH, maxW, maxH);
		return this;
	}

	/**
	 * Fluent method to set dialog size in percentage.
	 * @param widthPercent width percentage
	 * @param heightPercent height percentage
	 * @return this dialog for chaining
	 */
	public Dialog dialogSizePercent(final float widthPercent, final float heightPercent) {
		setDialogSizePercent(widthPercent, heightPercent);
		return this;
	}

	/**
	 * Fluent method to connect a close callback.
	 * @param callback the callback to invoke when the dialog is closed
	 * @return this dialog for chaining
	 */
	public Dialog onClose(final Runnable callback) {
		this.signalClose.connect(callback);
		return this;
	}
}
