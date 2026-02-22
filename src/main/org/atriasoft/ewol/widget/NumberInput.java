package org.atriasoft.ewol.widget;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlProperty;
import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.Signal;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.ewol.widget.meta.NumberInputBase;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * NumberInput widget allowing the user to select a numeric value using +/- buttons or direct entry.
 *
 * Signals emitted:
 * - signalValue: when the value changes (emits Long value)
 * - signalValueDouble: when the value changes (emits Double value with mantis applied)
 */
public class NumberInput extends NumberInputBase {
	private static final Logger LOGGER = LoggerFactory.getLogger(NumberInput.class);

	public Signal<Long> signalValue = new Signal<>();

	public Signal<Double> signalValueDouble = new Signal<>();

	protected long propertyValue = 0;
	protected long propertyMin = Long.MIN_VALUE;
	protected long propertyMax = Long.MAX_VALUE;
	protected long propertyIncrement = 1;
	protected int propertyMantis = 0;

	protected Connection connectionEntry = new Connection();
	protected Connection connectionButtonUp = new Connection();
	protected Connection connectionButtonDown = new Connection();

	/**
	 * Default constructor.
	 */
	public NumberInput() {
		connectGui();
	}

	/**
	 * Validate and apply a new value, clamping it to min/max bounds.
	 * @param value The new value to set
	 */
	public void checkValue(long value) {
		value = FMath.clamp(value, this.propertyMin, this.propertyMax);
		final boolean changed = this.propertyValue != value;
		this.propertyValue = value;
		// Always update the entry display
		if (this.widgetEntry != null) {
			this.widgetEntry.setPropertyValue(Long.toString(value));
		}
		if (changed) {
			this.signalValue.emit(this.propertyValue);
			emitDoubleValue();
		}
	}

	/**
	 * Emit the double value signal with mantis applied.
	 */
	private void emitDoubleValue() {
		if (this.propertyMantis > 0) {
			final double divisor = Math.pow(10, this.propertyMantis);
			this.signalValueDouble.emit(this.propertyValue / divisor);
		} else {
			this.signalValueDouble.emit((double) this.propertyValue);
		}
	}

	/**
	 * Connect GUI elements (buttons and entry) to their callbacks.
	 */
	public void connectGui() {
		LOGGER.debug("connectGui [START]");
		super.updateGui();
		if (this.widgetEntry != null && !this.connectionEntry.isConnected()) {
			this.connectionEntry = this.widgetEntry.signalModify.connect(this, NumberInput::onCallbackModify);
		}
		if (this.widgetButtonUp != null && !this.connectionButtonUp.isConnected()) {
			this.connectionButtonUp = this.widgetButtonUp.signalClick.connect(this, NumberInput::onCallbackUp);
		}
		if (this.widgetButtonDown != null && !this.connectionButtonDown.isConnected()) {
			this.connectionButtonDown = this.widgetButtonDown.signalClick.connect(this, NumberInput::onCallbackDown);
		}
		checkValue(this.propertyValue);
		LOGGER.debug("connectGui [STOP]");
	}

	@JsonProperty("increment")
	@JacksonXmlProperty(isAttribute = true, localName = "increment")
	public long getPropertyIncrement() {
		return this.propertyIncrement;
	}

	@JsonProperty("mantis")
	@JacksonXmlProperty(isAttribute = true, localName = "mantis")
	public int getPropertyMantis() {
		return this.propertyMantis;
	}

	@JsonProperty("max")
	@JacksonXmlProperty(isAttribute = true, localName = "max")
	public long getPropertyMax() {
		return this.propertyMax;
	}

	@JsonProperty("min")
	@JacksonXmlProperty(isAttribute = true, localName = "min")
	public long getPropertyMin() {
		return this.propertyMin;
	}

	@JsonProperty("value")
	@JacksonXmlProperty(isAttribute = true, localName = "value")
	public long getPropertyValue() {
		return this.propertyValue;
	}

	/**
	 * Get the value as a double with mantis applied.
	 * @return The value as double
	 */
	public double getValueAsDouble() {
		if (this.propertyMantis > 0) {
			final double divisor = Math.pow(10, this.propertyMantis);
			return this.propertyValue / divisor;
		}
		return this.propertyValue;
	}

	protected void onCallbackDown() {
		LOGGER.debug("NumberInput decrement button clicked");
		final long data = this.propertyValue - this.propertyIncrement;
		checkValue(data);
	}

