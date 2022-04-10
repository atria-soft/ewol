/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.etranslate.ETranslate;
import org.atriasoft.ewol.annotation.EwolDescription;
import org.atriasoft.ewol.annotation.EwolSignal;
import org.atriasoft.ewol.compositing.AlignMode;
import org.atriasoft.ewol.compositing.CompositingText;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.resource.ResourceColorFile;
import org.atriasoft.exml.annotation.XmlAttribute;
import org.atriasoft.exml.annotation.XmlManaged;
import org.atriasoft.exml.annotation.XmlName;
import org.atriasoft.gale.key.KeyStatus;

public class Label extends Widget {
	@EwolSignal(name = "pressed")
	@EwolDescription(value = "Label is pressed")
	public SignalEmpty signalPressed = new SignalEmpty();
	private String propertyValue = ""; //!< decorated text to display.
	private int propertyFontSize = 0; //!< default size of the font.
	private final CompositingText textCompose = new CompositingText(); //!< Compositing text element.
	private String value = "";
	
	protected int colorDefaultBgText = -1; //!< Default Background color of the text
	protected int colorDefaultFgText = -1; //!< Default color of the text
	protected ResourceColorFile colorProperty; //!< theme color property
	protected boolean propertyAutoTranslate = true; //!< if at true the data is translate automaticaly translate.
	
	public Label() {
		this("---");
	}
	
	public Label(final String label) {
		this.colorProperty = ResourceColorFile.create(new Uri("THEME", "/color/Label.json", "ewol"));
		if (this.colorProperty != null) {
			this.colorDefaultFgText = this.colorProperty.request("foreground");
			this.colorDefaultBgText = this.colorProperty.request("background");
		}
		setMouseLimit(1);
		setPropertyCanFocus(false);
		setPropertyValue(label);
	}
	
	@Override
	public void calculateMinMaxSize() {
		Log.verbose("calculateMinMaxSize !!! data = '{}'", this.value);
		final Vector3f tmpMax = this.propertyMaxSize.getPixel();
		final Vector3f tmpMin = this.propertyMinSize.getPixel();
		//EWOL_DEBUG("[" + getId() + "] {" + getObjectType() + "} tmpMax : " + tmpMax);
		if (tmpMax.x() <= 999999) {
			this.textCompose.setTextAlignment(0, tmpMax.x() - 4, AlignMode.LEFT);
			//EWOL_DEBUG("[" + getId() + "] {" + getObjectType() + "}     force Alignment ");
		} else {
			this.textCompose.setTextAlignment(0, 0, AlignMode.LEFT);
		}
		Vector3f minSize = this.textCompose.calculateSizeDecorated(this.value);
		this.textCompose.flush();
		minSize = minSize.add(2, 2, 0);
		//EWOL_DEBUG("[" + getId() + "] {" + getObjectType() + "} minSize : " + minSize);
		
		this.minSize = new Vector3f(FMath.avg(tmpMin.x(), 4 + minSize.x(), tmpMax.x()), //
				FMath.avg(tmpMin.y(), 4 + minSize.y(), tmpMax.y()), //
				10);
		Log.verbose("[{}] Result min size : {}", getId(), this.minSize);
	}
	
	public int getPropertyFontSize() {
		return this.propertyFontSize;
	}
	
	public String getPropertyValue() {
		return this.propertyValue;
	}
	
	public boolean isPropertyAutoTranslate() {
		return this.propertyAutoTranslate;
	}
	
	@Override
	protected void onDraw() {
		this.textCompose.draw();
	}
	
	@Override
	public boolean onEventInput(final EventInput event) {
		//Log.debug("Event on Label ...");
		if (event.inputId() == 1) {
			if (KeyStatus.pressSingle == event.status()) {
				// nothing to do ...
				this.signalPressed.emit();
				return true;
			}
		}
		return false;
	}
	
