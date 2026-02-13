/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlProperty;
import org.atriasoft.egami.ImageByteRGBA;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.compositing.CompositingImage;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.resource.ResourceColorFile;
import org.atriasoft.gale.key.KeyStatus;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ImageDisplay extends Widget {
	private static final Logger LOGGER = LoggerFactory.getLogger(ImageDisplay.class);
	protected int colorId = -1;
	protected ResourceColorFile colorProperty = null;
	protected CompositingImage compositing = new CompositingImage();
	protected Vector2f imageRenderSize = Vector2f.ZERO;

	protected Dimension2f propertyBorder = Dimension2f.ZERO;
	protected Dimension2f propertyImageSize = Dimension2f.ZERO;
	protected boolean propertyKeepRatio = true;
	protected Vector2f propertyPosStart = Vector2f.ZERO;
	protected Vector2f propertyPosStop = Vector2f.ONE;

	protected boolean propertySmooth = true;
	protected Uri propertySource = null;
	protected boolean propertyUseThemeColor = false;
	public final SignalEmpty signalPressed = new SignalEmpty();
	
	/**
	 *
	 */
	public ImageDisplay() {}
	
	@Override
	public void calculateMinMaxSize() {
		LOGGER.debug("calculate min size: border={} size={} min-size={}", this.propertyBorder, this.propertyImageSize,
				this.propertyMinSize);
		final Vector2f imageBoder = this.propertyBorder.getPixel().multiply(2.0f);
		final Vector2f imageSize = this.propertyImageSize.getPixel();
		final Vector2f size = this.propertyMinSize.getPixel();
		LOGGER.debug("                ==> border={} size={} min-size={}", imageBoder, imageSize, size);
		if (!imageSize.isZero()) {
			final Vector2f tmp = imageBoder.add(imageSize);
			this.minSize = new Vector2f(tmp.x(), tmp.y());
			this.maxSize = this.minSize;
		} else {
			final Vector2i imageSizeReal = getPropertyMinSize().getPixeli();//.compositing.getRealSize();
			LOGGER.trace("Real Size = {}", imageSizeReal);
			final Vector2f min1 = this.propertyMinSize.getPixel().add(imageBoder.x(), imageBoder.y());
			this.minSize = new Vector2f(imageBoder.x() + imageSizeReal.x(), imageBoder.y() + imageSizeReal.y());
			LOGGER.trace("set max: {} min1={}", this.minSize, min1);
			this.minSize = Vector2f.max(this.minSize, min1);
			LOGGER.trace("     result: {}", this.minSize);
			this.maxSize = this.propertyMaxSize.getPixel().add(imageBoder.x(), imageBoder.y());
			this.minSize = Vector2f.min(this.minSize, this.maxSize);
		}
		this.imageRenderSize = new Vector2f(this.minSize.x(), this.minSize.y());
		this.minSize = Vector2f.max(this.minSize, size);
		this.maxSize = Vector2f.max(this.maxSize, this.minSize);
		LOGGER.debug("set widget min={} max={} with real Image size={} img size={}  {}", this.minSize, this.maxSize,
				this.imageRenderSize, imageSize, this.propertyImageSize);
		markToRedraw();
	}
	
	@JsonProperty("image-border")
	@JacksonXmlProperty(isAttribute = true, localName = "image-border")
	public Dimension2f getPropertyBorder() {
		return this.propertyBorder;
	}
	
	@JsonProperty("image-size")
	@JacksonXmlProperty(isAttribute = true, localName = "image-size")
	public Dimension2f getPropertyImageSize() {
		return this.propertyImageSize;
	}
	
	@JsonProperty("part-start")
	@JacksonXmlProperty(isAttribute = true, localName = "part-start")
	public Vector2f getPropertyPosStart() {
		return this.propertyPosStart;
	}
	
	@JsonProperty("part-stop")
	@JacksonXmlProperty(isAttribute = true, localName = "part-stop")
	public Vector2f getPropertyPosStop() {
		return this.propertyPosStop;
	}
	
	@JsonProperty("src")
	@JacksonXmlProperty(isAttribute = true, localName = "src")
	public Uri getPropertySource() {
		return this.propertySource;
	}
	
	@JsonProperty("ratio")
	@JacksonXmlProperty(isAttribute = true, localName = "ratio")
	public boolean isPropertyKeepRatio() {
		return this.propertyKeepRatio;
	}
	
	@JsonProperty("smooth")
	@JacksonXmlProperty(isAttribute = true, localName = "smooth")
	public boolean isPropertySmooth() {
		return this.propertySmooth;
	}
	
	@JsonProperty("use-theme-color")
	@JacksonXmlProperty(isAttribute = true, localName = "use-theme-color")
	public boolean isPropertyUseThemeColor() {
		return this.propertyUseThemeColor;
	}
	
	@Override
	protected void onDraw() {
		this.compositing.draw();
	}
	
	@Override
	public boolean onEventInput(final EventInput event) {
		//LOGGER.debug("Event on BT ...");
		if (event.inputId() == 1) {
			if (KeyStatus.pressSingle == event.status()) {
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
		// remove data of the previous composition :
		this.compositing.clear();
		if (this.propertyUseThemeColor && this.colorProperty != null) {
			this.compositing.setColor(this.colorProperty.get(this.colorId));
		}
		// Calculate the new position and size:
		Vector2f imageBoder = this.propertyBorder.getPixel();
		Vector2f origin = new Vector2f(imageBoder.x(), imageBoder.y());
		imageBoder = imageBoder.multiply(2.0f);
		Vector2f imageRealSize = this.imageRenderSize.less(imageBoder);
		final Vector2f imageRealSizeMax = this.size.less(imageBoder.x(), imageBoder.y());
		
		final Vector2f ratioSizeDisplayRequested = this.propertyPosStop.less(this.propertyPosStart);
		//imageRealSizeMax *= ratioSizeDisplayRequested;
		
		Vector2f delta = this.propertyGravity
				.gravityGenerateDelta(this.size.less(this.imageRenderSize.x(), this.imageRenderSize.y()));
		if (this.propertyFill.x()) {
			imageRealSize = imageRealSize.withX(imageRealSizeMax.x());
			delta = delta.withX(0.0f);
		}
		if (this.propertyFill.y()) {
			imageRealSize = imageRealSize.withY(imageRealSizeMax.y());
			delta = delta.withY(0.0f);
		}
		origin = origin.add(delta);
		
		if (this.propertyKeepRatio) {
			final Vector2i tmpSize = this.compositing.getRealSize();
			//float ratio = tmpSize.x() / tmpSize.y();
			final float ratio = (tmpSize.x() * ratioSizeDisplayRequested.x())
					/ (tmpSize.y() * ratioSizeDisplayRequested.y());
			//float ratioCurrent = (imageRealSize.x()*ratioSizeDisplayRequested.x()) / (imageRealSize.y() * ratioSizeDisplayRequested.y());
			final float ratioCurrent = imageRealSize.x() / imageRealSize.y();
			if (ratio == ratioCurrent) {
				// nothing to do ...
			} else if (ratio < ratioCurrent) {
				final float oldX = imageRealSize.x();
				imageRealSize = imageRealSize.withX(imageRealSize.y() * ratio);
				origin = origin.add((oldX - imageRealSize.x()) * 0.5f, 0);
			} else {
				final float oldY = imageRealSize.y();
				imageRealSize = imageRealSize.withY(imageRealSize.x() / ratio);
				origin = origin.add(0, (oldY - imageRealSize.y()) * 0.5f);
			}
		}
		
		// set the somposition properties :
		if (this.propertySmooth) {
			this.compositing.setPos(origin);
		} else {
			this.compositing.setPos(Vector2f.clipInt(origin));
		}
		this.compositing.printPart(imageRealSize, this.propertyPosStart, this.propertyPosStop);
		LOGGER.trace("Paint Image at: {} size={}", origin, imageRealSize);
		LOGGER.trace("Paint Image: {} realsize={} origin={} size={}", this.propertySource,
				this.compositing.getRealSize(), origin, imageRealSize);
		LOGGER.trace("      start={} stop={}", this.propertyPosStart, this.propertyPosStop);
		this.compositing.flush();
	}
	
	/**
	 * set All the configuration of the current image
	 * @param uri URI of the new image
	 * @param border New border size to set
	 */
	public void set(final Uri uri, final Dimension2f border) {
		LOGGER.trace("Set Image: {} border={}", uri, border);
		setPropertyBorder(border);
		setPropertySource(uri);
	}
	
	/**
	 * Set an image with direct elements
	 * @param image Image to set in the display
	 */
	public void setCustumSource(final ImageByteRGBA image) {
		// TODO : Better interfacing of all element internal ==> this is a temporary prototype
		this.compositing.setSource(image);
		markToRedraw();
		requestUpdateSize();
	}
	
	public void setPropertyBorder(final Dimension2f propertyBorder) {
		if (this.propertyBorder.equals(propertyBorder)) {
			return;
		}
		this.propertyBorder = propertyBorder;
		markToRedraw();
		requestUpdateSize();
	}
	
	public void setPropertyImageSize(final Dimension2f propertyImageSize) {
		if (this.propertyImageSize.equals(propertyImageSize)) {
			return;
		}
		this.propertyImageSize = propertyImageSize;
		markToRedraw();
		requestUpdateSize();
		LOGGER.trace("Set sources: {} size={}", this.propertySource, propertyImageSize);
		this.compositing.setSource(this.propertySource, propertyImageSize.getPixeli());
	}
	
	public void setPropertyKeepRatio(final boolean propertyKeepRatio) {
		if (this.propertyKeepRatio == propertyKeepRatio) {
			return;
		}
		this.propertyKeepRatio = propertyKeepRatio;
		markToRedraw();
		requestUpdateSize();
	}
	
	public void setPropertyPosStart(final Vector2f propertyPosStart) {
		if (this.propertyPosStart.equals(propertyPosStart)) {
			return;
		}
		this.propertyPosStart = propertyPosStart;
		markToRedraw();
		requestUpdateSize();
	}
	
	public void setPropertyPosStop(final Vector2f propertyPosStop) {
		if (this.propertyPosStop.equals(propertyPosStop)) {
			return;
		}
		this.propertyPosStop = propertyPosStop;
		markToRedraw();
		requestUpdateSize();
	}
	
	public void setPropertySmooth(final boolean propertySmooth) {
		if (this.propertySmooth == propertySmooth) {
			return;
		}
		this.propertySmooth = propertySmooth;
		markToRedraw();
	}
	
	public void setPropertySource(final Uri propertySource) {
		if (this.propertySource != null && this.propertySource.equals(propertySource)) {
			return;
		}
		this.propertySource = propertySource;
		markToRedraw();
		requestUpdateSize();
		LOGGER.trace("Set sources: {} size={}", propertySource, this.propertyImageSize);
		this.compositing.setSource(propertySource, this.propertyImageSize.getPixeli());
	}
	
	public void setPropertyUseThemeColor(final boolean propertyUseThemeColor) {
		if (this.propertyUseThemeColor == propertyUseThemeColor) {
			return;
		}
		this.propertyUseThemeColor = propertyUseThemeColor;
		markToRedraw();
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	/**
	 * Create a new ImageDisplay.
	 * @return a new ImageDisplay
	 */
	public static ImageDisplay create() {
		return new ImageDisplay();
	}

	/**
	 * Create a new ImageDisplay with source.
	 * @param source the image source URI
	 * @return a new ImageDisplay
	 */
	public static ImageDisplay create(final Uri source) {
		final ImageDisplay image = new ImageDisplay();
		image.setPropertySource(source);
		return image;
	}

	/**
	 * Fluent method to set image source.
	 * @param source the image source URI
	 * @return this image for chaining
	 */
	public ImageDisplay source(final Uri source) {
		setPropertySource(source);
		return this;
	}

	/**
	 * Fluent method to set image size.
	 * @param size the image size
	 * @return this image for chaining
	 */
	public ImageDisplay imageSize(final Dimension2f size) {
		setPropertyImageSize(size);
		return this;
	}

	/**
	 * Fluent method to set border.
	 * @param border the border size
	 * @return this image for chaining
	 */
	public ImageDisplay border(final Dimension2f border) {
		setPropertyBorder(border);
		return this;
	}

	/**
	 * Fluent method to set keep ratio.
	 * @param keepRatio true to keep aspect ratio
	 * @return this image for chaining
	 */
	public ImageDisplay keepRatio(final boolean keepRatio) {
		setPropertyKeepRatio(keepRatio);
		return this;
	}

	/**
	 * Fluent method to set smooth display.
	 * @param smooth true for smooth display
	 * @return this image for chaining
	 */
	public ImageDisplay smooth(final boolean smooth) {
		setPropertySmooth(smooth);
		return this;
	}

	/**
	 * Fluent method to connect a pressed callback.
	 * @param callback the callback to invoke when pressed
	 * @return this image for chaining
	 */
	public ImageDisplay onPressed(final Runnable callback) {
		this.signalPressed.connect(callback);
		return this;
	}
}