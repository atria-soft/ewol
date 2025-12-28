/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.aknot.annotation.AknotSignal;
import org.atriasoft.esignal.Signal;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.DimensionInsets;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.compositing.CompositingGC;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.gale.key.KeyStatus;
import org.atriasoft.gale.key.KeyType;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Color picker panel with HSV selection.
 * Contains:
 * - A saturation/value gradient area (main selector)
 * - A hue bar (vertical strip on the right)
 * - An alpha bar (optional, below the gradient)
 * - A preview area showing current color
 * - OK/Cancel buttons
 */
public class ColorPickerPanel extends Box {
	private static final Logger LOGGER = LoggerFactory.getLogger(ColorPickerPanel.class);

	private static final float HUE_BAR_WIDTH = 25.0f;
	private static final float ALPHA_BAR_HEIGHT = 20.0f;
	private static final float PREVIEW_HEIGHT = 40.0f;
	private static final float BUTTON_HEIGHT = 30.0f;
	private static final float PADDING = 8.0f;
	private static final float CURSOR_SIZE = 10.0f;

	@AknotSignal
	@AknotName(value = "color-changed")
	@AknotDescription("Color has been changed (live update)")
	public Signal<Color> signalColorChanged = new Signal<>();

	@AknotSignal
	@AknotName(value = "validate")
	@AknotDescription("Color selection validated")
	public Signal<Color> signalValidate = new Signal<>();

	@AknotSignal
	@AknotName(value = "cancel")
	@AknotDescription("Color selection cancelled")
	public SignalEmpty signalCancel = new SignalEmpty();

	// HSV components (0-1 range)
	protected float hue = 0.0f;
	protected float saturation = 1.0f;
	protected float value = 1.0f;
	protected float alpha = 1.0f;

	// Original color for cancel
	protected Color originalColor;

	// Drawing
	protected CompositingGC compositing = new CompositingGC();

	// Interaction state
	protected boolean draggingSV = false;
	protected boolean draggingHue = false;
	protected boolean draggingAlpha = false;

	// Popup reference for closing
	protected PopUp popup;

	// Calculated areas (in local coordinates)
	protected Vector2f svAreaStart = Vector2f.ZERO;
	protected Vector2f svAreaEnd = Vector2f.ZERO;
	protected Vector2f hueBarStart = Vector2f.ZERO;
	protected Vector2f hueBarEnd = Vector2f.ZERO;
	protected Vector2f alphaBarStart = Vector2f.ZERO;
	protected Vector2f alphaBarEnd = Vector2f.ZERO;
	protected Vector2f previewStart = Vector2f.ZERO;
	protected Vector2f previewEnd = Vector2f.ZERO;

	/**
	 * Constructor with initial color.
	 */
	public ColorPickerPanel(final Color initialColor) {
		this.originalColor = initialColor;
		setColorFromRGB(initialColor);

		setPropertyExpand(Vector2b.TRUE);
		setPropertyFill(Vector2b.TRUE);
		setPropertyBorderWidth(new DimensionInsets(1));
		setPropertyBorderColor(Color.DARK_GRAY);
		setPropertyColor(new Color(0.2f, 0.2f, 0.2f, 1.0f));
		setPropertyPadding(new DimensionInsets(PADDING));
		setPropertyMinSize(new Dimension2f(new Vector2f(280, 320), Distance.PIXEL));
	}

	/**
	 * Set the popup reference for closing.
	 */
	public void setPopup(final PopUp popup) {
		this.popup = popup;
	}

	/**
	 * Convert RGB to HSV and set internal state.
	 */
	protected void setColorFromRGB(final Color color) {
		final float r = color.r();
		final float g = color.g();
		final float b = color.b();
		this.alpha = color.a();

		final float max = Math.max(r, Math.max(g, b));
		final float min = Math.min(r, Math.min(g, b));
		final float delta = max - min;

		// Value
		this.value = max;

		// Saturation
		if (max > 0) {
			this.saturation = delta / max;
		} else {
			this.saturation = 0;
		}

		// Hue
		if (delta == 0) {
			this.hue = 0;
		} else if (max == r) {
			this.hue = ((g - b) / delta) / 6.0f;
			if (this.hue < 0) {
				this.hue += 1.0f;
			}
		} else if (max == g) {
			this.hue = (2.0f + (b - r) / delta) / 6.0f;
		} else {
			this.hue = (4.0f + (r - g) / delta) / 6.0f;
		}
	}