	protected void onCallbackModify(final String value) {
		if (value == null || value.isEmpty()) {
			return;
		}
		try {
			final long parsedValue = Long.parseLong(value);
			checkValue(parsedValue);
		} catch (final NumberFormatException ex) {
			LOGGER.warn("Invalid number format: '{}' - {}", value, ex.getMessage());
		}
	}

	protected void onCallbackUp() {
		LOGGER.debug("NumberInput increment button clicked");
		final long data = this.propertyValue + this.propertyIncrement;
		checkValue(data);
	}

	protected void onChangePropertyIncrement() {
		// Increment change doesn't require immediate action
	}

	protected void onChangePropertyMantis() {
		emitDoubleValue();
		markToRedraw();
	}

	protected void onChangePropertyMax() {
		checkValue(this.propertyValue);
	}

	protected void onChangePropertyMin() {
		checkValue(this.propertyValue);
	}

	protected void onChangePropertyValue() {
		markToRedraw();
		if (this.widgetEntry == null) {
			LOGGER.warn("Cannot access entry widget");
			return;
		}
		checkValue(this.propertyValue);
	}

	public void setPropertyIncrement(final long propertyIncrement) {
		if (this.propertyIncrement == propertyIncrement) {
			return;
		}
		this.propertyIncrement = propertyIncrement;
		onChangePropertyIncrement();
	}

	public void setPropertyMantis(final int propertyMantis) {
		if (this.propertyMantis == propertyMantis) {
			return;
		}
		this.propertyMantis = propertyMantis;
		onChangePropertyMantis();
	}

	public void setPropertyMax(final long propertyMax) {
		if (this.propertyMax == propertyMax) {
			return;
		}
		this.propertyMax = propertyMax;
		onChangePropertyMax();
	}

	public void setPropertyMin(final long propertyMin) {
		if (this.propertyMin == propertyMin) {
			return;
		}
		this.propertyMin = propertyMin;
		onChangePropertyMin();
	}

	public void setPropertyValue(final long propertyValue) {
		if (this.propertyValue == propertyValue) {
			return;
		}
		this.propertyValue = propertyValue;
		onChangePropertyValue();
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	/**
	 * Create a new NumberInput.
	 * @return a new NumberInput
	 */
	public static NumberInput create() {
		return new NumberInput();
	}

	/**
	 * Fluent method to set value.
	 * @param value the current value
	 * @return this spin for chaining
	 */
	public NumberInput value(final long value) {
		setPropertyValue(value);
		return this;
	}

	/**
	 * Fluent method to set minimum value.
	 * @param min the minimum value
	 * @return this spin for chaining
	 */
	public NumberInput min(final long min) {
		setPropertyMin(min);
		return this;
	}

	/**
	 * Fluent method to set maximum value.
	 * @param max the maximum value
	 * @return this spin for chaining
	 */
	public NumberInput max(final long max) {
		setPropertyMax(max);
		return this;
	}

	/**
	 * Fluent method to set range.
	 * @param min the minimum value
	 * @param max the maximum value
	 * @return this spin for chaining
	 */
	public NumberInput range(final long min, final long max) {
		setPropertyMin(min);
		setPropertyMax(max);
		return this;
	}

	/**
	 * Fluent method to set increment.
	 * @param increment the increment value
	 * @return this spin for chaining
	 */
	public NumberInput increment(final long increment) {
		setPropertyIncrement(increment);
		return this;
	}

	/**
	 * Fluent method to set mantis (decimal precision).
	 * @param mantis number of decimal places
	 * @return this spin for chaining
	 */
	public NumberInput mantis(final int mantis) {
		setPropertyMantis(mantis);
		return this;
	}

	/**
	 * Fluent method to connect a value change callback.
	 * @param callback the callback to invoke when value changes
	 * @return this spin for chaining
	 */
	public NumberInput onValueChange(final java.util.function.Consumer<Long> callback) {
		this.signalValue.connect(callback::accept);
		return this;
	}

	/**
	 * Fluent method to connect a double value change callback.
	 * @param callback the callback to invoke when value changes
	 * @return this spin for chaining
	 */
	public NumberInput onDoubleValueChange(final java.util.function.Consumer<Double> callback) {
		this.signalValueDouble.connect(callback::accept);
		return this;
	}
}
