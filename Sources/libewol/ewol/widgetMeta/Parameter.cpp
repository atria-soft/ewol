/**
 *******************************************************************************
 * @file ewol/widgetMeta/FileChooser.cpp
 * @brief ewol File chooser meta widget system (Sources)
 * @author Edouard DUPIN
 * @date 29/12/2011
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

#include <ewol/widgetMeta/Parameter.h>
#include <ewol/widget/SizerHori.h>
#include <ewol/widget/SizerVert.h>
#include <ewol/widget/List.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Image.h>
#include <ewol/WidgetManager.h>
#include <etk/VectorType.h>


#include <ewol/ewol.h>

#undef __class__
#define __class__	"Parameter"


extern const char * const ewolEventParameterValidate            = "ewol-event-parameter-validate";
extern const char * const ewolEventParameterClose               = "ewol-event-parameter-close";

static const char * const l_eventMenuSelected = "local-event-menu-selected";

ewol::Parameter::Parameter(void) : 
	m_currentIdList(0),
	m_widgetTitle(NULL),
	m_widgetCancel(NULL),
	m_paramList(NULL)
{
	AddEventId(ewolEventParameterClose);
	
	ewol::SizerVert * mySizerVert = NULL;
	ewol::SizerHori * mySizerHori = NULL;
	ewol::Spacer * mySpacer = NULL;
	#ifdef __PLATFORM__Android
		SetDisplayRatio(0.90);
	#else
		SetDisplayRatio(0.80);
	#endif
	
	mySizerVert = new ewol::SizerVert();
	if (NULL == mySizerVert) {
		EWOL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		EWOL_INFO("add widget");
		mySizerVert->LockExpendContamination(true);
		// set it in the pop-up-system :
		SubWidgetSet(mySizerVert);
		
		m_widgetTitle = new ewol::Label("File chooser ...");
		if (NULL == m_widgetTitle) {
			EWOL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			mySizerVert->SubWidgetAdd(m_widgetTitle);
		}
		
		mySpacer = new ewol::Spacer();
		if (NULL == mySpacer) {
			EWOL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			//mySpacer->SetExpendX(true);
			//mySpacer->SetSize(10);
			mySizerVert->SubWidgetAdd(mySpacer);
		}
		
		mySizerHori = new ewol::SizerHori();
		if (NULL == mySizerHori) {
			EWOL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			mySizerVert->SubWidgetAdd(mySizerHori);
			
			m_paramList = new ewol::ParameterList();
			if (NULL == m_paramList) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
			
				m_paramList->RegisterOnEvent(this, ewolEventParameterListSelect, l_eventMenuSelected);
				m_paramList->SetFillY(true);
				m_paramList->SetExpendY(true);
				mySizerHori->SubWidgetAdd(m_paramList);
			}
			
			m_wSlider = new ewol::WSlider();
			if (NULL == m_wSlider) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				m_wSlider->SetExpendX(true);
				m_wSlider->SetExpendY(true);
				mySizerHori->SubWidgetAdd(m_wSlider);
			}
		}
		
		mySizerHori = new ewol::SizerHori();
		if (NULL == mySizerHori) {
			EWOL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			mySizerVert->SubWidgetAdd(mySizerHori);
			
			mySpacer = new ewol::Spacer();
			if (NULL == mySpacer) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				mySpacer->SetExpendX(true);
				mySizerHori->SubWidgetAdd(mySpacer);
			}
			
			m_widgetCancel = new ewol::Button("Close");
			if (NULL == m_widgetCancel) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				m_widgetCancel->SetImage("icon/Remove.svg");
				m_widgetCancel->RegisterOnEvent(this, ewolEventButtonPressed, ewolEventParameterClose);
				mySizerHori->SubWidgetAdd(m_widgetCancel);
			}
		}
	}
	MarkToReedraw();
	//m_needFlipFlop = true;

}


ewol::Parameter::~Parameter(void)
{
	
}


//!< EObject name :
extern const char * const ewol::TYPE_EOBJECT_WIDGET_PARAMETER = "Parameter";

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool ewol::Parameter::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_PARAMETER << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == ewol::TYPE_EOBJECT_WIDGET_PARAMETER) {
		return true;
	} else {
		if(true == ewol::PopUp::CheckObjectType(objectType)) {
			return true;
		}
		EWOL_ERROR("check error : \"" << ewol::TYPE_EOBJECT_WIDGET_PARAMETER << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const ewol::Parameter::GetObjectType(void)
{
	return ewol::TYPE_EOBJECT_WIDGET_PARAMETER;
}



void ewol::Parameter::SetTitle(etk::UString label)
{
	if (NULL == m_widgetTitle) {
		return;
	}
	m_widgetTitle->SetLabel(label);
}


/**
 * @brief Receive a message from an other EObject with a specific eventId and data
 * @param[in] CallerObject Pointer on the EObject that information came from
 * @param[in] eventId Message registered by this class
 * @param[in] data Data registered by this class
 * @return ---
 */
void ewol::Parameter::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
{
	ewol::PopUp::OnReceiveMessage(CallerObject, eventId, data);
	EWOL_DEBUG("event on the parameter : " << eventId << " data=\"" << data << "\"");
	if (eventId == ewolEventParameterClose) {
		// inform that the parameter windows is closed
		GenerateEventId(ewolEventParameterClose);
		// Close this widget ...
		MarkToRemove();
	} else if (eventId == l_eventMenuSelected) {
		if (NULL != m_wSlider) {
			
			m_wSlider->SubWidgetSelectSet(1);
		}
	}
	
	return;
};


/**
 * @brief Inform object that an other object is removed ...
 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
 * @note : Sub classes must call this class
 * @return ---
 */
void ewol::Parameter::OnObjectRemove(ewol::EObject * removeObject)
{
	// First step call parrent : 
	ewol::PopUp::OnObjectRemove(removeObject);
	// second step find if in all the elements ...
	if(removeObject == m_widgetTitle) {
		m_widgetTitle = NULL;
		m_needFlipFlop = true;
	}
	if(removeObject == m_paramList) {
		m_paramList = NULL;
		m_needFlipFlop = true;
	}
	if(removeObject == m_widgetCancel) {
		m_widgetCancel = NULL;
		m_needFlipFlop = true;
	}
	if(removeObject == m_wSlider) {
		m_wSlider = NULL;
		m_needFlipFlop = true;
	}
}


void ewol::Parameter::MenuAdd(etk::UString label, etk::UString image, ewol::Widget* associateWidget)
{
	if (NULL != m_paramList) {
		m_paramList->MenuAdd(label, m_currentIdList, image);
		if (NULL != m_wSlider) {
			if (NULL != associateWidget) {
				m_wSlider->SubWidgetAdd(associateWidget);
			} else {
				ewol::Label * myLabel = new ewol::Label((etk::UString("No widget set ... ") + m_currentIdList));
				if (NULL == myLabel) {
					EWOL_ERROR("Can not allocate widget ==> display might be in error");
				} else {
					m_wSlider->SubWidgetAdd(myLabel);
				}
			}
			if (m_currentIdList == 0) {
				m_wSlider->SubWidgetSelectSet(0);
			}
		}
		m_currentIdList++;
	}
}

void ewol::Parameter::MenuClear(void)
{
	if (NULL != m_paramList) {
		m_paramList->MenuClear();
		m_currentIdList = 0;
	}
}

void ewol::Parameter::MenuSeparator(void)
{
	if (NULL != m_paramList) {
		m_paramList->MenuSeparator();
	}
}

