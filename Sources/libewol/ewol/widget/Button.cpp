/**
 *******************************************************************************
 * @file ewol/widget/Button.cpp
 * @brief ewol Button widget system (Sources)
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

#include <ewol/widget/Button.h>

#include <ewol/OObject.h>
#include <ewol/WidgetManager.h>


const char * const ewolEventButtonPressed    = "ewol Button Pressed";
const char * const ewolEventButtonEnter      = "ewol Button Enter";
const char * const ewolEventButtonLeave      = "ewol Button Leave";



#undef __class__
#define __class__	"ewol::Button"


void ewol::Button::Init(void)
{
	AddEventId(ewolEventButtonPressed);
	AddEventId(ewolEventButtonEnter);
	AddEventId(ewolEventButtonLeave);
	
	m_textColorFg.red   = 0.0;
	m_textColorFg.green = 0.0;
	m_textColorFg.blue  = 0.0;
	m_textColorFg.alpha = 1.0;
	
	m_textColorBg.red   = 0.0;
	m_textColorBg.green = 0.0;
	m_textColorBg.blue  = 0.0;
	m_textColorBg.alpha = 0.25;
	SetCanHaveFocus(true);
}

ewol::Button::Button(void)
{
	m_label = "No Label";
	Init();
}

ewol::Button::Button(etk::String newLabel)
{
	m_label = newLabel;
	Init();
}


ewol::Button::~Button(void)
{
	
}

bool ewol::Button::CalculateMinSize(void)
{
	int32_t fontId = GetDefaultFontId();
	int32_t minWidth = ewol::GetWidth(fontId, m_label.c_str());
	int32_t minHeight = ewol::GetHeight(fontId);
	m_minSize.x = 16+minWidth;
	m_minSize.y = 16+minHeight;
	return true;
}


void ewol::Button::SetLabel(etk::String newLabel)
{
	m_label = newLabel;
}

void ewol::Button::SetValue(bool val)
{
	
}

bool ewol::Button::GetValue(void)
{
	return false;
}


void ewol::Button::OnRegenerateDisplay(void)
{
	// clean the object list ...
	ClearOObjectList();

	// TODO later : Add this in the basic element of the widget ...
	int32_t borderSize = 2;
	int32_t paddingSize = 3;
	
	int32_t tmpSizeX = m_minSize.x;
	int32_t tmpSizeY = m_minSize.y;
	int32_t tmpOriginX = (m_size.x - tmpSizeX) / 2;
	int32_t tmpOriginY = (m_size.y - tmpSizeY) / 2;
	// no change for the text orogin : 
	int32_t tmpTextOriginX = (m_size.x - tmpSizeX) / 2 + borderSize + 2*paddingSize;
	int32_t tmpTextOriginY = (m_size.y - tmpSizeY) / 2 + borderSize + 2*paddingSize;
	
	if (true==m_userFillX) {
		tmpSizeX = m_size.x;
		tmpOriginX = 0;
	}
	if (true==m_userFillY) {
		tmpSizeY = m_size.y;
		tmpOriginY = 0;
	}
	tmpOriginX += paddingSize;
	tmpOriginY += paddingSize;
	tmpSizeX -= 2*paddingSize;
	tmpSizeY -= 2*paddingSize;

	ewol::OObject2DText * tmpText = new ewol::OObject2DText("", -1, m_textColorFg);
	/*
	int32_t fontId = GetDefaultFontId();
	int32_t fontHeight = ewol::GetHeight(fontId);
	int32_t fontWidth = ewol::GetWidth(fontId, m_label.c_str());
	*/
	tmpText->Text(tmpTextOriginX, tmpTextOriginY, m_label.c_str(), m_size.x - borderSize - 2*paddingSize);

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
	AddEventArea(origin, size, FLAG_EVENT_INPUT_1 | FLAG_EVENT_INPUT_CLICKED_ALL, ewolEventButtonPressed);
	AddEventArea(origin, size, FLAG_EVENT_INPUT_ENTER, ewolEventButtonEnter);
	AddEventArea(origin, size, FLAG_EVENT_INPUT_LEAVE, ewolEventButtonLeave);
}

/*
bool ewol::Button::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y)
{
	EWOL_DEBUG("Event on BT ...");
	return true;
}
*/

bool ewol::Button::OnEventArea(const char * generateEventId, etkFloat_t x, etkFloat_t y)
{
	//bool eventIsOK = false;
	//EWOL_DEBUG("Receive event : \"" << generateEventId << "\"");
	if(ewolEventButtonPressed == generateEventId) {
		EWOL_INFO("BT pressed ... " << m_label);
		//eventIsOK = true;
		ewol::widgetManager::FocusKeep(this);
	} else if(ewolEventButtonEnter == generateEventId) {
		OnRegenerateDisplay();
	}
	//return eventIsOK;
	// in every case this not stop the propagation of the event
	return false;
	// if overwrited... you can ...
}


bool ewol::Button::OnEventKb(eventKbType_te typeEvent, char UTF8_data[UTF8_MAX_SIZE])
{
	//EWOL_DEBUG("BT PRESSED : \"" << UTF8_data << "\" size=" << strlen(UTF8_data));
	if(    UTF8_data != NULL
	    && typeEvent == ewol::EVENT_KB_TYPE_DOWN
	    && UTF8_data[0] == '\r') {
		return OnEventArea(ewolEventButtonPressed, -1, -1);
	}
	return false;
}


