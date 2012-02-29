/**
 *******************************************************************************
 * @file ewol/widget/Entry.cpp
 * @brief ewol entry widget system (Sources)
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


#include <etk/unicode.h>
#include <ewol/widget/Entry.h>
#include <ewol/OObject.h>
#include <ewol/WidgetManager.h>
#include <ewol/ewol.h>


extern const char * const ewolEventEntryClick      = "ewol-Entry-click";
extern const char * const ewolEventEntryEnter      = "ewol-Entry-Enter";
extern const char * const ewolEventEntryModify     = "ewol-Entry-Modify";



#undef __class__
#define __class__	"Entry"


void ewol::Entry::Init(void)
{
	AddEventId(ewolEventEntryClick);
	AddEventId(ewolEventEntryEnter);
	AddEventId(ewolEventEntryModify);
	m_displayStartPosition = 0;
	m_displayCursorPos = 0;
	m_userSize = 50;
	m_borderSize = 2;
	m_paddingSize = 3;
	m_displayCursor = false;
	m_textColorFg.red   = 0.0;
	m_textColorFg.green = 0.0;
	m_textColorFg.blue  = 0.0;
	m_textColorFg.alpha = 1.0;
	
	m_textColorBg.red   = 1.0;
	m_textColorBg.green = 1.0;
	m_textColorBg.blue  = 1.0;
	m_textColorBg.alpha = 0.75;
	SetCanHaveFocus(true);
}

ewol::Entry::Entry(void)
{
	Init();
	m_data = "";
	UpdateTextPosition();
	MarkToReedraw();
}

ewol::Entry::Entry(etk::UString newData)
{
	Init();
	SetValue(newData);
	UpdateTextPosition();
	MarkToReedraw();
}


ewol::Entry::~Entry(void)
{
	
}

bool ewol::Entry::CalculateMinSize(void)
{
	int32_t fontId = GetDefaultFontId();
	int32_t minHeight = ewol::GetHeight(fontId);
	m_minSize.x = m_userSize;
	m_minSize.y = minHeight + 2*(m_borderSize + 2*m_paddingSize);
	UpdateTextPosition();
	MarkToReedraw();
	return true;
}


void ewol::Entry::SetValue(etk::UString newData)
{
	m_data = newData;
	UpdateTextPosition();
	MarkToReedraw();
}

etk::UString ewol::Entry::GetValue(void)
{
	return m_data;
}


void ewol::Entry::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		// clean the object list ...
		ClearOObjectList();
		
		// TODO later : Add this in the basic element of the widget ...
		
		int32_t tmpSizeX = m_minSize.x;
		int32_t tmpSizeY = m_minSize.y;
		int32_t tmpOriginX = 0;
		int32_t tmpOriginY = (m_size.y - tmpSizeY) / 2;
		// no change for the text orogin : 
		int32_t tmpTextOriginX = m_borderSize + 2*m_paddingSize;
		int32_t tmpTextOriginY = tmpOriginY + m_borderSize + 2*m_paddingSize;
		
		if (true==m_userFillX) {
			tmpSizeX = m_size.x;
		}
		if (true==m_userFillY) {
			//tmpSizeY = m_size.y;
			tmpOriginY = 0;
			tmpTextOriginY = tmpOriginY + m_borderSize + 2*m_paddingSize;
		}
		tmpOriginX += m_paddingSize;
		tmpOriginY += m_paddingSize;
		tmpSizeX -= 2*m_paddingSize;
		tmpSizeY -= 2*m_paddingSize;
		
		ewol::OObject2DText * tmpText = new ewol::OObject2DText("", -1, m_textColorFg);
		
		etk::UString tmpDisplay = m_data.Extract(m_displayStartPosition);
		
		coord2D_ts textPos;
		textPos.x = tmpTextOriginX;
		textPos.y = tmpTextOriginY;
		clipping_ts drawClipping;
		drawClipping.x = 0;
		drawClipping.y = 0;
		drawClipping.w = m_size.x - (m_borderSize + 2*m_paddingSize);
		drawClipping.h = m_size.y;
		tmpText->Text(textPos, drawClipping, tmpDisplay);
		
		ewol::OObject2DColored * tmpOObjects = new ewol::OObject2DColored;
		tmpOObjects->SetColor(m_textColorBg);
		tmpOObjects->Rectangle( tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY);
		tmpOObjects->SetColor(m_textColorFg);
		tmpOObjects->RectangleBorder( tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY, m_borderSize);
		if (true == m_displayCursor) {
			int32_t fontId = GetDefaultFontId();
			int32_t fontHeight = ewol::GetHeight(fontId);
			int32_t fontWidth = ewol::GetWidth(fontId, tmpDisplay);
			int32_t XCursorPos = fontWidth + m_borderSize + 2*m_paddingSize;
			tmpOObjects->Line(XCursorPos, tmpTextOriginY, XCursorPos, tmpTextOriginY + fontHeight, 1);
		}
		AddOObject(tmpOObjects);
		AddOObject(tmpText);
	}
}


bool ewol::Entry::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, eventPosition_ts pos)
{
	//EWOL_DEBUG("Event on Entry ...");
	if (1 == IdInput) {
		if (ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent) {
			// nothing to do ...
			GenerateEventId(ewolEventEntryClick);
			ewol::widgetManager::FocusKeep(this);
			MarkToReedraw();
			return true;
		}
	}
	return false;
}


bool ewol::Entry::OnEventKb(eventKbType_te typeEvent, uniChar_t unicodeData)
{
	if( typeEvent == ewol::EVENT_KB_TYPE_DOWN) {
		//EWOL_DEBUG("Entry input data ... : \"" << unicodeData << "\" " );
		//return GenEventInputExternal(ewolEventEntryEnter, -1, -1);
	if (0x7F == unicodeData) {
			// SUPPR
		} else if (0x08 == unicodeData) {
			// DEL : 
			m_data.Remove(m_data.Size()-1, 1);
		} else if(unicodeData >= 20) {
			char UTF8_data[50];
			unicode::convertUnicodeToUtf8(unicodeData, UTF8_data);
			m_data += UTF8_data;
		}
		GenerateEventId(ewolEventEntryModify);
		UpdateTextPosition();
		MarkToReedraw();
		return true;
	}
	return false;
}


void ewol::Entry::UpdateTextPosition(void)
{
	int32_t iii=m_data.Size();
	int32_t fontId = GetDefaultFontId();
	
	int32_t tmpSizeX = m_minSize.x;
	if (true==m_userFillX) {
		tmpSizeX = m_size.x;
	}
	int32_t tmpUserSize = tmpSizeX - 2*(m_borderSize + 2*m_paddingSize);
	while (iii > 0) {
		if (ewol::GetWidth(fontId, m_data[iii]) > tmpUserSize) {
			break;
		}
		iii--;
	}
	iii++;
	m_displayStartPosition = iii-1;
	m_displayCursorPos = m_data.Size();
}


void ewol::Entry::OnGetFocus(void)
{
	m_displayCursor = true;
	ewol::KeyboardShow(ewol::KEYBOARD_MODE_CODE);
	MarkToReedraw();
}

void ewol::Entry::OnLostFocus(void)
{
	m_displayCursor = false;
	ewol::KeyboardHide();
	MarkToReedraw();
}
