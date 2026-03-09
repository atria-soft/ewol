package org.atriasoft.ewol.widget;

import org.atriasoft.esignal.Signal;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;

/**
 * RadioButton widget combining a {@link RadioIndicator} (circle indicator) and an arbitrary content widget.
 * Clicking on either the indicator or the content selects this radio button.
 *
 * <p>Unlike {@link CheckBox}, a RadioButton always selects itself on click (never deselects).
 * Deselection is managed by {@link RadioGroup}.</p>
 *
 * <p>Example usage:</p>
 * <pre>{@code
 * // Simple text radio button
 * RadioButton rb = RadioButton.create("Option 1");
 *
 * // Radio button with custom widget content
 * RadioButton rb2 = RadioButton.create(myCustomWidget);
 * }</pre>
 *
 * Signals emitted:
 * - signalDown: when radio button is pressed down
 * - signalUp: when radio button is released
 * - signalClick: when radio button is clicked
 * - signalValue: when radio button value changes (emits the new boolean value)
 */
public class RadioButton extends Container {

	protected static void eventIndicatorClick(final RadioButton self) {
		self.signalClick.emit();
	}

	protected static void eventIndicatorDown(final RadioButton self) {
		self.signalDown.emit();
	}

	protected static void eventIndicatorUp(final RadioButton self) {
		self.signalUp.emit();
	}

	protected static void eventIndicatorValue(final RadioButton self, final Boolean value) {
		self.signalValue.emit(value);
	}

	protected static void eventContentClick(final RadioButton self) {
		self.signalClick.emit();
		if (!self.indicator.isSelected()) {
			self.indicator.setPropertyValue(true);
		}
	}

	public SignalEmpty signalDown = new SignalEmpty();
	public SignalEmpty signalUp = new SignalEmpty();
	public SignalEmpty signalClick = new SignalEmpty();
	public Signal<Boolean> signalValue = new Signal<>();

	private final RadioIndicator indicator;
	private final Widget content;

	/**
	 * Constructor with no content (indicator only).
	 */
	public RadioButton() {
		this((Widget) null);
	}

	/**
	 * Constructor with text label content.
	 * @param labelText The label text to display
	 */
	public RadioButton(final String labelText) {
		this(createLabel(labelText));
	}

	/**
	 * Constructor with arbitrary widget content.
	 * @param contentWidget The widget to display next to the indicator (can be null)
	 */
	public RadioButton(final Widget contentWidget) {
		final Sizer subs = new Sizer(DisplayMode.HORIZONTAL);
		subs.setPropertyLockExpand(Vector2b.TRUE);
		subs.setPropertyGravity(Gravity.CENTER);
		setSubWidget(subs);

		this.indicator = new RadioIndicator();
		this.indicator.setPropertyExpand(Vector2b.FALSE_TRUE);
		this.indicator.setPropertyFill(Vector2b.FALSE);
		this.indicator.setPropertyGravity(Gravity.CENTER);
		subs.subWidgetAdd(this.indicator);
		this.indicator.signalClick.connectAuto(this, RadioButton::eventIndicatorClick);
		this.indicator.signalUp.connectAuto(this, RadioButton::eventIndicatorUp);
		this.indicator.signalDown.connectAuto(this, RadioButton::eventIndicatorDown);
		this.indicator.signalValue.connectAuto(this, RadioButton::eventIndicatorValue);

		if (contentWidget != null) {
			this.content = contentWidget;
			this.content.setPropertyExpand(Vector2b.TRUE);
			this.content.setPropertyFill(Vector2b.FALSE);
			this.content.setPropertyGravity(Gravity.LEFT);
			subs.subWidgetAdd(this.content);
			if (this.content instanceof Label) {
				((Label) this.content).signalPressed.connectAuto(this, RadioButton::eventContentClick);
			}
		} else {
			this.content = null;
		}
	}

	private static Label createLabel(final String text) {
		return new Label(text);
	}

	public Boolean getPropertyValue() {
		return this.indicator.getPropertyValue();
	}

	public void setPropertyValue(final Boolean value) {
		this.indicator.setPropertyValue(value);
	}

	public boolean isSelected() {
		return this.indicator.isSelected();
	}

	/**
	 * Get the content widget (may be null).
	 * @return the content widget
	 */
	public Widget getContent() {
		return this.content;
	}

	/**
	 * Get the radio indicator widget.
	 * @return the indicator
	 */
	public RadioIndicator getIndicator() {
		return this.indicator;
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	public static RadioButton create() {
		return new RadioButton();
	}

	public static RadioButton create(final String label) {
		return new RadioButton(label);
	}

	public static RadioButton create(final Widget content) {
		return new RadioButton(content);
	}

	public RadioButton selected(final boolean selected) {
		setPropertyValue(selected);
		return this;
	}

	public RadioButton onValueChange(final java.util.function.Consumer<Boolean> callback) {
		this.signalValue.connect(callback::accept);
		return this;
	}

	public RadioButton onClick(final Runnable callback) {
		this.signalClick.connect(callback);
		return this;
	}
}
