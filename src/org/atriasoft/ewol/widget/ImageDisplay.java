/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.egami.ImageByteRGBA;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Dimension;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.annotation.EwolDescription;
import org.atriasoft.ewol.annotation.EwolSignal;
import org.atriasoft.ewol.compositing.CompositingImage;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.resource.ResourceColorFile;
import org.atriasoft.exml.annotation.XmlManaged;
import org.atriasoft.exml.annotation.XmlName;
import org.atriasoft.exml.annotation.XmlProperty;
import org.atriasoft.exml.model.XmlElement;
import org.atriasoft.gale.key.KeyStatus;

/**
 * @ingroup ewolWidgetGroup
 */
public class ImageDisplay extends Widget {
	protected int colorId = -1; //!< Color of the image.
	protected ResourceColorFile colorProperty = null; //!< theme color property
	protected CompositingImage compositing = new CompositingImage(); //!< compositing element of the image.
	protected Vector2f imageRenderSize = Vector2f.ZERO; //!< size of the image when we render it
	@XmlManaged
	@XmlProperty
	@XmlName(value = "border")
	@EwolDescription(value = "Border of the image")
	protected Dimension propertyBorder = Dimension.ZERO; //!< border to add at the image.
	@XmlManaged
	@XmlProperty
	@XmlName(value = "size")
	@EwolDescription(value = "Basic display size of the image")
	protected Dimension propertyImageSize = Dimension.ZERO; //!< border to add at the image.
	@XmlManaged
	@XmlProperty
	@XmlName(value = "ratio")
	@EwolDescription(value = "Keep ratio of the image")
	protected boolean propertyKeepRatio = true; //!< keep the image ratio between width and hight
	@XmlManaged
	@XmlProperty
	@XmlName(value = "part-start")
	@EwolDescription(value = "Start display position in the image")
	protected Vector2f propertyPosStart = Vector2f.ZERO; //!< position in the image to start the sisplay (when we want not to display all the image)
	@XmlManaged
	@XmlProperty
	@XmlName(value = "part-stop")
	@EwolDescription(value = "Start display position in the image")
	protected Vector2f propertyPosStop = Vector2f.ZERO; //!< position in the image to start the sisplay (when we want not to display all the image)
	
	@XmlManaged
	@XmlProperty
	@XmlName(value = "smooth")
	@EwolDescription(value = "Smooth display of the image")
	protected boolean propertySmooth = true; //!< display is done in the pixed approximation if false
	@XmlManaged
	@XmlProperty
	@XmlName(value = "src")
	@EwolDescription(value = "Image source path")
	protected Uri propertySource = null; //!< file name of the image.
	@XmlManaged
	@XmlProperty
	@XmlName(value = "use-theme-color")
	@EwolDescription(value = "Use the theme color to display images")
	protected boolean propertyUseThemeColor = false; //!< Use the themo color management ("THEMECOLOR:///Image.json?lib=ewol") default false
	@EwolSignal(name = "pressed")
	@EwolDescription(value = "Image is pressed")
	public final SignalEmpty signalPressed = new SignalEmpty();
	
	/**
	 *
	 */
	public ImageDisplay() {}
	
	@Override
	public void calculateMinMaxSize() {
		Log.debug("calculate min size: border=" + this.propertyBorder + " size=" + this.propertyImageSize + " min-size=" + this.propertyMinSize);
		Vector2f imageBoder = this.propertyBorder.getPixel().multiply(2.0f);
		Vector2f imageSize = this.propertyImageSize.getPixel();
		Vector2f size = this.propertyMinSize.getPixel();
		Log.debug("                ==> border=" + imageBoder + " size=" + imageSize + " min-size=" + size);
		if (!imageSize.isZero()) {
			this.minSize = imageBoder.add(imageSize);
			this.maxSize = this.minSize;
		} else {
			Vector2i imageSizeReal = this.compositing.getRealSize();
			Log.verbose(" Real Size = " + imageSizeReal);
			Vector2f min1 = imageBoder.add(this.propertyMinSize.getPixel());
			this.minSize = imageBoder.add(imageSizeReal);
			Log.verbose(" set max : " + this.minSize + " min1=" + min1);
			this.minSize = Vector2f.max(this.minSize, min1);
			Log.verbose("     result : " + this.minSize);
			this.maxSize = imageBoder.add(this.propertyMaxSize.getPixel());
			this.minSize = Vector2f.min(this.minSize, this.maxSize);
		}
		this.imageRenderSize = this.minSize;
		this.minSize = Vector2f.max(this.minSize, size);
		this.maxSize = Vector2f.max(this.maxSize, this.minSize);
		Log.debug("set widget min=" + this.minSize + " max=" + this.maxSize + " with real Image size=" + this.imageRenderSize + " img size=" + imageSize + "  " + this.propertyImageSize);
		markToRedraw();
	}
	
	public Dimension getPropertyBorder() {
		return this.propertyBorder;
	}
	
	public Dimension getPropertyImageSize() {
		return this.propertyImageSize;
	}
	
	public Vector2f getPropertyPosStart() {
		return this.propertyPosStart;
	}
	
	public Vector2f getPropertyPosStop() {
		return this.propertyPosStop;
	}
	
