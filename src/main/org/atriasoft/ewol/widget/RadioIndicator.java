package org.atriasoft.ewol.widget;

import org.atriasoft.esignal.Signal;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.DimensionInsets;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.Padding;
import org.atriasoft.ewol.compositing.CompositingGC;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.gale.key.KeyStatus;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * RadioIndicator widget — circular indicator for radio button selection.
 * Displays an empty circle when unselected and a filled dot inside when selected.
 *
 * <p>Unlike {@link Tick}, clicking this widget always sets the value to {@code true}
 * (deselection is managed by {@link RadioGroup}).</p>
 *
 * Signals emitted:
 * - signalDown: when indicator is pressed down
 * - signalUp: when indicator is released
 * - signalClick: when indicator is clicked
 * - signalValue: when value changes (emits the new boolean value)
 */
public class RadioIndicator extends Box {
	private static final Logger LOGGER = LoggerFactory.getLogger(RadioIndicator.class);

	private static final Color BORDER_COLOR = Color.BLACK;
	private static final Color BACKGROUND_COLOR = Color.WHITE;
	private static final Color SELECTED_COLOR = new Color(0x21, 0x96, 0xF3, 0xFF);

	private final CompositingGC compositingCircle = new CompositingGC();

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
	public RadioIndicator() {
		this.propertyCanFocus = true;
		markToRedraw();
		setMouseLimit(1);
		setPropertyExpand(Vector2b.FALSE);
		setPropertyFill(Vector2b.TRUE);
		setPropertyMinSize(new Dimension2f(new Vector2f(16f, 16f)));
		setPropertyBorderWidth(new DimensionInsets(0));
		setPropertyPadding(new DimensionInsets(1));
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

	public Boolean getPropertyValue() {
		return this.propertyValue;
	}

	public void setPropertyValue(final Boolean value) {
		final boolean newValue = value != null && value;
		if (this.propertyValue == newValue) {
			return;
		}
		this.propertyValue = newValue;
		this.signalValue.emit(this.propertyValue);
		markToRedraw();
	}

	public boolean isSelected() {
		return this.propertyValue;
	}

	@Override
	protected void onDraw() {
		super.onDraw();
		this.compositingCircle.draw(true);
	}

	@Override
	public boolean onEventInput(final EventInput event) {
		final Vector2f relPos = relativePosition(event.pos());
		LOGGER.trace("Event on Input: {} relPos = {}", event, relPos);
		final boolean over = isInside(relPos);

		if (event.status() == KeyStatus.leave) {
			this.isDown = false;
			this.mouseHover = false;
			markToRedraw();
			return true;
		}

		if (event.inputId() == 0) {
			if (over != this.mouseHover) {
				this.mouseHover = over;
				markToRedraw();
			}
			return over;
		}

		if (event.inputId() != 1) {
			return false;
		}

		if (KeyStatus.pressSingle == event.status() && over) {
			keepFocus();
			this.signalClick.emit();
			// Radio: always set to true (deselection is managed by RadioGroup)
			if (!this.propertyValue) {
				setPropertyValue(true);
			}
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
		this.compositingCircle.clear();

		final Vector2f areaStart = this.overPositionStart;
		final Vector2f areaStop = this.overPositionStop;
		final Vector2f areaSize = areaStop.less(areaStart);
		final float side = Math.min(areaSize.x(), areaSize.y());
		final float centerX = areaStart.x() + areaSize.x() * 0.5f;
		final float centerY = areaStart.y() + areaSize.y() * 0.5f;
		final float outerRadius = side * 0.5f - 1f;
		final float borderThickness = 1.5f;

		// Draw outer circle border
		this.compositingCircle.setColor(BORDER_COLOR);
		this.compositingCircle.setPos(new Vector2f(centerX, centerY));
		this.compositingCircle.setThickness(borderThickness);
		this.compositingCircle.setColorBg(BACKGROUND_COLOR);
		this.compositingCircle.circle(outerRadius);

		// Draw inner filled dot when selected
		if (this.propertyValue) {
			final float innerRadius = outerRadius * 0.5f;
			this.compositingCircle.setColor(SELECTED_COLOR);
			this.compositingCircle.setColorBg(SELECTED_COLOR);
			this.compositingCircle.setThickness(0);
			this.compositingCircle.setPos(new Vector2f(centerX, centerY));
			this.compositingCircle.circle(innerRadius);
		}

		this.compositingCircle.flush();
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	public static RadioIndicator create() {
		return new RadioIndicator();
	}

	public RadioIndicator checked(final boolean checked) {
		setPropertyValue(checked);
		return this;
	}

	public RadioIndicator onValueChange(final java.util.function.Consumer<Boolean> callback) {
		this.signalValue.connect(callback::accept);
		return this;
	}

	public RadioIndicator onClick(final Runnable callback) {
		this.signalClick.connect(callback);
		return this;
	}
}
