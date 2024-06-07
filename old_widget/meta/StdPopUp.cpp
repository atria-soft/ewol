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
                      ewol::widget::StdPopUp::onChangePropertyTitle),
  propertyComment(this, "comment",
                        "No Label",
                        "Comment of the pop-up",
                        ewol::widget::StdPopUp::onChangePropertyComment),
  this.title(null),
  this.comment(null),
  this.subBar(null) {
	addObjectType("ewol::widget::StdPopUp");
}

void ewol::widget::StdPopUp::init() {
	ewol::widget::PopUp::init();
	propertyMinSize.set(gale::Dimension(Vector2f(20,10),gale::distance::pourcent));
	ewol::widget::Sizer mySizerVert;
	ewol::widget::Spacer mySpacer;
	
	mySizerVert = ewol::widget::Sizer::create();
		// set it in the pop-up-system : 
		setSubWidget(mySizerVert);
		mySizerVert.propertyMode.set(widget::Sizer::modeVert);
		this.subBar = ewol::widget::Sizer::create();
			this.subBar.propertyMode.set(widget::Sizer::modeHori);
			this.subBar.propertyLockExpand.set(Vector2b(true,true));
			this.subBar.propertyExpand.set(Vector2b(true,false));
			mySizerVert.subWidgetAdd(this.subBar);
			mySpacer = ewol::widget::Spacer::create();
				mySpacer.propertyExpand.set(Vector2b(true,false));
				this.subBar.subWidgetAdd(mySpacer);
		
		mySpacer = ewol::widget::Spacer::create();
			mySpacer.propertyExpand.set(Vector2b(true,false));
			mySpacer.propertyColor.set(etk::Color<>(0x88, 0x88, 0x88, 0xFF));
			mySpacer.propertyMinSize.set(gale::Dimension(Vector2f(0,3),gale::distance::pixel));
			mySizerVert.subWidgetAdd(mySpacer);
		
		mySpacer = ewol::widget::Spacer::create();
			mySpacer.propertyExpand.set(Vector2b(true,false));
			mySpacer.propertyMinSize.set(gale::Dimension(Vector2f(0,5),gale::distance::pixel));
			mySizerVert.subWidgetAdd(mySpacer);
		
		this.comment = ewol::widget::Label::create();
			this.comment.propertyValue.set(*propertyComment);
			this.comment.propertyExpand.set(Vector2b(true,true));
			mySizerVert.subWidgetAdd(this.comment);
		
		mySpacer = ewol::widget::Spacer::create();
			mySpacer.propertyExpand.set(Vector2b(true,false));
			mySpacer.propertyMinSize.set(gale::Dimension(Vector2f(0,5),gale::distance::pixel));
			mySizerVert.subWidgetAdd(mySpacer);
		
		mySpacer = ewol::widget::Spacer::create();
			mySpacer.propertyExpand.set(Vector2b(true,false));
			mySpacer.propertyColor.set(etk::Color<>(0x88, 0x88, 0x88, 0xFF));
			mySpacer.propertyMinSize.set(gale::Dimension(Vector2f(0,3),gale::distance::pixel));
			mySizerVert.subWidgetAdd(mySpacer);
		
		this.title = ewol::widget::Label::create();
			this.title.propertyValue.set(*propertyTitle);
			this.title.propertyExpand.set(Vector2b(true,false));
			this.title.propertyFill.set(Vector2b(true,true));
			mySizerVert.subWidgetAdd(this.title);
}

ewol::widget::StdPopUp::~StdPopUp() {
	
}

void ewol::widget::StdPopUp::onChangePropertyTitle() {
	if (this.title == null) {
		return;
	}
	this.title.propertyValue.set(*propertyTitle);
	markToRedraw();
}

void ewol::widget::StdPopUp::onChangePropertyComment() {
	if (this.comment == null) {
		return;
	}
	this.comment.propertyValue.set(*propertyComment);
	markToRedraw();
}

ewol::widget::Button ewol::widget::StdPopUp::addButton( String _text, boolean _autoExit) {
	if (this.subBar == null) {
		LOGGER.error("button-bar does not existed ...");
		return null;
	}
	ewol::widget::Button myButton = widget::Button::create();
	if (myButton == null) {
		LOGGER.error("Can not allocate new button ...");
		return null;
	}
	ewol::widget::Label myLabel = ewol::widget::Label::create();
	if (myLabel == null) {
		LOGGER.error("Can not allocate new label ...");
		return null;
	}
	myLabel.propertyValue.set(_text);
	myButton.setSubWidget(myLabel);
	if(_autoExit == true) {
		myButton.signalPressed.connect(sharedFromThis(), ewol::widget::StdPopUp::onCallBackButtonExit);
	}
	this.subBar.subWidgetAdd(myButton);
	markToRedraw();
	return myButton;
}

void ewol::widget::StdPopUp::onCallBackButtonExit() {
	autoDestroy();
}
