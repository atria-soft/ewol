/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/widget/Scroll.h>
#include <ewol/ewol.h>
#include <ewol/widget/Manager.h>
#include <ewol/debug.h>
#include <ewol/compositing/Drawing.h>

ewol::widget::Scroll::Scroll() :
  propertyLimit(this, "limit",
                      vec2(0.15,0.5), vec2(0.0,0.0), vec2(1.0,1.0),
                      "Limit the scroll maximum position [0..1]% represent the free space in the scoll when arrive at the end",
                      &ewol::widget::Scroll::onChangePropertyLimit),
  propertyShapeVert(this, "shape-vert",
                          "{ewol}THEME:GUI:WidgetScrolled.json",
                          "shape for the vertical display",
                          &ewol::widget::Scroll::onChangePropertyShapeVert),
  propertyShapeHori(this, "shape-hori",
                          "{ewol}THEME:GUI:WidgetScrolled.json",
                          "shape for the horizonal display",
                          &ewol::widget::Scroll::onChangePropertyShapeHori),
  propertyHover(this, "hover",
                      true,
                      "the display bar are hover the subWidget"),
  m_pixelScrolling(20),
  m_highSpeedStartPos(0,0),
  m_highSpeedMode(speedModeDisable),
  m_highSpeedButton(-1),
  m_highSpeedType(gale::key::type::unknow) {
	addObjectType("ewol::widget::Scroll");
	// Remove gravity property: (only keep top/buttom)
	propertyGravity.remove("center");
	propertyGravity.remove("top-left");
	//propertyGravity.remove("top");
	propertyGravity.remove("top-right");
	propertyGravity.remove("right");
	propertyGravity.remove("buttom-right");
	//propertyGravity.remove("buttom");
	propertyGravity.remove("buttom-left");
	propertyGravity.remove("left");
}

void ewol::widget::Scroll::init() {
	ewol::widget::Container::init();
	propertyShapeVert.notifyChange();
	propertyShapeHori.notifyChange();
}


ewol::widget::Scroll::~Scroll() {
	
}
// TODO : create a config for this ...
#define SCROLL_BAR_SPACE      (15)

// note: The widget will expand has possible and will control itself the display property
void ewol::widget::Scroll::onChangeSize() {
	// Note: No call of container ==> normal case ...
	ewol::Widget::onChangeSize();
	if (*propertyHide == true) {
		return;
	}
	if (m_subWidget == nullptr) {
		return;
	}
	// remove the bar if hover
	vec2 basicSize = m_size;
	if (*propertyHover == false) {
		basicSize -= vec2(SCROLL_BAR_SPACE,SCROLL_BAR_SPACE);
	}
	
	
	vec2 origin = m_origin+m_offset;
	vec2 minSize = m_subWidget->getCalculateMinSize();
	bvec2 expand = m_subWidget->propertyExpand.get();
	//The gravity is not set on the sub element ==> special use of the widget
	//origin += ewol::gravityGenerateDelta(propertyGravity.get(), minSize - m_size);
	if (    expand.x() == true
	     && minSize.x() < basicSize.x()) {
		minSize.setX(basicSize.x());
	}
	if (    expand.y() == true
	     && minSize.y() < basicSize.y()) {
		minSize.setY(basicSize.y());
	}
	m_subWidget->setSize(minSize);
	if (*propertyGravity == ewol::gravity_top) {
		origin += vec2(0.0f, basicSize.y()-minSize.y());
		if (*propertyHover == false) {
			origin += vec2(0,SCROLL_BAR_SPACE);
		}
	} else if (*propertyGravity == ewol::gravity_buttom) {
		// nothing to do ... origin += 
	} else {
		EWOL_ERROR(" Not manage other gravity ...");
	}
	m_subWidget->setOrigin(origin);
	m_subWidget->onChangeSize();
}

