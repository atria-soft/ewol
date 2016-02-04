/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/ewol.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/Manager.h>

#undef __class__
#define __class__ "Sizer"

ewol::widget::Sizer::Sizer() :
  m_mode(*this, "mode", modeHori, "The display mode"),
  m_borderSize(*this, "border", vec2(0,0), "The sizer border size"),
  m_borderColor(*this, "border-color", etk::color::none, "Color of the border"),
  m_animation(animationNone),
  m_animationTime(0) {
	addObjectType("ewol::widget::Sizer");
	m_mode.add(modeHori, "hori");
	m_mode.add(modeVert, "vert");
}

void ewol::widget::Sizer::init(enum displayMode _mode) {
	ewol::widget::ContainerN::init();
	m_mode.set(_mode);
}

ewol::widget::Sizer::~Sizer() {
	// disable annimation to remore "remove" error
	m_animation = animationNone;
	m_animationTime = 0;
	//EWOL_DEBUG("[" << getId() << "]={" << getObjectType() << "}  sizer : destroy (mode=" << (m_mode == ewol::widget::Sizer::modeVert?"Vert":"Hori") << ")");
}


void ewol::widget::Sizer::onChangeSize() {
	ewol::Widget::onChangeSize();
	vec2 tmpBorderSize = m_borderSize->getPixel();
	EWOL_VERBOSE("[" << getId() << "] update size : " << m_size << " nbElement : " << m_subWidget.size() << " borderSize=" << tmpBorderSize << " from border=" << m_borderSize);
	vec2 localWidgetSize = m_size - tmpBorderSize*2.0f;
	// -1- calculate min-size and expand requested:
	vec2 minSize(0.0f, 0.0f);
	ivec2 nbWidgetExpand(0,0);
	for (auto &it : m_subWidget) {
		if (it == nullptr) {
			continue;
		}
		vec2 tmpSize = it->getCalculateMinSize();
		if (m_mode == ewol::widget::Sizer::modeVert) {
			minSize = vec2(std::max(minSize.x(), tmpSize.x()),
			               minSize.y() + tmpSize.y());
		} else {
			minSize = vec2(minSize.x() + tmpSize.x(),
			               std::max(minSize.y(), tmpSize.y()));
		}
		bvec2 expand = it->canExpand();
		nbWidgetExpand += ivec2(expand.x()==true?1:0,
		                        expand.y()==true?1:0);
	}
	// -2- Calculate the size to add at every elements...
	float deltaExpandSize = 0.0f;
	if (nbWidgetExpand != ivec2(0,0)) {
		if (m_mode == ewol::widget::Sizer::modeVert) {
			deltaExpandSize = (localWidgetSize.y() - minSize.y()) / float(nbWidgetExpand.y());
		} else {
			deltaExpandSize = (localWidgetSize.x() - minSize.x()) / float(nbWidgetExpand.x());
		}
		if (deltaExpandSize<0.0) {
			deltaExpandSize=0;
		}
	}
	// -3- Configure all at the min size ...
	for (auto &it : m_subWidget) {
		if (it == nullptr) {
			continue;
		}
		it->setSize(it->getCalculateMinSize());
	}
	// -4- For each element we apply the minmax range and update if needed
	while (deltaExpandSize > 0.0001f) {
		float residualNext = 0.0f;
		// get the number of element that need to devide...
		int32_t countCalculation = nbWidgetExpand.x();
		if (m_mode == ewol::widget::Sizer::modeVert) {
			countCalculation = nbWidgetExpand.y();
		}
		// -4.1- Update every subWidget size
		for (auto &it : m_subWidget) {
			if (it == nullptr) {
				continue;
			}
			vec2 tmpSizeMin = it->getSize();
			vec2 tmpSizeMax = it->getCalculateMaxSize();
			// Now update his size  his size in X and the curent sizer size in Y:
			if (m_mode == ewol::widget::Sizer::modeVert) {
				if (it->canExpand().y() == true) {
					float sizeExpand = tmpSizeMin.y() + deltaExpandSize;
					if (sizeExpand > tmpSizeMax.y()) {
						residualNext += (sizeExpand - tmpSizeMax.y());
						sizeExpand = tmpSizeMax.y();
						countCalculation--;
					}
					tmpSizeMin.setY(sizeExpand);
				}
				it->setSize(tmpSizeMin);
			} else {
				if (it->canExpand().x() == true) {
					float sizeExpand = tmpSizeMin.x() + deltaExpandSize;
					if (sizeExpand > tmpSizeMax.x()) {
						residualNext += (sizeExpand - tmpSizeMax.x());
						sizeExpand = tmpSizeMax.x();
						countCalculation--;
					}
					tmpSizeMin.setX(sizeExpand);
				}
				it->setSize(tmpSizeMin);
			}
		}
		// Reset size add ...
		deltaExpandSize = 0.0f;
		if (residualNext < 0.0001f) {
			break;
		}
		if (countCalculation <= 0) {
			break;
		}
		if (m_mode == ewol::widget::Sizer::modeVert) {
			deltaExpandSize = residualNext / float(countCalculation);
		} else {
			deltaExpandSize = residualNext / float(countCalculation);
		}
		if (deltaExpandSize < 0.0f) {
			deltaExpandSize = 0.0f;
			break;
		}
	}
	// -5- Update the expand in the second size if vert ==> X and if hori ==> Y
	for (auto &it : m_subWidget) {
		if (it == nullptr) {
			continue;
		}
		// Now update his size  his size in X and the curent sizer size in Y:
		if (m_mode == ewol::widget::Sizer::modeVert) {
			if (it->canExpand().x() == false) {
				continue;
			}
			vec2 tmpSizeMin = it->getSize();
			tmpSizeMin.setX(std::avg(tmpSizeMin.x(), localWidgetSize.x(), it->getCalculateMaxSize().x()));
			it->setSize(tmpSizeMin);
		} else {
			if (it->canExpand().y() == false) {
				continue;
			}
			vec2 tmpSizeMin = it->getSize();
			tmpSizeMin.setY(std::avg(tmpSizeMin.y(), localWidgetSize.y(), it->getCalculateMaxSize().y()));
			it->setSize(tmpSizeMin);
		}
	}
	// -6- Force size at the entire number:
	for (auto &it : m_subWidget) {
		if (it == nullptr) {
			continue;
		}
		it->setSize(vec2ClipInt32(it->getSize()));
	}
	// -7- Set the origin for every element with the gravity update:
	vec2 tmpOrigin = m_origin + tmpBorderSize;
	for (auto &it : m_subWidget) {
		if (it == nullptr) {
			continue;
		}
		it->setOrigin(vec2ClipInt32(tmpOrigin+m_offset));
		vec2 size = it->getSize();
		if (m_mode == ewol::widget::Sizer::modeVert) {
			tmpOrigin.setY(tmpOrigin.y() + size.y());
		} else {
			tmpOrigin.setX(tmpOrigin.x() + size.x());
		}
		// TODO : Set origin with the correct gravity
	}
	// -8- Update all subSize at every element:
	for (auto &it : m_subWidget) {
		if (it == nullptr) {
			continue;
		}
		it->onChangeSize();
	}
	markToRedraw();
}

