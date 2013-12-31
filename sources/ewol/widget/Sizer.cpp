/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/ewol.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/Manager.h>

const char* const ewol::widget::Sizer::configBorder = "border";
const char* const ewol::widget::Sizer::configMode = "mode";

#undef __class__
#define __class__ "Sizer"

static ewol::Widget* create(void) {
	return new ewol::widget::Sizer();
}

void ewol::widget::Sizer::init(ewol::widget::Manager& _widgetManager) {
	_widgetManager.addWidgetCreator(__class__,&create);
}


ewol::widget::Sizer::Sizer(enum displayMode _mode):
  m_mode(_mode),
  m_borderSize(),
  m_animation(animationNone),
  m_animationTime(0) {
	addObjectType("ewol::widget::Sizer");
	registerConfig(configBorder, "dimension", NULL, "The sizer border size");
	registerConfig(configMode, "list", "{vert,hori}", "The display mode");
	
}

ewol::widget::Sizer::~Sizer(void) {
	// disable annimation to remore "remove" error
	m_animation = animationNone;
	m_animationTime = 0;
	//EWOL_DEBUG("[" << getId() << "]={" << getObjectType() << "}  sizer : destroy (mode=" << (m_mode == ewol::widget::Sizer::modeVert?"Vert":"Hori") << ")");
}


void ewol::widget::Sizer::setBorderSize(const ewol::Dimension& _newBorderSize) {
	m_borderSize = _newBorderSize;
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::Sizer::setMode(enum displayMode _mode) {
	m_mode = _mode;
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::Sizer::calculateSize(const vec2& _availlable) {
	ewol::Widget::calculateSize(_availlable);
	vec2 tmpBorderSize = m_borderSize.getPixel();
	EWOL_VERBOSE("[" << getId() << "] update size : " << _availlable << " nbElement : " << m_subWidget.size() << " borderSize=" << tmpBorderSize << " from border=" << m_borderSize);
	m_size -= tmpBorderSize*2;
	// calculate unExpandable size :
	float unexpandableSize=0.0;
	int32_t nbWidgetFixedSize=0;
	int32_t nbWidgetNotFixedSize=0;
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			vec2 tmpSize = m_subWidget[iii]->getCalculateMinSize();
			if (m_mode == ewol::widget::Sizer::modeVert) {
				unexpandableSize += tmpSize.y();
				if (false == m_subWidget[iii]->canExpand().y()) {
					nbWidgetFixedSize++;
				} else {
					nbWidgetNotFixedSize++;
				}
			} else {
				unexpandableSize += tmpSize.x();
				if (false == m_subWidget[iii]->canExpand().x()) {
					nbWidgetFixedSize++;
				} else {
					nbWidgetNotFixedSize++;
				}
			}
		}
	}
	// 2 cases : 1 or more can Expand, or all is done ...
	float sizeToAddAtEveryOne = 0;
	// 2 cases : 1 or more can Expand, or all is done ...
	if (0 != nbWidgetNotFixedSize) {
		if (m_mode == ewol::widget::Sizer::modeVert) {
			sizeToAddAtEveryOne = (m_size.y() - unexpandableSize) / nbWidgetNotFixedSize;
		} else {
			sizeToAddAtEveryOne = (m_size.x() - unexpandableSize) / nbWidgetNotFixedSize;
		}
		if (sizeToAddAtEveryOne<0.0) {
			sizeToAddAtEveryOne=0;
		}
	}
	vec2 tmpOrigin = m_origin + tmpBorderSize;
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			vec2 tmpSize = m_subWidget[iii]->getCalculateMinSize();
			// set the origin :
			EWOL_VERBOSE("[" << getId() << "] set iii=" << iii << " ORIGIN : " << tmpOrigin << " & offset=" << m_offset);
			m_subWidget[iii]->setOrigin(vec2ClipInt32(tmpOrigin+m_offset));
			// Now update his size  his size in X and the curent sizer size in Y:
			if (m_mode == ewol::widget::Sizer::modeVert) {
				if (true == m_subWidget[iii]->canExpand().y()) {
					m_subWidget[iii]->calculateSize(vec2ClipInt32(vec2(m_size.x(), tmpSize.y()+sizeToAddAtEveryOne)));
					tmpOrigin.setY(tmpOrigin.y() + tmpSize.y()+sizeToAddAtEveryOne);
				} else {
					m_subWidget[iii]->calculateSize(vec2ClipInt32(vec2(m_size.x(), tmpSize.y())));
					tmpOrigin.setY(tmpOrigin.y() + tmpSize.y());
				}
			} else {
				if (true == m_subWidget[iii]->canExpand().x()) {
					m_subWidget[iii]->calculateSize(vec2ClipInt32(vec2(tmpSize.x()+sizeToAddAtEveryOne, m_size.y())));
					tmpOrigin.setX(tmpOrigin.x() + tmpSize.x()+sizeToAddAtEveryOne);
				} else {
					m_subWidget[iii]->calculateSize(vec2ClipInt32(vec2(tmpSize.x(), m_size.y())));
					tmpOrigin.setX(tmpOrigin.x() + tmpSize.x());
				}
			}
		}
	}
	m_size += tmpBorderSize*2;
	markToRedraw();
}

