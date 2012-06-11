/**
 *******************************************************************************
 * @file ewol/widget/Slider.cpp
 * @brief ewol Slider widget system (Sources)
 * @author Edouard DUPIN
 * @date 06/03/2012
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

#include <ewol/widget/Slider.h>

#include <ewol/OObject.h>
#include <ewol/WidgetManager.h>


extern const char * const ewolEventSliderChange    = "ewol-event-slider-change";



#undef __class__
#define __class__	"Slider"

const int32_t dotRadius = 6;

ewol::Slider::Slider(void)
{
	AddEventId(ewolEventSliderChange);
	
	m_value = 0;
	m_min = 0;
	m_max = 10;
	
	m_textColorFg = etk::color::color_Black;
	
	m_textColorBg.red   = 0;
	m_textColorBg.green = 0;
	m_textColorBg.blue  = 0;
	m_textColorBg.alpha = 0x3F;
	SetCanHaveFocus(true);
}

ewol::Slider::~Slider(void)
{
	
}


//!< EObject name :
extern const char * const ewol::TYPE_EOBJECT_WIDGET_SLIDER = "Slider";

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool ewol::Slider::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_SLIDER << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == ewol::TYPE_EOBJECT_WIDGET_SLIDER) {
		return true;
	} else {
		if(true == ewol::Drawable::CheckObjectType(objectType)) {
			return true;
		}
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_SLIDER << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const ewol::Slider::GetObjectType(void)
{
	return ewol::TYPE_EOBJECT_WIDGET_SLIDER;
}




bool ewol::Slider::CalculateMinSize(void)
{
	m_minSize.x = etk_max(m_userMinSize.x, 40);
	m_minSize.y = etk_max(m_userMinSize.y, dotRadius*2);
	MarkToReedraw();
	return true;
}


void ewol::Slider::SetValue(int32_t val)
{
	m_value = etk_max(etk_min(val, m_max), m_min);
	MarkToReedraw();
}


int32_t ewol::Slider::GetValue(void)
{
	return m_value;
}


void ewol::Slider::SetMin(int32_t val)
{
	m_min = val;
	m_value = etk_max(etk_min(m_value, m_max), m_min);
	MarkToReedraw();
}


void ewol::Slider::SetMax(int32_t val)
{
	m_max = val;
	m_value = etk_max(etk_min(m_value, m_max), m_min);
	MarkToReedraw();
}


void ewol::Slider::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		// clean the object list ...
		ClearOObjectList();
		
		ewol::OObject2DColored * tmpOObjects = new ewol::OObject2DColored;
		
		tmpOObjects->SetColor(m_textColorFg);
		// draw a line :
		tmpOObjects->Line(dotRadius, m_size.y/2, m_size.x-dotRadius, m_size.y/2, 1);
		color_ts borderDot = m_textColorFg;
		borderDot.alpha /= 2;
		tmpOObjects->SetColor(borderDot);
		tmpOObjects->Disc(4+((float)(m_value-m_min)/(float)(m_max-m_min))*(float)(m_size.x-2*dotRadius), m_size.y/2, dotRadius);
		
		tmpOObjects->SetColor(m_textColorFg);
		tmpOObjects->Disc(4+((float)(m_value-m_min)/(float)(m_max-m_min))*(float)(m_size.x-2*dotRadius), m_size.y/2, dotRadius/1.6);
		
		AddOObject(tmpOObjects);
	}
}


/**
 * @brief Event on an input of this Widget
 * @param[in] type Type of the input (ewol::INPUT_TYPE_MOUSE/ewol::INPUT_TYPE_FINGER ...)
 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
 * @param[in] pos Absolute position of the event
 * @return true the event is used
 * @return false the event is not used
 */
bool ewol::Slider::OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, Vector2D<float> pos)
{
	Vector2D<float> relativePos = RelativePosition(pos);
	//EWOL_DEBUG("Event on Slider ...");
	if (1 == IdInput) {
		if(    ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_DOUBLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_TRIPLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_MOVE   == typeEvent) {
			// get the new position :
			EWOL_DEBUG("Event on Slider (" << relativePos.x << "," << relativePos.y << ")");
			int32_t oldValue = m_value;
			m_value = m_min + (float)(relativePos.x - dotRadius) / (float)(m_size.x-2*dotRadius) * (float)(m_max-m_min);
			m_value = etk_max(etk_min(m_value, m_max), m_min);
			if (oldValue != m_value) {
				EWOL_DEBUG(" new value : " << m_value << " in [" << m_min << ".." << m_max << "]");
				GenerateEventId(ewolEventSliderChange);
				MarkToReedraw();
			}
			return true;
		}
	}
	return false;
}


