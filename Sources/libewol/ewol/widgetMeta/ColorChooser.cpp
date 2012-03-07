/**
 *******************************************************************************
 * @file ewol/widgetMeta/ColorChooser.cpp
 * @brief ewol Color chooser meta widget system (Sources)
 * @author Edouard DUPIN
 * @date 05/03/2012
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

#include <ewol/widgetMeta/ColorChooser.h>
#include <ewol/widget/SizerHori.h>
#include <ewol/widget/SizerVert.h>
#include <ewol/widget/List.h>
#include <ewol/widget/Spacer.h>
#include <ewol/WidgetManager.h>
//#include <etk/Vector.h>
#include <etk/VectorType.h>

extern "C" {
	// file browsing ...
	#include <dirent.h>
}

#undef __class__
#define __class__	"ColorChooser"




extern const char * const ewolEventColorChooserChange          = "ewol-event-color-chooser-change";


const char * const eventColorBarHasChange          = "event-color-bar-has-change";
const char * const eventColorSpecificHasChange     = "event-color-specific-has-change";


ewol::ColorChooser::ColorChooser(void)
{
	AddEventId(ewolEventColorChooserChange);
	
	
	m_widgetColorBar = NULL;
	m_widgetRed = NULL;
	m_widgetGreen = NULL;
	m_widgetBlue = NULL;
	m_widgetAlpha = NULL;
	
	LockExpendContamination(true);
		m_widgetColorBar = new ewol::ColorBar();
			m_widgetColorBar->RegisterOnEvent(this, ewolEventColorBarChange, eventColorBarHasChange);
			m_widgetColorBar->SetFillY(true);
			m_widgetColorBar->SetFillX(true);
			/*
			m_widgetColorBar->SetWidth(200);
			m_widgetColorBar->SetHeigh(200);
			*/
			SubWidgetAdd(m_widgetColorBar);
		
		color_ts sliderColor;
		sliderColor.red = 0.0;
		sliderColor.green = 0.0;
		sliderColor.blue = 0.0;
		sliderColor.alpha = 1.0;
		
		m_widgetRed = new ewol::Slider();
			m_widgetRed->RegisterOnEvent(this, ewolEventSliderChange, eventColorSpecificHasChange);
			m_widgetRed->SetExpendX(true);
			m_widgetRed->SetFillX(true);
			m_widgetRed->SetMin(0);
			m_widgetRed->SetMax(255);
			sliderColor.red = 1.0;
			m_widgetRed->SetColor(sliderColor);
			sliderColor.red = 0.0;
			SubWidgetAdd(m_widgetRed);
		m_widgetGreen = new ewol::Slider();
			m_widgetGreen->RegisterOnEvent(this, ewolEventSliderChange, eventColorSpecificHasChange);
			m_widgetGreen->SetExpendX(true);
			m_widgetGreen->SetFillX(true);
			m_widgetGreen->SetMin(0);
			sliderColor.green = 1.0;
			m_widgetGreen->SetColor(sliderColor);
			sliderColor.green = 0.0;
			m_widgetGreen->SetMax(255);
			SubWidgetAdd(m_widgetGreen);
		m_widgetBlue = new ewol::Slider();
			m_widgetBlue->RegisterOnEvent(this, ewolEventSliderChange, eventColorSpecificHasChange);
			m_widgetBlue->SetExpendX(true);
			m_widgetBlue->SetFillX(true);
			m_widgetBlue->SetMin(0);
			sliderColor.blue = 1.0;
			m_widgetBlue->SetColor(sliderColor);
			sliderColor.blue = 0.0;
			m_widgetBlue->SetMax(255);
			SubWidgetAdd(m_widgetBlue);
		m_widgetAlpha = new ewol::Slider();
			m_widgetAlpha->RegisterOnEvent(this, ewolEventSliderChange, eventColorSpecificHasChange);
			m_widgetAlpha->SetExpendX(true);
			m_widgetAlpha->SetFillX(true);
			m_widgetAlpha->SetMin(0);
			m_widgetAlpha->SetMax(255);
			SubWidgetAdd(m_widgetAlpha);
	
	m_currentColor.red   = 1.0;
	m_currentColor.green = 1.0;
	m_currentColor.blue  = 1.0;
	m_currentColor.alpha = 1.0;
}


ewol::ColorChooser::~ColorChooser(void)
{
	
}

void ewol::ColorChooser::SetColor(color_ts newColor)
{
	m_currentColor = newColor;
	if (NULL != m_widgetRed) {
		m_widgetRed->SetValue(m_currentColor.red * 255.);
	}
	if (NULL != m_widgetGreen) {
		m_widgetGreen->SetValue(m_currentColor.green * 255.);
	}
	if (NULL != m_widgetBlue) {
		m_widgetBlue->SetValue(m_currentColor.blue * 255.);
	}
	if (NULL != m_widgetAlpha) {
		m_widgetAlpha->SetValue(m_currentColor.alpha * 255.);
	}
	if (NULL != m_widgetColorBar) {
		m_widgetColorBar->SetCurrentColor(m_currentColor);
	}
}


color_ts ewol::ColorChooser::GetColor(void)
{
	return m_currentColor;
}


/**
 * @brief Receive a message from an other EObject with a specific eventId and data
 * @param[in] CallerObject Pointer on the EObject that information came from
 * @param[in] eventId Message registered by this class
 * @param[in] data Data registered by this class
 * @return ---
 */
void ewol::ColorChooser::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
{
	//EWOL_INFO("Receive Extern Event ... : widgetPointer=" << CallerObject << "\"" << eventId << "\" ==> data=\"" << data << "\"" );
	if (eventColorBarHasChange == eventId) {
		//==> colorBar has change ...
		
		if (NULL != m_widgetColorBar) {
			m_currentColor = m_widgetColorBar->GetCurrentColor();
		}
		if (NULL != m_widgetRed) {
			m_widgetRed->SetValue(m_currentColor.red * 255.);
		}
		if (NULL != m_widgetGreen) {
			m_widgetGreen->SetValue(m_currentColor.green * 255.);
		}
		if (NULL != m_widgetBlue) {
			m_widgetBlue->SetValue(m_currentColor.blue * 255.);
		}
		if (NULL != m_widgetAlpha) {
			m_widgetAlpha->SetValue(m_currentColor.alpha * 255.);
		}
		GenerateEventId(ewolEventColorChooserChange);
		return;
	} else if (eventColorSpecificHasChange == eventId) {
		//==> Entry has change ...
		return;
	}
	return;
};


/**
 * @brief Inform object that an other object is removed ...
 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
 * @note : Sub classes must call this class
 * @return ---
 */
void ewol::ColorChooser::OnObjectRemove(ewol::EObject * removeObject)
{
	// First step call parrent : 
	ewol::SizerVert::OnObjectRemove(removeObject);
	// second step find if in all the elements ...
	if(removeObject == m_widgetRed) {
		m_widgetRed = NULL;
		m_needFlipFlop = true;
	}
	if(removeObject == m_widgetGreen) {
		m_widgetGreen = NULL;
		m_needFlipFlop = true;
	}
	if(removeObject == m_widgetBlue) {
		m_widgetBlue = NULL;
		m_needFlipFlop = true;
	}
	if(removeObject == m_widgetAlpha) {
		m_widgetAlpha = NULL;
		m_needFlipFlop = true;
	}
	if(removeObject == m_widgetColorBar) {
		m_widgetColorBar = NULL;
		m_needFlipFlop = true;
	}
}

