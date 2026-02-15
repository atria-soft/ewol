/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.esignal.Signal;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.compositing.CompositingGC;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.gale.key.KeyStatus;
import org.atriasoft.gale.key.KeyType;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Widget that draws an HSL color gradient for color selection.
 *
 * The gradient displays:
 * - Horizontal axis: Hue (0° to 360°) - Red → Yellow → Green → Cyan → Blue → Magenta → Red
 * - Vertical axis: Lightness - White at top, Black at bottom
 *
 * Signals emitted:
 * - signalColorChanged: when a color is selected (emits the selected Color)
 * - signalHslChanged: when hue/lightness changes (emits hue, lightness as floats 0-1)
 */
public class ColorGradient extends Widget {
	private static final Logger LOGGER = LoggerFactory.getLogger(ColorGradient.class);

	private static final float DEFAULT_SIZE = 200.0f;
	private static final float CURSOR_SIZE = 12.0f;
	private static final int GRADIENT_STEPS = 40;

	public Signal<Color> signalColorChanged = new Signal<>();

	protected final CompositingGC compositing = new CompositingGC();
	protected boolean dragging = false;

	// Current HSL position (0.0 - 1.0)
	protected float hue = 0.0f;
	protected float lightness = 0.5f;
	protected int alpha = 255;

	/**
	 * Default constructor.
	 */
	public ColorGradient() {
		setMouseLimit(1);
		setPropertyExpand(Vector2b.FALSE);
		setPropertyFill(Vector2b.FALSE);
		setPropertyMinSize(new Dimension2f(new Vector2f(DEFAULT_SIZE, DEFAULT_SIZE), Distance.PIXEL));
	}

	/**
	 * Get the current hue value.
	 * @return hue value between 0.0 and 1.0
	 */
	public float getHue() {
		return this.hue;
	}

	/**
	 * Set the hue value.
	 * @param hue value between 0.0 and 1.0
	 */
	public void setHue(final float hue) {
		this.hue = FMath.avg(0.0f, hue, 1.0f);
		markToRedraw();
	}

	/**
	 * Get the current lightness value.
	 * @return lightness value between 0.0 and 1.0
	 */
	public float getLightness() {
		return this.lightness;
	}

	/**
	 * Set the lightness value.
	 * @param lightness value between 0.0 and 1.0
	 */
	public void setLightness(final float lightness) {
		this.lightness = FMath.avg(0.0f, lightness, 1.0f);
		markToRedraw();
	}

	/**
	 * Get the current alpha value.
	 * @return alpha value between 0 and 255
	 */
	public int getAlpha() {
		return this.alpha;
	}

	/**
	 * Set the alpha value.
	 * @param alpha value between 0 and 255
	 */
	public void setAlpha(final int alpha) {
		this.alpha = FMath.clamp(alpha, 0, 255);
	}

	/**
	 * Set the position from a color.
	 * @param color the color to set position from
	 */
	public void setFromColor(final Color color) {
		rgbToHsl(color);
		this.alpha = (int) (color.a() * 255);
		markToRedraw();
	}

	/**
	 * Get the currently selected color.
	 * @return the current color
	 */
	public Color getCurrentColor() {
		return hslToRgb(this.hue, 1.0f, this.lightness);
	}

	@Override
	public void onRegenerateDisplay() {
		if (!needRedraw()) {
			return;
		}

		this.compositing.clear();

		final float width = this.size.x();
		final float height = this.size.y();

		// Draw gradient using cells
		final float cellWidth = width / GRADIENT_STEPS;
		final float cellHeight = height / GRADIENT_STEPS;

		for (int x = 0; x < GRADIENT_STEPS; x++) {
			for (int y = 0; y < GRADIENT_STEPS; y++) {
				final float h = (float) x / GRADIENT_STEPS;
				final float l = 1.0f - (float) y / GRADIENT_STEPS;

				final Color color = hslToRgb(h, 1.0f, l);
				this.compositing.setColor(color);
				this.compositing.setPos(new Vector2f(x * cellWidth, y * cellHeight));
				this.compositing.rectangleWidth(new Vector2f(cellWidth + 1, cellHeight + 1));
			}
		}

		// Draw border
		this.compositing.setColor(Color.DARK_GRAY);
		this.compositing.setPos(Vector2f.ZERO);
		this.compositing.rectangleWidth(new Vector2f(width, 1));
		this.compositing.setPos(new Vector2f(0, height - 1));
		this.compositing.rectangleWidth(new Vector2f(width, 1));
		this.compositing.setPos(Vector2f.ZERO);
		this.compositing.rectangleWidth(new Vector2f(1, height));
		this.compositing.setPos(new Vector2f(width - 1, 0));
		this.compositing.rectangleWidth(new Vector2f(1, height));

		// Draw cursor
		final float cursorX = this.hue * width;
		final float cursorY = (1.0f - this.lightness) * height;

		// White outer ring
		this.compositing.setColor(Color.WHITE);
		this.compositing.setPos(new Vector2f(cursorX - CURSOR_SIZE / 2, cursorY - CURSOR_SIZE / 2));
		this.compositing.rectangleWidth(new Vector2f(CURSOR_SIZE, CURSOR_SIZE));

		// Black inner ring
		this.compositing.setColor(Color.BLACK);
		this.compositing.setPos(new Vector2f(cursorX - CURSOR_SIZE / 2 + 1, cursorY - CURSOR_SIZE / 2 + 1));
		this.compositing.rectangleWidth(new Vector2f(CURSOR_SIZE - 2, CURSOR_SIZE - 2));

		// Current color center
		this.compositing.setColor(getCurrentColor());
		this.compositing.setPos(new Vector2f(cursorX - CURSOR_SIZE / 2 + 2, cursorY - CURSOR_SIZE / 2 + 2));
		this.compositing.rectangleWidth(new Vector2f(CURSOR_SIZE - 4, CURSOR_SIZE - 4));

		this.compositing.flush();
	}

