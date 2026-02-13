package org.atriasoft.ewol.widget;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlProperty;
import org.atriasoft.esignal.Signal;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;

/**
 * CheckBox widget combining a Tick (checkbox indicator) and a Label.
 * Clicking on either the tick or the label toggles the checkbox state.
 *
 * Signals emitted:
 * - signalDown: when checkbox is pressed down
 * - signalUp: when checkbox is released
 * - signalClick: when checkbox is clicked
 * - signalValue: when checkbox value changes (emits the new boolean value)
 */
public class CheckBox extends Container {

	protected static void eventLabelClick(final CheckBox self) {
		self.signalClick.emit();
	}

	protected static void eventTickClick(final CheckBox self) {
		self.signalClick.emit();
	}

	protected static void eventTickDown(final CheckBox self) {
		self.signalDown.emit();
	}

	protected static void eventTickUp(final CheckBox self) {
		self.signalUp.emit();
	}

	protected static void eventTickValue(final CheckBox self, final Boolean value) {
		self.signalValue.emit(value);
	}

	public SignalEmpty signalDown = new SignalEmpty();

	public SignalEmpty signalUp = new SignalEmpty();

	public SignalEmpty signalClick = new SignalEmpty();

	public Signal<Boolean> signalValue = new Signal<>();

	private final Tick tick;
	private final Label label;

	/**
	 * Default constructor with "No Label" text.
	 */
	public CheckBox() {
		this("No Label");
	}

	/**
	 * Constructor with custom label text.
	 * @param basicLabel The label text to display
	 */
	public CheckBox(final String basicLabel) {
		final Sizer subs = new Sizer(DisplayMode.HORIZONTAL);
		subs.setPropertyLockExpand(Vector2b.TRUE);
		subs.setPropertyGravity(Gravity.CENTER);
		setSubWidget(subs);

		this.tick = new Tick();
		this.tick.setPropertyExpand(Vector2b.FALSE_TRUE);
		this.tick.setPropertyFill(Vector2b.FALSE);
		this.tick.setPropertyGravity(Gravity.CENTER);
		subs.subWidgetAdd(this.tick);
		this.tick.signalClick.connectAuto(this, CheckBox::eventTickClick);
		this.tick.signalUp.connectAuto(this, CheckBox::eventTickUp);
		this.tick.signalDown.connectAuto(this, CheckBox::eventTickDown);
		this.tick.signalValue.connectAuto(this, CheckBox::eventTickValue);

		this.label = new Label(basicLabel);
		this.label.setPropertyExpand(Vector2b.TRUE);
		this.label.setPropertyFill(Vector2b.FALSE);
		this.label.setPropertyGravity(Gravity.LEFT);
		subs.subWidgetAdd(this.label);
		this.label.signalPressed.connectAuto(this, CheckBox::eventLabelClick);
	}

	@JsonProperty("label")
	@JacksonXmlProperty(isAttribute = true, localName = "label")
	public String getPropertyLabel() {
		return this.label.getPropertyValue();
	}

	@JsonProperty("value")
	@JacksonXmlProperty(isAttribute = true, localName = "value")
	public Boolean getPropertyValue() {
		return this.tick.getPropertyValue();
	}

	/**
	 * Set the label text.
	 * @param value The new label text
	 */
	public void setPropertyLabel(final String value) {
		this.label.setPropertyValue(value);
	}

	/**
	 * Set the checkbox state.
	 * @param value true for checked, false for unchecked
	 */
	public void setPropertyValue(final Boolean value) {
		this.tick.setPropertyValue(value);
	}

	/**
	 * Toggle the checkbox state.
	 */
	public void toggle() {
		setPropertyValue(!getPropertyValue());
	}

	/**
	 * Check if the checkbox is checked.
	 * @return true if checked
	 */
	public boolean isChecked() {
		return Boolean.TRUE.equals(getPropertyValue());
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	/**
	 * Create a new CheckBox with label.
	 * @param label the label text
	 * @return a new CheckBox
	 */
	public static CheckBox create(final String label) {
		return new CheckBox(label);
	}

	/**
	 * Fluent method to set label text.
	 * @param label the label text
	 * @return this checkbox for chaining
	 */
	public CheckBox label(final String label) {
		setPropertyLabel(label);
		return this;
	}

	/**
	 * Fluent method to set checked state.
	 * @param checked true for checked
	 * @return this checkbox for chaining
	 */
	public CheckBox checked(final boolean checked) {
		setPropertyValue(checked);
		return this;
	}

	/**
	 * Fluent method to connect a value change callback.
	 * @param callback the callback to invoke on value change
	 * @return this checkbox for chaining
	 */
	public CheckBox onValueChange(final java.util.function.Consumer<Boolean> callback) {
		this.signalValue.connect(callback::accept);
		return this;
	}

	/**
	 * Fluent method to connect a click callback.
	 * @param callback the callback to invoke when clicked
	 * @return this checkbox for chaining
	 */
	public CheckBox onClick(final Runnable callback) {
		this.signalClick.connect(callback);
		return this;
	}
}
