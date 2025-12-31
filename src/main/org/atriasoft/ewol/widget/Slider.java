package org.atriasoft.ewol.widget;

import java.util.Objects;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.aknot.annotation.AknotSignal;
import org.atriasoft.esignal.Signal;
import org.atriasoft.etk.BorderRadius;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension1f;
import org.atriasoft.etk.Insets;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.Padding;
import org.atriasoft.ewol.compositing.CompositingGC;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.gale.key.KeyStatus;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Slider widget allowing the user to select a value within a range.
 *
 * Signals emitted:
 * - signalValue: when the slider value changes (emits the new Float value)
 */
public class Slider extends Widget {
	private static final Logger LOGGER = LoggerFactory.getLogger(Slider.class);

	private float propertyValue = 0.0f;

	@AknotSignal
	@AknotName("value")
	@AknotDescription("Slider value change")
	public Signal<Float> signalValue = new Signal<>();

	// Element boundaries for hit detection
	private Vector2f overPositionStart = Vector2f.ZERO;
	private Vector2f overPositionStop = Vector2f.ZERO;
	private Vector2f overPositionSize = Vector2f.ZERO;
	private Vector2f overCursorPositionStart = Vector2f.ZERO;
	private Vector2f overCursorPositionStop = Vector2f.ZERO;
	private Vector2f overCursorPositionSize = Vector2f.ZERO;

	private float propertyMinimum = 0.0f;
	private float propertyMaximum = 10.0f;
	private float propertyStep = 0.1f;

	private final Color textColorFg = Color.BLACK;
	private final Color textColorBg = Color.BLACK.withA(0x3F);

	private final CompositingGC vectorialDraw = new CompositingGC();
	private final Dimension1f propertyLineWidth = new Dimension1f(20);

	private boolean isDragging = false;

	/**
	 * Default constructor.
	 */
	public Slider() {
		this.propertyCanFocus = true;
		markToRedraw();
		setMouseLimit(1);
	}

	@Override
	public void calculateMinMaxSize() {
		super.calculateMinMaxSize();
		final Padding padding = Padding.ZERO;
		final Vector2i minHeight = Vector2i.VALUE_16;

		Vector2f minimumSizeBase = new Vector2f(minHeight.x(), minHeight.y());
		minimumSizeBase = minimumSizeBase.add(padding.x(), padding.y());
		this.minSize = Vector2f.max(this.minSize, minimumSizeBase);
		checkMinSize();
		LOGGER.debug("min size = {}", this.minSize);
	}

	private boolean isInsideSlider(final Vector2f relPos) {
		return relPos.x() > this.overPositionStart.x() && relPos.y() > this.overPositionStart.y()
				&& relPos.x() < this.overPositionStop.x() && relPos.y() < this.overPositionStop.y();
	}

