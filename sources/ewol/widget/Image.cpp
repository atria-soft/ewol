/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Image.h>
#include <ewol/compositing/Image.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/ewol.h>


#undef __class__
#define __class__ "Image"

static ewol::Widget* Create(void)
{
	return new widget::Image();
}

void widget::Image::Init(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,&Create);
}

void widget::Image::UnInit(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,NULL);
}


const char * const widget::Image::eventPressed = "ewol-widget-image-event-pressed";

const char * const widget::Image::configRatio = "ratio";
const char * const widget::Image::configSize = "size";
const char * const widget::Image::configBorder = "border";
const char * const widget::Image::configSource = "src";

widget::Image::Image(const etk::UString& _file, const ewol::Dimension& _border) :
	m_imageSize(vec2(0,0)),
	m_keepRatio(true)
{
	AddEventId(eventPressed);
	RegisterConfig(configRatio, "bool", NULL, "Keep ratio of the image");
	RegisterConfig(configSize, "Dimension", NULL, "Basic display size of the image");
	RegisterConfig(configBorder, "Dimension", NULL, "Border of the image");
	RegisterConfig(configSource, "string", "Image source path");
	Set(_file, _border);
}


void widget::Image::SetFile(const etk::UString& _file)
{
	EWOL_VERBOSE("Set Image : " << _file);
	if (m_fileName != _file) {
		// copy data :
		m_fileName = _file;
		// Force redraw all :
		MarkToRedraw();
		EWOL_VERBOSE("Set sources : " << m_fileName << " size=" << m_imageSize);
		m_compositing.SetSource(m_fileName, m_imageSize.GetPixel());
	}
}

void widget::Image::SetBorder(const ewol::Dimension& _border)
{
	EWOL_VERBOSE("Set border=" << _border);
	// copy data :
	m_border = _border;
	// Force redraw all :
	MarkToRedraw();
	// TODO : Change the size with no size requested ...
	ewol::RequestUpdateSize();
}

void widget::Image::SetKeepRatio(bool _keep)
{
	if (m_keepRatio != _keep) {
		// copy data :
		m_keepRatio = _keep;
		// Force redraw all :
		MarkToRedraw();
		ewol::RequestUpdateSize();
	}
}

void widget::Image::SetImageSize(const ewol::Dimension& _size)
{
	EWOL_VERBOSE("Set Image size : " << _size);
	if (_size != m_imageSize) {
		m_imageSize = _size;
		MarkToRedraw();
		ewol::RequestUpdateSize();
		EWOL_VERBOSE("Set sources : " << m_fileName << " size=" << m_imageSize);
		m_compositing.SetSource(m_fileName, m_imageSize.GetPixel());
	}
}

void widget::Image::Set(const etk::UString& _file, const ewol::Dimension& _border)
{
	EWOL_VERBOSE("Set Image : " << _file << " border=" << _border);
	// copy data :
	if (m_border != _border) {
		m_border = _border;
		ewol::RequestUpdateSize();
		MarkToRedraw();
	}
	SetFile(_file);
}


void widget::Image::OnDraw(void)
{
	m_compositing.Draw();
}

void widget::Image::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		// remove data of the previous composition :
		m_compositing.Clear();
		
		// calculate the new position and size :
		vec2 imageBoder = m_border.GetPixel();
		vec2 origin = imageBoder;
		imageBoder *= 2.0f;
		vec2 imageRealSize = m_minSize - imageBoder;
		vec2 imageRealSizeMax = m_size - imageBoder;
		
		vec2 tmpSize = m_compositing.GetRealSize();
		if (m_userFill.x()) {
			imageRealSize.setX(imageRealSizeMax.x());
		} else {
			origin.setX(origin.x() + (m_size.x()-m_minSize.x())*0.5f);
		}
		if (m_userFill.y()) {
			imageRealSize.setY(imageRealSizeMax.y());
		} else {
			origin.setY(origin.y() + (m_size.y()-m_minSize.y())*0.5f);
		}
		
		// set the somposition properties :
		m_compositing.SetPos(origin);
		m_compositing.Print(imageRealSize);
		//EWOL_DEBUG("Paint Image at : " << origin << " size=" << imageRealSize << "  origin=" << origin);
	}
}

