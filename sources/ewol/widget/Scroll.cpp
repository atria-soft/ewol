/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Scroll.h>
#include <ewol/ewol.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/debug.h>

#undef __class__
#define __class__	"Scroll"

static ewol::Widget* create(void)
{
	return new widget::Scroll();
}

void widget::Scroll::init(ewol::WidgetManager& _widgetManager)
{
	_widgetManager.addWidgetCreator(__class__,&create);
}

const char* const widget::Scroll::configLimit = "limit";

widget::Scroll::Scroll(void) :
	m_limit(0.15,0.5),
	m_pixelScrolling(20),
	m_highSpeedStartPos(0,0),
	m_highSpeedMode(SCROLL_DISABLE),
	m_highSpeedButton(-1),
	m_highSpeedType(ewol::keyEvent::typeUnknow)
{
	registerConfig(configLimit, "vec2", NULL, "Limit the scroll maximum position [0..1]% represent the free space in the scoll when arrive at the end");
	
}

widget::Scroll::~Scroll(void)
{
	
}

void widget::Scroll::setLimit(const vec2& _limit)
{
	m_limit = _limit;
	markToRedraw();
}

#define SCROLL_BAR_SPACE      (15)

void widget::Scroll::calculateMinMaxSize(void)
{
	// call main class !! and not containter class ...
	ewol::Widget::calculateMinMaxSize();
	// call sub classes
	if (NULL!=m_subWidget) {
		m_subWidget->calculateMinMaxSize();
	}
}

void widget::Scroll::systemDraw(const ewol::drawProperty& _displayProp)
{
	if (m_hide == true) {
		return;
	}
	if (NULL!=m_subWidget) {
		ewol::drawProperty prop = _displayProp;
		prop.limit(m_origin, m_size);
		m_subWidget->systemDraw(prop);
	}
	ewol::Widget::systemDraw(_displayProp);
}

void widget::Scroll::onDraw(void)
{
	m_draw.draw();
}

void widget::Scroll::onRegenerateDisplay(void)
{
	// call upper class
	widget::Container::onRegenerateDisplay();
	if (true == needRedraw()) {
		// clear all previous display
		m_draw.clear();
		
		m_draw.setColor(0xFF00007F);
		
		vec2 scrollOffset(0,0);
		vec2 scrollSize(0,0);
		if (NULL!=m_subWidget) {
			scrollOffset = m_subWidget->getOffset();
			scrollSize = m_subWidget->getSize();
		}
		m_draw.setThickness(1);
		if(    m_size.y() < scrollSize.y()
		    || scrollOffset.y()!=0) {
			//EWOL_DEBUG("plop : "  << vec2(m_size.x()-(SCROLL_BAR_SPACE/2), 0) << "  " << vec2(m_size.x()-(SCROLL_BAR_SPACE/2), m_size.y()));
			m_draw.setPos(vec2(m_size.x()-(SCROLL_BAR_SPACE/2), 0) );
			m_draw.lineTo(vec2(m_size.x()-(SCROLL_BAR_SPACE/2), m_size.y()) );
			float lenScrollBar = m_size.y()*m_size.y() / scrollSize.y();
			lenScrollBar = etk_avg(10, lenScrollBar, m_size.y());
			float originScrollBar = scrollOffset.y() / (scrollSize.y()-m_size.y()*m_limit.y());
			originScrollBar = etk_avg(0.0, originScrollBar, 1.0);
			originScrollBar *= (m_size.y()-lenScrollBar);
			m_draw.setPos(vec2(m_size.x()-SCROLL_BAR_SPACE, m_size.y() - originScrollBar - lenScrollBar) );
			m_draw.rectangleWidth(vec2(SCROLL_BAR_SPACE, lenScrollBar));
		}
		if(    m_size.x() < scrollSize.x()
		    || scrollOffset.x()!=0) {
			m_draw.setPos(vec2(0, (SCROLL_BAR_SPACE/2)) );
			m_draw.lineTo(vec2(m_size.x()-SCROLL_BAR_SPACE, (SCROLL_BAR_SPACE/2)) );
			float lenScrollBar = m_size.x()*(m_size.x()-SCROLL_BAR_SPACE) / scrollSize.x();
			lenScrollBar = etk_avg(10, lenScrollBar, (m_size.x()-SCROLL_BAR_SPACE));
			float originScrollBar = scrollOffset.x() / (scrollSize.x()-m_size.x()*m_limit.x());
			originScrollBar = etk_avg(0.0, originScrollBar, 1.0);
			originScrollBar *= (m_size.x()-SCROLL_BAR_SPACE-lenScrollBar);
			m_draw.setPos(vec2(originScrollBar, 0) );
			m_draw.rectangle(vec2(lenScrollBar, SCROLL_BAR_SPACE) );
		}
	}
}


