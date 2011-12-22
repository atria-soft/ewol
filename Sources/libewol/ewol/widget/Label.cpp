/**
 *******************************************************************************
 * @file ewol/widget/Label.cpp
 * @brief ewol Label widget system (Sources)
 * @author Edouard DUPIN
 * @date 22/12/2011
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

#include <ewol/widget/Label.h>

#include <ewol/OObject.h>
#include <ewol/WidgetManager.h>


const char * const ewolEventLabelPressed    = "ewol Label Pressed";



#undef __class__
#define __class__	"ewol::Label"


void ewol::Label::Init(void)
{
	m_textColorFg.red   = 0.0;
	m_textColorFg.green = 0.0;
	m_textColorFg.blue  = 0.0;
	m_textColorFg.alpha = 1.0;
	SetCanHaveFocus(true);
}

ewol::Label::Label(void)
{
	m_label = "---";
	Init();
}

ewol::Label::Label(etk::String newLabel)
{
	m_label = newLabel;
	Init();
}


ewol::Label::~Label(void)
{
	
}

bool ewol::Label::CalculateMinSize(void)
{
	int32_t fontId = GetDefaultFontId();
	int32_t minWidth = ewol::GetWidth(fontId, m_label.c_str());
	int32_t minHeight = ewol::GetHeight(fontId);
	m_minSize.x = 3+minWidth;
	m_minSize.y = 3+minHeight;
	return true;
}


void ewol::Label::SetLabel(etk::String newLabel)
{
	m_label = newLabel;
}


void ewol::Label::OnRegenerateDisplay(void)
{
	// clean the object list ...
	ClearOObjectList();
	
	int32_t paddingSize = 3;
	
	int32_t tmpOriginX = 0;
	int32_t tmpOriginY = 0;
	
	if (true==m_userFillX) {
		tmpOriginX = (m_size.x - m_minSize.x) / 2;
	}
	if (true==m_userFillY) {
		tmpOriginY = (m_size.y - m_minSize.y) / 2;
	}
	tmpOriginX += paddingSize;
	tmpOriginY += paddingSize;

	ewol::OObject2DText * tmpText = new ewol::OObject2DText("", -1, m_textColorFg);
	tmpText->Text(tmpOriginX, tmpOriginY, m_label.c_str());

	AddOObject(tmpText, "LabelText");

	// Regenerate the event Area:
	EventAreaRemoveAll();
	coord origin;
	coord size;
	origin.x = tmpOriginX;
	origin.y = tmpOriginY;
	size.x = m_minSize.x;
	size.y = m_minSize.y;
	AddEventArea(origin, size, FLAG_EVENT_INPUT_1 | FLAG_EVENT_INPUT_CLICKED_ALL, ewolEventLabelPressed);
}

bool ewol::Label::OnEventArea(const char * generateEventId, etkFloat_t x, etkFloat_t y)
{
	bool eventIsOK = false;
	//EWOL_DEBUG("Receive event : \"" << generateEventId << "\"");
	if(ewolEventLabelPressed == generateEventId) {
		EWOL_INFO("LBL pressed ... " << m_label);
		//ewol::widgetManager::FocusKeep(this);
	}
	return eventIsOK;
}


