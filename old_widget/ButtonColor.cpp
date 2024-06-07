/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */


#include <ewol/widget/ButtonColor.hpp>

#include <ewol/compositing/Drawing.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/widget/meta/ColorChooser.hpp>
#include <ewol/widget/Windows.hpp>
#include <ewol/ewol.hpp>
#include <ewol/object/Manager.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::ButtonColor);

// DEFINE for the shader display system :
#define STATUS_UP        (0)
#define STATUS_HOVER     (2)
#define STATUS_PRESSED   (1)
#define STATUS_DOWN      (3)

ewol::widget::ButtonColor::ButtonColor() :
  signalChange(this, "change", "Button color change value"),
  propertyValue(this, "color", etk::color::black, "Current color", ewol::widget::ButtonColor::onChangePropertyValue),
  propertyShape(this, "shape", etk::Uri("THEME_GUI:///Button.json?lib=ewol"), "shape of the widget", ewol::widget::ButtonColor::onChangePropertyShape),
  this.widgetContextMenu(null) {
	addObjectType("ewol::widget::ButtonColor");
	changeStatusIn(STATUS_UP);
	// Limit event at 1:
	setMouseLimit(1);
	propertyCanFocus.setDirectCheck(true);
}

void ewol::widget::ButtonColor::init() {
	Widget::init();
	propertyShape.notifyChange();
	propertyValue.notifyChange();
}

ewol::widget::ButtonColor::~ButtonColor() {
	
}

void ewol::widget::ButtonColor::calculateMinMaxSize() {
	ewol::Padding padding = this.shaper.getPadding();
	String label = propertyValue.getString();
	Vector3f minSize = this.text.calculateSize(label);
	this.minSize.setX(padding.x()*2 + minSize.x() + 7);
	this.minSize.setY(padding.y()*2 + minSize.y() );
	markToRedraw();
}



void ewol::widget::ButtonColor::onDraw() {
	this.shaper.draw();
	this.text.draw();
}


void ewol::widget::ButtonColor::onRegenerateDisplay() {
	if (needRedraw() == false) {
		return;
	}
	LOGGER.debug("redraw");
	this.text.clear();
	this.shaper.clear();
	
	ewol::Padding padding = this.shaper.getPadding();
	
	String label = propertyValue.getString();
	
	Vector2i localSize = this.minSize;
	
	Vector3f tmpOrigin((this.size.x() - this.minSize.x()) / 2.0,
	               (this.size.y() - this.minSize.y()) / 2.0,
	               0);
	// no change for the text orogin : 
	Vector3f tmpTextOrigin((this.size.x() - this.minSize.x()) / 2.0,
	                   (this.size.y() - this.minSize.y()) / 2.0,
	                   0);
	
	if (propertyFill.x() == true) {
		localSize.setX(this.size.x());
		tmpOrigin.setX(0);
		tmpTextOrigin.setX(0);
	}
	if (propertyFill.y() == true) {
		localSize.setY(this.size.y());
	}
	tmpOrigin += Vector3f(padding.xLeft(), padding.yButtom(), 0);
	tmpTextOrigin += Vector3f(padding.xLeft(), padding.yButtom(), 0);
	localSize -= Vector2i(padding.x(), padding.y());
	
	// clean the element
	this.text.reset();
	if(    propertyValue.get().r() < 100
	    || propertyValue.get().g() < 100
	    || propertyValue.get().b() < 100) {
		this.text.setColor(etk::color::white);
	} else {
		this.text.setColor(etk::color::black);
	}
	this.text.setPos(tmpTextOrigin);
	this.text.setColorBg(propertyValue.get());
	this.text.setTextAlignement(tmpTextOrigin.x(), tmpTextOrigin.x()+localSize.x(), ewol::compositing::alignCenter);
	this.text.print(label);
	
	
	if (propertyFill.y() == true) {
		tmpOrigin.setY(padding.yButtom());
	}
	
	// selection area :
	this.selectableAreaPos = Vector2f(tmpOrigin.x()-padding.xLeft(), tmpOrigin.y()-padding.yButtom());
	this.selectableAreaSize = localSize + Vector2f(padding.x(),padding.y());
	Vector3f tmpp = this.text.calculateSize(label);
	this.shaper.setShape(this.selectableAreaPos,
	                  this.selectableAreaSize,
	                  Vector2f(tmpTextOrigin.x(), tmpTextOrigin.y()),
	                  Vector2f(tmpp.x(), tmpp.y()));
}


