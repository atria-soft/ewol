package org.atriasoft.ewol.widget;

import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.Signal;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.ewol.annotation.EwolDescription;
import org.atriasoft.ewol.annotation.EwolSignal;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.widget.meta.SpinBase;
import org.atriasoft.exml.annotation.XmlAttribute;
import org.atriasoft.exml.annotation.XmlManaged;
import org.atriasoft.exml.annotation.XmlName;

/**
 * a composed Spin is a Spin with an inside composed with the specify XML element
 * ==> this permit to generate standard element simple
 */
public class Spin extends SpinBase {
	// Event list of properties
	@EwolSignal(name = "value", description = "Spin updated value (depend of the mantis)")
	public Signal<Long> signalValue = new Signal<>();
	@EwolSignal(name = "valueDouble", description = "Spin value change value in 'double' (application of the mantis)")
	public Signal<Double> signalValueDouble = new Signal<>();
	protected long propertyValue = 0; //!< Current value of the Spin.
	protected long propertyMin = Long.MIN_VALUE; //!< Minimum value
	protected long propertyMax = Long.MAX_VALUE; //!< Maximum value
	protected long propertyIncrement = 1; //!< Increment value
	protected int propertyMantis = 0; //!< number of value under '.' value
	// connection to the elements interface.
	protected Connection connectionEntry = new Connection();
	protected Connection connectionButtonUp = new Connection();
	protected Connection connectionButtonDown = new Connection();
	
	/**
	 * Constructor
	 * @param _mode mode to display the spin
	 * @param _shaperName Shaper file properties
	 */
	public Spin() {
		super(new Uri("THEME", "shape/Spin.json", "ewol"));
		connectGui();
	}
	
	public void checkValue(long value) {
		value = FMath.clamp(this.propertyMin, value, this.propertyMax);
		this.propertyValue = value;
		// TODO: manage the mantis ...
		this.widgetEntry.setPropertyValue(Long.toString(value));
		this.signalValue.emit(this.propertyValue);
	}
	
	public void connectGui() {
		Log.warning("updateGui [START]");
		super.updateGui();
		
		if (this.widgetEntry != null && !this.connectionEntry.isConnected()) {
			this.connectionEntry = this.widgetEntry.signalModify.connect(this, Spin::onCallbackModify);
			// TODO: set a regExp Filter
		}
		if (this.widgetButtonUp != null && !this.connectionButtonUp.isConnected()) {
			this.connectionButtonUp = this.widgetButtonUp.signalValue.connect(this, Spin::onCallbackUp);
		}
		if (this.widgetButtonDown != null && !this.connectionButtonDown.isConnected()) {
			this.connectionButtonDown = this.widgetButtonDown.signalValue.connect(this, Spin::onCallbackDown);
		}
		checkValue(this.propertyValue);
		Log.warning("updateGui [STOP]");
	}
	
	@XmlManaged
	@XmlAttribute
	@XmlName(value = "increment")
	@EwolDescription(value = "Increment value at each button event or keybord event")
	public long getPropertyIncrement() {
		return this.propertyIncrement;
	}
	
	@XmlManaged
	@XmlAttribute
	@XmlName(value = "mantis")
	@EwolDescription(value = "fix-point mantis element (number of digit under the .)")
	public int getPropertyMantis() {
		return this.propertyMantis;
	}
	
	@XmlManaged
	@XmlAttribute
	@XmlName(value = "max")
	@EwolDescription(value = "Maximum value of the spin (depend on mantis)")
	public long getPropertyMax() {
		return this.propertyMax;
	}
	
	@XmlManaged
	@XmlAttribute
	@XmlName(value = "min")
	@EwolDescription(value = "Minimum value of the spin (depend on mantis)")
	public long getPropertyMin() {
		return this.propertyMin;
	}
	
	@XmlManaged
	@XmlAttribute
	@XmlName(value = "value")
	@EwolDescription(value = "Value of the Spin")
	public long getPropertyValue() {
		return this.propertyValue;
	}
	
	protected void onCallbackDown(final Boolean value) {
		if (value) {
			return;
		}
		final long data = this.propertyValue - this.propertyIncrement;
		checkValue(data);
	}
	
	protected void onCallbackModify(final String value) {
		if (value.isEmpty()) {
			return;
		}
		final long value1 = Long.valueOf(value);
		checkValue(value1);
	}
	
	protected void onCallbackUp(final Boolean value) {
		if (value) {
			return;
		}
		final long data = this.propertyValue + this.propertyIncrement;
		checkValue(data);
	}
	
	protected void onChangePropertyIncrement() {
		
	}
	
	protected void onChangePropertyMantis() {
		
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
			Log.error("Can not acces at entry ...");
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
}
