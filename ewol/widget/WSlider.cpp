/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/widget/WSlider.h>
#include <ewol/ewol.h>


static const char* l_listsladingMode[ewol::widget::WSlider::sladingTransition_count] = {
	"transition vertical",
	"transition horisantal"
};

std::ostream& operator <<(std::ostream& _os, const enum ewol::widget::WSlider::sladingMode _obj) {
	_os << l_listsladingMode[_obj];
	return _os;
}

ewol::widget::WSlider::WSlider() :
  signalStartSlide(this, "start", ""),
  signalStopSlide(this, "stop", ""),
  propertyTransitionSpeed(this, "speed",
                                1.0f, 0.0f, 200.0f,
                                "Transition speed of the slider"),
  propertyTransitionMode(this, "mode",
                               sladingTransitionHori,
                               "Transition mode of the slider",
                               &ewol::widget::WSlider::onChangePropertyTransitionMode),
  propertySelectWidget(this, "select",
                             "",
                             "Select the requested widget to display",
                             &ewol::widget::WSlider::onChangePropertySelectWidget),
  m_windowsSources(0),
  m_windowsDestination(0),
  m_windowsRequested(-1),
  m_slidingProgress(1.0f) {
	addObjectType("ewol::widget::WSlider");
	propertyTransitionMode.add(sladingTransitionVert, "vert");
	propertyTransitionMode.add(sladingTransitionHori, "hori");
}

ewol::widget::WSlider::~WSlider() {
	
}


void ewol::widget::WSlider::onChangeSize() {
	ewol::widget::ContainerN::onChangeSize();
	if (m_windowsDestination == m_windowsSources) {
		auto it = m_subWidget.begin();
		std::advance(it, m_windowsDestination);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			(*it)->setOrigin(m_origin+m_offset);
			(*it)->setSize(m_size);
			(*it)->onChangeSize();
		}
	} else {
		float factor = -1.0f;
		if (m_windowsSources < m_windowsDestination) {
			factor = 1.0f;
		}
		auto it = m_subWidget.begin();
		std::advance(it, m_windowsSources);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
		     if (*propertyTransitionMode == sladingTransitionHori) {
				(*it)->setOrigin(   vec2(m_origin.x() + factor*(m_size.x()*m_slidingProgress),
				                         m_origin.y())
				                  + m_offset);
			} else {
				(*it)->setOrigin(   vec2(m_origin.x(),
				                         m_origin.y() + factor*(m_size.y()*m_slidingProgress))
				                  + m_offset);
			}
			(*it)->setSize(m_size);
			(*it)->onChangeSize();
		}
		it = m_subWidget.begin();
		std::advance(it, m_windowsDestination);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			if (*propertyTransitionMode == sladingTransitionHori) {
				(*it)->setOrigin(   vec2(m_origin.x() + factor*(m_size.x()*m_slidingProgress - m_size.x()),
				                         m_origin.y())
				                  + m_offset);
			} else {
				(*it)->setOrigin(   vec2(m_origin.x(),
				                         m_origin.y() + factor*(m_size.y()*m_slidingProgress - m_size.y()))
				                  + m_offset);
			}
			(*it)->setSize(m_size);
			(*it)->onChangeSize();
		}
	}
	markToRedraw();
}

void ewol::widget::WSlider::subWidgetSelectSetVectorId(int32_t _id) {
	if (_id<0) {
		EWOL_ERROR("Can not change to a widget not present : vectID=" << _id);
		return;
	}
	if (_id != m_windowsDestination) {
		m_windowsRequested = _id;
		signalStartSlide.emit();
		periodicCallEnable();
		markToRedraw();
	}
}

