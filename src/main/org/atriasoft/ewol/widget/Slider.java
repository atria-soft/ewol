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
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.Padding;
import org.atriasoft.ewol.compositing.CompositingDrawing;
import org.atriasoft.ewol.compositing.CompositingSVG;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.gale.key.KeyStatus;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

/**
 * @ingroup ewolWidgetGroup
 */
public class Slider extends Widget {
	private static final Logger LOGGER = LoggerFactory.getLogger(Slider.class);

	private Uri propertyConfig = new Uri("THEME", "shape/Slider.json", "ewol");

	private Float propertyValue = 0.0f; //!< string that must be displayed
	protected CompositingSVG compositing = new CompositingSVG();
	@AknotSignal
	@AknotName("value")
	@AknotDescription("Tick value change")
	public Signal<Float> signalValue = new Signal<>();
	// element over:
	Vector2f overPositionStart = Vector2f.ZERO;
	Vector2f overPositionStop = Vector2f.ZERO;
	Vector2f overPositionSize = Vector2f.ZERO;
	Vector2f overCursorPositionStart = Vector2f.ZERO;
	Vector2f overCursorPositionStop = Vector2f.ZERO;
	Vector2f overCursorPositionSize = Vector2f.ZERO;

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

	private boolean checkIfOver(final Vector2f relPos) {
		return relPos.x() > this.overPositionStart.x() && relPos.y() > this.overPositionStart.y()
				&& relPos.x() < this.overPositionStop.x() && relPos.y() < this.overPositionStop.y();
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
		//		if (this.shape == null) {
		//			this.shape = new GuiShape(this.propertyConfig);
		//		} else {
		//			this.shape.setSource(this.propertyConfig);
		//		}
	}

	@Override
	public void onDraw() {
		this.compositing.draw();
	}

	@Override
	public boolean onEventInput(final EventInput event) {
		final Vector2f positionAbsolute = new Vector2f(event.pos().x(), event.pos().y());
		final Vector2f relPos = relativePosition(positionAbsolute);
		LOGGER.warn("Event on Input ... " + event + " relPos = " + relPos);
		final boolean over = checkIfOver(relPos);
		if (event.inputId() != 1) {
			return false;
		}
		if (KeyStatus.pressSingle == event.status() && over) {
			keepFocus();
			// get percent value
			final float pourcent = (relPos.x() - this.overPositionStart.x())
					/ (this.overPositionStop.x() - this.overPositionStart.x());
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
			final float pourcent = (relPos.x() - this.overPositionStart.x())
					/ (this.overPositionStop.x() - this.overPositionStart.x());
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
			final float pourcent = (relPos.x() - this.overPositionStart.x())
					/ (this.overPositionStop.x() - this.overPositionStart.x());
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
			final float pourcent = (relPos.x() - this.overPositionStart.x())
					/ (this.overPositionStop.x() - this.overPositionStart.x());
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
		//LOGGER.trace("Regenerate Display ==> is needed: '" + this.propertyValue + "'");
		this.compositing.clear();
		//this.gc.clear();
		/*
		if (this.colorIdTextFg >= 0) {
			//this.text.setDefaultColorFg(this.shape.getColor(this.colorIdTextFg));
			//this.text.setDefaultColorBg(this.shape.getColor(this.colorIdTextBg));
			//this.text.setCursorColor(this.shape.getColor(this.colorIdCursor));
			//this.text.setSelectionColor(this.shape.getColor(this.colorIdSelection));
		}
		*/
		final Padding padding = Padding.ZERO;//this.shape.getPadding();
		{
			// Manage external shape:
			Vector2f tmpSizeShaper = this.minSize;
			Vector2f delta = this.propertyGravity.gravityGenerateDelta(this.size.less(this.minSize));
			if (this.propertyFill.x()) {
				tmpSizeShaper = tmpSizeShaper.withX(this.size.x());
				delta = delta.withX(0.0f);
			}
			if (this.propertyFill.y()) {
				tmpSizeShaper = tmpSizeShaper.withY(this.size.y());
				delta = delta.withY(0.0f);
			}

			Vector2f tmpOriginShaper = delta;
			Vector2f tmpSizeInside = tmpSizeShaper.less(padding.x(), padding.y());
			//Vector2f tmpOriginText = this.size.less(tmpSizeText).multiply(0.5f);
			Vector2f tmpOriginInside = Vector2f.ZERO;
			// sometimes, the user define an height bigger than the real size needed  == > in this case we need to center the text in the shaper ...
			// fix all the position in the int class:
			tmpSizeShaper = Vector2f.clipInt(tmpSizeShaper);
			tmpOriginShaper = Vector2f.clipInt(tmpOriginShaper);
			tmpSizeInside = Vector2f.clipInt(tmpSizeInside);
			tmpOriginInside = Vector2f.clipInt(tmpOriginInside);

			this.overPositionStart = tmpOriginShaper;
			this.overPositionSize = tmpSizeShaper;
			this.overPositionStop = tmpOriginShaper.add(tmpSizeShaper);
			//this.shape.setShape(0, tmpOriginShaper, tmpSizeShaper, tmpOriginInside, tmpSizeInside);
		}
		{
			// Manage cursor:
			Vector2f tmpSizeShaper = this.minSize;
			Vector2f delta = this.propertyGravity.gravityGenerateDelta(this.size.less(this.minSize));
			if (this.propertyFill.y()) {
				tmpSizeShaper = tmpSizeShaper.withY(this.size.y());
				delta = delta.withY(0.0f);
			}

			Vector2f tmpOriginShaper = delta;
			Vector2f tmpSizeInside = tmpSizeShaper.less(padding.x(), padding.y());
			//Vector2f tmpOriginText = this.size.less(tmpSizeText).multiply(0.5f);
			Vector2f tmpOriginInside = Vector2f.ZERO;

			final float xxx = tmpOriginShaper.x() * 2.0f;

			tmpOriginShaper = tmpOriginShaper.withX(
					xxx * (this.propertyValue - this.propertyMinimum) / (this.propertyMaximum - this.propertyMinimum));
			// sometimes, the user define an height bigger than the real size needed  == > in this case we need to center the text in the shaper ...
			// fix all the position in the int class:
			tmpSizeShaper = Vector2f.clipInt(tmpSizeShaper);
			tmpOriginShaper = Vector2f.clipInt(tmpOriginShaper);
			tmpSizeInside = Vector2f.clipInt(tmpSizeInside);
			tmpOriginInside = Vector2f.clipInt(tmpOriginInside);

			this.overCursorPositionStart = tmpOriginShaper;
			this.overCursorPositionSize = tmpSizeShaper;
			this.overCursorPositionStop = tmpOriginShaper.add(tmpSizeShaper);
			//this.shape.setShape(1, tmpOriginShaper, tmpSizeShaper, tmpOriginInside, tmpSizeInside);
		}
		this.compositing.setRectangleAsSource((int) this.overPositionSize.x(), (int) this.overPositionSize.y(),
				Color.GREEN);
		// TODO: Refaire le design de cet affichage...
		this.compositing.setPos(this.overPositionStart);
		this.compositing.print(new Vector2f(this.overPositionSize.x(), this.overPositionSize.y()));
		this.compositing.flush();
		
		//this.gc.flush();
		this.compositing.flush();

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