/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget.meta;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlProperty;
import org.atriasoft.esignal.Signal;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.ColorGradient;
import org.atriasoft.ewol.widget.Composer;
import org.atriasoft.ewol.widget.Entry;
import org.atriasoft.ewol.widget.Label;
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
public class ColorPickerPopup extends Composer {
	private static final Logger LOGGER = LoggerFactory.getLogger(ColorPickerPopup.class);

	public Signal<Color> signalColorChanged = new Signal<>();

	public Signal<Color> signalValidate = new Signal<>();

	public SignalEmpty signalCancel = new SignalEmpty();

	// Properties
	protected Color propertyValue = Color.WHITE;
	protected Color originalColor = Color.WHITE;
	protected String propertyLabelTitle = "_T{ColorPicker}";
	protected String propertyLabelSelect = "_T{Select}";
	protected String propertyLabelCancel = "_T{Cancel}";

	// Current RGBA values (0-255)
	protected int red = 255;
	protected int green = 255;
	protected int blue = 255;
	protected int alpha = 255;

	// Flag to prevent recursive updates
	protected boolean updating = false;

	/**
	 * Default constructor.
	 */
	public ColorPickerPopup() {
		loadFromFile(new Uri("DATA", "ewol-gui-color-picker.xml", "ewol"));

		// Update labels
		onChangePropertyLabelTitle();
		onChangePropertyLabelSelect();
		onChangePropertyLabelCancel();

		// Connect signals
		connectSignals();

		setPropertyCanFocus(true);
		// Ensure the composer expands to fill the popup area
		setPropertyExpand(Vector2b.TRUE);
		setPropertyFill(Vector2b.TRUE);
	}

	/**
	 * Constructor with initial color.
	 */
	public ColorPickerPopup(final Color initialColor) {
		this();
		setPropertyValue(initialColor);
		this.originalColor = initialColor;
		updatePreviewOriginal();
	}

	/**
	 * Connect signal handlers to widgets.
	 */
	protected void connectSignals() {
		final String prefix = "[" + getId() + "]color-picker:";

		// Gradient
		if (getSubObjectNamed(prefix + "gradient") instanceof final ColorGradient gradient) {
			gradient.signalColorChanged.connectAuto(this, ColorPickerPopup::onGradientColorChanged);
		}

		// Sliders
		if (getSubObjectNamed(prefix + "slider-red") instanceof final Slider slider) {
			slider.signalValue.connectAuto(this, ColorPickerPopup::onRedChanged);
		}
		if (getSubObjectNamed(prefix + "slider-green") instanceof final Slider slider) {
			slider.signalValue.connectAuto(this, ColorPickerPopup::onGreenChanged);
		}
		if (getSubObjectNamed(prefix + "slider-blue") instanceof final Slider slider) {
			slider.signalValue.connectAuto(this, ColorPickerPopup::onBlueChanged);
		}
		if (getSubObjectNamed(prefix + "slider-alpha") instanceof final Slider slider) {
			slider.signalValue.connectAuto(this, ColorPickerPopup::onAlphaChanged);
		}

		// Hex entry
		if (getSubObjectNamed(prefix + "entry-hex") instanceof final Entry entry) {
			entry.signalModify.connectAuto(this, ColorPickerPopup::onHexChanged);
		}

		// Buttons
		if (getSubObjectNamed(prefix + "button-cancel") instanceof final Button button) {
			button.signalClick.connectAuto(this, ColorPickerPopup::onCancelClicked);
		}
		if (getSubObjectNamed(prefix + "button-select") instanceof final Button button) {
			button.signalClick.connectAuto(this, ColorPickerPopup::onSelectClicked);
		}
	}

	// ========================================================================
	// Signal callbacks (static to avoid GC issues with WeakReferences)
	// ========================================================================

	protected static void onGradientColorChanged(final ColorPickerPopup self, final Color color) {
		if (self.updating) {
			return;
		}
		self.red = (int) (color.r() * 255);
		self.green = (int) (color.g() * 255);
		self.blue = (int) (color.b() * 255);
		// Keep alpha unchanged
		self.updateUIFromRGB();
		self.emitColorChanged();
	}

	protected static void onRedChanged(final ColorPickerPopup self, final Float value) {
		if (self.updating) {
			return;
		}
		self.red = Math.round(value);
		self.updateUIFromRGBKeepGradient();
		self.emitColorChanged();
	}

	protected static void onGreenChanged(final ColorPickerPopup self, final Float value) {
		if (self.updating) {
			return;
		}
		self.green = Math.round(value);
		self.updateUIFromRGBKeepGradient();
		self.emitColorChanged();
	}

