/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/Scroll.hpp>
#include <ewol/ewol.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/debug.hpp>
#include <ewol/compositing/Drawing.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::Scroll);

ewol::widget::Scroll::Scroll() :
  propertyLimit(this, "limit",
                      Vector2f(0.15,0.5), Vector2f(0.0,0.0), Vector2f(1.0,1.0),
                      "Limit the scroll maximum position [0..1]% represent the free space in the scoll when arrive at the end",
                      ewol::widget::Scroll::onChangePropertyLimit),
  propertyShapeVert(this, "shape-vert",
                          etk::Uri("THEME_GUI:///WidgetScrolled.json?lib=ewol"),
                          "shape for the vertical display",
                          ewol::widget::Scroll::onChangePropertyShapeVert),
  propertyShapeHori(this, "shape-hori",
                          etk::Uri("THEME_GUI:///WidgetScrolled.json?lib=ewol"),
                          "shape for the horizonal display",
                          ewol::widget::Scroll::onChangePropertyShapeHori),
  propertyHover(this, "hover",
                      true,
                      "the display bar are hover the subWidget"),
  this.pixelScrolling(20),
  this.highSpeedStartPos(0,0),
  this.highSpeedMode(speedModeDisable),
  this.highSpeedButton(-1),
  this.highSpeedType(KeyType::unknow) {
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
	Widget::onChangeSize();
	if (*propertyHide == true) {
		return;
	}
	if (this.subWidget == null) {
		return;
	}
	// remove the bar if hover
	Vector2f basicSize = this.size;
	if (*propertyHover == false) {
		basicSize -= Vector2f(SCROLL_BAR_SPACE,SCROLL_BAR_SPACE);
	}
	
	
	Vector2f origin = this.origin+this.offset;
	Vector2f minSize = this.subWidget.getCalculateMinSize();
	Vector2b expand = this.subWidget.propertyExpand.get();
	//The gravity is not set on the sub element ==> special use of the widget
	//origin += ewol::gravityGenerateDelta(propertyGravity.get(), minSize - this.size);
	if (    expand.x() == true
	     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM minSize.x() < basicSize.x()) {
		minSize.setX(basicSize.x());
	}
	if (    expand.y() == true
	     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM minSize.y() < basicSize.y()) {
		minSize.setY(basicSize.y());
	}
	this.subWidget.setSize(minSize);
	if (*propertyGravity == ewol::gravity_top) {
		origin += Vector2f(0.0f, basicSize.y()-minSize.y());
		if (*propertyHover == false) {
			origin += Vector2f(0,SCROLL_BAR_SPACE);
		}
	} else if (*propertyGravity == ewol::gravity_buttom) {
		// nothing to do ... origin += 
	} else {
		Log.error(" Not manage other gravity ...");
	}
	this.subWidget.setOrigin(origin);
	this.subWidget.onChangeSize();
}

void ewol::widget::Scroll::calculateMinMaxSize() {
	// Note: No call of container ==> normal case ...
	Widget::calculateMinMaxSize();
	// call sub classes
	if (this.subWidget != null) {
		this.subWidget.calculateMinMaxSize();
	}
}

void ewol::widget::Scroll::systemDraw( ewol::DrawProperty _displayProp) {
	if (*propertyHide == true) {
		return;
	}
	if (this.subWidget != null) {
		ewol::DrawProperty prop = _displayProp;
		prop.limit(this.origin, this.size);
		this.subWidget.systemDraw(prop);
	}
	Widget::systemDraw(_displayProp);
}

