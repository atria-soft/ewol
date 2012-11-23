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


extern const char * const ewolEventLabelPressed    = "ewol Label Pressed";



#undef __class__
#define __class__	"Label"


void widget::Label::Init(void)
{
	AddEventId(ewolEventLabelPressed);
	m_textColorFg = draw::color::black;
	SetCanHaveFocus(false);
}

widget::Label::Label(void)
{
	m_label = "---";
	Init();
}

widget::Label::Label(etk::UString newLabel)
{
	m_label = newLabel;
	Init();
}


widget::Label::~Label(void)
{
	
}


bool widget::Label::CalculateMinSize(void)
{
	etk::Vector3D<int32_t> minSize = m_oObjectText.CalculateSize(m_label);
	m_minSize.x = 3 + minSize.x;
	m_minSize.y = 3 + minSize.y;
	return true;
}


void widget::Label::SetLabel(etk::UString newLabel)
{
	m_label = newLabel;
	MarkToRedraw();
}

void widget::Label::OnDraw(ewol::DrawProperty& displayProp)
{
	m_oObjectText.Draw();
}


void widget::Label::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		m_oObjectText.Clear();
		int32_t paddingSize = 3;
		
		int32_t tmpOriginX = 0;
		int32_t tmpOriginY = 0;
		
		if (true==m_userFill.x) {
			tmpOriginX = (m_size.x - m_minSize.x) / 2;
		}
		if (true==m_userFill.y) {
			tmpOriginY = (m_size.y - m_minSize.y) / 2;
		}
		tmpOriginX += paddingSize;
		tmpOriginY += paddingSize;
		
		
		etk::Vector3D<float> textPos(tmpOriginX, tmpOriginY, 0);
		m_oObjectText.SetPos(textPos);
		m_oObjectText.Print(m_label);
		
	}
}

bool widget::Label::OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, etk::Vector2D<float> pos)
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


