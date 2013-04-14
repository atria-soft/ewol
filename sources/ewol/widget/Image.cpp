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


extern const char * const ewolEventImagePressed = "ewol-image-Pressed";

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


widget::Image::Image(const etk::UString& file, const ewol::Dimension& border) :
	m_imageSize(vec2(0,0)),
	m_keepRatio(true)
{
	AddEventId(ewolEventImagePressed);
	Set(file, border);
}


void widget::Image::SetFile(const etk::UString& file)
{
	// copy data :
	m_fileName = file;
	// Force redraw all :
	MarkToRedraw();
	ewol::RequestUpdateSize();
	m_compositing.SetSource(m_fileName, vec2(64,64));
}

void widget::Image::SetBorder(const ewol::Dimension& border)
{
	// copy data :
	m_border = border;
	// Force redraw all :
	MarkToRedraw();
	// TODO : Change the size with no size requested ...
	ewol::RequestUpdateSize();
}

void widget::Image::SetKeepRatio(bool keep)
{
	if (m_keepRatio != keep) {
		// copy data :
		m_keepRatio = keep;
		// Force redraw all :
		MarkToRedraw();
		ewol::RequestUpdateSize();
	}
}

void widget::Image::SetImageSize(const ewol::Dimension& size)
{
	m_imageSize = size;
	MarkToRedraw();
	ewol::RequestUpdateSize();
	m_compositing.SetSource(m_fileName, m_imageSize.GetPixel());
}

void widget::Image::Set(const etk::UString& file, const ewol::Dimension& border)
{
	// copy data :
	m_border = border;
	m_fileName = file;
	// Force redraw all :
	MarkToRedraw();
	ewol::RequestUpdateSize();
	m_compositing.SetSource(m_fileName, m_imageSize.GetPixel());
}


void widget::Image::OnDraw(ewol::DrawProperty& displayProp)
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
		EWOL_DEBUG("Paint Image at : " << origin << " size=" << imageRealSize << "  origin=" << origin);
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


bool widget::Image::OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, const vec2& pos)
{
	//EWOL_DEBUG("Event on BT ...");
	if (1 == IdInput) {
		if(    ewol::keyEvent::statusSingle == typeEvent) {
			GenerateEventId(ewolEventImagePressed);
			return true;
		}
	}
	return false;
}

bool widget::Image::LoadXML(TiXmlNode* node)
{
	if (NULL==node) {
		return false;
	}
	ewol::Widget::LoadXML(node);
	// get internal data : 
	
	const char *tmpAttributeValue = node->ToElement()->Attribute("ratio");
	if (NULL != tmpAttributeValue) {
		if (strcmp(tmpAttributeValue,"true")==0) {
			m_keepRatio = true;
		} else if (strcmp(tmpAttributeValue,"1")==0) {
			m_keepRatio = true;
		} else {
			m_keepRatio = false;
		}
	}
	tmpAttributeValue = node->ToElement()->Attribute("size");
	if (NULL != tmpAttributeValue) {
		//EWOL_CRITICAL(" Parse SIZE : " << tmpAttributeValue);
		m_imageSize = tmpAttributeValue;
		//EWOL_CRITICAL("              ==> " << m_imageSize);
	}
	tmpAttributeValue = node->ToElement()->Attribute("border");
	if (NULL != tmpAttributeValue) {
		m_border = tmpAttributeValue;
	}
	//EWOL_DEBUG("Load label:" << node->ToElement()->GetText());
	if (node->ToElement()->GetText() != NULL) {
		SetFile(node->ToElement()->GetText());
	} else {
		tmpAttributeValue = node->ToElement()->Attribute("src");
		if (NULL != tmpAttributeValue) {
			SetFile(tmpAttributeValue);
		}
	}
	return true;
}
