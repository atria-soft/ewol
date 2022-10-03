package org.atriasoft.ewol.widget;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.aknot.annotation.AknotSignal;
import org.atriasoft.esignal.Signal;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.etk.math.Vector3i;
import org.atriasoft.ewol.Padding;
import org.atriasoft.ewol.compositing.CompositingDrawing;
import org.atriasoft.ewol.compositing.GuiShape;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.gale.key.KeyStatus;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

/**
 * @ingroup ewolWidgetGroup
 */
public class Slider extends Widget {
	
	private Uri propertyConfig = new Uri("THEME", "shape/Slider.json", "ewol");
	
	private Float propertyValue = 0.0f; //!< string that must be displayed
	private GuiShape shape = null;
	private final GuiShape shapeTop = null;
	@AknotSignal
	@AknotName("value")
	@AknotDescription("Tick value change")
	public Signal<Float> signalValue = new Signal<>();
	// element over:
	Vector3f overPositionStart = Vector3f.ZERO;
	Vector3f overPositionStop = Vector3f.ZERO;
	Vector3f overCursorPositionStart = Vector3f.ZERO;
	Vector3f overCursorPositionStop = Vector3f.ZERO;
	
	//@AknotAutoGenerateProperty("minimum", "configuration of the widget")
	private Float propertyMinimum = 0.0f;
	
	private Float propertyMaximum = 10.0f;
	
	private Float propertyStep = 0.1f;
	
	private final Color textColorFg = Color.BLACK; //!< Text color
	
	private final Color textColorBg = Color.BLACK.withA(0x3F); //!< Background color
	
	CompositingDrawing draw = new CompositingDrawing(); //!< drawing tool.
	
