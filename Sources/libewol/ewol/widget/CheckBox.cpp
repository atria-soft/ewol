/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/CheckBox.h>

#include <ewol/oObject/OObject.h>
#include <ewol/widget/WidgetManager.h>


extern const char * const ewolEventCheckBoxClicked    = "ewol CheckBox Clicked";

#undef __class__
#define __class__	"CheckBox"


void ewol::CheckBox::Init(void)
{
	AddEventId(ewolEventCheckBoxClicked);
	m_textColorFg = draw::color::black;
	m_textColorBg = draw::color::white;
	m_value = false;
	SetCanHaveFocus(true);
	SetMouseLimit(1);
}

ewol::CheckBox::CheckBox(void)
{
	m_label = "No Label";
	Init();
}

ewol::CheckBox::CheckBox(etk::UString newLabel)
{
	m_label = newLabel;
	Init();
}


ewol::CheckBox::~CheckBox(void)
{
	
}


bool ewol::CheckBox::CalculateMinSize(void)
{
	etk::Vector2D<int32_t> minSize = m_oObjectText.GetSize(m_label);
	float boxSize = etk_max(20, minSize.y) + 5;
	m_minSize.x = boxSize+minSize.x;
	m_minSize.y = etk_max(boxSize, minSize.y)+3;
	MarkToRedraw();
	return true;
}


void ewol::CheckBox::SetLabel(etk::UString newLabel)
{
	m_label = newLabel;
	MarkToRedraw();
}

void ewol::CheckBox::SetValue(bool val)
{
	if (m_value == val) {
		return;
	}
	m_value = val;
	MarkToRedraw();
}

bool ewol::CheckBox::GetValue(void)
{
	return m_value;
}

void ewol::CheckBox::OnDraw(DrawProperty& displayProp)
{
	m_oObjectDecoration.Draw();
	m_oObjectText.Draw();
}

void ewol::CheckBox::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		
		m_oObjectDecoration.Clear();
		m_oObjectText.Clear();
		int32_t borderWidth = 2;
		
		
		etk::Vector2D<int32_t> minSize = m_oObjectText.GetSize(m_label);
		float boxSize = etk_max(20, minSize.y) + 5;
		//int32_t fontWidth = ewol::GetWidth(fontId, m_label.c_str());
		int32_t posy = (m_size.y - minSize.y - 6)/2 + 3;
		//int32_t posx = (m_size.x - fontWidth - 6)/2 + 25;
		
		
		etk::Vector2D<float> textPos;
		textPos.x = boxSize+5;
		textPos.y = posy;
		clipping_ts drawClipping;
		drawClipping.x = 0;
		drawClipping.y = 0;
		// note : pb on the clipping properties ...
		drawClipping.w = m_size.x;// - (boxSize+5);
		drawClipping.h = m_size.y;
		m_oObjectText.Text(textPos/*, drawClipping*/, m_label);
		
		
		
		m_oObjectDecoration.SetColor(m_textColorBg);
		m_oObjectDecoration.Rectangle( 2.5, 2.5, boxSize, boxSize);
		m_oObjectDecoration.SetColor(m_textColorFg);
		m_oObjectDecoration.RectangleBorder( 2.5, 2.5, boxSize, boxSize, borderWidth);
		if (m_value) {
			m_oObjectDecoration.Line( 2.5, 2.5, boxSize+2.5, boxSize+2.5, borderWidth);
			m_oObjectDecoration.Line( 2.5, boxSize+2.5, boxSize+2.5, 2.5, borderWidth);
		}
	}
}

bool ewol::CheckBox::OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, etk::Vector2D<float> pos)
{
	//EWOL_DEBUG("Event on checkbox ...");
	if (1 == IdInput) {
		if (ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent) {
			if(true == m_value) {
				m_value = false;
				GenerateEventId(ewolEventCheckBoxClicked, "false");
			} else {
				m_value = true;
				GenerateEventId(ewolEventCheckBoxClicked, "true");
			}
			ewol::widgetManager::FocusKeep(this);
			MarkToRedraw();
			return true;
		}
	}
	return false;
}


bool ewol::CheckBox::OnEventKb(eventKbType_te typeEvent, uniChar_t unicodeData)
{
	//EWOL_DEBUG("BT PRESSED : \"" << UTF8_data << "\" size=" << strlen(UTF8_data));
	if(    typeEvent == ewol::EVENT_KB_TYPE_DOWN
	    && (    unicodeData == '\r'
	         || unicodeData == ' ')
	       ) {
		if(true == m_value) {
			m_value = false;
			GenerateEventId(ewolEventCheckBoxClicked, "false");
		} else {
			m_value = true;
			GenerateEventId(ewolEventCheckBoxClicked, "true");
		}
		MarkToRedraw();
		return true;
	}
	return false;
}

