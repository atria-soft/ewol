/**
 *******************************************************************************
 * @file ewol/widget/WidgetScrolled.cpp
 * @brief ewol Scrooled widget system (Sources)
 * @author Edouard DUPIN
 * @date 06/02/2012
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

#include <ewol/widget/WidgetScrolled.h>

#include <ewol/OObject.h>
#include <ewol/WidgetManager.h>



ewol::WidgetScrooled::WidgetScrooled(void)
{
	m_originScrooledX = 0;
	m_originScrooledY = 0;
	m_pixelScrolling = 20;
}

ewol::WidgetScrooled::~WidgetScrooled(void)
{
	
}


bool ewol::WidgetScrooled::OnEventInput(int32_t IdInput, ewol::eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y)
{
	
	if (4 == IdInput && ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
		EWOL_INFO("mouse-event GDK_SCROLL_UP");
		m_originScrooledY -= m_pixelScrolling;
		if (m_originScrooledY < 0) {
			m_originScrooledY = 0;
		}
		MarkToReedraw();
		return true;
	} else if (5 == IdInput && ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
		EWOL_INFO("mouse-event GDK_SCROLL_DOWN");
		m_originScrooledY += m_pixelScrolling;
		MarkToReedraw();
		return true;
	}
	return false;
}

