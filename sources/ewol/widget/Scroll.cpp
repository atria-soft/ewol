/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Scroll.h>
#include <ewol/ewol.h>
#include <ewol/widget/Manager.h>
#include <ewol/debug.h>

#undef __class__
#define __class__ "Scroll"

static ewol::Widget* create() {
	return new ewol::widget::Scroll();
}

void ewol::widget::Scroll::init(ewol::widget::Manager& _widgetManager) {
	_widgetManager.addWidgetCreator(__class__,&create);
}

const char* const ewol::widget::Scroll::configLimit = "limit";

ewol::widget::Scroll::Scroll(const std::string& _shaperName) :
  m_shaperH(_shaperName),
  m_shaperV(_shaperName),
  m_limit(0.15,0.5),
  m_pixelScrolling(20),
  m_highSpeedStartPos(0,0),
  m_highSpeedMode(speedModeDisable),
  m_highSpeedButton(-1),
  m_highSpeedType(ewol::key::typeUnknow) {
	addObjectType("ewol::widget::Scroll");
	registerConfig(configLimit, "vec2", nullptr, "Limit the scroll maximum position [0..1]% represent the free space in the scoll when arrive at the end");
}

ewol::widget::Scroll::~Scroll() {
	
}

void ewol::widget::Scroll::setLimit(const vec2& _limit) {
	m_limit = _limit;
	markToRedraw();
}

#define SCROLL_BAR_SPACE      (15)

void ewol::widget::Scroll::calculateMinMaxSize() {
	// call main class !! and not containter class ...
	ewol::Widget::calculateMinMaxSize();
	// call sub classes
	if (nullptr!=m_subWidget) {
		m_subWidget->calculateMinMaxSize();
	}
}

void ewol::widget::Scroll::systemDraw(const ewol::DrawProperty& _displayProp) {
	if (m_hide == true) {
		return;
	}
	if (nullptr!=m_subWidget) {
		ewol::DrawProperty prop = _displayProp;
		prop.limit(m_origin, m_size);
		m_subWidget->systemDraw(prop);
	}
	ewol::Widget::systemDraw(_displayProp);
}

void ewol::widget::Scroll::onDraw() {
	m_shaperH.draw();
	m_shaperV.draw();
}

void ewol::widget::Scroll::onRegenerateDisplay() {
	// call upper class
	ewol::widget::Container::onRegenerateDisplay();
	if (needRedraw() == false) {
		return;
	}
	// clear all previous display
	m_shaperH.clear();
	m_shaperV.clear();
	ewol::Padding paddingVert = m_shaperV.getPadding();
	ewol::Padding paddingHori = m_shaperH.getPadding();
	vec2 scrollOffset(0,0);
	vec2 scrollSize(0,0);
	if (nullptr!=m_subWidget) {
		scrollOffset = m_subWidget->getOffset();
		scrollSize = m_subWidget->getSize();
	}
	if(    m_size.y() < scrollSize.y()
	    || scrollOffset.y()!=0) {
		float lenScrollBar = m_size.y()*m_size.y() / scrollSize.y();
		lenScrollBar = etk_avg(10, lenScrollBar, m_size.y());
		float originScrollBar = scrollOffset.y() / (scrollSize.y()-m_size.y()*m_limit.y());
		originScrollBar = etk_avg(0.0, originScrollBar, 1.0);
		originScrollBar *= (m_size.y()-lenScrollBar);
		m_shaperV.setShape(vec2(m_size.x() - paddingVert.x(), 0),
		                   vec2(paddingVert.x(), m_size.y()),
		                   vec2(m_size.x() - paddingVert.xRight(), m_size.y() - originScrollBar - lenScrollBar),
		                   vec2(0, lenScrollBar));
	}
	if(    m_size.x() < scrollSize.x()
	    || scrollOffset.x()!=0) {
		float lenScrollBar = (m_size.x()-paddingHori.xLeft())*(m_size.x()-paddingVert.x()) / scrollSize.x();
		lenScrollBar = etk_avg(10, lenScrollBar, (m_size.x()-paddingVert.x()));
		float originScrollBar = scrollOffset.x() / (scrollSize.x()-m_size.x()*m_limit.x());
		originScrollBar = etk_avg(0.0, originScrollBar, 1.0);
		originScrollBar *= (m_size.x()-paddingHori.xRight()-lenScrollBar);
		m_shaperH.setShape(vec2(0, 0),
		                   vec2(m_size.x()-paddingVert.x(), paddingHori.y()),
		                   vec2(originScrollBar, paddingHori.yButtom()),
		                   vec2(lenScrollBar, 0));
	}
}

