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

static bool l_displayBackground(true);
static etk::String l_background("");
static etk::String l_foreground("");
static float l_ratio(1.0/7.0);

ewol::widget::Joystick::Joystick() :
  signalEnable(this, "enable", ""),
  signalDisable(this, "disable", ""),
  signalMove(this, "move", "") {
	addObjectType("ewol::widget::Joystick");
	// by default the joy does not lock when free out
	m_lock = false;
	m_displayMode = modeNormal;
	
	m_colorFg = etk::color::blue;
	
	m_colorBg = etk::color::black;
	m_colorBg.setA(0x3F);
	
	m_displayPos.setValue(0,0);
	m_distance = 0.0;
	m_angle = -0.1;
	
	// set the generic parameters:
	m_displayBackground = l_displayBackground;
	m_background = l_background;
	m_foreground = l_foreground;
	m_ratio = l_ratio;
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
		if (true == m_displayBackground) {
			if (m_background == "") {
				tmpOObjects = ne w ewol::OObject2DColored;
				tmpOObjects->setColor(m_colorBg);
				tmpOObjects->Disc( m_size.x/2, m_size.y/2, m_size.x/2-1);
			} else {
				tmpOOtexBg = n ew ewol::OObject2DTextured(m_background, m_size.x, m_size.y);
				tmpOOtexBg->rectangle(0, 0, m_size.x, m_size.y);
			}
		}
		// set cursor point
		float sizeElement = m_size.x*m_ratio;
		if (m_foreground == "") {
			if (null == tmpOObjects) {
				tmpOObjects = ne w ewol::OObject2DColored;
			}
			tmpOObjects->setColor(m_colorFg);
			tmpOObjects->Disc( ((m_displayPos.x+1.0)/2.0)*(m_size.x-2*sizeElement) + sizeElement,
			                   ((m_displayPos.y+1.0)/2.0)*(m_size.y-2*sizeElement) + sizeElement, sizeElement);
		} else {
			tmpOOtexFg = ne w ewol::OObject2DTextured(m_foreground,sizeElement*2, sizeElement*2);
			tmpOOtexFg->rectangle(((m_displayPos.x+1.0)/2.0)*(m_size.x-2*sizeElement),
			                      ((m_displayPos.y+1.0)/2.0)*(m_size.y-2*sizeElement), sizeElement*2, sizeElement*2);
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
bool ewol::widget::Joystick::onEventInput(const ewol::event::Input& _event) {
/*
	if (1 == IdInput) {
		if(    gale::key::status::down == typeEvent
		    || gale::key::status::move == typeEvent) {
			// get local relative position
			vec2 relativePos = relativePosition(pos);
			float sizeElement = m_size.x*m_ratio;
			// calculate the position of the cursor...
			m_displayPos.x = (relativePos.x-sizeElement)/(m_size.x-sizeElement*2)*2.0 - 1.0;
			m_displayPos.y = (relativePos.y-sizeElement)/(m_size.y-sizeElement*2)*2.0 - 1.0;
			
			// distance :
			m_distance = m_displayPos.y*m_displayPos.y + m_displayPos.x * m_displayPos.x;
			m_distance = sqrt(m_distance);
			// angle : 
			m_angle = atan(m_displayPos.y/m_displayPos.x);
			if (m_displayPos.x < 0) {
				m_angle += M_PI;
			}
			
			// clip if needed ...
			if (m_distance > 1.0) {
				m_distance = 1.0;
				// regenerate n ew display position : 
				m_displayPos.x = cos(m_angle)*m_distance;
				m_displayPos.y = sin(m_angle)*m_distance;
			}
			markToRedraw();
			if(gale::key::status::down == typeEvent) {
				signalEnable.emit();
			} else {
				etk::String tmp = etk::String("distance=") + etk::String(m_distance) + etk::String("angle=") + etk::String(m_angle+M_PI/2);
				signalMove.emit(m_angle+M_PI/2);
			}
			//teta += M_PI/2;
			//EWOL_DEBUG("TETA = " << (m_angle*180/M_PI) << " deg distance = " << m_distance);
			return true;
		} else if( gale::key::status::up == typeEvent) {
			if(    true == m_lock
			    && m_distance == 1) {
				// nothing to do ...
			} else {
				m_displayPos.x = 0.0;
				m_displayPos.y = 0.0;
				m_angle = -0.1;
				m_distance = 0;
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
	m_ratio = _newRatio;
	EWOL_INFO("Set default Joystick ratio at " << m_ratio);
}


void ewol::widget::Joystick::background(etk::String _imageNameInData, bool _display) {
	// TODO : check if it existed
	m_background = _imageNameInData;
	m_displayBackground = _display;
	EWOL_INFO("Set default Joystick background at " << m_background << " display it=" << m_displayBackground);
}


void ewol::widget::Joystick::foreground(etk::String imageNameInData) {
	// TODO : check if it existed
	m_foreground = imageNameInData;
	EWOL_INFO("Set default Joystick Foreground at " << m_foreground);
}


void ewol::widget::Joystick::getProperty(float& distance, float& angle) {
	distance = m_distance;
	angle = m_angle+M_PI/2;
}


