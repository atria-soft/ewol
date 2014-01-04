/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/WidgetScrolled.h>

#include <ewol/compositing/Drawing.h>
#include <ewol/widget/Manager.h>
#include <ewol/openGL/openGL.h>
#include <ewol/ewol.h>



ewol::widget::WidgetScrooled::WidgetScrooled(void) {
	addObjectType("ewol::widget::WidgetScrooled");
	m_originScrooled.setValue(0,0);
	m_pixelScrolling = 20;
	m_highSpeedMode = ewol::widget::Scroll::speedModeDisable;
	m_scroollingMode = scroolModeNormal;
	m_highSpeedType = ewol::key::typeUnknow;
	m_highSpeedButton = -1;
	m_limitScrolling = 0.5;
	
	m_fingerScoolActivated = false;
	for (size_t iii = 0; iii < CALCULATE_SIMULTANEOUS_FINGER; ++iii) {
		m_fingerPresent[iii] = false;
	}
}

ewol::widget::WidgetScrooled::~WidgetScrooled(void) {
	
}


#define SCROLL_BAR_SPACE      (15)

void ewol::widget::WidgetScrooled::onRegenerateDisplay(void) {
	m_drawing.clear();
	if (m_scroollingMode == scroolModeGame) {
		
	} else {
		m_drawing.setColor(0xFF00007F);
		m_drawing.setThickness(1);
		if(    m_size.y() < m_maxSize.y()
		    || m_originScrooled.y()!=0) {
			m_drawing.setPos(vec3(m_size.x()-(SCROLL_BAR_SPACE/2), 0, 0) );
			m_drawing.lineTo(vec3(m_size.x()-(SCROLL_BAR_SPACE/2), m_size.y(), 0 ) );
			float lenScrollBar = m_size.y()*m_size.y() / m_maxSize.y();
			lenScrollBar = etk_avg(10, lenScrollBar, m_size.y());
			float originScrollBar = m_originScrooled.y() / (m_maxSize.y()-m_size.y()*m_limitScrolling);
			originScrollBar = etk_avg(0.0, originScrollBar, 1.0);
			originScrollBar *= (m_size.y()-lenScrollBar);
			m_drawing.setPos(vec3(m_size.x()-SCROLL_BAR_SPACE, m_size.y() - originScrollBar - lenScrollBar, 0) );
			m_drawing.rectangleWidth(vec3(SCROLL_BAR_SPACE, lenScrollBar, 0));
		}
		if(    m_size.x() < m_maxSize.x()
		    || m_originScrooled.x()!=0) {
			m_drawing.setPos(vec3(0, (SCROLL_BAR_SPACE/2), 0) );
			m_drawing.lineTo(vec3(m_size.x()-SCROLL_BAR_SPACE, (SCROLL_BAR_SPACE/2), 0 ) );
			float lenScrollBar = (m_size.x()-SCROLL_BAR_SPACE)*(m_size.x()-SCROLL_BAR_SPACE) / m_maxSize.x();
			lenScrollBar = etk_avg(10, lenScrollBar, (m_size.x()-SCROLL_BAR_SPACE));
			float originScrollBar = m_originScrooled.x() / (m_maxSize.x()-m_size.x()*m_limitScrolling);
			originScrollBar = etk_avg(0.0, originScrollBar, 1.0);
			originScrollBar *= (m_size.x()-SCROLL_BAR_SPACE-lenScrollBar);
			m_drawing.setPos(vec3(originScrollBar, 0, 0) );
			m_drawing.rectangleWidth(vec3(lenScrollBar, SCROLL_BAR_SPACE, 0) );
		}
	}
}

