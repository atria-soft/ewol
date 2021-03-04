/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/Image.hpp>
#include <ewol/compositing/Image.hpp>
#include <ewol/compositing/Drawing.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/ewol.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::Image);

ewol::widget::Image::Image() :
  signalPressed(this, "pressed", "Image is pressed"),
  propertySource(this, "src", "", "Image source path", ewol::widget::Image::onChangePropertySource),
  propertyBorder(this, "border", Vector2f(0,0), "Border of the image", ewol::widget::Image::onChangePropertyGlobalSize),
  propertyImageSize(this, "size", Vector2f(0,0), "Basic display size of the image", ewol::widget::Image::onChangePropertyGlobalSize),
  propertyKeepRatio(this, "ratio", true, "Keep ratio of the image", ewol::widget::Image::onChangePropertyGlobalSize),
  propertyPosStart(this, "part-start", Vector2f(0.0f, 0.0f), Vector2f(0.0f, 0.0f), Vector2f(1.0f, 1.0f), "Start display position in the image", ewol::widget::Image::onChangePropertyGlobalSize),
  propertyPosStop(this, "part-stop", Vector2f(1.0f, 1.0f), Vector2f(0.0f, 0.0f), Vector2f(1.0f, 1.0f), "Start display position in the image", ewol::widget::Image::onChangePropertyGlobalSize),
  propertyDistanceFieldMode(this, "distance-field", false, "Distance field mode", ewol::widget::Image::onChangePropertyDistanceFieldMode),
  propertySmooth(this, "smooth", true, "Smooth display of the image", ewol::widget::Image::onChangePropertySmooth),
  propertyUseThemeColor(this, "use-theme-color", false, "use the theme color to display images", ewol::widget::Image::onChangePropertyUseThemeColor),
  this.colorProperty(null),
  this.colorId(-1) {
	addObjectType("ewol::widget::Image");
	this.imageRenderSize = Vector2f(0,0);
	this.colorProperty = ewol::resource::ColorFile::create(etk::Uri("THEME_COLOR:///Image.json?lib=ewol"));
	if (this.colorProperty != null) {
		this.colorId = this.colorProperty.request("foreground");
	}
}
ewol::widget::Image::~Image() {
	
}

void ewol::widget::Image::init() {
	Widget::init();
	if (*propertySource != "") {
		onChangePropertySource();
	}
}

void ewol::widget::Image::set( etk::Uri _uri,  gale::Dimension _border) {
	Log.verbose("Set Image : " + _uri + " border=" + _border);
	propertyBorder.set(_border);
	propertySource.set(_uri);
}