void ewol::widget::WSlider::subWidgetSelectSet(int32_t _id) {
	size_t elementID = 0;
	// search element in the list : 
	for (auto &it : m_subWidget) {
		elementID ++;
		if (it != nullptr) {
			if (it->getId() == _id) {
				if (it->propertyName.get() != "") {
					// change the internal event parameter (in case...) ==> no event generation
					propertySelectWidget.setDirect(it->propertyName.get());
				} else {
					propertySelectWidget.setDirect("");
				}
				break;
			}
		}
	}
	if (elementID < m_subWidget.size()) {
		subWidgetSelectSetVectorId(elementID);
	} else {
		subWidgetSelectSetVectorId(-1);
		// change the internal event parameter (in case...) ==> no event generation
		propertySelectWidget.setDirect("");
	}
}

void ewol::widget::WSlider::subWidgetSelectSet(const ewol::WidgetShared& _widgetPointer) {
	if (_widgetPointer == nullptr) {
		EWOL_ERROR("Can not change to a widget nullptr");
		return;
	}
	int32_t iii = 0;
	for (auto &it : m_subWidget) {
		if (    it != nullptr
		     && it == _widgetPointer) {
			subWidgetSelectSetVectorId(iii);
			if (_widgetPointer->propertyName.get() != "") {
				// change the internal event parameter (in case...) ==> no event generation
				propertySelectWidget.setDirect(_widgetPointer->propertyName.get());
			} else {
				propertySelectWidget.setDirect("");
			}
			return;
		}
		iii++;
	}
	EWOL_ERROR("Can not change to a widget not present");
}

void ewol::widget::WSlider::subWidgetSelectSet(const std::string& _widgetName) {
	if (_widgetName == "") {
		EWOL_ERROR("Can not change to a widget with no name (input)");
		return;
	}
	EWOL_VERBOSE("Select a new sub-widget to dosplay : '" << _widgetName << "'");
	int32_t iii = 0;
	for (auto &it : m_subWidget) {
		if (    it != nullptr
		     && it->propertyName.get() == _widgetName) {
			subWidgetSelectSetVectorId(iii);
			// change the internal event parameter (in case...) ==> no event generation
			propertySelectWidget.setDirect(_widgetName);
			return;
		}
		iii++;
	}
	EWOL_ERROR("Can not change to a widget not present");
}

void ewol::widget::WSlider::periodicCall(const ewol::event::Time& _event) {
	EWOL_VERBOSE("Periodic: " << m_slidingProgress << "/1.0 " << m_windowsSources << " ==> " << m_windowsDestination);
	if (m_slidingProgress >= 1.0) {
		m_windowsSources = m_windowsDestination;
		if(    m_windowsRequested != -1
		    && m_windowsRequested != m_windowsSources) {
			m_windowsDestination = m_windowsRequested;
			m_slidingProgress = 0.0;
		} else {
			// end of periodic :
			periodicCallDisable();
			signalStopSlide.emit();
		}
		m_windowsRequested = -1;
	}
	
	if (m_slidingProgress < 1.0) {
		if (    m_windowsRequested != -1
		     && m_slidingProgress<0.5 ) {
			// invert sources with destination
			int32_t tmppp = m_windowsDestination;
			m_windowsDestination = m_windowsSources;
			m_windowsSources = tmppp;
			m_slidingProgress = 1.0f - m_slidingProgress;
			if (m_windowsRequested == m_windowsDestination) {
				m_windowsRequested = -1;
			}
		}
		m_slidingProgress += _event.getDeltaCall()/propertyTransitionSpeed;
		m_slidingProgress = std::avg(0.0f, m_slidingProgress, 1.0f);
	}
	onChangeSize();
}