	/**
	 * Convert HSV to RGB Color.
	 */
	protected Color hsvToRgb(final float h, final float s, final float v, final float a) {
		if (s == 0) {
			return new Color(v, v, v, a);
		}

		final float hue6 = h * 6.0f;
		final int sector = (int) hue6;
		final float f = hue6 - sector;
		final float p = v * (1 - s);
		final float q = v * (1 - s * f);
		final float t = v * (1 - s * (1 - f));

		return switch (sector % 6) {
			case 0 -> new Color(v, t, p, a);
			case 1 -> new Color(q, v, p, a);
			case 2 -> new Color(p, v, t, a);
			case 3 -> new Color(p, q, v, a);
			case 4 -> new Color(t, p, v, a);
			case 5 -> new Color(v, p, q, a);
			default -> new Color(v, v, v, a);
		};
	}

	/**
	 * Get current color.
	 */
	public Color getCurrentColor() {
		return hsvToRgb(this.hue, this.saturation, this.value, this.alpha);
	}

	/**
	 * Calculate areas based on current size.
	 */
	protected void calculateAreas() {
		final float padding = PADDING;
		final float availableWidth = this.size.x() - 2 * padding;
		final float availableHeight = this.size.y() - 2 * padding;

		// Button area at bottom
		final float buttonY = padding;
		final float buttonTop = buttonY + BUTTON_HEIGHT;

		// Preview above buttons
		final float previewY = buttonTop + padding;
		final float previewTop = previewY + PREVIEW_HEIGHT;

		// Alpha bar above preview
		final float alphaY = previewTop + padding;
		final float alphaTop = alphaY + ALPHA_BAR_HEIGHT;

		// SV area and Hue bar share remaining space
		final float svBottom = alphaTop + padding;
		final float svTop = this.size.y() - padding;
		final float svRight = this.size.x() - padding - HUE_BAR_WIDTH - padding;

		// Saturation/Value area
		this.svAreaStart = new Vector2f(padding, svBottom);
		this.svAreaEnd = new Vector2f(svRight, svTop);

		// Hue bar (right side)
		this.hueBarStart = new Vector2f(svRight + padding, svBottom);
		this.hueBarEnd = new Vector2f(this.size.x() - padding, svTop);

		// Alpha bar
		this.alphaBarStart = new Vector2f(padding, alphaY);
		this.alphaBarEnd = new Vector2f(this.size.x() - padding, alphaTop);

		// Preview area
		this.previewStart = new Vector2f(padding, previewY);
		this.previewEnd = new Vector2f(this.size.x() - padding, previewTop);
	}

	@Override
	public void onChangeSize() {
		super.onChangeSize();
		calculateAreas();
	}

	@Override
	public boolean onEventInput(final EventInput event) {
		if (event.type() != KeyType.mouse) {
			return false;
		}

		final Vector2f relPos = relativePosition(event.pos());

		// Handle drag end
		if (event.status() == KeyStatus.up || event.status() == KeyStatus.upAfter || event.status() == KeyStatus.leave) {
			this.draggingSV = false;
			this.draggingHue = false;
			this.draggingAlpha = false;
			return true;
		}

		// Handle dragging
		if (event.status() == KeyStatus.move && event.inputId() == 1) {
			if (this.draggingSV) {
				updateSV(relPos);
				return true;
			}
			if (this.draggingHue) {
				updateHue(relPos);
				return true;
			}
			if (this.draggingAlpha) {
				updateAlpha(relPos);
				return true;
			}
		}

		// Handle click start
		if (event.inputId() == 1 && event.status() == KeyStatus.down) {
			// Check SV area
			if (isInArea(relPos, this.svAreaStart, this.svAreaEnd)) {
				this.draggingSV = true;
				updateSV(relPos);
				return true;
			}

			// Check Hue bar
			if (isInArea(relPos, this.hueBarStart, this.hueBarEnd)) {
				this.draggingHue = true;
				updateHue(relPos);
				return true;
			}

			// Check Alpha bar
			if (isInArea(relPos, this.alphaBarStart, this.alphaBarEnd)) {
				this.draggingAlpha = true;
				updateAlpha(relPos);
				return true;
			}

			// Check OK button (left half of button area)
			final float buttonY = PADDING;
			final float buttonTop = buttonY + BUTTON_HEIGHT;
			final float buttonMid = this.size.x() / 2;
			if (relPos.y() >= buttonY && relPos.y() <= buttonTop) {
				if (relPos.x() < buttonMid) {
					// OK button
					onOkClicked();
					return true;
				} else {
					// Cancel button
					onCancelClicked();
					return true;
				}
			}
		}

		return isInside(relPos);
	}

