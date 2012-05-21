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
#include <ewol/importgl.h>
#include <ewol/ewol.h>



ewol::WidgetScrooled::WidgetScrooled(void)
{
	m_originScrooled.x = 0;
	m_originScrooled.y = 0;
	m_pixelScrolling = 20;
	m_highSpeedMode = SCROLL_DISABLE;
	m_scroollingMode = SCROLL_MODE_NORMAL;
	m_zoom = 1.0;
	m_highSpeedButton = -1;
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

#define SCROLL_BAR_SPACE      (15)

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
	ClearOObjectList();
	ewol::OObject2DColored* myOObjectsColored = NULL;
	if(m_size.y < m_maxSize.y || m_size.x < m_maxSize.x) {
		myOObjectsColored = new ewol::OObject2DColored();
		myOObjectsColored->SetColor(1.0, 0.0, 0.0, 0.6);
	}
	if(m_size.y < m_maxSize.y) {
		//myOObjectsColored->Line(m_size.x-SCROLL_BAR_SPACE, SCROLL_BAR_SPACE, m_size.x-SCROLL_BAR_SPACE, m_size.y-SCROLL_BAR_SPACE, 1);
		myOObjectsColored->Line(m_size.x-(SCROLL_BAR_SPACE/2), SCROLL_BAR_SPACE, m_size.x-(SCROLL_BAR_SPACE/2), m_size.y-SCROLL_BAR_SPACE, 1);
		etkFloat_t lenScrollBar = m_size.y*(m_size.y-SCROLL_BAR_SPACE*2) / m_maxSize.y;
		lenScrollBar = etk_avg(10, lenScrollBar, (m_size.y-SCROLL_BAR_SPACE*2));
		etkFloat_t originScrollBar = m_originScrooled.y / m_maxSize.y;
		originScrollBar = etk_avg(0.0, originScrollBar, 1.0);
		originScrollBar *= (m_size.y-SCROLL_BAR_SPACE*2-lenScrollBar);
		originScrollBar += 15;
		myOObjectsColored->Rectangle(m_size.x-SCROLL_BAR_SPACE, originScrollBar, SCROLL_BAR_SPACE, lenScrollBar);
	}
	if(m_size.x < m_maxSize.x) {
		//myOObjectsColored->Line(SCROLL_BAR_SPACE, m_size.y-SCROLL_BAR_SPACE, m_size.x-SCROLL_BAR_SPACE, m_size.y-SCROLL_BAR_SPACE, 1);
		myOObjectsColored->Line(SCROLL_BAR_SPACE, m_size.y-(SCROLL_BAR_SPACE/2), m_size.x-SCROLL_BAR_SPACE, m_size.y-(SCROLL_BAR_SPACE/2), 1);
		etkFloat_t lenScrollBar = m_size.x*(m_size.x-SCROLL_BAR_SPACE*2) / m_maxSize.x;
		lenScrollBar = etk_avg(10, lenScrollBar, (m_size.x-SCROLL_BAR_SPACE*2));
		etkFloat_t originScrollBar = m_originScrooled.x / m_maxSize.x;
		originScrollBar = etk_avg(0.0, originScrollBar, 1.0);
		originScrollBar *= (m_size.x-SCROLL_BAR_SPACE*2-lenScrollBar);
		originScrollBar += 15;
		myOObjectsColored->Rectangle(originScrollBar, m_size.y-SCROLL_BAR_SPACE, lenScrollBar, SCROLL_BAR_SPACE);
	}
	if (NULL!=myOObjectsColored) {
		AddOObject(myOObjectsColored);
	}
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
	if (SCROLL_MODE_NORMAL == m_scroollingMode) {
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
		if (1 == IdInput && ewol::EVENT_INPUT_TYPE_DOWN == typeEvent) {
			// check if selected the scrolling position whth the scrolling bar ...
			if (relativePos.x >= (m_size.x-SCROLL_BAR_SPACE)) {
				if(m_size.y < m_maxSize.y) {
					m_highSpeedMode = ewol::SCROLL_ENABLE_VERTICAL;
					m_highSpeedStartPos.x = relativePos.x;
					m_highSpeedStartPos.y = m_originScrooled.y / m_maxSize.y * (m_size.y-SCROLL_BAR_SPACE*2);
					m_highSpeedButton = 1;
					// force direct scrolling in this case
					m_originScrooled.y = (int32_t)(m_maxSize.y * (relativePos.y-SCROLL_BAR_SPACE) / (m_size.y-SCROLL_BAR_SPACE*2));
					m_originScrooled.y = etk_avg(0, m_originScrooled.y, m_maxSize.y);
					MarkToReedraw();
					return true;
				}
			} else if (relativePos.y >= (m_size.y-SCROLL_BAR_SPACE)) {
				if(m_size.x < m_maxSize.x) {
					m_highSpeedMode = ewol::SCROLL_ENABLE_HORIZONTAL;
					m_highSpeedStartPos.x = m_originScrooled.x / m_maxSize.x * (m_size.x-SCROLL_BAR_SPACE*2);
					m_highSpeedStartPos.y = relativePos.y;
					m_highSpeedButton = 1;
					// force direct scrolling in this case
					m_originScrooled.x = (int32_t)(m_maxSize.x * (relativePos.x-SCROLL_BAR_SPACE) / (m_size.x-SCROLL_BAR_SPACE*2));
					m_originScrooled.x = etk_avg(0, m_originScrooled.x, m_maxSize.x);
					MarkToReedraw();
					return true;
				}
			}
			return false;
		} else if (4 == IdInput && ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
			m_originScrooled.y -= m_pixelScrolling;
			m_originScrooled.y = etk_avg(0, m_originScrooled.y, m_maxSize.y);
			MarkToReedraw();
			return true;
		} else if (5 == IdInput && ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
			m_originScrooled.y += m_pixelScrolling;
			m_originScrooled.y = etk_avg(0, m_originScrooled.y, m_maxSize.y);
			MarkToReedraw();
			return true;
		}else if (2 == IdInput) {
			if (ewol::EVENT_INPUT_TYPE_DOWN == typeEvent) {
				m_highSpeedMode = ewol::SCROLL_INIT;
				m_highSpeedStartPos.x = relativePos.x;
				m_highSpeedStartPos.y = relativePos.y;
				m_highSpeedButton = 2;
				return true;
			}
		} else if (ewol::SCROLL_DISABLE!=m_highSpeedMode && ewol::EVENT_INPUT_TYPE_LEAVE == typeEvent) {
			m_highSpeedMode = ewol::SCROLL_DISABLE;
			MarkToReedraw();
			return true;
		}
		if (IdInput==m_highSpeedButton && ewol::SCROLL_DISABLE!=m_highSpeedMode) {
			if (ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
				m_highSpeedMode = ewol::SCROLL_GREP_END_EVENT;
				MarkToReedraw();
				return true;
			} else if (ewol::SCROLL_GREP_END_EVENT == m_highSpeedMode) {
				if (ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent) {
					m_highSpeedMode = ewol::SCROLL_DISABLE;
					m_highSpeedButton = -1;
					MarkToReedraw();
				} else if (ewol::EVENT_INPUT_TYPE_DOUBLE == typeEvent) {
					m_highSpeedMode = ewol::SCROLL_DISABLE;
					m_highSpeedButton = -1;
					MarkToReedraw();
				} else if (ewol::EVENT_INPUT_TYPE_TRIPLE == typeEvent) {
					m_highSpeedMode = ewol::SCROLL_DISABLE;
					m_highSpeedButton = -1;
					MarkToReedraw();
				}
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
						m_highSpeedStartPos.x = m_originScrooled.x / m_maxSize.x * (m_size.x-SCROLL_BAR_SPACE*2);
					} else {
						m_highSpeedStartPos.y = m_originScrooled.y / m_maxSize.y * (m_size.y-SCROLL_BAR_SPACE*2);
					}
					MarkToReedraw();
				}
				m_originScrooled.y = etk_avg(0, m_originScrooled.y, m_maxSize.y);
				return true;
			} if (ewol::SCROLL_ENABLE_HORIZONTAL==m_highSpeedMode && ewol::EVENT_INPUT_TYPE_MOVE == typeEvent) {
				m_originScrooled.x = (int32_t)(m_maxSize.x * (relativePos.x-SCROLL_BAR_SPACE) / (m_size.x-SCROLL_BAR_SPACE*2));
				m_originScrooled.x = etk_avg(0, m_originScrooled.x, m_maxSize.x);
				MarkToReedraw();
				return true;
			} if (ewol::SCROLL_ENABLE_VERTICAL==m_highSpeedMode && ewol::EVENT_INPUT_TYPE_MOVE == typeEvent) {
				m_originScrooled.y = (int32_t)(m_maxSize.y * (relativePos.y-SCROLL_BAR_SPACE) / (m_size.y-SCROLL_BAR_SPACE*2));
				m_originScrooled.y = etk_avg(0, m_originScrooled.y, m_maxSize.y);
				MarkToReedraw();
				return true;
			}
		}
	#endif
	} else if (SCROLL_MODE_CENTER == m_scroollingMode) {
	#ifdef __MODE__Touch
		// TODO ...
	#else
		etkFloat_t tmp1=ewol::GetCurrentHeight() / m_maxSize.y;
		etkFloat_t tmp2=ewol::GetCurrentWidth() / m_maxSize.x;
		//EWOL_INFO(" elements Zoom : " << tmp1 << " " << tmp2);
		tmp1 = etk_min(tmp1, tmp2);
		if (4 == IdInput && ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
			m_zoom -= 0.1;
			if (tmp1 < 1.0) {
				m_zoom = etk_max(tmp1, m_zoom);
			} else {
				m_zoom = etk_max(1.0, m_zoom);
			}
			MarkToReedraw();
			return true;
		} else if (5 == IdInput && ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
			m_zoom += 0.1;
			if (tmp1 > 1.0) {
				m_zoom = etk_min(tmp1, m_zoom);
			} else {
				m_zoom = etk_min(1.0, m_zoom);
			}
			MarkToReedraw();
			return true;
		}
	#endif
	} else {
		EWOL_ERROR("Scrolling mode unknow ... " << m_scroollingMode );
	}
	return false;
}