	@Override
	public void onRegenerateDisplay() {
		if (!needRedraw()) {
			return;
		}
		this.textCompose.clear();
		final int paddingSize = 2;
		
		final Vector3f tmpMax = this.propertyMaxSize.getPixel();
		// to know the size of one line :
		final Vector3f minSize = this.textCompose.calculateSize('A');
		
		//minSize.setX(etk::max(minSize.x(), this.minSize.x()));
		//minSize.setY(etk::max(minSize.y(), this.minSize.y()));
		if (tmpMax.x() <= 999999) {
			this.textCompose.setTextAlignment(0, tmpMax.x() - 2.0f * paddingSize, AlignMode.LEFT);
		}
		final Vector3f curentTextSize = this.textCompose.calculateSizeDecorated(this.value);
		
		Vector3f localSize = this.minSize.clipInteger();
		
		// no change for the text origin :
		Vector3f tmpTextOrigin = new Vector3f((this.size.x() - minSize.x()) * 0.5f, (this.size.y() - minSize.y()) * 0.5f, 0);
		
		if (this.propertyFill.x()) {
			localSize = localSize.withX(this.size.x());
			tmpTextOrigin = tmpTextOrigin.withX(0);
		}
		if (this.propertyFill.y()) {
			localSize = localSize.withY(this.size.y());
			tmpTextOrigin = tmpTextOrigin.withY(this.size.y() - 2 * paddingSize - curentTextSize.y());
		}
		tmpTextOrigin = tmpTextOrigin.add(paddingSize, paddingSize, 0);
		localSize = localSize.less(2 * paddingSize, 2 * paddingSize, 0);
		
		//tmpTextOrigin = tmpTextOrigin.withY(tmpTextOrigin.y() + (this.minSize.y() - 2 * paddingSize) - minSize.y());
		tmpTextOrigin = tmpTextOrigin.withY(tmpTextOrigin.y() + this.minSize.y() / 2 - this.textCompose.getHeight());// - this.minSize.y() - paddingSize);
		tmpTextOrigin = tmpTextOrigin.withX(tmpTextOrigin.x() - curentTextSize.x() * 0.5f);
		
		final Vector3f textPos = new Vector3f(tmpTextOrigin.x(), tmpTextOrigin.y(), 0);
		
		final Vector3f drawClippingPos = new Vector3f(paddingSize, paddingSize, -0.5f);
		final Vector3f drawClippingSize = new Vector3f((this.size.x() - paddingSize), (this.size.y() - paddingSize), 1);
		
		// clean the element
		this.textCompose.reset();
		if (this.propertyFontSize != 0) {
			this.textCompose.setFontSize(this.propertyFontSize);
		}
		if (this.colorProperty != null) {
			this.textCompose.setDefaultColorFg(this.colorProperty.get(this.colorDefaultFgText));
			this.textCompose.setDefaultColorBg(this.colorProperty.get(this.colorDefaultBgText));
		}
		this.textCompose.setPos(tmpTextOrigin);
		Log.verbose("[{}] '{}' display at pos={}, size={}", getId(), this.value, tmpTextOrigin, this.size);
		this.textCompose.setTextAlignment(tmpTextOrigin.x(), tmpTextOrigin.x() + localSize.x(), AlignMode.LEFT);
		//this.textCompose.setClipping(drawClippingPos, drawClippingSize);
		this.textCompose.printDecorated(this.value);
		this.textCompose.flush();
	}
	
	@XmlManaged
	@XmlAttribute
	@XmlName(value = "auto-translate")
	@EwolDescription(value = "Translate the String with the marker {T:xxxxxx}")
	public void setPropertyAutoTranslate(final boolean propertyAutoTranslate) {
		if (this.propertyAutoTranslate == propertyAutoTranslate) {
			return;
		}
		this.propertyAutoTranslate = propertyAutoTranslate;
		if (propertyAutoTranslate) {
			this.value = ETranslate.get(this.propertyValue);
		} else {
			this.value = this.propertyValue;
		}
		markToRedraw();
		requestUpdateSize();
	}
	
	@XmlManaged
	@XmlAttribute
	@XmlName(value = "font-size")
	@EwolDescription(value = "Default font size (0=> system default)")
	public void setPropertyFontSize(final int propertyFontSize) {
		if (this.propertyFontSize == propertyFontSize) {
			return;
		}
		this.propertyFontSize = propertyFontSize;
		markToRedraw();
		requestUpdateSize();
	}
	
	@XmlManaged
	@XmlAttribute
	@XmlName(value = "value")
	@EwolDescription(value = "Displayed value string")
	public void setPropertyValue(final String propertyValue) {
		if (this.propertyValue.equals(propertyValue)) {
			return;
		}
		if (this.propertyAutoTranslate) {
			this.value = ETranslate.get(propertyValue);
		} else {
			this.value = propertyValue;
		}
		markToRedraw();
		requestUpdateSize();
		this.propertyValue = propertyValue;
	}
	
}
