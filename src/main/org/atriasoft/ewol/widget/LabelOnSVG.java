/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlProperty;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.etranslate.ETranslate;
import org.atriasoft.ewol.compositing.AlignMode;
import org.atriasoft.ewol.compositing.CompositingText;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.resource.ResourceColorFile;
import org.atriasoft.gale.key.KeyStatus;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class LabelOnSVG extends Widget {
	private static final Logger LOGGER = LoggerFactory.getLogger(LabelOnSVG.class);
	protected int colorDefaultBgText = -1;
	protected int colorDefaultFgText = -1;
	protected ResourceColorFile colorProperty;
	protected boolean propertyAutoTranslate = true;

	protected int propertyFontSize = 0;
	protected String propertyValue = "";
	public SignalEmpty signalPressed = new SignalEmpty();
	protected CompositingText text = new CompositingText();
	protected String value = "";

	public LabelOnSVG() {
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
	public LabelOnSVG(final String newLabel) {
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
		final Vector2f tmpMax = this.propertyMaxSize.getPixel();
		final Vector2f tmpMin = this.propertyMinSize.getPixel();
		LOGGER.debug("[{}] {{}} tmpMax: {}", getId(), getClass().getCanonicalName(), tmpMax);
		if (tmpMax.x() <= 999999) {
			this.text.setTextAlignment(0, tmpMax.x() - 4, AlignMode.LEFT);
			LOGGER.debug("[{}] {{}}     force Alignement", getId(), getClass().getCanonicalName());
		}
		final Vector2f minSize = this.text.calculateSizeDecorated(this.value);
		LOGGER.debug("[{}] {{}} minSize: {}", getId(), getClass().getCanonicalName(), minSize);

		this.minSize = new Vector2f(FMath.avg(tmpMin.x(), 4 + minSize.x(), tmpMax.x()),
				FMath.avg(tmpMin.y(), 4 + minSize.y(), tmpMax.y()));
		LOGGER.trace("[{}] {{}} Result min size: {} < {} < {}", getId(), getClass().getCanonicalName(), tmpMin,
				this.minSize, tmpMax);
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
		this.text.draw();
	}

	@Override
	public boolean onEventInput(final EventInput event) {
		//LOGGER.debug("Event on Label ...");
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
		final int paddingSize = 2;

		final Vector2f tmpMax = this.propertyMaxSize.getPixel();
		// to know the size of one line :
		final Vector2f minSize = this.text.calculateSize('A');

		//minSize.setX(etk::max(minSize.x(), this.minSize.x()));
		//minSize.setY(etk::max(minSize.y(), this.minSize.y()));
		if (tmpMax.x() <= 999999) {
			this.text.setTextAlignment(0, tmpMax.x() - 2 * paddingSize, AlignMode.LEFT);
		}
		final Vector2f currentTextSize = this.text.calculateSizeDecorated(this.value);

		Vector2i localSize = new Vector2i((int) this.minSize.x(), (int) this.minSize.y());

		// no change for the text origin :
		Vector2f tmpTextOrigin = new Vector2f((this.size.x() - this.minSize.x()) / 2.0f,
				(this.size.y() - this.minSize.y()) / 2.0f);

		if (this.propertyFill.x()) {
			localSize = localSize.withX((int) this.size.x());
			tmpTextOrigin = tmpTextOrigin.withX(0);
		}
		if (this.propertyFill.y()) {
			localSize = localSize.withY((int) this.size.y());
			tmpTextOrigin = tmpTextOrigin.withY(this.size.y() - 2 * paddingSize - currentTextSize.y());
		}
		tmpTextOrigin = tmpTextOrigin.add(paddingSize, paddingSize);
		localSize = localSize.less(2 * paddingSize, 2 * paddingSize);

		tmpTextOrigin = tmpTextOrigin.withY(tmpTextOrigin.y() + (this.minSize.y() - 2 * paddingSize) - minSize.y());

		final Vector2f textPos = new Vector2f(tmpTextOrigin.x(), tmpTextOrigin.y());

		final Vector2f drawClippingPos = new Vector2f(paddingSize, paddingSize);
		final Vector2f drawClippingSize = new Vector2f((this.size.x() - paddingSize), (this.size.y() - paddingSize));

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
		LOGGER.trace("[{}] {{}} display at pos: {}", getId(), this.value, tmpTextOrigin);
		this.text.setTextAlignment(tmpTextOrigin.x(), tmpTextOrigin.x() + localSize.x(), AlignMode.LEFT);
		this.text.setClipping(drawClippingPos, drawClippingSize);
		this.text.printDecorated(this.value);

		this.text.flush();
	}

	@JsonProperty("auto-translate")
	@JacksonXmlProperty(isAttribute = true, localName = "auto-translate")
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

	@JsonProperty("font-size")
	@JacksonXmlProperty(isAttribute = true, localName = "font-size")
	public void setPropertyFontSize(final int propertyFontSize) {
		if (this.propertyFontSize == propertyFontSize) {
			return;
		}
		this.propertyFontSize = propertyFontSize;
		markToRedraw();
		requestUpdateSize();
	}

	@JsonProperty("value")
	@JacksonXmlProperty(isAttribute = true, localName = "value")
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

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	/**
	 * Create a new LabelOnSVG.
	 * @return a new LabelOnSVG
	 */
	public static LabelOnSVG create() {
		return new LabelOnSVG();
	}

	/**
	 * Create a new LabelOnSVG with text.
	 * @param text the text to display
	 * @return a new LabelOnSVG
	 */
	public static LabelOnSVG create(final String text) {
		return new LabelOnSVG(text);
	}

	/**
	 * Fluent method to set text.
	 * @param text the text to display
	 * @return this label for chaining
	 */
	public LabelOnSVG text(final String text) {
		setPropertyValue(text);
		return this;
	}

	/**
	 * Fluent method to set font size.
	 * @param size the font size
	 * @return this label for chaining
	 */
	public LabelOnSVG fontSize(final int size) {
		setPropertyFontSize(size);
		return this;
	}

	/**
	 * Fluent method to connect a pressed callback.
	 * @param callback the callback to invoke when pressed
	 * @return this label for chaining
	 */
	public LabelOnSVG onPressed(final Runnable callback) {
		this.signalPressed.connect(callback);
		return this;
	}
}
