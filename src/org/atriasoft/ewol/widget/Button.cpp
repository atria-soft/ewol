/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */


#include <ewol/ewol.hpp>
#include <ewol/widget/Button.hpp>
#include <ewol/object/Manager.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::Button);
ETK_DECLARE_TYPE(ewol::widget::Button::buttonLock);
// DEFINE for the shader display system:
 static int STATUS_UP(0);
 static int STATUS_HOVER(2);
 static int STATUS_PRESSED(1);
 static int STATUS_DOWN(3);

ewol::widget::Button::Button() :
  signalPressed(this, "pressed", "Button is pressed"),
  signalDown(this, "down", "Button is DOWN"),
  signalUp(this, "up", "Button is UP"),
  signalEnter(this, "enter", "The cursor enter inside the button"),
  signalLeave(this, "leave", "the cursor leave the button"),
  signalValue(this, "value", "button value change"),
  propertyShape(this, "shape", etk::Uri("THEME_GUI:///Button.json?lib=ewol"), "The display name for config file", ewol::widget::Button::onChangePropertyShape),
  propertyValue(this, "value", false, "Value of the Button", ewol::widget::Button::onChangePropertyValue),
  propertyLock(this, "lock", lockNone, "Lock the button in a special state to permit changing state only by the coder", ewol::widget::Button::onChangePropertyLock),
  propertyToggleMode(this, "toggle", false, "The Button can toogle", ewol::widget::Button::onChangePropertyToggleMode),
  propertyEnableSingle(this, "enable-single", false, "If one element set in the Button ==> display only set", ewol::widget::Button::onChangePropertyEnableSingle),
  this.mouseHover(false),
  this.buttonPressed(false),
  this.selectableAreaPos(0,0),
  this.selectableAreaSize(0,0) {
	addObjectType("ewol::widget::Button");
	
	// set property list:
	propertyLock.add(lockNone, "none");
	propertyLock.add(lockWhenPressed, "pressed");
	propertyLock.add(lockWhenReleased, "released");
	propertyLock.add(lockAccess, "access");
	
	propertyCanFocus.setDirectCheck(true);
	
	// shaper satatus update:
	CheckStatus();
	// Limit event at 1:
	setMouseLimit(1);
}

void ewol::widget::Button::init() {
	ewol::widget::Container2::init();
	propertyShape.notifyChange();
}

ewol::widget::Button::~Button() {
	
}

void ewol::widget::Button::onChangeSize() {
	ewol::Padding padding = this.shaper.getPadding();
	ewol::Padding ret = onChangeSizePadded(padding);
	//Log.debug(" configuring : origin=" + origin + " size=" + subElementSize + "");
	this.selectableAreaPos = Vector2f(ret.xLeft(), ret.yButtom());
	this.selectableAreaSize = this.size - (this.selectableAreaPos + Vector2f(ret.xRight(), ret.yTop()));
}


void ewol::widget::Button::calculateMinMaxSize() {
	ewol::Padding padding = this.shaper.getPadding();
	calculateMinMaxSizePadded(padding);
}

void ewol::widget::Button::onDraw() {
	// draw the shaaper (if needed indeed)
	this.shaper.draw();
}

void ewol::widget::Button::onRegenerateDisplay() {
	ewol::widget::Container2::onRegenerateDisplay();
	if (needRedraw() == false) {
		return;
	}
	ewol::Padding padding = this.shaper.getPadding();
	this.shaper.setShape(Vector2f(0,0),
	                  this.size,
	                  Vector2fClipInt32(this.selectableAreaPos+Vector2f(padding.xLeft(),padding.yButtom()) ),
	                  Vector2fClipInt32(this.selectableAreaSize-Vector2f(padding.x(),padding.y()) ) );
	//Log.error("pos=" + this.origin + " size=" + this.size);
}

