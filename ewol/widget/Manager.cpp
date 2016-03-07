/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/ewol.h>
#include <ewol/widget/Manager.h>
#include <ewol/widget/Joystick.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/ContextMenu.h>
#include <ewol/widget/ButtonColor.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Slider.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/ProgressBar.h>
#include <ewol/widget/Layer.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Image.h>
#include <ewol/widget/Gird.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/CheckBox.h>
#include <ewol/widget/Scroll.h>
#include <ewol/widget/PopUp.h>
#include <ewol/widget/WSlider.h>
#include <ewol/widget/ListFileSystem.h>
#include <ewol/widget/Composer.h>
#include <ewol/widget/Select.h>
#include <ewol/widget/Spin.h>
#include <vector>

#undef __class__
#define __class__ "ewol::widget::Manager"

ewol::widget::Manager::Manager() :
  m_havePeriodic(false),
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

void ewol::widget::Manager::focusKeep(const ewol::WidgetShared& _newWidget) {
	if (_newWidget == nullptr) {
		// nothing to do ...
		return;
	}
	EWOL_VERBOSE("focusKeep=" << _newWidget->getId() );
	//etk::log::displayBacktrace();
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

void ewol::widget::Manager::focusSetDefault(const ewol::WidgetShared& _newWidget) {
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
	if (nullptr != focusWidgetCurrent) {
		EWOL_DEBUG("Set focus on WidgetID=" << focusWidgetCurrent->getId() );
		focusWidgetCurrent->setFocus();
	}
}


ewol::WidgetShared ewol::widget::Manager::focusGet() {
	return m_focusWidgetCurrent.lock();
}

void ewol::widget::Manager::focusRemoveIfRemove(const ewol::WidgetShared& _newWidget) {
	ewol::WidgetShared focusWidgetDefault = m_focusWidgetDefault.lock();
	ewol::WidgetShared focusWidgetCurrent = m_focusWidgetCurrent.lock();
	if (focusWidgetCurrent == _newWidget) {
		EWOL_WARNING("Release focus when remove widget");
		focusRelease();
	}
	if (focusWidgetDefault == _newWidget) {
		EWOL_WARNING("Release default focus when remove widget");
		focusSetDefault(nullptr);
	}
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
                                             ewol::widget::Manager::creator_tf _pointer) {
	if (nullptr == _pointer) {
		return;
	}
	//Keep name in lower case :
	std::string nameLower = etk::tolower(_name);
	if (true == m_creatorList.exist(nameLower)) {
		EWOL_WARNING("Replace Creator of a specify widget : " << nameLower);
		m_creatorList[nameLower] = _pointer;
		return;
	}
	EWOL_INFO("Add Creator of a specify widget : " << nameLower);
	m_creatorList.add(nameLower, _pointer);
}

ewol::WidgetShared ewol::widget::Manager::create(const std::string& _name) {
	std::string nameLower = etk::tolower(_name);
	if (m_creatorList.exist(nameLower) == true) {
		ewol::widget::Manager::creator_tf pointerFunction = m_creatorList[nameLower];
		if (pointerFunction != nullptr) {
			return pointerFunction();
		}
	}
	EWOL_WARNING("try to create an UnExistant widget : " << nameLower);
	return nullptr;
}

bool ewol::widget::Manager::exist(const std::string& _name) {
	std::string nameLower = etk::tolower(_name);
	return m_creatorList.exist(nameLower);
}

std::string ewol::widget::Manager::list() {
	std::string tmpVal;
	for (int32_t iii=0; iii<m_creatorList.size() ; iii++) {
		tmpVal += m_creatorList.getKey(iii);
		tmpVal += ",";
	}
	return tmpVal;
}