	/**
	 * Calculate the value from a relative position.
	 * @param relPos The relative position of the cursor
	 * @return The calculated value
	 */
	private float calculateValueFromPosition(final Vector2f relPos) {
		final float sliderWidth = this.overPositionStop.x() - this.overPositionStart.x();
		if (sliderWidth <= 0) {
			return this.propertyMinimum;
		}
		final float percent = FMath.clamp(
				(relPos.x() - this.overPositionStart.x()) / sliderWidth,
				0.0f, 1.0f);
		float value = (this.propertyMaximum - this.propertyMinimum) * percent + this.propertyMinimum;
		if (this.propertyStep != 0.0f) {
			value += this.propertyStep * 0.5f;
		}
		return value;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName("maximum")
	@AknotDescription("Maximum value of the slider")
	public Float getPropertyMaximum() {
		return this.propertyMaximum;
	}

	public void setPropertyMaximum(final Float propertyMaximum) {
		if (Objects.equals(this.propertyMaximum, propertyMaximum)) {
			return;
		}
		this.propertyMaximum = propertyMaximum;
		updateValue(this.propertyValue);
	}

	@AknotManaged
	@AknotAttribute
	@AknotName("minimum")
	@AknotDescription("Minimum value of the slider")
	public Float getPropertyMinimum() {
		return this.propertyMinimum;
	}

	public void setPropertyMinimum(final Float propertyMinimum) {
		if (Objects.equals(this.propertyMinimum, propertyMinimum)) {
			return;
		}
		this.propertyMinimum = propertyMinimum;
		updateValue(this.propertyValue);
	}

	@AknotManaged
	@AknotAttribute
	@AknotName("step")
	@AknotDescription("Step value of the slider")
	public Float getPropertyStep() {
		return this.propertyStep;
	}

	public void setPropertyStep(final Float propertyStep) {
		if (Objects.equals(this.propertyStep, propertyStep)) {
			return;
		}
		this.propertyStep = propertyStep;
		updateValue(this.propertyValue);
	}

	@AknotManaged
	@AknotAttribute
	@AknotName("value")
	@AknotDescription("Value of the slider")
	public Float getPropertyValue() {
		return this.propertyValue;
	}

	public void setPropertyValue(final Float propertyValue) {
		final float oldValue = this.propertyValue;
		updateValue(propertyValue != null ? propertyValue : 0.0f);
		if (oldValue != this.propertyValue) {
			this.signalValue.emit(this.propertyValue);
		}
	}

	/**
	 * Update the value, clamping it to min/max and applying step.
	 * @param newValue The new value to set
	 */
	protected void updateValue(float newValue) {
		newValue = FMath.clamp(newValue, this.propertyMinimum, this.propertyMaximum);
		if (this.propertyStep != 0.0f) {
			final float steps = Math.round((newValue - this.propertyMinimum) / this.propertyStep);
			newValue = this.propertyMinimum + steps * this.propertyStep;
			newValue = FMath.clamp(newValue, this.propertyMinimum, this.propertyMaximum);
		}
		this.propertyValue = newValue;
		markToRedraw();
	}

	@Override
	public void onDraw() {
		this.vectorialDraw.draw();
	}

	@Override
	public boolean onEventInput(final EventInput event) {
		final Vector2f relPos = relativePosition(event.pos());
		LOGGER.trace("Event on Input: {} relPos = {}", event, relPos);
		final boolean over = isInsideSlider(relPos);

		if (event.inputId() != 1) {
			return false;
		}

		switch (event.status()) {
			case pressSingle:
			case down:
				if (over) {
					keepFocus();
					this.isDragging = true;
					setPropertyValue(calculateValueFromPosition(relPos));
					return true;
				}
				break;

			case move:
				if (this.isDragging) {
					keepFocus();
					setPropertyValue(calculateValueFromPosition(relPos));
					return true;
				}
				break;

			case up:
				if (this.isDragging) {
					keepFocus();
					this.isDragging = false;
					setPropertyValue(calculateValueFromPosition(relPos));
					return true;
				}
				break;

			case leave:
				this.isDragging = false;
				break;

			default:
				break;
		}
		return false;
	}

	@Override
	protected void onLostFocus() {
		this.isDragging = false;
	}

	@Override
	public void onRegenerateDisplay() {
		if (!needRedraw()) {
			return;
		}
		this.vectorialDraw.clear();
		{
			// Manage external shape:
			Vector2f sizeInsideRender = this.minSize;
			Vector2f delta = this.propertyGravity.gravityGenerateDelta(this.size.less(this.minSize));
			if (this.propertyFill.x()) {
				sizeInsideRender = sizeInsideRender.withX(this.size.x());
				delta = delta.withX(0.0f);
			}
			if (this.propertyFill.y()) {
				sizeInsideRender = sizeInsideRender.withY(this.size.y());
				delta = delta.withY(0.0f);
			}

			Vector2f tmpOriginShaper = delta;
			// fix all the position in the int class:
			sizeInsideRender = Vector2f.clipInt(sizeInsideRender);
			tmpOriginShaper = Vector2f
					.clipInt(tmpOriginShaper.addY(sizeInsideRender.y() * 0.5f - this.propertyLineWidth.size() * 0.5f));

			this.overPositionStart = tmpOriginShaper;
			this.overPositionSize = sizeInsideRender.withY(this.propertyLineWidth.size());
			this.overPositionStop = tmpOriginShaper.add(this.overPositionSize);
			this.vectorialDraw.setPaintFillColor(this.textColorBg);
			this.vectorialDraw.addRectangle(this.overPositionStart, this.overPositionStop, new Insets(0),
					new BorderRadius(this.propertyLineWidth.size() * 0.5f));
		}
			{
			// Manage cursor:
			final float cursorWidth = this.propertyLineWidth.size() * 1.5f;
			final float cursorHeight = this.propertyLineWidth.size() * 2.0f;

			// Calculate the position ratio (0.0 to 1.0)
			final float ratio = (this.propertyValue - this.propertyMinimum) / (this.propertyMaximum - this.propertyMinimum);

			// Calculate the cursor X position based on the slider track
			final float sliderTrackWidth = this.overPositionSize.x() - cursorWidth;
			final float cursorX = this.overPositionStart.x() + sliderTrackWidth * ratio;

			// Center the cursor vertically on the slider track
			final float cursorY = this.overPositionStart.y() + (this.overPositionSize.y() - cursorHeight) * 0.5f;

			this.overCursorPositionStart = Vector2f.clipInt(new Vector2f(cursorX, cursorY));
			this.overCursorPositionSize = Vector2f.clipInt(new Vector2f(cursorWidth, cursorHeight));
			this.overCursorPositionStop = this.overCursorPositionStart.add(this.overCursorPositionSize);

			this.vectorialDraw.setColor(Color.RED);
			this.vectorialDraw.addRectangle(this.overCursorPositionStart, this.overCursorPositionStop, new Insets(0),
					new BorderRadius(this.propertyLineWidth.size() * 0.4f));
		}
		this.vectorialDraw.flush();

	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	/**
	 * Create a new Slider with default settings.
	 * @return a new Slider
	 */
	public static Slider create() {
		return new Slider();
	}

	/**
	 * Fluent method to set minimum value.
	 * @param min the minimum value
	 * @return this slider for chaining
	 */
	public Slider min(final float min) {
		setPropertyMinimum(min);
		return this;
	}

	/**
	 * Fluent method to set maximum value.
	 * @param max the maximum value
	 * @return this slider for chaining
	 */
	public Slider max(final float max) {
		setPropertyMaximum(max);
		return this;
	}

	/**
	 * Fluent method to set value.
	 * @param value the value
	 * @return this slider for chaining
	 */
	public Slider value(final float value) {
		setPropertyValue(value);
		return this;
	}

	/**
	 * Fluent method to set step.
	 * @param step the step value
	 * @return this slider for chaining
	 */
	public Slider step(final float step) {
		setPropertyStep(step);
		return this;
	}

	/**
	 * Fluent method to set range (min and max).
	 * @param min the minimum value
	 * @param max the maximum value
	 * @return this slider for chaining
	 */
	public Slider range(final float min, final float max) {
		setPropertyMinimum(min);
		setPropertyMaximum(max);
		return this;
	}

	/**
	 * Fluent method to connect a value change callback.
	 * @param callback the callback to invoke on value change
	 * @return this slider for chaining
	 */
	public Slider onValueChange(final java.util.function.Consumer<Float> callback) {
		this.signalValue.connect(callback::accept);
		return this;
	}
}