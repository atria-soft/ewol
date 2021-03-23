/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/meta/SpinBase.hpp>
#include <ewol/widget/Label.hpp>
#include <ewol/widget/Composer.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::SpinBase);
ETK_DECLARE_TYPE(enum ewol::widget::spinPosition);

ewol::widget::SpinBase::SpinBase() :
  propertyShape(this, "shape",
                      "",
                      "shape for the display",
                      ewol::widget::SpinBase::onChangePropertyShape),
  propertySpinMode(this, "spin-mode",
                         ewol::widget::spinPosition_RightRight,
                         "The display spin mode",
                         ewol::widget::SpinBase::onChangePropertySpinMode),
  this.confIdEntryShaper(-1),
  this.confIdUpShaper(-1),
  this.confIdDownShaper(-1),
  this.confIdUpData(-1),
  this.confIdDownData(-1) {
	
	addObjectType("ewol::widget::SpinBase");
	propertySpinMode.add(ewol::widget::spinPosition_noneNone, "none-none");
	propertySpinMode.add(ewol::widget::spinPosition_noneRight, "none-right");
	propertySpinMode.add(ewol::widget::spinPosition_leftNone, "left-none");
	propertySpinMode.add(ewol::widget::spinPosition_leftRight, "left-right");
	propertySpinMode.add(ewol::widget::spinPosition_leftLeft, "left-left");
	propertySpinMode.add(ewol::widget::spinPosition_RightRight, "right-right");
	propertyLockExpand.setDirectCheck(Vector2b(true,true));
	propertyGravity.setDirectCheck(gravity_center);
}

void ewol::widget::SpinBase::init() {
	ewol::widget::Sizer::init();
	propertyShape.notifyChange();
	updateGui();
}

ewol::widget::SpinBase::~SpinBase() {
	
}

void ewol::widget::SpinBase::onChangePropertySpinMode() {
	updateGui();
}

void ewol::widget::SpinBase::onChangePropertyShape() {
	this.config = ewol::resource::ConfigFile::create(propertyShape);
	if (this.config != null) {
		this.confIdEntryShaper = this.config.request("entry-shaper");
		this.confIdUpShaper = this.config.request("up-shaper");
		this.confIdDownShaper = this.config.request("down-shaper");
		this.confIdUpData = this.config.request("up-data");
		this.confIdDownData = this.config.request("down-data");
	}
	markToRedraw();
}



void ewol::widget::SpinBase::updateGui() {
	subWidgetRemoveAll();
	markToRedraw();
	requestUpdateSize();
	if (this.widgetEntry == null) {
		String shaper;
		if (this.config != null) {
			shaper = this.config.getString(this.confIdEntryShaper);
			Log.verbose("shaper entry : " + shaper);
		}
		this.widgetEntry = ewol::widget::Entry::create("shape", shaper);
		if (this.widgetEntry != null) {
			this.widgetEntry.propertyExpand.set(Vector2b(true,false));
			this.widgetEntry.propertyFill.set(Vector2b(true,true));
		}
	}
	if (this.widgetButtonDown == null) {
		String shaper;
		if (this.config != null) {
			shaper = this.config.getString(this.confIdDownShaper);
			Log.verbose("shaper button DOWN : " + shaper);
		}
		this.widgetButtonDown = ewol::widget::Button::create("shape", shaper);
		if (this.widgetButtonDown != null) {
			this.widgetButtonDown.propertyExpand.set(Vector2b(false,false));
			this.widgetButtonDown.propertyFill.set(Vector2b(true,true));
			String data = this.config.getString(this.confIdDownData);
			Widget widget = ewol::widget::composerGenerateString(data);
			this.widgetButtonDown.setSubWidget(widget);
		}
	}
	if (this.widgetButtonUp == null) {
		String shaper;
		if (this.config != null) {
			shaper = this.config.getString(this.confIdUpShaper);
			Log.verbose("shaper button UP : " + shaper);
		}
		this.widgetButtonUp = ewol::widget::Button::create("shape", shaper);
		if (this.widgetButtonUp != null) {
			this.widgetButtonUp.propertyExpand.set(Vector2b(false,false));
			this.widgetButtonUp.propertyFill.set(Vector2b(true,true));
			String data = this.config.getString(this.confIdUpData);
			Widget widget = ewol::widget::composerGenerateString(data);
			this.widgetButtonUp.setSubWidget(widget);
		}
	}
	switch (propertySpinMode) {
		case ewol::widget::spinPosition_noneNone:
			subWidgetAdd(this.widgetEntry);
			break;
		case ewol::widget::spinPosition_noneRight:
			subWidgetAdd(this.widgetEntry);
			subWidgetAdd(this.widgetButtonUp);
			break;
		case ewol::widget::spinPosition_leftNone:
			subWidgetAdd(this.widgetButtonDown);
			subWidgetAdd(this.widgetEntry);
			break;
		case ewol::widget::spinPosition_leftRight:
			subWidgetAdd(this.widgetButtonDown);
			subWidgetAdd(this.widgetEntry);
			subWidgetAdd(this.widgetButtonUp);
			break;
		case ewol::widget::spinPosition_leftLeft:
			subWidgetAdd(this.widgetButtonDown);
			subWidgetAdd(this.widgetButtonUp);
			subWidgetAdd(this.widgetEntry);
			break;
		case ewol::widget::spinPosition_RightRight:
			subWidgetAdd(this.widgetEntry);
			subWidgetAdd(this.widgetButtonDown);
			subWidgetAdd(this.widgetButtonUp);
			break;
	}
}

boolean ewol::widget::SpinBase::loadXML( exml::Element _node) {
	if (_node.exist() == false) {
		return false;
	}
	// parse generic properties: (we not parse the sizer property, it remove all subwidget)
	return Widget::loadXML(_node);
}