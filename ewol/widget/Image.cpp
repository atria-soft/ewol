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
  propertySource(this, "src", "", "Image source path", &ewol::widget::Image::onChangePropertySource),
  propertyBorder(this, "border", vec2(0,0), "Border of the image", &ewol::widget::Image::onChangePropertyGlobalSize),
  propertyImageSize(this, "size", vec2(0,0), "Basic display size of the image", &ewol::widget::Image::onChangePropertyGlobalSize),
  propertyKeepRatio(this, "ratio", true, "Keep ratio of the image", &ewol::widget::Image::onChangePropertyGlobalSize),
  propertyPosStart(this, "part-start", vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), vec2(1.0f, 1.0f), "Start display position in the image", &ewol::widget::Image::onChangePropertyGlobalSize),
  propertyPosStop(this, "part-stop", vec2(1.0f, 1.0f), vec2(0.0f, 0.0f), vec2(1.0f, 1.0f), "Start display position in the image", &ewol::widget::Image::onChangePropertyGlobalSize),
  propertyDistanceFieldMode(this, "distance-field", false, "Distance field mode", &ewol::widget::Image::onChangePropertyDistanceFieldMode),
  propertySmooth(this, "smooth", true, "Smooth display of the image", &ewol::widget::Image::onChangePropertySmooth),
  propertyUseThemeColor(this, "use-theme-color", false, "use the theme color to display images", &ewol::widget::Image::onChangePropertyUseThemeColor),
  m_colorProperty(null),
  m_colorId(-1) {
	addObjectType("ewol::widget::Image");
	m_imageRenderSize = vec2(0,0);
	m_colorProperty = ewol::resource::ColorFile::create(etk::Uri("THEME_COLOR:///Image.json?lib=ewol"));
	if (m_colorProperty != null) {
		m_colorId = m_colorProperty->request("foreground");
	}
}
ewol::widget::Image::~Image() {
	
}

void ewol::widget::Image::init() {
	ewol::Widget::init();
	if (*propertySource != "") {
		onChangePropertySource();
	}
}

void ewol::widget::Image::set(const etk::Uri& _uri, const gale::Dimension& _border) {
	EWOL_VERBOSE("Set Image : " << _uri << " border=" << _border);
	propertyBorder.set(_border);
	propertySource.set(_uri);
}