void ewol::widget::Scroll::calculateMinMaxSize() {
	// Note: No call of container ==> normal case ...
	ewol::Widget::calculateMinMaxSize();
	// call sub classes
	if (m_subWidget != nullptr) {
		m_subWidget->calculateMinMaxSize();
	}
}

void ewol::widget::Scroll::systemDraw(const ewol::DrawProperty& _displayProp) {
	if (*propertyHide == true) {
		return;
	}
	if (m_subWidget != nullptr) {
		ewol::DrawProperty prop = _displayProp;
		prop.limit(m_origin, m_size);
		m_subWidget->systemDraw(prop);
	}
	ewol::Widget::systemDraw(_displayProp);
}

void ewol::widget::Scroll::onDraw() {
	m_shaperH.draw();
	m_shaperV.draw();
	/*
	ewol::compositing::Drawing draw;
	draw.setPos(vec2(10,10));
	draw.setColor(etk::color::orange);
	draw.rectangleWidth(vec2(25,25));
	draw.setPos(m_size - vec2(35,35));
	draw.setColor(etk::color::green);
	draw.rectangleWidth(vec2(25,25));
	draw.draw();
	*/
}

void ewol::widget::Scroll::onRegenerateDisplay() {
	if (*propertyHide == true) {
		return;
	}
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
	if (m_subWidget != nullptr) {
		scrollOffset = m_subWidget->getOffset();
		scrollSize = m_subWidget->getSize();
	}
	if(    m_size.y() < scrollSize.y()
	    || scrollOffset.y() != 0) {
		float lenScrollBar = m_size.y()*m_size.y() / scrollSize.y();
		lenScrollBar = std::avg(10.0f, lenScrollBar, m_size.y());
		float originScrollBar = scrollOffset.y() / (scrollSize.y()-m_size.y()*propertyLimit->y());
		originScrollBar = std::avg(0.0f, originScrollBar, 1.0f);
		originScrollBar *= (m_size.y()-lenScrollBar);
		m_shaperV.setShape(vec2(m_size.x() - paddingVert.x(), 0),
		                   vec2(paddingVert.x(), m_size.y()),
		                   vec2(m_size.x() - paddingVert.xRight(), m_size.y() - originScrollBar - lenScrollBar),
		                   vec2(0, lenScrollBar));
	}
	if(    m_size.x() < scrollSize.x()
	    || scrollOffset.x() != 0) {
		float lenScrollBar = (m_size.x()-paddingHori.xLeft())*(m_size.x()-paddingVert.x()) / scrollSize.x();
		lenScrollBar = std::avg(10.0f, lenScrollBar, (m_size.x()-paddingVert.x()));
		float originScrollBar = scrollOffset.x() / (scrollSize.x()-m_size.x()*propertyLimit->x());
		originScrollBar = std::avg(0.0f, originScrollBar, 1.0f);
		originScrollBar *= (m_size.x()-paddingHori.xRight()-lenScrollBar);
		m_shaperH.setShape(vec2(0, 0),
		                   vec2(m_size.x()-paddingVert.x(), paddingHori.y()),
		                   vec2(originScrollBar, paddingHori.yButtom()),
		                   vec2(lenScrollBar, 0));
	}
}

