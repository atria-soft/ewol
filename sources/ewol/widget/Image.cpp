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

const char * const ewol::widget::Image::eventPressed = "pressed";

ewol::widget::Image::Image() :
  m_colorProperty(nullptr),
  m_colorId(-1),
  m_fileName(*this, "src", "", "Image source path"),
  m_border(*this, "border", vec2(0,0), "Border of the image"),
  m_imageSize(*this, "size", vec2(0,0), "Basic display size of the image"),
  m_keepRatio(*this, "ratio", true, "Keep ratio of the image"),
  m_posStart(*this, "part-start", vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), vec2(1.0f, 1.0f), "Start display position in the image"),
  m_posStop(*this, "part-stop", vec2(1.0f, 1.0f), vec2(0.0f, 0.0f), vec2(1.0f, 1.0f), "Start display position in the image"),
  m_distanceFieldMode(*this, "distance-field", false, "Distance field mode") {
	addObjectType("ewol::widget::Image");
	addEventId(eventPressed);
	m_colorProperty = ewol::resource::ColorFile::create("THEME:COLOR:Image.json");
	if (m_colorProperty != nullptr) {
		m_colorId = m_colorProperty->request("foreground");
	}
}

void ewol::widget::Image::init(const std::string& _file, const ewol::Dimension& _border) {
	ewol::Widget::init();
	set(_file, _border);
}


ewol::widget::Image::~Image() {
	
}

void ewol::widget::Image::setFile(const std::string& _file) {
	EWOL_VERBOSE("Set Image : " << _file);
	if (m_fileName.get() != _file) {
		// copy data :
		m_fileName.set(_file);
		// force redraw all :
		markToRedraw();
		EWOL_VERBOSE("Set sources : " << m_fileName << " size=" << m_imageSize);
		m_compositing.setSource(m_fileName, m_imageSize->getPixel());
	}
}

void ewol::widget::Image::setBorder(const ewol::Dimension& _border) {
	EWOL_VERBOSE("Set border=" << _border);
	// copy data :
	m_border.set(_border);
	// force redraw all :
	markToRedraw();
	// TODO : change the size with no size requested ...
	requestUpdateSize();
}

