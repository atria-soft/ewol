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


extern const char * const ewolEventImagePressed    = "ewol-image-Pressed";

#undef __class__
#define __class__	"Image"

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


widget::Image::Image(const etk::UString& file, const ewol::Dimension& size, const ewol::Dimension& border)
{
	Set(file, border, size);
}


void widget::Image::SetFile(const etk::UString& file)
{
	// copy data :
	m_fileName = file;
	// Force redraw all :
	MarkToRedraw();
	ewol::RequestUpdateSize();
	m_compositing.SetSource(m_fileName, m_size);
}

void widget::Image::SetBorder(const ewol::Dimension& border)
{
	// copy data :
	m_border = border;
	// Force redraw all :
	MarkToRedraw();
	ewol::RequestUpdateSize();
}

void widget::Image::SetSize(const ewol::Dimension& size)
{
	// copy data :
	m_imageSize = size;
	// Force redraw all :
	MarkToRedraw();
	if (m_compositing.HasSources()) {
		m_compositing.SetSource(m_fileName, m_size);
	}
}

void widget::Image::Set(const etk::UString& file, const ewol::Dimension& size, const ewol::Dimension& border)
{
	// copy data :
	m_border = border;
	m_imageSize = size;
	m_fileName = file;
	// Force redraw all :
	MarkToRedraw();
	ewol::RequestUpdateSize();
	m_compositing.SetSource(m_fileName, m_size);
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
		vec2 origin = m_origin + m_border.GetPixel();
		// set the somposition properties :
		m_compositing.SetPos(vec3(origin.x(), origin.y(), 0) );
		m_compositing.Print(m_imageSize.GetPixel());
		EWOL_DEBUG("Paint Image at : " << origin << " size=" << m_imageSize.GetPixel() << "  border=" << m_border.GetPixel());
	}
}

void widget::Image::CalculateMinMaxSize(void)
{
	m_minSize = m_border.GetPixel()*2+m_imageSize.GetPixel();
	m_maxSize = m_userMaxSize.GetPixel();
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
	// TODO : Unparse data type XML ...
	//EWOL_DEBUG("Load label:" << node->ToElement()->GetText());
	//SetLabel(node->ToElement()->GetText());
	return true;
}

