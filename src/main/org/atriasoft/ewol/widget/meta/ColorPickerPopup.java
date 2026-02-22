/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget.meta;

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
import org.atriasoft.ewol.widget.ColorGradient;
import org.atriasoft.ewol.widget.Entry;
import org.atriasoft.ewol.widget.Icon;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.PopUp;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;
import org.atriasoft.ewol.widget.Slider;
import org.atriasoft.ewol.widget.Spacer;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * ColorPickerPopup is a popup widget for selecting colors.
 *
 * <p>Features:</p>
 * <ul>
 *   <li>HSL gradient for visual color selection</li>
 *   <li>RGBA sliders (0-255) for precise control</li>
 *   <li>Hex entry field (#RRGGBBAA)</li>
 *   <li>Preview of current and original colors</li>
 *   <li>Cancel and Select buttons</li>
 * </ul>
 *
 * <p>Example usage:</p>
 * <pre>{@code
 * ColorPickerPopup picker = ColorPickerPopup.create(Color.CORAL);
 * picker.signalValidate.connectAuto(this, MyClass::onColorSelected);
 * picker.signalCancel.connectAuto(this, MyClass::onColorCanceled);
 *
 * Windows windows = getWindows();
 * if (windows != null) {
 *     windows.popUpWidgetPush(picker);
 * }
 * }</pre>
 */
public class ColorPickerPopup extends PopUp {
	private static final Logger LOGGER = LoggerFactory.getLogger(ColorPickerPopup.class);

	private static final Color COLOR_OVERLAY = new Color(0x00, 0x00, 0x00, 0xA0);
	private static final Color COLOR_CONTENT = new Color(0x35, 0x35, 0x35, 0xFF);
	private static final Color COLOR_HEADER_FOOTER = new Color(0xB0, 0xB0, 0xB0, 0xFF);
	private static final Color COLOR_BORDER = new Color(0xB0, 0xB0, 0xB0, 0xFF);
	private static final Color COLOR_PREVIEW_BORDER = new Color(0x60, 0x60, 0x60, 0xFF);
	private static final Color COLOR_ICON_SELECT = new Color(0x80, 0xFF, 0x80, 0xFF);
	private static final Color COLOR_ICON_CANCEL = new Color(0xFF, 0x80, 0x80, 0xFF);

	// ========================================================================
	// Signals
	// ========================================================================

	public final Signal<Color> signalColorChanged = new Signal<>();
	public final Signal<Color> signalValidate = new Signal<>();
	public final SignalEmpty signalCancel = new SignalEmpty();

	// ========================================================================
	// Properties
	// ========================================================================

	private Color propertyValue = Color.WHITE;
	private Color originalColor = Color.WHITE;
	private String propertyLabelTitle = "ColorPicker";
	private String propertyLabelSelect = "Select";
	private String propertyLabelCancel = "Cancel";

	// Current RGBA values (0-255)
	private int red = 255;
	private int green = 255;
	private int blue = 255;
	private int alpha = 255;

	// Flag to prevent recursive updates
	private boolean updating = false;

	// ========================================================================
	// Internal widgets
	// ========================================================================

	private final Label titleLabel;
	private final ColorGradient gradient;
	private final Slider sliderRed;
	private final Slider sliderGreen;
	private final Slider sliderBlue;
	private final Slider sliderAlpha;
	private final Entry entryHex;
	private final Spacer previewCurrent;
	private final Spacer previewOriginal;
	private final Button selectButton;
	private final Button cancelButton;
	private final Label selectLabelWidget;
	private final Label cancelLabelWidget;

	// ========================================================================
	// Constructor
	// ========================================================================

	public ColorPickerPopup() {
		setPropertyColor(COLOR_OVERLAY);
		setPropertyExpand(Vector2b.TRUE);
		setPropertyFill(Vector2b.TRUE);

		// Create all internal widgets
		this.titleLabel = new Label(this.propertyLabelTitle);
		this.titleLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
		this.titleLabel.setPropertyFill(Vector2b.TRUE_FALSE);

		this.gradient = ColorGradient.create();
		this.gradient.setPropertyMinSize(new Dimension2f(new Vector2f(200, 200), Distance.PIXEL));
		this.gradient.setPropertyExpand(Vector2b.FALSE);
		this.gradient.setPropertyFill(Vector2b.FALSE);

		this.sliderRed = Slider.create().range(0, 255).step(1).value(255)
				.fillColor(new Color(0xF4, 0x43, 0x36, 0xFF))
				.cursorBorderColor(new Color(0xD3, 0x2F, 0x2F, 0xFF));
		this.sliderRed.setPropertyExpand(Vector2b.TRUE_FALSE);
		this.sliderRed.setPropertyFill(Vector2b.TRUE_FALSE);
		this.sliderRed.setPropertyMinSize(new Dimension2f(new Vector2f(120, 25), Distance.PIXEL));

		this.sliderGreen = Slider.create().range(0, 255).step(1).value(255)
				.fillColor(new Color(0x4C, 0xAF, 0x50, 0xFF))
				.cursorBorderColor(new Color(0x38, 0x8E, 0x3C, 0xFF));
		this.sliderGreen.setPropertyExpand(Vector2b.TRUE_FALSE);
		this.sliderGreen.setPropertyFill(Vector2b.TRUE_FALSE);
		this.sliderGreen.setPropertyMinSize(new Dimension2f(new Vector2f(120, 25), Distance.PIXEL));

		this.sliderBlue = Slider.create().range(0, 255).step(1).value(255)
				.fillColor(new Color(0x21, 0x96, 0xF3, 0xFF))
				.cursorBorderColor(new Color(0x19, 0x76, 0xD2, 0xFF));
		this.sliderBlue.setPropertyExpand(Vector2b.TRUE_FALSE);
		this.sliderBlue.setPropertyFill(Vector2b.TRUE_FALSE);
		this.sliderBlue.setPropertyMinSize(new Dimension2f(new Vector2f(120, 25), Distance.PIXEL));

		this.sliderAlpha = Slider.create().range(0, 255).step(1).value(255);
		this.sliderAlpha.setPropertyExpand(Vector2b.TRUE_FALSE);
		this.sliderAlpha.setPropertyFill(Vector2b.TRUE_FALSE);
		this.sliderAlpha.setPropertyMinSize(new Dimension2f(new Vector2f(120, 25), Distance.PIXEL));

		this.entryHex = Entry.create();
		this.entryHex.maxCharacters(9);
		this.entryHex.setPropertyExpand(Vector2b.TRUE_FALSE);
		this.entryHex.setPropertyFill(Vector2b.TRUE_FALSE);

		this.previewCurrent = new Spacer();
		this.previewCurrent.setPropertyExpand(Vector2b.TRUE);
		this.previewCurrent.setPropertyFill(Vector2b.TRUE);

		this.previewOriginal = new Spacer();
		this.previewOriginal.setPropertyExpand(Vector2b.TRUE);
		this.previewOriginal.setPropertyFill(Vector2b.TRUE);

		this.selectLabelWidget = new Label(this.propertyLabelSelect);
		this.cancelLabelWidget = new Label(this.propertyLabelCancel);

		this.selectButton = new Button();
		this.selectButton.setSubWidget(buildButtonContent("check", COLOR_ICON_SELECT, this.selectLabelWidget));

		this.cancelButton = new Button();
		this.cancelButton.setSubWidget(buildButtonContent("cancel", COLOR_ICON_CANCEL, this.cancelLabelWidget));

		// Connect signals
		this.gradient.signalColorChanged.connectAuto(this, ColorPickerPopup::onGradientColorChanged);
		this.sliderRed.signalValue.connectAuto(this, ColorPickerPopup::onRedChanged);
		this.sliderGreen.signalValue.connectAuto(this, ColorPickerPopup::onGreenChanged);
		this.sliderBlue.signalValue.connectAuto(this, ColorPickerPopup::onBlueChanged);
		this.sliderAlpha.signalValue.connectAuto(this, ColorPickerPopup::onAlphaChanged);
		this.entryHex.signalModify.connectAuto(this, ColorPickerPopup::onHexChanged);
		this.selectButton.signalClick.connectAuto(this, ColorPickerPopup::onSelectClicked);
		this.cancelButton.signalClick.connectAuto(this, ColorPickerPopup::onCancelClicked);

		// Build the widget tree
		setSubWidget(buildLayout());

		setPropertyCanFocus(true);
	}

	public ColorPickerPopup(final Color initialColor) {
		this();
		setPropertyValue(initialColor);
		this.originalColor = initialColor;
		this.previewOriginal.setPropertyColor(this.originalColor);
	}

	// ========================================================================
	// Layout construction
	// ========================================================================

	private Box buildLayout() {
		final Box dialogBox = new Box();
		dialogBox.setPropertyColor(new Color(0x00, 0x00, 0x00, 0x00));
		dialogBox.setPropertyPadding(new DimensionInsets(0));
		dialogBox.setPropertyMargin(new DimensionInsets(0));
		dialogBox.setPropertyMinSize(new Dimension2f(new Vector2f(500, 320), Distance.PIXEL));
		dialogBox.setPropertyMaxSize(new Dimension2f(new Vector2f(500, 400), Distance.PIXEL));
		dialogBox.setPropertyExpand(Vector2b.FALSE);
		dialogBox.setPropertyFill(Vector2b.FALSE);

		final Sizer mainSizer = new Sizer(DisplayMode.VERTICAL);
		mainSizer.setPropertyExpand(Vector2b.TRUE);
		mainSizer.setPropertyFill(Vector2b.TRUE);
		mainSizer.setPropertyLockExpand(Vector2b.TRUE);
		dialogBox.setSubWidget(mainSizer);

		mainSizer.subWidgetAdd(buildTitleBar());
		mainSizer.subWidgetAdd(buildContentArea());
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

		final Sizer contentSizer = new Sizer(DisplayMode.HORIZONTAL);
		contentSizer.setPropertyExpand(Vector2b.TRUE);
		contentSizer.setPropertyFill(Vector2b.TRUE);
		contentBox.setSubWidget(contentSizer);

		// Left: Color gradient
		contentSizer.subWidgetAdd(this.gradient);

		// Spacer
		contentSizer.subWidgetAdd(createHorizontalSpacer(10));

		// Right: Controls
		contentSizer.subWidgetAdd(buildControlsPanel());

		return contentBox;
	}

	private Sizer buildControlsPanel() {
		final Sizer controls = new Sizer(DisplayMode.VERTICAL);
		controls.setPropertyExpand(Vector2b.TRUE_FALSE);
		controls.setPropertyFill(Vector2b.TRUE_FALSE);

		// Red slider row
		controls.subWidgetAdd(buildSliderRow("R:", this.sliderRed));

		// Green slider row
		controls.subWidgetAdd(buildSliderRow("G:", this.sliderGreen));

		// Blue slider row
		controls.subWidgetAdd(buildSliderRow("B:", this.sliderBlue));

		// Alpha slider row
		controls.subWidgetAdd(buildSliderRow("A:", this.sliderAlpha));

		// Spacer
		controls.subWidgetAdd(createVerticalSpacer(8));

		// Hex entry row
		final Sizer hexRow = new Sizer(DisplayMode.HORIZONTAL);
		hexRow.setPropertyExpand(Vector2b.TRUE_FALSE);
		hexRow.setPropertyFill(Vector2b.TRUE_FALSE);
		final Label hexLabel = new Label("Hex:");
		hexLabel.setPropertyMinSize(new Dimension2f(new Vector2f(35, 25), Distance.PIXEL));
		hexLabel.setPropertyExpand(Vector2b.FALSE);
		hexRow.subWidgetAdd(hexLabel);
		hexRow.subWidgetAdd(this.entryHex);
		controls.subWidgetAdd(hexRow);

		// Spacer
		controls.subWidgetAdd(createVerticalSpacer(8));

		// Preview boxes
		final Sizer previewRow = new Sizer(DisplayMode.HORIZONTAL);
		previewRow.setPropertyExpand(Vector2b.TRUE_FALSE);
		previewRow.setPropertyFill(Vector2b.TRUE_FALSE);
		previewRow.setPropertyMinSize(new Dimension2f(new Vector2f(0, 50), Distance.PIXEL));

		final Box previewCurrentBox = new Box();
		previewCurrentBox.setPropertyExpand(Vector2b.TRUE);
		previewCurrentBox.setPropertyFill(Vector2b.TRUE);
		previewCurrentBox.setPropertyBorderWidth(new DimensionInsets(1));
		previewCurrentBox.setPropertyBorderColor(COLOR_PREVIEW_BORDER);
		previewCurrentBox.setSubWidget(this.previewCurrent);
		previewRow.subWidgetAdd(previewCurrentBox);

		final Box previewOriginalBox = new Box();
		previewOriginalBox.setPropertyExpand(Vector2b.TRUE);
		previewOriginalBox.setPropertyFill(Vector2b.TRUE);
		previewOriginalBox.setPropertyBorderWidth(new DimensionInsets(1));
		previewOriginalBox.setPropertyBorderColor(COLOR_PREVIEW_BORDER);
		previewOriginalBox.setSubWidget(this.previewOriginal);
		previewRow.subWidgetAdd(previewOriginalBox);

		controls.subWidgetAdd(previewRow);

		// Expanding spacer to push content up
		final Spacer expandSpacer = new Spacer();
		expandSpacer.setPropertyExpand(Vector2b.TRUE);
		controls.subWidgetAdd(expandSpacer);

		return controls;
	}

	private Sizer buildSliderRow(final String labelText, final Slider slider) {
		final Sizer row = new Sizer(DisplayMode.HORIZONTAL);
		row.setPropertyExpand(Vector2b.TRUE_FALSE);
		row.setPropertyFill(Vector2b.TRUE_FALSE);
		final Label label = new Label(labelText);
		label.setPropertyMinSize(new Dimension2f(new Vector2f(25, 25), Distance.PIXEL));
		label.setPropertyExpand(Vector2b.FALSE);
		row.subWidgetAdd(label);
		row.subWidgetAdd(slider);
		return row;
	}

	private Box buildFooter() {
		final Box footerBox = new Box();
		footerBox.setPropertyColor(COLOR_HEADER_FOOTER);
		footerBox.setPropertyBorderRadius(new DimensionBorderRadius(0, 0, 8, 8));
		footerBox.setPropertyPadding(new DimensionInsets(10));
		footerBox.setPropertyExpand(Vector2b.TRUE_FALSE);
		footerBox.setPropertyFill(Vector2b.TRUE_FALSE);

		final Sizer footerSizer = new Sizer(DisplayMode.HORIZONTAL);
		footerSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
		footerSizer.setPropertyFill(Vector2b.TRUE_FALSE);
		footerBox.setSubWidget(footerSizer);

		footerSizer.subWidgetAdd(this.selectButton);
		footerSizer.subWidgetAdd(createHorizontalSpacer(10));
		footerSizer.subWidgetAdd(this.cancelButton);

		return footerBox;
	}

	private Sizer buildButtonContent(final String iconName, final Color iconColor, final Label label) {
		final Sizer sizer = new Sizer(DisplayMode.HORIZONTAL);

		final Icon icon = Icon.create(iconName);
		icon.setPropertyFillColor(iconColor);
		icon.setPropertyIconSize(new Dimension2f(new Vector2f(20, 20), Distance.PIXEL));
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

	static void onGradientColorChanged(final ColorPickerPopup self, final Color color) {
		if (self.updating) {
			return;
		}
		self.red = (int) (color.r() * 255);
		self.green = (int) (color.g() * 255);
		self.blue = (int) (color.b() * 255);
		// Full update (sliders + hex + preview) on every color change
		self.updateUIFromRGB();
		self.emitColorChanged();
	}

	static void onRedChanged(final ColorPickerPopup self, final Float value) {
		if (self.updating) {
			return;
		}
		self.red = Math.round(value);
		self.updateUIFromRGBKeepGradient();
		self.emitColorChanged();
	}

	static void onGreenChanged(final ColorPickerPopup self, final Float value) {
		if (self.updating) {
			return;
		}
		self.green = Math.round(value);
		self.updateUIFromRGBKeepGradient();
		self.emitColorChanged();
	}

	static void onBlueChanged(final ColorPickerPopup self, final Float value) {
		if (self.updating) {
			return;
		}
		self.blue = Math.round(value);
		self.updateUIFromRGBKeepGradient();
		self.emitColorChanged();
	}

	static void onAlphaChanged(final ColorPickerPopup self, final Float value) {
		if (self.updating) {
			return;
		}
		self.alpha = Math.round(value);
		self.updateUIFromRGBKeepGradient();
		self.emitColorChanged();
	}

	static void onHexChanged(final ColorPickerPopup self, final String value) {
		if (self.updating || value == null || value.isEmpty()) {
			return;
		}
		try {
			final Color color = hexToColor(value);
			if (color != null) {
				self.setColorFromRGB(color);
				self.updateUIFromRGB();
				self.emitColorChanged();
			}
		} catch (final Exception e) {
			// Invalid hex, ignore
		}
	}

	static void onCancelClicked(final ColorPickerPopup self) {
		self.signalCancel.emit();
		self.autoDestroy();
	}

	static void onSelectClicked(final ColorPickerPopup self) {
		self.signalValidate.emit(self.getCurrentColor());
		self.autoDestroy();
	}

	// ========================================================================
	// UI Update methods
	// ========================================================================

	private void updateUIFromRGB() {
		this.updating = true;
		try {
			this.sliderRed.setPropertyValue((float) this.red);
			this.sliderGreen.setPropertyValue((float) this.green);
			this.sliderBlue.setPropertyValue((float) this.blue);
			this.sliderAlpha.setPropertyValue((float) this.alpha);
			this.entryHex.setPropertyValue(colorToHex(getCurrentColor()));
			this.previewCurrent.setPropertyColor(getCurrentColor());
			this.gradient.setFromColor(getCurrentColor());
		} finally {
			this.updating = false;
		}
	}

	private void updateUIFromRGBKeepGradient() {
		this.updating = true;
		try {
			this.sliderRed.setPropertyValue((float) this.red);
			this.sliderGreen.setPropertyValue((float) this.green);
			this.sliderBlue.setPropertyValue((float) this.blue);
			this.sliderAlpha.setPropertyValue((float) this.alpha);
			this.entryHex.setPropertyValue(colorToHex(getCurrentColor()));
			this.previewCurrent.setPropertyColor(getCurrentColor());
		} finally {
			this.updating = false;
		}
	}

	private void emitColorChanged() {
		this.propertyValue = getCurrentColor();
		this.signalColorChanged.emit(this.propertyValue);
	}

	// ========================================================================
	// Color conversion utilities
	// ========================================================================

	public Color getCurrentColor() {
		return new Color(this.red / 255.0f, this.green / 255.0f, this.blue / 255.0f, this.alpha / 255.0f);
	}

	private void setColorFromRGB(final Color color) {
		this.red = (int) (color.r() * 255);
		this.green = (int) (color.g() * 255);
		this.blue = (int) (color.b() * 255);
		this.alpha = (int) (color.a() * 255);
	}

	private String colorToHex(final Color color) {
		return String.format("#%02X%02X%02X%02X",
				(int) (color.r() * 255),
				(int) (color.g() * 255),
				(int) (color.b() * 255),
				(int) (color.a() * 255));
	}

	static Color hexToColor(final String hex) {
		if (hex == null || !hex.startsWith("#")) {
			return null;
		}
		final String h = hex.substring(1);
		if (h.length() != 6 && h.length() != 8) {
			return null;
		}

		final int r = Integer.parseInt(h.substring(0, 2), 16);
		final int g = Integer.parseInt(h.substring(2, 4), 16);
		final int b = Integer.parseInt(h.substring(4, 6), 16);
		final int a = h.length() == 8 ? Integer.parseInt(h.substring(6, 8), 16) : 255;

		return new Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}

	// ========================================================================
	// Property accessors
	// ========================================================================

	public Color getPropertyValue() {
		return this.propertyValue;
	}

	public void setPropertyValue(final Color value) {
		if (this.propertyValue.equals(value)) {
			return;
		}
		this.propertyValue = value;
		setColorFromRGB(value);
		updateUIFromRGB();
	}

	public String getPropertyLabelTitle() {
		return this.propertyLabelTitle;
	}

	public void setPropertyLabelTitle(final String title) {
		if (this.propertyLabelTitle.equals(title)) {
			return;
		}
		this.propertyLabelTitle = title;
		this.titleLabel.setPropertyValue(this.propertyLabelTitle);
	}

	public String getPropertyLabelSelect() {
		return this.propertyLabelSelect;
	}

	public void setPropertyLabelSelect(final String label) {
		if (this.propertyLabelSelect.equals(label)) {
			return;
		}
		this.propertyLabelSelect = label;
		this.selectLabelWidget.setPropertyValue(this.propertyLabelSelect);
	}

	public String getPropertyLabelCancel() {
		return this.propertyLabelCancel;
	}

	public void setPropertyLabelCancel(final String label) {
		if (this.propertyLabelCancel.equals(label)) {
			return;
		}
		this.propertyLabelCancel = label;
		this.cancelLabelWidget.setPropertyValue(this.propertyLabelCancel);
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	public static ColorPickerPopup create() {
		return new ColorPickerPopup();
	}

	public static ColorPickerPopup create(final Color color) {
		return new ColorPickerPopup(color);
	}

	public ColorPickerPopup value(final Color color) {
		setPropertyValue(color);
		return this;
	}

	public ColorPickerPopup title(final String title) {
		setPropertyLabelTitle(title);
		return this;
	}

	public ColorPickerPopup selectLabel(final String label) {
		setPropertyLabelSelect(label);
		return this;
	}

	public ColorPickerPopup cancelLabel(final String label) {
		setPropertyLabelCancel(label);
		return this;
	}

	public ColorPickerPopup onColorChanged(final java.util.function.Consumer<Color> callback) {
		this.signalColorChanged.connect(callback);
		return this;
	}

	public ColorPickerPopup onValidate(final java.util.function.Consumer<Color> callback) {
		this.signalValidate.connect(callback);
		return this;
	}

	public ColorPickerPopup onCancel(final Runnable callback) {
		this.signalCancel.connect(callback);
		return this;
	}
}