void ewol::widget::Sizer::calculateMinMaxSize(void) {
	EWOL_VERBOSE("[" << getId() << "] update minimum size");
	m_subExpend.setValue(false, false);
	m_minSize = m_userMinSize.getPixel();
	vec2 tmpBorderSize = m_borderSize.getPixel();
	EWOL_VERBOSE("[" << getId() << "] {" << getObjectType() << "} set min size : " <<  m_minSize);
	m_minSize += tmpBorderSize*2;
	for (int32_t iii=0; iii<m_subWidget.size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->calculateMinMaxSize();
			if (true == m_subWidget[iii]->canExpand().x()) {
				m_subExpend.setX(true);
			}
			if (true == m_subWidget[iii]->canExpand().y()) {
				m_subExpend.setY(true);
			}
			vec2 tmpSize = m_subWidget[iii]->getCalculateMinSize();
			EWOL_VERBOSE("[" << getId() << "] NewMinSize=" << tmpSize);
			EWOL_VERBOSE("[" << getId() << "] {" << getObjectType() << "}     Get minSize[" << iii << "] "<< tmpSize);
			if (m_mode == ewol::widget::Sizer::modeVert) {
				m_minSize.setY(m_minSize.y() + tmpSize.y());
				if (tmpSize.x()>m_minSize.x()) {
					m_minSize.setX(tmpSize.x());
				}
			} else {
				m_minSize.setX(m_minSize.x() + tmpSize.x());
				if (tmpSize.y()>m_minSize.y()) {
					m_minSize.setY(tmpSize.y());
				}
			}
		}
	}
	//EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} Result min size : " <<  m_minSize);
}

int32_t ewol::widget::Sizer::subWidgetAdd(ewol::Widget* _newWidget) {
	if (m_animation == animationNone) {
		return ewol::widget::ContainerN::subWidgetAdd(_newWidget);
	}
	// TODO : ...
	return ewol::widget::ContainerN::subWidgetAdd(_newWidget);
}

int32_t ewol::widget::Sizer::subWidgetAddStart(ewol::Widget* _newWidget) {
	if (m_animation == animationNone) {
		return ewol::widget::ContainerN::subWidgetAddStart(_newWidget);
	}
	// TODO : ...
	return ewol::widget::ContainerN::subWidgetAddStart(_newWidget);
}

void ewol::widget::Sizer::subWidgetRemove(ewol::Widget* _newWidget) {
	if (m_animation == animationNone) {
		ewol::widget::ContainerN::subWidgetRemove(_newWidget);
		return;
	}
	// TODO : ...
	ewol::widget::ContainerN::subWidgetRemove(_newWidget);
}

void ewol::widget::Sizer::subWidgetUnLink(ewol::Widget* _newWidget) {
	if (m_animation == animationNone) {
		ewol::widget::ContainerN::subWidgetUnLink(_newWidget);
		return;
	}
	// TODO : ...
	ewol::widget::ContainerN::subWidgetUnLink(_newWidget);
}

bool ewol::widget::Sizer::onSetConfig(const ewol::object::Config& _conf) {
	if (true == ewol::widget::ContainerN::onSetConfig(_conf)) {
		return true;
	}
	if (_conf.getConfig() == configBorder) {
		setBorderSize(_conf.getData());
		return true;
	}
	if (_conf.getConfig() == configMode) {
		if (_conf.getData() == "vert") {
			setMode(modeVert);
		} else {
			setMode(modeHori);
		}
		return true;
	}
	return false;
}

bool ewol::widget::Sizer::onGetConfig(const char* _config, std::string& _result) const {
	if (true == ewol::widget::ContainerN::onGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configBorder) {
		_result = (std::string)getBorderSize();
		return true;
	}
	if (_config == configMode) {
		if (getMode() == modeVert) {
			_result = "vert";
		} else {
			_result = "hori";
		}
		return true;
	}
	return false;
}