	protected static void onBlueChanged(final ColorPickerPopup self, final Float value) {
		if (self.updating) {
			return;
		}
		self.blue = Math.round(value);
		self.updateUIFromRGBKeepGradient();
		self.emitColorChanged();
	}

	protected static void onAlphaChanged(final ColorPickerPopup self, final Float value) {
		if (self.updating) {
			return;
		}
		self.alpha = Math.round(value);
		self.updateUIFromRGBKeepGradient();
		self.emitColorChanged();
	}

	protected static void onHexChanged(final ColorPickerPopup self, final String value) {
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

	protected static void onCancelClicked(final ColorPickerPopup self) {
		self.signalCancel.emit();
		self.autoDestroy();
	}

	protected static void onSelectClicked(final ColorPickerPopup self) {
		self.signalValidate.emit(self.getCurrentColor());
		self.autoDestroy();
	}

	// ========================================================================
	// UI Update methods
	// ========================================================================

	protected void updateUIFromRGB() {
		this.updating = true;
		try {
			final String prefix = "[" + getId() + "]color-picker:";

			// Update sliders
			if (getSubObjectNamed(prefix + "slider-red") instanceof final Slider slider) {
				slider.setPropertyValue((float) this.red);
			}
			if (getSubObjectNamed(prefix + "slider-green") instanceof final Slider slider) {
				slider.setPropertyValue((float) this.green);
			}
			if (getSubObjectNamed(prefix + "slider-blue") instanceof final Slider slider) {
				slider.setPropertyValue((float) this.blue);
			}
			if (getSubObjectNamed(prefix + "slider-alpha") instanceof final Slider slider) {
				slider.setPropertyValue((float) this.alpha);
			}

			// Update hex entry
			if (getSubObjectNamed(prefix + "entry-hex") instanceof final Entry entry) {
				entry.setPropertyValue(colorToHex(getCurrentColor()));
			}

			// Update preview
			if (getSubObjectNamed(prefix + "preview-current") instanceof final Spacer spacer) {
				spacer.setPropertyColor(getCurrentColor());
			}

			// Update gradient position
			if (getSubObjectNamed(prefix + "gradient") instanceof final ColorGradient gradient) {
				gradient.setFromColor(getCurrentColor());
			}
		} finally {
			this.updating = false;
		}
	}

	protected void updateUIFromRGBKeepGradient() {
		this.updating = true;
		try {
			final String prefix = "[" + getId() + "]color-picker:";

			// Update sliders
			if (getSubObjectNamed(prefix + "slider-red") instanceof final Slider slider) {
				slider.setPropertyValue((float) this.red);
			}
			if (getSubObjectNamed(prefix + "slider-green") instanceof final Slider slider) {
				slider.setPropertyValue((float) this.green);
			}
			if (getSubObjectNamed(prefix + "slider-blue") instanceof final Slider slider) {
				slider.setPropertyValue((float) this.blue);
			}
			if (getSubObjectNamed(prefix + "slider-alpha") instanceof final Slider slider) {
				slider.setPropertyValue((float) this.alpha);
			}

			// Update hex entry
			if (getSubObjectNamed(prefix + "entry-hex") instanceof final Entry entry) {
				entry.setPropertyValue(colorToHex(getCurrentColor()));
			}

			// Update preview
			if (getSubObjectNamed(prefix + "preview-current") instanceof final Spacer spacer) {
				spacer.setPropertyColor(getCurrentColor());
			}
		} finally {
			this.updating = false;
		}
	}

	protected void updatePreviewOriginal() {
		final String prefix = "[" + getId() + "]color-picker:";
		if (getSubObjectNamed(prefix + "preview-original") instanceof final Spacer spacer) {
			spacer.setPropertyColor(this.originalColor);
		}
	}

	protected void emitColorChanged() {
		this.propertyValue = getCurrentColor();
		this.signalColorChanged.emit(this.propertyValue);
	}

	// ========================================================================
	// Color conversion utilities
	// ========================================================================

	public Color getCurrentColor() {
		return new Color(this.red / 255.0f, this.green / 255.0f, this.blue / 255.0f, this.alpha / 255.0f);
	}

	protected void setColorFromRGB(final Color color) {
		this.red = (int) (color.r() * 255);
		this.green = (int) (color.g() * 255);
		this.blue = (int) (color.b() * 255);
		this.alpha = (int) (color.a() * 255);
	}

	protected String colorToHex(final Color color) {
		return String.format("#%02X%02X%02X%02X",
				(int) (color.r() * 255),
				(int) (color.g() * 255),
				(int) (color.b() * 255),
				(int) (color.a() * 255));
	}

	protected static Color hexToColor(final String hex) {
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

	@JsonProperty("value")
	@JacksonXmlProperty(isAttribute = true, localName = "value")
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

	@JsonProperty("title")
	@JacksonXmlProperty(isAttribute = true, localName = "title")
	public String getPropertyLabelTitle() {
		return this.propertyLabelTitle;
	}

	public void setPropertyLabelTitle(final String title) {
		if (this.propertyLabelTitle.equals(title)) {
			return;
		}
		this.propertyLabelTitle = title;
		onChangePropertyLabelTitle();
	}

	protected void onChangePropertyLabelTitle() {
		final String prefix = "[" + getId() + "]color-picker:";
		if (getSubObjectNamed(prefix + "title-label") instanceof final Label label) {
			label.setPropertyValue(this.propertyLabelTitle);
		}
	}

	@JsonProperty("label-select")
	@JacksonXmlProperty(isAttribute = true, localName = "label-select")
	public String getPropertyLabelSelect() {
		return this.propertyLabelSelect;
	}

	public void setPropertyLabelSelect(final String label) {
		if (this.propertyLabelSelect.equals(label)) {
			return;
		}
		this.propertyLabelSelect = label;
		onChangePropertyLabelSelect();
	}

	protected void onChangePropertyLabelSelect() {
		final String prefix = "[" + getId() + "]color-picker:";
		if (getSubObjectNamed(prefix + "select-label") instanceof final Label label) {
			label.setPropertyValue(this.propertyLabelSelect);
		}
	}

	@JsonProperty("label-cancel")
	@JacksonXmlProperty(isAttribute = true, localName = "label-cancel")
	public String getPropertyLabelCancel() {
		return this.propertyLabelCancel;
	}

	public void setPropertyLabelCancel(final String label) {
		if (this.propertyLabelCancel.equals(label)) {
			return;
		}
		this.propertyLabelCancel = label;
		onChangePropertyLabelCancel();
	}

	protected void onChangePropertyLabelCancel() {
		final String prefix = "[" + getId() + "]color-picker:";
		if (getSubObjectNamed(prefix + "cancel-label") instanceof final Label label) {
			label.setPropertyValue(this.propertyLabelCancel);
		}
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	/**
	 * Create a new ColorPickerPopup.
	 * @return a new ColorPickerPopup instance
	 */
	public static ColorPickerPopup create() {
		return new ColorPickerPopup();
	}

	/**
	 * Create a new ColorPickerPopup with initial color.
	 * @param color initial color
	 * @return a new ColorPickerPopup instance
	 */
	public static ColorPickerPopup create(final Color color) {
		return new ColorPickerPopup(color);
	}

	/**
	 * Fluent method to set color value.
	 * @param color the color
	 * @return this picker for chaining
	 */
	public ColorPickerPopup value(final Color color) {
		setPropertyValue(color);
		return this;
	}

	/**
	 * Fluent method to set title.
	 * @param title the title
	 * @return this picker for chaining
	 */
	public ColorPickerPopup title(final String title) {
		setPropertyLabelTitle(title);
		return this;
	}

	/**
	 * Fluent method to set select button label.
	 * @param label the label
	 * @return this picker for chaining
	 */
	public ColorPickerPopup selectLabel(final String label) {
		setPropertyLabelSelect(label);
		return this;
	}

	/**
	 * Fluent method to set cancel button label.
	 * @param label the label
	 * @return this picker for chaining
	 */
	public ColorPickerPopup cancelLabel(final String label) {
		setPropertyLabelCancel(label);
		return this;
	}

	/**
	 * Fluent method to connect a color changed callback.
	 * @param callback the callback to invoke when color changes
	 * @return this picker for chaining
	 */
	public ColorPickerPopup onColorChanged(final java.util.function.Consumer<Color> callback) {
		this.signalColorChanged.connect(callback);
		return this;
	}

	/**
	 * Fluent method to connect a validate callback.
	 * @param callback the callback to invoke when color is validated
	 * @return this picker for chaining
	 */
	public ColorPickerPopup onValidate(final java.util.function.Consumer<Color> callback) {
		this.signalValidate.connect(callback);
		return this;
	}

	/**
	 * Fluent method to connect a cancel callback.
	 * @param callback the callback to invoke when cancelled
	 * @return this picker for chaining
	 */
	public ColorPickerPopup onCancel(final Runnable callback) {
		this.signalCancel.connect(callback);
		return this;
	}
}
