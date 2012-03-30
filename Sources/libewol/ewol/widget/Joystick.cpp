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
	
	m_colorFg.red   = 1.0;
	m_colorFg.green = 0.0;
	m_colorFg.blue  = 0.0;
	m_colorFg.alpha = 1.0;
	
	m_colorBg.red   = 0.0;
	m_colorBg.green = 0.0;
	m_colorBg.blue  = 0.0;
	m_colorBg.alpha = 0.25;
	
	m_displayPos.x = 0.0;
	m_displayPos.y = 0.0;
	m_distance = 0.0;
	m_angle = -0.1;
	
	SetCanHaveFocus(true);
}


ewol::Joystick::~Joystick(void)
{
	
}

//!< EObject name :
extern const char * const ewol::TYPE_EOBJECT_WIDGET_JOYSTICK = "Joystick";

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool ewol::Joystick::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_JOYSTICK << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == ewol::TYPE_EOBJECT_WIDGET_JOYSTICK) {
		return true;
	} else {
		if(true == ewol::Drawable::CheckObjectType(objectType)) {
			return true;
		}
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_JOYSTICK << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const ewol::Joystick::GetObjectType(void)
{
	return ewol::TYPE_EOBJECT_WIDGET_JOYSTICK;
}


/**
 * @brief Parrent set the possible diplay size of the current widget whith his own possibilities
 *        By default this save the widget availlable size in the widget size
 * @param[in] availlableX Availlable horisantal pixel size
 * @param[in] availlableY Availlable vertical pixel size
 * @return ---
 */
bool ewol::Joystick::CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY)
{
	etkFloat_t minimumSize = etk_min(availlableX, availlableY);
	m_size.x = minimumSize;
	m_size.y = minimumSize;
	MarkToReedraw();
	return true;
}
#define INTERNAL_ELEMENT_RATIO			(7)
void ewol::Joystick::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		// clean the object list ...
		ClearOObjectList();
		
		ewol::OObject2DColored * tmpOObjects = new ewol::OObject2DColored;
		// set background
		tmpOObjects->SetColor(m_colorBg);
		if (true) { //(ewol::JOYSTICK_CIRCLE_MODE==m_displayMode) {
			tmpOObjects->Disc( m_size.x/2, m_size.y/2, m_size.x/2-1);
		} else {
			tmpOObjects->Rectangle( 2, 2, m_size.x-4, m_size.y-4);
		}
		// set cursor point
		tmpOObjects->SetColor(m_colorFg);
		etkFloat_t sizeElement = m_size.x/INTERNAL_ELEMENT_RATIO;
		if (true) { //(ewol::JOYSTICK_CIRCLE_MODE==m_displayMode) {
			tmpOObjects->Disc( ((m_displayPos.x+1.0)/2.0)*m_size.x, ((m_displayPos.y+1.0)/2.0)*m_size.y, sizeElement);
		} else {
			tmpOObjects->Rectangle( ((m_displayPos.x+1.0)/2.0)*m_size.x-sizeElement/2.0, ((m_displayPos.y+1.0)/2.0)*m_size.y-sizeElement/2.0, sizeElement, sizeElement);
		}
		// add all needed objects ...
		if (NULL != tmpOObjects) {
			AddOObject(tmpOObjects);
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
 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
 * @param[in] pos Absolute position of the event
 * @return true the event is used
 * @return false the event is not used
 */
bool ewol::Joystick::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, coord2D_ts pos)
{
	if (1 == IdInput) {
		if(    ewol::EVENT_INPUT_TYPE_DOWN == typeEvent
		    || ewol::EVENT_INPUT_TYPE_MOVE == typeEvent) {
			// get local relative position
			coord2D_ts relativePos = RelativePosition(pos);
			// Calculate the position of the cursor...
			m_displayPos.x = relativePos.x/m_size.x*2.0 - 1.0;
			m_displayPos.y = relativePos.y/m_size.y*2.0 - 1.0;
			
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
			MarkToReedraw();
			if(ewol::EVENT_INPUT_TYPE_DOWN == typeEvent) {
				GenerateEventId(ewolEventJoystickEnable);
			} else {
				GenerateEventId(ewolEventJoystickMove);
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
			MarkToReedraw();
			GenerateEventId(ewolEventJoystickDisable);
			return true;
		}
		return false;
	}
	return false;
}

