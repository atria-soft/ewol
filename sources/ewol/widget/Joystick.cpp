/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Joystick.h>
#include <math.h>

#include <ewol/compositing/Image.h>
#include <ewol/widget/Manager.h>

const char * const ewol::Widget::Joystick::eventEnable   = "enable";
const char * const ewol::Widget::Joystick::eventDisable  = "disable";
const char * const ewol::Widget::Joystick::eventMove     = "move";

static bool         l_displayBackground(true);
static std::string l_background("");
static std::string l_foreground("");
static float   l_ratio(1.0/7.0);

#undef __class__
#define __class__ "Joystick"

ewol::Widget::Joystick::Joystick() {
	addObjectType("ewol::Widget::Joystick");
	addEventId(eventEnable);
	addEventId(eventDisable);
	addEventId(eventMove);
	// by default the joy does not lock when free out
	m_lock = false;
	m_displayMode = modeNormal;
	
	m_colorFg = draw::color::blue;
	
	m_colorBg = draw::color::black;
	m_colorBg.a = 0x3F;
	
	m_displayPos.setValue(0,0);
	m_distance = 0.0;
	m_angle = -0.1;
	
	// set the generic parameters:
	m_displayBackground = l_displayBackground;
	m_background = l_background;
	m_foreground = l_foreground;
	m_ratio = l_ratio;
	setCanHaveFocus(true);
}


ewol::Widget::Joystick::~Joystick() {
	
}


void ewol::Widget::Joystick::calculateSize(const vec2& availlable) {
	float minimumSize = etk_min(availlable.x(), availlable.y());
	m_size.setValue(minimumSize, minimumSize);
	markToRedraw();
}

void ewol::Widget::Joystick::onRegenerateDisplay() {
	if (true == needRedraw()) {
		// clean the object list ...
		
		/*
		ewol::OObject2DColored * tmpOObjects = NULL;
		ewol::OObject2DTextured * tmpOOtexBg = NULL;
		ewol::OObject2DTextured * tmpOOtexFg = NULL;
		// set background
		if (true == m_displayBackground) {
			if (m_background == "") {
				tmpOObjects = new ewol::OObject2DColored;
				tmpOObjects->setColor(m_colorBg);
				tmpOObjects->Disc( m_size.x/2, m_size.y/2, m_size.x/2-1);
			} else {
				tmpOOtexBg = new ewol::OObject2DTextured(m_background, m_size.x, m_size.y);
				tmpOOtexBg->rectangle(0, 0, m_size.x, m_size.y);
			}
		}
		// set cursor point
		float sizeElement = m_size.x*m_ratio;
		if (m_foreground == "") {
			if (NULL == tmpOObjects) {
				tmpOObjects = new ewol::OObject2DColored;
			}
			tmpOObjects->setColor(m_colorFg);
			tmpOObjects->Disc( ((m_displayPos.x+1.0)/2.0)*(m_size.x-2*sizeElement) + sizeElement,
			                   ((m_displayPos.y+1.0)/2.0)*(m_size.y-2*sizeElement) + sizeElement, sizeElement);
		} else {
			tmpOOtexFg = new ewol::OObject2DTextured(m_foreground,sizeElement*2, sizeElement*2);
			tmpOOtexFg->rectangle(((m_displayPos.x+1.0)/2.0)*(m_size.x-2*sizeElement),
			                      ((m_displayPos.y+1.0)/2.0)*(m_size.y-2*sizeElement), sizeElement*2, sizeElement*2);
		}
		// add all needed objects ...
		if (NULL != tmpOObjects) {
			addOObject(tmpOObjects);
		}
		if (NULL != tmpOOtexBg) {
			addOObject(tmpOOtexBg);
		}
		if (NULL != tmpOOtexFg) {
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
bool ewol::Widget::Joystick::onEventInput(const ewol::event::Input& _event) {
/*
	if (1 == IdInput) {
		if(    ewol::key::statusDown == typeEvent
		    || ewol::key::statusMove == typeEvent) {
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
				// regenerate new display position : 
				m_displayPos.x = cos(m_angle)*m_distance;
				m_displayPos.y = sin(m_angle)*m_distance;
			}
			markToRedraw();
			if(ewol::key::statusDown == typeEvent) {
				generateEventId(eventEnable);
			} else {
				std::string tmp = std::string("distance=") + std::string(m_distance) + std::string("angle=") + std::string(m_angle+M_PI/2);
				generateEventId(eventMove, tmp);
			}
			//teta += M_PI/2;
			//EWOL_DEBUG("TETA = " << (m_angle*180/M_PI) << " deg distance = " << m_distance);
			return true;
		} else if( ewol::key::statusUp == typeEvent) {
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
			generateEventId(eventDisable);
			return true;
		}
		return false;
	}
	*/
	return false;
}


void ewol::Widget::Joystick::ratio(float newRatio) {
	if (newRatio > 1) {
		newRatio = 1;
	}
	m_ratio = newRatio;
	EWOL_INFO("Set default Joystick ratio at " << m_ratio);
}


void ewol::Widget::Joystick::background(std::string imageNameInData, bool display) {
	// TODO : check if it existed
	m_background = imageNameInData;
	m_displayBackground = display;
	EWOL_INFO("Set default Joystick background at " << m_background << " display it=" << m_displayBackground);
}


void ewol::Widget::Joystick::foreground(std::string imageNameInData) {
	// TODO : check if it existed
	m_foreground = imageNameInData;
	EWOL_INFO("Set default Joystick Foreground at " << m_foreground);
}


void ewol::Widget::Joystick::getProperty(float& distance, float& angle) {
	distance = m_distance;
	angle = m_angle+M_PI/2;
}


