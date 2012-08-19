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
#include <ewol/openGl.h>
#include <ewol/ewol.h>



ewol::WidgetScrooled::WidgetScrooled(void)
{
	m_originScrooled.x = 0;
	m_originScrooled.y = 0;
	m_pixelScrolling = 20;
	m_highSpeedMode = SCROLL_DISABLE;
	m_scroollingMode = SCROLL_MODE_NORMAL;
	m_highSpeedType = ewol::INPUT_TYPE_UNKNOW;
	m_zoom = 1.0;
	m_highSpeedButton = -1;
	m_limitScrolling = 0.5;
}

ewol::WidgetScrooled::~WidgetScrooled(void)
{
	
}


#define SCROLL_BAR_SPACE      (15)

void ewol::WidgetScrooled::OnRegenerateDisplay(void)
{
	ClearOObjectList();
	if (SCROLL_MODE_GAME == m_scroollingMode) {
		
	} else {
		ewol::OObject2DColored* myOObjectsColored = NULL;
		if(m_size.y < m_maxSize.y || m_size.x < m_maxSize.x) {
			myOObjectsColored = new ewol::OObject2DColored();
			myOObjectsColored->SetColor(1.0, 0.0, 0.0, 0.6);
		}
		if(m_size.y < m_maxSize.y) {
			//myOObjectsColored->Line(m_size.x-SCROLL_BAR_SPACE, SCROLL_BAR_SPACE, m_size.x-SCROLL_BAR_SPACE, m_size.y-SCROLL_BAR_SPACE, 1);
			myOObjectsColored->Line(m_size.x-(SCROLL_BAR_SPACE/2), SCROLL_BAR_SPACE, m_size.x-(SCROLL_BAR_SPACE/2), m_size.y, 1);
			float lenScrollBar = m_size.y*(m_size.y-SCROLL_BAR_SPACE) / m_maxSize.y;
			lenScrollBar = etk_avg(10, lenScrollBar, (m_size.y-SCROLL_BAR_SPACE));
			float originScrollBar = m_originScrooled.y / (m_maxSize.y-m_size.y*m_limitScrolling);
			originScrollBar = etk_avg(0.0, originScrollBar, 1.0);
			originScrollBar *= (m_size.y-SCROLL_BAR_SPACE-lenScrollBar);
			myOObjectsColored->Rectangle(m_size.x-SCROLL_BAR_SPACE, m_size.y - originScrollBar - lenScrollBar, SCROLL_BAR_SPACE, lenScrollBar);
		}
		if(m_size.x < m_maxSize.x) {
			//myOObjectsColored->Line(SCROLL_BAR_SPACE, m_size.y-SCROLL_BAR_SPACE, m_size.x-SCROLL_BAR_SPACE, m_size.y-SCROLL_BAR_SPACE, 1);
			myOObjectsColored->Line(0, (SCROLL_BAR_SPACE/2), m_size.x-SCROLL_BAR_SPACE, (SCROLL_BAR_SPACE/2), 1);
			float lenScrollBar = m_size.x*(m_size.x-SCROLL_BAR_SPACE) / m_maxSize.x;
			lenScrollBar = etk_avg(10, lenScrollBar, (m_size.x-SCROLL_BAR_SPACE));
			float originScrollBar = m_originScrooled.x / (m_maxSize.x-m_size.x*m_limitScrolling);
			originScrollBar = etk_avg(0.0, originScrollBar, 1.0);
			originScrollBar *= (m_size.x-SCROLL_BAR_SPACE-lenScrollBar);
			myOObjectsColored->Rectangle(originScrollBar, 0, lenScrollBar, SCROLL_BAR_SPACE);
		}
		if (NULL!=myOObjectsColored) {
			AddOObject(myOObjectsColored);
		}
	}
}


/**
 * @brief Event on an input of this Widget
 * @param[in] type Type of the input (ewol::INPUT_TYPE_MOUSE/ewol::INPUT_TYPE_FINGER ...)
 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
 * @param[in] pos Absolute position of the event
 * @return true the event is used
 * @return false the event is not used
 */
