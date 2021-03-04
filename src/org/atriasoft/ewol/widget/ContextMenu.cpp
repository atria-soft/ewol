/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/ewol.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/widget/ContextMenu.hpp>
#include <ewol/compositing/Drawing.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/widget/Windows.hpp>
#include <ewol/Padding.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::ContextMenu);
ETK_DECLARE_TYPE(enum ewol::widget::ContextMenu::markPosition);

ewol::widget::ContextMenu::ContextMenu():
  propertyShape(this, "shape",
                      etk::Uri("THEME_GUI:///ContextMenu.json?lib=ewol"),
                      "the display name for config file",
                      ewol::widget::ContextMenu::onChangePropertyShape),
  propertyArrowPos(this, "arrow-position",
                         Vector2f(0,0),
                         "Position of the arrow in the pop-up",
                         ewol::widget::ContextMenu::onChangePropertyArrowPos),
  propertyArrawBorder(this, "arrow-mode",
                            markTop,
                            "position of the arrow",
                            ewol::widget::ContextMenu::onChangePropertyArrawBorder) {
	addObjectType("ewol::widget::ContextMenu");
	propertyArrawBorder.add(markTop, "top");
	propertyArrawBorder.add(markRight, "right");
	propertyArrawBorder.add(markButtom, "buttom");
	propertyArrawBorder.add(markLeft, "left");
	propertyArrawBorder.add(markNone, "none");
	
	
	this.offset = 20;
	
	
	this.colorBorder = etk::color::black;
	this.colorBorder.setA(0x7F);
	
	setMouseLimit(1);
}

void ewol::widget::ContextMenu::init() {
	ewol::widget::Container::init();
	propertyShape.notifyChange();
}

ewol::widget::ContextMenu::~ContextMenu() {
	
}

void ewol::widget::ContextMenu::onChangeSize() {
	markToRedraw();
	// pop-up fill all the display :
	ewol::Padding padding = this.shaper.getPadding();
	Log.verbose("our origin=" + this.origin + " size=" + this.size);
	if (this.subWidget == null) {
		return;
	}
	Vector2f subWidgetSize(0,0);
	Vector2f subWidgetOrigin(0,0);
	subWidgetSize = this.subWidget.getCalculateMinSize();
	if (this.subWidget.canExpand().x() == true) {
		subWidgetSize.setX(this.size.x());
	}
	if (this.subWidget.canExpand().y() == true) {
		subWidgetSize.setY(this.size.y());
	}
	int minWidth = 100;
	int maxWidth = 300;
	subWidgetSize.setX((int)etk::max(minWidth, (int)subWidgetSize.x()));
	subWidgetSize.setX((int)etk::min(maxWidth, (int)subWidgetSize.x()));
	subWidgetSize.setY((int)subWidgetSize.y());
	
	// set config to the Sub-widget
	switch (propertyArrawBorder.get()) {
		case markTop:
			subWidgetOrigin.setX((int)(propertyArrowPos.x() - subWidgetSize.x()/2));
			subWidgetOrigin.setY((int)(propertyArrowPos.y() - this.offset - subWidgetSize.y()));
			break;
		case markButtom:
			subWidgetOrigin.setX((int)(propertyArrowPos.x() - subWidgetSize.x()/2));
			subWidgetOrigin.setY((int)(propertyArrowPos.y() + this.offset));
			break;
		case markRight:
		case markLeft:
		default:
			subWidgetOrigin.setX((int)(this.size.x() - this.origin.x() - subWidgetSize.x())/2 + this.origin.x());
			subWidgetOrigin.setY((int)(this.size.y() - this.origin.y() - subWidgetSize.y())/2 + this.origin.y());
			break;
	}
	// set the widget position at the border of the screen
	subWidgetOrigin.setX( (int)(   etk::max(0, (int)(subWidgetOrigin.x()-padding.x()))
	                                 + padding.x()) );
	subWidgetOrigin.setY( (int)(   etk::max(0, (int)(subWidgetOrigin.y()-padding.y()))
	                                 + padding.y()) );
	switch (propertyArrawBorder.get()) {
		default:
		case markTop:
		case markButtom:
			if (propertyArrowPos.x() <= this.offset ) {
				subWidgetOrigin.setX(propertyArrowPos.x()+padding.xLeft());
			}
			break;
		case markRight:
		case markLeft:
			if (propertyArrowPos.y() <= this.offset ) {
				subWidgetOrigin.setY(propertyArrowPos.y()+padding.yButtom());
			}
			break;
	}
	Log.verbose("       == > sub origin=" + subWidgetOrigin + " size=" + subWidgetSize);
	this.subWidget.setOrigin(subWidgetOrigin);
	this.subWidget.setSize(subWidgetSize);
	this.subWidget.onChangeSize();
}


void ewol::widget::ContextMenu::calculateMinMaxSize() {
	// call main class to calculate the min size...
	ewol::widget::Container::calculateMinMaxSize();
	// add padding of the display
	ewol::Padding padding = this.shaper.getPadding();
	this.minSize += Vector2f(padding.x(), padding.y());
	//Log.debug("CalculateMinSize=>>" + this.minSize);
	markToRedraw();
}


void ewol::widget::ContextMenu::onDraw() {
	this.compositing.draw();
	this.shaper.draw();
}


