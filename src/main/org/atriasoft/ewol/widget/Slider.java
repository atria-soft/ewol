package org.atriasoft.ewol.widget;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlProperty;
import org.atriasoft.esignal.Signal;
import org.atriasoft.etk.BorderRadius;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Insets;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.compositing.CompositingGC;
import org.atriasoft.ewol.event.EventInput;
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

	// Default dimensions
	private static final float DEFAULT_MIN_WIDTH = 150.0f;
	private static final float DEFAULT_TRACK_HEIGHT = 8.0f;
	private static final float DEFAULT_CURSOR_WIDTH = 20.0f;
	private static final float DEFAULT_CURSOR_HEIGHT = 20.0f;

	// Default colors
	private static final Color DEFAULT_TRACK_COLOR = new Color(0xE0, 0xE0, 0xE0, 0xFF);
	private static final Color DEFAULT_FILL_COLOR = new Color(0x42, 0x85, 0xF4, 0xFF);
	private static final Color DEFAULT_CURSOR_COLOR = Color.WHITE;
	private static final Color DEFAULT_CURSOR_BORDER_COLOR = new Color(0x42, 0x85, 0xF4, 0xFF);
	private static final Color DEFAULT_MARKER_COLOR = new Color(0x60, 0x60, 0x60, 0xFF);
	private static final float DEFAULT_MARKER_RADIUS = 3.0f;

	private float propertyValue = 0.0f;

	public Signal<Float> signalValue = new Signal<>();

	// Element boundaries for hit detection
	private Vector2f overPositionStart = Vector2f.ZERO;
	private Vector2f overPositionStop = Vector2f.ZERO;
	private Vector2f overPositionSize = Vector2f.ZERO;

	private float propertyMinimum = 0.0f;
	private float propertyMaximum = 10.0f;
	private float propertyStep = 0.1f;

	// Customizable colors
	private Color propertyTrackColor = DEFAULT_TRACK_COLOR;
	private Color propertyFillColor = DEFAULT_FILL_COLOR;
	private Color propertyCursorColor = DEFAULT_CURSOR_COLOR;
	private Color propertyCursorBorderColor = DEFAULT_CURSOR_BORDER_COLOR;
	private Color propertyMarkerColor = DEFAULT_MARKER_COLOR;

	// Customizable dimensions
	private final float propertyTrackHeight = DEFAULT_TRACK_HEIGHT;
	private final float propertyCursorWidth = DEFAULT_CURSOR_WIDTH;
	private final float propertyCursorHeight = DEFAULT_CURSOR_HEIGHT;
	private float propertyMarkerRadius = DEFAULT_MARKER_RADIUS;

	// Markers (points on the track)
	private List<Float> markers = new ArrayList<>();

	private final CompositingGC vectorialDraw = new CompositingGC();

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
		// Minimum size: width to fit cursor + some track, height to fit cursor
		final float minWidth = DEFAULT_MIN_WIDTH;
		final float minHeight = this.propertyCursorHeight + 4.0f;
		this.minSize = Vector2f.max(this.minSize, new Vector2f(minWidth, minHeight));
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
		final float percent = FMath.clamp((relPos.x() - this.overPositionStart.x()) / sliderWidth, 0.0f, 1.0f);
		float value = (this.propertyMaximum - this.propertyMinimum) * percent + this.propertyMinimum;
		if (this.propertyStep != 0.0f) {
			value += this.propertyStep * 0.5f;
		}
		return value;
	}

	@JsonProperty("maximum")
	@JacksonXmlProperty(isAttribute = true, localName = "maximum")
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

	@JsonProperty("minimum")
	@JacksonXmlProperty(isAttribute = true, localName = "minimum")
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

	@JsonProperty("step")
	@JacksonXmlProperty(isAttribute = true, localName = "step")
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

	@JsonProperty("value")
	@JacksonXmlProperty(isAttribute = true, localName = "value")
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

	// ========================================================================
	// Color properties
	// ========================================================================

	@JsonProperty("track-color")
	@JacksonXmlProperty(isAttribute = true, localName = "track-color")
	public Color getPropertyTrackColor() {
		return this.propertyTrackColor;
	}

	public void setPropertyTrackColor(final Color color) {
		if (Objects.equals(this.propertyTrackColor, color)) {
			return;
		}
		this.propertyTrackColor = color;
		markToRedraw();
	}

	@JsonProperty("fill-color")
	@JacksonXmlProperty(isAttribute = true, localName = "fill-color")
	public Color getPropertyFillColor() {
		return this.propertyFillColor;
	}

	public void setPropertyFillColor(final Color color) {
		if (Objects.equals(this.propertyFillColor, color)) {
			return;
		}
		this.propertyFillColor = color;
		markToRedraw();
	}

	@JsonProperty("cursor-color")
	@JacksonXmlProperty(isAttribute = true, localName = "cursor-color")
	public Color getPropertyCursorColor() {
		return this.propertyCursorColor;
	}

	public void setPropertyCursorColor(final Color color) {
		if (Objects.equals(this.propertyCursorColor, color)) {
			return;
		}
		this.propertyCursorColor = color;
		markToRedraw();
	}

	@JsonProperty("cursor-border-color")
	@JacksonXmlProperty(isAttribute = true, localName = "cursor-border-color")
	public Color getPropertyCursorBorderColor() {
		return this.propertyCursorBorderColor;
	}

	public void setPropertyCursorBorderColor(final Color color) {
		if (Objects.equals(this.propertyCursorBorderColor, color)) {
			return;
		}
		this.propertyCursorBorderColor = color;
		markToRedraw();
	}

	@JsonProperty("marker-color")
	@JacksonXmlProperty(isAttribute = true, localName = "marker-color")
	public Color getPropertyMarkerColor() {
		return this.propertyMarkerColor;
	}

	public void setPropertyMarkerColor(final Color color) {
		if (Objects.equals(this.propertyMarkerColor, color)) {
			return;
		}
		this.propertyMarkerColor = color;
		markToRedraw();
	}

	@JsonProperty("marker-radius")
	@JacksonXmlProperty(isAttribute = true, localName = "marker-radius")
	public float getPropertyMarkerRadius() {
		return this.propertyMarkerRadius;
	}

	public void setPropertyMarkerRadius(final float radius) {
		if (this.propertyMarkerRadius == radius) {
			return;
		}
		this.propertyMarkerRadius = radius;
		markToRedraw();
	}

	/**
	 * Get the list of marker values.
	 * @return the list of marker values
	 */
	public List<Float> getMarkers() {
		return new ArrayList<>(this.markers);
	}

	/**
	 * Set the list of marker values.
	 * @param markers the list of marker values
	 */
	public void setMarkers(final List<Float> markers) {
		this.markers = markers != null ? new ArrayList<>(markers) : new ArrayList<>();
		markToRedraw();
	}

	/**
	 * Add a marker at the specified value.
	 * @param value the value where to place the marker
	 */
	public void addMarker(final float value) {
		this.markers.add(value);
		markToRedraw();
	}

	/**
	 * Clear all markers.
	 */
	public void clearMarkers() {
		this.markers.clear();
		markToRedraw();
	}

	/**
	 * Draw markers on the track.
	 * @param trackStartX the X position where the track starts
	 * @param trackY the Y position of the track
	 * @param trackWidth the width of the track
	 */
	private void drawMarkers(final float trackStartX, final float trackY, final float trackWidth) {
		if (this.markers.isEmpty() || this.propertyMaximum == this.propertyMinimum) {
			return;
		}
		this.vectorialDraw.setPaintFillColor(this.propertyMarkerColor);
		final float trackCenterY = trackY + this.propertyTrackHeight * 0.5f;
		final float range = this.propertyMaximum - this.propertyMinimum;
		for (final Float markerValue : this.markers) {
			if (markerValue == null) {
				continue;
			}
			final float markerRatio = (markerValue - this.propertyMinimum) / range;
			if (markerRatio >= 0.0f && markerRatio <= 1.0f) {
				final float markerX = trackStartX + trackWidth * markerRatio;
				final Vector2f markerStart = new Vector2f(markerX - this.propertyMarkerRadius,
						trackCenterY - this.propertyMarkerRadius);
				final Vector2f markerStop = new Vector2f(markerX + this.propertyMarkerRadius,
						trackCenterY + this.propertyMarkerRadius);
				this.vectorialDraw.addRectangle(markerStart, markerStop, new Insets(0),
						new BorderRadius(this.propertyMarkerRadius));
			}
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
					grabEvents();
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
					unGrabEvents();
					setPropertyValue(calculateValueFromPosition(relPos));
					return true;
				}
				break;

			case leave:
				// With event grab, leave should not fire during drag.
				// Safety fallback: if leave fires anyway (e.g. window exit),
				// stop the drag.
				if (this.isDragging) {
					this.isDragging = false;
					unGrabEvents();
				}
				break;

			default:
				break;
		}
		return false;
	}

	@Override
	protected void onLostFocus() {
		if (this.isDragging) {
			this.isDragging = false;
			unGrabEvents();
		}
	}

	@Override
	public void onRegenerateDisplay() {
		if (!needRedraw()) {
			return;
		}
		this.vectorialDraw.clear();

		// Calculate available size
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

		// Calculate track position (centered vertically, with padding for cursor)
		final float trackPaddingX = this.propertyCursorWidth * 0.5f;
		final float trackY = delta.y() + (sizeInsideRender.y() - this.propertyTrackHeight) * 0.5f;
		final float trackStartX = delta.x() + trackPaddingX;
		final float trackWidth = sizeInsideRender.x() - this.propertyCursorWidth;

		this.overPositionStart = new Vector2f(trackStartX, trackY);
		this.overPositionSize = new Vector2f(trackWidth, this.propertyTrackHeight);
		this.overPositionStop = this.overPositionStart.add(this.overPositionSize);

		final float trackRadius = this.propertyTrackHeight * 0.5f;

		// Calculate the position ratio (0.0 to 1.0)
		float ratio = 0.0f;
		if (this.propertyMaximum != this.propertyMinimum) {
			ratio = (this.propertyValue - this.propertyMinimum) / (this.propertyMaximum - this.propertyMinimum);
		}

		// Draw track background (unfilled portion)
		this.vectorialDraw.setPaintFillColor(this.propertyTrackColor);
		this.vectorialDraw.addRectangle(this.overPositionStart, this.overPositionStop, new Insets(0),
				new BorderRadius(trackRadius));

		// Draw filled portion of track
		if (ratio > 0.0f) {
			final float filledWidth = trackWidth * ratio;
			final Vector2f fillStop = new Vector2f(trackStartX + filledWidth, trackY + this.propertyTrackHeight);
			this.vectorialDraw.setPaintFillColor(this.propertyFillColor);
			this.vectorialDraw.addRectangle(this.overPositionStart, fillStop, new Insets(0),
					new BorderRadius(trackRadius, 0, 0, trackRadius));
		}

		// Draw markers
		drawMarkers(trackStartX, trackY, trackWidth);

		// Draw cursor
		final float cursorX = trackStartX + trackWidth * ratio - this.propertyCursorWidth * 0.5f;
		final float cursorY = delta.y() + (sizeInsideRender.y() - this.propertyCursorHeight) * 0.5f;
		final Vector2f cursorStart = new Vector2f(cursorX, cursorY);
		final Vector2f cursorStop = cursorStart.add(this.propertyCursorWidth, this.propertyCursorHeight);
		final float cursorRadius = this.propertyCursorHeight * 0.5f;

		// Cursor border (white shadow effect)
		this.vectorialDraw.setPaintFillColor(this.propertyCursorBorderColor);
		this.vectorialDraw.addRectangle(cursorStart, cursorStop, new Insets(0), new BorderRadius(cursorRadius));

		final float borderWidth = this.propertyCursorHeight * 0.15f;
		final Vector2f innerStart = cursorStart.add(borderWidth, borderWidth);
		final Vector2f innerStop = cursorStop.less(borderWidth, borderWidth);
		this.vectorialDraw.setPaintFillColor(this.propertyCursorColor);
		this.vectorialDraw.addRectangle(innerStart, innerStop, new Insets(0),
				new BorderRadius(cursorRadius - borderWidth));

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

	/**
	 * Fluent method to set track background color.
	 * @param color the track color
	 * @return this slider for chaining
	 */
	public Slider trackColor(final Color color) {
		setPropertyTrackColor(color);
		return this;
	}

	/**
	 * Fluent method to set filled portion color.
	 * @param color the fill color
	 * @return this slider for chaining
	 */
	public Slider fillColor(final Color color) {
		setPropertyFillColor(color);
		return this;
	}

	/**
	 * Fluent method to set cursor color.
	 * @param color the cursor color
	 * @return this slider for chaining
	 */
	public Slider cursorColor(final Color color) {
		setPropertyCursorColor(color);
		return this;
	}

	/**
	 * Fluent method to set cursor border color.
	 * @param color the cursor border color
	 * @return this slider for chaining
	 */
	public Slider cursorBorderColor(final Color color) {
		setPropertyCursorBorderColor(color);
		return this;
	}

	/**
	 * Fluent method to set marker color.
	 * @param color the marker color
	 * @return this slider for chaining
	 */
	public Slider markerColor(final Color color) {
		setPropertyMarkerColor(color);
		return this;
	}

	/**
	 * Fluent method to set marker radius.
	 * @param radius the marker radius
	 * @return this slider for chaining
	 */
	public Slider markerRadius(final float radius) {
		setPropertyMarkerRadius(radius);
		return this;
	}

	/**
	 * Fluent method to set markers from a list of values.
	 * @param values the marker values
	 * @return this slider for chaining
	 */
	public Slider markers(final List<Float> values) {
		setMarkers(values);
		return this;
	}

	/**
	 * Fluent method to set markers from varargs.
	 * @param values the marker values
	 * @return this slider for chaining
	 */
	public Slider markers(final float... values) {
		this.markers.clear();
		if (values != null) {
			for (final float value : values) {
				this.markers.add(value);
			}
		}
		markToRedraw();
		return this;
	}

	/**
	 * Fluent method to add a single marker.
	 * @param value the marker value
	 * @return this slider for chaining
	 */
	public Slider marker(final float value) {
		addMarker(value);
		return this;
	}
}