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


extern const char * const ewolEventLabelPressed    = "ewol Label Pressed";



#undef __class__
#define __class__	"Label"


void ewol::Label::Init(void)
{
	AddEventId(ewolEventLabelPressed);
	m_textColorFg.red   = 0.0;
	m_textColorFg.green = 0.0;
	m_textColorFg.blue  = 0.0;
	m_textColorFg.alpha = 1.0;
	SetCanHaveFocus(false);
}

ewol::Label::Label(void)
{
	m_label = "---";
	Init();
}

ewol::Label::Label(etk::UString newLabel)
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
	int32_t minWidth = ewol::GetWidth(fontId, m_label);
	int32_t minHeight = ewol::GetHeight(fontId);
	m_minSize.x = 3+minWidth;
	m_minSize.y = 3+minHeight;
	MarkToReedraw();
	return true;
}


void ewol::Label::SetLabel(etk::UString newLabel)
{
	m_label = newLabel;
	MarkToReedraw();
}


void ewol::Label::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
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
		
		coord2D_ts textPos;
		textPos.x = tmpOriginX;
		textPos.y = tmpOriginY;
		clipping_ts drawClipping;
		drawClipping.x = paddingSize;
		drawClipping.y = paddingSize;
		drawClipping.w = m_size.x - 2*paddingSize;
		drawClipping.h = m_size.y - 2*paddingSize;
		tmpText->Text(textPos, drawClipping, m_label);
		
		AddOObject(tmpText);
	}
}


bool ewol::Label::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, eventPosition_ts pos)
{
	//EWOL_DEBUG("Event on Label ...");
	if (1 == IdInput) {
		if (ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent) {
			// nothing to do ...
			GenerateEventId(ewolEventLabelPressed);
			return true;
		}
	}
	return false;
}


