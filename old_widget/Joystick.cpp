/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/Joystick.hpp>
#include <cmath>

#include <ewol/compositing/Image.hpp>
#include <ewol/widget/Manager.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::Joystick);

static boolean l_displayBackground(true);
static String l_background("");
static String l_foreground("");
static float l_ratio(1.0/7.0);

ewol::widget::Joystick::Joystick() :
  signalEnable(this, "enable", ""),
  signalDisable(this, "disable", ""),
  signalMove(this, "move", "") {
	addObjectType("ewol::widget::Joystick");
	// by default the joy does not lock when free out
	this.lock = false;
	this.displayMode = modeNormal;
	
	this.colorFg = etk::color::blue;
	
	this.colorBg = etk::color::black;
	this.colorBg.setA(0x3F);
	
	this.displayPos.setValue(0,0);
	this.distance = 0.0;
	this.angle = -0.1;
	
	// set the generic parameters:
	this.displayBackground = l_displayBackground;
	this.background = l_background;
	this.foreground = l_foreground;
	this.ratio = l_ratio;
	propertyCanFocus.setDirectCheck(true);
}

ewol::widget::Joystick::~Joystick() {
	
}

void ewol::widget::Joystick::onRegenerateDisplay() {
	if (needRedraw() == true) {
		// clean the object list ...
		
		/*
		ewol::OObject2DColored * tmpOObjects = null;
		ewol::OObject2DTextured * tmpOOtexBg = null;
		ewol::OObject2DTextured * tmpOOtexFg = null;
		// set background
		if (true == this.displayBackground) {
			if (this.background == "") {
				tmpOObjects = ne w ewol::OObject2DColored;
				tmpOObjects.setColor(this.colorBg);
				tmpOObjects.Disc( this.size.x/2, this.size.y/2, this.size.x/2-1);
			} else {
				tmpOOtexBg = n ew ewol::OObject2DTextured(this.background, this.size.x, this.size.y);
				tmpOOtexBg.rectangle(0, 0, this.size.x, this.size.y);
			}
		}
		// set cursor point
		float sizeElement = this.size.x*this.ratio;
		if (this.foreground == "") {
			if (null == tmpOObjects) {
				tmpOObjects = ne w ewol::OObject2DColored;
			}
			tmpOObjects.setColor(this.colorFg);
			tmpOObjects.Disc( ((this.displayPos.x+1.0)/2.0)*(this.size.x-2*sizeElement) + sizeElement,
			                   ((this.displayPos.y+1.0)/2.0)*(this.size.y-2*sizeElement) + sizeElement, sizeElement);
		} else {
			tmpOOtexFg = ne w ewol::OObject2DTextured(this.foreground,sizeElement*2, sizeElement*2);
			tmpOOtexFg.rectangle(((this.displayPos.x+1.0)/2.0)*(this.size.x-2*sizeElement),
			                      ((this.displayPos.y+1.0)/2.0)*(this.size.y-2*sizeElement), sizeElement*2, sizeElement*2);
		}
		// add all needed objects ...
		if (null != tmpOObjects) {
			addOObject(tmpOObjects);
		}
		if (null != tmpOOtexBg) {
			addOObject(tmpOOtexBg);
		}
		if (null != tmpOOtexFg) {
			addOObject(tmpOOtexFg);
		}
		*/
	}
}

/*
Sine Function:    sin(teta) = Opposite / Hypotenuse
Cosine Function:  cos(teta) = Adjacent / Hypotenuse
Tangent Function: tan(teta) = Opposite / Adjacent
*/
boolean ewol::widget::Joystick::onEventInput( ewol::event::Input _event) {
/*
	if (1 == IdInput) {
		if(    KeyStatus::down == typeEvent
		    || KeyStatus::move == typeEvent) {
			// get local relative position
			Vector2f relativePos = relativePosition(pos);
			float sizeElement = this.size.x*this.ratio;
			// calculate the position of the cursor...
			this.displayPos.x = (relativePos.x-sizeElement)/(this.size.x-sizeElement*2)*2.0 - 1.0;
			this.displayPos.y = (relativePos.y-sizeElement)/(this.size.y-sizeElement*2)*2.0 - 1.0;
			
			// distance :
			this.distance = this.displayPos.y*this.displayPos.y + this.displayPos.x * this.displayPos.x;
			this.distance = sqrt(this.distance);
			// angle : 
			this.angle = atan(this.displayPos.y/this.displayPos.x);
			if (this.displayPos.x < 0) {
				this.angle += M_PI;
			}
			
			// clip if needed ...
			if (this.distance > 1.0) {
				this.distance = 1.0;
				// regenerate n ew display position : 
				this.displayPos.x = cos(this.angle)*this.distance;
				this.displayPos.y = sin(this.angle)*this.distance;
			}
			markToRedraw();
			if(KeyStatus::down == typeEvent) {
				signalEnable.emit();
			} else {
				String tmp = String("distance=") + String(this.distance) + String("angle=") + String(this.angle+M_PI/2);
				signalMove.emit(this.angle+M_PI/2);
			}
			//teta += M_PI/2;
			//Log.debug("TETA = " + (this.angle*180/M_PI) + " deg distance = " + this.distance);
			return true;
		} else if( KeyStatus::up == typeEvent) {
			if(    true == this.lock
			    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.distance == 1) {
				// nothing to do ...
			} else {
				this.displayPos.x = 0.0;
				this.displayPos.y = 0.0;
				this.angle = -0.1;
				this.distance = 0;
			}
			markToRedraw();
			signalDisable.emit();
			return true;
		}
		return false;
	}
	*/
	return false;
}


void ewol::widget::Joystick::ratio(float _newRatio) {
	if (_newRatio > 1) {
		_newRatio = 1;
	}
	this.ratio = _newRatio;
	Log.info("Set default Joystick ratio at " + this.ratio);
}


void ewol::widget::Joystick::background(String _imageNameInData, boolean _display) {
	// TODO : check if it existed
	this.background = _imageNameInData;
	this.displayBackground = _display;
	Log.info("Set default Joystick background at " + this.background + " display it=" + this.displayBackground);
}


void ewol::widget::Joystick::foreground(String imageNameInData) {
	// TODO : check if it existed
	this.foreground = imageNameInData;
	Log.info("Set default Joystick Foreground at " + this.foreground);
}


void ewol::widget::Joystick::getProperty(float distance, float angle) {
	distance = this.distance;
	angle = this.angle+M_PI/2;
}


