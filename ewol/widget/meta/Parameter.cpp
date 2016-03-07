/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
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

ewol::widget::Parameter::Parameter() :
  signalClose(this, "close", ""),
  propertyLabelTitle(this, "title",
                           "TRANSLATE:Parameter",
                           "Title of the parameter interface",
                           &ewol::widget::Parameter::onChangePropertyLabelTitle),
  m_currentIdList(0),
  m_widgetTitle(),
  m_paramList() {
	addObjectType("ewol::widget::Parameter");
}

void ewol::widget::Parameter::init() {
	ewol::widget::PopUp::init();
	
	ewol::widget::SizerShared mySizerVert = nullptr;
	ewol::widget::SizerShared mySizerHori = nullptr;
	ewol::widget::SpacerShared mySpacer = nullptr;
	#ifdef __TARGET_OS__Android
		propertyMinSize.set(gale::Dimension(vec2(90, 90), gale::Dimension::Pourcent));
	#else
		propertyMinSize.set(gale::Dimension(vec2(80, 80), gale::Dimension::Pourcent));
	#endif
	
	mySizerVert = ewol::widget::Sizer::create();
	if (mySizerVert == nullptr) {
		EWOL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		EWOL_INFO("add widget");
		mySizerVert->propertyMode.set(widget::Sizer::modeVert);
		mySizerVert->propertyLockExpand.set(bvec2(true,true));
		mySizerVert->propertyExpand.set(bvec2(true,true));
		// set it in the pop-up-system :
		setSubWidget(mySizerVert);
		
		mySizerHori = ewol::widget::Sizer::create();
		if (mySizerHori == nullptr) {
			EWOL_ERROR("Can not allocate widget  == > display might be in error");
		} else {
			mySizerHori->propertyMode.set(widget::Sizer::modeHori);
			mySizerVert->subWidgetAdd(mySizerHori);
			
			mySpacer = ewol::widget::Spacer::create();
			if (mySpacer == nullptr) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				mySpacer->propertyExpand.set(bvec2(true,false));
				mySizerHori->subWidgetAdd(mySpacer);
			}
			
			ewol::widget::ButtonShared tmpButton = widget::Button::create();
			if (tmpButton == nullptr) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				tmpButton->setSubWidget(ewol::widget::composerGenerateString(
				        "<sizer mode=\"hori\">\n"
				        "	<image src=\"{ewol}THEME:GUI:Save.svg\" expand=\"true\" size=\"8,8mm\"/>\n"
				        "	<label>Save</label>\n"
				        "</sizer>\n"));
				tmpButton->signalPressed.connect(shared_from_this(), &ewol::widget::Parameter::onCallbackParameterSave);
				mySizerHori->subWidgetAdd(tmpButton);
			}
			
			mySpacer = ewol::widget::Spacer::create();
			if (mySpacer == nullptr) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				mySpacer->propertyExpand.set(bvec2(false,false));
				mySpacer->propertyMinSize.set(gale::Dimension(vec2(10,0)));
				mySizerHori->subWidgetAdd(mySpacer);
			}
			
			tmpButton = ewol::widget::Button::create();
			if (tmpButton == nullptr) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				tmpButton->setSubWidget(ewol::widget::composerGenerateString(
				        "<sizer mode=\"hori\">\n"
				        "	<image src=\"{ewol}THEME:GUI:Remove.svg\" expand=\"true\" size=\"8,8mm\"/>\n"
				        "	<label>Close</label>\n"
				        "</sizer>\n"));
				tmpButton->signalPressed.connect(shared_from_this(), &ewol::widget::Parameter::onCallbackMenuclosed);
				mySizerHori->subWidgetAdd(tmpButton);
			}
		}
		
		mySizerHori = ewol::widget::Sizer::create();
		if (mySizerHori == nullptr) {
			EWOL_ERROR("Can not allocate widget  == > display might be in error");
		} else {
			mySizerHori->propertyMode.set(widget::Sizer::modeHori);
			mySizerVert->subWidgetAdd(mySizerHori);
			
			m_paramList = ewol::widget::ParameterList::create();
			if (m_paramList == nullptr) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
			
				m_paramList->signalSelect.connect(shared_from_this(), &ewol::widget::Parameter::onCallbackMenuSelected);
				m_paramList->propertyFill.set(bvec2(false,true));
				m_paramList->propertyExpand.set(bvec2(false,true));
				mySizerHori->subWidgetAdd(m_paramList);
			}
			mySpacer = ewol::widget::Spacer::create();
			if (mySpacer == nullptr) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				mySpacer->propertyFill.set(bvec2(false,true));
				mySpacer->propertyMinSize.set(vec2(5,5));
				mySpacer->propertyColor.set(0x000000BF);
				mySizerHori->subWidgetAdd(mySpacer);
			}
			
			ewol::widget::SizerShared mySizerVert2 = widget::Sizer::create();
			if (mySizerVert2 == nullptr) {
				EWOL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				mySizerVert2->propertyMode.set(widget::Sizer::modeVert);
				mySizerHori->subWidgetAdd(mySizerVert2);
				
				mySpacer = ewol::widget::Spacer::create();
				if (mySpacer == nullptr) {
					EWOL_ERROR("Can not allocate widget  == > display might be in error");
				} else {
					mySpacer->propertyExpand.set(bvec2(true,false));
					mySpacer->propertyMinSize.set(vec2(5,5));
					mySpacer->propertyColor.set(0x000000BF);
					mySizerVert2->subWidgetAdd(mySpacer);
				}
				
				m_wSlider = ewol::widget::WSlider::create();
				if (m_wSlider == nullptr) {
					EWOL_ERROR("Can not allocate widget  == > display might be in error");
				} else {
					m_wSlider->propertyTransitionSpeed.set(0.5);
					m_wSlider->propertyTransitionMode.set(ewol::widget::WSlider::sladingTransitionVert);
					m_wSlider->propertyExpand.set(bvec2(true,true));
					mySizerVert2->subWidgetAdd(m_wSlider);
				}
			}
		}
		
		mySpacer = ewol::widget::Spacer::create();
		if (mySpacer == nullptr) {
			EWOL_ERROR("Can not allocate widget  == > display might be in error");
		} else {
			mySpacer->propertyExpand.set(bvec2(true,false));
			mySpacer->propertyMinSize.set(vec2(5,5));
			mySpacer->propertyColor.set(0x000000BF);
			mySizerVert->subWidgetAdd(mySpacer);
		}
		
		m_widgetTitle = ewol::widget::Label::create();
		if (m_widgetTitle == nullptr) {
			EWOL_ERROR("Can not allocate widget  == > display might be in error");
		} else {
			m_widgetTitle->propertyValue.set(TRANSLATE(propertyLabelTitle));
			m_widgetTitle->propertyExpand.set(bvec2(true,false));
			mySizerVert->subWidgetAdd(m_widgetTitle);
		}
	}
	markToRedraw();
}

