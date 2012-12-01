/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Slider.h>

#include <ewol/compositing/Drawing.h>
#include <ewol/widget/WidgetManager.h>


extern const char * const ewolEventSliderChange    = "ewol-event-slider-change";



#undef __class__
#define __class__	"Slider"

const int32_t dotRadius = 6;

widget::Slider::Slider(void)
{
	AddEventId(ewolEventSliderChange);
	
	m_value = 0;
	m_min = 0;
	m_max = 10;
	
	m_textColorFg = draw::color::black;
	
	m_textColorBg = draw::color::black;
	m_textColorBg.a = 0x3F;
	SetCanHaveFocus(true);
	// Limit event at 1:
	SetMouseLimit(1);
}

widget::Slider::~Slider(void)
{
	
}


bool widget::Slider::CalculateMinSize(void)
{
	m_minSize.x = etk_max(m_userMinSize.x, 40);
	m_minSize.y = etk_max(m_userMinSize.y, dotRadius*2);
	MarkToRedraw();
	return true;
}


void widget::Slider::SetValue(int32_t val)
{
	m_value = etk_max(etk_min(val, m_max), m_min);
	MarkToRedraw();
}


int32_t widget::Slider::GetValue(void)
{
	return m_value;
}


void widget::Slider::SetMin(int32_t val)
{
	m_min = val;
	m_value = etk_max(etk_min(m_value, m_max), m_min);
	MarkToRedraw();
}


void widget::Slider::SetMax(int32_t val)
{
	m_max = val;
	m_value = etk_max(etk_min(m_value, m_max), m_min);
	MarkToRedraw();
}


void widget::Slider::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		// clean the object list ...
		ClearOObjectList();
		
		ewol::Drawing * tmpDraw = new ewol::Drawing;
		
		tmpDraw->SetColor(m_textColorFg);
		// draw a line :
		tmpDraw->SetThickness(1);
		tmpDraw->SetPos(etk::Vector3D<float>(dotRadius, m_size.y/2) );
		tmpDraw->LineTo(etk::Vector3D<float>(m_size.x-dotRadius, m_size.y/2) );
		tmpDraw->SetThickness(0);
		
		draw::Color borderDot = m_textColorFg;
		borderDot.a /= 2;
		tmpDraw->SetPos(etk::Vector3D<float>(4+((float)(m_value-m_min)/(float)(m_max-m_min))*(float)(m_size.x-2*dotRadius), m_size.y/2) );
		tmpDraw->SetColorBg(borderDot);
		tmpDraw->Circle(dotRadius);
		tmpDraw->SetColorBg(m_textColorFg);
		tmpDraw->Circle(dotRadius/1.6);
		AddOObject(tmpDraw);
	}
}


bool widget::Slider::OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, etk::Vector2D<float> pos)
{
	etk::Vector2D<float> relativePos = RelativePosition(pos);
	//EWOL_DEBUG("Event on Slider ...");
	if (1 == IdInput) {
		if(    ewol::keyEvent::statusSingle == typeEvent
		    || ewol::keyEvent::statusMove   == typeEvent) {
			// get the new position :
			EWOL_VERBOSE("Event on Slider (" << relativePos.x << "," << relativePos.y << ")");
			int32_t oldValue = m_value;
			m_value = m_min + (float)(relativePos.x - dotRadius) / (float)(m_size.x-2*dotRadius) * (float)(m_max-m_min);
			m_value = etk_max(etk_min(m_value, m_max), m_min);
			if (oldValue != m_value) {
				EWOL_DEBUG(" new value : " << m_value << " in [" << m_min << ".." << m_max << "]");
				GenerateEventId(ewolEventSliderChange, m_value);
				MarkToRedraw();
			}
			return true;
		}
	}
	return false;
}