	protected boolean isInArea(final Vector2f pos, final Vector2f start, final Vector2f end) {
		return pos.x() >= start.x() && pos.x() <= end.x() && pos.y() >= start.y() && pos.y() <= end.y();
	}

	protected void updateSV(final Vector2f pos) {
		final float width = this.svAreaEnd.x() - this.svAreaStart.x();
		final float height = this.svAreaEnd.y() - this.svAreaStart.y();

		this.saturation = FMath.avg(0.0f, (pos.x() - this.svAreaStart.x()) / width, 1.0f);
		this.value = FMath.avg(0.0f, (pos.y() - this.svAreaStart.y()) / height, 1.0f);

		markToRedraw();
		this.signalColorChanged.emit(getCurrentColor());
	}

	protected void updateHue(final Vector2f pos) {
		final float height = this.hueBarEnd.y() - this.hueBarStart.y();
		this.hue = FMath.avg(0.0f, (pos.y() - this.hueBarStart.y()) / height, 1.0f);

		markToRedraw();
		this.signalColorChanged.emit(getCurrentColor());
	}

	protected void updateAlpha(final Vector2f pos) {
		final float width = this.alphaBarEnd.x() - this.alphaBarStart.x();
		this.alpha = FMath.avg(0.0f, (pos.x() - this.alphaBarStart.x()) / width, 1.0f);

		markToRedraw();
		this.signalColorChanged.emit(getCurrentColor());
	}

	protected void onOkClicked() {
		this.signalValidate.emit(getCurrentColor());
		closePopup();
	}

	protected void onCancelClicked() {
		this.signalCancel.emit();
		closePopup();
	}

	protected void closePopup() {
		if (this.popup != null) {
			final Windows windows = getWindows();
			if (windows != null) {
				windows.popUpWidgetPop();
			}
		}
	}

	@Override
	public void onRegenerateDisplay() {
		if (!needRedraw()) {
			return;
		}

		this.compositing.clear();

		// Draw background
		this.compositing.setColor(new Color(0.25f, 0.25f, 0.25f, 1.0f));
		this.compositing.setPos(Vector2f.ZERO);
		this.compositing.rectangleWidth(this.size);

		// Draw SV gradient area
		drawSVGradient();

		// Draw Hue bar
		drawHueBar();

		// Draw Alpha bar
		drawAlphaBar();

		// Draw preview
		drawPreview();

		// Draw buttons
		drawButtons();

		// Draw cursors
		drawCursors();

		this.compositing.flush();
	}

	protected void drawSVGradient() {
		// Draw base color at full saturation and value
		final Color baseColor = hsvToRgb(this.hue, 1.0f, 1.0f, 1.0f);

		// Draw the gradient using multiple rectangles for smooth appearance
		final int steps = 20;
		final float stepWidth = (this.svAreaEnd.x() - this.svAreaStart.x()) / steps;
		final float stepHeight = (this.svAreaEnd.y() - this.svAreaStart.y()) / steps;

		for (int x = 0; x < steps; x++) {
			for (int y = 0; y < steps; y++) {
				final float s = (x + 0.5f) / steps;
				final float v = (y + 0.5f) / steps;
				final Color cellColor = hsvToRgb(this.hue, s, v, 1.0f);

				this.compositing.setColor(cellColor);
				this.compositing.setPos(new Vector2f(this.svAreaStart.x() + x * stepWidth, this.svAreaStart.y() + y * stepHeight));
				this.compositing.rectangleWidth(new Vector2f(stepWidth + 1, stepHeight + 1));
			}
		}

		// Draw border
		this.compositing.setColor(Color.DARK_GRAY);
		this.compositing.setPos(this.svAreaStart);
		this.compositing.rectangleWidth(new Vector2f(this.svAreaEnd.x() - this.svAreaStart.x(), 1));
		this.compositing.setPos(new Vector2f(this.svAreaStart.x(), this.svAreaEnd.y() - 1));
		this.compositing.rectangleWidth(new Vector2f(this.svAreaEnd.x() - this.svAreaStart.x(), 1));
		this.compositing.setPos(this.svAreaStart);
		this.compositing.rectangleWidth(new Vector2f(1, this.svAreaEnd.y() - this.svAreaStart.y()));
		this.compositing.setPos(new Vector2f(this.svAreaEnd.x() - 1, this.svAreaStart.y()));
		this.compositing.rectangleWidth(new Vector2f(1, this.svAreaEnd.y() - this.svAreaStart.y()));
	}

