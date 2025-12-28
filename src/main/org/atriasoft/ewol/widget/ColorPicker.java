/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.aknot.annotation.AknotSignal;
import org.atriasoft.esignal.Signal;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.DimensionInsets;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.gale.key.KeyStatus;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * ColorPicker widget that displays a color preview and opens a popup for color selection.
 *
 * Signals emitted:
 * - signalColorChanged: when the color is changed
 */
public class ColorPicker extends Box {
	private static final Logger LOGGER = LoggerFactory.getLogger(ColorPicker.class);

	@AknotSignal
	@AknotName(value = "color-changed")
	@AknotDescription("Color has been changed")
	public Signal<Color> signalColorChanged = new Signal<>();

	protected Color propertyValue = Color.WHITE;
	protected Spacer colorPreview;

	/**
	 * Default constructor.
	 */
	public ColorPicker() {
		this.propertyCanFocus = true;
		setMouseLimit(1);
		setPropertyExpand(Vector2b.FALSE);
		setPropertyFill(Vector2b.FALSE);
		setPropertyBorderWidth(new DimensionInsets(2));
		setPropertyBorderColor(Color.BLACK);
		setPropertyColor(Color.LIGHT_GRAY);
		setPropertyPadding(new DimensionInsets(2));
		setPropertyMargin(new DimensionInsets(2));
		setPropertyMinSize(new Dimension2f(new Vector2f(40, 30), Distance.PIXEL));

		// Create color preview
		this.colorPreview = new Spacer();
		this.colorPreview.setPropertyColor(this.propertyValue);
		this.colorPreview.setPropertyExpand(Vector2b.TRUE);
		this.colorPreview.setPropertyFill(Vector2b.TRUE);
		this.colorPreview.setPropertyMinSize(new Dimension2f(new Vector2f(30, 20), Distance.PIXEL));
		setSubWidget(this.colorPreview);
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "value")
	@AknotDescription(value = "Current color value")
	public Color getPropertyValue() {
		return this.propertyValue;
	}

	public void setPropertyValue(final Color value) {
		if (this.propertyValue.equals(value)) {
			return;
		}
		this.propertyValue = value;
		if (this.colorPreview != null) {
			this.colorPreview.setPropertyColor(value);
		}
		markToRedraw();
	}

	@Override
	public boolean onEventInput(final EventInput event) {
		final Vector2f relPos = relativePosition(event.pos());
		final boolean over = isInside(relPos);

		if (event.status() == KeyStatus.leave) {
			markToRedraw();
			return true;
		}

		if (event.inputId() != 1) {
			return over;
		}

		if (KeyStatus.pressSingle == event.status() && over) {
			keepFocus();
			openColorPickerPopup(event.pos());
			return true;
		}

		return over;
	}

	/**
	 * Opens the color picker popup at the specified position.
	 */
	protected void openColorPickerPopup(final Vector2f position) {
		final Windows windows = getWindows();
		if (windows == null) {
			LOGGER.error("Cannot open popup: no windows found");
			return;
		}

		// Create the popup content
		final ColorPickerPanel pickerPanel = new ColorPickerPanel(this.propertyValue);
		pickerPanel.signalColorChanged.connectAuto(this, (final ColorPicker self, final Color color) -> {
			self.setPropertyValue(color);
			self.signalColorChanged.emit(color);
		});
		pickerPanel.signalValidate.connectAuto(this, (final ColorPicker self, final Color color) -> {
			self.setPropertyValue(color);
			self.signalColorChanged.emit(color);
		});
		pickerPanel.signalCancel.connectAuto(this, (final ColorPicker self) -> {
			// Color already reverted in panel if needed
		});

		// Create popup
		final PopUp popup = PopUp.create()
				.closeOnOutside(true)
				.content(pickerPanel);
		popup.setPropertyExpand(Vector2b.FALSE);
		popup.setPropertyFill(Vector2b.FALSE);
		popup.setPropertyGravity(Gravity.CENTER);
		popup.setPropertyMinSize(new Dimension2f(new Vector2f(300, 350), Distance.PIXEL));

		// Store reference for closing
		pickerPanel.setPopup(popup);

		windows.popUpWidgetPush(popup);
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	/**
	 * Create a new ColorPicker.
	 * @return a new ColorPicker
	 */
	public static ColorPicker create() {
		return new ColorPicker();
	}

	/**
	 * Create a new ColorPicker with initial color.
	 * @param color initial color
	 * @return a new ColorPicker
	 */
	public static ColorPicker create(final Color color) {
		final ColorPicker picker = new ColorPicker();
		picker.setPropertyValue(color);
		return picker;
	}

	/**
	 * Fluent method to set color value.
	 * @param color the color
	 * @return this picker for chaining
	 */
	public ColorPicker value(final Color color) {
		setPropertyValue(color);
		return this;
	}

	/**
	 * Fluent method to connect a color changed callback.
	 * @param callback the callback to invoke when color changes
	 * @return this picker for chaining
	 */
	public ColorPicker onColorChanged(final java.util.function.Consumer<Color> callback) {
		this.signalColorChanged.connect(callback);
		return this;
	}
}