bool ewol::widget::WidgetScrooled::onEventInput(const ewol::event::Input& _event) {
	EWOL_VERBOSE("event XXX " << _event);
	vec2 relativePos = relativePosition(_event.getPos());
	// corection due to the open Gl invertion ...
	relativePos.setY(m_size.y() - relativePos.y());
	if (m_scroollingMode == scroolModeNormal) {
		if (    _event.getType() == ewol::key::typeMouse
		     && (    m_highSpeedType == ewol::key::typeUnknow
		          || m_highSpeedType == ewol::key::typeMouse) ) {
			if (    _event.getId() == 1
			     && _event.getStatus() == ewol::key::statusDown) {
				// check if selected the scrolling position whth the scrolling bar ...
				if (relativePos.x() >= (m_size.x()-SCROLL_BAR_SPACE)) {
					if(    m_size.y() < m_maxSize.y()
					    || m_originScrooled.y() != 0) {
						m_highSpeedMode = ewol::widget::Scroll::speedModeEnableVertical;
						m_highSpeedType = ewol::key::typeMouse;
						m_highSpeedStartPos.setX(relativePos.x());
						m_highSpeedStartPos.setY(m_originScrooled.y() / m_maxSize.y() * (m_size.y()-SCROLL_BAR_SPACE*2));
						m_highSpeedButton = 1;
						// force direct scrolling in this case
						m_originScrooled.setY((int32_t)(m_maxSize.y() * (relativePos.y()-SCROLL_BAR_SPACE) / (m_size.y()-SCROLL_BAR_SPACE*2)));
						m_originScrooled.setY(etk_avg(0, m_originScrooled.y(), (m_maxSize.y() - m_size.y()*m_limitScrolling)));
						markToRedraw();
						return true;
					}
				} else if (relativePos.y() >= (m_size.y()-SCROLL_BAR_SPACE)) {
					if(    m_size.x() < m_maxSize.x()
					    || m_originScrooled.x()!=0) {
						m_highSpeedMode = ewol::widget::Scroll::speedModeEnableHorizontal;
						m_highSpeedType = ewol::key::typeMouse;
						m_highSpeedStartPos.setX(m_originScrooled.x() / m_maxSize.x() * (m_size.x()-SCROLL_BAR_SPACE*2));
						m_highSpeedStartPos.setY(relativePos.y());
						m_highSpeedButton = 1;
						// force direct scrolling in this case
						m_originScrooled.setX((int32_t)(m_maxSize.x() * (relativePos.x()-SCROLL_BAR_SPACE) / (m_size.x()-SCROLL_BAR_SPACE*2)));
						m_originScrooled.setY(etk_avg(0, m_originScrooled.x(), (m_maxSize.x() - m_size.x()*m_limitScrolling)));
						markToRedraw();
						return true;
					}
				}
				return false;
			} else if (    _event.getId() == 4
			            && _event.getStatus() == ewol::key::statusUp) {
				/*
				if (true == ewol::isSetCtrl()) {
					float zoom = getZoom()*1.1;
					zoom = etk_avg(0.1, zoom, 5000);
					setZoom(zoom);
				} else */{
					if(m_size.y() < m_maxSize.y()
					    || m_originScrooled.y() != 0
					    || m_size.y()*m_limitScrolling < m_maxSize.y() ) {
						m_originScrooled.setY(m_originScrooled.y()-m_pixelScrolling);
						m_originScrooled.setY(etk_avg(0, m_originScrooled.y(), (m_maxSize.y() - m_size.y()*m_limitScrolling)));
						markToRedraw();
						return true;
					}
				}
			} else if (    _event.getId() == 5
			            && _event.getStatus() == ewol::key::statusUp) {
				/*
				if (true == ewol::isSetCtrl()) {
					float zoom = getZoom()*0.9;
					zoom = etk_avg(0.1, zoom, 5000);
					setZoom(zoom);
				} else */{
					if(m_size.y() < m_maxSize.y()
					    || m_originScrooled.y()!=0
					    || m_size.y()*m_limitScrolling < m_maxSize.y() ) {
						m_originScrooled.setY(m_originScrooled.y()+m_pixelScrolling);
						m_originScrooled.setY(etk_avg(0, m_originScrooled.y(), (m_maxSize.y() - m_size.y()*m_limitScrolling)));
						markToRedraw();
						return true;
					}
				}
			} else if (    _event.getId() == 11
			            && _event.getStatus() == ewol::key::statusUp) {
				// Scrool Left
				if(m_size.x() < m_maxSize.x()
				    || m_originScrooled.x()!=0
				    || m_size.x()*m_limitScrolling < m_maxSize.x() ) {
					m_originScrooled.setX(m_originScrooled.x()-m_pixelScrolling);
					m_originScrooled.setX(etk_avg(0, m_originScrooled.x(), (m_maxSize.x() - m_size.x()*m_limitScrolling)));
					markToRedraw();
					return true;
				}
			} else if (    _event.getId() == 10
			            && _event.getStatus() == ewol::key::statusUp) {
				// Scrool Right
				if(m_size.x() < m_maxSize.x()
				    || m_originScrooled.x()!=0
				    || m_size.x()*m_limitScrolling < m_maxSize.x() ) {
					m_originScrooled.setX(m_originScrooled.x()+m_pixelScrolling);
					m_originScrooled.setX(etk_avg(0, m_originScrooled.x(), (m_maxSize.x() - m_size.x()*m_limitScrolling)));
					markToRedraw();
					return true;
				}
			}else if (_event.getId() == 2) {
				/*
				if (true == ewol::isSetCtrl()) {
					if (ewol::key::statusDown == typeEvent) {
						float zoom = 1.0;
						setZoom(zoom);
					}
				} else */{
					if (_event.getStatus() == ewol::key::statusDown) {
						m_highSpeedMode = ewol::widget::Scroll::speedModeInit;
						m_highSpeedType = ewol::key::typeMouse;
						m_highSpeedStartPos.setValue(relativePos.x(), relativePos.y());
						m_highSpeedButton = 2;
						return true;
					}
				}
			} else if (    m_highSpeedMode != ewol::widget::Scroll::speedModeDisable
			            && _event.getStatus() == ewol::key::statusLeave) {
				m_highSpeedMode = ewol::widget::Scroll::speedModeDisable;
				m_highSpeedType = ewol::key::typeUnknow;
				markToRedraw();
				return true;
			}
			if (    _event.getId() == m_highSpeedButton
			     && m_highSpeedMode != ewol::widget::Scroll::speedModeDisable) {
				if (_event.getStatus() == ewol::key::statusUp) {
					if (m_highSpeedMode == ewol::widget::Scroll::speedModeInit) {
						// TODO : generate back the down event ...
						m_highSpeedMode = ewol::widget::Scroll::speedModeDisable;
						m_highSpeedType = ewol::key::typeUnknow;
						return false;
					} else {
						m_highSpeedMode = ewol::widget::Scroll::speedModeGrepEndEvent;
						markToRedraw();
						return true;
					}
				} else if (m_highSpeedMode == ewol::widget::Scroll::speedModeGrepEndEvent) {
					if (_event.getStatus() == ewol::key::statusSingle) {
						m_highSpeedMode = ewol::widget::Scroll::speedModeDisable;
						m_highSpeedType = ewol::key::typeUnknow;
						m_highSpeedButton = -1;
						markToRedraw();
					}
					return true;
				} else if (    m_highSpeedMode == ewol::widget::Scroll::speedModeInit
				            && _event.getStatus() == ewol::key::statusMove) {
					// wait that the cursor move more than 10 px to enable it :
					if(    abs(relativePos.x() - m_highSpeedStartPos.x()) > 10 
					    || abs(relativePos.y() - m_highSpeedStartPos.y()) > 10 ) {
						// the scrooling can start : 
						// select the direction :
						if (relativePos.x() == m_highSpeedStartPos.x()) {
							m_highSpeedMode = ewol::widget::Scroll::speedModeEnableVertical;
						} else if (relativePos.y() == m_highSpeedStartPos.y()) {
							m_highSpeedMode = ewol::widget::Scroll::speedModeEnableHorizontal;
						} else {
							float coef = (relativePos.y() - m_highSpeedStartPos.y()) / (relativePos.x() - m_highSpeedStartPos.x());
							if (abs(coef) <= 1 ) {
								m_highSpeedMode = ewol::widget::Scroll::speedModeEnableHorizontal;
							} else {
								m_highSpeedMode = ewol::widget::Scroll::speedModeEnableVertical;
							}
						}
						if (m_highSpeedMode == ewol::widget::Scroll::speedModeEnableHorizontal) {
							m_highSpeedStartPos.setX(m_originScrooled.x() / m_maxSize.x() * (m_size.x()-SCROLL_BAR_SPACE*2));
						} else {
							m_highSpeedStartPos.setY(m_originScrooled.y() / m_maxSize.y() * (m_size.y()-SCROLL_BAR_SPACE*2));
						}
						markToRedraw();
					}
					m_originScrooled.setY(etk_avg(0, m_originScrooled.y(), (m_maxSize.y() - m_size.y()*m_limitScrolling)));
					return true;
				}
				if (    m_highSpeedMode == ewol::widget::Scroll::speedModeEnableHorizontal
				     && _event.getStatus() == ewol::key::statusMove) {
					m_originScrooled.setX((int32_t)(m_maxSize.x() * (relativePos.x()-SCROLL_BAR_SPACE) / (m_size.x()-SCROLL_BAR_SPACE*2)));
					m_originScrooled.setX(etk_avg(0, m_originScrooled.x(), (m_maxSize.x() - m_size.x()*m_limitScrolling)));
					markToRedraw();
					return true;
				}
				if (    m_highSpeedMode == ewol::widget::Scroll::speedModeEnableVertical
				     && _event.getStatus() == ewol::key::statusMove) {
					m_originScrooled.setY((int32_t)(m_maxSize.y() * (relativePos.y()-SCROLL_BAR_SPACE) / (m_size.y()-SCROLL_BAR_SPACE*2)));
					m_originScrooled.setY(etk_avg(0, m_originScrooled.y(), (m_maxSize.y() - m_size.y()*m_limitScrolling)));
					markToRedraw();
					return true;
				}
			}
		} else if (    _event.getType() == ewol::key::typeFinger
		            && (    m_highSpeedType == ewol::key::typeUnknow
		                 || m_highSpeedType == ewol::key::typeFinger) ) {
			if (_event.getId() >= 3) {
				return false;
			}
			int32_t idTable = _event.getId()-1;
			if (_event.getStatus() == ewol::key::statusDown) {
				m_fingerPresent[idTable] = true;
			} else if (_event.getStatus() == ewol::key::statusUp) {
				m_fingerPresent[idTable] = false;
			}
			if (m_fingerScoolActivated == false) {
				m_fingerMoveStartPos[idTable] = relativePos;
			}
			if (    m_fingerPresent[0] == true
			     && m_fingerPresent[1] == true
			     && m_fingerScoolActivated == false) {
				m_fingerScoolActivated = true;
				EWOL_DEBUG("SCROOL  == > START pos=" << m_fingerMoveStartPos);
			}
			if (m_fingerScoolActivated == true) {
				// 1: scroll...
				// 2: remove all unneeded sub event ... ==> maybe a better methode ...
				if (_event.getStatus() == ewol::key::statusMove) {
					m_originScrooled.setX(m_originScrooled.x() - (relativePos.x() - m_fingerMoveStartPos[idTable].x())*0.5f);
					m_originScrooled.setY(m_originScrooled.y() - (relativePos.y() - m_fingerMoveStartPos[idTable].y())*0.5f);
					m_originScrooled.setX(etk_avg(0, m_originScrooled.x(), (m_maxSize.x() - m_size.x()*m_limitScrolling)));
					m_originScrooled.setY(etk_avg(0, m_originScrooled.y(), (m_maxSize.y() - m_size.y()*m_limitScrolling)));
					m_fingerMoveStartPos[idTable] = relativePos;
					EWOL_DEBUG("SCROOL  == > MOVE m_originScrooled=" << m_originScrooled << " " << relativePos << " " << m_highSpeedStartPos);
					markToRedraw();
				}
				if (    m_fingerPresent[0] == false
				     && m_fingerPresent[1] == false) {
					if (_event.getStatus() == ewol::key::statusUp) {
						// TODO : Reset event ...
						m_fingerScoolActivated = false;
						_event.reset();
					}
				}
				return true;
			}
			#if 0 // old mode of first finger move...
			if (_event.getId() == 1) {
				EWOL_VERBOSE("event 1  " << _event);
				if (_event.getStatus() == ewol::key::statusDown) {
					m_highSpeedMode = ewol::widget::Scroll::speedModeInit;
					m_highSpeedType = ewol::key::typeFinger;
					m_highSpeedStartPos.setValue(relativePos.x(), relativePos.y());
					EWOL_DEBUG("SCROOL  == > INIT");
					return true;
				} else if (_event.getStatus() == ewol::key::statusUp) {
					m_highSpeedMode = ewol::widget::Scroll::speedModeDisable;
					m_highSpeedType = ewol::key::typeUnknow;
					EWOL_DEBUG("SCROOL  == > DISABLE");
					markToRedraw();
					return true;
				} else if (    m_highSpeedMode == ewol::widget::Scroll::speedModeInit
				            && _event.getStatus() == ewol::key::statusMove) {
					// wait that the cursor move more than 10 px to enable it :
					if(    abs(relativePos.x() - m_highSpeedStartPos.x()) > 10 
					    || abs(relativePos.y() - m_highSpeedStartPos.y()) > 10 ) {
						// the scrooling can start : 
						// select the direction :
						m_highSpeedMode = ewol::widget::Scroll::speedModeEnableFinger;
						EWOL_DEBUG("SCROOL  == > ENABLE");
						markToRedraw();
					}
					return true;
				} if (    m_highSpeedMode == ewol::widget::Scroll::speedModeEnableFinger
				       && _event.getStatus() == ewol::key::statusMove) {
					//m_originScrooled.x = (int32_t)(m_maxSize.x * x / m_size.x);
					m_originScrooled.setX(m_originScrooled.x() - (relativePos.x() - m_highSpeedStartPos.x()));
					m_originScrooled.setY(m_originScrooled.y() - (relativePos.y() - m_highSpeedStartPos.y()));
					m_originScrooled.setX(etk_avg(0, m_originScrooled.x(), (m_maxSize.x() - m_size.x()*m_limitScrolling)));
					m_originScrooled.setY(etk_avg(0, m_originScrooled.y(), (m_maxSize.y() - m_size.y()*m_limitScrolling)));
					m_highSpeedStartPos.setValue(relativePos.x(), relativePos.y());
					EWOL_DEBUG("SCROOL  == > MOVE m_originScrooled=" << m_originScrooled << " " << relativePos << " " << m_highSpeedStartPos);
					markToRedraw();
					return true;
				}
			} else if (    m_highSpeedMode == ewol::widget::Scroll::speedModeDisable
			            && _event.getStatus() == ewol::key::statusLeave) {
				m_highSpeedMode = ewol::widget::Scroll::speedModeDisable;
				m_highSpeedType = ewol::key::typeUnknow;
				EWOL_DEBUG("SCROOL  == > DISABLE");
				markToRedraw();
				return true;
			}
			#endif
		}
	} else if (m_scroollingMode == scroolModeCenter) {
		if (_event.getType() == ewol::key::typeMouse) {
			float tmp1=m_size.x() / m_maxSize.y();
			float tmp2=m_size.y() / m_maxSize.x();
			//EWOL_INFO(" elements Zoom : " << tmp1 << " " << tmp2);
			tmp1 = etk_min(tmp1, tmp2);
			if (    _event.getId() == 4
			     && _event.getStatus() == ewol::key::statusUp) {
				m_zoom -= 0.1;
				if (tmp1 < 1.0) {
					m_zoom = etk_max(tmp1, m_zoom);
				} else {
					m_zoom = etk_max(1.0, m_zoom);
				}
				markToRedraw();
				return true;
			} else if (    _event.getId() == 5
			            && _event.getStatus() == ewol::key::statusUp) {
				m_zoom += 0.1;
				if (tmp1 > 1.0) {
					m_zoom = etk_min(tmp1, m_zoom);
				} else {
					m_zoom = etk_min(1.0, m_zoom);
				}
				markToRedraw();
				return true;
			}
		}
	} else if (m_scroollingMode == scroolModeGame) {
		
	} else {
		EWOL_ERROR("Scrolling mode unknow ... " << m_scroollingMode );
	}
	return false;
}