boolean ewol::widget::ButtonColor::onEventInput( ewol::event::Input _event) {
	boolean previousHoverState = this.mouseHover;
	if(KeyStatus::leave == _event.getStatus()) {
		this.mouseHover = false;
		this.buttonPressed = false;
	} else {
		Vector2f relativePos = relativePosition(_event.getPos());
		// prevent error from ouside the button
		if(    relativePos.x() < this.selectableAreaPos.x()
		    || relativePos.y() < this.selectableAreaPos.y()
		    || relativePos.x() > this.selectableAreaPos.x() + this.selectableAreaSize.x()
		    || relativePos.y() > this.selectableAreaPos.y() + this.selectableAreaSize.y() ) {
			this.mouseHover = false;
			this.buttonPressed = false;
		} else {
			this.mouseHover = true;
		}
	}
	boolean previousPressed = this.buttonPressed;
	//LOGGER.debug("Event on BT ... mouse position : " + this.mouseHover);
	if (true == this.mouseHover) {
		if (1 == _event.getId()) {
			if(KeyStatus::down == _event.getStatus()) {
				this.buttonPressed = true;
				markToRedraw();
			}
			if(KeyStatus::up == _event.getStatus()) {
				this.buttonPressed = false;
				markToRedraw();
			}
			if(KeyStatus::pressSingle == _event.getStatus()) {
				this.buttonPressed = false;
				this.mouseHover = false;
				// create a context menu : 
				this.widgetContextMenu = ewol::widget::ContextMenu::create();
				if (this.widgetContextMenu == null) {
					LOGGER.error("Allocation Error");
					return true;
				}
				Vector2f tmpPos = this.origin + this.selectableAreaPos + this.selectableAreaSize;
				tmpPos.setX( tmpPos.x() - this.minSize.x()/2.0);
				this.widgetContextMenu.setPositionMark(ewol::widget::ContextMenu::markButtom, tmpPos );
				
				ewol::widget::ColorChooser myColorChooser = widget::ColorChooser::create();
				myColorChooser.propertyValue.set(propertyValue.get());
				// set it in the pop-up-system : 
				this.widgetContextMenu.setSubWidget(myColorChooser);
				myColorChooser.signalChange.connect(sharedFromThis(), ewol::widget::ButtonColor::onCallbackColorChange);
				ewol::widget::Windows currentWindows = getWindows();
				if (currentWindows == null) {
					LOGGER.error("Can not get the curent Windows...");
					this.widgetContextMenu.reset();
				} else {
					currentWindows.popUpWidgetPush(this.widgetContextMenu);
				}
				markToRedraw();
			}
		}
	}
	if(    this.mouseHover != previousHoverState
	    || this.buttonPressed != previousPressed) {
		if (this.buttonPressed == true) {
			changeStatusIn(STATUS_PRESSED);
		} else {
			if (this.mouseHover == true) {
				changeStatusIn(STATUS_HOVER);
			} else {
				changeStatusIn(STATUS_UP);
			}
		}
	}
	return this.mouseHover;
}

void ewol::widget::ButtonColor::onCallbackColorChange( etk::Color<> _color) {
	propertyValue.set(_color);
}

void ewol::widget::ButtonColor::changeStatusIn(int _newStatusId) {
	if (this.shaper.changeStatusIn(_newStatusId) == true) {
		this.PCH = getObjectManager().periodicCall.connect(this, ewol::widget::ButtonColor::periodicCall);
		markToRedraw();
	}
}

void ewol::widget::ButtonColor::periodicCall( ewol::event::Time _event) {
	if (this.shaper.periodicCall(_event) == false) {
		this.PCH.disconnect();
	}
	markToRedraw();
}

void ewol::widget::ButtonColor::onChangePropertyValue() {
	signalChange.emit(propertyValue);
}

void ewol::widget::ButtonColor::onChangePropertyShape() {
	this.shaper.setSource(propertyShape.get());
	markToRedraw();
}