bool widget::Scroll::onEventInput(const ewol::EventInput& _event)
{
	vec2 relativePos = relativePosition(_event.getPos());
	vec2 scrollOffset(0,0);
	vec2 scrollSize(0,0);
	if (NULL!=m_subWidget) {
		scrollOffset = m_subWidget->getOffset();
		scrollSize = m_subWidget->getSize();
	}
	relativePos.setY(m_size.y() - relativePos.y());
	if(    _event.getType() == ewol::keyEvent::typeMouse
	    && (    ewol::keyEvent::typeUnknow == m_highSpeedType
	         || ewol::keyEvent::typeMouse == m_highSpeedType ) ) {
		if(    _event.getId() == 1
		    && _event.getStatus() == ewol::keyEvent::statusDown) {
			// check if selected the scrolling position whth the scrolling bar ...
			if (relativePos.x() >= (m_size.x()-SCROLL_BAR_SPACE)) {
				if(    m_size.y() < scrollSize.y()
				    || scrollOffset.y() != 0) {
					m_highSpeedMode = widget::SCROLL_ENABLE_VERTICAL;
					m_highSpeedType = ewol::keyEvent::typeMouse;
					m_highSpeedStartPos.setX(relativePos.x());
					m_highSpeedStartPos.setY(scrollOffset.y() / scrollSize.y() * (m_size.y()-SCROLL_BAR_SPACE*2));
					m_highSpeedButton = 1;
					// force direct scrolling in this case
					scrollOffset.setY((int32_t)(scrollSize.y() * (relativePos.y()-SCROLL_BAR_SPACE) / (m_size.y()-SCROLL_BAR_SPACE*2)));
					scrollOffset.setY(etk_avg(0, scrollOffset.y(), (scrollSize.y() - m_size.y()*m_limit.y())));
					markToRedraw();
					if (NULL!=m_subWidget) {
						m_subWidget->setOffset(scrollOffset);
					}
					return true;
				}
			} else if (relativePos.y() >= (m_size.y()-SCROLL_BAR_SPACE)) {
				if(    m_size.x() < scrollSize.x()
				    || scrollOffset.x()!=0) {
					m_highSpeedMode = widget::SCROLL_ENABLE_HORIZONTAL;
					m_highSpeedType = ewol::keyEvent::typeMouse;
					m_highSpeedStartPos.setX(scrollOffset.x() / scrollSize.x() * (m_size.x()-SCROLL_BAR_SPACE*2));
					m_highSpeedStartPos.setY(relativePos.y());
					m_highSpeedButton = 1;
					// force direct scrolling in this case
					scrollOffset.setX((int32_t)(scrollSize.x() * (relativePos.x()-SCROLL_BAR_SPACE) / (m_size.x()-SCROLL_BAR_SPACE*2)));
					scrollOffset.setY(etk_avg(0, scrollOffset.x(), (scrollSize.x() - m_size.x()*m_limit.x())));
					markToRedraw();
					if (NULL!=m_subWidget) {
						m_subWidget->setOffset(scrollOffset);
					}
					return true;
				}
			}
			return false;
		} else if(    _event.getId() == 4
		           && _event.getStatus() == ewol::keyEvent::statusUp) {
			if(m_size.y() < scrollSize.y()) {
				scrollOffset.setY(scrollOffset.y()-m_pixelScrolling);
				scrollOffset.setY(etk_avg(0, scrollOffset.y(), (scrollSize.y() - m_size.y()*m_limit.y())));
				markToRedraw();
				if (NULL!=m_subWidget) {
					m_subWidget->setOffset(scrollOffset);
				}
				return true;
			}
		} else if(    _event.getId() == 5
		           && _event.getStatus() == ewol::keyEvent::statusUp) {
			if(m_size.y() < scrollSize.y()) {
				scrollOffset.setY(scrollOffset.y()+m_pixelScrolling);
				scrollOffset.setY(etk_avg(0, scrollOffset.y(), (scrollSize.y() - m_size.y()*m_limit.y())));
				markToRedraw();
				if (NULL!=m_subWidget) {
					m_subWidget->setOffset(scrollOffset);
				}
				return true;
			}
		}else if (_event.getId() == 2) {
			if (_event.getStatus() == ewol::keyEvent::statusDown) {
				m_highSpeedMode = widget::SCROLL_INIT;
				m_highSpeedType = ewol::keyEvent::typeMouse;
				m_highSpeedStartPos.setValue(relativePos.x(), relativePos.y());
				m_highSpeedButton = 2;
				// not really use...  == > just keep some informations
				return false;
			}
		} else if(    widget::SCROLL_DISABLE!=m_highSpeedMode
		           && _event.getStatus() == ewol::keyEvent::statusLeave) {
			m_highSpeedMode = widget::SCROLL_DISABLE;
			m_highSpeedType = ewol::keyEvent::typeUnknow;
			markToRedraw();
			return true;
		}
		if (_event.getId() == m_highSpeedButton && widget::SCROLL_DISABLE!=m_highSpeedMode) {
			if (_event.getStatus() == ewol::keyEvent::statusUp) {
				if (widget::SCROLL_INIT == m_highSpeedMode) {
					// TODO : generate back the down event ...
					m_highSpeedMode = widget::SCROLL_DISABLE;
					m_highSpeedType = ewol::keyEvent::typeUnknow;
					return false;
				} else {
					m_highSpeedMode = widget::SCROLL_GREP_END_EVENT;
					markToRedraw();
					return true;
				}
			} else if (widget::SCROLL_GREP_END_EVENT == m_highSpeedMode) {
				if (_event.getStatus() == ewol::keyEvent::statusSingle) {
					m_highSpeedMode = widget::SCROLL_DISABLE;
					m_highSpeedType = ewol::keyEvent::typeUnknow;
					m_highSpeedButton = -1;
					markToRedraw();
				}
				return true;
			} else if(    widget::SCROLL_INIT == m_highSpeedMode
			           && _event.getStatus() == ewol::keyEvent::statusMove) {
				// wait that the cursor move more than 10 px to enable it :
				if(    abs(relativePos.x() - m_highSpeedStartPos.x()) > 10 
				    || abs(relativePos.y() - m_highSpeedStartPos.y()) > 10 ) {
					// the scrooling can start : 
					// select the direction :
					if (relativePos.x() == m_highSpeedStartPos.x()) {
						m_highSpeedMode = widget::SCROLL_ENABLE_VERTICAL;
					} else if (relativePos.y() == m_highSpeedStartPos.y()) {
						m_highSpeedMode = widget::SCROLL_ENABLE_HORIZONTAL;
					} else {
						float coef = (relativePos.y() - m_highSpeedStartPos.y()) / (relativePos.x() - m_highSpeedStartPos.x());
						if (abs(coef) <= 1 ) {
							m_highSpeedMode = widget::SCROLL_ENABLE_HORIZONTAL;
						} else {
							m_highSpeedMode = widget::SCROLL_ENABLE_VERTICAL;
						}
					}
					if (m_highSpeedMode == widget::SCROLL_ENABLE_HORIZONTAL) {
						m_highSpeedStartPos.setX(scrollOffset.x() / scrollSize.x() * (m_size.x()-SCROLL_BAR_SPACE*2));
					} else {
						m_highSpeedStartPos.setY(scrollOffset.y() / scrollSize.y() * (m_size.y()-SCROLL_BAR_SPACE*2));
					}
					markToRedraw();
				}
				scrollOffset.setY(etk_avg(0, scrollOffset.y(), (scrollSize.y() - m_size.y()*m_limit.y())));
				if (NULL!=m_subWidget) {
					m_subWidget->setOffset(scrollOffset);
				}
				return true;
			}
			if(    widget::SCROLL_ENABLE_HORIZONTAL == m_highSpeedMode
			    && _event.getStatus() == ewol::keyEvent::statusMove) {
				scrollOffset.setX((int32_t)(scrollSize.x() * (relativePos.x()-SCROLL_BAR_SPACE) / (m_size.x()-SCROLL_BAR_SPACE*2)));
				scrollOffset.setX(etk_avg(0, scrollOffset.x(), (scrollSize.x() - m_size.x()*m_limit.x() )));
				markToRedraw();
				if (NULL!=m_subWidget) {
					m_subWidget->setOffset(scrollOffset);
				}
				return true;
			}
			if(    widget::SCROLL_ENABLE_VERTICAL == m_highSpeedMode
			    && _event.getStatus() == ewol::keyEvent::statusMove) {
				scrollOffset.setY((int32_t)(scrollSize.y() * (relativePos.y()-SCROLL_BAR_SPACE) / (m_size.y()-SCROLL_BAR_SPACE*2)));
				scrollOffset.setY(etk_avg(0, scrollOffset.y(), (scrollSize.y() - m_size.y()*m_limit.x())));
				markToRedraw();
				if (NULL!=m_subWidget) {
					m_subWidget->setOffset(scrollOffset);
				}
				return true;
			}
		}
	} else if(    ewol::keyEvent::typeFinger == _event.getType()
	           && (    ewol::keyEvent::typeUnknow == m_highSpeedType
	                || ewol::keyEvent::typeFinger == m_highSpeedType ) ) {
		if (1 == _event.getId()) {
			//EWOL_VERBOSE("event 1  << " << (int32_t)typeEvent << "(" << x << "," << y << ")");
			if (ewol::keyEvent::statusDown == _event.getStatus()) {
				m_highSpeedMode = widget::SCROLL_INIT;
				m_highSpeedType = ewol::keyEvent::typeFinger;
				m_highSpeedStartPos.setValue(relativePos.x(), relativePos.y());
				EWOL_VERBOSE("SCROOL  == > INIT");
				return true;
			} else if (ewol::keyEvent::statusUp == _event.getStatus()) {
				m_highSpeedMode = widget::SCROLL_DISABLE;
				m_highSpeedType = ewol::keyEvent::typeUnknow;
				EWOL_VERBOSE("SCROOL  == > DISABLE");
				markToRedraw();
				return true;
			} else if (widget::SCROLL_INIT == m_highSpeedMode && ewol::keyEvent::statusMove == _event.getStatus()) {
				// wait that the cursor move more than 10 px to enable it :
				if(    abs(relativePos.x() - m_highSpeedStartPos.x()) > 10 
				    || abs(relativePos.y() - m_highSpeedStartPos.y()) > 10 ) {
					// the scrooling can start : 
					// select the direction :
					m_highSpeedMode = widget::SCROLL_ENABLE_FINGER;
					EWOL_VERBOSE("SCROOL  == > ENABLE");
					markToRedraw();
				}
				return true;
			} if (widget::SCROLL_ENABLE_FINGER == m_highSpeedMode && ewol::keyEvent::statusMove == _event.getStatus()) {
				//scrollOffset.x = (int32_t)(scrollSize.x * x / m_size.x);
				scrollOffset.setX(scrollOffset.x() - relativePos.x() - m_highSpeedStartPos.x());
				scrollOffset.setY(scrollOffset.y() - relativePos.y() - m_highSpeedStartPos.y());
				scrollOffset.setX(etk_avg(0, scrollOffset.x(), (scrollSize.x() - m_size.x()*m_limit.x())));
				scrollOffset.setY(etk_avg(0, scrollOffset.y(), (scrollSize.y() - m_size.y()*m_limit.y())));
				m_highSpeedStartPos.setValue(relativePos.x(), relativePos.y());
				EWOL_VERBOSE("SCROOL  == > MOVE (" << scrollOffset.x() << "," << scrollOffset.y() << ")");
				markToRedraw();
				return true;
			}
		} else if (widget::SCROLL_DISABLE!=m_highSpeedMode && ewol::keyEvent::statusLeave == _event.getStatus()) {
			m_highSpeedMode = widget::SCROLL_DISABLE;
			m_highSpeedType = ewol::keyEvent::typeUnknow;
			EWOL_VERBOSE("SCROOL  == > DISABLE");
			markToRedraw();
			return true;
		}
	}
	return false;
}

ewol::Widget* widget::Scroll::getWidgetAtPos(const vec2& _pos)
{
	ewol::Widget* tmpWidget = widget::Container::getWidgetAtPos(_pos);
	if (NULL != tmpWidget) {
		return tmpWidget;
	}
	return this;
}

bool widget::Scroll::onSetConfig(const ewol::EConfig& _conf)
{
	if (true == widget::Container::onSetConfig(_conf)) {
		return true;
	}
	if (_conf.getConfig() == configLimit) {
		setLimit(_conf.getData());
		return true;
	}
	return false;
}

bool widget::Scroll::onGetConfig(const char* _config, etk::UString& _result) const
{
	if (true == widget::Container::onGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configLimit) {
		_result = getLimit();
		return true;
	}
	return false;
}


