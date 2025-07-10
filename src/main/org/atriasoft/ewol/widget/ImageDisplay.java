/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.aknot.annotation.AknotSignal;
import org.atriasoft.egami.ImageByteRGBA;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.compositing.CompositingImage;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.resource.ResourceColorFile;
import org.atriasoft.gale.key.KeyStatus;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ImageDisplay extends Widget {
	private static final Logger LOGGER = LoggerFactory.getLogger(ImageDisplay.class);
	protected int colorId = -1; //!< Color of the image.
	protected ResourceColorFile colorProperty = null; //!< theme color property
	protected CompositingImage compositing = new CompositingImage(); //!< compositing element of the image.
	protected Vector2f imageRenderSize = Vector2f.ZERO; //!< size of the image when we render it

	protected Dimension2f propertyBorder = Dimension2f.ZERO; //!< border to add at the image.
	protected Dimension2f propertyImageSize = Dimension2f.ZERO; //!< border to add at the image.
	protected boolean propertyKeepRatio = true; //!< keep the image ratio between width and height
	protected Vector2f propertyPosStart = Vector2f.ZERO; //!< position in the image to start the display (when we want not to display all the image)
	protected Vector2f propertyPosStop = Vector2f.ONE; //!< position in the image to start the display (when we want not to display all the image)

	protected boolean propertySmooth = true; //!< display is done in the pixel approximation if false
	protected Uri propertySource = null; //!< file name of the image.
	protected boolean propertyUseThemeColor = false; //!< Use the themo color management ("THEMECOLOR:///Image.json?lib=ewol") default false
	@AknotSignal
	@AknotName("pressed")
	@AknotDescription(value = "Image is pressed")
	public final SignalEmpty signalPressed = new SignalEmpty();

	/**
	 *
	 */
	public ImageDisplay() {}

	@Override
	public void calculateMinMaxSize() {
		LOGGER.debug("calculate min size: border=" + this.propertyBorder + " size=" + this.propertyImageSize
				+ " min-size=" + this.propertyMinSize);
		final Vector2f imageBoder = this.propertyBorder.getPixel().multiply(2.0f);
		final Vector2f imageSize = this.propertyImageSize.getPixel();
		final Vector2f size = this.propertyMinSize.getPixel();
		LOGGER.debug("                ==> border=" + imageBoder + " size=" + imageSize + " min-size=" + size);
		if (!imageSize.isZero()) {
			final Vector2f tmp = imageBoder.add(imageSize);
			this.minSize = new Vector2f(tmp.x(), tmp.y());
			this.maxSize = this.minSize;
		} else {
			final Vector2i imageSizeReal = getPropertyMinSize().getPixeli();//.compositing.getRealSize();
			LOGGER.trace(" Real Size = " + imageSizeReal);
			final Vector2f min1 = this.propertyMinSize.getPixel().add(imageBoder.x(), imageBoder.y());
			this.minSize = new Vector2f(imageBoder.x() + imageSizeReal.x(), imageBoder.y() + imageSizeReal.y());
			LOGGER.trace(" set max : " + this.minSize + " min1=" + min1);
			this.minSize = Vector2f.max(this.minSize, min1);
			LOGGER.trace("     result : " + this.minSize);
			this.maxSize = this.propertyMaxSize.getPixel().add(imageBoder.x(), imageBoder.y());
			this.minSize = Vector2f.min(this.minSize, this.maxSize);
		}
		this.imageRenderSize = new Vector2f(this.minSize.x(), this.minSize.y());
		this.minSize = Vector2f.max(this.minSize, size);
		this.maxSize = Vector2f.max(this.maxSize, this.minSize);
		LOGGER.debug("set widget min=" + this.minSize + " max=" + this.maxSize + " with real Image size="
				+ this.imageRenderSize + " img size=" + imageSize + "  " + this.propertyImageSize);
		markToRedraw();
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "image-border")
	@AknotDescription(value = "Border of the image")
	public Dimension2f getPropertyBorder() {
		return this.propertyBorder;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "image-size")
	@AknotDescription(value = "Basic display size of the image")
	public Dimension2f getPropertyImageSize() {
		return this.propertyImageSize;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "part-start")
	@AknotDescription(value = "Start display position in the image")
	public Vector2f getPropertyPosStart() {
		return this.propertyPosStart;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "part-stop")
	@AknotDescription(value = "Start display position in the image")
	public Vector2f getPropertyPosStop() {
		return this.propertyPosStop;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "src")
	@AknotDescription(value = "Image source path")
	public Uri getPropertySource() {
		return this.propertySource;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "ratio")
	@AknotDescription(value = "Keep ratio of the image")
	public boolean isPropertyKeepRatio() {
		return this.propertyKeepRatio;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "smooth")
	@AknotDescription(value = "Smooth display of the image")
	public boolean isPropertySmooth() {
		return this.propertySmooth;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "use-theme-color")
	@AknotDescription(value = "Use the theme color to display images")
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
		LOGGER.debug("Paint Image at : " + origin + " size=" + imageRealSize);
		LOGGER.debug("Paint Image :" + this.propertySource + " realsize=" + this.compositing.getRealSize() + " origin="
				+ origin + " size=" + imageRealSize);
		LOGGER.debug("      start=" + this.propertyPosStart + " stop=" + this.propertyPosStop);
		this.compositing.flush();
	}

	/**
	 * set All the configuration of the current image
	 * @param uri URI of the new image
	 * @param border New border size to set
	 */
	public void set(final Uri uri, final Dimension2f border) {
		LOGGER.trace("Set Image : " + uri + " border=" + border);
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
		LOGGER.trace("Set sources : " + this.propertySource + " size=" + propertyImageSize);
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
		LOGGER.trace("Set sources : " + propertySource + " size=" + this.propertyImageSize);
		this.compositing.setSource(propertySource, this.propertyImageSize.getPixeli());
	}

	public void setPropertyUseThemeColor(final boolean propertyUseThemeColor) {
		if (this.propertyUseThemeColor == propertyUseThemeColor) {
			return;
		}
		this.propertyUseThemeColor = propertyUseThemeColor;
		markToRedraw();
	}
}