void ewol::WidgetScrooled::AddOObject(ewol::OObject* newObject, int32_t pos)
{
	if (NULL == newObject) {
		EWOL_ERROR("Try to add an empty object in the Widget generic display system");
		return;
	}
	if (pos < 0 || pos >= m_listOObject[m_currentCreateId].Size() ) {
		m_listOObject[m_currentCreateId].PushBack(newObject);
	} else {
		m_listOObject[m_currentCreateId].Insert(pos, newObject);
	}
	m_needFlipFlop = true;
}


void ewol::WidgetScrooled::ClearOObjectList(void)
{
	for (int32_t iii=0; iii<m_listOObject[m_currentCreateId].Size(); iii++) {
		delete(m_listOObject[m_currentCreateId][iii]);
		m_listOObject[m_currentCreateId][iii] = NULL;
	}
	m_listOObject[m_currentCreateId].Clear();
}

void ewol::WidgetScrooled::OnDraw(void)
{
	for (int32_t iii=0; iii<m_listOObject[m_currentDrawId].Size(); iii++) {
		if (NULL != m_listOObject[m_currentDrawId][iii]) {
			m_listOObject[m_currentDrawId][iii]->Draw();
		}
	}
}

/**
 * @brief extern interface to request a draw ...  (called by the drawing thread [Android, X11, ...])
 * This function generate a clipping with the viewport openGL system. Like this a widget draw can not draw over an other widget
 * @note This function is virtual for the scrolled widget, and the more complicated OpenGl widget
 * @param ---
 * @return ---
 */
