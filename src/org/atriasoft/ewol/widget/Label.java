/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.etranslate.ETranslate;
import org.atriasoft.ewol.annotation.EwolDescription;
import org.atriasoft.ewol.annotation.EwolSignal;
import org.atriasoft.ewol.compositing.AlignMode;
import org.atriasoft.ewol.compositing.CompositingText;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.resource.ResourceColorFile;
import org.atriasoft.exml.annotation.XmlManaged;
import org.atriasoft.exml.annotation.XmlName;
import org.atriasoft.exml.annotation.XmlProperty;
import org.atriasoft.exml.model.XmlElement;
import org.atriasoft.gale.key.KeyStatus;

public class Label extends Widget {
	protected int colorDefaultBgText = -1; //!< Default Background color of the text
	protected int colorDefaultFgText = -1; //!< Default color of the text
	protected ResourceColorFile colorProperty; //!< theme color property
	@XmlManaged
	@XmlProperty
	@XmlName(value = "auto-translate")
	@EwolDescription(value = "Translate the String with the marker {T:xxxxxx}")
	protected boolean propertyAutoTranslate = true; //!< if at true the data is translate automaticaly translate.
	@XmlManaged
	@XmlProperty
	@XmlName(value = "font-size")
	@EwolDescription(value = "Default font size (0=> system default)")
	
	protected int propertyFontSize = 0; //!< default size of the font.
	@XmlManaged
	@XmlProperty
	@XmlName(value = "value")
	@EwolDescription(value = "Displayed value string")
	protected String propertyValue = ""; //!< decorated text to display.
	@EwolSignal(name = "pressed")
	@EwolDescription(value = "Label is pressed")
	public SignalEmpty signalPressed = new SignalEmpty();
	protected CompositingText text = new CompositingText(); //!< Compositing text element.
	protected String value = "";
	
	public Label() {
		this.colorProperty = ResourceColorFile.create(new Uri("THEME", "/color/Label.json", "ewol"));
		if (this.colorProperty != null) {
			this.colorDefaultFgText = this.colorProperty.request("foreground");
			this.colorDefaultBgText = this.colorProperty.request("background");
		}
		setMouseLimit(1);
		setPropertyCanFocus(false);
	}
	
	/**
	 * Constructor
	 * @param newLabel The displayed decorated text.
	 */
	public Label(final String newLabel) {
		this.colorProperty = ResourceColorFile.create(new Uri("THEME", "/color/Label.json", "ewol"));
		if (this.colorProperty != null) {
			this.colorDefaultFgText = this.colorProperty.request("foreground");
			this.colorDefaultBgText = this.colorProperty.request("background");
		}
		setMouseLimit(1);
		setPropertyCanFocus(false);
		setPropertyValue(newLabel);
	}
	
	@Override
	public void calculateMinMaxSize() {
		Vector2f tmpMax = this.propertyMaxSize.getPixel();
		Vector2f tmpMin = this.propertyMinSize.getPixel();
		Log.debug("[" + getId() + "] {" + getClass().getCanonicalName() + "} tmpMax : " + tmpMax);
		if (tmpMax.x() <= 999999) {
			this.text.setTextAlignment(0, tmpMax.x() - 4, AlignMode.LEFT);
			Log.debug("[" + getId() + "] {" + getClass().getCanonicalName() + "}     force Alignement ");
		}
		Vector3f minSize = this.text.calculateSizeDecorated(this.value);
		Log.debug("[" + getId() + "] {" + getClass().getCanonicalName() + "} minSize : " + minSize);
		
		this.minSize = new Vector2f(FMath.avg(tmpMin.x(), 4 + minSize.x(), tmpMax.x()), FMath.avg(tmpMin.y(), 4 + minSize.y(), tmpMax.y()));
		Log.verbose("[" + getId() + "] {" + getClass().getCanonicalName() + "} Result min size : " + tmpMin + " < " + this.minSize + " < " + tmpMax);
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
	public boolean loadXML(final XmlElement node) {
		if (node == null) {
			return false;
		}
		super.loadXML(node);
		// get internal data :
		Log.debug("Load label:" + node.getText());
		setPropertyValue(node.getText());
		return true;
	}
	
	@Override
	protected void onDraw() {
		this.text.draw();
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
		this.text.clear();
		int paddingSize = 2;
		
		Vector2f tmpMax = this.propertyMaxSize.getPixel();
		// to know the size of one line :
		Vector3f minSize = this.text.calculateSize('A');
		
		//minSize.setX(etk::max(minSize.x(), this.minSize.x()));
		//minSize.setY(etk::max(minSize.y(), this.minSize.y()));
		if (tmpMax.x() <= 999999) {
			this.text.setTextAlignment(0, tmpMax.x() - 2 * paddingSize, AlignMode.LEFT);
		}
		Vector3f currentTextSize = this.text.calculateSizeDecorated(this.value);
		
		Vector2i localSize = new Vector2i((int) this.minSize.x(), (int) this.minSize.y());
		
		// no change for the text orogin :
		Vector3f tmpTextOrigin = new Vector3f((this.size.x() - this.minSize.x()) / 2.0f, (this.size.y() - this.minSize.y()) / 2.0f, 0);
		
		if (this.propertyFill.x()) {
			localSize = localSize.withX((int) this.size.x());
			tmpTextOrigin = tmpTextOrigin.withX(0);
		}
		if (this.propertyFill.y()) {
			localSize = localSize.withY((int) this.size.y());
			tmpTextOrigin = tmpTextOrigin.withY(this.size.y() - 2 * paddingSize - currentTextSize.y());
		}
		tmpTextOrigin = tmpTextOrigin.add(paddingSize, paddingSize, 0);
		localSize = localSize.less(2 * paddingSize, 2 * paddingSize);
		
		tmpTextOrigin = tmpTextOrigin.withY(tmpTextOrigin.y() + (this.minSize.y() - 2 * paddingSize) - minSize.y());
		
		Vector2f textPos = new Vector2f(tmpTextOrigin.x(), tmpTextOrigin.y());
		
		Vector3f drawClippingPos = new Vector3f(paddingSize, paddingSize, -0.5f);
		Vector3f drawClippingSize = new Vector3f((this.size.x() - paddingSize), (this.size.y() - paddingSize), 1);
		
		// clean the element
		this.text.reset();
		if (this.propertyFontSize != 0) {
			this.text.setFontSize(this.propertyFontSize);
		}
		if (this.colorProperty != null) {
			this.text.setDefaultColorFg(this.colorProperty.get(this.colorDefaultFgText));
			this.text.setDefaultColorBg(this.colorProperty.get(this.colorDefaultBgText));
		}
		this.text.setPos(tmpTextOrigin);
		Log.verbose("[" + getId() + "] {" + this.value + "} display at pos : " + tmpTextOrigin);
		this.text.setTextAlignment(tmpTextOrigin.x(), tmpTextOrigin.x() + localSize.x(), AlignMode.LEFT);
		this.text.setClipping(drawClippingPos, drawClippingSize);
		this.text.printDecorated(this.value);
		
		this.text.flush();
	}
	
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
	
	public void setPropertyFontSize(final int propertyFontSize) {
		if (this.propertyFontSize == propertyFontSize) {
			return;
		}
		this.propertyFontSize = propertyFontSize;
		markToRedraw();
		requestUpdateSize();
	}
	
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
