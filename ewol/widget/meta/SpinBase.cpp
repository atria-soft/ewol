/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/meta/SpinBase.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Composer.h>

void ewol::widget::SpinBase::init(ewol::widget::Manager& _widgetManager) {
	
}

ewol::widget::SpinBase::SpinBase() :
  m_confIdEntryShaper(-1),
  m_confIdUpShaper(-1),
  m_confIdDownShaper(-1),
  m_confIdUpData(-1),
  m_confIdDownData(-1),
  m_spinMode(*this, "mode", ewol::widget::spinPosition_RightRight, "The display spin mode") {
	addObjectType("ewol::widget::SpinBase");
	m_spinMode.add(ewol::widget::spinPosition_noneNone, "none-none");
	m_spinMode.add(ewol::widget::spinPosition_noneRight, "none-right");
	m_spinMode.add(ewol::widget::spinPosition_leftNone, "left-none");
	m_spinMode.add(ewol::widget::spinPosition_leftRight, "left-right");
	m_spinMode.add(ewol::widget::spinPosition_leftLeft, "left-left");
	m_spinMode.add(ewol::widget::spinPosition_RightRight, "right-right");
}

void ewol::widget::SpinBase::init(enum ewol::widget::spinPosition _mode,
                                  const std::string& _shaperName) {
	ewol::widget::Sizer::init(ewol::widget::Sizer::modeHori);
	lockExpand(bvec2(true,true));	
	m_config = ewol::resource::ConfigFile::create(_shaperName);
	if (m_config != nullptr) {
		m_confIdEntryShaper = m_config->request("entry-shaper");
		m_confIdUpShaper = m_config->request("up-shaper");
		m_confIdDownShaper = m_config->request("down-shaper");
		m_confIdUpData = m_config->request("up-data");
		m_confIdDownData = m_config->request("down-data");
	}
	m_spinMode = _mode;
	setGravity(gravity_center);
	updateGui();
}

ewol::widget::SpinBase::~SpinBase() {
	
}

void ewol::widget::SpinBase::onParameterChangeValue(const ewol::parameter::Ref& _paramPointer) {
	ewol::widget::Sizer::onParameterChangeValue(_paramPointer);
	if (_paramPointer == m_spinMode) {
		updateGui();
	}
}



void ewol::widget::SpinBase::updateGui() {
	subWidgetRemoveAll();
	markToRedraw();
	requestUpdateSize();
	if (m_widgetEntry == nullptr) {
		std::string shaper;
		if (m_config != nullptr) {
			shaper = m_config->getString(m_confIdEntryShaper);
		}
		m_widgetEntry = ewol::widget::Entry::create("", shaper);
		if (m_widgetEntry != nullptr) {
			m_widgetEntry->setExpand(bvec2(true,false));
			m_widgetEntry->setFill(bvec2(true,true));
		}
	}
	if (m_widgetButtonDown == nullptr) {
		std::string shaper;
		if (m_config != nullptr) {
			shaper = m_config->getString(m_confIdDownShaper);
		}
		m_widgetButtonDown = ewol::widget::Button::create(shaper);
		if (m_widgetButtonDown != nullptr) {
			m_widgetButtonDown->setExpand(bvec2(false,false));
			m_widgetButtonDown->setFill(bvec2(true,true));
			std::string data = m_config->getString(m_confIdDownData);
			std::shared_ptr<ewol::Widget> widget = ewol::widget::composerGenerate(ewol::widget::Composer::String, data);
			m_widgetButtonDown->setSubWidget(widget);
		}
	}
	if (m_widgetButtonUp == nullptr) {
		std::string shaper;
		if (m_config != nullptr) {
			shaper = m_config->getString(m_confIdUpShaper);
		}
		m_widgetButtonUp = ewol::widget::Button::create(shaper);
		if (m_widgetButtonUp != nullptr) {
			m_widgetButtonUp->setExpand(bvec2(false,false));
			m_widgetButtonUp->setFill(bvec2(true,true));
			std::string data = m_config->getString(m_confIdUpData);
			std::shared_ptr<ewol::Widget> widget = ewol::widget::composerGenerate(ewol::widget::Composer::String, data);
			m_widgetButtonUp->setSubWidget(widget);
		}
	}
	switch (m_spinMode) {
		case ewol::widget::spinPosition_noneNone:
			subWidgetAdd(m_widgetEntry);
			break;
		case ewol::widget::spinPosition_noneRight:
			subWidgetAdd(m_widgetEntry);
			subWidgetAdd(m_widgetButtonUp);
			break;
		case ewol::widget::spinPosition_leftNone:
			subWidgetAdd(m_widgetButtonDown);
			subWidgetAdd(m_widgetEntry);
			break;
		case ewol::widget::spinPosition_leftRight:
			subWidgetAdd(m_widgetButtonDown);
			subWidgetAdd(m_widgetEntry);
			subWidgetAdd(m_widgetButtonUp);
			break;
		case ewol::widget::spinPosition_leftLeft:
			subWidgetAdd(m_widgetButtonDown);
			subWidgetAdd(m_widgetButtonUp);
			subWidgetAdd(m_widgetEntry);
			break;
		case ewol::widget::spinPosition_RightRight:
			subWidgetAdd(m_widgetEntry);
			subWidgetAdd(m_widgetButtonDown);
			subWidgetAdd(m_widgetButtonUp);
			break;
	}
}

