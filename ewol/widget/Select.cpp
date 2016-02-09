/**
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

#undef __class__
#define __class__ "widget::Select::Element"


ewol::widget::Select::Element::Element(int32_t _value, std::string _name, bool _selected):
  m_value(_value),
  m_name(_name),
  m_selected(_selected) {
	
}

#undef __class__
#define __class__ "widget::Select"


ewol::widget::Select::Select() :
  signalValue(*this, "value", "Select value change"),
  m_value(*this, "value", false, "Value of the Select") {
	addObjectType("ewol::widget::Select");
}

void ewol::widget::Select::init(const std::string& _shaperName) {
	ewol::widget::SpinBase::init(ewol::widget::spinPosition_noneRight, _shaperName);
	//m_shaper->setSource(_shaperName);
	//m_shaperIdSize = m_shaper->requestConfig("box-size");
	//m_shaperIdSizeInsize = m_shaper->requestConfig("box-inside");
}

ewol::widget::Select::~Select() {
	
}

void ewol::widget::Select::onParameterChangeValue(const ewol::parameter::Ref& _paramPointer) {
	ewol::widget::SpinBase::onParameterChangeValue(_paramPointer);
	if (_paramPointer == m_value) {
		markToRedraw();
		if (m_widgetEntry == nullptr) {
			EWOL_ERROR("Can not acces at entry ...");
			return;
		}
		for (auto &it : m_listElement) {
			if (it.m_value == m_value.get()) {
				if (it.m_selected == false) {
					it.m_selected = true;
					m_widgetEntry->setValue(it.m_name);
					signalValue.emit(m_value.get());
				}
			} else {
				it.m_selected = false;
			}
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
		m_widgetEntry->setValue("");
	}
	m_widgetEntry->setValue(m_listElement[0].m_name);
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

bool ewol::widget::Select::loadXML(const std::shared_ptr<const exml::Element>& _node) {
	if (_node == nullptr) {
		return false;
	}
	// parse generic properties:
	ewol::Widget::loadXML(_node);
	// remove previous element:
	//subWidgetRemove();
	// parse all the elements:
	for(size_t iii=0; iii< _node->size(); iii++) {
		std::shared_ptr<const exml::Element> pNode = _node->getElement(iii);
		if (pNode == nullptr) {
			// trash here all that is not element
			continue;
		}
		if (pNode->getValue() != "option") {
			EWOL_ERROR("(l "<<pNode->getPos()<<") Unknown basic node='" << pNode->getValue() << "' not in : [option]" );
			continue;
		}
		std::string valId = pNode->getAttribute("id");
		std::string valIsSelected = pNode->getAttribute("select");
		std::string valText = pNode->getText();
		int32_t id = etk::string_to_int32_t(valId);
		int32_t select = etk::string_to_bool(valIsSelected);
		optionAdd(id, valText);
		if (select == true) {
			setValue(id);
		}
		
		EWOL_WARNING("Add option : id='" << valId << "' select='" << valIsSelected << "' text='" << valText << "'");
	}
	return true;
}

void ewol::widget::Select::updateGui() {
	ewol::widget::SpinBase::updateGui();
	
	if (m_widgetEntry != nullptr) {
		m_widgetEntry->signalUnBindAll(shared_from_this());
		
	}
	if (m_widgetButtonUp != nullptr) {
		m_widgetButtonUp->signalUnBindAll(shared_from_this());
		m_widgetButtonUp->signalPressed.bind(shared_from_this(), &ewol::widget::Select::onCallbackOpenMenu);
	}
	
}

void ewol::widget::Select::onCallbackLabelPressed(int32_t _value) {
	EWOL_VERBOSE("User select:" << _value);
	setValue(_value);
}

void ewol::widget::Select::onCallbackOpenMenu() {
	// create a context menu:
	std::shared_ptr<ewol::widget::ContextMenu> tmpContext = ewol::widget::ContextMenu::create();
	if (tmpContext == nullptr) {
		EWOL_ERROR("Allocation Error");
		return;
	}
	// auto-select mark position:
	tmpContext->setPositionMarkAuto(m_origin, m_size);
	std::shared_ptr<ewol::widget::Sizer> mySizer;
	mySizer = ewol::widget::Sizer::create(widget::Sizer::modeVert);
	if (mySizer == nullptr) {
		EWOL_ERROR("Allocation Error or sizer");
		return;
	}
	mySizer->lockExpand(vec2(true,true));
	mySizer->setFill(vec2(true,true));
	// set it in the pop-up-system:
	tmpContext->setSubWidget(mySizer);
	for (auto &it : m_listElement) {
		std::shared_ptr<ewol::widget::Label> myLabel;
		if (it.m_selected == true) {
			myLabel = ewol::widget::Label::create(std::string("<b>") + it.m_name + "</b>");
		} else {
			myLabel = ewol::widget::Label::create(it.m_name);
		}
		if (myLabel == nullptr) {
			EWOL_ERROR("Allocation Error");
			continue;
		}
		myLabel->setExpand(bvec2(true,true));
		myLabel->setFill(bvec2(true,true));
		// set callback
		myLabel->signalPressed.bind(shared_from_this(), &ewol::widget::Select::onCallbackLabelPressed, it.m_value);
		myLabel->signalPressed.bind(tmpContext, &ewol::widget::ContextMenu::destroy);
		// add it in the widget list
		mySizer->subWidgetAddStart(myLabel);
	}
	std::shared_ptr<ewol::widget::Windows> currentWindows = getWindows();
	if (currentWindows == nullptr) {
		EWOL_ERROR("Can not get the curent Windows...");
	} else {
		currentWindows->popUpWidgetPush(tmpContext);
	}
}


void ewol::widget::Select::setValue(int32_t _val) {
	m_value.set(_val);
}
bool ewol::widget::Select::getValue() const {
	return m_value;
};