void ewol::widget::Sizer::calculateMinMaxSize() {
	EWOL_VERBOSE("[" << getId() << "] update minimum size");
	m_subExpend.setValue(false, false);
	m_minSize = m_userMinSize->getPixel();
	vec2 tmpBorderSize = m_borderSize->getPixel();
	EWOL_VERBOSE("[" << getId() << "] {" << getObjectType() << "} set min size : " <<  m_minSize);
	m_minSize += tmpBorderSize*2;
	for (auto &it : m_subWidget) {
		if (it == nullptr) {
			continue;
		}
		it->calculateMinMaxSize();
		if (it->canExpand().x() == true) {
			m_subExpend.setX(true);
		}
		if (it->canExpand().y() == true) {
			m_subExpend.setY(true);
		}
		vec2 tmpSize = it->getCalculateMinSize();
		EWOL_VERBOSE("[" << getId() << "] NewMinSize=" << tmpSize);
		EWOL_VERBOSE("[" << getId() << "] {" << getObjectType() << "}     Get minSize="<< tmpSize);
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
	//EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} Result min size : " <<  m_minSize);
}

void ewol::widget::Sizer::onRegenerateDisplay() {
	ewol::widget::ContainerN::onRegenerateDisplay();
	m_draw.clear();
	vec2 tmpBorderSize = m_borderSize->getPixel();
	if (tmpBorderSize == vec2(0.0f, 0.0f)) {
		return;
	}
	if (m_borderColor->a() == 0) {
		return;
	}
	m_draw.setColor(m_borderColor);
	m_draw.setPos(vec3(0, 0, 0) );
	m_draw.rectangleWidth(vec3(tmpBorderSize.x(), m_size.y(),0) );
	m_draw.setPos(vec3(m_size.x() - tmpBorderSize.x(), 0, 0) );
	m_draw.rectangleWidth(vec3(tmpBorderSize.x(), m_size.y(),0) );
	m_draw.setPos(vec3(tmpBorderSize.x(), 0, 0) );
	m_draw.rectangleWidth(vec3(m_size.x()-tmpBorderSize.x()*2.0f, tmpBorderSize.y(),0) );
	m_draw.setPos(vec3(tmpBorderSize.x(), m_size.y()-tmpBorderSize.y(), 0) );
	m_draw.rectangleWidth(vec3(m_size.x()-tmpBorderSize.x()*2.0f, tmpBorderSize.y(),0) );
	for (auto &it : m_subWidget) {
		if (it == nullptr) {
			continue;
		}
		vec2 deltaOrigin = it->getOrigin() - (m_origin );
		vec2 size = it->getSize();
		// now we display around the widget every element needed
		if (m_mode == ewol::widget::Sizer::modeHori) {
			// under
			
			// upper
			if (size.y() < m_size.y()-tmpBorderSize.y()*2.0f) {
				m_draw.setColor(etk::color::orange);
				m_draw.setPos(deltaOrigin + vec2(0, it->getSize().y()) );
				m_draw.rectangleWidth(vec2(it->getSize().x(), m_size.y()-tmpBorderSize.y()*2.0f-it->getSize().y()) );
			}
		} else {
			// left
			
			// right
			if (size.x() < m_size.x()-tmpBorderSize.x()*2.0f) {
				m_draw.setColor(etk::color::orange);
				m_draw.setPos(deltaOrigin + vec2(it->getSize().x(), 0) );
				m_draw.rectangleWidth(vec2(m_size.x()-tmpBorderSize.x()*2.0f-it->getSize().x(), it->getSize().y()) );
			}
		}
	}
}
void ewol::widget::Sizer::onDraw() {
	m_draw.draw();
	ewol::widget::ContainerN::onDraw();
}

int32_t ewol::widget::Sizer::subWidgetAdd(std::shared_ptr<ewol::Widget> _newWidget) {
	if (m_animation == animationNone) {
		return ewol::widget::ContainerN::subWidgetAdd(_newWidget);
	}
	// TODO : ...
	return ewol::widget::ContainerN::subWidgetAdd(_newWidget);
}

int32_t ewol::widget::Sizer::subWidgetAddStart(std::shared_ptr<ewol::Widget> _newWidget) {
	if (m_animation == animationNone) {
		return ewol::widget::ContainerN::subWidgetAddStart(_newWidget);
	}
	// TODO : ...
	return ewol::widget::ContainerN::subWidgetAddStart(_newWidget);
}

void ewol::widget::Sizer::subWidgetRemove(std::shared_ptr<ewol::Widget> _newWidget) {
	if (m_animation == animationNone) {
		ewol::widget::ContainerN::subWidgetRemove(_newWidget);
		return;
	}
	// TODO : ...
	ewol::widget::ContainerN::subWidgetRemove(_newWidget);
}

void ewol::widget::Sizer::subWidgetUnLink(std::shared_ptr<ewol::Widget> _newWidget) {
	if (m_animation == animationNone) {
		ewol::widget::ContainerN::subWidgetUnLink(_newWidget);
		return;
	}
	// TODO : ...
	ewol::widget::ContainerN::subWidgetUnLink(_newWidget);
}

void ewol::widget::Sizer::onParameterChangeValue(const ewol::parameter::Ref& _paramPointer) {
	ewol::widget::ContainerN::onParameterChangeValue(_paramPointer);
	if (_paramPointer == m_mode) {
		markToRedraw();
		requestUpdateSize();
	} else if (_paramPointer == m_borderSize) {
		markToRedraw();
		requestUpdateSize();
	}
}