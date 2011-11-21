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
const char * ewolEventButtonEnter      = "ewol Button Enter";
const char * ewolEventButtonLeave      = "ewol Button Leave";



#undef __class__
#define __class__	"ewol::Button"


void ewol::Button::Init(void)
{
	m_textColorFg.red   = 0.0;
	m_textColorFg.green = 0.0;
	m_textColorFg.blue  = 0.0;
	m_textColorFg.alpha = 1.0;
	
	m_textColorBg.red   = 0.0;
	m_textColorBg.green = 0.0;
	m_textColorBg.blue  = 0.0;
	m_textColorBg.alpha = 0.25;
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
	
	
	
	/*
	ewol::OObject2DColored * tmpOObjects = new ewol::OObject2DColored;
	tmpOObjects->SetColor(0.0, 0.0, 0.0, 1.0);
	tmpOObjects->Rectangle( 2, 2, m_size.x-4, m_size.y-4);
	tmpOObjects->SetColor(1.0, 1.0, 1.0, 1.0);
	tmpOObjects->Rectangle( 3, 3, m_size.x-6, m_size.y-6);
	AddOObject(tmpOObjects, "BouttonDecoration");
	
	*/
	int32_t borderWidth = 2;
	ewol::OObject2DText * tmpText = new ewol::OObject2DText("", -1, m_textColorFg);
	
	int32_t fontId = GetDefaultFontId();
	int32_t fontHeight = ewol::GetHeight(fontId);
	int32_t fontWidth = ewol::GetWidth(fontId, m_label.c_str());
	int32_t posy = (m_size.y - fontHeight - 6)/2 + 3;
	int32_t posx = (m_size.x - fontWidth - 6)/2 + 3;
	tmpText->Text(posx+2, posy+3, m_label.c_str());
	
	ewol::OObject2DColored * tmpOObjects = new ewol::OObject2DColored;
	int32_t radius = fontHeight / 2;
	tmpOObjects->SetColor(m_textColorBg);
	tmpOObjects->Rectangle( ((m_size.x-fontWidth-10)/2)+radius, posy, fontWidth-radius, radius*2);
	tmpOObjects->SetColor(m_textColorFg);
	//EWOL_DEBUG("m_textColorFg=(" << m_textColorFg.red << " " << m_textColorFg.green << " " << m_textColorFg.blue << " " << m_textColorFg.alpha << ")" );
	tmpOObjects->Line( ((m_size.x-fontWidth-10)/2)+radius, posy, ((m_size.x-fontWidth-10)/2)+fontWidth, posy, borderWidth);
	tmpOObjects->Line( ((m_size.x-fontWidth-10)/2)+radius, posy+fontHeight, ((m_size.x-fontWidth-10)/2)+fontWidth, posy+fontHeight, borderWidth);
	posy += fontHeight/2;
	tmpOObjects->SetColor(m_textColorBg);
	tmpOObjects->DiscPart(((m_size.x-fontWidth-10)/2)+radius, posy, radius, 180, 360);
	tmpOObjects->SetColor(m_textColorFg);
	tmpOObjects->CirclePart(((m_size.x-fontWidth-10)/2)+radius, posy, radius, borderWidth, 180, 360);
	tmpOObjects->SetColor(m_textColorBg);
	tmpOObjects->DiscPart(((m_size.x-fontWidth-10)/2)+fontWidth, posy, radius, 0, 180);
	tmpOObjects->SetColor(m_textColorFg);
	tmpOObjects->CirclePart(((m_size.x-fontWidth-10)/2)+fontWidth, posy, radius, borderWidth, 0, 180);
	
	AddOObject(tmpOObjects, "BouttonDecoration");
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
	bool eventIsOK = false;
	//EWOL_DEBUG("Receive event : \"" << generateEventId << "\"");
	if(ewolEventButtonPressed == generateEventId) {
		EWOL_INFO("BT pressed ... " << m_label);
		eventIsOK = true;
	} else if(ewolEventButtonEnter == generateEventId) {
		OnRegenerateDisplay();
	}
	return eventIsOK;
}
