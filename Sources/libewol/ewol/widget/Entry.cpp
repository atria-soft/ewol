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
	m_data = "";
	Init();
}

ewol::Entry::Entry(etk::String newData)
{
	m_data = newData;
	Init();
}


ewol::Entry::~Entry(void)
{
	
}

bool ewol::Entry::CalculateMinSize(void)
{
	int32_t fontId = GetDefaultFontId();
	int32_t minWidth = ewol::GetWidth(fontId, m_data.c_str());
	int32_t minHeight = ewol::GetHeight(fontId);
	m_minSize.x = 16+minWidth;
	m_minSize.y = 16+minHeight;
	return true;
}


void ewol::Entry::SetValue(etk::String newData)
{
	m_data = newData;
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
	int32_t borderSize = 2;
	int32_t paddingSize = 3;
	
	int32_t tmpSizeX = m_size.x;
	int32_t tmpSizeY = m_minSize.y;
	int32_t tmpOriginX = 0;
	int32_t tmpOriginY = (m_size.y - tmpSizeY) / 2;
	// no change for the text orogin : 
	int32_t tmpTextOriginX = borderSize + 2*paddingSize;
	int32_t tmpTextOriginY = (m_size.y - tmpSizeY) / 2 + borderSize + 2*paddingSize;
	
	// m_userFillX ==> in every case start at the left ...
	if (true==m_userFillY) {
		tmpSizeY = m_size.y;
		tmpOriginY = 0;
	}
	tmpOriginX += paddingSize;
	tmpOriginY += paddingSize;
	tmpSizeX -= 2*paddingSize;
	tmpSizeY -= 2*paddingSize;

	ewol::OObject2DText * tmpText = new ewol::OObject2DText("", -1, m_textColorFg);
	tmpText->Text(tmpTextOriginX, tmpTextOriginY, m_data.c_str());

	ewol::OObject2DColored * tmpOObjects = new ewol::OObject2DColored;
	tmpOObjects->SetColor(m_textColorBg);
	tmpOObjects->Rectangle( tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY);
	tmpOObjects->SetColor(m_textColorFg);
	tmpOObjects->RectangleBorder( tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY, borderSize);

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
		EWOL_DEBUG("Entry input data ... : \"" << UTF8_data << "\" size=" << strlen(UTF8_data) << "  lkjlkj=" << (int32_t)UTF8_data[0] );
		//return OnEventArea(ewolEventButtonPressed, -1, -1);
	if (0x7F == UTF8_data[0]) {
			if (m_data.Size() > 0) {
				// SUPPR
			}
		} else if (0x08 == UTF8_data[0]) {
			EWOL_DEBUG("Entrsgfqsfgqsdfgkljshdlfkjghsdlkfjghsldkjfhglkdjf");
			if (m_data.Size() > 0) {
				// DEL : 
				m_data.Remove(m_data.Size()-1, 1);
			}
		} else if(UTF8_data[0] >= 20) {
			m_data += UTF8_data;
		}
		OnRegenerateDisplay();
		return true;
	}
	return false;
}