bool ewol::widget::Scroll::onEventInput(const ewol::event::Input& _event) {
	//ewol::event::Input _event = event;
	//_event.setType(ewol::key::typeFinger);
	vec2 relativePos = relativePosition(_event.getPos());
	vec2 scrollOffset(0,0);
	vec2 scrollSize(0,0);
	if (nullptr!=m_subWidget) {
		scrollOffset = m_subWidget->getOffset();
		scrollSize = m_subWidget->getSize();
	}
	EWOL_VERBOSE("Get Event on scroll : " << _event);
	relativePos.setY(m_size.y() - relativePos.y());
	if(    _event.getType() == ewol::key::typeMouse
	    && (    ewol::key::typeUnknow == m_highSpeedType
	         || ewol::key::typeMouse == m_highSpeedType ) ) {
		if(    _event.getId() == 1
		    && _event.getStatus() == ewol::key::statusDown) {
			// check if selected the scrolling position whth the scrolling bar ...
			if (relativePos.x() >= (m_size.x()-SCROLL_BAR_SPACE)) {
				if(    m_size.y() < scrollSize.y()
				    || scrollOffset.y() != 0) {
					m_highSpeedMode = speedModeEnableVertical;
					m_highSpeedType = ewol::key::typeMouse;
					m_highSpeedStartPos.setX(relativePos.x());
					m_highSpeedStartPos.setY(scrollOffset.y() / scrollSize.y() * (m_size.y()-SCROLL_BAR_SPACE*2));
					m_highSpeedButton = 1;
					// force direct scrolling in this case
					scrollOffset.setY((int32_t)(scrollSize.y() * (relativePos.y()-SCROLL_BAR_SPACE) / (m_size.y()-SCROLL_BAR_SPACE*2)));
					scrollOffset.setY(etk_avg(0, scrollOffset.y(), (scrollSize.y() - m_size.y()*m_limit.y())));
					markToRedraw();
					if (nullptr!=m_subWidget) {
						m_subWidget->setOffset(scrollOffset);
					}
					return true;
				}
			} else if (relativePos.y() >= (m_size.y()-SCROLL_BAR_SPACE)) {
				if(    m_size.x() < scrollSize.x()
				    || scrollOffset.x()!=0) {
					m_highSpeedMode = speedModeEnableHorizontal;
					m_highSpeedType = ewol::key::typeMouse;
					m_highSpeedStartPos.setX(scrollOffset.x() / scrollSize.x() * (m_size.x()-SCROLL_BAR_SPACE*2));
					m_highSpeedStartPos.setY(relativePos.y());
					m_highSpeedButton = 1;
					// force direct scrolling in this case
					scrollOffset.setX((int32_t)(scrollSize.x() * (relativePos.x()-SCROLL_BAR_SPACE) / (m_size.x()-SCROLL_BAR_SPACE*2)));
					scrollOffset.setY(etk_avg(0, scrollOffset.x(), (scrollSize.x() - m_size.x()*m_limit.x())));
					markToRedraw();
					if (nullptr!=m_subWidget) {
						m_subWidget->setOffset(scrollOffset);
					}
					return true;
				}
			}
			return false;
		} else if(    _event.getId() == 4
		           && _event.getStatus() == ewol::key::statusUp) {
			if(m_size.y() < scrollSize.y()) {
				scrollOffset.setY(scrollOffset.y()-m_pixelScrolling);
				scrollOffset.setY(etk_avg(0, scrollOffset.y(), (scrollSize.y() - m_size.y()*m_limit.y())));
				markToRedraw();
				if (nullptr!=m_subWidget) {
					m_subWidget->setOffset(scrollOffset);
				}
				return true;
			}
		} else if(    _event.getId() == 5
		           && _event.getStatus() == ewol::key::statusUp) {
			if(m_size.y() < scrollSize.y()) {
				scrollOffset.setY(scrollOffset.y()+m_pixelScrolling);
				scrollOffset.setY(etk_avg(0, scrollOffset.y(), (scrollSize.y() - m_size.y()*m_limit.y())));
				markToRedraw();
				if (nullptr!=m_subWidget) {
					m_subWidget->setOffset(scrollOffset);
				}
				return true;
			}
		}else if (_event.getId() == 2) {
			if (_event.getStatus() == ewol::key::statusDown) {
				m_highSpeedMode = speedModeInit;
				m_highSpeedType = ewol::key::typeMouse;
				m_highSpeedStartPos.setValue(relativePos.x(), relativePos.y());
				m_highSpeedButton = 2;
				// not really use...  == > just keep some informations
				return false;
			}
		} else if(    m_highSpeedMode != speedModeDisable
		           && _event.getStatus() == ewol::key::statusLeave) {
			m_highSpeedMode = speedModeDisable;
			m_highSpeedType = ewol::key::typeUnknow;
			markToRedraw();
			return true;
		}
		if (    _event.getId() == m_highSpeedButton
		     && m_highSpeedMode != speedModeDisable) {
			if (_event.getStatus() == ewol::key::statusUp) {
				if (m_highSpeedMode == speedModeInit) {
					// TODO : generate back the down event ...
					m_highSpeedMode = speedModeDisable;
					m_highSpeedType = ewol::key::typeUnknow;
					return false;
				} else {
					m_highSpeedMode = speedModeGrepEndEvent;
					markToRedraw();
					return true;
				}
			} else if (m_highSpeedMode == speedModeGrepEndEvent) {
				if (_event.getStatus() == ewol::key::statusSingle) {
					m_highSpeedMode = speedModeDisable;
					m_highSpeedType = ewol::key::typeUnknow;
					m_highSpeedButton = -1;
					markToRedraw();
				}
				return true;
			} else if(    m_highSpeedMode == speedModeInit
			           && _event.getStatus() == ewol::key::statusMove) {
				// wait that the cursor move more than 10 px to enable it :
				if(    abs(relativePos.x() - m_highSpeedStartPos.x()) > 10 
				    || abs(relativePos.y() - m_highSpeedStartPos.y()) > 10 ) {
					// the scrooling can start : 
					// select the direction :
					if (relativePos.x() == m_highSpeedStartPos.x()) {
						m_highSpeedMode = speedModeEnableVertical;
					} else if (relativePos.y() == m_highSpeedStartPos.y()) {
						m_highSpeedMode = speedModeEnableHorizontal;
					} else {
						float coef = (relativePos.y() - m_highSpeedStartPos.y()) / (relativePos.x() - m_highSpeedStartPos.x());
						if (abs(coef) <= 1 ) {
							m_highSpeedMode = speedModeEnableHorizontal;
						} else {
							m_highSpeedMode = speedModeEnableVertical;
						}
					}
					if (m_highSpeedMode == speedModeEnableHorizontal) {
						m_highSpeedStartPos.setX(scrollOffset.x() / scrollSize.x() * (m_size.x()-SCROLL_BAR_SPACE*2));
					} else {
						m_highSpeedStartPos.setY(scrollOffset.y() / scrollSize.y() * (m_size.y()-SCROLL_BAR_SPACE*2));
					}
					markToRedraw();
				}
				scrollOffset.setY(etk_avg(0, scrollOffset.y(), (scrollSize.y() - m_size.y()*m_limit.y())));
				if (nullptr!=m_subWidget) {
					m_subWidget->setOffset(scrollOffset);
				}
				return true;
			}
			if(    m_highSpeedMode == speedModeEnableHorizontal
			    && _event.getStatus() == ewol::key::statusMove) {
				scrollOffset.setX((int32_t)(scrollSize.x() * (relativePos.x()-SCROLL_BAR_SPACE) / (m_size.x()-SCROLL_BAR_SPACE*2)));
				scrollOffset.setX(etk_avg(0, scrollOffset.x(), (scrollSize.x() - m_size.x()*m_limit.x() )));
				markToRedraw();
				if (nullptr!=m_subWidget) {
					m_subWidget->setOffset(scrollOffset);
				}
				return true;
			}
			if(    m_highSpeedMode == speedModeEnableVertical
			    && _event.getStatus() == ewol::key::statusMove) {
				scrollOffset.setY((int32_t)(scrollSize.y() * (relativePos.y()-SCROLL_BAR_SPACE) / (m_size.y()-SCROLL_BAR_SPACE*2)));
				scrollOffset.setY(etk_avg(0, scrollOffset.y(), (scrollSize.y() - m_size.y()*m_limit.x())));
				markToRedraw();
				if (nullptr!=m_subWidget) {
					m_subWidget->setOffset(scrollOffset);
				}
				return true;
			}
		}
	} else if(    ewol::key::typeFinger == _event.getType()
	           && (    ewol::key::typeUnknow == m_highSpeedType
	                || ewol::key::typeFinger == m_highSpeedType ) ) {
		if (1 == _event.getId()) {
			EWOL_VERBOSE("event: " << _event);
			if (ewol::key::statusDown == _event.getStatus()) {
				m_highSpeedMode = speedModeInit;
				m_highSpeedType = ewol::key::typeFinger;
				m_highSpeedStartPos.setValue(relativePos.x(), relativePos.y());
				EWOL_VERBOSE("SCROOL  == > INIT pos=" << m_highSpeedStartPos << " && curent scrollOffset=" << scrollOffset);
				return true;
			} else if (ewol::key::statusUpAfter == _event.getStatus()) {
				m_highSpeedMode = speedModeDisable;
				m_highSpeedType = ewol::key::typeUnknow;
				EWOL_VERBOSE("SCROOL  == > DISABLE");
				markToRedraw();
				return true;
			} else if (    m_highSpeedMode == speedModeInit
			            && ewol::key::statusMove == _event.getStatus()) {
				// wait that the cursor move more than 10 px to enable it :
				if(    abs(relativePos.x() - m_highSpeedStartPos.x()) > 10 
				    || abs(relativePos.y() - m_highSpeedStartPos.y()) > 10 ) {
					// the scrooling can start : 
					// select the direction :
					m_highSpeedMode = speedModeEnableFinger;
					EWOL_VERBOSE("SCROOL  == > ENABLE");
					markToRedraw();
				}
				return true;
			}
			if (    m_highSpeedMode == speedModeEnableFinger
			     && ewol::key::statusMove == _event.getStatus()) {
				EWOL_VERBOSE("SCROOL  == > INIT scrollOffset=" << scrollOffset.y() << " relativePos=" << relativePos.y() << " m_highSpeedStartPos=" << m_highSpeedStartPos.y());
				//scrollOffset.x = (int32_t)(scrollSize.x * x / m_size.x);
				if (m_limit.x() != 0.0f) {
					scrollOffset.setX(scrollOffset.x() + (relativePos.x() - m_highSpeedStartPos.x()));
					scrollOffset.setX(etk_avg(0, scrollOffset.x(), (scrollSize.x() - m_size.x()*m_limit.x())));
				}
				if (m_limit.y() != 0.0f) {
					scrollOffset.setY(scrollOffset.y() - (relativePos.y() - m_highSpeedStartPos.y()));
					scrollOffset.setY(etk_avg(0, scrollOffset.y(), (scrollSize.y() - m_size.y()*m_limit.y())));
				}
				// update current position:
				m_highSpeedStartPos = relativePos;
				EWOL_VERBOSE("SCROOL  == > MOVE " << scrollOffset);
				markToRedraw();
				if (nullptr!=m_subWidget) {
					m_subWidget->setOffset(scrollOffset);
				}
				return true;
			}
			if (m_highSpeedMode == speedModeEnableFinger) {
				return true;
			}
		} else if (    m_highSpeedMode != speedModeDisable
		            && ewol::key::statusLeave == _event.getStatus()) {
			m_highSpeedMode = speedModeDisable;
			m_highSpeedType = ewol::key::typeUnknow;
			EWOL_VERBOSE("SCROOL  == > DISABLE");
			markToRedraw();
			return true;
		}
	}
	return false;
}

ewol::object::Shared<ewol::Widget> ewol::widget::Scroll::getWidgetAtPos(const vec2& _pos) {
	ewol::object::Shared<ewol::Widget> tmpWidget = ewol::widget::Container::getWidgetAtPos(_pos);
	if (nullptr != tmpWidget) {
		return tmpWidget;
	}
	return this;
}

bool ewol::widget::Scroll::onSetConfig(const ewol::object::Config& _conf) {
	if (true == ewol::widget::Container::onSetConfig(_conf)) {
		return true;
	}
	if (_conf.getConfig() == configLimit) {
		setLimit(_conf.getData());
		return true;
	}
	return false;
}

bool ewol::widget::Scroll::onGetConfig(const char* _config, std::string& _result) const {
	if (true == ewol::widget::Container::onGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configLimit) {
		_result = getLimit();
		return true;
	}
	return false;
}


