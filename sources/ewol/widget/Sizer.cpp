/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/ewol.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/WidgetManager.h>

#undef __class__
#define __class__ "Sizer"

static ewol::Widget* create(void) {
	return new widget::Sizer();
}

void widget::Sizer::init(ewol::WidgetManager& _widgetManager) {
	_widgetManager.addWidgetCreator(__class__,&create);
}


widget::Sizer::Sizer(widget::Sizer::displayMode_te _mode):
  m_mode(_mode),
  m_borderSize(),
  m_animation(animationNone),
  m_animationTime(0) {
	
}

widget::Sizer::~Sizer(void) {
	// disable annimation to remore "remove" error
	m_animation = animationNone;
	m_animationTime = 0;
	//EWOL_DEBUG("[" << getId() << "]={" << getObjectType() << "}  sizer : destroy (mode=" << (m_mode == widget::sizer::modeVert?"Vert":"Hori") << ")");
}


void widget::Sizer::setBorderSize(const ewol::Dimension& _newBorderSize) {
	m_borderSize = _newBorderSize;
	markToRedraw();
	requestUpdateSize();
}

void widget::Sizer::setMode(widget::Sizer::displayMode_te _mode) {
	m_mode = _mode;
	markToRedraw();
	requestUpdateSize();
}

widget::Sizer::displayMode_te widget::Sizer::getMode(void) {
	return m_mode;
}

void widget::Sizer::calculateSize(const vec2& _availlable) {
	ewol::Widget::calculateSize(_availlable);
	vec2 tmpBorderSize = m_borderSize.getPixel();
	//EWOL_DEBUG("[" << getId() << "] update size : " << _availlable << " nbElement : " << m_subWidget.size() << " borderSize=" << tmpBorderSize << " from border=" << m_borderSize);
	m_size -= tmpBorderSize*2;
	// calculate unExpandable size :
	float unexpandableSize=0.0;
	int32_t nbWidgetFixedSize=0;
	int32_t nbWidgetNotFixedSize=0;
	for (int32_t iii=0; iii<m_subWidget.size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			vec2 tmpSize = m_subWidget[iii]->getCalculateMinSize();
			if (m_mode == widget::Sizer::modeVert) {
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
		if (m_mode == widget::Sizer::modeVert) {
			sizeToAddAtEveryOne = (m_size.y() - unexpandableSize) / nbWidgetNotFixedSize;
		} else {
			sizeToAddAtEveryOne = (m_size.x() - unexpandableSize) / nbWidgetNotFixedSize;
		}
		if (sizeToAddAtEveryOne<0.0) {
			sizeToAddAtEveryOne=0;
		}
	}
	vec2 tmpOrigin = m_origin + tmpBorderSize;
	for (int32_t iii=0; iii<m_subWidget.size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			vec2 tmpSize = m_subWidget[iii]->getCalculateMinSize();
			// set the origin :
			//EWOL_DEBUG("[" << getId() << "] set iii=" << iii << " ORIGIN : " << tmpOrigin << " & offset=" << m_offset);
			m_subWidget[iii]->setOrigin(vec2ClipInt32(tmpOrigin+m_offset));
			// Now update his size  his size in X and the curent sizer size in Y:
			if (m_mode == widget::Sizer::modeVert) {
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

void widget::Sizer::calculateMinMaxSize(void) {
	//EWOL_DEBUG("[" << getId() << "] update minimum size");
	m_subExpend.setValue(false, false);
	m_minSize = m_userMinSize.getPixel();
	vec2 tmpBorderSize = m_borderSize.getPixel();
	//EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} set min size : " <<  m_minSize);
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
			//EWOL_DEBUG("[" << getId() << "] NewMinSize=" << tmpSize);
			//EWOL_DEBUG("[" << getId() << "] {" << getObjectType() << "}     Get minSize[" << iii << "] "<< tmpSize);
			if (m_mode == widget::Sizer::modeVert) {
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

bool widget::Sizer::loadXML(exml::Element* _node) {
	if (NULL == _node) {
		return false;
	}
	// parse generic properties :
	widget::ContainerN::loadXML(_node);
	
	etk::UString tmpAttributeValue = _node->getAttribute("border");
	if (tmpAttributeValue.size()!=0) {
		m_borderSize = tmpAttributeValue;
	}
	tmpAttributeValue = _node->getAttribute("mode");
	if (tmpAttributeValue.size()!=0) {
		if(    tmpAttributeValue.compareNoCase("vert")
		    || tmpAttributeValue.compareNoCase("vertical")) {
			m_mode = widget::Sizer::modeVert;
		} else {
			m_mode = widget::Sizer::modeHori;
		}
	}
	return true;
}

int32_t widget::Sizer::subWidgetAdd(ewol::Widget* _newWidget) {
	if (m_animation == animationNone) {
		return widget::ContainerN::subWidgetAdd(_newWidget);
	}
	// TODO : ...
	return widget::ContainerN::subWidgetAdd(_newWidget);
}

int32_t widget::Sizer::subWidgetAddStart(ewol::Widget* _newWidget) {
	if (m_animation == animationNone) {
		return widget::ContainerN::subWidgetAddStart(_newWidget);
	}
	// TODO : ...
	return widget::ContainerN::subWidgetAddStart(_newWidget);
}

void widget::Sizer::subWidgetRemove(ewol::Widget* _newWidget) {
	if (m_animation == animationNone) {
		widget::ContainerN::subWidgetRemove(_newWidget);
		return;
	}
	// TODO : ...
	widget::ContainerN::subWidgetRemove(_newWidget);
}

void widget::Sizer::subWidgetUnLink(ewol::Widget* _newWidget) {
	if (m_animation == animationNone) {
		widget::ContainerN::subWidgetUnLink(_newWidget);
		return;
	}
	// TODO : ...
	widget::ContainerN::subWidgetUnLink(_newWidget);
}