void ewol::widget::WidgetScrooled::onDraw(void) {
	m_drawing.draw();
}

void ewol::widget::WidgetScrooled::systemDraw(const ewol::DrawProperty& _displayProp) {
	ewol::openGL::push();
	if (m_scroollingMode == scroolModeCenter) {
		// here we invert the reference of the standard openGl view because the reference in the common display is Top left and not buttom left
		glViewport( m_origin.x(),
		            m_origin.y(),
		            m_size.x(),
		            m_size.y());
		mat4 tmpProjection = etk::matOrtho(-m_size.x()/2, m_size.x()/2, -m_size.y()/2, m_size.y()/2, -1, 1);
		mat4 tmpScale = etk::matScale(vec3(m_zoom, m_zoom, 1.0) );
		mat4 tmpTranslate = etk::matTranslate(vec3(-m_maxSize.x()/2, -m_maxSize.y()/2, -1.0) );
		mat4 tmpMat = tmpProjection * tmpScale * tmpTranslate;
		// set internal matrix system :
		ewol::openGL::setMatrix(tmpMat);
		// Call the widget drawing methode
		onDraw();
	} if (m_scroollingMode == scroolModeGame) {
		// here we invert the reference of the standard openGl view because the reference in the common display is Top left and not buttom left
		glViewport( m_origin.x(),
		            m_origin.y(),
		            m_size.x(),
		            m_size.y());
		
		mat4 tmpProjection = etk::matOrtho(-m_size.x()/2, m_size.x()/2, -m_size.y()/2, m_size.y()/2, -1, 1);
		mat4 tmpTranslate = etk::matTranslate(vec3( -m_maxSize.x()/2, -m_maxSize.y()/2, -1.0) );
		mat4 tmpMat = tmpProjection * tmpTranslate;
		// set internal matrix system :
		ewol::openGL::setMatrix(tmpMat);
		// Call the widget drawing methode
		onDraw();
	} else {
		ewol::Widget::systemDraw(_displayProp);
	}
	ewol::openGL::pop();
}

