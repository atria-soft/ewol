package org.atriasoft.ewol.widget;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.aknot.annotation.AknotSignal;
import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.DimensionInsets;
import org.atriasoft.etk.Uri;

import java.util.ArrayList;
import java.util.List;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.event.EventEntry;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.event.EventTime;
import org.atriasoft.gale.key.KeyKeyboard;
import org.atriasoft.gale.key.KeyStatus;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Button widget that can be clicked by the user.
 *
 * Signals emitted:
 * - signalDown: when button is pressed down
 * - signalUp: when button is released
 * - signalClick: when button is clicked (press + release)
 * - signalEnter: when cursor enters the button area
 * - signalLeave: when cursor leaves the button area
 */
public class Button extends Box {
	private static final Logger LOGGER = LoggerFactory.getLogger(Button.class);

	/**
	 * Factory method to create a button with a label.
	 * @param label The text to display on the button
	 * @return A new Button instance with the label set
	 */
	public static Button createLabelButton(final String label) {
		final Button out = new Button();
		final Label labelWidget = new Label();
		labelWidget.setPropertyFontSize(12);
		labelWidget.setPropertyFill(Vector2b.FALSE);
		labelWidget.setPropertyExpand(Vector2b.FALSE);
		labelWidget.setPropertyGravity(Gravity.CENTER);
		labelWidget.setPropertyValue(label);
		out.setSubWidget(labelWidget);
		return out;
	}

	/**
	 * Periodic call to update graphic display.
	 * @param self The button instance
	 * @param event Time generic event
	 */
	protected static void periodicCall(final Button self, final EventTime event) {
		LOGGER.trace("Periodic call on Button({})", event);
		self.markToRedraw();
	}

	/** Periodic call handle to remove it when needed */
	protected Connection periodicConnectionHandle = new Connection();

	/** Stored connections from fluent API to prevent GC */
	private final List<Connection> fluentConnections = new ArrayList<>();

	private Uri propertyConfig = new Uri("THEME", "shape/Button.json", "ewol");

	@AknotSignal
	@AknotName(value = "down")
	@AknotDescription("Button is Down")
	public SignalEmpty signalDown = new SignalEmpty();

	@AknotSignal
	@AknotName(value = "up")
	@AknotDescription("Button is Up")
	public SignalEmpty signalUp = new SignalEmpty();

	@AknotSignal
	@AknotName(value = "click")
	@AknotDescription("Button is Clicked")
	public SignalEmpty signalClick = new SignalEmpty();

	@AknotSignal
	@AknotName(value = "enter")
	@AknotDescription("The cursor enters the button area")
	public SignalEmpty signalEnter = new SignalEmpty();

	@AknotSignal
	@AknotName(value = "leave")
	@AknotDescription("The cursor leaves the button area")
	public SignalEmpty signalLeave = new SignalEmpty();

	private boolean buttonPressed = false;
	private boolean mouseHover = false;

	/**
	 * Default constructor.
	 */
	public Button() {
		this.propertyCanFocus = true;
		setMouseLimit(1);
		setPropertyExpand(Vector2b.TRUE);
		setPropertyFill(Vector2b.TRUE);
		setPropertyBorderWidth(new DimensionInsets(4));
		setPropertyBorderColor(Color.BLACK);
		setPropertyColor(Color.WHITE);
		setPropertyPadding(new DimensionInsets(3));
		setPropertyMargin(new DimensionInsets(0));
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "config")
	@AknotDescription(value = "Configuration of the widget")
	public Uri getPropertyConfig() {
		return this.propertyConfig;
	}

	@Override
	protected boolean onEventEntry(final EventEntry event) {
		if (event.type() == KeyKeyboard.CHARACTER && event.status() == KeyStatus.down && event.getChar() == '\r') {
			this.signalClick.emit();
			return true;
		}
		return super.onEventEntry(event);
	}