boolean ewol::widget::Button::onEventInput( ewol::event::Input _event) {
	Log.verbose("Event on BT : " + _event);
	// disable event in the lock access mode :
	if(ewol::widget::Button::lockAccess == *propertyLock) {
		return false;
	}
	if(    _event.getStatus() == KeyStatus::leave
	    || _event.getStatus() == KeyStatus::abort) {
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
	Log.verbose("Event on BT ... mouse hover : " + this.mouseHover);
	if (this.mouseHover == true) {
		if (_event.getId() == 1) {
			if(_event.getStatus() == KeyStatus::down) {
				Log.verbose(*propertyName + " : Generate event : " + signalDown);
				signalDown.emit();
				this.buttonPressed = true;
				markToRedraw();
			}
			if(_event.getStatus() == KeyStatus::up) {
				Log.verbose(*propertyName + " : Generate event : " + signalUp);
				signalUp.emit();
				this.buttonPressed = false;
				markToRedraw();
			}
			if(_event.getStatus() == KeyStatus::pressSingle) {
				if (    (    *propertyValue == true
				          LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM *propertyLock == ewol::widget::Button::lockWhenPressed)
				     || (    *propertyValue == false
				          LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM *propertyLock == ewol::widget::Button::lockWhenReleased) ) {
					// nothing to do : Lock mode ...
					// user might set himself the new correct value with @ref setValue(xxx)
				} else {
					// inverse value :
					propertyValue.set((*propertyValue)?false:true);
					Log.verbose(*propertyName + " : Generate event : " + signalPressed);
					signalPressed.emit();
					Log.verbose(*propertyName + " : Generate event : " + signalValue + " val=" + *propertyValue );
					signalValue.emit(*propertyValue);
					if(    *propertyToggleMode == false
					    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM *propertyValue == true) {
						propertyValue.set(false);
						Log.verbose(*propertyName + " : Generate event : " + signalValue + " val=" + *propertyValue);
						signalValue.emit(*propertyValue);
					}
				}
				markToRedraw();
			}
		}
	}
	CheckStatus();
	return this.mouseHover;
}


boolean ewol::widget::Button::onEventEntry( ewol::event::Entry _event) {
	//Log.debug("BT PRESSED : \"" + UTF8_data + "\" size=" + strlen(UTF8_data));
	if(    _event.getType() == KeyKeyboard::character
	    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::down
	    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getChar() == '\r') {
		signalEnter.emit();
		return true;
	}
	return false;
}

void ewol::widget::Button::onLostFocus() {
	this.buttonPressed = false;
	Log.verbose(propertyName.get() + " : Remove Focus ...");
	CheckStatus();
}

void ewol::widget::Button::CheckStatus() {
	if (this.buttonPressed == true) {
		changeStatusIn(STATUS_PRESSED);
		return;
	}
	if (this.mouseHover == true) {
		changeStatusIn(STATUS_HOVER);
		return;
	}
	if (*propertyValue == true) {
		changeStatusIn(STATUS_DOWN);
	}
	changeStatusIn(STATUS_UP);
}

void ewol::widget::Button::changeStatusIn(int _newStatusId) {
	if (this.shaper.changeStatusIn(_newStatusId) == true) {
		this.PCH = getObjectManager().periodicCall.connect(this, ewol::widget::Button::periodicCall);
		markToRedraw();
	}
}


void ewol::widget::Button::periodicCall( ewol::event::Time _event) {
	if (this.shaper.periodicCall(_event) == false) {
		this.PCH.disconnect();
	}
	markToRedraw();
}

void ewol::widget::Button::onChangePropertyShape() {
	this.shaper.setSource(*propertyShape);
	markToRedraw();
}
void ewol::widget::Button::onChangePropertyValue() {
	if (*propertyToggleMode == true) {
		if (*propertyValue == false) {
			this.idWidgetDisplayed = 0;
		} else {
			this.idWidgetDisplayed = 1;
		}
	}
	if (*propertyEnableSingle == true) {
		if (    this.idWidgetDisplayed == 0
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.subWidget[0] == null
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.subWidget[1] != null) {
			this.idWidgetDisplayed = 1;
		} else if (    this.idWidgetDisplayed == 1
		            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.subWidget[1] == null
		            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.subWidget[0] != null) {
			this.idWidgetDisplayed = 0;
		}
	}
	CheckStatus();
	markToRedraw();
}

void ewol::widget::Button::onChangePropertyLock() {
	if(ewol::widget::Button::lockAccess == *propertyLock) {
		this.buttonPressed = false;
		this.mouseHover = false;
	}
	CheckStatus();
	markToRedraw();
}

void ewol::widget::Button::onChangePropertyToggleMode() {
	if (*propertyValue == true) {
		propertyValue.setDirect(false);
		// TODO : change display and send event ...
	}
	if (*propertyToggleMode == false) {
		this.idWidgetDisplayed = 0;
	} else {
		if (*propertyValue == false) {
			this.idWidgetDisplayed = 0;
		} else {
			this.idWidgetDisplayed = 1;
		}
	}
	if (*propertyEnableSingle == true) {
		if (    this.idWidgetDisplayed == 0
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.subWidget[0] == null
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.subWidget[1] != null) {
			this.idWidgetDisplayed = 1;
		} else if (    this.idWidgetDisplayed == 1
		            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.subWidget[1] == null
		            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.subWidget[0] != null) {
			this.idWidgetDisplayed = 0;
		}
	}
	CheckStatus();
	markToRedraw();
}

void ewol::widget::Button::onChangePropertyEnableSingle() {
	if (*propertyEnableSingle == true) {
		if (    this.idWidgetDisplayed == 0
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.subWidget[0] == null
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.subWidget[1] != null) {
			this.idWidgetDisplayed = 1;
		} else if (    this.idWidgetDisplayed == 1
		            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.subWidget[1] == null
		            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.subWidget[0] != null) {
			this.idWidgetDisplayed = 0;
		} else if (    this.subWidget[0] == null
		            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.subWidget[1] == null) {
			this.idWidgetDisplayed = 0;
		}
	}
}
