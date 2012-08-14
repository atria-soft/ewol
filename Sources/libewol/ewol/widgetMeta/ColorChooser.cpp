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
		
		etk::Color sliderColor;
		sliderColor = etk::color::black;
		
		m_widgetRed = new ewol::Slider();
			m_widgetRed->RegisterOnEvent(this, ewolEventSliderChange, eventColorSpecificHasChange);
			m_widgetRed->SetExpendX(true);
			m_widgetRed->SetFillX(true);
			m_widgetRed->SetMin(0);
			m_widgetRed->SetMax(255);
			sliderColor = 0xFF0000FF;
			m_widgetRed->SetColor(sliderColor);
			SubWidgetAdd(m_widgetRed);
		m_widgetGreen = new ewol::Slider();
			m_widgetGreen->RegisterOnEvent(this, ewolEventSliderChange, eventColorSpecificHasChange);
			m_widgetGreen->SetExpendX(true);
			m_widgetGreen->SetFillX(true);
			m_widgetGreen->SetMin(0);
			sliderColor = 0x00FF00FF;
			m_widgetGreen->SetColor(sliderColor);
			m_widgetGreen->SetMax(255);
			SubWidgetAdd(m_widgetGreen);
		m_widgetBlue = new ewol::Slider();
			m_widgetBlue->RegisterOnEvent(this, ewolEventSliderChange, eventColorSpecificHasChange);
			m_widgetBlue->SetExpendX(true);
			m_widgetBlue->SetFillX(true);
			m_widgetBlue->SetMin(0);
			sliderColor = 0x0000FFFF;
			m_widgetBlue->SetColor(sliderColor);
			m_widgetBlue->SetMax(255);
			SubWidgetAdd(m_widgetBlue);
		m_widgetAlpha = new ewol::Slider();
			m_widgetAlpha->RegisterOnEvent(this, ewolEventSliderChange, eventColorSpecificHasChange);
			m_widgetAlpha->SetExpendX(true);
			m_widgetAlpha->SetFillX(true);
			m_widgetAlpha->SetMin(0);
			m_widgetAlpha->SetMax(255);
			SubWidgetAdd(m_widgetAlpha);
	
	m_currentColor = etk::color::white;
}


ewol::ColorChooser::~ColorChooser(void)
{
	
}


void ewol::ColorChooser::SetColor(etk::Color newColor)
{
	m_currentColor = newColor;
	if (NULL != m_widgetRed) {
		m_widgetRed->SetValue(m_currentColor.red);
	}
	if (NULL != m_widgetGreen) {
		m_widgetGreen->SetValue(m_currentColor.green);
	}
	if (NULL != m_widgetBlue) {
		m_widgetBlue->SetValue(m_currentColor.blue);
	}
	if (NULL != m_widgetAlpha) {
		m_widgetAlpha->SetValue(m_currentColor.alpha);
	}
	if (NULL != m_widgetColorBar) {
		m_widgetColorBar->SetCurrentColor(m_currentColor);
	}
}


etk::Color ewol::ColorChooser::GetColor(void)
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
	if (NULL == CallerObject) {
		return;
	}
	//EWOL_INFO("Receive Extern Event ... : widgetPointer=" << CallerObject << "\"" << eventId << "\" ==> data=\"" << data << "\"" );
	if (eventColorBarHasChange == eventId) {
		//==> colorBar has change ...
		uint8_t tmpAlpha = m_currentColor.alpha;
		// the colorbar has no notion of the alpha ==> keep it ...
		if (NULL != m_widgetColorBar) {
			m_currentColor = m_widgetColorBar->GetCurrentColor();
		}
		m_currentColor.alpha = tmpAlpha;
		if (NULL != m_widgetRed) {
			m_widgetRed->SetValue(m_currentColor.red);
		}
		if (NULL != m_widgetGreen) {
			m_widgetGreen->SetValue(m_currentColor.green);
		}
		if (NULL != m_widgetBlue) {
			m_widgetBlue->SetValue(m_currentColor.blue);
		}
		if (NULL != m_widgetAlpha) {
			m_widgetAlpha->SetValue(m_currentColor.alpha);
		}
		// TODO : send the real color ...
		GenerateEventId(ewolEventColorChooserChange, "0x51452563");
	} else if (eventColorSpecificHasChange == eventId) {
		// Slider has changes his color ==> get the one change ...
		if (CallerObject == m_widgetRed) {
			m_currentColor.red   = m_widgetRed->GetValue();
		}
		if (CallerObject == m_widgetGreen) {
			m_currentColor.green = m_widgetGreen->GetValue();
		}
		if (CallerObject == m_widgetBlue) {
			m_currentColor.blue  = m_widgetBlue->GetValue();
		}
		if (CallerObject == m_widgetAlpha) {
			m_currentColor.alpha = m_widgetAlpha->GetValue();
		}
		if (NULL != m_widgetColorBar) {
			m_widgetColorBar->SetCurrentColor(m_currentColor);
		}
		// TODO : send the real color ...
		GenerateEventId(ewolEventColorChooserChange, "0x51452563");
	}
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