bool ewol::WidgetScrooled::OnEventInput(ewol::inputType_te type, int32_t IdInput, ewol::eventInputType_te typeEvent, Vector2D<float> pos)
{
	Vector2D<float> relativePos = RelativePosition(pos);
	// corection due to the open Gl invertion ...
	relativePos.y = m_size.y - relativePos.y;
	if (SCROLL_MODE_NORMAL == m_scroollingMode) {
		if (ewol::INPUT_TYPE_MOUSE==type && ( ewol::INPUT_TYPE_UNKNOW==m_highSpeedType || ewol::INPUT_TYPE_MOUSE==m_highSpeedType )) {
			if (1 == IdInput && ewol::EVENT_INPUT_TYPE_DOWN == typeEvent) {
				// check if selected the scrolling position whth the scrolling bar ...
				if (relativePos.x >= (m_size.x-SCROLL_BAR_SPACE)) {
					if(m_size.y < m_maxSize.y) {
						m_highSpeedMode = ewol::SCROLL_ENABLE_VERTICAL;
						m_highSpeedType = ewol::INPUT_TYPE_MOUSE;
						m_highSpeedStartPos.x = relativePos.x;
						m_highSpeedStartPos.y = m_originScrooled.y / m_maxSize.y * (m_size.y-SCROLL_BAR_SPACE*2);
						m_highSpeedButton = 1;
						// force direct scrolling in this case
						m_originScrooled.y = (int32_t)(m_maxSize.y * (relativePos.y-SCROLL_BAR_SPACE) / (m_size.y-SCROLL_BAR_SPACE*2));
						m_originScrooled.y = etk_avg(0, m_originScrooled.y, (m_maxSize.y - m_size.y*m_limitScrolling));
						MarkToRedraw();
						return true;
					}
				} else if (relativePos.y >= (m_size.y-SCROLL_BAR_SPACE)) {
					if(m_size.x < m_maxSize.x) {
						m_highSpeedMode = ewol::SCROLL_ENABLE_HORIZONTAL;
						m_highSpeedType = ewol::INPUT_TYPE_MOUSE;
						m_highSpeedStartPos.x = m_originScrooled.x / m_maxSize.x * (m_size.x-SCROLL_BAR_SPACE*2);
						m_highSpeedStartPos.y = relativePos.y;
						m_highSpeedButton = 1;
						// force direct scrolling in this case
						m_originScrooled.x = (int32_t)(m_maxSize.x * (relativePos.x-SCROLL_BAR_SPACE) / (m_size.x-SCROLL_BAR_SPACE*2));
						m_originScrooled.x = etk_avg(0, m_originScrooled.x, (m_maxSize.x - m_size.x*m_limitScrolling));
						MarkToRedraw();
						return true;
					}
				}
				return false;
			} else if (4 == IdInput && ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
				if(m_size.y < m_maxSize.y) {
					m_originScrooled.y -= m_pixelScrolling;
					m_originScrooled.y = etk_avg(0, m_originScrooled.y, (m_maxSize.y - m_size.y*m_limitScrolling));
					MarkToRedraw();
					return true;
				}
			} else if (5 == IdInput && ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
				if(m_size.y < m_maxSize.y) {
					m_originScrooled.y += m_pixelScrolling;
					m_originScrooled.y = etk_avg(0, m_originScrooled.y, (m_maxSize.y - m_size.y*m_limitScrolling));
					MarkToRedraw();
					return true;
				}
			}else if (2 == IdInput) {
				if (ewol::EVENT_INPUT_TYPE_DOWN == typeEvent) {
					m_highSpeedMode = ewol::SCROLL_INIT;
					m_highSpeedType = ewol::INPUT_TYPE_MOUSE;
					m_highSpeedStartPos.x = relativePos.x;
					m_highSpeedStartPos.y = relativePos.y;
					m_highSpeedButton = 2;
					return true;
				}
			} else if (ewol::SCROLL_DISABLE!=m_highSpeedMode && ewol::EVENT_INPUT_TYPE_LEAVE == typeEvent) {
				m_highSpeedMode = ewol::SCROLL_DISABLE;
				m_highSpeedType = ewol::INPUT_TYPE_UNKNOW;
				MarkToRedraw();
				return true;
			}
			if (IdInput==m_highSpeedButton && ewol::SCROLL_DISABLE!=m_highSpeedMode) {
				if (ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
					if (ewol::SCROLL_INIT==m_highSpeedMode) {
						// TODO : Generate back the down event ...
						m_highSpeedMode = ewol::SCROLL_DISABLE;
						m_highSpeedType = ewol::INPUT_TYPE_UNKNOW;
						return false;
					} else {
						m_highSpeedMode = ewol::SCROLL_GREP_END_EVENT;
						MarkToRedraw();
						return true;
					}
				} else if (ewol::SCROLL_GREP_END_EVENT == m_highSpeedMode) {
					if (ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent) {
						m_highSpeedMode = ewol::SCROLL_DISABLE;
						m_highSpeedType = ewol::INPUT_TYPE_UNKNOW;
						m_highSpeedButton = -1;
						MarkToRedraw();
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
							float coef = (relativePos.y - m_highSpeedStartPos.y) / (relativePos.x - m_highSpeedStartPos.x);
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
						MarkToRedraw();
					}
					m_originScrooled.y = etk_avg(0, m_originScrooled.y, (m_maxSize.y - m_size.y*m_limitScrolling));
					return true;
				} if (ewol::SCROLL_ENABLE_HORIZONTAL==m_highSpeedMode && ewol::EVENT_INPUT_TYPE_MOVE == typeEvent) {
					m_originScrooled.x = (int32_t)(m_maxSize.x * (relativePos.x-SCROLL_BAR_SPACE) / (m_size.x-SCROLL_BAR_SPACE*2));
					m_originScrooled.x = etk_avg(0, m_originScrooled.x, (m_maxSize.x - m_size.x*m_limitScrolling));
					MarkToRedraw();
					return true;
				} if (ewol::SCROLL_ENABLE_VERTICAL==m_highSpeedMode && ewol::EVENT_INPUT_TYPE_MOVE == typeEvent) {
					m_originScrooled.y = (int32_t)(m_maxSize.y * (relativePos.y-SCROLL_BAR_SPACE) / (m_size.y-SCROLL_BAR_SPACE*2));
					m_originScrooled.y = etk_avg(0, m_originScrooled.y, (m_maxSize.y - m_size.y*m_limitScrolling));
					MarkToRedraw();
					return true;
				}
			}
		} else if (ewol::INPUT_TYPE_FINGER==type && ( ewol::INPUT_TYPE_UNKNOW==m_highSpeedType || ewol::INPUT_TYPE_FINGER==m_highSpeedType )) {
			if (1 == IdInput) {
				EWOL_VERBOSE("event 1  << " << (int32_t)typeEvent << "(" << x << "," << y << ")");
				if (ewol::EVENT_INPUT_TYPE_DOWN == typeEvent) {
					m_highSpeedMode = ewol::SCROLL_INIT;
					m_highSpeedType = ewol::INPUT_TYPE_FINGER;
					m_highSpeedStartPos.x = relativePos.x;
					m_highSpeedStartPos.y = relativePos.y;
					EWOL_VERBOSE("SCROOL ==> INIT");
					return true;
				} else if (ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
					m_highSpeedMode = ewol::SCROLL_DISABLE;
					m_highSpeedType = ewol::INPUT_TYPE_UNKNOW;
					EWOL_VERBOSE("SCROOL ==> DISABLE");
					MarkToRedraw();
					return true;
				} else if (ewol::SCROLL_INIT==m_highSpeedMode && ewol::EVENT_INPUT_TYPE_MOVE == typeEvent) {
					// wait that the cursor move more than 10 px to enable it :
					if(    abs(relativePos.x - m_highSpeedStartPos.x) > 10 
					    || abs(relativePos.y - m_highSpeedStartPos.y) > 10 ) {
						// the scrooling can start : 
						// select the direction :
						m_highSpeedMode = ewol::SCROLL_ENABLE_FINGER;
						EWOL_VERBOSE("SCROOL ==> ENABLE");
						MarkToRedraw();
					}
					return true;
				} if (ewol::SCROLL_ENABLE_FINGER==m_highSpeedMode && ewol::EVENT_INPUT_TYPE_MOVE == typeEvent) {
					//m_originScrooled.x = (int32_t)(m_maxSize.x * x / m_size.x);
					m_originScrooled.x -= relativePos.x - m_highSpeedStartPos.x;
					m_originScrooled.y -= relativePos.y - m_highSpeedStartPos.y;
					m_originScrooled.x = etk_avg(0, m_originScrooled.x, (m_maxSize.x - m_size.x*m_limitScrolling));
					m_originScrooled.y = etk_avg(0, m_originScrooled.y, (m_maxSize.y - m_size.y*m_limitScrolling));
					m_highSpeedStartPos.x = relativePos.x;
					m_highSpeedStartPos.y = relativePos.y;
					EWOL_VERBOSE("SCROOL ==> MOVE (" << m_originScrooled.x << "," << m_originScrooled.y << ")");
					MarkToRedraw();
					return true;
				}
			} else if (ewol::SCROLL_DISABLE!=m_highSpeedMode && ewol::EVENT_INPUT_TYPE_LEAVE == typeEvent) {
				m_highSpeedMode = ewol::SCROLL_DISABLE;
				m_highSpeedType = ewol::INPUT_TYPE_UNKNOW;
				EWOL_VERBOSE("SCROOL ==> DISABLE");
				MarkToRedraw();
				return true;
			}
		}
	} else if (SCROLL_MODE_CENTER == m_scroollingMode) {
		if (ewol::INPUT_TYPE_MOUSE==type) {
			float tmp1=ewol::GetCurrentHeight() / m_maxSize.y;
			float tmp2=ewol::GetCurrentWidth() / m_maxSize.x;
			//EWOL_INFO(" elements Zoom : " << tmp1 << " " << tmp2);
			tmp1 = etk_min(tmp1, tmp2);
			if (4 == IdInput && ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
				m_zoom -= 0.1;
				if (tmp1 < 1.0) {
					m_zoom = etk_max(tmp1, m_zoom);
				} else {
					m_zoom = etk_max(1.0, m_zoom);
				}
				MarkToRedraw();
				return true;
			} else if (5 == IdInput && ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
				m_zoom += 0.1;
				if (tmp1 > 1.0) {
					m_zoom = etk_min(tmp1, m_zoom);
				} else {
					m_zoom = etk_min(1.0, m_zoom);
				}
				MarkToRedraw();
				return true;
			}
		}
	} else if (SCROLL_MODE_GAME == m_scroollingMode) {
		
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
	if (pos < 0 || pos >= m_listOObject.Size() ) {
		m_listOObject.PushBack(newObject);
	} else {
		m_listOObject.Insert(pos, newObject);
	}
}


void ewol::WidgetScrooled::ClearOObjectList(void)
{
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		delete(m_listOObject[iii]);
		m_listOObject[iii] = NULL;
	}
	m_listOObject.Clear();
}

