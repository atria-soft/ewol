package org.atriasoft.ewol.widget;

import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.Padding;
import org.atriasoft.ewol.annotation.EwolDescription;
import org.atriasoft.ewol.annotation.EwolSignal;
import org.atriasoft.ewol.compositing.CompositingGraphicContext;
import org.atriasoft.ewol.compositing.GuiShape;
import org.atriasoft.ewol.compositing.GuiShapeMode;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.event.EventTime;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.object.EwolObject;
import org.atriasoft.exml.annotation.XmlAttribute;
import org.atriasoft.exml.annotation.XmlManaged;
import org.atriasoft.exml.annotation.XmlName;
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
public class Button extends Widget {
	/**
	 * Periodic call to update grapgic display
	 * @param _event Time generic event
	 */
	protected static void periodicCall(final Button self, final EventTime event) {
		Log.verbose("Periodic call on Entry(" + event + ")");
		if (!self.shape.periodicCall(event)) {
			self.periodicConnectionHanble.close();
		}
		self.markToRedraw();
	}
	
	/// color property of the text foreground
	private int colorIdTextFg;
	/// text display this.text
	private final CompositingGraphicContext gc = new CompositingGraphicContext();
	/// Periodic call handle to remove it when needed
	protected Connection periodicConnectionHanble = new Connection();
	
	private Uri propertyConfig = new Uri("THEME", "shape/Button.json", "ewol");
	
	private String propertyValue = "Test Text..."; //!< string that must be displayed
	private GuiShape shape;
	@EwolSignal(name = "down", description = "Button is Down")
	public SignalEmpty signalDown = new SignalEmpty();
	@EwolSignal(name = "up", description = "Button is Up")
	public SignalEmpty signalUp = new SignalEmpty();
	
	@EwolSignal(name = "click", description = "Button is Clicked")
	public SignalEmpty signalClick = new SignalEmpty();
	// element over:
	Vector2f overPositionStart = Vector2f.ZERO;
	Vector2f overPositionStop = Vector2f.ZERO;
	
	private boolean isDown;
	
	/**
	 * Constuctor
	 */
	public Button() {
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
		Vector2i minHeight = this.gc.calculateTextSize(this.propertyValue);
		
		Vector2f minimumSizeBase = new Vector2f(minHeight.x(), minHeight.y());
		// add padding :
		minimumSizeBase = minimumSizeBase.add(padding.x(), padding.y());
		this.minSize = Vector2f.max(this.minSize, minimumSizeBase);
		// verify the min max of the min size ...
		checkMinSize();
		Log.error("min size = " + this.minSize);
	}
	
	protected void changeStatusIn(final GuiShapeMode newStatusId) {
		if (this.shape.changeStatusIn(newStatusId)) {
			if (!this.periodicConnectionHanble.isConnected()) {
				//Log.error("REQUEST: connection on operiodic call");
				this.periodicConnectionHanble = EwolObject.getObjectManager().periodicCall.connect(this, Button::periodicCall);
			}
			markToRedraw();
		}
	}
	
	private boolean checkIfOver(Vector2f relPos) {
		return relPos.x() > this.overPositionStart.x() && relPos.y() > this.overPositionStart.y() && relPos.x() < this.overPositionStop.x() && relPos.y() < this.overPositionStop.y();
	}
	
	@XmlManaged
	@XmlAttribute
	@XmlName(value = "config")
	@EwolDescription(value = "configuration of the widget")
	public Uri getPropertyConfig() {
		return this.propertyConfig;
	}
	
	@XmlManaged
	@XmlAttribute
	@XmlName(value = "value")
	@EwolDescription(value = "Value display in the entry (decorated text)")
	public String getPropertyValue() {
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
		String newData = this.propertyValue;
		markToRedraw();
	}
	
	@Override
	protected void onDraw() {
		if (this.shape != null) {
			this.shape.draw(this.gc.getResourceTexture(), true);
		}
	}
	
