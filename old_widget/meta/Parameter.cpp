/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/meta/Parameter.hpp>
#include <ewol/widget/Sizer.hpp>
#include <ewol/widget/List.hpp>
#include <ewol/widget/Spacer.hpp>
#include <ewol/widget/Image.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/widget/Composer.hpp>
#include <etk/Vector.hpp>

#include <ewol/ewol.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::Parameter);

ewol::widget::Parameter::Parameter() :
  signalClose(this, "close", ""),
  propertyLabelTitle(this, "title",
                           "_T{Parameter}",
                           "Title of the parameter interface",
                           ewol::widget::Parameter::onChangePropertyLabelTitle),
  this.currentIdList(0),
  this.widgetTitle(),
  this.paramList() {
	addObjectType("ewol::widget::Parameter");
}

void ewol::widget::Parameter::init() {
	ewol::widget::PopUp::init();
	
	ewol::widget::Sizer mySizerVert = null;
	ewol::widget::Sizer mySizerHori = null;
	ewol::widget::Spacer mySpacer = null;
	#ifdef __TARGET_OS__Android
		propertyMinSize.set(gale::Dimension(Vector2f(90, 90), gale::distance::pourcent));
	#else
		propertyMinSize.set(gale::Dimension(Vector2f(80, 80), gale::distance::pourcent));
	#endif
	
	mySizerVert = ewol::widget::Sizer::create();
	if (mySizerVert == null) {
		LOGGER.error("Can not allocate widget  == > display might be in error");
	} else {
		LOGGER.info("add widget");
		mySizerVert.propertyMode.set(widget::Sizer::modeVert);
		mySizerVert.propertyLockExpand.set(Vector2b(true,true));
		mySizerVert.propertyExpand.set(Vector2b(true,true));
		// set it in the pop-up-system :
		setSubWidget(mySizerVert);
		
		mySizerHori = ewol::widget::Sizer::create();
		if (mySizerHori == null) {
			LOGGER.error("Can not allocate widget  == > display might be in error");
		} else {
			mySizerHori.propertyMode.set(widget::Sizer::modeHori);
			mySizerVert.subWidgetAdd(mySizerHori);
			
			mySpacer = ewol::widget::Spacer::create();
			if (mySpacer == null) {
				LOGGER.error("Can not allocate widget  == > display might be in error");
			} else {
				mySpacer.propertyExpand.set(Vector2b(true,false));
				mySizerHori.subWidgetAdd(mySpacer);
			}
			
			ewol::widget::Button tmpButton = widget::Button::create();
			if (tmpButton == null) {
				LOGGER.error("Can not allocate widget  == > display might be in error");
			} else {
				tmpButton.setSubWidget(ewol::widget::composerGenerateString(
				        "<sizer mode='hori'>\n"
				        "	<image src='THEME_GUI:///Save.svg?lib=ewol' expand='true' size='8,8mm'/>\n"
				        "	<label>_T{Save}</label>\n"
				        "</sizer>\n"));
				tmpButton.signalPressed.connect(sharedFromThis(), ewol::widget::Parameter::onCallbackParameterSave);
				mySizerHori.subWidgetAdd(tmpButton);
			}
			
			mySpacer = ewol::widget::Spacer::create();
			if (mySpacer == null) {
				LOGGER.error("Can not allocate widget  == > display might be in error");
			} else {
				mySpacer.propertyExpand.set(Vector2b(false,false));
				mySpacer.propertyMinSize.set(gale::Dimension(Vector2f(10,0)));
				mySizerHori.subWidgetAdd(mySpacer);
			}
			
			tmpButton = ewol::widget::Button::create();
			if (tmpButton == null) {
				LOGGER.error("Can not allocate widget  == > display might be in error");
			} else {
				tmpButton.setSubWidget(ewol::widget::composerGenerateString(
				        "<sizer mode='hori'>\n"
				        "	<image src='THEME_GUI:///Remove.svg?lib=ewol' expand='true' size='8,8mm'/>\n"
				        "	<label>_T{Close}</label>\n"
				        "</sizer>\n"));
				tmpButton.signalPressed.connect(sharedFromThis(), ewol::widget::Parameter::onCallbackMenuclosed);
				mySizerHori.subWidgetAdd(tmpButton);
			}
		}
		
		mySizerHori = ewol::widget::Sizer::create();
		if (mySizerHori == null) {
			LOGGER.error("Can not allocate widget  == > display might be in error");
		} else {
			mySizerHori.propertyMode.set(widget::Sizer::modeHori);
			mySizerVert.subWidgetAdd(mySizerHori);
			
			this.paramList = ewol::widget::ParameterList::create();
			if (this.paramList == null) {
				LOGGER.error("Can not allocate widget  == > display might be in error");
			} else {
			
				this.paramList.signalSelect.connect(sharedFromThis(), ewol::widget::Parameter::onCallbackMenuSelected);
				this.paramList.propertyFill.set(Vector2b(false,true));
				this.paramList.propertyExpand.set(Vector2b(false,true));
				mySizerHori.subWidgetAdd(this.paramList);
			}
			mySpacer = ewol::widget::Spacer::create();
			if (mySpacer == null) {
				LOGGER.error("Can not allocate widget  == > display might be in error");
			} else {
				mySpacer.propertyFill.set(Vector2b(false,true));
				mySpacer.propertyMinSize.set(Vector2f(5,5));
				mySpacer.propertyColor.set(0x000000BF);
				mySizerHori.subWidgetAdd(mySpacer);
			}
			
			ewol::widget::Sizer mySizerVert2 = widget::Sizer::create();
			if (mySizerVert2 == null) {
				LOGGER.error("Can not allocate widget  == > display might be in error");
			} else {
				mySizerVert2.propertyMode.set(widget::Sizer::modeVert);
				mySizerHori.subWidgetAdd(mySizerVert2);
				
				mySpacer = ewol::widget::Spacer::create();
				if (mySpacer == null) {
					LOGGER.error("Can not allocate widget  == > display might be in error");
				} else {
					mySpacer.propertyExpand.set(Vector2b(true,false));
					mySpacer.propertyMinSize.set(Vector2f(5,5));
					mySpacer.propertyColor.set(0x000000BF);
					mySizerVert2.subWidgetAdd(mySpacer);
				}
				
				this.wSlider = ewol::widget::WSlider::create();
				if (this.wSlider == null) {
					LOGGER.error("Can not allocate widget  == > display might be in error");
				} else {
					this.wSlider.propertyTransitionSpeed.set(0.5);
					this.wSlider.propertyTransitionMode.set(ewol::widget::WSlider::sladingTransitionVert);
					this.wSlider.propertyExpand.set(Vector2b(true,true));
					mySizerVert2.subWidgetAdd(this.wSlider);
				}
			}
		}
		
		mySpacer = ewol::widget::Spacer::create();
		if (mySpacer == null) {
			LOGGER.error("Can not allocate widget  == > display might be in error");
		} else {
			mySpacer.propertyExpand.set(Vector2b(true,false));
			mySpacer.propertyMinSize.set(Vector2f(5,5));
			mySpacer.propertyColor.set(0x000000BF);
			mySizerVert.subWidgetAdd(mySpacer);
		}
		
		this.widgetTitle = ewol::widget::Label::create();
		if (this.widgetTitle == null) {
			LOGGER.error("Can not allocate widget  == > display might be in error");
		} else {
			this.widgetTitle.propertyValue.set(propertyLabelTitle);
			this.widgetTitle.propertyExpand.set(Vector2b(true,false));
			mySizerVert.subWidgetAdd(this.widgetTitle);
		}
	}
	markToRedraw();
}

