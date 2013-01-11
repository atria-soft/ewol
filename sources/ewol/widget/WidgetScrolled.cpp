/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/WidgetScrolled.h>

#include <ewol/compositing/Drawing.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/renderer/openGL.h>
#include <ewol/ewol.h>



widget::WidgetScrooled::WidgetScrooled(void)
{
	m_originScrooled.x = 0;
	m_originScrooled.y = 0;
	m_pixelScrolling = 20;
	m_highSpeedMode = SCROLL_DISABLE;
	m_scroollingMode = SCROLL_MODE_NORMAL;
	m_highSpeedType = ewol::keyEvent::typeUnknow;
	m_highSpeedButton = -1;
	m_limitScrolling = 0.5;
}

widget::WidgetScrooled::~WidgetScrooled(void)
{
	
}


#define SCROLL_BAR_SPACE      (15)

void widget::WidgetScrooled::OnRegenerateDisplay(void)
{
	ClearOObjectList();
	if (SCROLL_MODE_GAME == m_scroollingMode) {
		
	} else {
		ewol::Drawing* tmpDraw = NULL;
		if(m_size.y < m_maxSize.y || m_size.x < m_maxSize.x) {
			tmpDraw = new ewol::Drawing();
			tmpDraw->SetColor(0xFF00007F);
		}
		if (NULL==tmpDraw) {
			return;
		}
		tmpDraw->SetThickness(1);
		if(    m_size.y < m_maxSize.y
		    || m_originScrooled.y!=0) {
			tmpDraw->SetPos(vec3(m_size.x-(SCROLL_BAR_SPACE/2), 0) );
			tmpDraw->LineTo(vec3(m_size.x-(SCROLL_BAR_SPACE/2), m_size.y ) );
			float lenScrollBar = m_size.y*m_size.y / m_maxSize.y;
			lenScrollBar = etk_avg(10, lenScrollBar, m_size.y);
			float originScrollBar = m_originScrooled.y / (m_maxSize.y-m_size.y*m_limitScrolling);
			originScrollBar = etk_avg(0.0, originScrollBar, 1.0);
			originScrollBar *= (m_size.y-lenScrollBar);
			tmpDraw->SetPos(vec3(m_size.x-SCROLL_BAR_SPACE, m_size.y - originScrollBar - lenScrollBar) );
			tmpDraw->RectangleWidth(vec3(SCROLL_BAR_SPACE, lenScrollBar));
		}
		if(    m_size.x < m_maxSize.x
		    || m_originScrooled.x!=0) {
			tmpDraw->SetPos(vec3(0, (SCROLL_BAR_SPACE/2), 0) );
			tmpDraw->LineTo(vec3(m_size.x-SCROLL_BAR_SPACE, (SCROLL_BAR_SPACE/2) ) );
			float lenScrollBar = m_size.x*(m_size.x-SCROLL_BAR_SPACE) / m_maxSize.x;
			lenScrollBar = etk_avg(10, lenScrollBar, (m_size.x-SCROLL_BAR_SPACE));
			float originScrollBar = m_originScrooled.x / (m_maxSize.x-m_size.x*m_limitScrolling);
			originScrollBar = etk_avg(0.0, originScrollBar, 1.0);
			originScrollBar *= (m_size.x-SCROLL_BAR_SPACE-lenScrollBar);
			tmpDraw->SetPos(vec3(originScrollBar, 0, 0) );
			tmpDraw->Rectangle(vec3(lenScrollBar, SCROLL_BAR_SPACE) );
		}
		AddOObject(tmpDraw);
	}
}