	public Uri getPropertySource() {
		return this.propertySource;
	}
	
	public boolean isPropertyKeepRatio() {
		return this.propertyKeepRatio;
	}
	
	public boolean isPropertySmooth() {
		return this.propertySmooth;
	}
	
	public boolean isPropertyUseThemeColor() {
		return this.propertyUseThemeColor;
	}
	
	@Override
	public boolean loadXML(final XmlElement node) {
		if (node == null) {
			return false;
		}
		super.loadXML(node);
		// get internal data :
		
		String tmpAttributeValue = node.getAttribute("ratio", "").toLowerCase();
		if (tmpAttributeValue.length() != 0) {
			if (tmpAttributeValue.equals("true")) {
				this.propertyKeepRatio = true;
			} else if (tmpAttributeValue.equals("1")) {
				this.propertyKeepRatio = true;
			} else {
				this.propertyKeepRatio = false;
			}
		}
		tmpAttributeValue = node.getAttribute("size", "");
		if (tmpAttributeValue.length() != 0) {
			//Log.critical(" Parse SIZE : " + tmpAttributeValue);
			this.propertyImageSize = Dimension.valueOf(tmpAttributeValue);
			//Log.critical("               == > " + propertyImageSize);
		}
		tmpAttributeValue = node.getAttribute("border", "");
		if (tmpAttributeValue.length() != 0) {
			this.propertyBorder = Dimension.valueOf(tmpAttributeValue);
		}
		tmpAttributeValue = node.getAttribute("smooth", "");
		if (tmpAttributeValue.length() != 0) {
			this.propertySmooth = Boolean.parseBoolean(tmpAttributeValue);
		}
		//Log.debug("Load label:" + node.ToElement().getText());
		if (node.getNodes().size() != 0) {
			this.propertySource = Uri.valueOf(node.getText());
		} else {
			tmpAttributeValue = node.getAttribute("src", "");
			if (tmpAttributeValue.length() != 0) {
				this.propertySource = Uri.valueOf(tmpAttributeValue);
			}
		}
		return true;
	}
	
	@Override
	protected void onDraw() {
		this.compositing.draw();
	}
	
	@Override
	public boolean onEventInput(final EventInput event) {
		//Log.debug("Event on BT ...");
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
		Vector2f origin = imageBoder;
		imageBoder = imageBoder.multiply(2.0f);
		Vector2f imageRealSize = this.imageRenderSize.less(imageBoder);
		Vector2f imageRealSizeMax = this.size.less(imageBoder);
		
		Vector2f ratioSizeDisplayRequested = this.propertyPosStop.less(this.propertyPosStart);
		//imageRealSizeMax *= ratioSizeDisplayRequested;
		
		Vector2f delta = Gravity.gravityGenerateDelta(this.propertyGravity, this.size.less(this.imageRenderSize));
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
			Vector2i tmpSize = this.compositing.getRealSize();
			//float ratio = tmpSize.x() / tmpSize.y();
			float ratio = (tmpSize.x() * ratioSizeDisplayRequested.x()) / (tmpSize.y() * ratioSizeDisplayRequested.y());
			//float ratioCurrent = (imageRealSize.x()*ratioSizeDisplayRequested.x()) / (imageRealSize.y() * ratioSizeDisplayRequested.y());
			float ratioCurrent = imageRealSize.x() / imageRealSize.y();
			if (ratio == ratioCurrent) {
				// nothing to do ...
			} else if (ratio < ratioCurrent) {
				float oldX = imageRealSize.x();
				imageRealSize = imageRealSize.withX(imageRealSize.y() * ratio);
				origin = origin.add((oldX - imageRealSize.x()) * 0.5f, 0);
			} else {
				float oldY = imageRealSize.y();
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
		Log.debug("Paint Image at : " + origin + " size=" + imageRealSize);
		Log.debug("Paint Image :" + this.propertySource + " realsize=" + this.compositing.getRealSize() + " origin=" + origin + " size=" + imageRealSize);
		Log.debug("      start=" + this.propertyPosStart + " stop=" + this.propertyPosStop);
	}
	
	/**
	 * set All the configuration of the current image
	 * @param uri URI of the new image
	 * @param border New border size to set
	 */
	public void set(final Uri uri, final Dimension border) {
		Log.verbose("Set Image : " + uri + " border=" + border);
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
	
	public void setPropertyBorder(final Dimension propertyBorder) {
		if (this.propertyBorder.equals(propertyBorder)) {
			return;
		}
		this.propertyBorder = propertyBorder;
		markToRedraw();
		requestUpdateSize();
	}
	
	public void setPropertyImageSize(final Dimension propertyImageSize) {
		if (this.propertyImageSize.equals(propertyImageSize)) {
			return;
		}
		this.propertyImageSize = propertyImageSize;
		markToRedraw();
		requestUpdateSize();
		Log.verbose("Set sources : " + this.propertySource + " size=" + propertyImageSize);
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
		if (this.propertySource.equals(propertySource)) {
			return;
		}
		this.propertySource = propertySource;
		markToRedraw();
		requestUpdateSize();
		Log.verbose("Set sources : " + propertySource + " size=" + this.propertyImageSize);
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