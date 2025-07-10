package org.atriasoft.ewol.widget;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.aknot.annotation.AknotSignal;
import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.Signal;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension1f;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.Padding;
import org.atriasoft.ewol.compositing.CompositingSVG;
import org.atriasoft.ewol.compositing.GuiShapeMode;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.gale.key.KeyStatus;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * @ingroup ewolWidgetGroup
 * Entry box display :
 *
 * ~~~~~~~~~~~~~~~~~~~~~~
 * 	----------------------------------------------
 * 	|                Text Label                  |
 * 	----------------------------------------------
 * ~~~~~~~~~~~~~~~~~~~~~~
 */
/*
public SignalEmpty signalPressed;
public SignalEmpty signalDown;
public SignalEmpty signalUp;
public SignalEmpty signalEnter;
public Signal<Boolean> signalValue;
	public boolean propertyValue; //!< Current state of the Tick.
public 	Uri> propertyShape; //!< shape of the widget
*/
public class Tick extends Box {
	private static final Logger LOGGER = LoggerFactory.getLogger(Tick.class);
	protected CompositingSVG compositingTick = new CompositingSVG();

	/// color property of the text foreground
	private int colorIdTextFg;
	/// text display this.text
	//private final CompositingGraphicContext gc = new CompositingGraphicContext();
	/// Periodic call handle to remove it when needed
	protected Connection periodicConnectionHanble = new Connection();
	
	private final Uri propertyConfig = new Uri("THEME", "shape/Tick.json", "ewol");
	private final Uri uriCheckGreen = new Uri("THEME", "CheckBoxCrossRed.svg", "ewol");
	
	private Boolean propertyValue = false; //!< string that must be displayed

	@AknotSignal
	@AknotName("down")
	@AknotDescription("Tick is Down")
	public SignalEmpty signalDown = new SignalEmpty();
	@AknotSignal
	@AknotName("up")
	@AknotDescription("Tick is Up")
	public SignalEmpty signalUp = new SignalEmpty();
	@AknotSignal
	@AknotName("click")
	@AknotDescription("Tick is Clicked")
	public SignalEmpty signalClick = new SignalEmpty();
	@AknotSignal
	@AknotName("value")
	@AknotDescription("Tick value change")
	public Signal<Boolean> signalValue = new Signal<>();
	// element over:
	Vector2f overPositionStart = Vector2f.ZERO;
	Vector2f overPositionStop = Vector2f.ZERO;
	
	private boolean isDown;
	
	/**
	 * Constuctor
	 */
	public Tick() {
		this.propertyCanFocus = true;
		markToRedraw();
		// can not support multiple click...
		setMouseLimit(1);
		setPropertyExpand(Vector2b.FALSE);
		setPropertyFill(Vector2b.TRUE);
		setPropertyMinSize(new Dimension2f(new Vector2f(32f, 32f)));
		setPropertyBorderWidth(new Dimension1f(4));
		//setPropertyBorderRadius(new Dimension1f(15));
		setPropertyBorderColor(Color.BLACK);
		setPropertyColor(Color.WHITE);
		setPropertyPadding(new Dimension2f(new Vector2f(3, 3)));
		setPropertyMargin(new Dimension2f(new Vector2f(0, 0)));

	}
	
	@Override
	public void calculateMinMaxSize() {
		// call main class
		super.calculateMinMaxSize();
		// get generic padding
		final Padding padding = Padding.ZERO;
		final Vector2i minHeight = Vector2i.VALUE_16;
		
		Vector2f minimumSizeBase = new Vector2f(minHeight.x(), minHeight.y());
		// add padding :
		minimumSizeBase = minimumSizeBase.add(padding.x(), padding.y());
		this.minSize = Vector2f.max(this.minSize, minimumSizeBase);
		// verify the min max of the min size ...
		checkMinSize();
		LOGGER.error("min size = " + this.minSize);
	}
	
