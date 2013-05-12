/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Label.h>

#include <ewol/compositing/Text.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/ewol.h>

extern const char * const ewolEventLabelPressed    = "ewol Label Pressed";

#undef __class__
#define __class__	"Label"

static ewol::Widget* Create(void)
{
	return new widget::Label();
}

void widget::Label::Init(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,&Create);
}

void widget::Label::UnInit(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,NULL);
}

widget::Label::Label(etk::UString _newLabel)
{
	m_label = _newLabel;
	AddEventId(ewolEventLabelPressed);
	SetCanHaveFocus(false);
}

void widget::Label::CalculateMinMaxSize(void)
{
	vec2 tmpMax = m_userMaxSize.GetPixel();
	if (tmpMax.x() <= 999999) {
		m_text.SetTextAlignement(0, tmpMax.x()-4, ewol::Text::alignLeft);
	}
	vec3 minSize = m_text.CalculateSizeDecorated(m_label);
	
	m_minSize.setX(etk_min(4 + minSize.x(), tmpMax.x()));
	m_minSize.setY(etk_min(4 + minSize.y(), tmpMax.y()));
}

void widget::Label::SetLabel(const etk::UString& _newLabel)
{
	m_label = _newLabel;
	MarkToRedraw();
	ewol::RequestUpdateSize();
}

etk::UString widget::Label::GetLabel(void)
{
	return m_label;
}

void widget::Label::OnDraw(void)
{
	m_text.Draw();
}

void widget::Label::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		m_text.Clear();
		int32_t paddingSize = 2;
		
		vec2 tmpMax = m_userMaxSize.GetPixel();
		// to know the size of one Line : 
		vec3 minSize = m_text.CalculateSize('A');
		if (tmpMax.x() <= 999999) {
			m_text.SetTextAlignement(0, tmpMax.x()-2*paddingSize, ewol::Text::alignLeft);
		}
		vec3 curentTextSize = m_text.CalculateSizeDecorated(m_label);
		
		ivec2 localSize = m_minSize;
		
		// no change for the text orogin : 
		vec3 tmpTextOrigin((m_size.x() - m_minSize.x()) / 2.0,
		                   (m_size.y() - m_minSize.y()) / 2.0,
		                   0);
		
		if (true==m_userFill.x()) {
			localSize.setX(m_size.x());
			tmpTextOrigin.setX(0);
		}
		if (true==m_userFill.y()) {
			localSize.setY(m_size.y());
			tmpTextOrigin.setY(m_size.y() - 2*paddingSize - curentTextSize.y());
		}
		tmpTextOrigin += vec3(paddingSize, paddingSize, 0);
		localSize -= vec2(2*paddingSize,2*paddingSize);
		
		tmpTextOrigin.setY( tmpTextOrigin.y() + (m_minSize.y()-2*paddingSize) - minSize.y());
		
		vec2 textPos(tmpTextOrigin.x(), tmpTextOrigin.y());
		
		vec3 drawClippingPos(paddingSize, paddingSize, -0.5);
		vec3 drawClippingSize((m_size.x() - paddingSize),
		                      (m_size.y() - paddingSize),
		                      1);
		
		// clean the element
		m_text.Reset();
		m_text.SetPos(tmpTextOrigin);
		m_text.SetTextAlignement(tmpTextOrigin.x(), tmpTextOrigin.x()+localSize.x(), ewol::Text::alignLeft);
		m_text.SetClipping(drawClippingPos, drawClippingSize);
		m_text.PrintDecorated(m_label);
	}
}

bool widget::Label::OnEventInput(const ewol::EventInput& _event)
{
	//EWOL_DEBUG("Event on Label ...");
	if (1 == _event.GetId()) {
		if (ewol::keyEvent::statusSingle == _event.GetStatus()) {
			// nothing to do ...
			GenerateEventId(ewolEventLabelPressed);
			return true;
		}
	}
	return false;
}

bool widget::Label::LoadXML(TiXmlNode* _node)
{
	if (NULL==_node) {
		return false;
	}
	ewol::Widget::LoadXML(_node);
	// get internal data : 
	// TODO : Unparse data type XML ...
	EWOL_DEBUG("Load label:" << _node->ToElement()->GetText());
	SetLabel(_node->ToElement()->GetText());
	return true;
}
