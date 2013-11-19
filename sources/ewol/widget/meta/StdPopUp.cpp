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
#include <ewol/widget/WidgetManager.h>
#include <vector>

#undef __class__
#define __class__ "ewol::StdPopUp"

static const char * const eventButtonExit = "ewol-event-pop-up-exit-button";

widget::StdPopUp::StdPopUp(void) :
  m_title(NULL),
  m_comment(NULL),
  m_subBar(NULL) {
	addObjectType("widget::StdPopUp");
	setMinSize(ewol::Dimension(vec2(20,10),ewol::Dimension::Pourcent));
	
	widget::Sizer* mySizerVert = NULL;
	widget::Spacer* mySpacer = NULL;
	
	mySizerVert = new widget::Sizer(widget::Sizer::modeVert);
		// set it in the pop-up-system : 
		setSubWidget(mySizerVert);
		
		m_subBar = new widget::Sizer(widget::Sizer::modeHori);
			m_subBar->lockExpand(bvec2(true,true));
			m_subBar->setExpand(bvec2(true,false));
			mySizerVert->subWidgetAdd(m_subBar);
			mySpacer = new widget::Spacer();
				mySpacer->setExpand(bvec2(true,false));
				m_subBar->subWidgetAdd(mySpacer);
		
		mySpacer = new widget::Spacer();
			mySpacer->setExpand(bvec2(true,false));
			mySpacer->setColor(etk::Color<>(0x888888FF));
			mySpacer->setMinSize(ewol::Dimension(vec2(0,3),ewol::Dimension::Pixel));
			mySizerVert->subWidgetAdd(mySpacer);
		
		mySpacer = new widget::Spacer();
			mySpacer->setExpand(bvec2(true,false));
			mySpacer->setMinSize(ewol::Dimension(vec2(0,5),ewol::Dimension::Pixel));
			mySizerVert->subWidgetAdd(mySpacer);
		
		m_comment = new widget::Label("No Label");
			m_comment->setExpand(bvec2(true,true));
			mySizerVert->subWidgetAdd(m_comment);
		
		mySpacer = new widget::Spacer();
			mySpacer->setExpand(bvec2(true,false));
			mySpacer->setMinSize(ewol::Dimension(vec2(0,5),ewol::Dimension::Pixel));
			mySizerVert->subWidgetAdd(mySpacer);
		
		mySpacer = new widget::Spacer();
			mySpacer->setExpand(bvec2(true,false));
			mySpacer->setColor(etk::Color<>(0x888888FF));
			mySpacer->setMinSize(ewol::Dimension(vec2(0,3),ewol::Dimension::Pixel));
			mySizerVert->subWidgetAdd(mySpacer);
		
		m_title = new widget::Label("<bold>Message</bold>");
			m_title->setExpand(bvec2(true,false));
			m_title->setFill(bvec2(true,true));
			mySizerVert->subWidgetAdd(m_title);
}

widget::StdPopUp::~StdPopUp(void) {
	
}

void widget::StdPopUp::setTitle(const std::string& _text) {
	if (m_title == NULL) {
		return;
	}
	m_title->setLabel(_text);
	markToRedraw();
}

void widget::StdPopUp::setComment(const std::string& _text) {
	if (m_comment == NULL) {
		return;
	}
	m_comment->setLabel(_text);
	markToRedraw();
}

widget::Button* widget::StdPopUp::addButton(const std::string& _text, bool _autoExit) {
	if (m_subBar == NULL) {
		EWOL_ERROR("button-bar does not existed ...");
		return NULL;
	}
	widget::Button* myButton = new widget::Button();
	if (myButton == NULL) {
		EWOL_ERROR("Can not allocate new button ...");
		return NULL;
	}
	myButton->setSubWidget(new widget::Label(_text));
	if(_autoExit == true) {
		myButton->registerOnEvent(this, widget::Button::eventPressed, eventButtonExit);
	}
	m_subBar->subWidgetAdd(myButton);
	markToRedraw();
	return myButton;
}

void widget::StdPopUp::onObjectRemove(ewol::EObject* _removeObject) {
	// call parent:
	widget::PopUp::onObjectRemove(_removeObject);
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

void widget::StdPopUp::onReceiveMessage(const ewol::EMessage& _msg) {
	// call parent:
	widget::PopUp::onReceiveMessage(_msg);
	if (_msg.getMessage() == eventButtonExit) {
		autoDestroy();
	}
}
