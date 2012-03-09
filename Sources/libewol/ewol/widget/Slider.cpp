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
	
	m_textColorFg.red   = 0.0;
	m_textColorFg.green = 0.0;
	m_textColorFg.blue  = 0.0;
	m_textColorFg.alpha = 1.0;
	
	m_textColorBg.red   = 0.0;
	m_textColorBg.green = 0.0;
	m_textColorBg.blue  = 0.0;
	m_textColorBg.alpha = 0.25;
	SetCanHaveFocus(true);
}

ewol::Slider::~Slider(void)
{
	
}



bool ewol::Slider::CalculateMinSize(void)
{
	m_minSize.x = 40;
	m_minSize.y = dotRadius*2;
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
		
		tmpOObjects->Disc(4+((etkFloat_t)(m_value-m_min)/(etkFloat_t)(m_max-m_min))*(etkFloat_t)(m_size.x-2*dotRadius), m_size.y/2, dotRadius);
		
		AddOObject(tmpOObjects);
	}
}


/**
 * @brief Event on an input of this Widget
 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
 * @param[in] pos Absolute position of the event
 * @return true the event is used
 * @return false the event is not used
 */
bool ewol::Slider::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, coord2D_ts pos)
{
	coord2D_ts relativePos = RelativePosition(pos);
	//EWOL_DEBUG("Event on Slider ...");
	if (1 == IdInput) {
		if(    ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_DOUBLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_TRIPLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_MOVE   == typeEvent) {
			// get the new position :
			EWOL_DEBUG("Event on Slider (" << relativePos.x << "," << relativePos.y << ")");
			m_value = m_min + (etkFloat_t)(relativePos.x - dotRadius) / (etkFloat_t)(m_size.x-2*dotRadius) * (etkFloat_t)(m_max-m_min);
			m_value = etk_max(etk_min(m_value, m_max), m_min);
			EWOL_DEBUG(" new value : " << m_value << " in [" << m_min << ".." << m_max << "]");
			GenerateEventId(ewolEventSliderChange);
			MarkToReedraw();
			return true;
		}
	}
	return false;
}