	protected void changeStatusIn(final GuiShapeMode newStatusId) {
		//		if (this.shape.changeStatusIn(newStatusId)) {
		//			if (!this.periodicConnectionHanble.isConnected()) {
		//				//LOGGER.error("REQUEST: connection on periodic call");
		//				this.periodicConnectionHanble = EwolObject.getObjectManager().periodicCall.connect(this,
		//						Tick::periodicCall);
		//			}
		//			markToRedraw();
		//		}
	}
	
	private boolean checkIfOver(final Vector2f relPos) {
		return relPos.x() > this.overPositionStart.x() && relPos.y() > this.overPositionStart.y()
				&& relPos.x() < this.overPositionStop.x() && relPos.y() < this.overPositionStop.y();
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName("value")
	@AknotDescription("State of the Tick")
	public Boolean getPropertyValue() {
		return this.propertyValue;
	}
	
	protected void onChangePropertyTextWhenNothing() {
		markToRedraw();
	}
	
	protected void onChangePropertyValue() {
		//Boolean newData = this.propertyValue;
		markToRedraw();
	}
	
	@Override
	protected void onDraw() {
		super.onDraw();
		if (this.propertyValue) {
			if (this.compositingTick != null) {
				this.compositingTick.draw(true);
			}
		}
		//		if (this.shape != null) {
		//			this.shape.draw(true, this.propertyValue ? 0 : 1);
		//		}
	}
	
	@Override
	public boolean onEventInput(final EventInput event) {
		final Vector2f positionAbsolute = new Vector2f(event.pos().x(), event.pos().y());
		final Vector2f relPos = relativePosition(positionAbsolute);
		System.out.println("Event on Input ... " + event + " relPos = " + relPos);
		LOGGER.trace("Event on Input ... " + event + " relPos = " + relPos);
		final boolean over = checkIfOver(relPos);
		//filter if outside the element...
		if (event.status() == KeyStatus.leave) {
			changeStatusIn(GuiShapeMode.NORMAL);
			this.isDown = false;
			return true;
		}
		if (event.inputId() == 0) {
			if (!this.isDown) {
				if (KeyStatus.leave == event.status()) {
					changeStatusIn(GuiShapeMode.NORMAL);
				} else {
					LOGGER.trace("Detect Over : " + this.overPositionStart + " -> " + this.overPositionStop);
					if (over) {
						changeStatusIn(GuiShapeMode.OVER);
					} else {
						changeStatusIn(GuiShapeMode.NORMAL);
					}
				}
				return true;
			}
		}
		if (event.inputId() != 1) {
			return false;
		}
		if (KeyStatus.pressSingle == event.status() && over) {
			keepFocus();
			System.out.println("event ....");
			this.signalClick.emit();
			setPropertyValue(!this.propertyValue);
			return true;
		}
		if (KeyStatus.down == event.status() && over) {
			keepFocus();
			this.isDown = true;
			changeStatusIn(GuiShapeMode.SELECT);
			markToRedraw();
			this.signalDown.emit();
			return true;
		}
		if (KeyStatus.move == event.status() && over) {
			keepFocus();
			markToRedraw();
			return true;
		}
		if (KeyStatus.up == event.status() && this.isDown) {
			keepFocus();
			this.isDown = false;
			this.signalUp.emit();
			changeStatusIn(GuiShapeMode.OVER);
			markToRedraw();
			return true;
		}
		return false;
	}
	
	@Override
	public void onRegenerateDisplay() {
		super.onRegenerateDisplay();
		if (!needRedraw()) {
			//return;
		}
		this.compositingTick.setSource(Uri.getAllDataString(this.uriCheckGreen), this.renderSize.less(4));

		this.compositingTick.setPos(this.propertyMargin.size().add(2));
		this.compositingTick.print(this.renderSize.less(4));
		this.compositingTick.flush();
	}
	
	/**
	 * internal check the value with RegExp checking
	 * @param newData The new string to display
	 */
	protected void setInternalValue(final Boolean newData) {
		this.propertyValue = newData;
		markToRedraw();
	}
	
	public void setPropertyValue(final Boolean propertyValue) {
		if (this.propertyValue.equals(propertyValue)) {
			return;
		}
		this.propertyValue = propertyValue;
		this.signalValue.emit(this.propertyValue);
		onChangePropertyValue();
	}
	
}