	protected void drawHueBar() {
		final int steps = 30;
		final float stepHeight = (this.hueBarEnd.y() - this.hueBarStart.y()) / steps;
		final float width = this.hueBarEnd.x() - this.hueBarStart.x();

		for (int i = 0; i < steps; i++) {
			final float h = (float) i / steps;
			final Color hueColor = hsvToRgb(h, 1.0f, 1.0f, 1.0f);

			this.compositing.setColor(hueColor);
			this.compositing.setPos(new Vector2f(this.hueBarStart.x(), this.hueBarStart.y() + i * stepHeight));
			this.compositing.rectangleWidth(new Vector2f(width, stepHeight + 1));
		}

		// Draw border
		this.compositing.setColor(Color.DARK_GRAY);
		this.compositing.setPos(this.hueBarStart);
		this.compositing.rectangleWidth(new Vector2f(width, 1));
		this.compositing.setPos(new Vector2f(this.hueBarStart.x(), this.hueBarEnd.y() - 1));
		this.compositing.rectangleWidth(new Vector2f(width, 1));
	}

	protected void drawAlphaBar() {
		// Draw checkered background for transparency
		final float checkerSize = 8;
		final float width = this.alphaBarEnd.x() - this.alphaBarStart.x();
		final float height = this.alphaBarEnd.y() - this.alphaBarStart.y();

		for (float x = 0; x < width; x += checkerSize) {
			for (float y = 0; y < height; y += checkerSize) {
				final boolean light = ((int) (x / checkerSize) + (int) (y / checkerSize)) % 2 == 0;
				this.compositing.setColor(light ? Color.WHITE : Color.LIGHT_GRAY);
				this.compositing.setPos(new Vector2f(this.alphaBarStart.x() + x, this.alphaBarStart.y() + y));
				this.compositing.rectangleWidth(new Vector2f(Math.min(checkerSize, width - x), Math.min(checkerSize, height - y)));
			}
		}

		// Draw alpha gradient
		final int steps = 20;
		final float stepWidth = width / steps;
		final Color baseColor = hsvToRgb(this.hue, this.saturation, this.value, 1.0f);

		for (int i = 0; i < steps; i++) {
			final float a = (float) i / steps;
			final Color alphaColor = new Color(baseColor.r(), baseColor.g(), baseColor.b(), a);

			this.compositing.setColor(alphaColor);
			this.compositing.setPos(new Vector2f(this.alphaBarStart.x() + i * stepWidth, this.alphaBarStart.y()));
			this.compositing.rectangleWidth(new Vector2f(stepWidth + 1, height));
		}

		// Draw border
		this.compositing.setColor(Color.DARK_GRAY);
		this.compositing.setPos(this.alphaBarStart);
		this.compositing.rectangleWidth(new Vector2f(width, 1));
		this.compositing.setPos(new Vector2f(this.alphaBarStart.x(), this.alphaBarEnd.y() - 1));
		this.compositing.rectangleWidth(new Vector2f(width, 1));
	}

	protected void drawPreview() {
		final float width = this.previewEnd.x() - this.previewStart.x();
		final float height = this.previewEnd.y() - this.previewStart.y();

		// Draw checkered background
		final float checkerSize = 10;
		for (float x = 0; x < width; x += checkerSize) {
			for (float y = 0; y < height; y += checkerSize) {
				final boolean light = ((int) (x / checkerSize) + (int) (y / checkerSize)) % 2 == 0;
				this.compositing.setColor(light ? Color.WHITE : Color.LIGHT_GRAY);
				this.compositing.setPos(new Vector2f(this.previewStart.x() + x, this.previewStart.y() + y));
				this.compositing.rectangleWidth(new Vector2f(Math.min(checkerSize, width - x), Math.min(checkerSize, height - y)));
			}
		}

		// Draw current color on left half
		final Color currentColor = getCurrentColor();
		this.compositing.setColor(currentColor);
		this.compositing.setPos(this.previewStart);
		this.compositing.rectangleWidth(new Vector2f(width / 2, height));

		// Draw original color on right half
		this.compositing.setColor(this.originalColor);
		this.compositing.setPos(new Vector2f(this.previewStart.x() + width / 2, this.previewStart.y()));
		this.compositing.rectangleWidth(new Vector2f(width / 2, height));

		// Draw border
		this.compositing.setColor(Color.DARK_GRAY);
		this.compositing.setPos(this.previewStart);
		this.compositing.rectangleWidth(new Vector2f(width, 1));
		this.compositing.setPos(new Vector2f(this.previewStart.x(), this.previewEnd.y() - 1));
		this.compositing.rectangleWidth(new Vector2f(width, 1));
		this.compositing.setPos(this.previewStart);
		this.compositing.rectangleWidth(new Vector2f(1, height));
		this.compositing.setPos(new Vector2f(this.previewEnd.x() - 1, this.previewStart.y()));
		this.compositing.rectangleWidth(new Vector2f(1, height));
		// Separator
		this.compositing.setPos(new Vector2f(this.previewStart.x() + width / 2, this.previewStart.y()));
		this.compositing.rectangleWidth(new Vector2f(1, height));
	}