	public Slider() {
		this.propertyCanFocus = true;
		onChangePropertyShaper();
		markToRedraw();
		// Limit event at 1:
		setMouseLimit(1);
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
	@AknotName("maximum")
	@AknotDescription("Maximum value of the slider")
	public Float getPropertyMaximum() {
		return this.propertyMaximum;
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName("minimum")
	@AknotDescription("Minimum value of the slider")
	public Float getPropertyMinimum() {
		return this.propertyMinimum;
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName("step")
	@AknotDescription("Step value of the slider")
	public Float getPropertyStep() {
		return this.propertyStep;
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName("value")
	@AknotDescription("Value of the slider")
	public Float getPropertyValue() {
		return this.propertyValue;
	}
	
	protected void onChangePropertyShaper() {
		if (this.shape == null) {
			this.shape = new GuiShape(this.propertyConfig);
		} else {
			this.shape.setSource(this.propertyConfig);
		}
	}
	
	@Override
	public void onDraw() {
		if (this.shape != null) {
			// draw background
			this.shape.draw(true, 0);
			// draw slider
			this.shape.draw(true, 1);
		}
		
	}
	
	@Override
	public boolean onEventInput(final EventInput event) {
		final Vector3f positionAbsolute = new Vector3f(event.pos().x(), event.pos().y(), 0);
		final Vector3f relPos = relativePosition(positionAbsolute);
		Log.warning("Event on Input ... " + event + " relPos = " + relPos);
		final boolean over = checkIfOver(relPos);
		if (event.inputId() != 1) {
			return false;
		}
		if (KeyStatus.pressSingle == event.status() && over) {
			keepFocus();
			// get percent value
			final float pourcent = (relPos.x() - this.overPositionStart.x()) / (this.overPositionStop.x() - this.overPositionStart.x());
			float value = (this.propertyMaximum - this.propertyMinimum) * pourcent + this.propertyMinimum;
			if (this.propertyStep != 0.0f) {
				value += this.propertyStep * 0.5f;
			}
			setPropertyValue(value);
			return true;
		}
		if (KeyStatus.down == event.status() && over) {
			keepFocus();
			// get percent value
			final float pourcent = (relPos.x() - this.overPositionStart.x()) / (this.overPositionStop.x() - this.overPositionStart.x());
			float value = (this.propertyMaximum - this.propertyMinimum) * pourcent + this.propertyMinimum;
			if (this.propertyStep != 0.0f) {
				value += this.propertyStep * 0.5f;
			}
			setPropertyValue(value);
			return true;
		}
		if (KeyStatus.move == event.status() && over) {
			keepFocus();
			// get percent value
			final float pourcent = (relPos.x() - this.overPositionStart.x()) / (this.overPositionStop.x() - this.overPositionStart.x());
			float value = (this.propertyMaximum - this.propertyMinimum) * pourcent + this.propertyMinimum;
			if (this.propertyStep != 0.0f) {
				value += this.propertyStep * 0.5f;
			}
			setPropertyValue(value);
			return true;
		}
		if (KeyStatus.up == event.status() && over) {
			keepFocus();
			// get percent value
			final float pourcent = (relPos.x() - this.overPositionStart.x()) / (this.overPositionStop.x() - this.overPositionStart.x());
			float value = (this.propertyMaximum - this.propertyMinimum) * pourcent + this.propertyMinimum;
			if (this.propertyStep != 0.0f) {
				value += this.propertyStep * 0.5f;
			}
			setPropertyValue(value);
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
		/*
		if (this.colorIdTextFg >= 0) {
			//this.text.setDefaultColorFg(this.shape.getColor(this.colorIdTextFg));
			//this.text.setDefaultColorBg(this.shape.getColor(this.colorIdTextBg));
			//this.text.setCursorColor(this.shape.getColor(this.colorIdCursor));
			//this.text.setSelectionColor(this.shape.getColor(this.colorIdSelection));
		}
		*/
		final Padding padding = this.shape.getPadding();
		{
			// Manage external shape:
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
			Vector3f tmpOriginInside = Vector3f.ZERO;
			// sometimes, the user define an height bigger than the real size needed  == > in this case we need to center the text in the shaper ...
			// fix all the position in the int class:
			tmpSizeShaper = Vector3f.clipInt(tmpSizeShaper);
			tmpOriginShaper = Vector3f.clipInt(tmpOriginShaper);
			tmpSizeInside = Vector3f.clipInt(tmpSizeInside);
			tmpOriginInside = Vector3f.clipInt(tmpOriginInside);
			
			this.overPositionStart = tmpOriginShaper;
			this.overPositionStop = tmpOriginShaper.add(tmpSizeShaper);
			this.shape.setShape(0, tmpOriginShaper, tmpSizeShaper, tmpOriginInside, tmpSizeInside);
		}
		{
			// Manage cursor:
			Vector3f tmpSizeShaper = this.minSize;
			Vector3f delta = this.propertyGravity.gravityGenerateDelta(this.size.less(this.minSize));
			if (this.propertyFill.y()) {
				tmpSizeShaper = tmpSizeShaper.withY(this.size.y());
				delta = delta.withY(0.0f);
			}
			
			Vector3f tmpOriginShaper = delta;
			Vector3f tmpSizeInside = tmpSizeShaper.less(padding.x(), padding.y(), padding.z());
			//Vector3f tmpOriginText = this.size.less(tmpSizeText).multiply(0.5f);
			Vector3f tmpOriginInside = Vector3f.ZERO;
			
			final float xxx = tmpOriginShaper.x() * 2.0f;
			
			tmpOriginShaper = tmpOriginShaper.withX(xxx * (this.propertyValue - this.propertyMinimum) / (this.propertyMaximum - this.propertyMinimum));
			// sometimes, the user define an height bigger than the real size needed  == > in this case we need to center the text in the shaper ...
			// fix all the position in the int class:
			tmpSizeShaper = Vector3f.clipInt(tmpSizeShaper);
			tmpOriginShaper = Vector3f.clipInt(tmpOriginShaper);
			tmpSizeInside = Vector3f.clipInt(tmpSizeInside);
			tmpOriginInside = Vector3f.clipInt(tmpOriginInside);
			
			this.overCursorPositionStart = tmpOriginShaper;
			this.overCursorPositionStop = tmpOriginShaper.add(tmpSizeShaper);
			this.shape.setShape(1, tmpOriginShaper, tmpSizeShaper, tmpOriginInside, tmpSizeInside);
		}
		//this.gc.flush();
		this.shape.flush();
		
	}
	
	public void setPropertyConfig(final Uri propertyConfig) {
		if (this.propertyConfig.equals(propertyConfig)) {
			return;
		}
		this.propertyConfig = propertyConfig;
		onChangePropertyShaper();
	}
	
	public void setPropertyMaximum(final Float propertyMaximum) {
		if (this.propertyMaximum == propertyMaximum) {
			return;
		}
		this.propertyMaximum = propertyMaximum;
		updateValue(this.propertyValue);
		this.signalValue.emit(this.propertyValue);
	}
	
	public void setPropertyMinimum(final Float propertyMinimum) {
		if (this.propertyMinimum == propertyMinimum) {
			return;
		}
		this.propertyMinimum = propertyMinimum;
		updateValue(this.propertyValue);
		this.signalValue.emit(this.propertyValue);
	}
	
	public void setPropertyStep(final Float propertyStep) {
		if (this.propertyStep == propertyStep) {
			return;
		}
		this.propertyStep = propertyStep;
		updateValue(this.propertyValue);
		this.signalValue.emit(this.propertyValue);
	}
	
	public void setPropertyValue(final Float propertyValue) {
		if (this.propertyValue == propertyValue) {
			return;
		}
		this.propertyValue = propertyValue;
		updateValue(this.propertyValue);
		this.signalValue.emit(this.propertyValue);
	}
	
	protected void updateValue(float newValue) {
		newValue = FMath.max(FMath.min(newValue, this.propertyMaximum), this.propertyMinimum);
		if (this.propertyStep == 0.0f) {
			this.propertyValue = newValue;
		} else {
			final float basicVal = (long) (newValue / this.propertyStep);
			this.propertyValue = basicVal * this.propertyStep;
		}
		markToRedraw();
	}
}