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
	etkFloat_t boxSize = etk_max(20, minHeight) + 5;
	m_minSize.x = boxSize+minWidth;
	m_minSize.y = etk_max(boxSize, minHeight)+3;
	MarkToReedraw();
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
		
		
		coord2D_ts textPos;
		textPos.x = boxSize+5;
		textPos.y = posy;
		clipping_ts drawClipping;
		drawClipping.x = 0;
		drawClipping.y = 0;
		drawClipping.w = m_size.x - (boxSize+5);
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
		
		AddOObject(tmpOObjects, "Decoration");
		AddOObject(tmpText, "Text");
	}
}


bool ewol::CheckBox::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y)
{
	//EWOL_DEBUG("Event on checkbox ...");
	if (1 == IdInput) {
		if (ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent) {
			if(true == m_value) {
				m_value = false;
			} else {
				m_value = true;
			}
			GenEventInputExternal(ewolEventCheckBoxClicked, x, y);
			ewol::widgetManager::FocusKeep(this);
			MarkToReedraw();
			return true;
		}
	}
	return false;
}


bool ewol::CheckBox::OnEventKb(eventKbType_te typeEvent, char UTF8_data[UTF8_MAX_SIZE])
{
	//EWOL_DEBUG("BT PRESSED : \"" << UTF8_data << "\" size=" << strlen(UTF8_data));
	if(    UTF8_data != NULL
	    && typeEvent == ewol::EVENT_KB_TYPE_DOWN
	    && (    UTF8_data[0] == '\r'
	         || UTF8_data[0] == ' ')
	       ) {
		if(true == m_value) {
			m_value = false;
		} else {
			m_value = true;
		}
		MarkToReedraw();
		return GenEventInputExternal(ewolEventCheckBoxClicked, -1,-1);
	}
	return false;
}

