/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/PopUp.h>
#include <ewol/widget/Manager.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/Manager.h>
#include <ewol/ewol.h>

#undef __class__
#define __class__ "PopUp"

const char* const ewol::widget::PopUp::configShaper="shaper";
const char* const ewol::widget::PopUp::configRemoveOnExternClick="out-click-remove";
const char* const ewol::widget::PopUp::configAnimation="animation";
const char* const ewol::widget::PopUp::configLockExpand="lock";

static const char* annimationIncrease = "increase";

ewol::widget::PopUp::PopUp() :
  m_lockExpand(true,true),
  m_closeOutEvent(false) {
	addObjectType("ewol::widget::PopUp");
	m_userExpand.setValue(false, false);
	setMinSize(ewol::Dimension(vec2(80,80),ewol::Dimension::Pourcent));
	registerConfig(configShaper, "string", nullptr, "The shaper properties");
	registerConfig(configRemoveOnExternClick, "bool", nullptr, "Remove the widget if the use click outside");
	registerConfig(configLockExpand, "bool", nullptr, "Lock expand contamination");
	registerConfig(configAnimation, "list", "none;increase", "Annimation type");
	
	// Add annimations :
	addAnnimationType(ewol::Widget::annimationModeEnableAdd, annimationIncrease);
}

void ewol::widget::PopUp::init(const std::string& _shaperName) {
	ewol::widget::Container::init();
	m_shaper.setSource(_shaperName);
}
ewol::widget::PopUp::~PopUp() {
	
}

void ewol::widget::PopUp::lockExpand(const bvec2& _lockExpand) {
	if (_lockExpand != m_lockExpand) {
		m_lockExpand = _lockExpand;
		markToRedraw();
		requestUpdateSize();
	}
}

void ewol::widget::PopUp::setShaperName(const std::string& _shaperName) {
	m_shaper.setSource(_shaperName);
	markToRedraw();
}

void ewol::widget::PopUp::calculateSize(const vec2& _available) {
	ewol::Widget::calculateSize(_available);
	if (nullptr != m_subWidget) {
		ewol::Padding padding = m_shaper.getPadding();
		vec2 subWidgetSize = m_subWidget->getCalculateMinSize();
		if (true == m_subWidget->canExpand().x()) {
			if (m_lockExpand.x() == true) {
				subWidgetSize.setX(m_minSize.x());
			} else {
				subWidgetSize.setX(m_size.x()-padding.xLeft());
			}
		}
		if (true == m_subWidget->canExpand().y()) {
			if (m_lockExpand.y() == true) {
				subWidgetSize.setY(m_minSize.y());
			} else {
				subWidgetSize.setY(m_size.y()-padding.yButtom());
			}
		}
		// limit the size of the element :
		//subWidgetSize.setMin(m_minSize);
		// posiition at a int32_t pos :
		subWidgetSize = vec2ClipInt32(subWidgetSize);
		
		// set config to the Sub-widget
		vec2 subWidgetOrigin = m_origin + (m_size-subWidgetSize)/2.0f;
		subWidgetOrigin = vec2ClipInt32(subWidgetOrigin);
		
		m_subWidget->setOrigin(subWidgetOrigin);
		m_subWidget->calculateSize(subWidgetSize);
	}
	markToRedraw();
}

void ewol::widget::PopUp::systemDraw(const ewol::DrawProperty& _displayProp) {
	if (true == m_hide){
		// widget is hidden ...
		return;
	}
	ewol::Widget::systemDraw(_displayProp);
	if (nullptr!=m_subWidget) {
		if(    m_shaper.getNextDisplayedStatus() == -1
		    && m_shaper.getTransitionStatus() >= 1.0) {
			ewol::DrawProperty prop = _displayProp;
			prop.limit(m_origin, m_size);
			m_subWidget->systemDraw(prop);
		}
	}
}

void ewol::widget::PopUp::onDraw() {
	m_shaper.draw();
}

