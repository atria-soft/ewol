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
                      &ewol::widget::SpinBase::onChangePropertyShape),
  propertySpinMode(this, "spin-mode",
                         ewol::widget::spinPosition_RightRight,
                         "The display spin mode",
                         &ewol::widget::SpinBase::onChangePropertySpinMode),
  m_confIdEntryShaper(-1),
  m_confIdUpShaper(-1),
  m_confIdDownShaper(-1),
  m_confIdUpData(-1),
  m_confIdDownData(-1) {
	
	addObjectType("ewol::widget::SpinBase");
	propertySpinMode.add(ewol::widget::spinPosition_noneNone, "none-none");
	propertySpinMode.add(ewol::widget::spinPosition_noneRight, "none-right");
	propertySpinMode.add(ewol::widget::spinPosition_leftNone, "left-none");
	propertySpinMode.add(ewol::widget::spinPosition_leftRight, "left-right");
	propertySpinMode.add(ewol::widget::spinPosition_leftLeft, "left-left");
	propertySpinMode.add(ewol::widget::spinPosition_RightRight, "right-right");
	propertyLockExpand.setDirectCheck(bvec2(true,true));
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
	m_config = ewol::resource::ConfigFile::create(propertyShape);
	if (m_config != null) {
		m_confIdEntryShaper = m_config->request("entry-shaper");
		m_confIdUpShaper = m_config->request("up-shaper");
		m_confIdDownShaper = m_config->request("down-shaper");
		m_confIdUpData = m_config->request("up-data");
		m_confIdDownData = m_config->request("down-data");
	}
	markToRedraw();
}



void ewol::widget::SpinBase::updateGui() {
	subWidgetRemoveAll();
	markToRedraw();
	requestUpdateSize();
	if (m_widgetEntry == null) {
		etk::String shaper;
		if (m_config != null) {
			shaper = m_config->getString(m_confIdEntryShaper);
			EWOL_VERBOSE("shaper entry : " << shaper);
		}
		m_widgetEntry = ewol::widget::Entry::create("shape", shaper);
		if (m_widgetEntry != null) {
			m_widgetEntry->propertyExpand.set(bvec2(true,false));
			m_widgetEntry->propertyFill.set(bvec2(true,true));
		}
	}
	if (m_widgetButtonDown == null) {
		etk::String shaper;
		if (m_config != null) {
			shaper = m_config->getString(m_confIdDownShaper);
			EWOL_VERBOSE("shaper button DOWN : " << shaper);
		}
		m_widgetButtonDown = ewol::widget::Button::create("shape", shaper);
		if (m_widgetButtonDown != null) {
			m_widgetButtonDown->propertyExpand.set(bvec2(false,false));
			m_widgetButtonDown->propertyFill.set(bvec2(true,true));
			etk::String data = m_config->getString(m_confIdDownData);
			ewol::WidgetShared widget = ewol::widget::composerGenerateString(data);
			m_widgetButtonDown->setSubWidget(widget);
		}
	}
	if (m_widgetButtonUp == null) {
		etk::String shaper;
		if (m_config != null) {
			shaper = m_config->getString(m_confIdUpShaper);
			EWOL_VERBOSE("shaper button UP : " << shaper);
		}
		m_widgetButtonUp = ewol::widget::Button::create("shape", shaper);
		if (m_widgetButtonUp != null) {
			m_widgetButtonUp->propertyExpand.set(bvec2(false,false));
			m_widgetButtonUp->propertyFill.set(bvec2(true,true));
			etk::String data = m_config->getString(m_confIdUpData);
			ewol::WidgetShared widget = ewol::widget::composerGenerateString(data);
			m_widgetButtonUp->setSubWidget(widget);
		}
	}
	switch (propertySpinMode) {
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

bool ewol::widget::SpinBase::loadXML(const exml::Element& _node) {
	if (_node.exist() == false) {
		return false;
	}
	// parse generic properties: (we not parse the sizer property, it remove all subwidget)
	return ewol::Widget::loadXML(_node);
}