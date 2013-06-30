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
#include <ewol/widget/Composer.h>
#include <ewol/UserConfig.h>
#include <etk/Vector.h>


#include <ewol/ewol.h>

#undef __class__
#define __class__	"Parameter"


extern const char * const ewolEventParameterValidate            = "ewol-event-parameter-validate";
extern const char * const ewolEventParameterClose               = "ewol-event-parameter-close";
extern const char * const ewolEventParameterSave                = "ewol-event-parameter-save";

static const char * const l_eventMenuSelected = "local-event-menu-selected";

widget::Parameter::Parameter(void) : 
	m_currentIdList(0),
	m_widgetTitle(NULL),
	m_paramList(NULL)
{
	AddEventId(ewolEventParameterClose);
	
	widget::Sizer * mySizerVert = NULL;
	widget::Sizer * mySizerHori = NULL;
	widget::Spacer * mySpacer = NULL;
	#ifdef __TARGET_OS__Android
		SetMinSize(ewol::Dimension(vec2(90,90),ewol::Dimension::Pourcent));
	#else
		SetMinSize(ewol::Dimension(vec2(80,80),ewol::Dimension::Pourcent));
	#endif
	
	mySizerVert = new widget::Sizer(widget::Sizer::modeVert);
	if (NULL == mySizerVert) {
		EWOL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		EWOL_INFO("add widget");
		mySizerVert->LockExpand(bvec2(true,true));
		mySizerVert->SetExpand(bvec2(true,true));
		// set it in the pop-up-system :
		SetSubWidget(mySizerVert);
		
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
			
			widget::Button* tmpButton = new widget::Button();
			if (NULL == tmpButton) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				tmpButton->SetSubWidget(
				    new widget::Composer(widget::Composer::String,
				        "<composer>\n"
				        "	<sizer mode=\"hori\">\n"
				        "		<image src=\"THEME:GUI:Save.svg\" expand=\"true\" size=\"8,8mm\"/>\n"
				        "		<label>Save</label>\n"
				        "	</sizer>\n"
				        "</composer>\n"));
				tmpButton->RegisterOnEvent(this, widget::Button::eventPressed, ewolEventParameterSave);
				mySizerHori->SubWidgetAdd(tmpButton);
			}
			
			mySpacer = new widget::Spacer();
			if (NULL == mySpacer) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				mySpacer->SetExpand(bvec2(false,false));
				mySpacer->SetMinSize(ewol::Dimension(vec2(10,0)));
				mySizerHori->SubWidgetAdd(mySpacer);
			}
			
			tmpButton = new widget::Button();
			if (NULL == tmpButton) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				tmpButton->SetSubWidget(
				    new widget::Composer(widget::Composer::String,
				        "<composer>\n"
				        "	<sizer mode=\"hori\">\n"
				        "		<image src=\"THEME:GUI:Remove.svg\" expand=\"true\" size=\"8,8mm\"/>\n"
				        "		<label>Close</label>\n"
				        "	</sizer>\n"
				        "</composer>\n"));
				tmpButton->RegisterOnEvent(this, widget::Button::eventPressed, ewolEventParameterClose);
				mySizerHori->SubWidgetAdd(tmpButton);
			}
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
				mySpacer->SetMinSize(vec2(5,5));
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
					mySpacer->SetMinSize(vec2(5,5));
					mySpacer->SetColor(0x000000BF);
					mySizerVert2->SubWidgetAdd(mySpacer);
				}
				
				m_wSlider = new widget::WSlider();
				if (NULL == m_wSlider) {
					EWOL_ERROR("Can not allocate widget ==> display might be in error");
				} else {
					m_wSlider->SetTransitionSpeed(0.5);
					m_wSlider->SetTransitionMode(widget::WSlider::sladingTransitionVert);
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
			mySpacer->SetMinSize(vec2(5,5));
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


void widget::Parameter::OnReceiveMessage(const ewol::EMessage& _msg)
{
	widget::PopUp::OnReceiveMessage(_msg);
	EWOL_DEBUG("event on the parameter : " << _msg);
	if (_msg.GetMessage() == ewolEventParameterClose) {
		// inform that the parameter windows is closed
		GenerateEventId(ewolEventParameterClose);
		// Close this widget ...
		AutoDestroy();
	} else if (_msg.GetMessage() == ewolEventParameterSave) {
		ewol::userConfig::Save();
	} else if (_msg.GetMessage() == l_eventMenuSelected) {
		if (NULL != m_wSlider) {
			int32_t value = 0;
			sscanf(_msg.GetData().c_str(), "%d", &value);
			EWOL_DEBUG("event on the parameter : " << _msg.GetMessage() << " select ID=" << value << "");
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