void ewol::WidgetScrooled::OnDraw(DrawProperty& displayProp)
{
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		if (NULL != m_listOObject[iii]) {
			m_listOObject[iii]->Draw();
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
void ewol::WidgetScrooled::GenDraw(DrawProperty displayProp)
{
	if (SCROLL_MODE_CENTER == m_scroollingMode) {
		glPushMatrix();
		// here we invert the reference of the standard OpenGl view because the reference in the common display is Top left and not buttom left
		glViewport( m_origin.x,
		            m_origin.y,
		            m_size.x,
		            m_size.y);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrthoEwol(-m_size.x/2, m_size.x/2, -m_size.y/2, m_size.y/2, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glScalef(m_zoom, m_zoom, 1.0);
		glTranslatef(-m_maxSize.x/2, -m_maxSize.y/2, -1.0);
		
		// Call the widget drawing methode
		OnDraw(displayProp);
		glPopMatrix();
	} if (SCROLL_MODE_GAME == m_scroollingMode) {
		glPushMatrix();
		// here we invert the reference of the standard OpenGl view because the reference in the common display is Top left and not buttom left
		glViewport( m_origin.x,
		            m_origin.y,
		            m_size.x,
		            m_size.y);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrthoEwol(-m_size.x/2, m_size.x/2, -m_size.y/2, m_size.y/2, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//glScalef(m_zoom, m_zoom, 1.0);
		glTranslatef(-m_maxSize.x/2, -m_maxSize.y/2, -1.0);
		
		// Call the widget drawing methode
		OnDraw(displayProp);
		glPopMatrix();
	} else {
		ewol::Widget::GenDraw(displayProp);
	}
	
}


/**
 * @brief Request a specific position for the scrolling of the current windows.
 * @param[in] borderWidth Size of the border that requested the element might not to be
 * @param[in] currentPosition Position that is requested to view
 * @param[in] center True if the position might be at the center of the widget
 * @return ---
 */
void ewol::WidgetScrooled::SetScrollingPositionDynamic(Vector2D<float> borderWidth, Vector2D<float> currentPosition, bool center)
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



/**
 * @brief Specify the mode of scrolling for this windows
 * @param[in] newMode the selected mode for the scrolling...
 * @return ---
 */
void ewol::WidgetScrooled::ScroolingMode(scrollingMode_te newMode)
{
	m_scroollingMode = newMode;
	if (SCROLL_MODE_GAME == m_scroollingMode) {
		// set the scene maximum size :
		m_maxSize.x = etk_max(ewol::GetCurrentHeight(), ewol::GetCurrentWidth());
		m_maxSize.y = m_maxSize.x;
		m_zoom = 1;
	}
}

