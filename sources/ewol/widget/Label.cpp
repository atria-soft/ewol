/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Label.h>

#include <ewol/oObject/OObject.h>
#include <ewol/widget/WidgetManager.h>


extern const char * const ewolEventLabelPressed    = "ewol Label Pressed";



#undef __class__
#define __class__	"Label"


void ewol::Label::Init(void)
{
	AddEventId(ewolEventLabelPressed);
	m_textColorFg = draw::color::black;
	SetCanHaveFocus(false);
}

ewol::Label::Label(void)
{
	m_label = "---";
	Init();
}

ewol::Label::Label(etk::UString newLabel)
{
	m_label = newLabel;
	Init();
}


ewol::Label::~Label(void)
{
	
}


bool ewol::Label::CalculateMinSize(void)
{
	etk::Vector2D<int32_t> minSize = m_oObjectText.GetSize(m_label);
	m_minSize.x = 3 + minSize.x;
	m_minSize.y = 3 + minSize.y;
	return true;
}


void ewol::Label::SetLabel(etk::UString newLabel)
{
	m_label = newLabel;
	MarkToRedraw();
}

void ewol::Label::OnDraw(DrawProperty& displayProp)
{
	m_oObjectText.Draw();
}


void ewol::Label::OnRegenerateDisplay(void)
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
		
		
		etk::Vector2D<float> textPos(tmpOriginX, tmpOriginY);
		clipping_ts drawClipping;
		drawClipping.x = paddingSize;
		drawClipping.y = paddingSize;
		drawClipping.w = m_size.x - 2*paddingSize;
		drawClipping.h = m_size.y - 2*paddingSize;
		m_oObjectText.Text(textPos/*, drawClipping*/, m_label);
	}
}

bool ewol::Label::OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, etk::Vector2D<float> pos)
{
	//EWOL_DEBUG("Event on Label ...");
	if (1 == IdInput) {
		if (ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent) {
			// nothing to do ...
			GenerateEventId(ewolEventLabelPressed);
			return true;
		}
	}
	return false;
}


