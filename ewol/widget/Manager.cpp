/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/ewol.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/widget/Joystick.hpp>
#include <ewol/widget/Button.hpp>
#include <ewol/widget/ContextMenu.hpp>
#include <ewol/widget/ButtonColor.hpp>
#include <ewol/widget/Spacer.hpp>
#include <ewol/widget/Slider.hpp>
#include <ewol/widget/Sizer.hpp>
#include <ewol/widget/ProgressBar.hpp>
#include <ewol/widget/Layer.hpp>
#include <ewol/widget/Label.hpp>
#include <ewol/widget/Image.hpp>
#include <ewol/widget/Gird.hpp>
#include <ewol/widget/Entry.hpp>
#include <ewol/widget/CheckBox.hpp>
#include <ewol/widget/Scroll.hpp>
#include <ewol/widget/PopUp.hpp>
#include <ewol/widget/WSlider.hpp>
#include <ewol/widget/ListFileSystem.hpp>
#include <ewol/widget/Composer.hpp>
#include <ewol/widget/Select.hpp>
#include <ewol/widget/Spin.hpp>
#include <ewol/widget/Menu.hpp>
#include <vector>

ewol::widget::Manager::Manager() :
  m_haveRedraw(true) {
	EWOL_DEBUG(" == > init Widget-Manager");
	
	ewol::widget::Button::createManagerWidget(*this);
	ewol::widget::ButtonColor::createManagerWidget(*this);
	ewol::widget::Spacer::createManagerWidget(*this);
	ewol::widget::Slider::createManagerWidget(*this);
	ewol::widget::Sizer::createManagerWidget(*this);
	ewol::widget::ProgressBar::createManagerWidget(*this);
	ewol::widget::Layer::createManagerWidget(*this);
	ewol::widget::Label::createManagerWidget(*this);
	ewol::widget::Image::createManagerWidget(*this);
	ewol::widget::Gird::createManagerWidget(*this);
	ewol::widget::Entry::createManagerWidget(*this);
	ewol::widget::Menu::createManagerWidget(*this);
	ewol::widget::CheckBox::createManagerWidget(*this);
	ewol::widget::Scroll::createManagerWidget(*this);
	ewol::widget::ContextMenu::createManagerWidget(*this);
	ewol::widget::PopUp::createManagerWidget(*this);
	ewol::widget::WSlider::createManagerWidget(*this);
	ewol::widget::ListFileSystem::createManagerWidget(*this);
	ewol::widget::Composer::createManagerWidget(*this);
	ewol::widget::Select::createManagerWidget(*this);
	ewol::widget::Spin::createManagerWidget(*this);
}

ewol::widget::Manager::~Manager() {
	EWOL_DEBUG(" == > Un-Init Widget-Manager");
	EWOL_INFO("Realease all FOCUS");
	focusSetDefault(nullptr);
	focusRelease();
	
	m_creatorList.clear();
}

/* *************************************************************************
 * focus Area : 
 * *************************************************************************/

void ewol::widget::Manager::focusKeep(ewol::WidgetShared _newWidget) {
	if (_newWidget == nullptr) {
		// nothing to do ...
		return;
	}
	EWOL_DEBUG("focusKeep=" << _newWidget->getId() );
	//elog::displayBacktrace();
	auto focusWidgetCurrent = m_focusWidgetCurrent.lock();
	if (_newWidget == focusWidgetCurrent) {
		// nothing to do ...
		return;
	}
	if (focusWidgetCurrent != nullptr) {
		EWOL_DEBUG("Rm focus on WidgetID=" << focusWidgetCurrent->getId() );
		focusWidgetCurrent->rmFocus();
		focusWidgetCurrent.reset();
	}
	if (_newWidget->propertyCanFocus.get() == false) {
		EWOL_DEBUG("Widget can not have focus, id=" << _newWidget->getId() );
		return;
	}
	m_focusWidgetCurrent = _newWidget;
	if (_newWidget != nullptr) {
		EWOL_DEBUG("Set focus on WidgetID=" << _newWidget->getId() );
		_newWidget->setFocus();
	}
}

