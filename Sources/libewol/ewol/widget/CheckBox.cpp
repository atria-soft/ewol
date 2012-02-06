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
#define __class__	"ewol::CheckBox"


void ewol::CheckBox::Init(void)
{
	AddEventId(ewolEventCheckBoxClicked);
	m_textColorFg.red   = 0.0;
	m_textColorFg.green = 0.0;
	m_textColorFg.blue  = 0.0;
	m_textColorFg.alpha = 1.0;
	
	m_textColorBg.red   = 1.0;
	m_textColorBg.green = 1.0;
	m_textColorBg.blue  = 1.0;
	m_textColorBg.alpha = 1.0;
	m_value = false;
	SetCanHaveFocus(true);
}

ewol::CheckBox::CheckBox(void)
{
	m_label = "No Label";
	Init();
}

ewol::CheckBox::CheckBox(etk::String newLabel)
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
	int32_t minWidth = ewol::GetWidth(fontId, m_label.c_str());
	int32_t minHeight = ewol::GetHeight(fontId);
	etkFloat_t boxSize = etk_max(20, minHeight) + 5;
	m_minSize.x = boxSize+minWidth;
	m_minSize.y = etk_max(boxSize, minHeight)+3;
	return true;
}


void ewol::CheckBox::SetLabel(etk::String newLabel)
{
	m_label = newLabel;
}

void ewol::CheckBox::SetValue(bool val)
{
	if (m_value == val) {
		return;
	}
	m_value = val;
	MarkToReedraw();
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
		etkFloat_t boxSize = etk_max(20, fontHeight);
		//int32_t fontWidth = ewol::GetWidth(fontId, m_label.c_str());
		int32_t posy = (m_size.y - fontHeight - 6)/2 + 3;
		//int32_t posx = (m_size.x - fontWidth - 6)/2 + 25;
		tmpText->Text(boxSize+5, posy, m_label.c_str(), m_size.x - (boxSize+5));
		
		
		ewol::OObject2DColored * tmpOObjects = new ewol::OObject2DColored;
		tmpOObjects->SetColor(m_textColorBg);
		tmpOObjects->Rectangle( 2.5, 2.5, boxSize, boxSize);
		tmpOObjects->SetColor(m_textColorFg);
		tmpOObjects->RectangleBorder( 2.5, 2.5, boxSize, boxSize, borderWidth);
		if (m_value) {
			tmpOObjects->Line( 2.5, 2.5, boxSize+2.5, boxSize+2.5, borderWidth);
			tmpOObjects->Line( 2.5, boxSize+2.5, boxSize+2.5, 2.5, borderWidth);
		}
		
		AddOObject(tmpOObjects, "Decoration");
		AddOObject(tmpText, "Text");
	
		// Regenerate the event Area:
		EventAreaRemoveAll();
		coord origin;
		coord size;
		origin.x = 3.0;
		origin.y = 3.0;
		size.x = m_size.x-6;
		size.y = m_size.y-6;
		AddEventArea(origin, size, FLAG_EVENT_INPUT_1 | FLAG_EVENT_INPUT_CLICKED_ALL, ewolEventCheckBoxClicked);
	}
}

/*
bool ewol::CheckBox::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y)
{
	EWOL_DEBUG("Event on BT ...");
	return true;
}
*/

bool ewol::CheckBox::OnEventArea(const char * generateEventId, etkFloat_t x, etkFloat_t y)
{
	bool eventIsOK = false;
	//EWOL_DEBUG("Receive event : \"" << generateEventId << "\"");
	if(ewolEventCheckBoxClicked == generateEventId) {
		ewol::widgetManager::FocusKeep(this);
		EWOL_INFO("CB pressed ... " << m_label);
		if(true == m_value) {
			m_value = false;
		} else {
			m_value = true;
		}
		MarkToReedraw();
		eventIsOK = true;
	}
	return eventIsOK;
}

bool ewol::CheckBox::OnEventKb(eventKbType_te typeEvent, char UTF8_data[UTF8_MAX_SIZE])
{
	//EWOL_DEBUG("BT PRESSED : \"" << UTF8_data << "\" size=" << strlen(UTF8_data));
	if(    UTF8_data != NULL
	    && typeEvent == ewol::EVENT_KB_TYPE_DOWN
	    && (    UTF8_data[0] == '\r'
	         || UTF8_data[0] == ' ')
	       ) {
		return OnEventArea(ewolEventCheckBoxClicked, -1, -1);
	}
	return false;
}

