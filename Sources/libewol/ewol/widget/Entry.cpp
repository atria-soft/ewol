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


#include <ewol/widget/Entry.h>
#include <ewol/OObject.h>
#include <ewol/WidgetManager.h>


const char * const ewolEventEntryClick      = "ewol Entry click";
const char * const ewolEventEntryEnter      = "ewol Entry Enter";



#undef __class__
#define __class__	"ewol::Entry"


void ewol::Entry::Init(void)
{
	AddEventId(ewolEventEntryClick);
	AddEventId(ewolEventEntryEnter);
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
}

ewol::Entry::Entry(etk::String newData)
{
	Init();
	SetValue(newData);
	/*
	m_data = newData;
	m_displayCursorPos = m_data.Size();
	UpdateTextPosition();
	*/
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
	return true;
}


void ewol::Entry::SetValue(etk::String newData)
{
	m_data = newData;
	UpdateTextPosition();
}

etk::String ewol::Entry::GetValue(void)
{
	return m_data;
}


void ewol::Entry::OnRegenerateDisplay(void)
{
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
	tmpText->Text(tmpTextOriginX, tmpTextOriginY, m_data.c_str() + m_displayStartPosition);

	ewol::OObject2DColored * tmpOObjects = new ewol::OObject2DColored;
	tmpOObjects->SetColor(m_textColorBg);
	tmpOObjects->Rectangle( tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY);
	tmpOObjects->SetColor(m_textColorFg);
	tmpOObjects->RectangleBorder( tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY, m_borderSize);
	if (true == m_displayCursor) {
		int32_t fontId = GetDefaultFontId();
		int32_t fontHeight = ewol::GetHeight(fontId);
		int32_t fontWidth = ewol::GetWidth(fontId, m_data.c_str() + m_displayStartPosition);
		int32_t XCursorPos = fontWidth + m_borderSize + 2*m_paddingSize;
		tmpOObjects->Line(XCursorPos, tmpTextOriginY, XCursorPos, tmpTextOriginY + fontHeight, 1);
	}

	AddOObject(tmpOObjects, "BouttonDecoration");

	AddOObject(tmpText, "BouttonText");

	// Regenerate the event Area:
	EventAreaRemoveAll();
	coord origin;
	coord size;
	origin.x = tmpOriginX;
	origin.y = tmpOriginY;
	size.x = tmpSizeX;
	size.y = tmpSizeY;
	AddEventArea(origin, size, FLAG_EVENT_INPUT_1 | FLAG_EVENT_INPUT_CLICKED_ALL, ewolEventEntryClick);
	AddEventArea(origin, size, FLAG_EVENT_INPUT_ENTER, ewolEventEntryEnter);
}


bool ewol::Entry::OnEventArea(const char * generateEventId, etkFloat_t x, etkFloat_t y)
{
	bool eventIsOK = false;
	//EWOL_DEBUG("Receive event : \"" << generateEventId << "\"");
	if(ewolEventEntryClick == generateEventId) {
		EWOL_INFO("Entry Clicked ... " << m_data);
		eventIsOK = true;
		ewol::widgetManager::FocusKeep(this);
	} else if(ewolEventEntryEnter == generateEventId) {
		//OnRegenerateDisplay();
	}
	return eventIsOK;
}


bool ewol::Entry::OnEventKb(eventKbType_te typeEvent, char UTF8_data[UTF8_MAX_SIZE])
{
	if(    UTF8_data != NULL
	    && typeEvent == ewol::EVENT_KB_TYPE_DOWN) {
		//EWOL_DEBUG("Entry input data ... : \"" << UTF8_data << "\" size=" << strlen(UTF8_data) << "  data=" << (int32_t)UTF8_data[0] );
		//return OnEventArea(ewolEventButtonPressed, -1, -1);
	if (0x7F == UTF8_data[0]) {
			if (m_data.Size() > 0) {
				// SUPPR
			}
		} else if (0x08 == UTF8_data[0]) {
			EWOL_DEBUG("The entry is an UTF8 string ...");
			EWOL_TODO("later...");
			if (m_data.Size() > 0) {
				// DEL : 
				m_data.Remove(m_data.Size()-1, 1);
			}
		} else if(UTF8_data[0] >= 20) {
			m_data += UTF8_data;
		}
		UpdateTextPosition();
		OnRegenerateDisplay();
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
		if (ewol::GetWidth(fontId, m_data.c_str()+(iii-1)) > tmpUserSize) {
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
	OnRegenerateDisplay();
}

void ewol::Entry::OnLostFocus(void)
{
	m_displayCursor = false;
	OnRegenerateDisplay();
}
