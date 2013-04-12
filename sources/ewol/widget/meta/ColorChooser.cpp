/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/meta/ColorChooser.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/List.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/WidgetManager.h>
//#include <etk/Vector.h>
#include <etk/Vector.h>

extern "C" {
	// file browsing ...
	#include <dirent.h>
}

#undef __class__
#define __class__	"ColorChooser"




extern const char * const ewolEventColorChooserChange          = "ewol-event-color-chooser-change";


const char * const eventColorBarHasChange          = "event-color-bar-has-change";
const char * const eventColorSpecificHasChange     = "event-color-specific-has-change";


widget::ColorChooser::ColorChooser(void) :
	widget::Sizer(widget::Sizer::modeVert)
{
	AddEventId(ewolEventColorChooserChange);
	
	
	m_widgetColorBar = NULL;
	m_widgetRed = NULL;
	m_widgetGreen = NULL;
	m_widgetBlue = NULL;
	m_widgetAlpha = NULL;
	
	LockExpand(bvec2(true,true));
		m_widgetColorBar = new widget::ColorBar();
			m_widgetColorBar->RegisterOnEvent(this, ewolEventColorBarChange, eventColorBarHasChange);
			m_widgetColorBar->SetFill(bvec2(true,true));
			/*
			m_widgetColorBar->SetWidth(200);
			m_widgetColorBar->SetHeigh(200);
			*/
			SubWidgetAdd(m_widgetColorBar);
		
		draw::Color sliderColor;
		sliderColor = draw::color::black;
		
		m_widgetRed = new widget::Slider();
			m_widgetRed->RegisterOnEvent(this, ewolEventSliderChange, eventColorSpecificHasChange);
			m_widgetRed->SetExpand(bvec2(true,false));
			m_widgetRed->SetFill(bvec2(true,false));
			m_widgetRed->SetMin(0);
			m_widgetRed->SetMax(255);
			sliderColor = 0xFF0000FF;
			m_widgetRed->SetColor(sliderColor);
			SubWidgetAdd(m_widgetRed);
		m_widgetGreen = new widget::Slider();
			m_widgetGreen->RegisterOnEvent(this, ewolEventSliderChange, eventColorSpecificHasChange);
			m_widgetGreen->SetExpand(bvec2(true,false));
			m_widgetGreen->SetFill(bvec2(true,false));
			m_widgetGreen->SetMin(0);
			sliderColor = 0x00FF00FF;
			m_widgetGreen->SetColor(sliderColor);
			m_widgetGreen->SetMax(255);
			SubWidgetAdd(m_widgetGreen);
		m_widgetBlue = new widget::Slider();
			m_widgetBlue->RegisterOnEvent(this, ewolEventSliderChange, eventColorSpecificHasChange);
			m_widgetBlue->SetExpand(bvec2(true,false));
			m_widgetBlue->SetFill(bvec2(true,false));
			m_widgetBlue->SetMin(0);
			sliderColor = 0x0000FFFF;
			m_widgetBlue->SetColor(sliderColor);
			m_widgetBlue->SetMax(255);
			SubWidgetAdd(m_widgetBlue);
		m_widgetAlpha = new widget::Slider();
			m_widgetAlpha->RegisterOnEvent(this, ewolEventSliderChange, eventColorSpecificHasChange);
			m_widgetAlpha->SetExpand(bvec2(true,false));
			m_widgetAlpha->SetFill(bvec2(true,false));
			m_widgetAlpha->SetMin(0);
			m_widgetAlpha->SetMax(255);
			SubWidgetAdd(m_widgetAlpha);
	
	m_currentColor = draw::color::white;
}


widget::ColorChooser::~ColorChooser(void)
{
	
}


void widget::ColorChooser::SetColor(draw::Color newColor)
{
	m_currentColor = newColor;
	if (NULL != m_widgetRed) {
		m_widgetRed->SetValue(m_currentColor.r);
	}
	if (NULL != m_widgetGreen) {
		m_widgetGreen->SetValue(m_currentColor.g);
	}
	if (NULL != m_widgetBlue) {
		m_widgetBlue->SetValue(m_currentColor.b);
	}
	if (NULL != m_widgetAlpha) {
		m_widgetAlpha->SetValue(m_currentColor.a);
	}
	if (NULL != m_widgetColorBar) {
		m_widgetColorBar->SetCurrentColor(m_currentColor);
	}
}


draw::Color widget::ColorChooser::GetColor(void)
{
	return m_currentColor;
}


void widget::ColorChooser::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, const etk::UString& data)
{
	if (NULL == CallerObject) {
		return;
	}
	//EWOL_INFO("Receive Extern Event ... : widgetPointer=" << CallerObject << "\"" << eventId << "\" ==> data=\"" << data << "\"" );
	if (eventColorBarHasChange == eventId) {
		//==> colorBar has change ...
		uint8_t tmpAlpha = m_currentColor.a;
		// the colorbar has no notion of the alpha ==> keep it ...
		if (NULL != m_widgetColorBar) {
			m_currentColor = m_widgetColorBar->GetCurrentColor();
		}
		m_currentColor.a = tmpAlpha;
		if (NULL != m_widgetRed) {
			m_widgetRed->SetValue(m_currentColor.r);
		}
		if (NULL != m_widgetGreen) {
			m_widgetGreen->SetValue(m_currentColor.g);
		}
		if (NULL != m_widgetBlue) {
			m_widgetBlue->SetValue(m_currentColor.b);
		}
		if (NULL != m_widgetAlpha) {
			m_widgetAlpha->SetValue(m_currentColor.a);
		}
		GenerateEventId(ewolEventColorChooserChange, draw::GetString(m_currentColor));
	} else if (eventColorSpecificHasChange == eventId) {
		// Slider has changes his color ==> get the one change ...
		if (CallerObject == m_widgetRed) {
			m_currentColor.r = m_widgetRed->GetValue();
		}
		if (CallerObject == m_widgetGreen) {
			m_currentColor.g = m_widgetGreen->GetValue();
		}
		if (CallerObject == m_widgetBlue) {
			m_currentColor.b = m_widgetBlue->GetValue();
		}
		if (CallerObject == m_widgetAlpha) {
			m_currentColor.a = m_widgetAlpha->GetValue();
		}
		if (NULL != m_widgetColorBar) {
			m_widgetColorBar->SetCurrentColor(m_currentColor);
		}
		GenerateEventId(ewolEventColorChooserChange, draw::GetString(m_currentColor));
	}
};


void widget::ColorChooser::OnObjectRemove(ewol::EObject * removeObject)
{
	// First step call parrent : 
	widget::Sizer::OnObjectRemove(removeObject);
	// second step find if in all the elements ...
	if(removeObject == m_widgetRed) {
		m_widgetRed = NULL;
	}
	if(removeObject == m_widgetGreen) {
		m_widgetGreen = NULL;
	}
	if(removeObject == m_widgetBlue) {
		m_widgetBlue = NULL;
	}
	if(removeObject == m_widgetAlpha) {
		m_widgetAlpha = NULL;
	}
	if(removeObject == m_widgetColorBar) {
		m_widgetColorBar = NULL;
	}
}