void ewol::widget::Manager::focusSetDefault(ewol::WidgetShared _newWidget) {
	if(    _newWidget != nullptr
	    && _newWidget->propertyCanFocus.get() == false) {
		EWOL_VERBOSE("Widget can not have focus, id=" << _newWidget->getId() );
		return;
	}
	ewol::WidgetShared focusWidgetDefault = m_focusWidgetDefault.lock();
	ewol::WidgetShared focusWidgetCurrent = m_focusWidgetCurrent.lock();
	if (focusWidgetDefault == focusWidgetCurrent) {
		if (focusWidgetCurrent != nullptr) {
			EWOL_DEBUG("Rm focus on WidgetID=" << focusWidgetCurrent->getId() );
			focusWidgetCurrent->rmFocus();
		}
		m_focusWidgetCurrent = _newWidget;
		if (_newWidget != nullptr) {
			EWOL_DEBUG("Set focus on WidgetID=" << _newWidget->getId() );
			_newWidget->setFocus();
		}
	}
	m_focusWidgetDefault = _newWidget;
}

void ewol::widget::Manager::focusRelease() {
	ewol::WidgetShared focusWidgetDefault = m_focusWidgetDefault.lock();
	ewol::WidgetShared focusWidgetCurrent = m_focusWidgetCurrent.lock();
	if (focusWidgetDefault == focusWidgetCurrent) {
		// nothink to do ...
		return;
	}
	if (focusWidgetCurrent != nullptr) {
		EWOL_DEBUG("Rm focus on WidgetID=" << focusWidgetCurrent->getId() );
		focusWidgetCurrent->rmFocus();
	}
	m_focusWidgetCurrent = m_focusWidgetDefault;
	focusWidgetCurrent = m_focusWidgetCurrent.lock();
	if (focusWidgetCurrent != nullptr) {
		EWOL_DEBUG("Set focus on WidgetID=" << focusWidgetCurrent->getId() );
		focusWidgetCurrent->setFocus();
	}
}

ewol::WidgetShared ewol::widget::Manager::focusGet() {
	return m_focusWidgetCurrent.lock();
}

void ewol::widget::Manager::setCallbackonRedrawNeeded(const std::function<void()>& _func) {
	m_funcRedrawNeeded = _func;
}

void ewol::widget::Manager::markDrawingIsNeeded() {
	if (m_haveRedraw == true) {
		return;
	}
	m_haveRedraw = true;
	if (m_funcRedrawNeeded != nullptr) {
		m_funcRedrawNeeded();
	}
}

bool ewol::widget::Manager::isDrawingNeeded() {
	bool tmp = m_haveRedraw;
	m_haveRedraw = false;
	return tmp;
}

// element that generate the list of elements
void ewol::widget::Manager::addWidgetCreator(const std::string& _name,
                                             ewol::widget::Manager::widgetCreatorFunction _pointer) {
	if (_pointer == nullptr) {
		return;
	}
	//Keep name in lower case :
	std::string nameLower = etk::tolower(_name);
	auto it = m_creatorList.find(nameLower);
	if (it != m_creatorList.end()) {
		EWOL_WARNING("Replace Creator of a specify widget : " << nameLower);
		it->second = _pointer;
		return;
	}
	EWOL_INFO("Add Creator of a specify widget : " << nameLower);
	m_creatorList.insert(make_pair(nameLower, _pointer));
}

ewol::WidgetShared ewol::widget::Manager::create(const std::string& _name) {
	std::string nameLower = etk::tolower(_name);
	auto it = m_creatorList.find(nameLower);
	if (it != m_creatorList.end()) {
		if (it->second != nullptr) {
			return it->second();
		}
	}
	EWOL_WARNING("try to create an UnExistant widget : " << nameLower);
	return nullptr;
}

bool ewol::widget::Manager::exist(const std::string& _name) {
	std::string nameLower = etk::tolower(_name);
	auto it = m_creatorList.find(nameLower);
	if (it == m_creatorList.end()) {
		return false;
	}
	return true;
}

std::string ewol::widget::Manager::list() {
	std::string tmpVal;
	for (auto &it : m_creatorList) {
		if (tmpVal.size() != 0) {
			tmpVal += ",";
		}
		tmpVal += it.first;
	}
	return tmpVal;
}