void ewol::widget::Scroll::onDraw() {
	this.shaperH.draw();
	this.shaperV.draw();
	/*
	ewol::compositing::Drawing draw;
	draw.setPos(Vector2f(10,10));
	draw.setColor(etk::color::orange);
	draw.rectangleWidth(Vector2f(25,25));
	draw.setPos(this.size - Vector2f(35,35));
	draw.setColor(etk::color::green);
	draw.rectangleWidth(Vector2f(25,25));
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
	this.shaperH.clear();
	this.shaperV.clear();
	ewol::Padding paddingVert = this.shaperV.getPadding();
	ewol::Padding paddingHori = this.shaperH.getPadding();
	Vector2f scrollOffset(0,0);
	Vector2f scrollSize(0,0);
	if (this.subWidget != null) {
		scrollOffset = this.subWidget.getOffset();
		scrollSize = this.subWidget.getSize();
	}
	if(    this.size.y() < scrollSize.y()
	    || scrollOffset.y() != 0) {
		float lenScrollBar = this.size.y()*this.size.y() / scrollSize.y();
		lenScrollBar = etk::avg(10.0f, lenScrollBar, this.size.y());
		float originScrollBar = scrollOffset.y() / (scrollSize.y()-this.size.y()*propertyLimit.y());
		originScrollBar = etk::avg(0.0f, originScrollBar, 1.0f);
		originScrollBar *= (this.size.y()-lenScrollBar);
		this.shaperV.setShape(Vector2f(this.size.x() - paddingVert.x(), 0),
		                   Vector2f(paddingVert.x(), this.size.y()),
		                   Vector2f(this.size.x() - paddingVert.xRight(), this.size.y() - originScrollBar - lenScrollBar),
		                   Vector2f(0, lenScrollBar));
	}
	if(    this.size.x() < scrollSize.x()
	    || scrollOffset.x() != 0) {
		float lenScrollBar = (this.size.x()-paddingHori.xLeft())*(this.size.x()-paddingVert.x()) / scrollSize.x();
		lenScrollBar = etk::avg(10.0f, lenScrollBar, (this.size.x()-paddingVert.x()));
		float originScrollBar = scrollOffset.x() / (scrollSize.x()-this.size.x()*propertyLimit.x());
		originScrollBar = etk::avg(0.0f, originScrollBar, 1.0f);
		originScrollBar *= (this.size.x()-paddingHori.xRight()-lenScrollBar);
		this.shaperH.setShape(Vector2f(0, 0),
		                   Vector2f(this.size.x()-paddingVert.x(), paddingHori.y()),
		                   Vector2f(originScrollBar, paddingHori.yButtom()),
		                   Vector2f(lenScrollBar, 0));
	}
}

boolean ewol::widget::Scroll::onEventInput( ewol::event::Input _event) {
	//ewol::event::Input _event = event;
	//_event.setType(KeyType::finger);
	Vector2f relativePos = relativePosition(_event.getPos());
	Vector2f scrollOffset(0,0);
	Vector2f scrollSize(0,0);
	if (this.subWidget != null) {
		scrollOffset = this.subWidget.getOffset();
		scrollSize = this.subWidget.getSize();
	}
	Log.verbose("Get Event on scroll : " + _event);
	relativePos.setY(this.size.y() - relativePos.y());
	if(    _event.getType() == KeyType::mouse
	    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (    this.highSpeedType == KeyType::unknow
	         || this.highSpeedType == KeyType::mouse) ) {
		if(    _event.getId() == 1
		    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::down) {
			// check if selected the scrolling position whth the scrolling bar ...
			if (relativePos.x() >= (this.size.x()-SCROLL_BAR_SPACE)) {
				if(    this.size.y() < scrollSize.y()
				    || scrollOffset.y() != 0) {
					this.highSpeedMode = speedModeEnableVertical;
					this.highSpeedType = KeyType::mouse;
					this.highSpeedStartPos.setX(relativePos.x());
					this.highSpeedStartPos.setY(scrollOffset.y() / scrollSize.y() * (this.size.y()-SCROLL_BAR_SPACE*2));
					this.highSpeedButton = 1;
					// force direct scrolling in this case
					scrollOffset.setY((int)(scrollSize.y() * (relativePos.y()-SCROLL_BAR_SPACE) / (this.size.y()-SCROLL_BAR_SPACE*2)));
					scrollOffset.setY(etk::avg(0.0f, scrollOffset.y(), (scrollSize.y() - this.size.y()*propertyLimit.y())));
					markToRedraw();
					if (this.subWidget != null) {
						this.subWidget.setOffset(scrollOffset);
					}
					return true;
				}
			} else if (relativePos.y() >= (this.size.y()-SCROLL_BAR_SPACE)) {
				if(    this.size.x() < scrollSize.x()
				    || scrollOffset.x()!=0) {
					this.highSpeedMode = speedModeEnableHorizontal;
					this.highSpeedType = KeyType::mouse;
					this.highSpeedStartPos.setX(scrollOffset.x() / scrollSize.x() * (this.size.x()-SCROLL_BAR_SPACE*2));
					this.highSpeedStartPos.setY(relativePos.y());
					this.highSpeedButton = 1;
					// force direct scrolling in this case
					scrollOffset.setX((int)(scrollSize.x() * (relativePos.x()-SCROLL_BAR_SPACE) / (this.size.x()-SCROLL_BAR_SPACE*2)));
					scrollOffset.setY(etk::avg(0.0f, scrollOffset.x(), (scrollSize.x() - this.size.x()*propertyLimit.x())));
					markToRedraw();
					if (this.subWidget != null) {
						this.subWidget.setOffset(scrollOffset);
					}
					return true;
				}
			}
			return false;
		} else if(    _event.getId() == 4
		           LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::up) {
			Log.verbose("    mode UP " + this.size.y() + "<" + scrollSize.y());
			if(this.size.y() < scrollSize.y()) {
				scrollOffset.setY(scrollOffset.y()-this.pixelScrolling);
				scrollOffset.setY(etk::avg(0.0f, scrollOffset.y(), (scrollSize.y() - this.size.y()*propertyLimit.y())));
				markToRedraw();
				if (this.subWidget != null) {
					this.subWidget.setOffset(scrollOffset);
				}
				return true;
			}
		} else if(    _event.getId() == 5
		           LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::up) {
			Log.verbose("    mode DOWN " + this.size.y() + "<" + scrollSize.y());
			if(this.size.y() < scrollSize.y()) {
				scrollOffset.setY(scrollOffset.y()+this.pixelScrolling);
				scrollOffset.setY(etk::avg(0.0f, scrollOffset.y(), (scrollSize.y() - this.size.y()*propertyLimit.y())));
				markToRedraw();
				if (this.subWidget != null) {
					this.subWidget.setOffset(scrollOffset);
				}
				return true;
			}
		}else if (_event.getId() == 2) {
			if (_event.getStatus() == KeyStatus::down) {
				this.highSpeedMode = speedModeInit;
				this.highSpeedType = KeyType::mouse;
				this.highSpeedStartPos.setValue(relativePos.x(), relativePos.y());
				this.highSpeedButton = 2;
				// not really use...  == > just keep some informations
				return false;
			}
		} else if(    this.highSpeedMode != speedModeDisable
		           LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::leave) {
			this.highSpeedMode = speedModeDisable;
			this.highSpeedType = KeyType::unknow;
			markToRedraw();
			return true;
		}
		if (    _event.getId() == this.highSpeedButton
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.highSpeedMode != speedModeDisable) {
			if (_event.getStatus() == KeyStatus::up) {
				if (this.highSpeedMode == speedModeInit) {
					// TODO : generate back the down event ...
					this.highSpeedMode = speedModeDisable;
					this.highSpeedType = KeyType::unknow;
					return false;
				} else {
					this.highSpeedMode = speedModeGrepEndEvent;
					markToRedraw();
					return true;
				}
			} else if (this.highSpeedMode == speedModeGrepEndEvent) {
				if (_event.getStatus() == KeyStatus::pressSingle) {
					this.highSpeedMode = speedModeDisable;
					this.highSpeedType = KeyType::unknow;
					this.highSpeedButton = -1;
					markToRedraw();
				}
				return true;
			} else if(    this.highSpeedMode == speedModeInit
			           LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::move) {
				// wait that the cursor move more than 10 px to enable it :
				if(    etk::abs(relativePos.x() - this.highSpeedStartPos.x()) > 10 
				    || etk::abs(relativePos.y() - this.highSpeedStartPos.y()) > 10 ) {
					// the scrooling can start : 
					// select the direction :
					if (relativePos.x() == this.highSpeedStartPos.x()) {
						this.highSpeedMode = speedModeEnableVertical;
					} else if (relativePos.y() == this.highSpeedStartPos.y()) {
						this.highSpeedMode = speedModeEnableHorizontal;
					} else {
						float coef = (relativePos.y() - this.highSpeedStartPos.y()) / (relativePos.x() - this.highSpeedStartPos.x());
						if (etk::abs(coef) <= 1 ) {
							this.highSpeedMode = speedModeEnableHorizontal;
						} else {
							this.highSpeedMode = speedModeEnableVertical;
						}
					}
					if (this.highSpeedMode == speedModeEnableHorizontal) {
						this.highSpeedStartPos.setX(scrollOffset.x() / scrollSize.x() * (this.size.x()-SCROLL_BAR_SPACE*2));
					} else {
						this.highSpeedStartPos.setY(scrollOffset.y() / scrollSize.y() * (this.size.y()-SCROLL_BAR_SPACE*2));
					}
					markToRedraw();
				}
				scrollOffset.setY(etk::avg(0.0f, scrollOffset.y(), (scrollSize.y() - this.size.y()*propertyLimit.y())));
				if (this.subWidget != null) {
					this.subWidget.setOffset(scrollOffset);
				}
				return true;
			}
			if(    this.highSpeedMode == speedModeEnableHorizontal
			    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::move) {
				scrollOffset.setX((int)(scrollSize.x() * (relativePos.x()-SCROLL_BAR_SPACE) / (this.size.x()-SCROLL_BAR_SPACE*2)));
				scrollOffset.setX(etk::avg(0.0f, scrollOffset.x(), (scrollSize.x() - this.size.x()*propertyLimit.x() )));
				markToRedraw();
				if (this.subWidget != null) {
					this.subWidget.setOffset(scrollOffset);
				}
				return true;
			}
			if(    this.highSpeedMode == speedModeEnableVertical
			    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::move) {
				scrollOffset.setY((int)(scrollSize.y() * (relativePos.y()-SCROLL_BAR_SPACE) / (this.size.y()-SCROLL_BAR_SPACE*2)));
				scrollOffset.setY(etk::avg(0.0f, scrollOffset.y(), (scrollSize.y() - this.size.y()*propertyLimit.x())));
				markToRedraw();
				if (this.subWidget != null) {
					this.subWidget.setOffset(scrollOffset);
				}
				return true;
			}
		}
	} else if(    KeyType::finger == _event.getType()
	           LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (    KeyType::unknow == this.highSpeedType
	                || KeyType::finger == this.highSpeedType ) ) {
		if (1 == _event.getId()) {
			Log.verbose("event: " + _event);
			if (KeyStatus::down == _event.getStatus()) {
				this.highSpeedMode = speedModeInit;
				this.highSpeedType = KeyType::finger;
				this.highSpeedStartPos.setValue(relativePos.x(), relativePos.y());
				Log.verbose("SCROOL  == > INIT pos=" + this.highSpeedStartPos + " LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM curent scrollOffset=" + scrollOffset);
				return true;
			} else if (KeyStatus::upAfter == _event.getStatus()) {
				this.highSpeedMode = speedModeDisable;
				this.highSpeedType = KeyType::unknow;
				Log.verbose("SCROOL  == > DISABLE");
				markToRedraw();
				return true;
			} else if (    this.highSpeedMode == speedModeInit
			            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM KeyStatus::move == _event.getStatus()) {
				// wait that the cursor move more than 10 px to enable it :
				if(    etk::abs(relativePos.x() - this.highSpeedStartPos.x()) > 10 
				    || etk::abs(relativePos.y() - this.highSpeedStartPos.y()) > 10 ) {
					// the scrooling can start : 
					// select the direction :
					this.highSpeedMode = speedModeEnableFinger;
					Log.verbose("SCROOL  == > ENABLE");
					markToRedraw();
				}
				return true;
			}
			if (    this.highSpeedMode == speedModeEnableFinger
			     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM KeyStatus::move == _event.getStatus()) {
				Log.verbose("SCROOL  == > INIT scrollOffset=" + scrollOffset.y() + " relativePos=" + relativePos.y() + " this.highSpeedStartPos=" + this.highSpeedStartPos.y());
				//scrollOffset.x = (int)(scrollSize.x * x / this.size.x);
				if (propertyLimit.x() != 0.0f) {
					scrollOffset.setX(scrollOffset.x() + (relativePos.x() - this.highSpeedStartPos.x()));
					scrollOffset.setX(etk::avg(0.0f, scrollOffset.x(), (scrollSize.x() - this.size.x()*propertyLimit.x())));
				}
				if (propertyLimit.y() != 0.0f) {
					scrollOffset.setY(scrollOffset.y() - (relativePos.y() - this.highSpeedStartPos.y()));
					scrollOffset.setY(etk::avg(0.0f, scrollOffset.y(), (scrollSize.y() - this.size.y()*propertyLimit.y())));
				}
				// update current position:
				this.highSpeedStartPos = relativePos;
				Log.verbose("SCROOL  == > MOVE " + scrollOffset);
				markToRedraw();
				if (this.subWidget != null) {
					this.subWidget.setOffset(scrollOffset);
				}
				return true;
			}
			if (this.highSpeedMode == speedModeEnableFinger) {
				return true;
			}
		} else if (    this.highSpeedMode != speedModeDisable
		            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM KeyStatus::leave == _event.getStatus()) {
			this.highSpeedMode = speedModeDisable;
			this.highSpeedType = KeyType::unknow;
			Log.verbose("SCROOL  == > DISABLE");
			markToRedraw();
			return true;
		}
	}
	return false;
}

Widget ewol::widget::Scroll::getWidgetAtPos( Vector2f _pos) {
	Widget tmpWidget = ewol::widget::Container::getWidgetAtPos(_pos);
	if (tmpWidget != null) {
		return tmpWidget;
	}
	return ememory::dynamicPointerCast<Widget>(sharedFromThis());;
}

void ewol::widget::Scroll::onChangePropertyLimit() {
	markToRedraw();
}

void ewol::widget::Scroll::onChangePropertyShapeVert() {
	this.shaperV.setSource(propertyShapeVert);
	markToRedraw();
}

void ewol::widget::Scroll::onChangePropertyShapeHori() {
	this.shaperH.setSource(propertyShapeHori);
	markToRedraw();
}

