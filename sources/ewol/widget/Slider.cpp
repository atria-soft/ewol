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


static ewol::Widget* Create(void)
{
	return new widget::Slider();
}

void widget::Slider::Init(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,&Create);
}

void widget::Slider::UnInit(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,NULL);
}



const int32_t dotRadius = 6;

widget::Slider::Slider(void)
{
	AddEventId(ewolEventSliderChange);
	
	m_value = 0;
	m_min = 0;
	m_max = 10;
	
	m_textColorFg = etk::color::black;
	
	m_textColorBg = etk::color::black;
	m_textColorBg.SetA(0x3F);
	SetCanHaveFocus(true);
	// Limit event at 1:
	SetMouseLimit(1);
}

widget::Slider::~Slider(void)
{
	
}


void widget::Slider::CalculateMinMaxSize(void)
{
	vec2 minTmp = m_userMinSize.GetPixel();
	m_minSize.setValue(etk_max(minTmp.x(), 40),
	                   etk_max(minTmp.y(), dotRadius*2) );
	MarkToRedraw();
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
		tmpDraw->SetPos(vec3(dotRadius, m_size.y()/2, 0) );
		tmpDraw->LineTo(vec3(m_size.x()-dotRadius, m_size.y()/2, 0) );
		tmpDraw->SetThickness(0);
		
		etk::Color<> borderDot = m_textColorFg;
		borderDot.SetA(borderDot.a()/2);
		tmpDraw->SetPos(vec3(4+((float)(m_value-m_min)/(float)(m_max-m_min))*(float)(m_size.x()-2*dotRadius), m_size.y()/2, 0) );
		tmpDraw->SetColorBg(borderDot);
		tmpDraw->Circle(dotRadius);
		tmpDraw->SetColorBg(m_textColorFg);
		tmpDraw->Circle(dotRadius/1.6);
		AddOObject(tmpDraw);
	}
}


bool widget::Slider::OnEventInput(const ewol::EventInput& _event)
{
	vec2 relativePos = RelativePosition(_event.GetPos());
	//EWOL_DEBUG("Event on Slider ...");
	if (1 == _event.GetId()) {
		if(    ewol::keyEvent::statusSingle == _event.GetStatus()
		    || ewol::keyEvent::statusMove   == _event.GetStatus()) {
			// get the new position :
			EWOL_VERBOSE("Event on Slider (" << relativePos.x() << "," << relativePos.y() << ")");
			int32_t oldValue = m_value;
			m_value = m_min + (float)(relativePos.x() - dotRadius) / (float)(m_size.x()-2*dotRadius) * (float)(m_max-m_min);
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