void ewol::widget::Image::setKeepRatio(bool _keep) {
	if (m_keepRatio.get() == _keep) {
		return;
	}
	// copy data :
	m_keepRatio.set(_keep);
	// force redraw all :
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::Image::setStartPos(const vec2& _pos) {
	if (m_posStart.get() == _pos) {
		return;
	}
	// copy data :
	m_posStart.set(_pos);
	// force redraw all :
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::Image::setStopPos(const vec2& _pos) {
	// copy data :
	m_posStop.set(_pos);
	// force redraw all :
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::Image::setImageSize(const ewol::Dimension& _size) {
	EWOL_VERBOSE("Set Image size : " << _size);
	if (_size != m_imageSize.get()) {
		m_imageSize.set(_size);
		markToRedraw();
		requestUpdateSize();
		EWOL_VERBOSE("Set sources : " << m_fileName << " size=" << m_imageSize);
		m_compositing.setSource(m_fileName, m_imageSize->getPixel());
	}
}

void ewol::widget::Image::set(const std::string& _file, const ewol::Dimension& _border) {
	EWOL_VERBOSE("Set Image : " << _file << " border=" << _border);
	// copy data :
	if (m_border.get() != _border) {
		m_border.set(_border);
		requestUpdateSize();
		markToRedraw();
	}
	setFile(_file);
}

void ewol::widget::Image::onDraw() {
	m_compositing.draw();
}

void ewol::widget::Image::onRegenerateDisplay() {
	if (true == needRedraw()) {
		// remove data of the previous composition :
		m_compositing.clear();
		if (m_colorProperty != nullptr) {
			m_compositing.setColor(m_colorProperty->get(m_colorId));
		}
		// calculate the new position and size :
		vec2 imageBoder = m_border->getPixel();
		vec2 origin = imageBoder;
		imageBoder *= 2.0f;
		vec2 imageRealSize = m_minSize - imageBoder;
		vec2 imageRealSizeMax = m_size - imageBoder;
		
		vec2 ratioSizeDisplayRequested = m_posStop.get() - m_posStart.get();
		//imageRealSizeMax *= ratioSizeDisplayRequested;
		
		if (m_userFill->x() == true) {
			imageRealSize.setX(imageRealSizeMax.x());
		} else {
			origin.setX(origin.x() + (m_size.x()-m_minSize.x())*0.5f);
		}
		if (m_userFill->y() == true) {
			imageRealSize.setY(imageRealSizeMax.y());
		} else {
			origin.setY(origin.y() + (m_size.y()-m_minSize.y())*0.5f);
		}
		if (m_keepRatio == true) {
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
		m_compositing.setPos(origin);
		m_compositing.printPart(imageRealSize, m_posStart, m_posStop);
		//EWOL_DEBUG("Paint Image at : " << origin << " size=" << imageRealSize << "  origin=" << origin);
		EWOL_VERBOSE("Paint Image :" << m_fileName << " realsize=" << m_compositing.getRealSize() << " size=" << imageRealSize);
	}
}

void ewol::widget::Image::calculateMinMaxSize() {
	vec2 imageBoder = m_border->getPixel()*2.0f;
	vec2 imageSize = m_imageSize->getPixel();
	if (imageSize!=vec2(0,0)) {
		m_minSize = imageBoder+imageSize;
		m_maxSize = m_minSize;
	} else {
		vec2 imageSizeReal = m_compositing.getRealSize();
		vec2 min1 = imageBoder+m_userMinSize->getPixel();
		m_minSize = imageBoder+imageSizeReal;
		//EWOL_DEBUG(" set max : " << m_minSize << " " << min1);
		m_minSize.setMax(min1);
		//EWOL_DEBUG("     result : " << m_minSize);
		m_maxSize = imageBoder+m_userMaxSize->getPixel();
		m_minSize.setMin(m_maxSize);
	}
	//EWOL_DEBUG("set widget min=" << m_minSize << " max=" << m_maxSize << " with real Image size=" << imageSizeReal);
	markToRedraw();
}


bool ewol::widget::Image::onEventInput(const ewol::event::Input& _event) {
	//EWOL_DEBUG("Event on BT ...");
	if (1 == _event.getId()) {
		if(ewol::key::statusSingle == _event.getStatus()) {
			generateEventId(eventPressed);
			return true;
		}
	}
	return false;
}

bool ewol::widget::Image::loadXML(exml::Element* _node) {
	if (nullptr == _node) {
		return false;
	}
	ewol::Widget::loadXML(_node);
	// get internal data : 
	
	std::string tmpAttributeValue = _node->getAttribute("ratio");
	if (tmpAttributeValue.size()!=0) {
		if (etk::compare_no_case(tmpAttributeValue, "true") == true) {
			m_keepRatio = true;
		} else if (tmpAttributeValue == "1") {
			m_keepRatio = true;
		} else {
			m_keepRatio = false;
		}
	}
	tmpAttributeValue = _node->getAttribute("size");
	if (tmpAttributeValue.size()!=0) {
		//EWOL_CRITICAL(" Parse SIZE : " << tmpAttributeValue);
		m_imageSize = tmpAttributeValue;
		//EWOL_CRITICAL("               == > " << m_imageSize);
	}
	tmpAttributeValue = _node->getAttribute("border");
	if (tmpAttributeValue.size()!=0) {
		m_border = tmpAttributeValue;
	}
	//EWOL_DEBUG("Load label:" << node->ToElement()->getText());
	if (_node->size()!=0) {
		setFile(_node->getText());
	} else {
		tmpAttributeValue = _node->getAttribute("src");
		if (tmpAttributeValue.size()!=0) {
			setFile(tmpAttributeValue);
		}
	}
	return true;
}

/*
bool ewol::widget::Image::onSetConfig(const ewol::object::Config& _conf) {
	if (true == ewol::Widget::onSetConfig(_conf)) {
		return true;
	}
	if (_conf.getConfig() == configRatio) {
		setKeepRatio(etk::string_to_bool(_conf.getData()));
		return true;
	}
	if (_conf.getConfig() == configSize) {
		setImageSize(_conf.getData());
		return true;
	}
	if (_conf.getConfig() == configBorder) {
		setBorder(_conf.getData());
		return true;
	}
	if (_conf.getConfig() == configSource) {
		setFile(_conf.getData());
		return true;
	}
	if (_conf.getConfig() == configDistanceField) {
		setDistanceField(etk::string_to_bool(_conf.getData()));
		return true;
	}
	if (_conf.getConfig() == configPartStart) {
		setStartPos(vec2(_conf.getData()));
		return true;
	}
	if (_conf.getConfig() == configPartStop) {
		setStopPos(vec2(_conf.getData()));
		return true;
	}
	return false;
}

bool ewol::widget::Image::onGetConfig(const char* _config, std::string& _result) const {
	if (true == ewol::Widget::onGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configRatio) {
		_result = etk::to_string(getKeepRatio());
		return true;
	}
	if (_config == configSize) {
		_result = getImageSize();
		return true;
	}
	if (_config == configBorder) {
		_result = getBorder();
		return true;
	}
	if (_config == configSource) {
		_result = getFile();
		return true;
	}
	if (_config == configDistanceField) {
		_result = etk::to_string(getDistanceField());
		return true;
	}
	if (_config == configPartStart) {
		_result = (std::string)getStartPos();
		return true;
	}
	if (_config == configPartStop) {
		_result = (std::string)getStopPos();
		return true;
	}
	
	return false;
}
*/
