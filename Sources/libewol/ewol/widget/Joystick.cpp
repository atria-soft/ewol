/**
 *******************************************************************************
 * @file ewol/widget/Joystick.cpp
 * @brief ewol Joystick widget system (Sources)
 * @author Edouard DUPIN
 * @date 30/03/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#include <ewol/widget/Joystick.h>
#include <math.h>

#include <ewol/OObject.h>
#include <ewol/WidgetManager.h>

extern const char * const ewolEventJoystickEnable   = "ewol-joystick-enable";
extern const char * const ewolEventJoystickDisable  = "ewol-joystick-disable";
extern const char * const ewolEventJoystickMove     = "ewol-joystick-move";

static bool         l_displayBackground(true);
static etk::UString l_background("");
static etk::UString l_foreground("");
static float   l_ratio(1.0/7.0);

/**
 * @brief Initilise the basic widget property ==> due to the android system
 * @note all widget that have template might have this initializer ...
 * @param ---
 * @return ---
 */
void ewol::WIDGET_JoystickInit(void)
{
	l_displayBackground = true;
	l_background = "";
	l_foreground = "";
	l_ratio = 1.0/7.0;
}

/**
 * @brief Set the default ratio of the widget joystick
 * @param[in] newRatio the new ratio that might be set
 * @return ---
 */
void ewol::WIDGET_JoystickDefaultRatio(float newRatio)
{
	if (newRatio > 1) {
		newRatio = 1;
	}
	l_ratio = newRatio;
	EWOL_INFO("Set default Joystick ratio at " << l_ratio);
}

/**
 * @brief Set the default Background of the widget joystick
 * @param[in] imageNameInData the new rbackground that might be set
 * @return ---
 */
void ewol::WIDGET_JoystickDefaultBackground(etk::UString imageNameInData, bool display)
{
	// TODO : check if it existed
	l_background = imageNameInData;
	l_displayBackground = display;
	EWOL_INFO("Set default Joystick background at " << l_background << " Display it=" << l_displayBackground);
}

/**
 * @brief Set the default Foreground of the widget joystick
 * @param[in] imageNameInData the new Foreground that might be set
 * @return ---
 */
void ewol::WIDGET_JoystickDefaultForeground(etk::UString imageNameInData)
{
	// TODO : check if it existed
	l_foreground = imageNameInData;
	EWOL_INFO("Set default Joystick Foreground at " << l_foreground);
}


#undef __class__
#define __class__	"Joystick"

ewol::Joystick::Joystick(void)
{
	AddEventId(ewolEventJoystickEnable);
	AddEventId(ewolEventJoystickDisable);
	AddEventId(ewolEventJoystickMove);
	// by default the joy does not lock when free out
	m_lock = false;
	m_displayMode = ewol::JOYSTICK_NORMAL_MODE;
	
	m_colorFg = etk::color::color_Blue;
	
	m_colorBg = etk::color::color_Black;
	m_colorBg.alpha = 0x3F;
	
	m_displayPos.x = 0.0;
	m_displayPos.y = 0.0;
	m_distance = 0.0;
	m_angle = -0.1;
	
	// set the generic parameters:
	m_displayBackground = l_displayBackground;
	m_background = l_background;
	m_foreground = l_foreground;
	m_ratio = l_ratio;
	SetCanHaveFocus(true);
}


ewol::Joystick::~Joystick(void)
{
	
}


/**
 * @brief Parrent set the possible diplay size of the current widget whith his own possibilities
 *        By default this save the widget availlable size in the widget size
 * @param[in] availlableX Availlable horisantal pixel size
 * @param[in] availlableY Availlable vertical pixel size
 * @return ---
 */
bool ewol::Joystick::CalculateSize(float availlableX, float availlableY)
{
	float minimumSize = etk_min(availlableX, availlableY);
	m_size.x = minimumSize;
	m_size.y = minimumSize;
	MarkToRedraw();
	return true;
}