	@Override
	public boolean onEventInput(final EventInput event) {
		Vector2f relPos = relativePosition(event.pos());
		Log.warning("Event on Input ... " + event + " relPos = " + relPos);
		boolean over = checkIfOver(relPos);
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
		this.gc.clear();
		if (this.colorIdTextFg >= 0) {
			//this.text.setDefaultColorFg(this.shape.getColor(this.colorIdTextFg));
			//this.text.setDefaultColorBg(this.shape.getColor(this.colorIdTextBg));
			//this.text.setCursorColor(this.shape.getColor(this.colorIdCursor));
			//this.text.setSelectionColor(this.shape.getColor(this.colorIdSelection));
		}
		Padding padding = this.shape.getPadding();
		
		Vector2f tmpSizeShaper = this.minSize;
		Vector2f delta = Gravity.gravityGenerateDelta(this.propertyGravity, this.size.less(this.minSize));
		if (this.propertyFill.x()) {
			tmpSizeShaper = tmpSizeShaper.withX(this.size.x());
			delta = delta.withX(0.0f);
		}
		if (this.propertyFill.y()) {
			tmpSizeShaper = tmpSizeShaper.withY(this.size.y());
			delta = delta.withY(0.0f);
		}
		
		Vector2f tmpOriginShaper = delta;
		Vector2f tmpSizeText = tmpSizeShaper.less(padding.x(), padding.y());
		//Vector2f tmpOriginText = this.size.less(tmpSizeText).multiply(0.5f);
		Vector2f tmpOriginText = new Vector2f(0, this.gc.getTextSize());
		// sometimes, the user define an height bigger than the real size needed  == > in this case we need to center the text in the shaper ...
		/*
		int minHeight = this.gc.getTextHeight();
		if (tmpSizeText.y() > minHeight) {
			tmpOriginText = tmpOriginText.add(0, (tmpSizeText.y() - minHeight) * 0.5f);
		}
		*/
		// fix all the position in the int class:
		tmpSizeShaper = Vector2f.clipInt(tmpSizeShaper);
		tmpOriginShaper = Vector2f.clipInt(tmpOriginShaper);
		tmpSizeText = Vector2f.clipInt(tmpSizeText);
		tmpOriginText = Vector2f.clipInt(tmpOriginText);
		
		this.gc.clear();
		this.gc.setSize((int) tmpSizeText.x(), (int) tmpSizeText.y());
		
		this.gc.setColorFill(Color.BLACK);
		this.gc.setColorStroke(Color.NONE);
		this.gc.setStrokeWidth(1);
		this.gc.text(tmpOriginText, this.propertyValue);
		this.overPositionStart = tmpOriginShaper;
		this.overPositionStop = tmpOriginShaper.add(tmpSizeShaper);
		this.shape.setShape(tmpOriginShaper, tmpSizeShaper, tmpOriginText, tmpSizeText);
		this.gc.flush();
		this.shape.flush();
		
	}
	
	/**
	 * internal check the value with RegExp checking
	 * @param newData The new string to display
	 */
	protected void setInternalValue(final String newData) {
		String previous = this.propertyValue;
		// check the RegExp :
		if (newData.length() > 0) {
			/*
			if (this.regex.parse(_newData, 0, _newData.size()) == false) {
				Log.info("The input data does not match with the regExp '" + _newData + "' Regex='" + propertyRegex + "'" );
				return;
			}
			if (this.regex.start() != 0) {
				Log.info("The input data does not match with the regExp '" + _newData + "' Regex='" + propertyRegex + "' (start position error)" );
				return;
			}
			if (this.regex.stop() != _newData.size()) {
				Log.info("The input data does not match with the regExp '" + _newData + "' Regex='" + propertyRegex + "' (stop position error)" );
				return;
			}
			*/
		}
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
	
	public void setPropertyValue(final String propertyValue) {
		if (this.propertyValue.equals(propertyValue)) {
			return;
		}
		this.propertyValue = propertyValue;
		onChangePropertyValue();
	}
	
}
