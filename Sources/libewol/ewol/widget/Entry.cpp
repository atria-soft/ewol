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
	m_textColorFg = etk::color::black;
	
	m_textColorBg = etk::color::white;
	m_textColorBg.alpha = 0xAF;
	SetCanHaveFocus(true);
}

ewol::Entry::Entry(void)
{
	Init();
	m_data = "";
	UpdateTextPosition();
	MarkToRedraw();
}

ewol::Entry::Entry(etk::UString newData)
{
	Init();
	SetValue(newData);
	UpdateTextPosition();
	MarkToRedraw();
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
	MarkToRedraw();
	return true;
}


void ewol::Entry::SetValue(etk::UString newData)
{
	m_data = newData;
	m_displayCursorPos = m_data.Size();
	EWOL_DEBUG("Set ... " << newData);
	MarkToRedraw();
}

etk::UString ewol::Entry::GetValue(void)
{
	return m_data;
}


void ewol::Entry::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		UpdateTextPosition();
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
		
		Vector2D<float> textPos;
		textPos.x = tmpTextOriginX + m_displayStartPosition;
		textPos.y = tmpTextOriginY;
		clipping_ts drawClipping;
		drawClipping.x = 2*m_paddingSize + m_borderSize;
		drawClipping.y = 2*m_paddingSize + m_borderSize;
		drawClipping.w = m_size.x;// - (m_borderSize + 2*m_paddingSize);
		drawClipping.h = m_size.y;
		tmpText->Text(textPos, drawClipping, m_data);
		
		ewol::OObject2DColored * tmpOObjects = new ewol::OObject2DColored;
		tmpOObjects->SetColor(m_textColorBg);
		tmpOObjects->Rectangle( tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY);
		tmpOObjects->SetColor(m_textColorFg);
		tmpOObjects->RectangleBorder( tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY, m_borderSize);
		if (true == m_displayCursor) {
			int32_t fontId = GetDefaultFontId();
			int32_t fontHeight = ewol::GetHeight(fontId);
			etk::UString tmpDisplay = m_data.Extract(0, m_displayCursorPos);
			int32_t fontWidth = ewol::GetWidth(fontId, tmpDisplay);
			int32_t XCursorPos = fontWidth + m_borderSize + 2*m_paddingSize + m_displayStartPosition;
			if (XCursorPos >= m_borderSize + 2*m_paddingSize) {
				tmpOObjects->Line(XCursorPos, tmpTextOriginY, XCursorPos, tmpTextOriginY + fontHeight, 1);
			}
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
bool ewol::Entry::OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, Vector2D<float> pos)
{
	//EWOL_DEBUG("Event on Entry ...");
	if (1 == IdInput) {
		if (ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent) {
			GenerateEventId(ewolEventEntryClick);
			KeepFocus();
			MarkToRedraw();
			Vector2D<float> relPos = RelativePosition(pos);
			// try to find the new cursor position :
			int32_t fontId = GetDefaultFontId();
			etk::UString tmpDisplay = m_data.Extract(0, m_displayStartPosition);
			int32_t displayHidenSize = ewol::GetWidth(fontId, tmpDisplay);
			//EWOL_DEBUG("hidenSize : " << displayHidenSize);
			int32_t newCursorPosition = -1;
			int32_t tmpTextOriginX = m_borderSize + 2*m_paddingSize;
			for (int32_t iii=0; iii<m_data.Size(); iii++) {
				tmpDisplay = m_data.Extract(0, iii);
				int32_t tmpWidth = ewol::GetWidth(fontId, tmpDisplay) - displayHidenSize;
				if (tmpWidth>=relPos.x-tmpTextOriginX) {
					newCursorPosition = iii;
					break;
				}
			}
			if (newCursorPosition == -1) {
				newCursorPosition = m_data.Size();
			}
			m_displayCursorPos = newCursorPosition;
			return true;
		} else if (ewol::EVENT_INPUT_TYPE_DOUBLE == typeEvent) {
			// TODO : selected area ...
		} else if (ewol::EVENT_INPUT_TYPE_TRIPLE == typeEvent) {
			// TODO : select all the line
		}
	}
	return false;
}


