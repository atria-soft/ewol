/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.esignal.Signal;
import org.atriasoft.esignal.SignalEmpty;
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
 */
public class ColorGradient extends Widget {
	private static final Logger LOGGER = LoggerFactory.getLogger(ColorGradient.class);

	private static final float DEFAULT_SIZE = 200.0f;
	private static final float CURSOR_SIZE = 12.0f;
	private static final int GRADIENT_STEPS = 40;

	public Signal<Color> signalColorChanged = new Signal<>();
	public SignalEmpty signalDragEnd = new SignalEmpty();

	// Static gradient background (only regenerated on resize)
	private final CompositingGC gradientBackground = new CompositingGC();
	// Dynamic cursor overlay (regenerated on each cursor move)
	private final CompositingGC cursorOverlay = new CompositingGC();

	// Cached size to detect when gradient needs regeneration
	private float cachedWidth = -1;
	private float cachedHeight = -1;

	protected boolean dragging = false;

	// Current HSL position (0.0 - 1.0)
	protected float hue = 0.0f;
	protected float lightness = 0.5f;
	protected int alpha = 255;

	public ColorGradient() {
		setMouseLimit(1);
		setPropertyExpand(Vector2b.FALSE);
		setPropertyFill(Vector2b.FALSE);
		setPropertyMinSize(new Dimension2f(new Vector2f(DEFAULT_SIZE, DEFAULT_SIZE), Distance.PIXEL));
	}

	public boolean isDragging() {
		return this.dragging;
	}

	public float getHue() {
		return this.hue;
	}

	public void setHue(final float hue) {
		this.hue = FMath.avg(0.0f, hue, 1.0f);
		markToRedraw();
	}

	public float getLightness() {
		return this.lightness;
	}

	public void setLightness(final float lightness) {
		this.lightness = FMath.avg(0.0f, lightness, 1.0f);
		markToRedraw();
	}

	public int getAlpha() {
		return this.alpha;
	}

	public void setAlpha(final int alpha) {
		this.alpha = FMath.clamp(alpha, 0, 255);
	}

	public void setFromColor(final Color color) {
		rgbToHsl(color);
		this.alpha = (int) (color.a() * 255);
		markToRedraw();
	}

	public Color getCurrentColor() {
		return hslToRgb(this.hue, 1.0f, this.lightness);
	}

	@Override
	public void onRegenerateDisplay() {
		if (!needRedraw()) {
			return;
		}

		final float width = this.size.x();
		final float height = this.size.y();

		// Only regenerate the gradient background if the size changed
		if (width != this.cachedWidth || height != this.cachedHeight) {
			regenerateGradientBackground(width, height);
			this.cachedWidth = width;
			this.cachedHeight = height;
		}

		// Always regenerate the cursor overlay (lightweight)
		regenerateCursorOverlay(width, height);
	}

	private void regenerateGradientBackground(final float width, final float height) {
		this.gradientBackground.clear();

		final float cellWidth = width / GRADIENT_STEPS;
		final float cellHeight = height / GRADIENT_STEPS;

		for (int x = 0; x < GRADIENT_STEPS; x++) {
			for (int y = 0; y < GRADIENT_STEPS; y++) {
				final float h = (float) x / GRADIENT_STEPS;
				final float l = 1.0f - (float) y / GRADIENT_STEPS;

				final Color color = hslToRgb(h, 1.0f, l);
				this.gradientBackground.setColor(color);
				this.gradientBackground.setPos(new Vector2f(x * cellWidth, y * cellHeight));
				this.gradientBackground.rectangleWidth(new Vector2f(cellWidth + 1, cellHeight + 1));
			}
		}

		// Border
		this.gradientBackground.setColor(Color.DARK_GRAY);
		this.gradientBackground.setPos(Vector2f.ZERO);
		this.gradientBackground.rectangleWidth(new Vector2f(width, 1));
		this.gradientBackground.setPos(new Vector2f(0, height - 1));
		this.gradientBackground.rectangleWidth(new Vector2f(width, 1));
		this.gradientBackground.setPos(Vector2f.ZERO);
		this.gradientBackground.rectangleWidth(new Vector2f(1, height));
		this.gradientBackground.setPos(new Vector2f(width - 1, 0));
		this.gradientBackground.rectangleWidth(new Vector2f(1, height));

		this.gradientBackground.flush();
	}

	private void regenerateCursorOverlay(final float width, final float height) {
		this.cursorOverlay.clear();

		final float cursorX = this.hue * width;
		final float cursorY = (1.0f - this.lightness) * height;

		// White outer ring
		this.cursorOverlay.setColor(Color.WHITE);
		this.cursorOverlay.setPos(new Vector2f(cursorX - CURSOR_SIZE / 2, cursorY - CURSOR_SIZE / 2));
		this.cursorOverlay.rectangleWidth(new Vector2f(CURSOR_SIZE, CURSOR_SIZE));

		// Black inner ring
		this.cursorOverlay.setColor(Color.BLACK);
		this.cursorOverlay.setPos(new Vector2f(cursorX - CURSOR_SIZE / 2 + 1, cursorY - CURSOR_SIZE / 2 + 1));
		this.cursorOverlay.rectangleWidth(new Vector2f(CURSOR_SIZE - 2, CURSOR_SIZE - 2));

		// Current color center
		this.cursorOverlay.setColor(getCurrentColor());
		this.cursorOverlay.setPos(new Vector2f(cursorX - CURSOR_SIZE / 2 + 2, cursorY - CURSOR_SIZE / 2 + 2));
		this.cursorOverlay.rectangleWidth(new Vector2f(CURSOR_SIZE - 4, CURSOR_SIZE - 4));

		this.cursorOverlay.flush();
	}

	@Override
	protected void onDraw() {
		this.gradientBackground.draw();
		this.cursorOverlay.draw();
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
				this.signalDragEnd.emit();
			}
			return true;
		}
		// Safety fallback: if leave fires during drag (e.g. window exit)
		if (event.status() == KeyStatus.leave) {
			if (this.dragging) {
				this.dragging = false;
				unGrabEvents();
				this.signalDragEnd.emit();
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

	public static ColorGradient create() {
		return new ColorGradient();
	}

	public static ColorGradient create(final Color color) {
		final ColorGradient gradient = new ColorGradient();
		gradient.setFromColor(color);
		return gradient;
	}
}
