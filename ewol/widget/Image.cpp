/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/widget/Image.h>
#include <ewol/compositing/Image.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/Manager.h>
#include <ewol/ewol.h>

#undef __class__
#define __class__ "Image"

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
  m_colorProperty(nullptr),
  m_colorId(-1) {
	addObjectType("ewol::widget::Image");
	m_imageRenderSize = vec2(0,0);
	m_colorProperty = ewol::resource::ColorFile::create("{ewol}THEME:COLOR:Image.json");
	if (m_colorProperty != nullptr) {
		m_colorId = m_colorProperty->request("foreground");
	}
}

ewol::widget::Image::~Image() {
	
}

void ewol::widget::Image::set(const std::string& _file, const gale::Dimension& _border) {
	EWOL_VERBOSE("Set Image : " << _file << " border=" << _border);
	propertyBorder.set(_border);
	propertySource.set(_file);
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
	if (m_colorProperty != nullptr) {
		m_compositing.setColor(m_colorProperty->get(m_colorId));
	}
	// calculate the new position and size :
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
	//EWOL_DEBUG("Paint Image at : " << origin << " size=" << imageRealSize << "  origin=" << origin);
	EWOL_VERBOSE("Paint Image :" << *propertySource << " realsize=" << *m_compositing.getRealSize() << " size=" << imageRealSize);
}

void ewol::widget::Image::calculateMinMaxSize() {
	vec2 imageBoder = propertyBorder->getPixel()*2.0f;
	vec2 imageSize = propertyImageSize->getPixel();
	vec2 size = propertyMinSize->getPixel();
	if (imageSize != vec2(0,0)) {
		m_minSize = imageBoder+imageSize;
		m_maxSize = m_minSize;
	} else {
		vec2 imageSizeReal = m_compositing.getRealSize();
		vec2 min1 = imageBoder+propertyMinSize->getPixel();
		m_minSize = imageBoder+imageSizeReal;
		//EWOL_DEBUG(" set max : " << m_minSize << " " << min1);
		m_minSize.setMax(min1);
		//EWOL_DEBUG("     result : " << m_minSize);
		m_maxSize = imageBoder+propertyMaxSize->getPixel();
		m_minSize.setMin(m_maxSize);
	}
	m_imageRenderSize = m_minSize;
	m_minSize.setMax(size);
	m_maxSize.setMax(m_minSize);
	//EWOL_ERROR("set widget min=" << m_minSize << " max=" << m_maxSize << " with real Image size=" << m_imageRenderSize << " img size=" << imageSize << "  " << propertyImageSize);
	markToRedraw();
}


bool ewol::widget::Image::onEventInput(const ewol::event::Input& _event) {
	//EWOL_DEBUG("Event on BT ...");
	if (_event.getId() == 1) {
		if(gale::key::status_single == _event.getStatus()) {
			signalPressed.emit();
			return true;
		}
	}
	return false;
}

bool ewol::widget::Image::loadXML(const std::shared_ptr<const exml::Element>& _node) {
	if (_node == nullptr) {
		return false;
	}
	ewol::Widget::loadXML(_node);
	// get internal data : 
	
	std::string tmpAttributeValue = _node->getAttribute("ratio");
	if (tmpAttributeValue.size() != 0) {
		if (etk::compare_no_case(tmpAttributeValue, "true") == true) {
			propertyKeepRatio.setDirect(true);
		} else if (tmpAttributeValue == "1") {
			propertyKeepRatio.setDirect(true);
		} else {
			propertyKeepRatio.setDirect(false);
		}
	}
	tmpAttributeValue = _node->getAttribute("size");
	if (tmpAttributeValue.size() != 0) {
		//EWOL_CRITICAL(" Parse SIZE : " << tmpAttributeValue);
		propertyImageSize.setDirect(tmpAttributeValue);
		//EWOL_CRITICAL("               == > " << propertyImageSize);
	}
	tmpAttributeValue = _node->getAttribute("border");
	if (tmpAttributeValue.size() != 0) {
		propertyBorder.setDirect(tmpAttributeValue);
	}
	tmpAttributeValue = _node->getAttribute("smooth");
	if (tmpAttributeValue.size() != 0) {
		propertySmooth.setDirect(etk::string_to_bool(tmpAttributeValue));
	}
	//EWOL_DEBUG("Load label:" << node->ToElement()->getText());
	if (_node->size() != 0) {
		propertySource.set(_node->getText());
	} else {
		tmpAttributeValue = _node->getAttribute("src");
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