	@Override
	protected void onDraw() {
		this.compositing.draw();
	}

	@Override
	public boolean onEventInput(final EventInput event) {
		if (event.type() != KeyType.mouse) {
			return false;
		}

		final Vector2f relPos = relativePosition(event.pos());

		// Handle drag end
		if (event.status() == KeyStatus.up || event.status() == KeyStatus.upAfter) {
			if (this.dragging) {
				this.dragging = false;
				unGrabEvents();
			}
			return true;
		}
		// Safety fallback: if leave fires during drag (e.g. window exit)
		if (event.status() == KeyStatus.leave) {
			if (this.dragging) {
				this.dragging = false;
				unGrabEvents();
			}
			return true;
		}

		// Handle click/drag
		if (event.inputId() == 1) {
			if (event.status() == KeyStatus.down) {
				this.dragging = true;
				grabEvents();
				updateFromPosition(relPos);
				return true;
			}
			if (event.status() == KeyStatus.move && this.dragging) {
				updateFromPosition(relPos);
				return true;
			}
		}

		return isInsideBounds(relPos);
	}

	protected boolean isInsideBounds(final Vector2f pos) {
		return pos.x() >= 0 && pos.x() < this.size.x() && pos.y() >= 0 && pos.y() < this.size.y();
	}

	protected void updateFromPosition(final Vector2f pos) {
		this.hue = FMath.avg(0.0f, pos.x() / this.size.x(), 1.0f);
		this.lightness = FMath.avg(0.0f, 1.0f - pos.y() / this.size.y(), 1.0f);
		markToRedraw();
		this.signalColorChanged.emit(getCurrentColor());
	}

	/**
	 * Convert RGB color to HSL values (updates hue and lightness fields).
	 */
	protected void rgbToHsl(final Color color) {
		final float r = color.r();
		final float g = color.g();
		final float b = color.b();

		final float max = Math.max(r, Math.max(g, b));
		final float min = Math.min(r, Math.min(g, b));
		final float l = (max + min) / 2.0f;

		if (max == min) {
			this.hue = 0;
		} else {
			final float d = max - min;
			if (max == r) {
				this.hue = ((g - b) / d + (g < b ? 6 : 0)) / 6.0f;
			} else if (max == g) {
				this.hue = ((b - r) / d + 2) / 6.0f;
			} else {
				this.hue = ((r - g) / d + 4) / 6.0f;
			}
		}
		this.lightness = l;
	}

	/**
	 * Convert HSL to RGB color.
	 */
	protected Color hslToRgb(final float h, final float s, final float l) {
		float r, g, b;

		if (s == 0) {
			r = g = b = l;
		} else {
			final float q = l < 0.5f ? l * (1 + s) : l + s - l * s;
			final float p = 2 * l - q;
			r = hueToRgb(p, q, h + 1.0f / 3.0f);
			g = hueToRgb(p, q, h);
			b = hueToRgb(p, q, h - 1.0f / 3.0f);
		}

		return new Color(r, g, b, this.alpha / 255.0f);
	}

	protected float hueToRgb(final float p, final float q, float t) {
		if (t < 0) {
			t += 1;
		}
		if (t > 1) {
			t -= 1;
		}
		if (t < 1.0f / 6.0f) {
			return p + (q - p) * 6 * t;
		}
		if (t < 1.0f / 2.0f) {
			return q;
		}
		if (t < 2.0f / 3.0f) {
			return p + (q - p) * (2.0f / 3.0f - t) * 6;
		}
		return p;
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	/**
	 * Create a new ColorGradient.
	 * @return a new ColorGradient
	 */
	public static ColorGradient create() {
		return new ColorGradient();
	}

	/**
	 * Create a new ColorGradient with initial color.
	 * @param color initial color to position cursor
	 * @return a new ColorGradient
	 */
	public static ColorGradient create(final Color color) {
		final ColorGradient gradient = new ColorGradient();
		gradient.setFromColor(color);
		return gradient;
	}
}