void ewol::widget::WidgetScrooled::setScrollingPositionDynamic(vec2 _borderWidth, const vec2& _currentPosition, bool _center) {
	if (true == _center) {
		_borderWidth.setValue(m_size.x() / 2 - _borderWidth.x(),
		                      m_size.y() / 2 - _borderWidth.y() );
	}
	// check scrooling in X
	if (_currentPosition.x() < (m_originScrooled.x() + _borderWidth.x()) ) {
		m_originScrooled.setX(_currentPosition.x() - _borderWidth.x());
		m_originScrooled.setX(etk_max(0.0, m_originScrooled.x()));
	} else if (_currentPosition.x() > (m_originScrooled.x()+m_size.x()-2*_borderWidth.x()) ) {
		m_originScrooled.setX(_currentPosition.x() - m_size.x() + 2*_borderWidth.x());
		m_originScrooled.setX(etk_max(0.0, m_originScrooled.x()));
	}
	// check scrooling in Y
	if (_currentPosition.y() < (m_originScrooled.y() + _borderWidth.y()) ) {
		m_originScrooled.setY(_currentPosition.y() - _borderWidth.y());
		m_originScrooled.setY(etk_max(0.0, m_originScrooled.y()));
	} else if (_currentPosition.y() > (m_originScrooled.y()+m_size.y()-2*_borderWidth.y()) ) {
		m_originScrooled.setY(_currentPosition.y() - m_size.y() + 2*_borderWidth.y());
		m_originScrooled.setY(etk_max(0.0, m_originScrooled.y()));
	}
}

void ewol::widget::WidgetScrooled::scroolingMode(enum scrollingMode _newMode) {
	m_scroollingMode = _newMode;
	if (m_scroollingMode == scroolModeGame) {
		// set the scene maximum size :
		m_maxSize.setValue(etk_max(m_size.x(), m_size.y()),
		                   m_maxSize.x());
		m_zoom = 1;
	}
}

