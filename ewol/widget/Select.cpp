/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#include <ewol/debug.h>
#include <ewol/ewol.h>
#include <ewol/widget/Select.h>
#include <ewol/widget/ContextMenu.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Windows.h>

ewol::widget::Select::Element::Element(int32_t _value, std::string _name, bool _selected):
  m_value(_value),
  m_name(_name),
  m_selected(_selected) {
	
}

ewol::widget::Select::Select() :
  signalValue(this, "value", "Select value change"),
  propertyValue(this, "value",
                      -1,
                      "Value of the Select",
                      &ewol::widget::Select::onChangePropertyValue) {
	addObjectType("ewol::widget::Select");
	// override the basic parameter:
	propertyShape.setDirectCheck("{ewol}THEME:GUI:Select.json");
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
	if (m_widgetEntry == nullptr) {
		EWOL_ERROR("Can not acces at entry ...");
		return;
	}
	for (auto &it : m_listElement) {
		if (it.m_value == propertyValue.get()) {
			if (it.m_selected == false) {
				it.m_selected = true;
				m_widgetEntry->propertyValue.set(it.m_name);
				signalValue.emit(propertyValue.get());
			}
		} else {
			it.m_selected = false;
		}
	}
}

void ewol::widget::Select::optionSelectDefault() {
	if (m_widgetEntry == nullptr) {
		EWOL_ERROR("Can not acces at entry ...");
		return;
	}
	for (auto &it : m_listElement) {
		if (it.m_selected == true) {
			return;
		}
	}
	if (m_listElement.size() == 0) {
		m_widgetEntry->propertyValue.set("");
	}
	m_widgetEntry->propertyValue.set(m_listElement[0].m_name);
}

void ewol::widget::Select::optionRemove(int32_t _value) {
	for (auto it=m_listElement.begin(); it != m_listElement.end(); ++it) {
		if (_value == it->m_value) {
			EWOL_DEBUG("remove element: " << _value);
			m_listElement.erase(it);
			break;
		}
	}
	optionSelectDefault();
}

void ewol::widget::Select::optionClear() {
	m_listElement.clear();
	optionSelectDefault();
}

void ewol::widget::Select::optionAdd(int32_t _value, std::string _data) {
	for (auto &it : m_listElement) {
		if (_value == it.m_value) {
			EWOL_DEBUG("replace element: " << _value << " with: '" << _data << "'");
			it.m_name = _data;
		}
	}
	m_listElement.push_back(ewol::widget::Select::Element(_value, _data, false));
}

bool ewol::widget::Select::loadXML(const exml::Element& _node) {
	if (_node.exist() == false) {
		return false;
	}
	// parse generic properties:
	ewol::widget::SpinBase::loadXML(_node);
	// remove previous element:
	//subWidgetRemove();
	// parse all the elements:
	for(const auto it : _node.nodes) {
		exml::Element pNode = it.toElement();
		if (pNode.exist() == false) {
			// trash here all that is not element
			continue;
		}
		if (pNode.getValue() != "option") {
			EWOL_ERROR("(l " << pNode.getPos() << ") Unknown basic node='" << pNode.getValue() << "' not in : [option]" );
			continue;
		}
		std::string valId = pNode.attributes["id"];
		std::string valIsSelected = pNode.attributes["select"];
		std::string valText = pNode.getText();
		int32_t id = etk::string_to_int32_t(valId);
		bool select = etk::string_to_bool(valIsSelected);
		optionAdd(id, valText);
		if (select == true) {
			propertyValue.set(id);
		}
		EWOL_WARNING("Add option : id='" << valId << "' select='" << valIsSelected << "' text='" << valText << "'");
	}
	return true;
}

void ewol::widget::Select::updateGui() {
	ewol::widget::SpinBase::updateGui();
	
	if (    m_widgetEntry != nullptr
	     && m_connectionEntry.isConnected() == false) {
		
	}
	if (    m_widgetButtonUp != nullptr
	     && m_connectionButton.isConnected() == false) {
		m_connectionButton = m_widgetButtonUp->signalPressed.connect(this, &ewol::widget::Select::onCallbackOpenMenu);
	}
	
}

void ewol::widget::Select::onCallbackLabelPressed(int32_t _value) {
	EWOL_VERBOSE("User select:" << _value);
	propertyValue.set(_value);
}

void ewol::widget::Select::onCallbackOpenMenu() {
	// create a context menu:
	ewol::widget::ContextMenuShared tmpContext = ewol::widget::ContextMenu::create();
	if (tmpContext == nullptr) {
		EWOL_ERROR("Allocation Error");
		return;
	}
	// auto-select mark position:
	tmpContext->setPositionMarkAuto(m_origin, m_size);
	ewol::widget::SizerShared mySizer;
	mySizer = ewol::widget::Sizer::create();
	if (mySizer == nullptr) {
		EWOL_ERROR("Allocation Error or sizer");
		return;
	}
	mySizer->propertyMode.set(widget::Sizer::modeVert);
	mySizer->propertyLockExpand.set(vec2(true,true));
	mySizer->propertyFill.set(vec2(true,true));
	// set it in the pop-up-system:
	tmpContext->setSubWidget(mySizer);
	for (auto &it : m_listElement) {
		ewol::widget::LabelShared myLabel = ewol::widget::Label::create();
		if (myLabel == nullptr) {
			EWOL_ERROR("Allocation Error");
			continue;
		}
		if (it.m_selected == true) {
			myLabel->propertyValue.set(std::string("<b>") + it.m_name + "</b>");
		} else {
			myLabel->propertyValue.set(it.m_name);
		}
		myLabel->propertyExpand.set(bvec2(true,true));
		myLabel->propertyFill.set(bvec2(true,true));
		// set callback
		myLabel->signalPressed.connect(shared_from_this(), &ewol::widget::Select::onCallbackLabelPressed, it.m_value);
		myLabel->signalPressed.connect(tmpContext, &ewol::widget::ContextMenu::destroy);
		// add it in the widget list
		mySizer->subWidgetAddStart(myLabel);
	}
	ewol::widget::WindowsShared currentWindows = getWindows();
	if (currentWindows == nullptr) {
		EWOL_ERROR("Can not get the curent Windows...");
	} else {
		currentWindows->popUpWidgetPush(tmpContext);
	}
}

