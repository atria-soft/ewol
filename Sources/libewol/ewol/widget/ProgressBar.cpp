/**
 *******************************************************************************
 * @file ewol/widget/ProgressBar.cpp
 * @brief ewol progress bar widget system (Sources)
 * @author Edouard DUPIN
 * @date 14/04/2012
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

#include <ewol/widget/ProgressBar.h>

#include <ewol/oObject/OObject.h>
#include <ewol/widget/WidgetManager.h>

#undef __class__
#define __class__	"Slider"

const int32_t dotRadius = 6;

ewol::ProgressBar::ProgressBar(void)
{
	m_value = 0.0;
	
	m_textColorFg = draw::color::black;
	
	m_textColorBgOn = 0x00FF00FF;
	
	m_textColorBgOff = draw::color::black;
	m_textColorBgOff.a = 0x3F;
	SetCanHaveFocus(true);
}

ewol::ProgressBar::~ProgressBar(void)
{
	
}


bool ewol::ProgressBar::CalculateMinSize(void)
{
	m_minSize.x = etk_max(m_userMinSize.x, 40);
	m_minSize.y = etk_max(m_userMinSize.y, dotRadius*2);
	MarkToRedraw();
	return true;
}


void ewol::ProgressBar::ValueSet(float val)
{
	m_value = etk_avg(0.0, val, 1.0);
	MarkToRedraw();
}


float ewol::ProgressBar::ValueGet(void)
{
	return m_value;
}


void ewol::ProgressBar::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		// clean the object list ...
		ClearOObjectList();
		
		ewol::OObject2DColored * tmpOObjects = new ewol::OObject2DColored;
		
		tmpOObjects->SetColor(m_textColorFg);
		
		int32_t tmpSizeX = m_size.x - 10;
		int32_t tmpSizeY = m_size.y - 10;
		int32_t tmpOriginX = 5;
		int32_t tmpOriginY = 5;
		tmpOObjects->SetColor(m_textColorBgOn);
		tmpOObjects->Rectangle( tmpOriginX, tmpOriginY, tmpSizeX*m_value, tmpSizeY);
		tmpOObjects->SetColor(m_textColorBgOff);
		tmpOObjects->Rectangle( tmpOriginX+tmpSizeX*m_value, tmpOriginY, tmpSizeX*(1.0-m_value), tmpSizeY);
		
		tmpOObjects->SetColor(m_textColorFg);
		tmpOObjects->RectangleBorder( tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY, 1);
		
		AddOObject(tmpOObjects);
	}
}



