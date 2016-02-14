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
  propertyMode(*this, "mode", modeHori, "The display mode"),
  propertyBorderSize(*this, "border", vec2(0,0), "The sizer border size"),
  propertyAnimation(*this, "annimation", animationNone, "sizer annimation"),
  propertyAnimationTime(*this, "annimation-time", 0, "time of the anniation") {
	addObjectType("ewol::widget::Sizer");
	propertyMode.add(modeHori, "hori");
	propertyMode.add(modeVert, "vert");
	propertyAnimation.add(animationNone, "none");
	propertyAnimation.add(animationTop, "top");
	propertyAnimation.add(animationbuttom, "buttom");
	propertyAnimation.add(animationLeft, "left");
	propertyAnimation.add(animationRight, "right");
}

void ewol::widget::Sizer::init(enum displayMode _mode) {
	ewol::widget::ContainerN::init();
	propertyMode.set(_mode);
}

ewol::widget::Sizer::~Sizer() {
	//EWOL_DEBUG("[" << getId() << "]={" << getObjectType() << "}  sizer : destroy (mode=" << (propertyMode == ewol::widget::Sizer::modeVert?"Vert":"Hori") << ")");
}


void ewol::widget::Sizer::onChangeSize() {
	ewol::Widget::onChangeSize();
	vec2 tmpBorderSize = propertyBorderSize->getPixel();
	EWOL_VERBOSE("[" << getId() << "] update size : " << m_size << " nbElement : " << m_subWidget.size() << " borderSize=" << tmpBorderSize << " from border=" << propertyBorderSize);
	vec2 localWidgetSize = m_size - tmpBorderSize*2.0f;
	// -1- calculate min-size and expand requested:
	vec2 minSize(0.0f, 0.0f);
	ivec2 nbWidgetExpand(0,0);
	for (auto &it : m_subWidget) {
		if (it == nullptr) {
			continue;
		}
		vec2 tmpSize = it->getCalculateMinSize();
		if (propertyMode == ewol::widget::Sizer::modeVert) {
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
		if (propertyMode == ewol::widget::Sizer::modeVert) {
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
		if (propertyMode == ewol::widget::Sizer::modeVert) {
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
			if (propertyMode == ewol::widget::Sizer::modeVert) {
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
		if (propertyMode == ewol::widget::Sizer::modeVert) {
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
		if (propertyMode == ewol::widget::Sizer::modeVert) {
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
	// -7- get under Size
	vec2 underSize(0,0);
	for (auto &it : m_subWidget) {
		if (it == nullptr) {
			continue;
		}
		vec2 size = it->getSize();
		if (propertyMode == ewol::widget::Sizer::modeVert) {
			underSize += vec2(0.0f, size.y());
			underSize.setX(std::max(underSize.x(), size.x()));
		} else {
			underSize += vec2(size.x(), 0.0f);
			underSize.setY(std::max(underSize.y(), size.y()));
		}
	}
	vec2 deltas = localWidgetSize - underSize;
	
	// -8- Calculate the local origin, depending of the gravity:
	vec2 tmpOrigin = m_origin + tmpBorderSize + ewol::gravityGenerateDelta(propertyGravity, deltas);
	// -9- Set sub widget origin:
	for (auto &it : m_subWidget) {
		if (it == nullptr) {
			continue;
		}
		vec2 origin;
		vec2 size = it->getSize();
		if (propertyMode == ewol::widget::Sizer::modeVert) {
			origin = vec2ClipInt32(tmpOrigin+m_offset + ewol::gravityGenerateDelta(propertyGravity, vec2(underSize.x()-size.x(),0.0f)));
		} else {
			origin = vec2ClipInt32(tmpOrigin+m_offset + ewol::gravityGenerateDelta(propertyGravity, vec2(0.0f, underSize.y()-size.y())));
		}
		it->setOrigin(origin);
		if (propertyMode == ewol::widget::Sizer::modeVert) {
			tmpOrigin.setY(tmpOrigin.y() + size.y());
		} else {
			tmpOrigin.setX(tmpOrigin.x() + size.x());
		}
	}
	// -10- Update all subSize at every element:
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
	m_minSize = propertyMinSize->getPixel();
	vec2 tmpBorderSize = propertyBorderSize->getPixel();
	EWOL_VERBOSE("[" << getId() << "] {" << getObjectType() << "} set min size : " <<  m_minSize);
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
		if (propertyMode == ewol::widget::Sizer::modeVert) {
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
	m_minSize += tmpBorderSize*2;
	//EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} Result min size : " <<  m_minSize);
}

int32_t ewol::widget::Sizer::subWidgetAdd(std::shared_ptr<ewol::Widget> _newWidget) {
	if (propertyAnimation == animationNone) {
		return ewol::widget::ContainerN::subWidgetAdd(_newWidget);
	}
	// TODO : ...
	return ewol::widget::ContainerN::subWidgetAdd(_newWidget);
}

int32_t ewol::widget::Sizer::subWidgetAddStart(std::shared_ptr<ewol::Widget> _newWidget) {
	if (propertyAnimation == animationNone) {
		return ewol::widget::ContainerN::subWidgetAddStart(_newWidget);
	}
	// TODO : ...
	return ewol::widget::ContainerN::subWidgetAddStart(_newWidget);
}

void ewol::widget::Sizer::subWidgetRemove(std::shared_ptr<ewol::Widget> _newWidget) {
	if (propertyAnimation == animationNone) {
		ewol::widget::ContainerN::subWidgetRemove(_newWidget);
		return;
	}
	// TODO : ...
	ewol::widget::ContainerN::subWidgetRemove(_newWidget);
}

void ewol::widget::Sizer::subWidgetUnLink(std::shared_ptr<ewol::Widget> _newWidget) {
	if (propertyAnimation == animationNone) {
		ewol::widget::ContainerN::subWidgetUnLink(_newWidget);
		return;
	}
	// TODO : ...
	ewol::widget::ContainerN::subWidgetUnLink(_newWidget);
}

void ewol::widget::Sizer::onPropertyChangeValue(const eproperty::Ref& _paramPointer) {
	ewol::widget::ContainerN::onPropertyChangeValue(_paramPointer);
	if (_paramPointer == propertyMode) {
		markToRedraw();
		requestUpdateSize();
	} else if (_paramPointer == propertyBorderSize) {
		markToRedraw();
		requestUpdateSize();
	}
}