void ewol::widget::WSlider::systemDraw(const ewol::DrawProperty& _displayProp) {
	if (*propertyHide == true){
		// widget is hidden ...
		return;
	}
	// note : do not call the widget container  == > overload this one ...
	ewol::Widget::systemDraw(_displayProp);
	
	// subwidget draw
	ewol::DrawProperty prop = _displayProp;
	prop.limit(m_origin, m_size);
	
	if (m_windowsDestination == m_windowsSources) {
		//EWOL_DEBUG("Draw : " << m_windowsDestination);
		auto it = m_subWidget.begin();
		std::advance(it, m_windowsDestination);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			(*it)->systemDraw(prop);
		}
	} else {
		//EWOL_DEBUG("Draw : " << m_windowsSources << "=>" << m_windowsDestination << "progress=" << ((float)m_slidingProgress/1000.) );
		// draw Sources :
		auto it = m_subWidget.begin();
		std::advance(it, m_windowsSources);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			(*it)->systemDraw(prop);
		}
		// draw Destination : 
		it = m_subWidget.begin();
		std::advance(it, m_windowsDestination);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			(*it)->systemDraw(prop);
		}
	}
}

void ewol::widget::WSlider::onRegenerateDisplay() {
	if (m_windowsDestination == m_windowsSources) {
		auto it = m_subWidget.begin();
		std::advance(it, m_windowsDestination);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			(*it)->onRegenerateDisplay();
		}
	} else {
		auto it = m_subWidget.begin();
		std::advance(it, m_windowsSources);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			(*it)->onRegenerateDisplay();
		}
		it = m_subWidget.begin();
		std::advance(it, m_windowsDestination);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			(*it)->onRegenerateDisplay();
		}
	}
}

void ewol::widget::WSlider::onChangePropertySelectWidget() {
	if (propertySelectWidget.get() != "") {
		subWidgetSelectSet(*propertySelectWidget);
	}
}

void ewol::widget::WSlider::onChangePropertyTransitionMode() {
	markToRedraw();
}


ewol::WidgetShared ewol::widget::WSlider::getWidgetAtPos(const vec2& _pos) {
	if (*propertyHide == true) {
		return nullptr;
	}
	if (m_windowsDestination == m_windowsSources) {
		auto it = m_subWidget.begin();
		std::advance(it, m_windowsDestination);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			vec2 tmpSize = (*it)->getSize();
			vec2 tmpOrigin = (*it)->getOrigin();
			if(    (tmpOrigin.x() <= _pos.x() && tmpOrigin.x() + tmpSize.x() >= _pos.x())
			    && (tmpOrigin.y() <= _pos.y() && tmpOrigin.y() + tmpSize.y() >= _pos.y()) )
			{
				ewol::WidgetShared tmpWidget = (*it)->getWidgetAtPos(_pos);
				if (tmpWidget != nullptr) {
					return tmpWidget;
				}
				return nullptr;
			}
		}
	} else {
		auto it = m_subWidget.begin();
		std::advance(it, m_windowsDestination);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			vec2 tmpSize = (*it)->getSize();
			vec2 tmpOrigin = (*it)->getOrigin();
			if(    (tmpOrigin.x() <= _pos.x() && tmpOrigin.x() + tmpSize.x() >= _pos.x())
			    && (tmpOrigin.y() <= _pos.y() && tmpOrigin.y() + tmpSize.y() >= _pos.y()) )
			{
				ewol::WidgetShared tmpWidget = (*it)->getWidgetAtPos(_pos);
				if (tmpWidget != nullptr) {
					return tmpWidget;
				}
				return nullptr;
			}
		}
		it = m_subWidget.begin();
		std::advance(it, m_windowsSources);
		if (    it != m_subWidget.end()
		     && *it != nullptr) {
			vec2 tmpSize = (*it)->getSize();
			vec2 tmpOrigin = (*it)->getOrigin();
			if(    (tmpOrigin.x() <= _pos.x() && tmpOrigin.x() + tmpSize.x() >= _pos.x())
			    && (tmpOrigin.y() <= _pos.y() && tmpOrigin.y() + tmpSize.y() >= _pos.y()) )
			{
				ewol::WidgetShared tmpWidget = (*it)->getWidgetAtPos(_pos);
				if (tmpWidget != nullptr) {
					return tmpWidget;
				}
				return nullptr;
			}
		}
	}
	return nullptr;
}

