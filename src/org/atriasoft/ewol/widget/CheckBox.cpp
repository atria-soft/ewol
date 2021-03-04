/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/CheckBox.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/object/Manager.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::CheckBox);

// DEFINE for the shader display system :
#define STATUS_UP        (0)
#define STATUS_HOVER     (2)
#define STATUS_PRESSED   (1)
#define STATUS_SELECTED  (2)

ewol::widget::CheckBox::CheckBox() :
  signalPressed(this, "pressed", "CheckBox is pressed"),
  signalDown(this, "down", "CheckBox is DOWN"),
  signalUp(this, "up", "CheckBox is UP"),
  signalEnter(this, "enter", "The cursor enter inside the CheckBox"),
  signalValue(this, "value", "CheckBox value change"),
  propertyValue(this, "value",
                      false,
                      "Basic value of the widget",
                      ewol::widget::CheckBox::onChangePropertyValue),
  propertyShape(this, "shape",
                      etk::Uri("THEME_GUI:///CheckBox.json?lib=ewol"),
                      "The display name for config file",
                      ewol::widget::CheckBox::onChangePropertyShape),
  this.mouseHover(false),
  this.buttonPressed(false),
  this.selectableAreaPos(0,0),
  this.selectableAreaSize(0,0),
  this.shaperIdSize(-1),
  this.shaperIdSizeInsize(-1) {
	addObjectType("ewol::widget::CheckBox");
	// shaper satatus update:
	CheckStatus();
	propertyCanFocus.setDirectCheck(true);
	// Limit event at 1:
	setMouseLimit(1);
}


void ewol::widget::CheckBox::init() {
	ewol::widget::Container2::init();
	propertyShape.notifyChange();
}

ewol::widget::CheckBox::~CheckBox() {
	
}

void ewol::widget::CheckBox::onChangeSize() {
	ewol::Padding padding = this.shaper.getPadding();
	float boxSize = this.shaper.getConfigNumber(this.shaperIdSize);
	padding.setXLeft(padding.xLeft()*2.0f + boxSize);
	ewol::Padding ret = onChangeSizePadded(padding);
	Log.debug(" configuring : padding=" + padding + " boxSize=" + boxSize + "");
	this.selectableAreaPos = Vector2f(ret.xLeft()/*-boxSize*/, ret.yButtom());
	this.selectableAreaSize = this.size - (this.selectableAreaPos + Vector2f(ret.xRight(), ret.yTop()));
}

void ewol::widget::CheckBox::calculateMinMaxSize() {
	ewol::Padding padding = this.shaper.getPadding();
	float boxSize = this.shaper.getConfigNumber(this.shaperIdSize);
	padding.setXLeft(padding.xLeft()*2.0f + boxSize);
	calculateMinMaxSizePadded(padding);
	if (this.minSize.y() < padding.y()+boxSize) {
		this.minSize.setY(padding.y()+boxSize);
	}
}

void ewol::widget::CheckBox::onDraw() {
	// draw the shaaper (if needed indeed)
	this.shaper.draw();
}

void ewol::widget::CheckBox::onRegenerateDisplay() {
	ewol::widget::Container2::onRegenerateDisplay();
	if (needRedraw() == false) {
		return;
	}
	ewol::Padding padding = this.shaper.getPadding();
	float boxSize = this.shaper.getConfigNumber(this.shaperIdSize);
	float boxInside = this.shaper.getConfigNumber(this.shaperIdSizeInsize);
	this.shaper.clear();
	Log.debug(" configuring : boxSize=" + boxSize + " boxInside=" + boxInside + "");
	Vector2f origin(this.selectableAreaPos + Vector2f(0, (this.selectableAreaSize.y() - (boxSize+padding.y()))*0.5f));
	Vector2f size = Vector2f(boxSize+padding.x(), boxSize+padding.y());
	
	Vector2f origin2 = this.selectableAreaPos + Vector2f((boxSize-boxInside)*0.5f, (this.selectableAreaSize.y() - (boxInside+padding.y()))*0.5f);
	Vector2f size2 = Vector2f(boxInside+padding.x(), boxInside+padding.y());
	this.shaper.setShape(Vector2fClipInt32(origin),
	                   Vector2fClipInt32(size),
	                   Vector2fClipInt32(origin2+Vector2f(padding.xLeft(),padding.yButtom()) ),
	                   Vector2fClipInt32(size2-Vector2f(padding.x(),padding.y()) ));
}

