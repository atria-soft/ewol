/**
 *******************************************************************************
 * @file ewol/widget/CheckBox.cpp
 * @brief ewol check-box widget system (Sources)
 * @author Edouard DUPIN
 * @date 07/11/2011
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */


#include <ewol/widget/CheckBox.h>

#include <ewol/OObject.h>
#include <ewol/WidgetManager.h>


extern const char * const ewolEventCheckBoxClicked    = "ewol CheckBox Clicked";

#undef __class__
#define __class__	"CheckBox"


void ewol::CheckBox::Init(void)
{
	AddEventId(ewolEventCheckBoxClicked);
	m_textColorFg = etk::color::color_Black;
	m_textColorBg = etk::color::color_White;
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
	int32_t fontId = GetDefaultFontId();
	int32_t minWidth = ewol::GetWidth(fontId, m_label);
	int32_t minHeight = ewol::GetHeight(fontId);
	float boxSize = etk_max(20, minHeight) + 5;
	m_minSize.x = boxSize+minWidth;
	m_minSize.y = etk_max(boxSize, minHeight)+3;
	MarkToRedraw();
	return true;
}


void ewol::CheckBox::SetLabel(etk::UString newLabel)
{
	m_label = newLabel;
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


void ewol::CheckBox::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		// clean the object list ...
		ClearOObjectList();
		
		int32_t borderWidth = 2;
		
		
		ewol::OObject2DText * tmpText = new ewol::OObject2DText("", -1, m_textColorFg);
		int32_t fontId = GetDefaultFontId();
		int32_t fontHeight = ewol::GetHeight(fontId);
		float boxSize = etk_max(20, fontHeight);
		//int32_t fontWidth = ewol::GetWidth(fontId, m_label.c_str());
		int32_t posy = (m_size.y - fontHeight - 6)/2 + 3;
		//int32_t posx = (m_size.x - fontWidth - 6)/2 + 25;
		
		
		Vector2D<float> textPos;
		textPos.x = boxSize+5;
		textPos.y = posy;
		clipping_ts drawClipping;
		drawClipping.x = 0;
		drawClipping.y = 0;
		// note : pb on the clipping properties ...
		drawClipping.w = m_size.x;// - (boxSize+5);
		drawClipping.h = m_size.y;
		tmpText->Text(textPos, drawClipping, m_label);
		
		
		
		ewol::OObject2DColored * tmpOObjects = new ewol::OObject2DColored;
		tmpOObjects->SetColor(m_textColorBg);
		tmpOObjects->Rectangle( 2.5, 2.5, boxSize, boxSize);
		tmpOObjects->SetColor(m_textColorFg);
		tmpOObjects->RectangleBorder( 2.5, 2.5, boxSize, boxSize, borderWidth);
		if (m_value) {
			tmpOObjects->Line( 2.5, 2.5, boxSize+2.5, boxSize+2.5, borderWidth);
			tmpOObjects->Line( 2.5, boxSize+2.5, boxSize+2.5, 2.5, borderWidth);
		}
		
		AddOObject(tmpOObjects);
		AddOObject(tmpText);
	}
}

/**
 * @brief Event on an input of this Widget
 * @param[in] type Type of the input (ewol::INPUT_TYPE_MOUSE/ewol::INPUT_TYPE_FINGER ...)
 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
 * @param[in] pos Absolute position of the event
 * @return true the event is used
 * @return false the event is not used
 */
bool ewol::CheckBox::OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, Vector2D<float> pos)
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