ewol::widget::Parameter::~Parameter() {
	
}

void ewol::widget::Parameter::onChangePropertyLabelTitle() {
	if (this.widgetTitle != null) {
		this.widgetTitle.propertyValue.set(propertyLabelTitle);
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
	LOGGER.info("[TODO] Save Parameter !!! ");
}
void ewol::widget::Parameter::onCallbackMenuSelected( int _value) {
	if (this.wSlider != null) {
		LOGGER.debug("event on the parameter : Menu-select select ID=" + _value + "");
		this.wSlider.subWidgetSelectSet(_value);
	}
}

void ewol::widget::Parameter::menuAdd(String _label, String _image, Widget _associateWidget) {
	if (this.paramList != null) {
		this.paramList.menuAdd(_label, this.currentIdList, _image);
		if (this.wSlider != null) {
			if (_associateWidget != null) {
				this.wSlider.subWidgetAdd(_associateWidget);
			} else { 
				LOGGER.debug("Associate an empty widget on it ...");
				ewol::widget::Label myLabel = widget::Label::create();
				if (myLabel == null) {
					LOGGER.error("Can not allocate widget  == > display might be in error");
				} else {
					myLabel.propertyValue.set(String("No widget for : ") + _label);
					myLabel.propertyExpand.set(Vector2b(true,true));
					this.wSlider.subWidgetAdd(myLabel);
				}
			}
			if (this.currentIdList == 0) {
				this.wSlider.subWidgetSelectSet(0);
			}
		}
		this.currentIdList++;
	}
}
void ewol::widget::Parameter::menuAddGroup(String _label) {
	if (this.paramList != null) {
		this.paramList.menuSeparator();
		this.paramList.menuAddGroup(_label);
	}
}

void ewol::widget::Parameter::menuClear() {
	if (this.paramList != null) {
		this.paramList.menuClear();
		this.currentIdList = 0;
	}
}

void ewol::widget::Parameter::menuSeparator() {
	if (this.paramList != null) {
		this.paramList.menuSeparator();
	}
}