void widget::Image::CalculateMinMaxSize(void)
{
	vec2 imageBoder = m_border.GetPixel()*2.0f;
	vec2 imageSize = m_imageSize.GetPixel();
	if (imageSize!=vec2(0,0)) {
		m_minSize = imageBoder+imageSize;
		m_maxSize = m_minSize;
	} else {
		vec2 imageSizeReal = m_compositing.GetRealSize();
		vec2 min1 = imageBoder+m_userMinSize.GetPixel();
		m_minSize = imageBoder+imageSizeReal;
		//EWOL_DEBUG(" set max : " << m_minSize << " " << min1);
		m_minSize.setMax(min1);
		//EWOL_DEBUG("     result : " << m_minSize);
		m_maxSize = imageBoder+m_userMaxSize.GetPixel();
		m_minSize.setMin(m_maxSize);
	}
	//EWOL_DEBUG("set widget min=" << m_minSize << " max=" << m_maxSize << " with real Image Size=" << imageSizeReal);
	MarkToRedraw();
}


bool widget::Image::OnEventInput(const ewol::EventInput& _event)
{
	//EWOL_DEBUG("Event on BT ...");
	if (1 == _event.GetId()) {
		if(    ewol::keyEvent::statusSingle == _event.GetStatus()) {
			GenerateEventId(eventPressed);
			return true;
		}
	}
	return false;
}

bool widget::Image::LoadXML(exml::Element* _node)
{
	if (NULL==_node) {
		return false;
	}
	ewol::Widget::LoadXML(_node);
	// get internal data : 
	
	etk::UString tmpAttributeValue = _node->GetAttribute("ratio");
	if (tmpAttributeValue.Size()!=0) {
		if (tmpAttributeValue.CompareNoCase("true")==true) {
			m_keepRatio = true;
		} else if (tmpAttributeValue == "1") {
			m_keepRatio = true;
		} else {
			m_keepRatio = false;
		}
	}
	tmpAttributeValue = _node->GetAttribute("size");
	if (tmpAttributeValue.Size()!=0) {
		//EWOL_CRITICAL(" Parse SIZE : " << tmpAttributeValue);
		m_imageSize = tmpAttributeValue;
		//EWOL_CRITICAL("              ==> " << m_imageSize);
	}
	tmpAttributeValue = _node->GetAttribute("border");
	if (tmpAttributeValue.Size()!=0) {
		m_border = tmpAttributeValue;
	}
	//EWOL_DEBUG("Load label:" << node->ToElement()->GetText());
	if (_node->Size()!=0) {
		SetFile(_node->GetText());
	} else {
		tmpAttributeValue = _node->GetAttribute("src");
		if (tmpAttributeValue.Size()!=0) {
			SetFile(tmpAttributeValue);
		}
	}
	return true;
}


bool widget::Image::OnSetConfig(const ewol::EConfig& _conf)
{
	if (true == ewol::Widget::OnSetConfig(_conf)) {
		return true;
	}
	if (_conf.GetConfig() == configRatio) {
		SetKeepRatio(_conf.GetData().ToBool());
		return true;
	}
	if (_conf.GetConfig() == configSize) {
		SetImageSize(_conf.GetData());
		return true;
	}
	if (_conf.GetConfig() == configBorder) {
		SetBorder(_conf.GetData());
		return true;
	}
	if (_conf.GetConfig() == configSource) {
		SetFile(_conf.GetData());
		return true;
	}
	return false;
}

bool widget::Image::OnGetConfig(const char* _config, etk::UString& _result) const
{
	if (true == ewol::Widget::OnGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configRatio) {
		if (true==GetKeepRatio()) {
			_result = "true";
		} else {
			_result = "false";
		}
		return true;
	}
	if (_config == configSize) {
		_result = GetImageSize();
		return true;
	}
	if (_config == configBorder) {
		_result = GetBorder();
		return true;
	}
	if (_config == configSource) {
		_result = GetFile();
		return true;
	}
	return false;
}