void ewol::Joystick::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		// clean the object list ...
		ClearOObjectList();
		
		ewol::OObject2DColored * tmpOObjects = NULL;
		ewol::OObject2DTextured * tmpOOtexBg = NULL;
		ewol::OObject2DTextured * tmpOOtexFg = NULL;
		// set background
		if (true == m_displayBackground) {
			if (m_background == "") {
				tmpOObjects = new ewol::OObject2DColored;
				tmpOObjects->SetColor(m_colorBg);
				tmpOObjects->Disc( m_size.x/2, m_size.y/2, m_size.x/2-1);
			} else {
				tmpOOtexBg = new ewol::OObject2DTextured(m_background, m_size.x, m_size.y);
				tmpOOtexBg->Rectangle(0, 0, m_size.x, m_size.y);
			}
		}
		// set cursor point
		float sizeElement = m_size.x*m_ratio;
		if (m_foreground == "") {
			if (NULL == tmpOObjects) {
				tmpOObjects = new ewol::OObject2DColored;
			}
			tmpOObjects->SetColor(m_colorFg);
			tmpOObjects->Disc( ((m_displayPos.x+1.0)/2.0)*(m_size.x-2*sizeElement) + sizeElement,
			                   ((m_displayPos.y+1.0)/2.0)*(m_size.y-2*sizeElement) + sizeElement, sizeElement);
		} else {
			tmpOOtexFg = new ewol::OObject2DTextured(m_foreground,sizeElement*2, sizeElement*2);
			tmpOOtexFg->Rectangle(((m_displayPos.x+1.0)/2.0)*(m_size.x-2*sizeElement),
			                      ((m_displayPos.y+1.0)/2.0)*(m_size.y-2*sizeElement), sizeElement*2, sizeElement*2);
		}
		// add all needed objects ...
		if (NULL != tmpOObjects) {
			AddOObject(tmpOObjects);
		}
		if (NULL != tmpOOtexBg) {
			AddOObject(tmpOOtexBg);
		}
		if (NULL != tmpOOtexFg) {
			AddOObject(tmpOOtexFg);
		}
	}
}

/*
Sine Function:    sin(teta) = Opposite / Hypotenuse
Cosine Function:  cos(teta) = Adjacent / Hypotenuse
Tangent Function: tan(teta) = Opposite / Adjacent
*/

/**
 * @brief Event on an input of this Widget
 * @param[in] type Type of the input (ewol::INPUT_TYPE_MOUSE/ewol::INPUT_TYPE_FINGER ...)
 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
 * @param[in] pos Absolute position of the event
 * @return true the event is used
 * @return false the event is not used
 */
bool ewol::Joystick::OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, Vector2D<float> pos)
{
	if (1 == IdInput) {
		if(    ewol::EVENT_INPUT_TYPE_DOWN == typeEvent
		    || ewol::EVENT_INPUT_TYPE_MOVE == typeEvent) {
			// get local relative position
			Vector2D<float> relativePos = RelativePosition(pos);
			float sizeElement = m_size.x*m_ratio;
			// Calculate the position of the cursor...
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
			MarkToRedraw();
			if(ewol::EVENT_INPUT_TYPE_DOWN == typeEvent) {
				GenerateEventId(ewolEventJoystickEnable);
			} else {
				etk::UString tmp = etk::UString("distance=") + etk::UString(m_distance) + etk::UString("angle=") + etk::UString(m_angle+M_PI/2);
				GenerateEventId(ewolEventJoystickMove, tmp);
			}
			//teta += M_PI/2;
			//EWOL_DEBUG("TETA = " << (m_angle*180/M_PI) << " deg distance = " << m_distance);
			return true;
		} else if( ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
			if(    true == m_lock
			    && m_distance == 1) {
				// nothing to do ...
			} else {
				m_displayPos.x = 0.0;
				m_displayPos.y = 0.0;
				m_angle = -0.1;
				m_distance = 0;
			}
			MarkToRedraw();
			GenerateEventId(ewolEventJoystickDisable);
			return true;
		}
		return false;
	}
	return false;
}


/**
 * @brief Set the ratio of the widget joystick
 * @param[in] newRatio the new ratio that might be set
 * @return ---
 */
void ewol::Joystick::Ratio(float newRatio)
{
	if (newRatio > 1) {
		newRatio = 1;
	}
	m_ratio = newRatio;
	EWOL_INFO("Set default Joystick ratio at " << m_ratio);
}

/**
 * @brief Set the Background of the widget joystick
 * @param[in] imageNameInData the new rbackground that might be set
 * @return ---
 */
void ewol::Joystick::Background(etk::UString imageNameInData, bool display)
{
	// TODO : check if it existed
	m_background = imageNameInData;
	m_displayBackground = display;
	EWOL_INFO("Set default Joystick background at " << m_background << " Display it=" << m_displayBackground);
}

/**
 * @brief Set the Foreground of the widget joystick
 * @param[in] imageNameInData the new Foreground that might be set
 * @return ---
 */
void ewol::Joystick::Foreground(etk::UString imageNameInData)
{
	// TODO : check if it existed
	m_foreground = imageNameInData;
	EWOL_INFO("Set default Joystick Foreground at " << m_foreground);
}


/**
 * @brief Get the property of the joystick
 * @param[out] distance distance to the center
 * @param[out] angle angle of the joy
 * @return ---
 */
void ewol::Joystick::GetProperty(float& distance, float& angle)
{
	distance = m_distance;
	angle = m_angle+M_PI/2;
}


