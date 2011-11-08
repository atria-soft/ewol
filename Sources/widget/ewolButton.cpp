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
	tmpOObjects->Rectangle( 2, 2, m_size.x-4, m_size.y-4,  0.0, 0.0, 0.0, 1.0);
	tmpOObjects->Rectangle( 3, 3, m_size.x-6, m_size.y-6,  1.0, 1.0, 1.0, 1.0);
	AddOObject(tmpOObjects, "BouttonDecoration");
	
	color_ts textColorFg;
	textColorFg.red = 0.0;
	textColorFg.green = 0.0;
	textColorFg.blue = 0.0;
	textColorFg.alpha = 1.0;
	
	int32_t fontId = GetDefaultFontId();
	int32_t fontHeight = ewol::GetHeight(fontId);
	int32_t pos = (m_size.y - fontHeight)/2;
	ewol::OObject2DText * tmpText = new ewol::OObject2DText(5, pos, "", -1, FONT_MODE_BOLD, textColorFg, m_label.c_str());
	
	AddOObject(tmpText, "BouttonText");
}