bool widget::WidgetScrooled::OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, vec2 pos)
{
	vec2 relativePos = RelativePosition(pos);
	// corection due to the open Gl invertion ...
	relativePos.y = m_size.y - relativePos.y;
	if (SCROLL_MODE_NORMAL == m_scroollingMode) {
		if (ewol::keyEvent::typeMouse==type && ( ewol::keyEvent::typeUnknow==m_highSpeedType || ewol::keyEvent::typeMouse==m_highSpeedType )) {
			if (1 == IdInput && ewol::keyEvent::statusDown == typeEvent) {
				// check if selected the scrolling position whth the scrolling bar ...
				if (relativePos.x >= (m_size.x-SCROLL_BAR_SPACE)) {
					if(    m_size.y < m_maxSize.y
					    || m_originScrooled.y != 0) {
						m_highSpeedMode = widget::SCROLL_ENABLE_VERTICAL;
						m_highSpeedType = ewol::keyEvent::typeMouse;
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
					if(    m_size.x < m_maxSize.x
					    || m_originScrooled.x!=0) {
						m_highSpeedMode = widget::SCROLL_ENABLE_HORIZONTAL;
						m_highSpeedType = ewol::keyEvent::typeMouse;
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
			} else if (4 == IdInput && ewol::keyEvent::statusUp == typeEvent) {
				/*
				if (true == ewol::IsSetCtrl()) {
					float zoom = GetZoom()*1.1;
					zoom = etk_avg(0.1, zoom, 5000);
					SetZoom(zoom);
				} else */{
					if(m_size.y < m_maxSize.y) {
						m_originScrooled.y -= m_pixelScrolling;
						m_originScrooled.y = etk_avg(0, m_originScrooled.y, (m_maxSize.y - m_size.y*m_limitScrolling));
						MarkToRedraw();
						return true;
					}
				}
			} else if (5 == IdInput && ewol::keyEvent::statusUp == typeEvent) {
				/*
				if (true == ewol::IsSetCtrl()) {
					float zoom = GetZoom()*0.9;
					zoom = etk_avg(0.1, zoom, 5000);
					SetZoom(zoom);
				} else */{
					if(m_size.y < m_maxSize.y) {
						m_originScrooled.y += m_pixelScrolling;
						m_originScrooled.y = etk_avg(0, m_originScrooled.y, (m_maxSize.y - m_size.y*m_limitScrolling));
						MarkToRedraw();
						return true;
					}
				}
			}else if (2 == IdInput) {
				/*
				if (true == ewol::IsSetCtrl()) {
					if (ewol::keyEvent::statusDown == typeEvent) {
						float zoom = 1.0;
						SetZoom(zoom);
					}
				} else */{
					if (ewol::keyEvent::statusDown == typeEvent) {
						m_highSpeedMode = widget::SCROLL_INIT;
						m_highSpeedType = ewol::keyEvent::typeMouse;
						m_highSpeedStartPos.x = relativePos.x;
						m_highSpeedStartPos.y = relativePos.y;
						m_highSpeedButton = 2;
						return true;
					}
				}
			} else if (widget::SCROLL_DISABLE!=m_highSpeedMode && ewol::keyEvent::statusLeave == typeEvent) {
				m_highSpeedMode = widget::SCROLL_DISABLE;
				m_highSpeedType = ewol::keyEvent::typeUnknow;
				MarkToRedraw();
				return true;
			}
			if (IdInput==m_highSpeedButton && widget::SCROLL_DISABLE!=m_highSpeedMode) {
				if (ewol::keyEvent::statusUp == typeEvent) {
					if (widget::SCROLL_INIT==m_highSpeedMode) {
						// TODO : Generate back the down event ...
						m_highSpeedMode = widget::SCROLL_DISABLE;
						m_highSpeedType = ewol::keyEvent::typeUnknow;
						return false;
					} else {
						m_highSpeedMode = widget::SCROLL_GREP_END_EVENT;
						MarkToRedraw();
						return true;
					}
				} else if (widget::SCROLL_GREP_END_EVENT == m_highSpeedMode) {
					if (ewol::keyEvent::statusSingle == typeEvent) {
						m_highSpeedMode = widget::SCROLL_DISABLE;
						m_highSpeedType = ewol::keyEvent::typeUnknow;
						m_highSpeedButton = -1;
						MarkToRedraw();
					}
					return true;
				} else if (widget::SCROLL_INIT==m_highSpeedMode && ewol::keyEvent::statusMove == typeEvent) {
					// wait that the cursor move more than 10 px to enable it :
					if(    abs(relativePos.x - m_highSpeedStartPos.x) > 10 
					    || abs(relativePos.y - m_highSpeedStartPos.y) > 10 ) {
						// the scrooling can start : 
						// select the direction :
						if (relativePos.x == m_highSpeedStartPos.x) {
							m_highSpeedMode = widget::SCROLL_ENABLE_VERTICAL;
						} else if (relativePos.y == m_highSpeedStartPos.y) {
							m_highSpeedMode = widget::SCROLL_ENABLE_HORIZONTAL;
						} else {
							float coef = (relativePos.y - m_highSpeedStartPos.y) / (relativePos.x - m_highSpeedStartPos.x);
							if (abs(coef) <= 1 ) {
								m_highSpeedMode = widget::SCROLL_ENABLE_HORIZONTAL;
							} else {
								m_highSpeedMode = widget::SCROLL_ENABLE_VERTICAL;
							}
						}
						if (m_highSpeedMode == widget::SCROLL_ENABLE_HORIZONTAL) {
							m_highSpeedStartPos.x = m_originScrooled.x / m_maxSize.x * (m_size.x-SCROLL_BAR_SPACE*2);
						} else {
							m_highSpeedStartPos.y = m_originScrooled.y / m_maxSize.y * (m_size.y-SCROLL_BAR_SPACE*2);
						}
						MarkToRedraw();
					}
					m_originScrooled.y = etk_avg(0, m_originScrooled.y, (m_maxSize.y - m_size.y*m_limitScrolling));
					return true;
				} if (widget::SCROLL_ENABLE_HORIZONTAL==m_highSpeedMode && ewol::keyEvent::statusMove == typeEvent) {
					m_originScrooled.x = (int32_t)(m_maxSize.x * (relativePos.x-SCROLL_BAR_SPACE) / (m_size.x-SCROLL_BAR_SPACE*2));
					m_originScrooled.x = etk_avg(0, m_originScrooled.x, (m_maxSize.x - m_size.x*m_limitScrolling));
					MarkToRedraw();
					return true;
				} if (widget::SCROLL_ENABLE_VERTICAL==m_highSpeedMode && ewol::keyEvent::statusMove == typeEvent) {
					m_originScrooled.y = (int32_t)(m_maxSize.y * (relativePos.y-SCROLL_BAR_SPACE) / (m_size.y-SCROLL_BAR_SPACE*2));
					m_originScrooled.y = etk_avg(0, m_originScrooled.y, (m_maxSize.y - m_size.y*m_limitScrolling));
					MarkToRedraw();
					return true;
				}
			}
		} else if (ewol::keyEvent::typeFinger==type && ( ewol::keyEvent::typeUnknow==m_highSpeedType || ewol::keyEvent::typeFinger==m_highSpeedType )) {
			if (1 == IdInput) {
				//EWOL_VERBOSE("event 1  << " << (int32_t)typeEvent << "(" << x << "," << y << ")");
				if (ewol::keyEvent::statusDown == typeEvent) {
					m_highSpeedMode = widget::SCROLL_INIT;
					m_highSpeedType = ewol::keyEvent::typeFinger;
					m_highSpeedStartPos.x = relativePos.x;
					m_highSpeedStartPos.y = relativePos.y;
					EWOL_VERBOSE("SCROOL ==> INIT");
					return true;
				} else if (ewol::keyEvent::statusUp == typeEvent) {
					m_highSpeedMode = widget::SCROLL_DISABLE;
					m_highSpeedType = ewol::keyEvent::typeUnknow;
					EWOL_VERBOSE("SCROOL ==> DISABLE");
					MarkToRedraw();
					return true;
				} else if (widget::SCROLL_INIT==m_highSpeedMode && ewol::keyEvent::statusMove == typeEvent) {
					// wait that the cursor move more than 10 px to enable it :
					if(    abs(relativePos.x - m_highSpeedStartPos.x) > 10 
					    || abs(relativePos.y - m_highSpeedStartPos.y) > 10 ) {
						// the scrooling can start : 
						// select the direction :
						m_highSpeedMode = widget::SCROLL_ENABLE_FINGER;
						EWOL_VERBOSE("SCROOL ==> ENABLE");
						MarkToRedraw();
					}
					return true;
				} if (widget::SCROLL_ENABLE_FINGER==m_highSpeedMode && ewol::keyEvent::statusMove == typeEvent) {
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
			} else if (widget::SCROLL_DISABLE!=m_highSpeedMode && ewol::keyEvent::statusLeave == typeEvent) {
				m_highSpeedMode = widget::SCROLL_DISABLE;
				m_highSpeedType = ewol::keyEvent::typeUnknow;
				EWOL_VERBOSE("SCROOL ==> DISABLE");
				MarkToRedraw();
				return true;
			}
		}
	} else if (SCROLL_MODE_CENTER == m_scroollingMode) {
		if (ewol::keyEvent::typeMouse==type) {
			float tmp1=m_size.x / m_maxSize.y;
			float tmp2=m_size.y / m_maxSize.x;
			//EWOL_INFO(" elements Zoom : " << tmp1 << " " << tmp2);
			tmp1 = etk_min(tmp1, tmp2);
			if (4 == IdInput && ewol::keyEvent::statusUp == typeEvent) {
				m_zoom -= 0.1;
				if (tmp1 < 1.0) {
					m_zoom = etk_max(tmp1, m_zoom);
				} else {
					m_zoom = etk_max(1.0, m_zoom);
				}
				MarkToRedraw();
				return true;
			} else if (5 == IdInput && ewol::keyEvent::statusUp == typeEvent) {
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

void widget::WidgetScrooled::AddOObject(ewol::Compositing* newObject, int32_t pos)
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


void widget::WidgetScrooled::ClearOObjectList(void)
{
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		delete(m_listOObject[iii]);
		m_listOObject[iii] = NULL;
	}
	m_listOObject.Clear();
}

void widget::WidgetScrooled::OnDraw(ewol::DrawProperty& displayProp)
{
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		if (NULL != m_listOObject[iii]) {
			m_listOObject[iii]->Draw();
		}
	}
}


void widget::WidgetScrooled::GenDraw(ewol::DrawProperty displayProp)
{
	ewol::openGL::Push();
	if (SCROLL_MODE_CENTER == m_scroollingMode) {
		// here we invert the reference of the standard OpenGl view because the reference in the common display is Top left and not buttom left
		glViewport( m_origin.x,
		            m_origin.y,
		            m_size.x,
		            m_size.y);
		mat4 tmpProjection = etk::matOrtho(-m_size.x/2, m_size.x/2, -m_size.y/2, m_size.y/2, -1, 1);
		mat4 tmpScale = etk::matScale(vec3(m_zoom, m_zoom, 1.0) );
		mat4 tmpTranslate = etk::matTranslate(vec3(-m_maxSize.x/2, -m_maxSize.y/2, -1.0) );
		mat4 tmpMat = tmpProjection * tmpScale * tmpTranslate;
		// set internal matrix system :
		ewol::openGL::SetMatrix(tmpMat);
		// Call the widget drawing methode
		OnDraw(displayProp);
	} if (SCROLL_MODE_GAME == m_scroollingMode) {
		// here we invert the reference of the standard OpenGl view because the reference in the common display is Top left and not buttom left
		glViewport( m_origin.x,
		            m_origin.y,
		            m_size.x,
		            m_size.y);
		
		mat4 tmpProjection = etk::matOrtho(-m_size.x/2, m_size.x/2, -m_size.y/2, m_size.y/2, -1, 1);
		mat4 tmpTranslate = etk::matTranslate(vec3( -m_maxSize.x/2, -m_maxSize.y/2, -1.0) );
		mat4 tmpMat = tmpProjection * tmpTranslate;
		// set internal matrix system :
		ewol::openGL::SetMatrix(tmpMat);
		// Call the widget drawing methode
		OnDraw(displayProp);
	} else {
		ewol::Widget::GenDraw(displayProp);
	}
	ewol::openGL::Pop();
}


void widget::WidgetScrooled::SetScrollingPositionDynamic(vec2 borderWidth, vec2 currentPosition, bool center)
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


void widget::WidgetScrooled::ScroolingMode(scrollingMode_te newMode)
{
	m_scroollingMode = newMode;
	if (SCROLL_MODE_GAME == m_scroollingMode) {
		// set the scene maximum size :
		m_maxSize.x = etk_max(m_size.x, m_size.y);
		m_maxSize.y = m_maxSize.x;
		m_zoom = 1;
	}
}