void ewol::widget::ContextMenu::onRegenerateDisplay() {
	// call upper class :
	ewol::widget::Container::onRegenerateDisplay();
	if (needRedraw() == false) {
		return;
	}
	this.compositing.clear();
	this.shaper.clear();
	ewol::Padding padding = this.shaper.getPadding();
	
	if (this.subWidget == null) {
		return;
	}
	Vector2f tmpSize = this.subWidget.getSize();
	Vector2f tmpOrigin = this.subWidget.getOrigin();
	
	// display border ...
	this.compositing.setColor(this.colorBorder);
	switch (propertyArrawBorder) {
		case markTop:
			this.compositing.setPos(Vector3f(propertyArrowPos.x(), propertyArrowPos.y(), 0.0f) );
			this.compositing.addVertex();
			if (propertyArrowPos.x() <= tmpOrigin.x() ) {
				float laking = this.offset - padding.yTop();
				this.compositing.setPos(Vector3f(propertyArrowPos.x()+laking, propertyArrowPos.y()-laking, 0.0f) );
				this.compositing.addVertex();
				this.compositing.setPos(Vector3f(propertyArrowPos.x(),        propertyArrowPos.y()-laking, 0.0f) );
				this.compositing.addVertex();
			} else {
				float laking = this.offset - padding.yTop();
				this.compositing.setPos(Vector3f(propertyArrowPos.x()+laking, propertyArrowPos.y()-laking, 0.0f) );
				this.compositing.addVertex();
				this.compositing.setPos(Vector3f(propertyArrowPos.x()-laking, propertyArrowPos.y()-laking, 0.0f) );
				this.compositing.addVertex();
			}
			break;
		case markButtom:
			this.compositing.setPos(Vector3f(propertyArrowPos.x(), propertyArrowPos.y(), 0) );
			this.compositing.addVertex();
			if (propertyArrowPos.x() <= tmpOrigin.x() ) {
				int laking = this.offset - padding.yTop();
				this.compositing.setPos(Vector3f(propertyArrowPos.x()+laking, propertyArrowPos.y()+laking, 0.0f) );
				this.compositing.addVertex();
				this.compositing.setPos(Vector3f(propertyArrowPos.x(),        propertyArrowPos.y()+laking, 0.0f) );
				this.compositing.addVertex();
			} else {
				int laking = this.offset - padding.yTop();
				this.compositing.setPos(Vector3f(propertyArrowPos.x()+laking, propertyArrowPos.y()+laking, 0.0f) );
				this.compositing.addVertex();
				this.compositing.setPos(Vector3f(propertyArrowPos.x()-laking, propertyArrowPos.y()+laking, 0.0f) );
				this.compositing.addVertex();
			}
			break;
		default:
		case markRight:
		case markLeft:
			Log.todo("later");
			break;
	}
	
	Vector2f shaperOrigin = tmpOrigin-Vector2f(padding.xLeft(), padding.yButtom());
	Vector2f shaperSize = tmpSize+Vector2f(padding.x(), padding.y());
	this.shaper.setShape(Vector2fClipInt32(shaperOrigin),
	                  Vector2fClipInt32(shaperSize));
}

boolean ewol::widget::ContextMenu::onEventInput( ewol::event::Input _event) {
	if (_event.getId() > 0) {
		if (ewol::widget::Container::getWidgetAtPos(_event.getPos()) != null) {
			return false;
		}
		if(    _event.getStatus() == KeyStatus::down
		    || _event.getStatus() == KeyStatus::move
		    || _event.getStatus() == KeyStatus::pressSingle
		    || _event.getStatus() == KeyStatus::up
		    || _event.getStatus() == KeyStatus::enter
		    || _event.getStatus() == KeyStatus::leave ) {
			// Auto-remove ...
			autoDestroy();
			return true;
		}
	}
	return false;
}

Widget ewol::widget::ContextMenu::getWidgetAtPos( Vector2f _pos) {
	Widget val = ewol::widget::Container::getWidgetAtPos(_pos);
	if (val != null) {
		return val;
	}
	return ememory::dynamicPointerCast<Widget>(sharedFromThis());
}

void ewol::widget::ContextMenu::onChangePropertyArrowPos() {
	markToRedraw();
}

void ewol::widget::ContextMenu::onChangePropertyArrawBorder() {
	markToRedraw();
}

void ewol::widget::ContextMenu::onChangePropertyShape() {
	this.shaper.setSource(propertyShape.get());
	markToRedraw();
}


void ewol::widget::ContextMenu::setPositionMarkAuto( Vector2f _origin,  Vector2f _size) {
	ewol::widget::Windows windows = getWindows();
	Vector2f globalSize = windows.getSize();
	// TODO : Support left and right
	float upperSize = globalSize.y() - (_origin.y() + _size.y());
	float underSize = _origin.y();
	if (underSize >= upperSize) {
		Vector2f pos = _origin + _size - Vector2f(_size.x()*0.5f, 0.0f);
		setPositionMark(ewol::widget::ContextMenu::markButtom, pos);
	} else {
		Vector2f pos = _origin + Vector2f(_size.x()*0.5f, 0.0f);
		setPositionMark(ewol::widget::ContextMenu::markTop, pos);
	}
}
void ewol::widget::ContextMenu::setPositionMark(enum markPosition _position,  Vector2f _arrowPos) {
	propertyArrawBorder.set(_position);
	propertyArrowPos.set(_arrowPos);
}

