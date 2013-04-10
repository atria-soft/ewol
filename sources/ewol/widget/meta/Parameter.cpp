/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/meta/Parameter.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/List.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Image.h>
#include <ewol/widget/WidgetManager.h>
#include <etk/Vector.h>


#include <ewol/ewol.h>

#undef __class__
#define __class__	"Parameter"


extern const char * const ewolEventParameterValidate            = "ewol-event-parameter-validate";
extern const char * const ewolEventParameterClose               = "ewol-event-parameter-close";

static const char * const l_eventMenuSelected = "local-event-menu-selected";

widget::Parameter::Parameter(void) : 
	m_currentIdList(0),
	m_widgetTitle(NULL),
	m_widgetCancel(NULL),
	m_paramList(NULL)
{
	AddEventId(ewolEventParameterClose);
	
	widget::Sizer * mySizerVert = NULL;
	widget::Sizer * mySizerHori = NULL;
	widget::Spacer * mySpacer = NULL;
	#ifdef __TARGET_OS__Android
		SetDisplayRatio(0.90);
	#else
		SetDisplayRatio(0.80);
	#endif
	
	mySizerVert = new widget::Sizer(widget::Sizer::modeVert);
	if (NULL == mySizerVert) {
		EWOL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		EWOL_INFO("add widget");
		mySizerVert->LockExpendContamination(bvec2(true,true));
		// set it in the pop-up-system :
		SubWidgetSet(mySizerVert);
		
		mySizerHori = new widget::Sizer(widget::Sizer::modeHori);
		if (NULL == mySizerHori) {
			EWOL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			mySizerVert->SubWidgetAdd(mySizerHori);
			
			mySpacer = new widget::Spacer();
			if (NULL == mySpacer) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				mySpacer->SetExpand(bvec2(true,false));
				mySizerHori->SubWidgetAdd(mySpacer);
			}
			
			// TODO : set if back :
			/*
			m_widgetCancel = new widget::Button("Close");
			if (NULL == m_widgetCancel) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				m_widgetCancel->SetImage("icon/Remove.svg");
				m_widgetCancel->RegisterOnEvent(this, ewolEventButtonPressed, ewolEventParameterClose);
				mySizerHori->SubWidgetAdd(m_widgetCancel);
			}
			*/
		}
		
		mySizerHori = new widget::Sizer(widget::Sizer::modeHori);
		if (NULL == mySizerHori) {
			EWOL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			mySizerVert->SubWidgetAdd(mySizerHori);
			
			m_paramList = new widget::ParameterList();
			if (NULL == m_paramList) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
			
				m_paramList->RegisterOnEvent(this, ewolEventParameterListSelect, l_eventMenuSelected);
				m_paramList->SetFill(bvec2(false,true));
				m_paramList->SetExpand(bvec2(false,true));
				mySizerHori->SubWidgetAdd(m_paramList);
			}
			mySpacer = new widget::Spacer();
			if (NULL == mySpacer) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				mySpacer->SetFill(bvec2(false,true));
				mySpacer->SetSize(5);
				mySpacer->SetColor(0x000000BF);
				mySizerHori->SubWidgetAdd(mySpacer);
			}
			
			widget::Sizer * mySizerVert2 = new widget::Sizer(widget::Sizer::modeVert);
			if (NULL == mySizerVert2) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				mySizerHori->SubWidgetAdd(mySizerVert2);
				
				mySpacer = new widget::Spacer();
				if (NULL == mySpacer) {
					EWOL_ERROR("Can not allocate widget ==> display might be in error");
				} else {
					mySpacer->SetExpand(bvec2(true,false));
					mySpacer->SetSize(5);
					mySpacer->SetColor(0x000000BF);
					mySizerVert2->SubWidgetAdd(mySpacer);
				}
				
				m_wSlider = new widget::WSlider();
				if (NULL == m_wSlider) {
					EWOL_ERROR("Can not allocate widget ==> display might be in error");
				} else {
					m_wSlider->SetExpand(bvec2(true,true));
					mySizerVert2->SubWidgetAdd(m_wSlider);
				}
			}
		}
		
		mySpacer = new widget::Spacer();
		if (NULL == mySpacer) {
			EWOL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			mySpacer->SetExpand(bvec2(true,false));
			mySpacer->SetSize(5);
			mySpacer->SetColor(0x000000BF);
			mySizerVert->SubWidgetAdd(mySpacer);
		}
		
		m_widgetTitle = new widget::Label("File chooser ...");
		if (NULL == m_widgetTitle) {
			EWOL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			m_widgetTitle->SetExpand(bvec2(true,false));
			mySizerVert->SubWidgetAdd(m_widgetTitle);
		}
	}
	MarkToRedraw();

}


widget::Parameter::~Parameter(void)
{
	
}


void widget::Parameter::SetTitle(etk::UString label)
{
	if (NULL == m_widgetTitle) {
		return;
	}
	m_widgetTitle->SetLabel(label);
}


void widget::Parameter::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, const etk::UString& data)
{
	widget::PopUp::OnReceiveMessage(CallerObject, eventId, data);
	EWOL_DEBUG("event on the parameter : " << eventId << " data=\"" << data << "\"");
	if (eventId == ewolEventParameterClose) {
		// inform that the parameter windows is closed
		GenerateEventId(ewolEventParameterClose);
		// Close this widget ...
		AutoDestroy();
	} else if (eventId == l_eventMenuSelected) {
		if (NULL != m_wSlider) {
			int32_t value = 0;
			sscanf(data.c_str(), "%d", &value);
			EWOL_DEBUG("event on the parameter : " << eventId << " select ID=" << value << "");
			m_wSlider->SubWidgetSelectSet(value);
		}
	}
	
	return;
};


void widget::Parameter::OnObjectRemove(ewol::EObject * removeObject)
{
	// First step call parrent : 
	widget::PopUp::OnObjectRemove(removeObject);
	// second step find if in all the elements ...
	if(removeObject == m_widgetTitle) {
		m_widgetTitle = NULL;
	}
	if(removeObject == m_paramList) {
		m_paramList = NULL;
	}
	if(removeObject == m_widgetCancel) {
		m_widgetCancel = NULL;
	}
	if(removeObject == m_wSlider) {
		m_wSlider = NULL;
	}
}


void widget::Parameter::MenuAdd(etk::UString label, etk::UString image, ewol::Widget* associateWidget)
{
	if (NULL != m_paramList) {
		m_paramList->MenuAdd(label, m_currentIdList, image);
		if (NULL != m_wSlider) {
			if (NULL != associateWidget) {
				m_wSlider->SubWidgetAdd(associateWidget);
			} else {
				EWOL_DEBUG("Associate an empty widget on it ...");
				widget::Label * myLabel = new widget::Label((etk::UString("No widget for : ") + label));
				if (NULL == myLabel) {
					EWOL_ERROR("Can not allocate widget ==> display might be in error");
				} else {
					myLabel->SetExpand(bvec2(true,true));
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
void widget::Parameter::MenuAddGroup(etk::UString label)
{
	if (NULL != m_paramList) {
		m_paramList->MenuSeparator();
		m_paramList->MenuAddGroup(label);
	}
}

void widget::Parameter::MenuClear(void)
{
	if (NULL != m_paramList) {
		m_paramList->MenuClear();
		m_currentIdList = 0;
	}
}

void widget::Parameter::MenuSeparator(void)
{
	if (NULL != m_paramList) {
		m_paramList->MenuSeparator();
	}
}