boolean ewol::widget::CheckBox::onEventInput( ewol::event::Input _event) {
	Log.verbose("Event on BT : " + _event);
	
	boolean previousHoverState = this.mouseHover;
	if(    KeyStatus::leave == _event.getStatus()
	    || KeyStatus::abort == _event.getStatus()) {
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
	Log.verbose("Event on BT ... mouse hover : " + this.mouseHover);
	if (this.mouseHover == true) {
		if (_event.getId() == 1) {
			if(KeyStatus::down == _event.getStatus()) {
				Log.verbose(*propertyName + " : Generate event : " + signalDown);
				signalDown.emit();
				this.buttonPressed = true;
				markToRedraw();
			}
			if(KeyStatus::up == _event.getStatus()) {
				Log.verbose(*propertyName + " : Generate event : " + signalUp);
				signalUp.emit();
				this.buttonPressed = false;
				markToRedraw();
			}
			if(KeyStatus::pressSingle == _event.getStatus()) {
				// inverse value :
				propertyValue.set((*propertyValue)?false:true);
				Log.verbose(*propertyName + " : Generate event : " + signalPressed);
				signalPressed.emit();
				Log.verbose(*propertyName + " : Generate event : " + signalValue + " val=" + propertyValue );
				signalValue.emit(*propertyValue);
				markToRedraw();
			}
		}
	}
	if(    this.mouseHover != previousHoverState
	    || this.buttonPressed != previousPressed) {
		CheckStatus();
	}
	return this.mouseHover;
}


boolean ewol::widget::CheckBox::onEventEntry( ewol::event::Entry _event) {
	//Log.debug("BT PRESSED : \"" + UTF8_data + "\" size=" + strlen(UTF8_data));
	if(    _event.getType() == KeyKeyboard::character
	    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::down
	    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getChar() == '\r') {
		signalEnter.emit();
		return true;
	}
	return false;
}

void ewol::widget::CheckBox::CheckStatus() {
	if (this.shaper.setState(*propertyValue==true?1:0) == true) {
		markToRedraw();
	}
	if (this.buttonPressed == true) {
		changeStatusIn(STATUS_PRESSED);
		return;
	}
	if (this.mouseHover == true) {
		changeStatusIn(STATUS_HOVER);
		return;
	}
	changeStatusIn(STATUS_UP);
}

void ewol::widget::CheckBox::changeStatusIn(int _newStatusId) {
	if (this.shaper.changeStatusIn(_newStatusId) == true) {
		this.PCH = getObjectManager().periodicCall.connect(this, ewol::widget::CheckBox::periodicCall);
		markToRedraw();
	}
}


void ewol::widget::CheckBox::periodicCall( ewol::event::Time _event) {
	if (this.shaper.periodicCall(_event) == false) {
		this.PCH.disconnect();
	}
	markToRedraw();
}

void ewol::widget::CheckBox::onChangePropertyShape() {
	this.shaper.setSource(*propertyShape);
	this.shaperIdSize = this.shaper.requestConfig("box-size");
	this.shaperIdSizeInsize = this.shaper.requestConfig("box-inside");
	markToRedraw();
}

void ewol::widget::CheckBox::onChangePropertyValue() {
	if (*propertyValue == false) {
		this.idWidgetDisplayed = convertId(0);
	} else {
		this.idWidgetDisplayed = convertId(1);
	}
	CheckStatus();
	markToRedraw();
	this.shaper.setActivateState(*propertyValue==true?1:0);
}
