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
#include <ewol/widget/Manager.h>
#include <ewol/widget/Composer.h>
#include <vector>


#include <ewol/ewol.h>

#undef __class__
#define __class__ "Parameter"


extern const char * const ewolEventParameterValidate            = "ewol-event-parameter-validate";
extern const char * const ewolEventParameterClose               = "ewol-event-parameter-close";
extern const char * const ewolEventParameterSave                = "ewol-event-parameter-save";

static const char * const l_eventMenuSelected = "local-event-menu-selected";

ewol::widget::Parameter::Parameter(void) :
  m_currentIdList(0),
  m_widgetTitle(NULL),
  m_paramList(NULL) {
	addObjectType("ewol::widget::Parameter");
	addEventId(ewolEventParameterClose);
	
	ewol::widget::Sizer * mySizerVert = NULL;
	ewol::widget::Sizer * mySizerHori = NULL;
	ewol::widget::Spacer * mySpacer = NULL;
	#ifdef __TARGET_OS__Android
		setMinSize(ewol::Dimension(vec2(90, 90), ewol::Dimension::Pourcent));
	#else
		setMinSize(ewol::Dimension(vec2(80, 80), ewol::Dimension::Pourcent));
	#endif
	
	mySizerVert = new ewol::widget::Sizer(widget::Sizer::modeVert);
	if (NULL == mySizerVert) {
		EWOL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		EWOL_INFO("add widget");
		mySizerVert->lockExpand(bvec2(true,true));
		mySizerVert->setExpand(bvec2(true,true));
		// set it in the pop-up-system :
		setSubWidget(mySizerVert);
		
		mySizerHori = new ewol::widget::Sizer(widget::Sizer::modeHori);
		if (NULL == mySizerHori) {
			EWOL_ERROR("Can not allocate widget  == > display might be in error");
		} else {
			mySizerVert->subWidgetAdd(mySizerHori);
			
			mySpacer = new ewol::widget::Spacer();
			if (NULL == mySpacer) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				mySpacer->setExpand(bvec2(true,false));
				mySizerHori->subWidgetAdd(mySpacer);
			}
			
			ewol::widget::Button* tmpButton = new widget::Button();
			if (NULL == tmpButton) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				tmpButton->setSubWidget(
				    new ewol::widget::Composer(widget::Composer::String,
				        "<composer>\n"
				        "	<sizer mode=\"hori\">\n"
				        "		<image src=\"THEME:GUI:Save.svg\" expand=\"true\" size=\"8,8mm\"/>\n"
				        "		<label>Save</label>\n"
				        "	</sizer>\n"
				        "</composer>\n"));
				tmpButton->registerOnEvent(this, ewol::widget::Button::eventPressed, ewolEventParameterSave);
				mySizerHori->subWidgetAdd(tmpButton);
			}
			
			mySpacer = new ewol::widget::Spacer();
			if (NULL == mySpacer) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				mySpacer->setExpand(bvec2(false,false));
				mySpacer->setMinSize(ewol::Dimension(vec2(10,0)));
				mySizerHori->subWidgetAdd(mySpacer);
			}
			
			tmpButton = new ewol::widget::Button();
			if (NULL == tmpButton) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				tmpButton->setSubWidget(
				    new ewol::widget::Composer(widget::Composer::String,
				        "<composer>\n"
				        "	<sizer mode=\"hori\">\n"
				        "		<image src=\"THEME:GUI:Remove.svg\" expand=\"true\" size=\"8,8mm\"/>\n"
				        "		<label>Close</label>\n"
				        "	</sizer>\n"
				        "</composer>\n"));
				tmpButton->registerOnEvent(this, ewol::widget::Button::eventPressed, ewolEventParameterClose);
				mySizerHori->subWidgetAdd(tmpButton);
			}
		}
		
		mySizerHori = new ewol::widget::Sizer(widget::Sizer::modeHori);
		if (NULL == mySizerHori) {
			EWOL_ERROR("Can not allocate widget  == > display might be in error");
		} else {
			mySizerVert->subWidgetAdd(mySizerHori);
			
			m_paramList = new ewol::widget::ParameterList();
			if (NULL == m_paramList) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
			
				m_paramList->registerOnEvent(this, ewolEventParameterListSelect, l_eventMenuSelected);
				m_paramList->setFill(bvec2(false,true));
				m_paramList->setExpand(bvec2(false,true));
				mySizerHori->subWidgetAdd(m_paramList);
			}
			mySpacer = new ewol::widget::Spacer();
			if (NULL == mySpacer) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				mySpacer->setFill(bvec2(false,true));
				mySpacer->setMinSize(vec2(5,5));
				mySpacer->setColor(0x000000BF);
				mySizerHori->subWidgetAdd(mySpacer);
			}
			
			ewol::widget::Sizer * mySizerVert2 = new widget::Sizer(widget::Sizer::modeVert);
			if (NULL == mySizerVert2) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				mySizerHori->subWidgetAdd(mySizerVert2);
				
				mySpacer = new ewol::widget::Spacer();
				if (NULL == mySpacer) {
					EWOL_ERROR("Can not allocate widget  == > display might be in error");
				} else {
					mySpacer->setExpand(bvec2(true,false));
					mySpacer->setMinSize(vec2(5,5));
					mySpacer->setColor(0x000000BF);
					mySizerVert2->subWidgetAdd(mySpacer);
				}
				
				m_wSlider = new ewol::widget::WSlider();
				if (NULL == m_wSlider) {
					EWOL_ERROR("Can not allocate widget  == > display might be in error");
				} else {
					m_wSlider->setTransitionSpeed(0.5);
					m_wSlider->setTransitionMode(ewol::widget::WSlider::sladingTransitionVert);
					m_wSlider->setExpand(bvec2(true,true));
					mySizerVert2->subWidgetAdd(m_wSlider);
				}
			}
		}
		
		mySpacer = new ewol::widget::Spacer();
		if (NULL == mySpacer) {
			EWOL_ERROR("Can not allocate widget  == > display might be in error");
		} else {
			mySpacer->setExpand(bvec2(true,false));
			mySpacer->setMinSize(vec2(5,5));
			mySpacer->setColor(0x000000BF);
			mySizerVert->subWidgetAdd(mySpacer);
		}
		
		m_widgetTitle = new ewol::widget::Label("File chooser ...");
		if (NULL == m_widgetTitle) {
			EWOL_ERROR("Can not allocate widget  == > display might be in error");
		} else {
			m_widgetTitle->setExpand(bvec2(true,false));
			mySizerVert->subWidgetAdd(m_widgetTitle);
		}
	}
	markToRedraw();
}