	protected void drawButtons() {
		final float buttonY = PADDING;
		final float buttonWidth = (this.size.x() - 3 * PADDING) / 2;

		// OK button
		this.compositing.setColor(new Color(0.3f, 0.6f, 0.3f, 1.0f));
		this.compositing.setPos(new Vector2f(PADDING, buttonY));
		this.compositing.rectangleWidth(new Vector2f(buttonWidth, BUTTON_HEIGHT));

		// Cancel button
		this.compositing.setColor(new Color(0.6f, 0.3f, 0.3f, 1.0f));
		this.compositing.setPos(new Vector2f(PADDING * 2 + buttonWidth, buttonY));
		this.compositing.rectangleWidth(new Vector2f(buttonWidth, BUTTON_HEIGHT));
	}

	protected void drawCursors() {
		// SV cursor
		final float svWidth = this.svAreaEnd.x() - this.svAreaStart.x();
		final float svHeight = this.svAreaEnd.y() - this.svAreaStart.y();
		final float svCursorX = this.svAreaStart.x() + this.saturation * svWidth;
		final float svCursorY = this.svAreaStart.y() + this.value * svHeight;

		// Draw circle cursor for SV
		this.compositing.setColor(Color.WHITE);
		this.compositing.setPos(new Vector2f(svCursorX - CURSOR_SIZE / 2, svCursorY - CURSOR_SIZE / 2));
		this.compositing.rectangleWidth(new Vector2f(CURSOR_SIZE, CURSOR_SIZE));
		this.compositing.setColor(Color.BLACK);
		this.compositing.setPos(new Vector2f(svCursorX - CURSOR_SIZE / 2 + 1, svCursorY - CURSOR_SIZE / 2 + 1));
		this.compositing.rectangleWidth(new Vector2f(CURSOR_SIZE - 2, CURSOR_SIZE - 2));
		this.compositing.setColor(getCurrentColor());
		this.compositing.setPos(new Vector2f(svCursorX - CURSOR_SIZE / 2 + 2, svCursorY - CURSOR_SIZE / 2 + 2));
		this.compositing.rectangleWidth(new Vector2f(CURSOR_SIZE - 4, CURSOR_SIZE - 4));

		// Hue cursor
		final float hueHeight = this.hueBarEnd.y() - this.hueBarStart.y();
		final float hueCursorY = this.hueBarStart.y() + this.hue * hueHeight;
		final float hueBarWidth = this.hueBarEnd.x() - this.hueBarStart.x();

		this.compositing.setColor(Color.WHITE);
		this.compositing.setPos(new Vector2f(this.hueBarStart.x() - 2, hueCursorY - 3));
		this.compositing.rectangleWidth(new Vector2f(hueBarWidth + 4, 6));
		this.compositing.setColor(Color.BLACK);
		this.compositing.setPos(new Vector2f(this.hueBarStart.x() - 1, hueCursorY - 2));
		this.compositing.rectangleWidth(new Vector2f(hueBarWidth + 2, 4));

		// Alpha cursor
		final float alphaWidth = this.alphaBarEnd.x() - this.alphaBarStart.x();
		final float alphaCursorX = this.alphaBarStart.x() + this.alpha * alphaWidth;
		final float alphaBarHeight = this.alphaBarEnd.y() - this.alphaBarStart.y();

		this.compositing.setColor(Color.WHITE);
		this.compositing.setPos(new Vector2f(alphaCursorX - 3, this.alphaBarStart.y() - 2));
		this.compositing.rectangleWidth(new Vector2f(6, alphaBarHeight + 4));
		this.compositing.setColor(Color.BLACK);
		this.compositing.setPos(new Vector2f(alphaCursorX - 2, this.alphaBarStart.y() - 1));
		this.compositing.rectangleWidth(new Vector2f(4, alphaBarHeight + 2));
	}

	@Override
	protected void onDraw() {
		if (this.compositing != null) {
			this.compositing.draw();
		}
		super.onDraw();
	}
}