ewol::widget::Parameter::~Parameter() {
	
}

void ewol::widget::Parameter::onChangePropertyLabelTitle() {
	if (m_widgetTitle != nullptr) {
		m_widgetTitle->propertyValue.set(TRANSLATE(propertyLabelTitle));
	}
}

void ewol::widget::Parameter::onCallbackMenuclosed() {
	// inform that the parameter windows is closed
	signalClose.emit();
	// close this widget ...
	autoDestroy();
}
void ewol::widget::Parameter::onCallbackParameterSave() {
	//ewol::userConfig::Save();
	EWOL_TODO("Save Parameter !!! ");
}
void ewol::widget::Parameter::onCallbackMenuSelected(const int32_t& _value) {
	if (m_wSlider != nullptr) {
		EWOL_DEBUG("event on the parameter : Menu-select select ID=" << _value << "");
		m_wSlider->subWidgetSelectSet(_value);
	}
}

void ewol::widget::Parameter::menuAdd(std::string _label, std::string _image, ewol::WidgetShared _associateWidget) {
	if (nullptr != m_paramList) {
		m_paramList->menuAdd(_label, m_currentIdList, _image);
		if (nullptr != m_wSlider) {
			if (nullptr != _associateWidget) {
				m_wSlider->subWidgetAdd(_associateWidget);
			} else { 
				EWOL_DEBUG("Associate an empty widget on it ...");
				ewol::widget::LabelShared myLabel = widget::Label::create();
				if (nullptr == myLabel) {
					EWOL_ERROR("Can not allocate widget  == > display might be in error");
				} else {
					myLabel->propertyValue.set(std::string("No widget for : ") + _label);
					myLabel->propertyExpand.set(bvec2(true,true));
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
	if (nullptr != m_paramList) {
		m_paramList->menuSeparator();
		m_paramList->menuAddGroup(_label);
	}
}

void ewol::widget::Parameter::menuClear() {
	if (nullptr != m_paramList) {
		m_paramList->menuClear();
		m_currentIdList = 0;
	}
}

void ewol::widget::Parameter::menuSeparator() {
	if (nullptr != m_paramList) {
		m_paramList->menuSeparator();
	}
}

