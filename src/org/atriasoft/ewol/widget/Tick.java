package org.atriasoft.ewol.widget;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.aknot.annotation.AknotSignal;
import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.Signal;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.etk.math.Vector3i;
import org.atriasoft.ewol.Padding;
import org.atriasoft.ewol.compositing.GuiShape;
import org.atriasoft.ewol.compositing.GuiShapeMode;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.event.EventTime;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.object.EwolObject;
import org.atriasoft.gale.key.KeyStatus;

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
public class Tick extends Widget {
	/**
	 * Periodic call to update grapgic display
	 * @param _event Time generic event
	 */
	protected static void periodicCall(final Tick self, final EventTime event) {
		Log.verbose("Periodic call on Entry(" + event + ")");
		if (!self.shape.periodicCall(event)) {
			//Log.error("end periodic call");
			self.periodicConnectionHanble.close();
		}
		self.markToRedraw();
	}
	
	/// color property of the text foreground
	private int colorIdTextFg;
	/// text display this.text
	//private final CompositingGraphicContext gc = new CompositingGraphicContext();
	/// Periodic call handle to remove it when needed
	protected Connection periodicConnectionHanble = new Connection();
	
	private Uri propertyConfig = new Uri("THEME", "shape/Tick.json", "ewol");
	
	private Boolean propertyValue = false; //!< string that must be displayed
	private GuiShape shape;
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
	Vector3f overPositionStart = Vector3f.ZERO;
	Vector3f overPositionStop = Vector3f.ZERO;
	
	private boolean isDown;
	
	/**
	 * Constuctor
	 */
	public Tick() {
		this.propertyCanFocus = true;
		onChangePropertyShaper();
		markToRedraw();
		// can not support multiple click...
		setMouseLimit(1);
		this.shape = new GuiShape(this.propertyConfig);
	}
	
	@Override
	public void calculateMinMaxSize() {
		// call main class
		super.calculateMinMaxSize();
		// get generic padding
		Padding padding = Padding.ZERO;
		if (this.shape != null) {
			padding = this.shape.getPadding();
		}
		final Vector3i minHeight = Vector3i.VALUE_16;
		
		Vector3f minimumSizeBase = new Vector3f(minHeight.x(), minHeight.y(), minHeight.z());
		// add padding :
		minimumSizeBase = minimumSizeBase.add(padding.x(), padding.y(), padding.z());
		this.minSize = Vector3f.max(this.minSize, minimumSizeBase);
		// verify the min max of the min size ...
		checkMinSize();
		Log.error("min size = " + this.minSize);
	}
	
	protected void changeStatusIn(final GuiShapeMode newStatusId) {
		if (this.shape.changeStatusIn(newStatusId)) {
			if (!this.periodicConnectionHanble.isConnected()) {
				//Log.error("REQUEST: connection on periodic call");
				this.periodicConnectionHanble = EwolObject.getObjectManager().periodicCall.connect(this, Tick::periodicCall);
			}
			markToRedraw();
		}
	}
	
	private boolean checkIfOver(final Vector3f relPos) {
		return relPos.x() > this.overPositionStart.x() && relPos.y() > this.overPositionStart.y() && relPos.x() < this.overPositionStop.x() && relPos.y() < this.overPositionStop.y();
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName("config")
	@AknotDescription("configuration of the widget")
	public Uri getPropertyConfig() {
		return this.propertyConfig;
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName("value")
	@AknotDescription("State of the Tick")
	public Boolean getPropertyValue() {
		return this.propertyValue;
	}
	
	protected void onChangePropertyShaper() {
		if (this.shape == null) {
			this.shape = new GuiShape(this.propertyConfig);
		} else {
			this.shape.setSource(this.propertyConfig);
		}
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
		if (this.shape != null) {
			this.shape.draw(true, this.propertyValue ? 0 : 1);
		}
	}
	
	@Override
	public boolean onEventInput(final EventInput event) {
		final Vector3f positionAbsolute = new Vector3f(event.pos().x(), event.pos().y(), 0);
		final Vector3f relPos = relativePosition(positionAbsolute);
		Log.warning("Event on Input ... " + event + " relPos = " + relPos);
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
					Log.verbose("Detect Over : " + this.overPositionStart + " -> " + this.overPositionStop);
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
			this.signalClick.emit();
			this.propertyValue = !this.propertyValue;
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
		if (!needRedraw()) {
			//return;
		}
		//Log.verbose("Regenerate Display ==> is needed: '" + this.propertyValue + "'");
		this.shape.clear();
		//this.gc.clear();
		if (this.colorIdTextFg >= 0) {
			//this.text.setDefaultColorFg(this.shape.getColor(this.colorIdTextFg));
			//this.text.setDefaultColorBg(this.shape.getColor(this.colorIdTextBg));
			//this.text.setCursorColor(this.shape.getColor(this.colorIdCursor));
			//this.text.setSelectionColor(this.shape.getColor(this.colorIdSelection));
		}
		final Padding padding = this.shape.getPadding();
		
		Vector3f tmpSizeShaper = this.minSize;
		Vector3f delta = this.propertyGravity.gravityGenerateDelta(this.size.less(this.minSize));
		if (this.propertyFill.x()) {
			tmpSizeShaper = tmpSizeShaper.withX(this.size.x());
			delta = delta.withX(0.0f);
		}
		if (this.propertyFill.y()) {
			tmpSizeShaper = tmpSizeShaper.withY(this.size.y());
			delta = delta.withY(0.0f);
		}
		
		Vector3f tmpOriginShaper = delta;
		Vector3f tmpSizeInside = tmpSizeShaper.less(padding.x(), padding.y(), padding.z());
		//Vector3f tmpOriginText = this.size.less(tmpSizeText).multiply(0.5f);
		Vector3f tmpOriginInside = Vector3f.ZERO;//this.gc.getTextSize());
		// sometimes, the user define an height bigger than the real size needed  == > in this case we need to center the text in the shaper ...
		/*
		int minHeight = this.gc.getTextHeight();
		if (tmpSizeText.y() > minHeight) {
			tmpOriginText = tmpOriginText.add(0, (tmpSizeText.y() - minHeight) * 0.5f);
		}
		*/
		// fix all the position in the int class:
		tmpSizeShaper = Vector3f.clipInt(tmpSizeShaper);
		tmpOriginShaper = Vector3f.clipInt(tmpOriginShaper);
		tmpSizeInside = Vector3f.clipInt(tmpSizeInside);
		tmpOriginInside = Vector3f.clipInt(tmpOriginInside);
		
		//this.gc.clear();
		//this.gc.setSize((int)tmpSizeText.x(), (int)tmpSizeText.y());
		
		//this.gc.setColorFill(Color.BLACK);
		//this.gc.setColorStroke(Color.NONE);
		//this.gc.setStrokeWidth(1);
		//this.gc.text(tmpOriginText, this.propertyValue);
		this.overPositionStart = tmpOriginShaper;
		this.overPositionStop = tmpOriginShaper.add(tmpSizeShaper);
		this.shape.setShape(tmpOriginShaper, tmpSizeShaper, tmpOriginInside, tmpSizeInside);
		//this.gc.flush();
		this.shape.flush();
		
	}
	
	/**
	 * internal check the value with RegExp checking
	 * @param newData The new string to display
	 */
	protected void setInternalValue(final Boolean newData) {
		this.propertyValue = newData;
		markToRedraw();
	}
	
	public void setPropertyConfig(final Uri propertyConfig) {
		if (this.propertyConfig.equals(propertyConfig)) {
			return;
		}
		this.propertyConfig = propertyConfig;
		onChangePropertyShaper();
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
