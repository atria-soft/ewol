package org.atriasoft.ewol.widget;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlProperty;
import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.Signal;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.DimensionInsets;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.Padding;
import org.atriasoft.ewol.compositing.CompositingSVG;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.gale.key.KeyStatus;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Tick widget (checkbox indicator) that can be toggled on/off.
 *
 * Signals emitted:
 * - signalDown: when tick is pressed down
 * - signalUp: when tick is released
 * - signalClick: when tick is clicked
 * - signalValue: when tick value changes (emits the new boolean value)
 */
public class Tick extends Box {
	private static final Logger LOGGER = LoggerFactory.getLogger(Tick.class);
	
	private final CompositingSVG compositingTick = new CompositingSVG();
	private final Uri uriCheckIcon = new Uri("THEME", "CheckBoxCrossRed.svg", "ewol");
	
	/** Periodic call handle to remove it when needed */
	protected Connection periodicConnectionHandle = new Connection();
	
	private boolean propertyValue = false;
	private boolean isDown = false;
	private boolean mouseHover = false;
	
	public SignalEmpty signalDown = new SignalEmpty();

	public SignalEmpty signalUp = new SignalEmpty();

	public SignalEmpty signalClick = new SignalEmpty();

	public Signal<Boolean> signalValue = new Signal<>();
	
	/**
	 * Default constructor.
	 */
	public Tick() {
		this.propertyCanFocus = true;
		markToRedraw();
		setMouseLimit(1);
		setPropertyExpand(Vector2b.FALSE);
		setPropertyFill(Vector2b.TRUE);
		setPropertyMinSize(new Dimension2f(new Vector2f(32f, 32f)));
		setPropertyBorderWidth(new DimensionInsets(4));
		setPropertyBorderColor(Color.BLACK);
		setPropertyColor(Color.WHITE);
		setPropertyPadding(new DimensionInsets(3));
		setPropertyMargin(new DimensionInsets(0));
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
		LOGGER.trace("min size = {}", this.minSize);
	}
	
	@JsonProperty("value")
	@JacksonXmlProperty(isAttribute = true, localName = "value")
	public Boolean getPropertyValue() {
		return this.propertyValue;
	}
	
	protected void onChangePropertyValue() {
		markToRedraw();
	}
	
	@Override
	protected void onDraw() {
		super.onDraw();
		if (this.propertyValue) {
			this.compositingTick.draw(true);
		}
	}
	
	@Override
	public boolean onEventInput(final EventInput event) {
		final Vector2f relPos = relativePosition(event.pos());
		LOGGER.trace("Event on Input: {} relPos = {}", event, relPos);
		final boolean over = isInside(relPos);
		
		// Handle cursor leave
		if (event.status() == KeyStatus.leave) {
			this.isDown = false;
			this.mouseHover = false;
			markToRedraw();
			return true;
		}
		
		// Handle hover detection (inputId == 0 means cursor movement without button press)
		if (event.inputId() == 0) {
			if (over != this.mouseHover) {
				this.mouseHover = over;
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
			setPropertyValue(!this.propertyValue);
			return true;
		}
		
		if (KeyStatus.down == event.status() && over) {
			keepFocus();
			this.isDown = true;
			markToRedraw();
			this.signalDown.emit();
			return true;
		}
		
		if (KeyStatus.move == event.status()) {
			if (this.isDown) {
				markToRedraw();
			}
			return over;
		}
		
		if (KeyStatus.up == event.status() && this.isDown) {
			keepFocus();
			this.isDown = false;
			this.signalUp.emit();
			markToRedraw();
			return true;
		}
		
		return false;
	}
	
	@Override
	protected void onLostFocus() {
		this.isDown = false;
	}
	
	@Override
	public void onRegenerateDisplay() {
		if (!needRedraw()) {
			return;
		}
		regenerateDisplay();
	}
	
	@Override
	public void regenerateDisplay() {
		super.regenerateDisplay();
		final Vector2f tickSize = this.overPositionStop.less(this.overPositionStart);
		this.compositingTick.setSource(Uri.getAllDataString(this.uriCheckIcon), tickSize.toVector2i());
		this.compositingTick.setPos(this.overPositionStart.add(2));
		this.compositingTick.print(tickSize.less(4));
		this.compositingTick.flush();
	}
	
	public void setPropertyValue(final Boolean propertyValue) {
		if (Boolean.valueOf(this.propertyValue).equals(propertyValue)) {
			return;
		}
		this.propertyValue = propertyValue != null && propertyValue;
		this.signalValue.emit(this.propertyValue);
		onChangePropertyValue();
	}
	
	/**
	 * Toggle the tick value.
	 */
	public void toggle() {
		setPropertyValue(!this.propertyValue);
	}
	
	/**
	 * Check if the tick is currently checked.
	 * @return true if checked
	 */
	public boolean isChecked() {
		return this.propertyValue;
	}
	
	/**
	 * Check if the tick is currently pressed.
	 * @return true if pressed
	 */
	public boolean isPressed() {
		return this.isDown;
	}
	
	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================
	
	/**
	 * Create a new Tick.
	 * @return a new Tick
	 */
	public static Tick create() {
		return new Tick();
	}
	
	/**
	 * Fluent method to set checked state.
	 * @param checked true to check
	 * @return this tick for chaining
	 */
	public Tick checked(final boolean checked) {
		setPropertyValue(checked);
		return this;
	}
	
	/**
	 * Fluent method to connect a value change callback.
	 * @param callback the callback to invoke when value changes
	 * @return this tick for chaining
	 */
	public Tick onValueChange(final java.util.function.Consumer<Boolean> callback) {
		this.signalValue.connect(callback::accept);
		return this;
	}
	
	/**
	 * Fluent method to connect a click callback.
	 * @param callback the callback to invoke when clicked
	 * @return this tick for chaining
	 */
	public Tick onClick(final Runnable callback) {
		this.signalClick.connect(callback);
		return this;
	}
}
