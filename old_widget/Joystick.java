package org.atriasoft.ewol.widget;

import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.Signal;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.Padding;
import org.atriasoft.ewol.annotation.EwolDescription;
import org.atriasoft.ewol.annotation.EwolSignal;
import org.atriasoft.ewol.compositing.GuiShape;
import org.atriasoft.ewol.compositing.GuiShapeMode;
import org.atriasoft.ewol.compositing.ShapeBox;
import org.atriasoft.ewol.event.EventEntry;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.event.EventTime;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.object.EwolObject;
import org.atriasoft.exml.annotation.XmlAttribute;
import org.atriasoft.exml.annotation.XmlManaged;
import org.atriasoft.exml.annotation.XmlName;
import org.atriasoft.gale.key.KeyKeyboard;
import org.atriasoft.gale.key.KeyStatus;

/**
 * @ingroup ewolWidgetGroup
 */
class Joystick extends Widget {
	private static boolean l_displayBackground(true);
	private static String l_background("");
	private static String l_foreground("");
	private static float l_ratio(1.0/7.0);
		// Event list of properties
	@EwolSignal(name = "enable")
	public SignalEmpty signalEnable = new SignalEmpty();
	@EwolSignal(name = "disable")
	public SignalEmpty signalDisable = new SignalEmpty();
	@EwolSignal(name = "move")
	public Signal<Vector2f> signalMove = new Signal<>();
	public enum joystickMode {
		modeNormal,
		modeArrow,
	};

	private 	Color colorFg = Color.BLUE.withA(0x3F); //!< Forground  color
	private 	Color colorBg = Color.BLACK; //!< Background color
	private 	Vector2f displayPos = Vector2f.ZERO; //!< direction of the cursor ...
	private 	float distance = 0.0f; //!< dintance from the center
	private 	float angle = -0.1f; //!< angle of the arraw (if < 0 : No arraw...) 0 is the TOP ...
	private 	boolean lock = false; //!< flag to mark the lock when the cursor is free when we are outside the circle
	private 	enum joystickMode displayMode = joystickMode.modeNormal; //!< Type of fonctionnal mode of the joystick
	// generic property of the joystick:
	private boolean displayBackground=  l_displayBackground;
	private String background = l_background;
	private String foreground = l_foreground;
	private float ratio = l_ratio;
	public Joystick() {
		
	}
	public void setLockMode(boolean _lockWhenOut) {
		this.lock = _lockWhenOut;
	};
	public void setDisplayMode(enum joystickMode _newMode) {
		this.displayMode = _newMode;
	};
	/**
	 * set the ratio of the widget joystick
	 * @param _newRatio the new ratio that might be set
	 */
	public void ratio(float _newRatio) {
		if (_newRatio > 1) {
			_newRatio = 1;
		}
		this.ratio = _newRatio;
		Log.info("Set default Joystick ratio at " + this.ratio);
	}
	/**
	 * set the Background of the widget joystick
	 * @param _imageNameInData the new rbackground that might be set
	 * @param _display
	 */
	public void background(String _imageNameInData, boolean _display=true) {
		// TODO : check if it existed
		this.background = _imageNameInData;
		this.displayBackground = _display;
		Log.info("Set default Joystick background at " + this.background + " display it=" + this.displayBackground);
	}
	/**
	 * set the Foreground of the widget joystick
	 * @param _imageNameInData the new Foreground that might be set
	 */
	public void foreground(String _imageNameInData) {
		// TODO : check if it existed
		this.foreground = imageNameInData;
		Log.info("Set default Joystick Foreground at " + this.foreground);
	}
	/**
	 * get the property of the joystick
	 * @param _distance distance to the center
	 * @param _angle angle of the joy
	 */
	public void getProperty(float _distance, float _angle) {
		distance = this.distance;
		angle = this.angle+M_PI/2;
	}
	
	public  void onRegenerateDisplay() {
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
	public  boolean onEventInput( ewol::event::Input _event) {
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
				    && this.distance == 1) {
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
}

