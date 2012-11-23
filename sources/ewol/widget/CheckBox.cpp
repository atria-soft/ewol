/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/CheckBox.h>
#include <ewol/widget/WidgetManager.h>


extern const char * const ewolEventCheckBoxClicked    = "ewol CheckBox Clicked";

#undef __class__
#define __class__	"CheckBox"


void widget::CheckBox::Init(void)
{
	AddEventId(ewolEventCheckBoxClicked);
	m_textColorFg = draw::color::black;
	m_textColorBg = draw::color::white;
	m_value = false;
	SetCanHaveFocus(true);
	SetMouseLimit(1);
}

widget::CheckBox::CheckBox(void)
{
	m_label = "No Label";
	Init();
}

widget::CheckBox::CheckBox(etk::UString newLabel)
{
	m_label = newLabel;
	Init();
}


widget::CheckBox::~CheckBox(void)
{
	
}


bool widget::CheckBox::CalculateMinSize(void)
{
	etk::Vector3D<int32_t> minSize = m_oObjectText.CalculateSize(m_label);
	float boxSize = etk_max(20, minSize.y) + 5;
	m_minSize.x = boxSize+minSize.x;
	m_minSize.y = etk_max(boxSize, minSize.y)+3;
	MarkToRedraw();
	return true;
}


void widget::CheckBox::SetLabel(etk::UString newLabel)
{
	m_label = newLabel;
	MarkToRedraw();
}

void widget::CheckBox::SetValue(bool val)
{
	if (m_value == val) {
		return;
	}
	m_value = val;
	MarkToRedraw();
}

bool widget::CheckBox::GetValue(void)
{
	return m_value;
}

void widget::CheckBox::OnDraw(ewol::DrawProperty& displayProp)
{
	m_oObjectDecoration.Draw();
	m_oObjectText.Draw();
}

void widget::CheckBox::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		
		m_oObjectDecoration.Clear();
		m_oObjectText.Clear();
		
		etk::Vector3D<int32_t> minSize = m_oObjectText.CalculateSize(m_label);
		float boxSize = etk_max(20, minSize.y) + 5;
		//int32_t fontWidth = ewol::GetWidth(fontId, m_label.c_str());
		int32_t posy = (m_size.y - minSize.y - 6)/2 + 3;
		//int32_t posx = (m_size.x - fontWidth - 6)/2 + 25;
		
		
		etk::Vector3D<float> textPos;
		textPos.x = boxSize+5;
		textPos.y = posy;
		textPos.z = 0;
		m_oObjectText.SetPos(textPos);
		m_oObjectText.Print(m_label);
		
		m_oObjectDecoration.SetColor(m_textColorBg);
		m_oObjectDecoration.SetPos(etk::Vector3D<float>(2.5f, 2.5f, 0.0f) );
		m_oObjectDecoration.RectangleWidth(etk::Vector3D<float>(boxSize, boxSize, 0.0f) );
		if (m_value) {
			m_oObjectDecoration.SetColor(m_textColorFg);
			m_oObjectDecoration.SetPos(etk::Vector3D<float>(2.5f, 2.5f, 0.0f) );
			m_oObjectDecoration.SetThickness(3);
			m_oObjectDecoration.LineRel(etk::Vector3D<float>(boxSize, boxSize, 0.0f) );
		}
	}
}

bool widget::CheckBox::OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, etk::Vector2D<float> pos)
{
	//EWOL_DEBUG("Event on checkbox ...");
	if (1 == IdInput) {
		if (ewol::keyEvent::statusSingle == typeEvent) {
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


bool widget::CheckBox::OnEventKb(ewol::keyEvent::status_te typeEvent, uniChar_t unicodeData)
{
	//EWOL_DEBUG("BT PRESSED : \"" << UTF8_data << "\" size=" << strlen(UTF8_data));
	if(    typeEvent == ewol::keyEvent::statusDown
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

