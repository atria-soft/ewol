/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/widget/meta/StdPopUp.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Manager.h>
#include <vector>

#undef __class__
#define __class__ "ewol::StdPopUp"

ewol::widget::StdPopUp::StdPopUp() :
  m_title(nullptr),
  m_comment(nullptr),
  m_subBar(nullptr) {
	addObjectType("ewol::widget::StdPopUp");
}

void ewol::widget::StdPopUp::init() {
	ewol::widget::PopUp::init();
	propertyMinSize.set(gale::Dimension(vec2(20,10),gale::Dimension::Pourcent));
	ewol::widget::SizerShared mySizerVert;
	ewol::widget::SpacerShared mySpacer;
	
	mySizerVert = ewol::widget::Sizer::create();
		// set it in the pop-up-system : 
		setSubWidget(mySizerVert);
		mySizerVert->propertyMode.set(widget::Sizer::modeVert);
		m_subBar = ewol::widget::Sizer::create();
			m_subBar->propertyMode.set(widget::Sizer::modeHori);
			m_subBar->propertyLockExpand.set(bvec2(true,true));
			m_subBar->propertyExpand.set(bvec2(true,false));
			mySizerVert->subWidgetAdd(m_subBar);
			mySpacer = ewol::widget::Spacer::create();
				mySpacer->propertyExpand.set(bvec2(true,false));
				m_subBar->subWidgetAdd(mySpacer);
		
		mySpacer = ewol::widget::Spacer::create();
			mySpacer->propertyExpand.set(bvec2(true,false));
			mySpacer->propertyColor.set(etk::Color<>(0x88, 0x88, 0x88, 0xFF));
			mySpacer->propertyMinSize.set(gale::Dimension(vec2(0,3),gale::Dimension::Pixel));
			mySizerVert->subWidgetAdd(mySpacer);
		
		mySpacer = ewol::widget::Spacer::create();
			mySpacer->propertyExpand.set(bvec2(true,false));
			mySpacer->propertyMinSize.set(gale::Dimension(vec2(0,5),gale::Dimension::Pixel));
			mySizerVert->subWidgetAdd(mySpacer);
		
		m_comment = ewol::widget::Label::create();
			m_comment->propertyValue.set("No Label");
			m_comment->propertyExpand.set(bvec2(true,true));
			mySizerVert->subWidgetAdd(m_comment);
		
		mySpacer = ewol::widget::Spacer::create();
			mySpacer->propertyExpand.set(bvec2(true,false));
			mySpacer->propertyMinSize.set(gale::Dimension(vec2(0,5),gale::Dimension::Pixel));
			mySizerVert->subWidgetAdd(mySpacer);
		
		mySpacer = ewol::widget::Spacer::create();
			mySpacer->propertyExpand.set(bvec2(true,false));
			mySpacer->propertyColor.set(etk::Color<>(0x88, 0x88, 0x88, 0xFF));
			mySpacer->propertyMinSize.set(gale::Dimension(vec2(0,3),gale::Dimension::Pixel));
			mySizerVert->subWidgetAdd(mySpacer);
		
		m_title = ewol::widget::Label::create();
			m_title->propertyValue.set("<bold>Message</bold>");
			m_title->propertyExpand.set(bvec2(true,false));
			m_title->propertyFill.set(bvec2(true,true));
			mySizerVert->subWidgetAdd(m_title);
}

ewol::widget::StdPopUp::~StdPopUp() {
	
}

void ewol::widget::StdPopUp::setTitle(const std::string& _text) {
	if (m_title == nullptr) {
		return;
	}
	m_title->propertyValue.set(_text);
	markToRedraw();
}

void ewol::widget::StdPopUp::setComment(const std::string& _text) {
	if (m_comment == nullptr) {
		return;
	}
	m_comment->propertyValue.set(_text);
	markToRedraw();
}

ewol::widget::ButtonShared ewol::widget::StdPopUp::addButton(const std::string& _text, bool _autoExit) {
	if (m_subBar == nullptr) {
		EWOL_ERROR("button-bar does not existed ...");
		return nullptr;
	}
	ewol::widget::ButtonShared myButton = widget::Button::create();
	if (myButton == nullptr) {
		EWOL_ERROR("Can not allocate new button ...");
		return nullptr;
	}
	myButton->setSubWidget(ewol::widget::Label::create("value", _text));
	if(_autoExit == true) {
		myButton->signalPressed.connect(shared_from_this(), &ewol::widget::StdPopUp::onCallBackButtonExit);
	}
	m_subBar->subWidgetAdd(myButton);
	markToRedraw();
	return myButton;
}

void ewol::widget::StdPopUp::onCallBackButtonExit() {
	autoDestroy();
}