void ewol::widget::Image::setCustumSource( egami::Image _image) {
	// TODO : Better interfacing of all element internal ==> this is a temporary prototype
	this.compositing.setSource(_image);
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::Image::onDraw() {
	this.compositing.draw();
}

void ewol::widget::Image::onRegenerateDisplay() {
	if (needRedraw() == false) {
		return;
	}
	// remove data of the previous composition :
	this.compositing.clear();
	if (    *propertyUseThemeColor == true
	     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.colorProperty != null) {
		this.compositing.setColor(this.colorProperty.get(this.colorId));
	}
	// Calculate the new position and size:
	Vector2f imageBoder = propertyBorder.getPixel();
	Vector2f origin = imageBoder;
	imageBoder *= 2.0f;
	Vector2f imageRealSize = this.imageRenderSize - imageBoder;
	Vector2f imageRealSizeMax = this.size - imageBoder;
	
	Vector2f ratioSizeDisplayRequested = *propertyPosStop - *propertyPosStart;
	//imageRealSizeMax *= ratioSizeDisplayRequested;
	
	Vector2f delta = ewol::gravityGenerateDelta(*propertyGravity, this.size-this.imageRenderSize);
	if (propertyFill.x() == true) {
		imageRealSize.setX(imageRealSizeMax.x());
		delta.setX(0.0);
	}
	if (propertyFill.y() == true) {
		imageRealSize.setY(imageRealSizeMax.y());
		delta.setY(0.0);
	}
	origin += delta;
	
	if (*propertyKeepRatio == true) {
		Vector2f tmpSize = this.compositing.getRealSize();
		//float ratio = tmpSize.x() / tmpSize.y();
		float ratio = (tmpSize.x()*ratioSizeDisplayRequested.x()) / (tmpSize.y() * ratioSizeDisplayRequested.y());
		//float ratioCurrent = (imageRealSize.x()*ratioSizeDisplayRequested.x()) / (imageRealSize.y() * ratioSizeDisplayRequested.y());
		float ratioCurrent = imageRealSize.x() / imageRealSize.y();
		if (ratio == ratioCurrent) {
			// nothing to do ...
		} else if (ratio < ratioCurrent) {
			float oldX = imageRealSize.x();
			imageRealSize.setX(imageRealSize.y()*ratio);
			origin += Vector2f((oldX - imageRealSize.x()) * 0.5f, 0);
		} else {
			float oldY = imageRealSize.y();
			imageRealSize.setY(imageRealSize.x()/ratio);
			origin += Vector2f(0, (oldY - imageRealSize.y()) * 0.5f);
		}
	}
	
	// set the somposition properties :
	if (*propertySmooth == true) {
		this.compositing.setPos(origin);
	} else {
		this.compositing.setPos(Vector2i(origin));
	}
	this.compositing.printPart(imageRealSize, *propertyPosStart, *propertyPosStop);
	Log.debug("Paint Image at : " + origin + " size=" + imageRealSize);
	Log.debug("Paint Image :" + *propertySource + " realsize=" + this.compositing.getRealSize() + " origin=" + origin + " size=" + imageRealSize);
	Log.debug("      start=" + *propertyPosStart + " stop=" + *propertyPosStop);
}

void ewol::widget::Image::calculateMinMaxSize() {
	Log.debug("calculate min size: border=" + propertyBorder + " size=" + propertyImageSize + " min-size=" + propertyMinSize);
	Vector2f imageBoder = propertyBorder.getPixel()*2.0f;
	Vector2f imageSize = propertyImageSize.getPixel();
	Vector2f size = propertyMinSize.getPixel();
	Log.debug("                ==> border=" + imageBoder + " size=" + imageSize + " min-size=" + size);
	if (imageSize != Vector2f(0,0)) {
		this.minSize = imageBoder+imageSize;
		this.maxSize = this.minSize;
	} else {
		Vector2f imageSizeReal = this.compositing.getRealSize();
		Log.verbose(" Real Size = " + imageSizeReal);
		Vector2f min1 = imageBoder+propertyMinSize.getPixel();
		this.minSize = imageBoder+imageSizeReal;
		Log.verbose(" set max : " + this.minSize + " min1=" + min1);
		this.minSize.setMax(min1);
		Log.verbose("     result : " + this.minSize);
		this.maxSize = imageBoder+propertyMaxSize.getPixel();
		this.minSize.setMin(this.maxSize);
	}
	this.imageRenderSize = this.minSize;
	this.minSize.setMax(size);
	this.maxSize.setMax(this.minSize);
	Log.debug("set widget min=" + this.minSize + " max=" + this.maxSize + " with real Image size=" + this.imageRenderSize + " img size=" + imageSize + "  " << propertyImageSize);
	markToRedraw();
}


boolean ewol::widget::Image::onEventInput( ewol::event::Input _event) {
	//Log.debug("Event on BT ...");
	if (_event.getId() == 1) {
		if(KeyStatus::pressSingle == _event.getStatus()) {
			signalPressed.emit();
			return true;
		}
	}
	return false;
}

boolean ewol::widget::Image::loadXML( exml::Element _node) {
	if (_node.exist() == false) {
		return false;
	}
	Widget::loadXML(_node);
	// get internal data : 
	
	String tmpAttributeValue = _node.attributes["ratio"];
	if (tmpAttributeValue.size() != 0) {
		if (etk::compare_no_case(tmpAttributeValue, "true") == true) {
			propertyKeepRatio.setDirect(true);
		} else if (tmpAttributeValue == "1") {
			propertyKeepRatio.setDirect(true);
		} else {
			propertyKeepRatio.setDirect(false);
		}
	}
	tmpAttributeValue = _node.attributes["size"];
	if (tmpAttributeValue.size() != 0) {
		//Log.critical(" Parse SIZE : " + tmpAttributeValue);
		propertyImageSize.setDirect(tmpAttributeValue);
		//Log.critical("               == > " + propertyImageSize);
	}
	tmpAttributeValue = _node.attributes["border"];
	if (tmpAttributeValue.size() != 0) {
		propertyBorder.setDirect(tmpAttributeValue);
	}
	tmpAttributeValue = _node.attributes["smooth"];
	if (tmpAttributeValue.size() != 0) {
		propertySmooth.setDirect(etk::string_to_bool(tmpAttributeValue));
	}
	//Log.debug("Load label:" + node.ToElement().getText());
	if (_node.nodes.size() != 0) {
		propertySource.set(_node.getText());
	} else {
		tmpAttributeValue = _node.attributes["src"];
		if (tmpAttributeValue.size() != 0) {
			propertySource.set(tmpAttributeValue);
		}
	}
	return true;
}

void ewol::widget::Image::onChangePropertySource() {
	markToRedraw();
	requestUpdateSize();
	Log.verbose("Set sources : " + *propertySource + " size=" + *propertyImageSize);
	this.compositing.setSource(*propertySource, propertyImageSize.getPixel());
}

void ewol::widget::Image::onChangePropertyImageSize() {
	markToRedraw();
	requestUpdateSize();
	Log.verbose("Set sources : " + *propertySource + " size=" + *propertyImageSize);
	this.compositing.setSource(*propertySource, propertyImageSize.getPixel());
}

void ewol::widget::Image::onChangePropertyGlobalSize() {
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::Image::onChangePropertySmooth() {
	markToRedraw();
}

void ewol::widget::Image::onChangePropertyDistanceFieldMode() {
	this.compositing.setDistanceFieldMode(*propertyDistanceFieldMode);
	markToRedraw();
}

void ewol::widget::Image::onChangePropertyUseThemeColor() {
	markToRedraw();
}

