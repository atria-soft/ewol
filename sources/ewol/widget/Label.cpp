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


widget::Label::Label(etk::UString newLabel)
{
	m_label = newLabel;
	AddEventId(ewolEventLabelPressed);
	SetCanHaveFocus(false);
}

widget::Label::~Label(void)
{
	
}

bool widget::Label::CalculateMinSize(void)
{
	if (m_userMaxSize.x != -1) {
		m_text.SetTextAlignement(0, m_userMaxSize.x-4, ewol::Text::alignLeft);
	}
	ivec3 minSize = m_text.CalculateSizeDecorated(m_label);
	if (m_userMaxSize.x!=-1) {
		m_minSize.x = etk_min(4 + minSize.x, m_userMaxSize.x);
	} else {
		m_minSize.x = 4 + minSize.x;
	}
	if (m_userMaxSize.y!=-1) {
		m_minSize.y = etk_min(4 + minSize.y, m_userMaxSize.y);
	} else {
		m_minSize.y = 4 + minSize.y;
	}
	return true;
}


void widget::Label::SetLabel(etk::UString newLabel)
{
	m_label = newLabel;
	MarkToRedraw();
	ewol::RequestUpdateSize();
}


etk::UString widget::Label::GetLabel(void)
{
	return m_label;
}


void widget::Label::OnDraw(ewol::DrawProperty& displayProp)
{
	m_text.Draw();
}


void widget::Label::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		m_text.Clear();
		int32_t paddingSize = 2;
		
		
		// to know the size of one Line : 
		ivec3 minSize = m_text.CalculateSize('A');
		if (m_userMaxSize.x != -1) {
			m_text.SetTextAlignement(0, m_userMaxSize.x-2*paddingSize, ewol::Text::alignLeft);
		}
		ivec3 curentTextSize = m_text.CalculateSizeDecorated(m_label);
		
		ivec2 localSize = m_minSize;
		
		// no change for the text orogin : 
		vec3 tmpTextOrigin((m_size.x - m_minSize.x) / 2.0,
		                   (m_size.y - m_minSize.y) / 2.0,
		                   0);
		
		if (true==m_userFill.x) {
			localSize.x = m_size.x;
			tmpTextOrigin.x = 0;
		}
		if (true==m_userFill.y) {
			localSize.y = m_size.y;
			tmpTextOrigin.y = m_size.y - 2*paddingSize - curentTextSize.y;
		}
		tmpTextOrigin.x += paddingSize;
		tmpTextOrigin.y += paddingSize;
		localSize.x -= 2*paddingSize;
		localSize.y -= 2*paddingSize;
		
		tmpTextOrigin.y += (m_minSize.y-2*paddingSize) - minSize.y;
		
		vec2 textPos(tmpTextOrigin.x, tmpTextOrigin.y);
		
		vec3 drawClippingPos(paddingSize, paddingSize, -0.5);
		vec3 drawClippingSize((m_size.x - paddingSize),
		                      (m_size.y - paddingSize),
		                      1);
		
		// clean the element
		m_text.Reset();
		m_text.SetPos(tmpTextOrigin);
		m_text.SetTextAlignement(tmpTextOrigin.x, tmpTextOrigin.x+localSize.x, ewol::Text::alignLeft);
		m_text.SetClipping(drawClippingPos, drawClippingSize);
		m_text.PrintDecorated(m_label);
	}
}

bool widget::Label::OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, vec2 pos)
{
	//EWOL_DEBUG("Event on Label ...");
	if (1 == IdInput) {
		if (ewol::keyEvent::statusSingle == typeEvent) {
			// nothing to do ...
			GenerateEventId(ewolEventLabelPressed);
			return true;
		}
	}
	return false;
}