void ewol::widget::Image::setCustumSource(const egami::Image& _image) {
	// TODO : Better interfacing of all element internal ==> this is a temporary prototype
	m_compositing.setSource(_image);
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::Image::onDraw() {
	m_compositing.draw();
}

void ewol::widget::Image::onRegenerateDisplay() {
	if (needRedraw() == false) {
		return;
	}
	// remove data of the previous composition :
	m_compositing.clear();
	if (    *propertyUseThemeColor == true
	     && m_colorProperty != null) {
		m_compositing.setColor(m_colorProperty->get(m_colorId));
	}
	// Calculate the new position and size:
	vec2 imageBoder = propertyBorder->getPixel();
	vec2 origin = imageBoder;
	imageBoder *= 2.0f;
	vec2 imageRealSize = m_imageRenderSize - imageBoder;
	vec2 imageRealSizeMax = m_size - imageBoder;
	
	vec2 ratioSizeDisplayRequested = *propertyPosStop - *propertyPosStart;
	//imageRealSizeMax *= ratioSizeDisplayRequested;
	
	vec2 delta = ewol::gravityGenerateDelta(*propertyGravity, m_size-m_imageRenderSize);
	if (propertyFill->x() == true) {
		imageRealSize.setX(imageRealSizeMax.x());
		delta.setX(0.0);
	}
	if (propertyFill->y() == true) {
		imageRealSize.setY(imageRealSizeMax.y());
		delta.setY(0.0);
	}
	origin += delta;
	
	if (*propertyKeepRatio == true) {
		vec2 tmpSize = m_compositing.getRealSize();
		//float ratio = tmpSize.x() / tmpSize.y();
		float ratio = (tmpSize.x()*ratioSizeDisplayRequested.x()) / (tmpSize.y() * ratioSizeDisplayRequested.y());
		//float ratioCurrent = (imageRealSize.x()*ratioSizeDisplayRequested.x()) / (imageRealSize.y() * ratioSizeDisplayRequested.y());
		float ratioCurrent = imageRealSize.x() / imageRealSize.y();
		if (ratio == ratioCurrent) {
			// nothing to do ...
		} else if (ratio < ratioCurrent) {
			float oldX = imageRealSize.x();
			imageRealSize.setX(imageRealSize.y()*ratio);
			origin += vec2((oldX - imageRealSize.x()) * 0.5f, 0);
		} else {
			float oldY = imageRealSize.y();
			imageRealSize.setY(imageRealSize.x()/ratio);
			origin += vec2(0, (oldY - imageRealSize.y()) * 0.5f);
		}
	}
	
	// set the somposition properties :
	if (*propertySmooth == true) {
		m_compositing.setPos(origin);
	} else {
		m_compositing.setPos(ivec2(origin));
	}
	m_compositing.printPart(imageRealSize, *propertyPosStart, *propertyPosStop);
	EWOL_DEBUG("Paint Image at : " << origin << " size=" << imageRealSize);
	EWOL_DEBUG("Paint Image :" << *propertySource << " realsize=" << m_compositing.getRealSize() << " origin=" << origin << " size=" << imageRealSize);
	EWOL_DEBUG("      start=" << *propertyPosStart << " stop=" << *propertyPosStop);
}

void ewol::widget::Image::calculateMinMaxSize() {
	EWOL_DEBUG("calculate min size: border=" << propertyBorder << " size=" << propertyImageSize << " min-size=" << propertyMinSize);
	vec2 imageBoder = propertyBorder->getPixel()*2.0f;
	vec2 imageSize = propertyImageSize->getPixel();
	vec2 size = propertyMinSize->getPixel();
	EWOL_DEBUG("                ==> border=" << imageBoder << " size=" << imageSize << " min-size=" << size);
	if (imageSize != vec2(0,0)) {
		m_minSize = imageBoder+imageSize;
		m_maxSize = m_minSize;
	} else {
		vec2 imageSizeReal = m_compositing.getRealSize();
		EWOL_VERBOSE(" Real Size = " << imageSizeReal);
		vec2 min1 = imageBoder+propertyMinSize->getPixel();
		m_minSize = imageBoder+imageSizeReal;
		EWOL_VERBOSE(" set max : " << m_minSize << " min1=" << min1);
		m_minSize.setMax(min1);
		EWOL_VERBOSE("     result : " << m_minSize);
		m_maxSize = imageBoder+propertyMaxSize->getPixel();
		m_minSize.setMin(m_maxSize);
	}
	m_imageRenderSize = m_minSize;
	m_minSize.setMax(size);
	m_maxSize.setMax(m_minSize);
	EWOL_DEBUG("set widget min=" << m_minSize << " max=" << m_maxSize << " with real Image size=" << m_imageRenderSize << " img size=" << imageSize << "  " << propertyImageSize);
	markToRedraw();
}


bool ewol::widget::Image::onEventInput(const ewol::event::Input& _event) {
	//EWOL_DEBUG("Event on BT ...");
	if (_event.getId() == 1) {
		if(gale::key::status::pressSingle == _event.getStatus()) {
			signalPressed.emit();
			return true;
		}
	}
	return false;
}

bool ewol::widget::Image::loadXML(const exml::Element& _node) {
	if (_node.exist() == false) {
		return false;
	}
	ewol::Widget::loadXML(_node);
	// get internal data : 
	
	etk::String tmpAttributeValue = _node.attributes["ratio"];
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
		//EWOL_CRITICAL(" Parse SIZE : " << tmpAttributeValue);
		propertyImageSize.setDirect(tmpAttributeValue);
		//EWOL_CRITICAL("               == > " << propertyImageSize);
	}
	tmpAttributeValue = _node.attributes["border"];
	if (tmpAttributeValue.size() != 0) {
		propertyBorder.setDirect(tmpAttributeValue);
	}
	tmpAttributeValue = _node.attributes["smooth"];
	if (tmpAttributeValue.size() != 0) {
		propertySmooth.setDirect(etk::string_to_bool(tmpAttributeValue));
	}
	//EWOL_DEBUG("Load label:" << node->ToElement()->getText());
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
	EWOL_VERBOSE("Set sources : " << *propertySource << " size=" << *propertyImageSize);
	m_compositing.setSource(*propertySource, propertyImageSize->getPixel());
}

void ewol::widget::Image::onChangePropertyImageSize() {
	markToRedraw();
	requestUpdateSize();
	EWOL_VERBOSE("Set sources : " << *propertySource << " size=" << *propertyImageSize);
	m_compositing.setSource(*propertySource, propertyImageSize->getPixel());
}

void ewol::widget::Image::onChangePropertyGlobalSize() {
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::Image::onChangePropertySmooth() {
	markToRedraw();
}

void ewol::widget::Image::onChangePropertyDistanceFieldMode() {
	m_compositing.setDistanceFieldMode(*propertyDistanceFieldMode);
	markToRedraw();
}

void ewol::widget::Image::onChangePropertyUseThemeColor() {
	markToRedraw();
}