	@Override
	public boolean onEventInput(final EventInput event) {
		final Vector2f relPos = relativePosition(event.pos());
		final boolean over = isInside(relPos);

		// Handle cursor leave event
		if (event.status() == KeyStatus.leave) {
			if (this.mouseHover) {
				this.mouseHover = false;
				this.signalLeave.emit();
			}
			this.buttonPressed = false;
			markToRedraw();
			return true;
		}

		// Handle hover detection (inputId == 0 means cursor movement without button press)
		if (event.inputId() == 0) {
			if (over && !this.mouseHover) {
				this.mouseHover = true;
				this.signalEnter.emit();
				markToRedraw();
			} else if (!over && this.mouseHover) {
				this.mouseHover = false;
				this.signalLeave.emit();
				markToRedraw();
			}
			return over;
		}

		// Only handle primary mouse button (inputId == 1)
		if (event.inputId() != 1) {
			return false;
		}

		if (KeyStatus.pressSingle == event.status() && over) {
			keepFocus();
			this.signalClick.emit();
			LOGGER.debug("Button click event generated");
			return true;
		}

		if (KeyStatus.down == event.status() && over) {
			keepFocus();
			this.buttonPressed = true;
			markToRedraw();
			this.signalDown.emit();
			return true;
		}

		if (KeyStatus.move == event.status()) {
			if (this.buttonPressed) {
				markToRedraw();
			}
			return over;
		}

		if (KeyStatus.up == event.status() && this.buttonPressed) {
			keepFocus();
			this.buttonPressed = false;
			this.signalUp.emit();
			markToRedraw();
			return true;
		}

		return false;
	}

	@Override
	protected void onLostFocus() {
		this.buttonPressed = false;
		LOGGER.trace("{}: Lost focus", this.name);
	}

	/**
	 * Check if the button is currently pressed.
	 * @return true if the button is pressed
	 */
	public boolean isPressed() {
		return this.buttonPressed;
	}

	/**
	 * Check if the mouse is hovering over the button.
	 * @return true if the mouse is over the button
	 */
	public boolean isHovered() {
		return this.mouseHover;
	}

	public void setPropertyConfig(final Uri propertyConfig) {
		if (this.propertyConfig.equals(propertyConfig)) {
			return;
		}
		this.propertyConfig = propertyConfig;
		markToRedraw();
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	/**
	 * Create a new Button.
	 * @return a new Button
	 */
	public static Button create() {
		return new Button();
	}

	/**
	 * Create a new Button with a label.
	 * @param label the text to display
	 * @return a new Button with label
	 */
	public static Button create(final String label) {
		return createLabelButton(label);
	}

	/**
	 * Fluent method to set button label (creates a Label child).
	 * @param label the text to display
	 * @return this button for chaining
	 */
	public Button label(final String label) {
		final Label labelWidget = new Label(label);
		labelWidget.setPropertyFontSize(12);
		labelWidget.setPropertyFill(Vector2b.FALSE);
		labelWidget.setPropertyExpand(Vector2b.FALSE);
		labelWidget.setPropertyGravity(Gravity.CENTER);
		setSubWidget(labelWidget);
		return this;
	}

	/**
	 * Fluent method to connect a click callback.
	 * @param callback the callback to invoke when clicked
	 * @return this button for chaining
	 */
	public Button onClick(final Runnable callback) {
		this.fluentConnections.add(this.signalClick.connect(callback));
		return this;
	}

	/**
	 * Fluent method to connect a down callback.
	 * @param callback the callback to invoke when pressed down
	 * @return this button for chaining
	 */
	public Button onDown(final Runnable callback) {
		this.fluentConnections.add(this.signalDown.connect(callback));
		return this;
	}

	/**
	 * Fluent method to connect an up callback.
	 * @param callback the callback to invoke when released
	 * @return this button for chaining
	 */
	public Button onUp(final Runnable callback) {
		this.fluentConnections.add(this.signalUp.connect(callback));
		return this;
	}
}
