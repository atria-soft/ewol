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
	m_originScrooled.x = 0;
	m_originScrooled.y = 0;
	m_pixelScrolling = 20;
	m_highSpeedMode = SCROLL_DISABLE;
}

ewol::WidgetScrooled::~WidgetScrooled(void)
{
	
}

void ewol::WidgetScrooled::OnRegenerateDisplay(void)
{
	#ifdef __MODE__Touch
	/*
		if(ewol::SCROLL_ENABLE==m_highSpeedMode) {
			ewol::OObject2DColored* myOObjectsColored = new ewol::OObject2DColored();
			myOObjectsColored->SetColor(1.0, 0.0, 0.0, 0.4);
			myOObjectsColored->Disc(m_highSpeedStartPos.x, m_highSpeedStartPos.y, 10);
			AddOObject(myOObjectsColored);
		}
	*/
	#else
	/*
		if(    ewol::SCROLL_ENABLE_HORIZONTAL==m_highSpeedMode
		    || ewol::SCROLL_ENABLE_VERTICAL  ==m_highSpeedMode) {
			ewol::OObject2DColored* myOObjectsColored = new ewol::OObject2DColored();
			myOObjectsColored->SetColor(1.0, 0.0, 0.0, 0.4);
			myOObjectsColored->Disc(m_highSpeedStartPos.x, m_highSpeedStartPos.y, 10);
			AddOObject(myOObjectsColored);
		}
	*/
	#endif
}

bool ewol::WidgetScrooled::OnEventInput(int32_t IdInput, ewol::eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y)
{
	#ifdef __MODE__Touch
		if (1 == IdInput) {
			EWOL_VERBOSE("event 1  << " << (int32_t)typeEvent << "(" << x << "," << y << ")");
			if (ewol::EVENT_INPUT_TYPE_DOWN == typeEvent) {
				m_highSpeedMode = ewol::SCROLL_INIT;
				m_highSpeedStartPos.x = x;
				m_highSpeedStartPos.y = y;
				EWOL_VERBOSE("SCROOL ==> INIT");
				return true;
			} else if (ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
				m_highSpeedMode = ewol::SCROLL_DISABLE;
				EWOL_VERBOSE("SCROOL ==> DISABLE");
				MarkToReedraw();
				return true;
			} else if (ewol::SCROLL_INIT==m_highSpeedMode && ewol::EVENT_INPUT_TYPE_MOVE == typeEvent) {
				// wait that the cursor move more than 10 px to enable it :
				if(    abs(x - m_highSpeedStartPos.x) > 10 
				    || abs(y - m_highSpeedStartPos.y) > 10 ) {
					// the scrooling can start : 
					// select the direction :
					m_highSpeedMode = ewol::SCROLL_ENABLE;
					EWOL_VERBOSE("SCROOL ==> ENABLE");
					MarkToReedraw();
				}
				return true;
			} if (ewol::SCROLL_ENABLE==m_highSpeedMode && ewol::EVENT_INPUT_TYPE_MOVE == typeEvent) {
				//m_originScrooled.x = (int32_t)(m_maxSize.x * x / m_size.x);
				m_originScrooled.x -= x - m_highSpeedStartPos.x;
				m_originScrooled.y -= y - m_highSpeedStartPos.y;
				m_originScrooled.x = etk_max(m_originScrooled.x, 0);
				m_originScrooled.y = etk_max(m_originScrooled.y, 0);
				m_originScrooled.x = etk_min(m_originScrooled.x, m_maxSize.x);
				m_originScrooled.y = etk_min(m_originScrooled.y, m_maxSize.y);
				m_highSpeedStartPos.x = x;
				m_highSpeedStartPos.y = y;
				EWOL_VERBOSE("SCROOL ==> MOVE (" << m_originScrooled.x << "," << m_originScrooled.y << ")");
				MarkToReedraw();
				return true;
			}
		} else if (ewol::SCROLL_DISABLE!=m_highSpeedMode && ewol::EVENT_INPUT_TYPE_LEAVE == typeEvent) {
			m_highSpeedMode = ewol::SCROLL_DISABLE;
			EWOL_VERBOSE("SCROOL ==> DISABLE");
			MarkToReedraw();
			return true;
		}
	#else
		if (4 == IdInput && ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
			//EWOL_INFO("mouse-event GDK_SCROLL_UP");
			m_originScrooled.y -= m_pixelScrolling;
			if (m_originScrooled.y < 0) {
				m_originScrooled.y = 0;
			}
			MarkToReedraw();
			return true;
		} else if (5 == IdInput && ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
			//EWOL_INFO("mouse-event GDK_SCROLL_DOWN");
			m_originScrooled.y += m_pixelScrolling;
			if (m_maxSize.y < m_originScrooled.y) {
				m_originScrooled.y = m_maxSize.y;
			}
			MarkToReedraw();
			return true;
		}else if (2 == IdInput) {
			if (ewol::EVENT_INPUT_TYPE_DOWN == typeEvent) {
				m_highSpeedMode = ewol::SCROLL_INIT;
				m_highSpeedStartPos.x = x;
				m_highSpeedStartPos.y = y;
				return true;
			} else if (ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
				m_highSpeedMode = ewol::SCROLL_DISABLE;
				MarkToReedraw();
				return true;
			} else if (ewol::SCROLL_INIT==m_highSpeedMode && ewol::EVENT_INPUT_TYPE_MOVE == typeEvent) {
				// wait that the cursor move more than 10 px to enable it :
				if(    abs(x - m_highSpeedStartPos.x) > 10 
				    || abs(y - m_highSpeedStartPos.y) > 10 ) {
					// the scrooling can start : 
					// select the direction :
					if (x == m_highSpeedStartPos.x) {
						m_highSpeedMode = ewol::SCROLL_ENABLE_VERTICAL;
					} else if (y == m_highSpeedStartPos.y) {
						m_highSpeedMode = ewol::SCROLL_ENABLE_HORIZONTAL;
					} else {
						etkFloat_t coef = (y - m_highSpeedStartPos.y) / (x - m_highSpeedStartPos.x);
						if (abs(coef) <= 1 ) {
							m_highSpeedMode = ewol::SCROLL_ENABLE_HORIZONTAL;
						} else {
							m_highSpeedMode = ewol::SCROLL_ENABLE_VERTICAL;
						}
					}
					if (m_highSpeedMode == ewol::SCROLL_ENABLE_HORIZONTAL) {
						m_highSpeedStartPos.x = m_originScrooled.x / m_maxSize.x * m_size.x;
					} else {
						m_highSpeedStartPos.y = m_originScrooled.y / m_maxSize.y * m_size.y;
					}
					MarkToReedraw();
				}
				return true;
			} if (ewol::SCROLL_ENABLE_HORIZONTAL==m_highSpeedMode && ewol::EVENT_INPUT_TYPE_MOVE == typeEvent) {
				m_originScrooled.x = (int32_t)(m_maxSize.x * x / m_size.x);
				MarkToReedraw();
				return true;
			} if (ewol::SCROLL_ENABLE_VERTICAL==m_highSpeedMode && ewol::EVENT_INPUT_TYPE_MOVE == typeEvent) {
				m_originScrooled.y = (int32_t)(m_maxSize.y * y / m_size.y);
				MarkToReedraw();
				return true;
			}
		} else if (ewol::SCROLL_DISABLE!=m_highSpeedMode && ewol::EVENT_INPUT_TYPE_LEAVE == typeEvent) {
			m_highSpeedMode = ewol::SCROLL_DISABLE;
			MarkToReedraw();
			return true;
		}
	#endif
	return false;
}

