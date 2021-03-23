/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ewol/debug.hpp>
#include <ewol/ewol.hpp>
#include <ewol/widget/Select.hpp>
#include <ewol/widget/ContextMenu.hpp>
#include <ewol/widget/Label.hpp>
#include <ewol/widget/Windows.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::Select);

ewol::widget::Select::Element::Element(int _value, String _name, boolean _selected):
  this.value(_value),
  this.name(_name),
  this.selected(_selected) {
	
}

ewol::widget::Select::Select() :
  signalValue(this, "value", "Select value change"),
  propertyValue(this, "value",
                      -1,
                      "Value of the Select",
                      ewol::widget::Select::onChangePropertyValue) {
	addObjectType("ewol::widget::Select");
	//  the basic parameter:
	propertyShape.setDirectCheck(etk::Uri("THEME_GUI:///Select.json?lib=ewol"));
	propertySpinMode.setDirect(ewol::widget::spinPosition_noneRight);
	propertySpinMode.changeDefault(ewol::widget::spinPosition_noneRight);
	propertySpinMode.rename("none-none", "none");
	propertySpinMode.rename("none-right", "right");
	propertySpinMode.rename("left-none", "left");
	propertySpinMode.remove("left-right");
	propertySpinMode.remove("left-left");
	propertySpinMode.remove("right-right");
}

ewol::widget::Select::~Select() {
	
}

void ewol::widget::Select::onChangePropertyValue() {
	markToRedraw();
	if (this.widgetEntry == null) {
		Log.error("Can not acces at entry ...");
		return;
	}
	for (auto it : this.listElement) {
		if (it.this.value == propertyValue.get()) {
			if (it.this.selected == false) {
				it.this.selected = true;
				this.widgetEntry.propertyValue.set(it.this.name);
				signalValue.emit(propertyValue.get());
			}
		} else {
			it.this.selected = false;
		}
	}
}

void ewol::widget::Select::optionSelectDefault() {
	if (this.widgetEntry == null) {
		Log.error("Can not acces at entry ...");
		return;
	}
	for (auto it : this.listElement) {
		if (it.this.selected == true) {
			return;
		}
	}
	if (this.listElement.size() == 0) {
		this.widgetEntry.propertyValue.set("");
	}
	this.widgetEntry.propertyValue.set(this.listElement[0].this.name);
}

void ewol::widget::Select::optionRemove(int _value) {
	for (auto it=this.listElement.begin(); it != this.listElement.end(); ++it) {
		if (_value == it.this.value) {
			Log.debug("remove element: " + _value);
			this.listElement.erase(it);
			break;
		}
	}
	optionSelectDefault();
}

void ewol::widget::Select::optionClear() {
	this.listElement.clear();
	optionSelectDefault();
}

void ewol::widget::Select::optionAdd(int _value, String _data) {
	for (auto it : this.listElement) {
		if (_value == it.this.value) {
			Log.debug("replace element: " + _value + " with: '" + _data + "'");
			it.this.name = _data;
		}
	}
	this.listElement.pushBack(ewol::widget::Select::Element(_value, _data, false));
}

boolean ewol::widget::Select::loadXML( exml::Element _node) {
	if (_node.exist() == false) {
		return false;
	}
	// parse generic properties:
	ewol::widget::SpinBase::loadXML(_node);
	// remove previous element:
	//subWidgetRemove();
	// parse all the elements:
	for( auto it : _node.nodes) {
		exml::Element pNode = it.toElement();
		if (pNode.exist() == false) {
			// trash here all that is not element
			continue;
		}
		if (pNode.getValue() != "option") {
			Log.error("(l " + pNode.getPos() + ") Unknown basic node='" + pNode.getValue() + "' not in : [option]" );
			continue;
		}
		String valId = pNode.attributes["id"];
		String valIsSelected = pNode.attributes["select"];
		String valText = pNode.getText();
		int id = etk::string_to_int(valId);
		boolean select = etk::string_to_bool(valIsSelected);
		optionAdd(id, valText);
		if (select == true) {
			propertyValue.set(id);
		}
		Log.warning("Add option : id='" + valId + "' select='" + valIsSelected + "' text='" + valText + "'");
	}
	return true;
}

void ewol::widget::Select::updateGui() {
	ewol::widget::SpinBase::updateGui();
	
	if (    this.widgetEntry != null
	     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.connectionEntry.isConnected() == false) {
		
	}
	if (    this.widgetButtonUp != null
	     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.connectionButton.isConnected() == false) {
		this.connectionButton = this.widgetButtonUp.signalPressed.connect(this, ewol::widget::Select::onCallbackOpenMenu);
	}
	
}

void ewol::widget::Select::onCallbackLabelPressed(int _value) {
	Log.verbose("User select:" + _value);
	propertyValue.set(_value);
}

void ewol::widget::Select::onCallbackOpenMenu() {
	// create a context menu:
	ewol::widget::ContextMenu tmpContext = ewol::widget::ContextMenu::create();
	if (tmpContext == null) {
		Log.error("Allocation Error");
		return;
	}
	// auto-select mark position:
	tmpContext.setPositionMarkAuto(this.origin, this.size);
	ewol::widget::Sizer mySizer;
	mySizer = ewol::widget::Sizer::create();
	if (mySizer == null) {
		Log.error("Allocation Error or sizer");
		return;
	}
	mySizer.propertyMode.set(widget::Sizer::modeVert);
	mySizer.propertyLockExpand.set(Vector2f(true,true));
	mySizer.propertyFill.set(Vector2f(true,true));
	// set it in the pop-up-system:
	tmpContext.setSubWidget(mySizer);
	for (auto it : this.listElement) {
		ewol::widget::Label myLabel = ewol::widget::Label::create();
		if (myLabel == null) {
			Log.error("Allocation Error");
			continue;
		}
		if (it.this.selected == true) {
			myLabel.propertyValue.set(String("<b>") + it.this.name + "</b>");
		} else {
			myLabel.propertyValue.set(it.this.name);
		}
		myLabel.propertyExpand.set(Vector2b(true,true));
		myLabel.propertyFill.set(Vector2b(true,true));
		// set callback
		myLabel.signalPressed.connect(sharedFromThis(), ewol::widget::Select::onCallbackLabelPressed, it.this.value);
		myLabel.signalPressed.connect(tmpContext, ewol::widget::ContextMenu::destroy);
		// add it in the widget list
		mySizer.subWidgetAddStart(myLabel);
	}
	ewol::widget::Windows currentWindows = getWindows();
	if (currentWindows == null) {
		Log.error("Can not get the curent Windows...");
	} else {
		currentWindows.popUpWidgetPush(tmpContext);
	}
}

