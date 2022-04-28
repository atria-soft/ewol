package org.atriasoft.ewol.widget;


import org.atriasoft.etk.Color;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Matrix4f;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.HighSpeedMode;
import org.atriasoft.ewol.Padding;
import org.atriasoft.ewol.annotation.EwolDescription;
import org.atriasoft.ewol.compositing.GuiShape;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.exml.annotation.XmlAttribute;
import org.atriasoft.exml.annotation.XmlManaged;
import org.atriasoft.exml.annotation.XmlName;
import org.atriasoft.gale.backend3d.OpenGL;
import org.atriasoft.gale.key.KeyStatus;
import org.atriasoft.gale.key.KeyType;

/**
 * @ingroup ewolWidgetGroup
 */
class ColorChooser entends Sizer {
	@EwolSignal(name = "change", description = "Update new color")
	public Signal<Color> signalChange;

	protected Color propertyValue = Color.WHITE;
	@XmlManaged
	@XmlAttribute
	@XmlName(value = "value")
	@EwolDescription(value = "color to select")
	public Color getPropertyValue() {
		return propertyValue;
	}
	public void setPropertyValue(Color propertyValue) {
		if (this.propertyValue.equals(propertyValue)) {
			return;
		}
		this.propertyValue = propertyValue;
		onChangePropertyValue();
	}
	
	
	protected ColorChooser() {
		{
			ewol::widget::Sizer::init();
			propertyMode.set(ewol::widget::Sizer::modeVert);
			propertyLockExpand.set(Vector2b(true,true));
				this.widgetColorBar = ewol::widget::ColorBar::create();
					this.widgetColorBar.signalChange.connect(sharedFromThis(), ewol::widget::ColorChooser::onCallbackColorChange);
					this.widgetColorBar.propertyFill.set(Vector2b(true,true));
					subWidgetAdd(this.widgetColorBar);
				
				etk::Color<> sliderColor;
				sliderColor = etk::color::black;
				
				this.widgetRed = ewol::widget::Slider::create();
					this.widgetRed.signalChange.connect(sharedFromThis(), ewol::widget::ColorChooser::onCallbackColorChangeRed);
					this.widgetRed.propertyExpand.set(Vector2b(true,false));
					this.widgetRed.propertyFill.set(Vector2b(true,false));
					this.widgetRed.propertyMinimum.set(0);
					this.widgetRed.propertyMaximum.set(255);
					sliderColor = etk::Color<>(0xFF, 0x00, 0x00, 0xFF);
					this.widgetRed.setColor(sliderColor);
					subWidgetAdd(this.widgetRed);
				this.widgetGreen = ewol::widget::Slider::create();
					this.widgetGreen.signalChange.connect(sharedFromThis(), ewol::widget::ColorChooser::onCallbackColorChangeGreen);
					this.widgetGreen.propertyExpand.set(Vector2b(true,false));
					this.widgetGreen.propertyFill.set(Vector2b(true,false));
					this.widgetGreen.propertyMinimum.set(0);
					this.widgetGreen.propertyMaximum.set(255);
					sliderColor = etk::Color<>(0x00, 0xFF, 0x00, 0xFF);
					this.widgetGreen.setColor(sliderColor);
					subWidgetAdd(this.widgetGreen);
				this.widgetBlue = ewol::widget::Slider::create();
					this.widgetBlue.signalChange.connect(sharedFromThis(), ewol::widget::ColorChooser::onCallbackColorChangeBlue);
					this.widgetBlue.propertyExpand.set(Vector2b(true,false));
					this.widgetBlue.propertyFill.set(Vector2b(true,false));
					this.widgetBlue.propertyMinimum.set(0);
					this.widgetBlue.propertyMaximum.set(255);
					sliderColor = etk::Color<>(0x00, 0x00, 0xFF, 0xFF);
					this.widgetBlue.setColor(sliderColor);
					subWidgetAdd(this.widgetBlue);
				this.widgetAlpha = ewol::widget::Slider::create();
					this.widgetAlpha.signalChange.connect(sharedFromThis(), ewol::widget::ColorChooser::onCallbackColorChangeAlpha);
					this.widgetAlpha.propertyExpand.set(Vector2b(true,false));
					this.widgetAlpha.propertyFill.set(Vector2b(true,false));
					this.widgetAlpha.propertyMinimum.set(0);
					this.widgetAlpha.propertyMaximum.set(255);
					subWidgetAdd(this.widgetAlpha);
	}
	protected ColorBar widgetColorBar;
	protected Slider widgetRed;
	protected Slider widgetGreen;
	protected Slider widgetBlue;
	protected Slider widgetAlpha;
	protected static void onCallbackColorChangeRed(ColorChooser self, float _newColor){
		propertyValue.getDirect().setR(_newColor);
		if (this.widgetColorBar != null) {
			this.widgetColorBar.propertyValue.set(propertyValue);
		}
		signalChange.emit(propertyValue);
	}
	protected static void onCallbackColorChangeGreen(ColorChooser self, float _newColor){
		propertyValue.getDirect().setG(_newColor);
		if (this.widgetColorBar != null) {
			this.widgetColorBar.propertyValue.set(propertyValue);
		}
		signalChange.emit(propertyValue);
	}
	protected static void onCallbackColorChangeBlue(ColorChooser self, float _newColor){
		propertyValue.getDirect().setB(_newColor);
		if (this.widgetColorBar != null) {
			this.widgetColorBar.propertyValue.set(propertyValue);
		}
		signalChange.emit(propertyValue);
	}
	protected static void onCallbackColorChangeAlpha(ColorChooser self, float _newColor){
		propertyValue.getDirect().setA(_newColor);
		if (this.widgetColorBar != null) {
			this.widgetColorBar.propertyValue.set(propertyValue);
		}
		signalChange.emit(propertyValue);
	}
	protected static void onCallbackColorChange(ColorChooser self, etk::Color<> _newColor){
		// == > colorBar has change ...
		int tmpAlpha = propertyValue.a();
		propertyValue.getDirect() = _newColor;
		propertyValue.getDirect().setA(tmpAlpha);
		if (this.widgetRed != null) {
			this.widgetRed.propertyValue.set(propertyValue.r());
		}
		if (this.widgetGreen != null) {
			this.widgetGreen.propertyValue.set(propertyValue.g());
		}
		if (this.widgetBlue != null) {
			this.widgetBlue.propertyValue.set(propertyValue.b());
		}
		if (this.widgetAlpha != null) {
			this.widgetAlpha.propertyValue.set(propertyValue.a());
		}
		signalChange.emit(propertyValue);
	}
	protected void onChangePropertyValue(){
		if (this.widgetRed != null) {
			this.widgetRed.propertyValue.set(propertyValue.r());
		}
		if (this.widgetGreen != null) {
			this.widgetGreen.propertyValue.set(propertyValue.g());
		}
		if (this.widgetBlue != null) {
			this.widgetBlue.propertyValue.set(propertyValue.b());
		}
		if (this.widgetAlpha != null) {
			this.widgetAlpha.propertyValue.set(propertyValue.a());
		}
		if (this.widgetColorBar != null) {
			this.widgetColorBar.propertyValue.set(propertyValue);
		}
}