bool ewol::widget::Scroll::onEventInput(const ewol::event::Input& _event) {
	//ewol::event::Input _event = event;
	//_event.setType(gale::key::type::finger);
	vec2 relativePos = relativePosition(_event.getPos());
	vec2 scrollOffset(0,0);
	vec2 scrollSize(0,0);
	if (m_subWidget != nullptr) {
		scrollOffset = m_subWidget->getOffset();
		scrollSize = m_subWidget->getSize();
	}
	EWOL_VERBOSE("Get Event on scroll : " << _event);
	relativePos.setY(m_size.y() - relativePos.y());
	if(    _event.getType() == gale::key::type::mouse
	    && (    m_highSpeedType == gale::key::type::unknow
	         || m_highSpeedType == gale::key::type::mouse) ) {
		if(    _event.getId() == 1
		    && _event.getStatus() == gale::key::status::down) {
			// check if selected the scrolling position whth the scrolling bar ...
			if (relativePos.x() >= (m_size.x()-SCROLL_BAR_SPACE)) {
				if(    m_size.y() < scrollSize.y()
				    || scrollOffset.y() != 0) {
					m_highSpeedMode = speedModeEnableVertical;
					m_highSpeedType = gale::key::type::mouse;
					m_highSpeedStartPos.setX(relativePos.x());
					m_highSpeedStartPos.setY(scrollOffset.y() / scrollSize.y() * (m_size.y()-SCROLL_BAR_SPACE*2));
					m_highSpeedButton = 1;
					// force direct scrolling in this case
					scrollOffset.setY((int32_t)(scrollSize.y() * (relativePos.y()-SCROLL_BAR_SPACE) / (m_size.y()-SCROLL_BAR_SPACE*2)));
					scrollOffset.setY(std::avg(0.0f, scrollOffset.y(), (scrollSize.y() - m_size.y()*propertyLimit->y())));
					markToRedraw();
					if (m_subWidget != nullptr) {
						m_subWidget->setOffset(scrollOffset);
					}
					return true;
				}
			} else if (relativePos.y() >= (m_size.y()-SCROLL_BAR_SPACE)) {
				if(    m_size.x() < scrollSize.x()
				    || scrollOffset.x()!=0) {
					m_highSpeedMode = speedModeEnableHorizontal;
					m_highSpeedType = gale::key::type::mouse;
					m_highSpeedStartPos.setX(scrollOffset.x() / scrollSize.x() * (m_size.x()-SCROLL_BAR_SPACE*2));
					m_highSpeedStartPos.setY(relativePos.y());
					m_highSpeedButton = 1;
					// force direct scrolling in this case
					scrollOffset.setX((int32_t)(scrollSize.x() * (relativePos.x()-SCROLL_BAR_SPACE) / (m_size.x()-SCROLL_BAR_SPACE*2)));
					scrollOffset.setY(std::avg(0.0f, scrollOffset.x(), (scrollSize.x() - m_size.x()*propertyLimit->x())));
					markToRedraw();
					if (m_subWidget != nullptr) {
						m_subWidget->setOffset(scrollOffset);
					}
					return true;
				}
			}
			return false;
		} else if(    _event.getId() == 4
		           && _event.getStatus() == gale::key::status::up) {
			EWOL_VERBOSE("    mode UP " << m_size.y() << "<" << scrollSize.y());
			if(m_size.y() < scrollSize.y()) {
				scrollOffset.setY(scrollOffset.y()-m_pixelScrolling);
				scrollOffset.setY(std::avg(0.0f, scrollOffset.y(), (scrollSize.y() - m_size.y()*propertyLimit->y())));
				markToRedraw();
				if (m_subWidget != nullptr) {
					m_subWidget->setOffset(scrollOffset);
				}
				return true;
			}
		} else if(    _event.getId() == 5
		           && _event.getStatus() == gale::key::status::up) {
			EWOL_VERBOSE("    mode DOWN " << m_size.y() << "<" << scrollSize.y());
			if(m_size.y() < scrollSize.y()) {
				scrollOffset.setY(scrollOffset.y()+m_pixelScrolling);
				scrollOffset.setY(std::avg(0.0f, scrollOffset.y(), (scrollSize.y() - m_size.y()*propertyLimit->y())));
				markToRedraw();
				if (m_subWidget != nullptr) {
					m_subWidget->setOffset(scrollOffset);
				}
				return true;
			}
		}else if (_event.getId() == 2) {
			if (_event.getStatus() == gale::key::status::down) {
				m_highSpeedMode = speedModeInit;
				m_highSpeedType = gale::key::type::mouse;
				m_highSpeedStartPos.setValue(relativePos.x(), relativePos.y());
				m_highSpeedButton = 2;
				// not really use...  == > just keep some informations
				return false;
			}
		} else if(    m_highSpeedMode != speedModeDisable
		           && _event.getStatus() == gale::key::status::leave) {
			m_highSpeedMode = speedModeDisable;
			m_highSpeedType = gale::key::type::unknow;
			markToRedraw();
			return true;
		}
		if (    _event.getId() == m_highSpeedButton
		     && m_highSpeedMode != speedModeDisable) {
			if (_event.getStatus() == gale::key::status::up) {
				if (m_highSpeedMode == speedModeInit) {
					// TODO : generate back the down event ...
					m_highSpeedMode = speedModeDisable;
					m_highSpeedType = gale::key::type::unknow;
					return false;
				} else {
					m_highSpeedMode = speedModeGrepEndEvent;
					markToRedraw();
					return true;
				}
			} else if (m_highSpeedMode == speedModeGrepEndEvent) {
				if (_event.getStatus() == gale::key::status::pressSingle) {
					m_highSpeedMode = speedModeDisable;
					m_highSpeedType = gale::key::type::unknow;
					m_highSpeedButton = -1;
					markToRedraw();
				}
				return true;
			} else if(    m_highSpeedMode == speedModeInit
			           && _event.getStatus() == gale::key::status::move) {
				// wait that the cursor move more than 10 px to enable it :
				if(    std::abs(relativePos.x() - m_highSpeedStartPos.x()) > 10 
				    || std::abs(relativePos.y() - m_highSpeedStartPos.y()) > 10 ) {
					// the scrooling can start : 
					// select the direction :
					if (relativePos.x() == m_highSpeedStartPos.x()) {
						m_highSpeedMode = speedModeEnableVertical;
					} else if (relativePos.y() == m_highSpeedStartPos.y()) {
						m_highSpeedMode = speedModeEnableHorizontal;
					} else {
						float coef = (relativePos.y() - m_highSpeedStartPos.y()) / (relativePos.x() - m_highSpeedStartPos.x());
						if (std::abs(coef) <= 1 ) {
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
				scrollOffset.setY(std::avg(0.0f, scrollOffset.y(), (scrollSize.y() - m_size.y()*propertyLimit->y())));
				if (m_subWidget != nullptr) {
					m_subWidget->setOffset(scrollOffset);
				}
				return true;
			}
			if(    m_highSpeedMode == speedModeEnableHorizontal
			    && _event.getStatus() == gale::key::status::move) {
				scrollOffset.setX((int32_t)(scrollSize.x() * (relativePos.x()-SCROLL_BAR_SPACE) / (m_size.x()-SCROLL_BAR_SPACE*2)));
				scrollOffset.setX(std::avg(0.0f, scrollOffset.x(), (scrollSize.x() - m_size.x()*propertyLimit->x() )));
				markToRedraw();
				if (m_subWidget != nullptr) {
					m_subWidget->setOffset(scrollOffset);
				}
				return true;
			}
			if(    m_highSpeedMode == speedModeEnableVertical
			    && _event.getStatus() == gale::key::status::move) {
				scrollOffset.setY((int32_t)(scrollSize.y() * (relativePos.y()-SCROLL_BAR_SPACE) / (m_size.y()-SCROLL_BAR_SPACE*2)));
				scrollOffset.setY(std::avg(0.0f, scrollOffset.y(), (scrollSize.y() - m_size.y()*propertyLimit->x())));
				markToRedraw();
				if (m_subWidget != nullptr) {
					m_subWidget->setOffset(scrollOffset);
				}
				return true;
			}
		}
	} else if(    gale::key::type::finger == _event.getType()
	           && (    gale::key::type::unknow == m_highSpeedType
	                || gale::key::type::finger == m_highSpeedType ) ) {
		if (1 == _event.getId()) {
			EWOL_VERBOSE("event: " << _event);
			if (gale::key::status::down == _event.getStatus()) {
				m_highSpeedMode = speedModeInit;
				m_highSpeedType = gale::key::type::finger;
				m_highSpeedStartPos.setValue(relativePos.x(), relativePos.y());
				EWOL_VERBOSE("SCROOL  == > INIT pos=" << m_highSpeedStartPos << " && curent scrollOffset=" << scrollOffset);
				return true;
			} else if (gale::key::status::upAfter == _event.getStatus()) {
				m_highSpeedMode = speedModeDisable;
				m_highSpeedType = gale::key::type::unknow;
				EWOL_VERBOSE("SCROOL  == > DISABLE");
				markToRedraw();
				return true;
			} else if (    m_highSpeedMode == speedModeInit
			            && gale::key::status::move == _event.getStatus()) {
				// wait that the cursor move more than 10 px to enable it :
				if(    std::abs(relativePos.x() - m_highSpeedStartPos.x()) > 10 
				    || std::abs(relativePos.y() - m_highSpeedStartPos.y()) > 10 ) {
					// the scrooling can start : 
					// select the direction :
					m_highSpeedMode = speedModeEnableFinger;
					EWOL_VERBOSE("SCROOL  == > ENABLE");
					markToRedraw();
				}
				return true;
			}
			if (    m_highSpeedMode == speedModeEnableFinger
			     && gale::key::status::move == _event.getStatus()) {
				EWOL_VERBOSE("SCROOL  == > INIT scrollOffset=" << scrollOffset.y() << " relativePos=" << relativePos.y() << " m_highSpeedStartPos=" << m_highSpeedStartPos.y());
				//scrollOffset.x = (int32_t)(scrollSize.x * x / m_size.x);
				if (propertyLimit->x() != 0.0f) {
					scrollOffset.setX(scrollOffset.x() + (relativePos.x() - m_highSpeedStartPos.x()));
					scrollOffset.setX(std::avg(0.0f, scrollOffset.x(), (scrollSize.x() - m_size.x()*propertyLimit->x())));
				}
				if (propertyLimit->y() != 0.0f) {
					scrollOffset.setY(scrollOffset.y() - (relativePos.y() - m_highSpeedStartPos.y()));
					scrollOffset.setY(std::avg(0.0f, scrollOffset.y(), (scrollSize.y() - m_size.y()*propertyLimit->y())));
				}
				// update current position:
				m_highSpeedStartPos = relativePos;
				EWOL_VERBOSE("SCROOL  == > MOVE " << scrollOffset);
				markToRedraw();
				if (m_subWidget != nullptr) {
					m_subWidget->setOffset(scrollOffset);
				}
				return true;
			}
			if (m_highSpeedMode == speedModeEnableFinger) {
				return true;
			}
		} else if (    m_highSpeedMode != speedModeDisable
		            && gale::key::status::leave == _event.getStatus()) {
			m_highSpeedMode = speedModeDisable;
			m_highSpeedType = gale::key::type::unknow;
			EWOL_VERBOSE("SCROOL  == > DISABLE");
			markToRedraw();
			return true;
		}
	}
	return false;
}

ewol::WidgetShared ewol::widget::Scroll::getWidgetAtPos(const vec2& _pos) {
	ewol::WidgetShared tmpWidget = ewol::widget::Container::getWidgetAtPos(_pos);
	if (tmpWidget != nullptr) {
		return tmpWidget;
	}
	return ememory::dynamicPointerCast<ewol::Widget>(sharedFromThis());;
}

void ewol::widget::Scroll::onChangePropertyLimit() {
	markToRedraw();
}

void ewol::widget::Scroll::onChangePropertyShapeVert() {
	m_shaperV.setSource(propertyShapeVert);
	markToRedraw();
}

void ewol::widget::Scroll::onChangePropertyShapeHori() {
	m_shaperH.setSource(propertyShapeHori);
	markToRedraw();
}

