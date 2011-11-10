/**
 *******************************************************************************
 * @file ewolButton.cpp
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

#include <widget/ewolButton.h>

#include <ewolOObject.h>



const char * ewolEventButtonPressed    = "ewol Button Pressed";



#undef __class__
#define __class__	"ewol::Button"

ewol::Button::Button(void)
{
	m_label = "No Label";
}

ewol::Button::Button(etk::String newLabel)
{
	m_label = newLabel;
}


ewol::Button::~Button(void)
{
	
}

bool ewol::Button::CalculateMinSize(void)
{
	//SetMinSise(55, 24);
	int32_t fontId = GetDefaultFontId();
	int32_t minWidth = ewol::GetWidth(fontId, m_label.c_str());
	int32_t minHeight = ewol::GetHeight(fontId);
	m_minSize.x = 10+minWidth;
	m_minSize.y = 10+minHeight;
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
	
	ewol::OObject2DColored * tmpOObjects = new ewol::OObject2DColored;
	tmpOObjects->SetColor(0.0, 0.0, 0.0, 1.0);
	tmpOObjects->Rectangle( 2, 2, m_size.x-4, m_size.y-4);
	tmpOObjects->SetColor(1.0, 1.0, 1.0, 1.0);
	tmpOObjects->Rectangle( 3, 3, m_size.x-6, m_size.y-6);
	AddOObject(tmpOObjects, "BouttonDecoration");
	
	color_ts textColorFg;
	textColorFg.red = 0.0;
	textColorFg.green = 0.0;
	textColorFg.blue = 0.0;
	textColorFg.alpha = 1.0;
	
	
	ewol::OObject2DText * tmpText = new ewol::OObject2DText("", -1, textColorFg);
	
	int32_t fontId = GetDefaultFontId();
	int32_t fontHeight = ewol::GetHeight(fontId);
	int32_t fontWidth = ewol::GetWidth(fontId, m_label.c_str());
	int32_t posy = (m_size.y - fontHeight - 6)/2 + 3;
	int32_t posx = (m_size.x - fontWidth - 6)/2 + 3;
	tmpText->Text(posx, posy, m_label.c_str());
	AddOObject(tmpText, "BouttonText");
	
	
	// Regenerate the event Area:
	EventAreaRemoveAll();
	coord origin;
	coord size;
	origin.x = 3.0;
	origin.y = 3.0;
	size.x = m_size.x-6;
	size.y = m_size.y-6;
	AddEventArea(origin, size, FLAG_EVENT_INPUT_1 | FLAG_EVENT_INPUT_CLICKED_ALL, ewolEventButtonPressed);
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
	bool eventIsOK = false;
	//EWOL_DEBUG("Receive event : \"" << generateEventId << "\"");
	if(ewolEventButtonPressed == generateEventId) {
		EWOL_INFO("BT pressed ... " << m_label);
		eventIsOK = true;
	}
	
	return eventIsOK;
}