void ewol::WidgetScrooled::GenDraw(void)
{
	if (SCROLL_MODE_CENTER == m_scroollingMode) {
		glPushMatrix();
		// here we invert the reference of the standard OpenGl view because the reference in the common display is Top left and not buttom left
		glViewport(                                       m_origin.x,
		            ewol::GetCurrentHeight() - m_size.y - m_origin.y,
		            m_size.x,
		            m_size.y);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrthoEwol(-m_size.x/2, m_size.x/2, m_size.y/2, -m_size.y/2, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glScalef(m_zoom, m_zoom, 1.0);
		glTranslatef(-m_maxSize.x/2, -m_maxSize.y/2, -1.0);
		
		// Call the widget drawing methode
		OnDraw();
		glPopMatrix();
	} else {
		ewol::Widget::GenDraw();
	}
	
}


/**
 * @brief Request a specific position for the scrolling of the current windows.
 * @param[in] borderWidth Size of the border that requested the element might not to be
 * @param[in] currentPosition Position that is requested to view
 * @param[in] center True if the position might be at the center of the widget
 * @return ---
 */
void ewol::WidgetScrooled::SetScrollingPositionDynamic(coord2D_ts borderWidth, coord2D_ts currentPosition, bool center)
{
	if (true == center) {
		borderWidth.x = m_size.x / 2 - borderWidth.x;
		borderWidth.y = m_size.y / 2 - borderWidth.y;
	}
	// check scrooling in X
	if( currentPosition.x < (m_originScrooled.x+borderWidth.x) ) {
		m_originScrooled.x = currentPosition.x - borderWidth.x;
		m_originScrooled.x = etk_max(0.0, m_originScrooled.x);
	} else if( currentPosition.x > (m_originScrooled.x+m_size.x-2*borderWidth.x) ) {
		m_originScrooled.x = currentPosition.x - m_size.x + 2*borderWidth.x;
		m_originScrooled.x = etk_max(0.0, m_originScrooled.x);
	}
	// check scrooling in Y
	if( currentPosition.y < (m_originScrooled.y+borderWidth.y) ) {
		m_originScrooled.y = currentPosition.y - borderWidth.y;
		m_originScrooled.y = etk_max(0.0, m_originScrooled.y);
	} else if( currentPosition.y > (m_originScrooled.y+m_size.y-2*borderWidth.y) ) {
		m_originScrooled.y = currentPosition.y - m_size.y + 2*borderWidth.y;
		m_originScrooled.y = etk_max(0.0, m_originScrooled.y);
	}
}