void ewol::widget::PopUp::onRegenerateDisplay() {
	if (true == needRedraw()) {
		m_shaper.clear();
		ewol::Padding padding = m_shaper.getPadding();
		vec2 tmpSize(0,0);
		bvec2 expand = canExpand();
		bvec2 fill = canFill();
		if (fill.x()) {
			tmpSize.setX(m_size.x()-padding.x());
		}
		if (fill.y()) {
			tmpSize.setY(m_size.y()-padding.y());
		}
		if (nullptr != m_subWidget) {
			vec2 tmpSize = m_subWidget->getSize();
		}
		tmpSize.setMax(m_minSize);
		vec2 tmpOrigin = (m_size-tmpSize)/2.0f;
		
		m_shaper.setShape(vec2(0,0),
		                  vec2ClipInt32(m_size),
		                  vec2ClipInt32(tmpOrigin-vec2(padding.xLeft(), padding.yButtom())),
		                  vec2ClipInt32(tmpSize + vec2(padding.x(), padding.y())));
	}
	// SUBwIDGET GENERATION ...
	if (nullptr != m_subWidget) {
		m_subWidget->onRegenerateDisplay();
	}
}

std::shared_ptr<ewol::Widget> ewol::widget::PopUp::getWidgetAtPos(const vec2& _pos) {
	std::shared_ptr<ewol::Widget> val = ewol::widget::Container::getWidgetAtPos(_pos);
	if (nullptr != val) {
		return val;
	}
	return std::dynamic_pointer_cast<ewol::Widget>(shared_from_this());
}

bool ewol::widget::PopUp::onSetConfig(const ewol::object::Config& _conf) {
	if (true == ewol::widget::Container::onSetConfig(_conf)) {
		return true;
	}
	if (_conf.getConfig() == configShaper) {
		setShaperName(_conf.getData());
		return true;
	}
	if (_conf.getConfig() == configRemoveOnExternClick) {
		setRemoveOnExternClick(std::stob(_conf.getData()));
		return true;
	}
	if (_conf.getConfig() == configLockExpand) {
		lockExpand(_conf.getData());
		return true;
	}
	return false;
}

bool ewol::widget::PopUp::onGetConfig(const char* _config, std::string& _result) const {
	if (true == ewol::widget::Container::onGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configShaper) {
		_result = m_shaper.getSource();
		return true;
	}
	if (_config == configLockExpand) {
		_result = m_lockExpand;
		return true;
	}
	if (_config == configRemoveOnExternClick) {
		_result = std::to_string(getRemoveOnExternClick());
		return true;
	}
	return false;
}

bool ewol::widget::PopUp::onEventInput(const ewol::event::Input& _event) {
	if (0 != _event.getId()) {
		if (true == m_closeOutEvent) {
			ewol::Padding padding = m_shaper.getPadding();
			vec2 tmpSize(0,0);
			if (nullptr != m_subWidget) {
				vec2 tmpSize = m_subWidget->getSize();
			}
			tmpSize.setMax(m_minSize);
			vec2 tmpOrigin = (m_size-tmpSize)/2.0f;
			
			tmpOrigin -= vec2(padding.xLeft(), padding.yButtom());
			tmpSize += vec2(padding.x(), padding.y());
			vec2 pos = relativePosition(_event.getPos());
			if(    pos.x() < tmpOrigin.x()
			    || pos.y() < tmpOrigin.y()
			    || pos.x() > tmpOrigin.x()+tmpSize.x()
			    || pos.y() > tmpOrigin.y()+tmpSize.y() ) {
				autoDestroy();
				return true;
			}
		}
	}
	return false;
}


bool ewol::widget::PopUp::onStartAnnimation(enum ewol::Widget::annimationMode _mode) {
	if (m_annimationType[_mode] != annimationIncrease) {
		return false;
	}
	if (true == m_shaper.changeStatusIn(1) ) {
		periodicCallEnable();
		return true;
	}
	return false;
}

void ewol::widget::PopUp::onStopAnnimation() {
	periodicCallDisable();
}

void ewol::widget::PopUp::periodicCall(const ewol::event::Time& _event) {
	if (false == m_shaper.periodicCall(_event) ) {
		stopAnnimation();
	}
	markToRedraw();
}