ewol::widget::Parameter::~Parameter(void) {
	
}

void ewol::widget::Parameter::setTitle(std::string _label) {
	if (NULL == m_widgetTitle) {
		return;
	}
	m_widgetTitle->setLabel(_label);
}

void ewol::widget::Parameter::onReceiveMessage(const ewol::object::Message& _msg) {
	ewol::widget::PopUp::onReceiveMessage(_msg);
	EWOL_DEBUG("event on the parameter : " << _msg);
	if (_msg.getMessage() == ewolEventParameterClose) {
		// inform that the parameter windows is closed
		generateEventId(ewolEventParameterClose);
		// close this widget ...
		autoDestroy();
	} else if (_msg.getMessage() == ewolEventParameterSave) {
		//ewol::userConfig::Save();
		EWOL_TODO("Save Parameter !!! ");
	} else if (_msg.getMessage() == l_eventMenuSelected) {
		if (NULL != m_wSlider) {
			int32_t value = 0;
			sscanf(_msg.getData().c_str(), "%d", &value);
			EWOL_DEBUG("event on the parameter : " << _msg.getMessage() << " select ID=" << value << "");
			m_wSlider->subWidgetSelectSet(value);
		}
	}
	return;
}

void ewol::widget::Parameter::onObjectRemove(ewol::Object * _removeObject) {
	// First step call parrent : 
	ewol::widget::PopUp::onObjectRemove(_removeObject);
	// second step find if in all the elements ...
	if(_removeObject == m_widgetTitle) {
		m_widgetTitle = NULL;
	}
	if(_removeObject == m_paramList) {
		m_paramList = NULL;
	}
	if(_removeObject == m_wSlider) {
		m_wSlider = NULL;
	}
}

void ewol::widget::Parameter::menuAdd(std::string _label, std::string _image, ewol::Widget* _associateWidget) {
	if (NULL != m_paramList) {
		m_paramList->menuAdd(_label, m_currentIdList, _image);
		if (NULL != m_wSlider) {
			if (NULL != _associateWidget) {
				m_wSlider->subWidgetAdd(_associateWidget);
			} else { 
				EWOL_DEBUG("Associate an empty widget on it ...");
				ewol::widget::Label * myLabel = new widget::Label((std::string("No widget for : ") + _label));
				if (NULL == myLabel) {
					EWOL_ERROR("Can not allocate widget  == > display might be in error");
				} else {
					myLabel->setExpand(bvec2(true,true));
					m_wSlider->subWidgetAdd(myLabel);
				}
			}
			if (m_currentIdList == 0) {
				m_wSlider->subWidgetSelectSet(0);
			}
		}
		m_currentIdList++;
	}
}
void ewol::widget::Parameter::menuAddGroup(std::string _label) {
	if (NULL != m_paramList) {
		m_paramList->menuSeparator();
		m_paramList->menuAddGroup(_label);
	}
}

void ewol::widget::Parameter::menuClear(void) {
	if (NULL != m_paramList) {
		m_paramList->menuClear();
		m_currentIdList = 0;
	}
}

void ewol::widget::Parameter::menuSeparator(void) {
	if (NULL != m_paramList) {
		m_paramList->menuSeparator();
	}
}

