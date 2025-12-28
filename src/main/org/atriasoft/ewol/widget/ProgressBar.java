package org.atriasoft.ewol.widget;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.aknot.annotation.AknotSignal;
import org.atriasoft.esignal.Signal;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.compositing.CompositingDrawing;
import org.atriasoft.ewol.compositing.CompositingGC;

/**
 * ProgressBar widget displaying a progress value between 0 and 1.
 *
 * Signals emitted:
 * - signalValue: when the progress value changes
 */
public class ProgressBar extends Widget {
	private static final int DOT_RADIUS = 6;
	private static final int PADDING = 5;

	private final CompositingDrawing vectorialDraw = new CompositingGC();

	@AknotSignal
	@AknotName("value")
	@AknotDescription("Progress bar value changed")
	public Signal<Float> signalValue = new Signal<>();

	private Color propertyColorOff = Color.NONE;
	private Color propertyColorOn = Color.GREEN;
	private Color propertyColorBorder = Color.BLACK;
	private float propertyValue = 0.0f;

	/**
	 * Default constructor.
	 */
	public ProgressBar() {
		setPropertyCanFocus(false);
	}

	@Override
	public void calculateMinMaxSize() {
		final Vector2f tmpMin = this.propertyMinSize.getPixel();
		this.minSize = new Vector2f(
				Math.max(tmpMin.x(), 40.0f),
				Math.max(tmpMin.y(), ProgressBar.DOT_RADIUS * 2.0f));
		markToRedraw();
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "color-off")
	@AknotDescription(value = "Color of the unfilled portion")
	public Color getPropertyColorOff() {
		return this.propertyColorOff;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "color-on")
	@AknotDescription(value = "Color of the filled portion")
	public Color getPropertyColorOn() {
		return this.propertyColorOn;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "color-border")
	@AknotDescription(value = "Border color")
	public Color getPropertyColorBorder() {
		return this.propertyColorBorder;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "value")
	@AknotDescription(value = "Progress value [0.0 - 1.0]")
	public float getPropertyValue() {
		return this.propertyValue;
	}

	@Override
	protected void onDraw() {
		this.vectorialDraw.draw();
	}

	@Override
	public void onRegenerateDisplay() {
		if (!needRedraw()) {
			return;
		}
		this.vectorialDraw.clear();

		final int barWidth = (int) (this.size.x() - PADDING * 2);
		final int barHeight = (int) (this.size.y() - PADDING * 2);
		final int filledWidth = (int) (barWidth * this.propertyValue);

		// Draw filled portion
		this.vectorialDraw.setColor(this.propertyColorOn);
		this.vectorialDraw.setPos(new Vector2f(PADDING, PADDING));
		this.vectorialDraw.rectangleWidth(new Vector2f(filledWidth, barHeight));

		// Draw unfilled portion
		this.vectorialDraw.setColor(this.propertyColorOff);
		this.vectorialDraw.setPos(new Vector2f(PADDING + filledWidth, PADDING));
		this.vectorialDraw.rectangleWidth(new Vector2f(barWidth - filledWidth, barHeight));
	}

	public void setPropertyColorOff(final Color propertyColorOff) {
		if (propertyColorOff.equals(this.propertyColorOff)) {
			return;
		}
		this.propertyColorOff = propertyColorOff;
		markToRedraw();
	}

	public void setPropertyColorOn(final Color propertyColorOn) {
		if (propertyColorOn.equals(this.propertyColorOn)) {
			return;
		}
		this.propertyColorOn = propertyColorOn;
		markToRedraw();
	}

	public void setPropertyColorBorder(final Color propertyColorBorder) {
		if (propertyColorBorder.equals(this.propertyColorBorder)) {
			return;
		}
		this.propertyColorBorder = propertyColorBorder;
		markToRedraw();
	}

	public void setPropertyValue(final float propertyValue) {
		final float clampedValue = FMath.clamp(0.0f, propertyValue, 1.0f);
		if (clampedValue == this.propertyValue) {
			return;
		}
		this.propertyValue = clampedValue;
		this.signalValue.emit(this.propertyValue);
		markToRedraw();
	}

	/**
	 * Set progress as a percentage (0-100).
	 * @param percent The percentage value
	 */
	public void setPercent(final float percent) {
		setPropertyValue(percent / 100.0f);
	}

	/**
	 * Get progress as a percentage (0-100).
	 * @return The percentage value
	 */
	public float getPercent() {
		return this.propertyValue * 100.0f;
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	/**
	 * Create a new ProgressBar.
	 * @return a new ProgressBar
	 */
	public static ProgressBar create() {
		return new ProgressBar();
	}

	/**
	 * Fluent method to set progress value [0-1].
	 * @param value the progress value
	 * @return this progress bar for chaining
	 */
	public ProgressBar value(final float value) {
		setPropertyValue(value);
		return this;
	}

	/**
	 * Fluent method to set progress as percentage [0-100].
	 * @param percent the percentage value
	 * @return this progress bar for chaining
	 */
	public ProgressBar percent(final float percent) {
		setPercent(percent);
		return this;
	}

	/**
	 * Fluent method to set filled color.
	 * @param color the filled portion color
	 * @return this progress bar for chaining
	 */
	public ProgressBar colorOn(final Color color) {
		setPropertyColorOn(color);
		return this;
	}

	/**
	 * Fluent method to set unfilled color.
	 * @param color the unfilled portion color
	 * @return this progress bar for chaining
	 */
	public ProgressBar colorOff(final Color color) {
		setPropertyColorOff(color);
		return this;
	}
}
