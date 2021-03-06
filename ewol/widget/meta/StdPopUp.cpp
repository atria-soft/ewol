/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/meta/StdPopUp.hpp>
#include <ewol/widget/Sizer.hpp>
#include <ewol/widget/Spacer.hpp>
#include <ewol/widget/Label.hpp>
#include <ewol/widget/Manager.hpp>
#include <etk/Vector.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::StdPopUp);

ewol::widget::StdPopUp::StdPopUp() :
  propertyTitle(this, "title",
                      "<bold>Message</bold>",
                      "Title of the pop-up",
                      &ewol::widget::StdPopUp::onChangePropertyTitle),
  propertyComment(this, "comment",
                        "No Label",
                        "Comment of the pop-up",
                        &ewol::widget::StdPopUp::onChangePropertyComment),
  m_title(null),
  m_comment(null),
  m_subBar(null) {
	addObjectType("ewol::widget::StdPopUp");
}

void ewol::widget::StdPopUp::init() {
	ewol::widget::PopUp::init();
	propertyMinSize.set(gale::Dimension(vec2(20,10),gale::distance::pourcent));
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
			mySpacer->propertyMinSize.set(gale::Dimension(vec2(0,3),gale::distance::pixel));
			mySizerVert->subWidgetAdd(mySpacer);
		
		mySpacer = ewol::widget::Spacer::create();
			mySpacer->propertyExpand.set(bvec2(true,false));
			mySpacer->propertyMinSize.set(gale::Dimension(vec2(0,5),gale::distance::pixel));
			mySizerVert->subWidgetAdd(mySpacer);
		
		m_comment = ewol::widget::Label::create();
			m_comment->propertyValue.set(*propertyComment);
			m_comment->propertyExpand.set(bvec2(true,true));
			mySizerVert->subWidgetAdd(m_comment);
		
		mySpacer = ewol::widget::Spacer::create();
			mySpacer->propertyExpand.set(bvec2(true,false));
			mySpacer->propertyMinSize.set(gale::Dimension(vec2(0,5),gale::distance::pixel));
			mySizerVert->subWidgetAdd(mySpacer);
		
		mySpacer = ewol::widget::Spacer::create();
			mySpacer->propertyExpand.set(bvec2(true,false));
			mySpacer->propertyColor.set(etk::Color<>(0x88, 0x88, 0x88, 0xFF));
			mySpacer->propertyMinSize.set(gale::Dimension(vec2(0,3),gale::distance::pixel));
			mySizerVert->subWidgetAdd(mySpacer);
		
		m_title = ewol::widget::Label::create();
			m_title->propertyValue.set(*propertyTitle);
			m_title->propertyExpand.set(bvec2(true,false));
			m_title->propertyFill.set(bvec2(true,true));
			mySizerVert->subWidgetAdd(m_title);
}

ewol::widget::StdPopUp::~StdPopUp() {
	
}

void ewol::widget::StdPopUp::onChangePropertyTitle() {
	if (m_title == null) {
		return;
	}
	m_title->propertyValue.set(*propertyTitle);
	markToRedraw();
}

void ewol::widget::StdPopUp::onChangePropertyComment() {
	if (m_comment == null) {
		return;
	}
	m_comment->propertyValue.set(*propertyComment);
	markToRedraw();
}

ewol::widget::ButtonShared ewol::widget::StdPopUp::addButton(const etk::String& _text, bool _autoExit) {
	if (m_subBar == null) {
		EWOL_ERROR("button-bar does not existed ...");
		return null;
	}
	ewol::widget::ButtonShared myButton = widget::Button::create();
	if (myButton == null) {
		EWOL_ERROR("Can not allocate new button ...");
		return null;
	}
	ewol::widget::LabelShared myLabel = ewol::widget::Label::create();
	if (myLabel == null) {
		EWOL_ERROR("Can not allocate new label ...");
		return null;
	}
	myLabel->propertyValue.set(_text);
	myButton->setSubWidget(myLabel);
	if(_autoExit == true) {
		myButton->signalPressed.connect(sharedFromThis(), &ewol::widget::StdPopUp::onCallBackButtonExit);
	}
	m_subBar->subWidgetAdd(myButton);
	markToRedraw();
	return myButton;
}

void ewol::widget::StdPopUp::onCallBackButtonExit() {
	autoDestroy();
}
