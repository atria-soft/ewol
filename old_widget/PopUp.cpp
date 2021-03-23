/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/PopUp.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/compositing/Drawing.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/object/Manager.hpp>
#include <ewol/ewol.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::PopUp);

static  char* annimationIncrease = "increase";

ewol::widget::PopUp::PopUp() :
  propertyShape(this, "shaper",
                      etk::Uri("THEME_GUI:///PopUp.json?lib=ewol"),
                      "The shaper properties",
                      ewol::widget::PopUp::onChangePropertyShape),
  propertyLockExpand(this, "lock",
                           Vector2b(true,true),
                           "Lock expand contamination",
                           ewol::widget::PopUp::onChangePropertyLockExpand),
  propertyCloseOutEvent(this, "out-click-remove",
                              false,
                              "Remove the widget if the use click outside") {
	addObjectType("ewol::widget::PopUp");
	
}

void ewol::widget::PopUp::init() {
	ewol::widget::Container::init();
	propertyFill.set(Vector2b(false,false));
	propertyShape.notifyChange();
	propertyMinSize.set(gale::Dimension(Vector2f(80,80),gale::distance::pourcent));
	propertyExpand.set(Vector2b(false, false));
}
ewol::widget::PopUp::~PopUp() {
	
}

void ewol::widget::PopUp::onChangeSize() {
	markToRedraw();
	if (this.subWidget == null) {
		return;
	}
	ewol::Padding padding = this.shaper.getPadding();
	Vector2f subWidgetSize = this.subWidget.getCalculateMinSize();
	if (this.subWidget.canExpand().x() == true) {
		if (propertyLockExpand.x() == true) {
			subWidgetSize.setX(this.minSize.x());
		} else {
			subWidgetSize.setX(this.size.x()-padding.xLeft());
		}
	}
	if (this.subWidget.canExpand().y() == true) {
		if (propertyLockExpand.y() == true) {
			subWidgetSize.setY(this.minSize.y());
		} else {
			subWidgetSize.setY(this.size.y()-padding.yButtom());
		}
	}
	// limit the size of the element :
	//subWidgetSize.setMin(this.minSize);
	// posiition at a int pos :
	subWidgetSize = Vector2fClipInt32(subWidgetSize);
	
	// set config to the Sub-widget
	Vector2f subWidgetOrigin = this.origin + (this.size-subWidgetSize)/2.0f;
	subWidgetOrigin = Vector2fClipInt32(subWidgetOrigin);
	
	this.subWidget.setOrigin(subWidgetOrigin);
	this.subWidget.setSize(subWidgetSize);
	this.subWidget.onChangeSize();
}

void ewol::widget::PopUp::systemDraw( ewol::DrawProperty _displayProp) {
	if (*propertyHide == true){
		// widget is hidden ...
		return;
	}
	Widget::systemDraw(_displayProp);
	if (this.subWidget == null) {
		return;
	}
	if(    this.shaper.getNextDisplayedStatus() == -1
	    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.shaper.getTransitionStatus() >= 1.0) {
		ewol::DrawProperty prop = _displayProp;
		prop.limit(this.origin, this.size);
		this.subWidget.systemDraw(prop);
	}
}

void ewol::widget::PopUp::onDraw() {
	this.shaper.draw();
}

void ewol::widget::PopUp::onRegenerateDisplay() {
	if (needRedraw() == true) {
		this.shaper.clear();
		ewol::Padding padding = this.shaper.getPadding();
		Vector2f tmpSize(0,0);
		Vector2b expand = canExpand();
		Vector2b fill = canFill();
		if (fill.x() == true) {
			tmpSize.setX(this.size.x()-padding.x());
		}
		if (fill.y() == true) {
			tmpSize.setY(this.size.y()-padding.y());
		}
		if (this.subWidget != null) {
			Vector2f tmpSize = this.subWidget.getSize();
		}
		tmpSize.setMax(this.minSize);
		Vector2f tmpOrigin = (this.size-tmpSize)/2.0f;
		this.shaper.setShape(Vector2f(0,0),
		                  Vector2fClipInt32(this.size),
		                  Vector2fClipInt32(tmpOrigin-Vector2f(padding.xLeft(), padding.yButtom())),
		                  Vector2fClipInt32(tmpSize + Vector2f(padding.x(), padding.y())));
	}
	// SUBwIDGET GENERATION ...
	if (this.subWidget != null) {
		this.subWidget.onRegenerateDisplay();
	}
}

Widget ewol::widget::PopUp::getWidgetAtPos( Vector2f _pos) {
	Widget val = ewol::widget::Container::getWidgetAtPos(_pos);
	if (val != null) {
		return val;
	}
	return ememory::dynamicPointerCast<Widget>(sharedFromThis());
}

void ewol::widget::PopUp::onChangePropertyShape() {
	this.shaper.setSource(*propertyShape);
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::PopUp::onChangePropertyLockExpand() {
	markToRedraw();
	requestUpdateSize();
}

boolean ewol::widget::PopUp::onEventInput( ewol::event::Input _event) {
	if (_event.getId() == 0) {
		return false;
	}
	if (_event.getStatus() ==  KeyStatus::move) {
		return false;
	}
	if (*propertyCloseOutEvent == true) {
		return false;
	}
	ewol::Padding padding = this.shaper.getPadding();
	Vector2f tmpSize(0,0);
	if (this.subWidget != null) {
		Vector2f tmpSize = this.subWidget.getSize();
	}
	tmpSize.setMax(this.minSize);
	Vector2f tmpOrigin = (this.size-tmpSize)/2.0f;
	
	tmpOrigin -= Vector2f(padding.xLeft(), padding.yButtom());
	tmpSize += Vector2f(padding.x(), padding.y());
	Vector2f pos = relativePosition(_event.getPos());
	if(    pos.x() < tmpOrigin.x()
	    || pos.y() < tmpOrigin.y()
	    || pos.x() > tmpOrigin.x()+tmpSize.x()
	    || pos.y() > tmpOrigin.y()+tmpSize.y() ) {
		autoDestroy();
		return true;
	}
	return false;
}