/**
 * @brief Event on the keybord (if no shortcut has been detected before).
 * @param[in] type of the event (ewol::EVENT_KB_TYPE_DOWN or ewol::EVENT_KB_TYPE_UP)
 * @param[in] unicodeValue key pressed by the user
 * @return true if the event has been used
 * @return false if the event has not been used
 */
bool ewol::Entry::OnEventKb(eventKbType_te typeEvent, uniChar_t unicodeData)
{
	if( typeEvent == ewol::EVENT_KB_TYPE_DOWN) {
		//EWOL_DEBUG("Entry input data ... : \"" << unicodeData << "\" " );
		//return GenEventInputExternal(ewolEventEntryEnter, -1, -1);
		if(    '\n' == unicodeData
		    || '\r' == unicodeData) {
			GenerateEventId(ewolEventEntryEnter, m_data);
			return true;
		} else if (0x7F == unicodeData) {
			// SUPPR :
			if (m_data.Size() > 0 && m_displayCursorPos<m_data.Size()) {
				m_data.Remove(m_displayCursorPos, 1);
				m_displayCursorPos = etk_max(m_displayCursorPos, 0);
			}
		} else if (0x08 == unicodeData) {
			// DEL :
			if (m_data.Size() > 0 && m_displayCursorPos != 0) {
				m_data.Remove(m_displayCursorPos-1, 1);
				m_displayCursorPos--;
				m_displayCursorPos = etk_max(m_displayCursorPos, 0);
			}
		} else if(unicodeData >= 20) {
			m_data.Add(m_displayCursorPos, unicodeData);
			m_displayCursorPos++;
		}
		GenerateEventId(ewolEventEntryModify, m_data);
		MarkToRedraw();
		return true;
	}
	return false;
}


/**
 * @brief Event on the keyboard that is not a printable key (if no shortcut has been detected before).
 * @return true if the event has been used
 * @return false if the event has not been used
 */
bool ewol::Entry::OnEventKbMove(eventKbType_te typeEvent, eventKbMoveType_te moveTypeEvent)
{
	if(typeEvent == ewol::EVENT_KB_TYPE_DOWN) {
		switch (moveTypeEvent)
		{
			case EVENT_KB_MOVE_TYPE_LEFT:
				m_displayCursorPos--;
				break;
			case EVENT_KB_MOVE_TYPE_RIGHT:
				m_displayCursorPos++;
				break;
			case EVENT_KB_MOVE_TYPE_START:
				m_displayCursorPos = 0;
				break;
			case EVENT_KB_MOVE_TYPE_END:
				m_displayCursorPos = m_data.Size();
				break;
			default:
				return false;
		}
		m_displayCursorPos = etk_avg(0, m_displayCursorPos, m_data.Size());
		MarkToRedraw();
		return true;
	}
	return false;
}


void ewol::Entry::UpdateTextPosition(void)
{
	int32_t fontId = GetDefaultFontId();
	
	int32_t tmpSizeX = m_minSize.x;
	if (true==m_userFillX) {
		tmpSizeX = m_size.x;
	}
	int32_t tmpUserSize = tmpSizeX - 2*(m_borderSize + 2*m_paddingSize);
	int32_t totalWidth = ewol::GetWidth(fontId, m_data);
	if (totalWidth < tmpUserSize) {
		m_displayStartPosition = 0;
	} else {
		m_displayStartPosition = -totalWidth + tmpUserSize;
	}
}


void ewol::Entry::OnGetFocus(void)
{
	m_displayCursor = true;
	ewol::KeyboardShow();
	MarkToRedraw();
}

void ewol::Entry::OnLostFocus(void)
{
	m_displayCursor = false;
	ewol::KeyboardHide();
	MarkToRedraw();
}
