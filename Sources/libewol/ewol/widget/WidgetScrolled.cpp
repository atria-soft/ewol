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


//!< EObject name :
extern const char * const ewol::TYPE_EOBJECT_WIDGET_SCROOLED = "WidgetScrooled";

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool ewol::WidgetScrooled::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_SCROOLED << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == ewol::TYPE_EOBJECT_WIDGET_SCROOLED) {
		return true;
	} else {
		if(true == ewol::Widget::CheckObjectType(objectType)) {
			return true;
		}
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_SCROOLED << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const ewol::WidgetScrooled::GetObjectType(void)
{
	return ewol::TYPE_EOBJECT;
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
/**
 * @brief Event on an input of this Widget
 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
 * @param[in] pos Absolute position of the event
 * @return true the event is used
 * @return false the event is not used
 */
bool ewol::WidgetScrooled::OnEventInput(int32_t IdInput, ewol::eventInputType_te typeEvent, coord2D_ts pos)
{
	coord2D_ts relativePos = RelativePosition(pos);
	#ifdef __MODE__Touch
		if (1 == IdInput) {
			EWOL_VERBOSE("event 1  << " << (int32_t)typeEvent << "(" << x << "," << y << ")");
			if (ewol::EVENT_INPUT_TYPE_DOWN == typeEvent) {
				m_highSpeedMode = ewol::SCROLL_INIT;
				m_highSpeedStartPos.x = relativePos.x;
				m_highSpeedStartPos.y = relativePos.y;
				EWOL_VERBOSE("SCROOL ==> INIT");
				return true;
			} else if (ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
				m_highSpeedMode = ewol::SCROLL_DISABLE;
				EWOL_VERBOSE("SCROOL ==> DISABLE");
				MarkToReedraw();
				return true;
			} else if (ewol::SCROLL_INIT==m_highSpeedMode && ewol::EVENT_INPUT_TYPE_MOVE == typeEvent) {
				// wait that the cursor move more than 10 px to enable it :
				if(    abs(relativePos.x - m_highSpeedStartPos.x) > 10 
				    || abs(relativePos.y - m_highSpeedStartPos.y) > 10 ) {
					// the scrooling can start : 
					// select the direction :
					m_highSpeedMode = ewol::SCROLL_ENABLE;
					EWOL_VERBOSE("SCROOL ==> ENABLE");
					MarkToReedraw();
				}
				return true;
			} if (ewol::SCROLL_ENABLE==m_highSpeedMode && ewol::EVENT_INPUT_TYPE_MOVE == typeEvent) {
				//m_originScrooled.x = (int32_t)(m_maxSize.x * x / m_size.x);
				m_originScrooled.x -= relativePos.x - m_highSpeedStartPos.x;
				m_originScrooled.y -= relativePos.y - m_highSpeedStartPos.y;
				m_originScrooled.x = etk_max(m_originScrooled.x, 0);
				m_originScrooled.y = etk_max(m_originScrooled.y, 0);
				m_originScrooled.x = etk_min(m_originScrooled.x, m_maxSize.x);
				m_originScrooled.y = etk_min(m_originScrooled.y, m_maxSize.y);
				m_highSpeedStartPos.x = relativePos.x;
				m_highSpeedStartPos.y = relativePos.y;
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
				m_highSpeedStartPos.x = relativePos.x;
				m_highSpeedStartPos.y = relativePos.y;
				return true;
			} else if (ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
				m_highSpeedMode = ewol::SCROLL_DISABLE;
				MarkToReedraw();
				return true;
			} else if (ewol::SCROLL_INIT==m_highSpeedMode && ewol::EVENT_INPUT_TYPE_MOVE == typeEvent) {
				// wait that the cursor move more than 10 px to enable it :
				if(    abs(relativePos.x - m_highSpeedStartPos.x) > 10 
				    || abs(relativePos.y - m_highSpeedStartPos.y) > 10 ) {
					// the scrooling can start : 
					// select the direction :
					if (relativePos.x == m_highSpeedStartPos.x) {
						m_highSpeedMode = ewol::SCROLL_ENABLE_VERTICAL;
					} else if (relativePos.y == m_highSpeedStartPos.y) {
						m_highSpeedMode = ewol::SCROLL_ENABLE_HORIZONTAL;
					} else {
						etkFloat_t coef = (relativePos.y - m_highSpeedStartPos.y) / (relativePos.x - m_highSpeedStartPos.x);
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
				m_originScrooled.x = (int32_t)(m_maxSize.x * relativePos.x / m_size.x);
				MarkToReedraw();
				return true;
			} if (ewol::SCROLL_ENABLE_VERTICAL==m_highSpeedMode && ewol::EVENT_INPUT_TYPE_MOVE == typeEvent) {
				m_originScrooled.y = (int32_t)(m_maxSize.y * relativePos.y / m_size.y);
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

