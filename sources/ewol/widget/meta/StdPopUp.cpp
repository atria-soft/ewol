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
  m_title(NULL),
  m_comment(NULL),
  m_subBar(NULL) {
	addObjectType("ewol::widget::StdPopUp");
	setMinSize(ewol::Dimension(vec2(20,10),ewol::Dimension::Pourcent));
	
	ewol::widget::Sizer* mySizerVert = NULL;
	ewol::widget::Spacer* mySpacer = NULL;
	
	mySizerVert = new ewol::widget::Sizer(widget::Sizer::modeVert);
		// set it in the pop-up-system : 
		setSubWidget(mySizerVert);
		
		m_subBar = new ewol::widget::Sizer(widget::Sizer::modeHori);
			m_subBar->lockExpand(bvec2(true,true));
			m_subBar->setExpand(bvec2(true,false));
			mySizerVert->subWidgetAdd(m_subBar.get());
			mySpacer = new ewol::widget::Spacer();
				mySpacer->setExpand(bvec2(true,false));
				m_subBar->subWidgetAdd(mySpacer);
		
		mySpacer = new ewol::widget::Spacer();
			mySpacer->setExpand(bvec2(true,false));
			mySpacer->setColor(etk::Color<>(0x888888FF));
			mySpacer->setMinSize(ewol::Dimension(vec2(0,3),ewol::Dimension::Pixel));
			mySizerVert->subWidgetAdd(mySpacer);
		
		mySpacer = new ewol::widget::Spacer();
			mySpacer->setExpand(bvec2(true,false));
			mySpacer->setMinSize(ewol::Dimension(vec2(0,5),ewol::Dimension::Pixel));
			mySizerVert->subWidgetAdd(mySpacer);
		
		m_comment = new ewol::widget::Label("No Label");
			m_comment->setExpand(bvec2(true,true));
			mySizerVert->subWidgetAdd(m_comment.get());
		
		mySpacer = new ewol::widget::Spacer();
			mySpacer->setExpand(bvec2(true,false));
			mySpacer->setMinSize(ewol::Dimension(vec2(0,5),ewol::Dimension::Pixel));
			mySizerVert->subWidgetAdd(mySpacer);
		
		mySpacer = new ewol::widget::Spacer();
			mySpacer->setExpand(bvec2(true,false));
			mySpacer->setColor(etk::Color<>(0x888888FF));
			mySpacer->setMinSize(ewol::Dimension(vec2(0,3),ewol::Dimension::Pixel));
			mySizerVert->subWidgetAdd(mySpacer);
		
		m_title = new ewol::widget::Label("<bold>Message</bold>");
			m_title->setExpand(bvec2(true,false));
			m_title->setFill(bvec2(true,true));
			mySizerVert->subWidgetAdd(m_title.get());
}

ewol::widget::StdPopUp::~StdPopUp() {
	
}

void ewol::widget::StdPopUp::setTitle(const std::string& _text) {
	if (m_title == NULL) {
		return;
	}
	m_title->setLabel(_text);
	markToRedraw();
}

void ewol::widget::StdPopUp::setComment(const std::string& _text) {
	if (m_comment == NULL) {
		return;
	}
	m_comment->setLabel(_text);
	markToRedraw();
}

ewol::object::Shared<ewol::widget::Button> ewol::widget::StdPopUp::addButton(const std::string& _text, bool _autoExit) {
	if (m_subBar == NULL) {
		EWOL_ERROR("button-bar does not existed ...");
		return NULL;
	}
	ewol::widget::Button* myButton = new widget::Button();
	if (myButton == NULL) {
		EWOL_ERROR("Can not allocate new button ...");
		return NULL;
	}
	myButton->setSubWidget(new ewol::widget::Label(_text));
	if(_autoExit == true) {
		myButton->registerOnEvent(this, ewol::widget::Button::eventPressed, eventButtonExit);
	}
	m_subBar->subWidgetAdd(myButton);
	markToRedraw();
	return myButton;
}

void ewol::widget::StdPopUp::onObjectRemove(ewol::object::Shared<ewol::Object> _removeObject) {
	// call parent:
	ewol::widget::PopUp::onObjectRemove(_removeObject);
	if (_removeObject == m_subBar) {
		m_subBar = NULL;
		markToRedraw();
		return;
	}
	if (_removeObject == m_comment) {
		m_comment = NULL;
		markToRedraw();
		return;
	}
	if (_removeObject == m_title) {
		m_title = NULL;
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
