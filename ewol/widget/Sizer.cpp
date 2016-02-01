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


void ewol::widget::Sizer::calculateSize(const vec2& _availlable) {
	ewol::Widget::calculateSize(_availlable);
	vec2 tmpBorderSize = m_borderSize->getPixel();
	EWOL_VERBOSE("[" << getId() << "] update size : " << _availlable << " nbElement : " << m_subWidget.size() << " borderSize=" << tmpBorderSize << " from border=" << m_borderSize);
	#if 1
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
				               minSize.x() + tmpSize.y());
			} else {
				minSize = vec2(minSize.y() + tmpSize.y(),
				               std::max(minSize.y(), tmpSize.y()));
			}
			bvec2 expand = it->canExpand();
			nbWidgetExpand += ivec(expand.x()==true?1:0,
			                       expand.y()==true?1:0);
		}
		// -2- Calculate the size to add at every elements...
		float deltaExpandSize = 0.0f;
		if (nbWidgetNotFixedSize != ivec2(0,0)) {
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
			it->ParrentSetSize(it->getCalculateMinSize());
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
					if (it->canExpand().x() == true) {
						float sizeExpand = etk::avg(tmpSizeMin.x(), minSize.x(), tmpSizeMax.x());
						tmpSizeMin.setX(sizeExpand);
					}
					it->parrentSetSize(tmpSizeMin);
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
					if (it->canExpand().y() == true) {
						float sizeExpand = etk::avg(tmpSizeMin.y(), minSize.y(), tmpSizeMax.y());
						tmpSizeMin.setY(sizeExpand);
					}
					it->parrentSetSize(tmpSizeMin);
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
			if (deltaExpandSize<0.0f) {
				deltaExpandSize=0.0f;
				break;
			}
		}
		// -5- Set the origin for every element with the gravity update:
		for (auto &it : m_subWidget) {
			if (it == nullptr) {
				continue;
			}
			// TODO : Set origin with the correct gravity
			
		}
	#else
		m_size -= tmpBorderSize*2.0f;
		// calculate unExpandable size :
		float unexpandableSize=0.0;
		ivec2 nbWidgetNotFixedSize = ivec2(0,0);
		for (auto &it : m_subWidget) {
			if (it == nullptr) {
				continue;
			}
			vec2 tmpSize = it->getCalculateMinSize();
			if (m_mode == ewol::widget::Sizer::modeVert) {
				unexpandableSize += tmpSize.y();
				if (it->canExpand().y() == true) {
					nbWidgetNotFixedSize+=ivec2(0,1);
				}
			} else {
				unexpandableSize += tmpSize.x();
				if (it->canExpand().x() == true) {
					nbWidgetNotFixedSize+=ivec2(1,0);
				}
			}
		}
		// 2 cases : 1 or more can Expand, or all is done ...
		float sizeToAddAtEveryOne = 0;
		// 2 cases : 1 or more can Expand, or all is done ...
		if (nbWidgetNotFixedSize != ivec2(0,0)) {
			if (m_mode == ewol::widget::Sizer::modeVert) {
				sizeToAddAtEveryOne = (m_size.y() - unexpandableSize) / float(nbWidgetNotFixedSize.y());
			} else {
				sizeToAddAtEveryOne = (m_size.x() - unexpandableSize) / float(nbWidgetNotFixedSize.x());
			}
			if (sizeToAddAtEveryOne<0.0) {
				sizeToAddAtEveryOne=0;
			}
		}
		// TODO : Need manage gravity ...
		vec2 tmpOrigin = m_origin + tmpBorderSize;
		for (auto &it : m_subWidget) {
			if (it != nullptr) {
				vec2 tmpSize = it->getCalculateMinSize();
				// set the origin :
				EWOL_VERBOSE("[" << getId() << "] set ORIGIN : " << tmpOrigin << " & offset=" << m_offset);
				it->setOrigin(vec2ClipInt32(tmpOrigin+m_offset));
				// Now update his size  his size in X and the curent sizer size in Y:
				if (m_mode == ewol::widget::Sizer::modeVert) {
					if (it->canExpand().y() == true) {
						vec2 expectedSize = vec2ClipInt32(vec2(m_size.x(), tmpSize.y()+sizeToAddAtEveryOne));
						it->calculateSize(expectedSize);
						vec2 underSize = it->getSize();
						if (it->canExpand().x() == true) {
							if (underSize.x() < expectedSize.x()) {
								EWOL_WARNING("Subwidget request exapnd and does not expand ... ==> rules impose it ...");
								//it->setSize(vec2(expectedSize.x(), underSize.y());
								//underSize = it->getSize();
							}
						}
						if (it->canExpand().y() == true) {
							if (underSize.y() < expectedSize.y()) {
								EWOL_WARNING("Subwidget request exapnd and does not expand ... ==> rules impose it ...");
								//it->setSize(vec2(underSize.y(), expectedSize.x());
							}
						}
						tmpOrigin.setY(tmpOrigin.y() + tmpSize.y()+sizeToAddAtEveryOne);
					} else {
						it->calculateSize(vec2ClipInt32(vec2(m_size.x(), tmpSize.y())));
						tmpOrigin.setY(tmpOrigin.y() + tmpSize.y());
					}
				} else {
					if (it->canExpand().x() == true) {
						it->calculateSize(vec2ClipInt32(vec2(tmpSize.x()+sizeToAddAtEveryOne, m_size.y())));
						tmpOrigin.setX(tmpOrigin.x() + tmpSize.x()+sizeToAddAtEveryOne);
					} else {
						it->calculateSize(vec2ClipInt32(vec2(tmpSize.x(), m_size.y())));
						
						int32_t subSize = it->getSize().y();
						if (subSize <= m_size.y()) {
							// move localy of half needed
							it->setOrigin(it->getOrigin() + ivec2(0, (m_size.y()-subSize)/2));
						}
						tmpOrigin.setX(tmpOrigin.x() + tmpSize.x());
					}
				}
			}
		}
		m_size += tmpBorderSize*2;
	#endif
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