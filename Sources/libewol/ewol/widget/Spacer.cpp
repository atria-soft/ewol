/**
 *******************************************************************************
 * @file ewol/widget/Spacer.cpp
 * @brief ewol Spacer widget system (Sources)
 * @author Edouard DUPIN
 * @date 29/12/2011
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

#include <ewol/widget/Spacer.h>

#include <ewol/OObject.h>
#include <ewol/WidgetManager.h>


#undef __class__
#define __class__	"Spacer"


ewol::Spacer::Spacer(void)
{
	m_localSize = 10;
	SetCanHaveFocus(false);
	m_color = 0x00000000;
}

ewol::Spacer::~Spacer(void)
{
	
}


bool ewol::Spacer::CalculateMinSize(void)
{
	m_minSize.x = m_localSize;
	m_minSize.y = m_localSize;
	return true;
}


void ewol::Spacer::SetSize(float size)
{
	m_localSize = size;
	MarkToRedraw();
}

#define BORDER_SIZE_TMP         (4)
void ewol::Spacer::OnRegenerateDisplay(void)
{
	if (false == NeedRedraw()) {
		return;
	}
	// generate a white background and take gray on other surfaces
	ClearOObjectList();
	if (m_color.alpha == 0) {
		return;
	}
	ewol::OObject2DColored * BGOObjects = new ewol::OObject2DColored();
	if (NULL == BGOObjects) {
		return;
	}
	AddOObject(BGOObjects);
	
	BGOObjects->SetColor(m_color);
	BGOObjects->Rectangle(0, 0, m_size.x, m_size.y);
}