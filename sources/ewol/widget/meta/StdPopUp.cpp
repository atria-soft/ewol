/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/meta/StdPopUp.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Manager.h>
#include <vector>

#undef __class__
#define __class__ "ewol::StdPopUp"

static const char * const eventButtonExit = "ewol-event-pop-up-exit-button";

ewol::widget::StdPopUp::StdPopUp() :
  m_title(nullptr),
  m_comment(nullptr),
  m_subBar(nullptr) {
	addObjectType("ewol::widget::StdPopUp");
	setMinSize(ewol::Dimension(vec2(20,10),ewol::Dimension::Pourcent));
}

void ewol::widget::StdPopUp::init() {
	ewol::widget::PopUp::init();
	std::shared_ptr<ewol::widget::Sizer> mySizerVert;
	std::shared_ptr<ewol::widget::Spacer> mySpacer;
	
	mySizerVert = ewol::widget::Sizer::create(widget::Sizer::modeVert);
		// set it in the pop-up-system : 
		setSubWidget(mySizerVert);
		
		m_subBar = ewol::widget::Sizer::create(widget::Sizer::modeHori);
			m_subBar->lockExpand(bvec2(true,true));
			m_subBar->setExpand(bvec2(true,false));
			mySizerVert->subWidgetAdd(m_subBar);
			mySpacer = ewol::widget::Spacer::create();
				mySpacer->setExpand(bvec2(true,false));
				m_subBar->subWidgetAdd(mySpacer);
		
		mySpacer = ewol::widget::Spacer::create();
			mySpacer->setExpand(bvec2(true,false));
			mySpacer->setColor(etk::Color<>(0x88, 0x88, 0x88, 0xFF));
			mySpacer->setMinSize(ewol::Dimension(vec2(0,3),ewol::Dimension::Pixel));
			mySizerVert->subWidgetAdd(mySpacer);
		
		mySpacer = ewol::widget::Spacer::create();
			mySpacer->setExpand(bvec2(true,false));
			mySpacer->setMinSize(ewol::Dimension(vec2(0,5),ewol::Dimension::Pixel));
			mySizerVert->subWidgetAdd(mySpacer);
		
		m_comment = ewol::widget::Label::create("No Label");
			m_comment->setExpand(bvec2(true,true));
			mySizerVert->subWidgetAdd(m_comment);
		
		mySpacer = ewol::widget::Spacer::create();
			mySpacer->setExpand(bvec2(true,false));
			mySpacer->setMinSize(ewol::Dimension(vec2(0,5),ewol::Dimension::Pixel));
			mySizerVert->subWidgetAdd(mySpacer);
		
		mySpacer = ewol::widget::Spacer::create();
			mySpacer->setExpand(bvec2(true,false));
			mySpacer->setColor(etk::Color<>(0x88, 0x88, 0x88, 0xFF));
			mySpacer->setMinSize(ewol::Dimension(vec2(0,3),ewol::Dimension::Pixel));
			mySizerVert->subWidgetAdd(mySpacer);
		
		m_title = ewol::widget::Label::create("<bold>Message</bold>");
			m_title->setExpand(bvec2(true,false));
			m_title->setFill(bvec2(true,true));
			mySizerVert->subWidgetAdd(m_title);
}

ewol::widget::StdPopUp::~StdPopUp() {
	
}

void ewol::widget::StdPopUp::setTitle(const std::string& _text) {
	if (m_title == nullptr) {
		return;
	}
	m_title->setLabel(_text);
	markToRedraw();
}

void ewol::widget::StdPopUp::setComment(const std::string& _text) {
	if (m_comment == nullptr) {
		return;
	}
	m_comment->setLabel(_text);
	markToRedraw();
}

std::shared_ptr<ewol::widget::Button> ewol::widget::StdPopUp::addButton(const std::string& _text, bool _autoExit) {
	if (m_subBar == nullptr) {
		EWOL_ERROR("button-bar does not existed ...");
		return nullptr;
	}
	std::shared_ptr<ewol::widget::Button> myButton = widget::Button::create();
	if (myButton == nullptr) {
		EWOL_ERROR("Can not allocate new button ...");
		return nullptr;
	}
	myButton->setSubWidget(ewol::widget::Label::create(_text));
	if(_autoExit == true) {
		myButton->registerOnEvent(shared_from_this(), ewol::widget::Button::eventPressed, eventButtonExit);
	}
	m_subBar->subWidgetAdd(myButton);
	markToRedraw();
	return myButton;
}

void ewol::widget::StdPopUp::onObjectRemove(const std::shared_ptr<ewol::Object>& _removeObject) {
	// call parent:
	ewol::widget::PopUp::onObjectRemove(_removeObject);
	if (_removeObject == m_subBar) {
		m_subBar = nullptr;
		markToRedraw();
		return;
	}
	if (_removeObject == m_comment) {
		m_comment = nullptr;
		markToRedraw();
		return;
	}
	if (_removeObject == m_title) {
		m_title = nullptr;
		markToRedraw();
		return;
	}
}

void ewol::widget::StdPopUp::onReceiveMessage(const ewol::object::Message& _msg) {
	// call parent:
	ewol::widget::PopUp::onReceiveMessage(_msg);
	if (_msg.getMessage() == eventButtonExit) {
		autoDestroy();
	}
}
